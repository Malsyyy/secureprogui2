// decryptMenu.h
#pragma once

#define OPENSSL_API_COMPAT 0x10100000L
#include <openssl/aes.h>
#include <fstream>
#include <vector>
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Windows::Forms;

namespace WindowsForm {

    public ref class decryptMenu : public Form {
    public:
        decryptMenu() {
            InitializeComponent();
        }

    protected:
        ~decryptMenu() {
            if (components) delete components;
        }

    private:
        System::ComponentModel::Container^ components;
        Button^ browseBtn;
        Button^ decryptBtn;
        RichTextBox^ outputBox; // Changed from fileBox to outputBox for clarity in decryption
        OpenFileDialog^ openFileDialog;
        String^ selectedFilePath;

#pragma region Designer Code
        void InitializeComponent(void) {
            browseBtn = gcnew Button();
            decryptBtn = gcnew Button();
            outputBox = gcnew RichTextBox();
            openFileDialog = gcnew OpenFileDialog();

            browseBtn->Text = L"Browse to Decrypt";
            browseBtn->Location = Drawing::Point(50, 30);
            browseBtn->Click += gcnew EventHandler(this, &decryptMenu::browseBtn_Click);

            decryptBtn->Text = L"Decrypt";
            decryptBtn->Location = Drawing::Point(200, 30);
            decryptBtn->Click += gcnew EventHandler(this, &decryptMenu::decryptBtn_Click);

            outputBox->Location = Drawing::Point(50, 80);
            outputBox->Size = Drawing::Size(300, 100);
            outputBox->ReadOnly = true; // Make the output box read-only

            this->Controls->Add(browseBtn);
            this->Controls->Add(decryptBtn);
            this->Controls->Add(outputBox);
            this->ClientSize = Drawing::Size(400, 220);
            this->Text = L"Decrypt Menu";
        }
#pragma endregion

        System::Void browseBtn_Click(System::Object^, System::EventArgs^) {
            openFileDialog->Filter = "Encrypted Files (*.enc)|*.enc";
            if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                selectedFilePath = openFileDialog->FileName;
                outputBox->Text = selectedFilePath; // Display selected file path in outputBox
            }
        }

        System::Void decryptBtn_Click(System::Object^, System::EventArgs^) {
            if (String::IsNullOrEmpty(selectedFilePath)) {
                MessageBox::Show("Please select a file first.");
                return;
            }

            msclr::interop::marshal_context context;
            std::string path = context.marshal_as<std::string>(selectedFilePath);
            std::ifstream in(path, std::ios::binary);
            if (!in) {
                MessageBox::Show("Failed to open file.");
                return;
            }

            // Read entire encrypted file content
            std::vector<unsigned char> enc((std::istreambuf_iterator<char>(in)), {});
            in.close();

            if (enc.size() < AES_BLOCK_SIZE) { // Check if file is even large enough for IV
                MessageBox::Show("File too small to contain IV and ciphertext.");
                return;
            }

            unsigned char key[16] = { 'e','x','a','m','p','l','e','k','e','y','1','2','3','4','5','6' };
            unsigned char iv[AES_BLOCK_SIZE];

            // Read the IV from the beginning of the encrypted file
            memcpy(iv, enc.data(), AES_BLOCK_SIZE);

            // Extract the ciphertext (data after the IV)
            std::vector<unsigned char> cipher(enc.begin() + AES_BLOCK_SIZE, enc.end());
            std::vector<unsigned char> plain(cipher.size());

            AES_KEY decKey;
            // For CFB mode, the encryption key schedule is used for both encryption and decryption
            AES_set_encrypt_key(key, 128, &decKey);
            int num = 0; // Initialize num to 0 for the start of the decryption stream

            // Perform decryption; 'iv' will be modified by this call (similar to encryption)
            AES_cfb128_encrypt(cipher.data(), plain.data(), cipher.size(), &decKey, iv, &num, AES_DECRYPT);

            SaveFileDialog^ saveDialog = gcnew SaveFileDialog();
            saveDialog->Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
            // Set default filename to original filename with a _decrypted suffix
            String^ originalFileName = System::IO::Path::GetFileNameWithoutExtension(selectedFilePath);
            saveDialog->FileName = originalFileName + "_decrypted.txt";

            if (saveDialog->ShowDialog() != System::Windows::Forms::DialogResult::OK) return;

            String^ saveFilePath = saveDialog->FileName;
            std::string savePath = context.marshal_as<std::string>(saveFilePath);
            std::ofstream out(savePath, std::ios::binary); // Use binary mode for consistency

            // Write the decrypted plaintext to the new file
            out.write(reinterpret_cast<char*>(plain.data()), plain.size());
            out.close();

            // Display a portion of the decrypted content in the output box (for text files)
            // Be careful if the decrypted data contains null characters or is very large.
            // For general binary files, this display might be misleading.
            if (!plain.empty()) {
                // Limit display to prevent UI overload if file is huge or binary garbage
                int displaySize = Math::Min(static_cast<int>(plain.size()), 1024); // Display max 1KB
                outputBox->Text = gcnew String(reinterpret_cast<char*>(plain.data()), 0, displaySize);
                if (plain.size() > 1024) {
                    outputBox->AppendText("\n\n(Content truncated for display)");
                }
            }
            else {
                outputBox->Text = "Decrypted file is empty.";
            }

            MessageBox::Show("Decryption complete and file saved.");
        }
    };
}
