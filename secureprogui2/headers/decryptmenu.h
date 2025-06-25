// decryptMenu.h
#pragma once

// Removed explicit definition of OPENSSL_API_COMPAT.
// This macro is usually handled by the OpenSSL library itself and
// manually defining it can lead to conflicts and errors.

#include <openssl/aes.h>
// #include <openssl/rand.h> // Not needed in decryptMenu
#include <fstream>
#include <vector>
#include <string> // For std::string (if used in C++ part)
#include <msclr/marshal_cppstd.h>

// Correct way to use .NET types in C++/CLI:
// Instead of #include <System.IO.Path.h> (which is not a valid include),
// use 'using namespace System::IO;' to bring the namespace into scope.
using namespace System::IO; // For File and Path classes (e.g., Path::GetFileNameWithoutExtension)

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing; // For Point, Size, etc.

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
        RichTextBox^ selectedFilePathDisplayBox; // Renamed for clarity: displays selected .enc file path
        RichTextBox^ decryptedPreviewBox;      // New: To preview decrypted text
        OpenFileDialog^ openFileDialog;
        String^ selectedFilePath;

#pragma region Designer Code
        void InitializeComponent(void) {
            browseBtn = gcnew Button();
            decryptBtn = gcnew Button();
            selectedFilePathDisplayBox = gcnew RichTextBox(); // Renamed component
            decryptedPreviewBox = gcnew RichTextBox();      // Initialize new preview text box
            openFileDialog = gcnew OpenFileDialog();

            // Browse Button
            browseBtn->Text = L"Browse Encrypted File"; // More descriptive text
            browseBtn->Location = Point(50, 30);
            browseBtn->Size = Drawing::Size(150, 30); // Adjusted size
            browseBtn->Click += gcnew EventHandler(this, &decryptMenu::browseBtn_Click);

            // Decrypt Button
            decryptBtn->Text = L"Decrypt";
            decryptBtn->Location = Point(210, 30); // Adjusted location
            decryptBtn->Size = Drawing::Size(80, 30); // Added size for consistency
            decryptBtn->Click += gcnew EventHandler(this, &decryptMenu::decryptBtn_Click);

            // Selected File Path Display Box
            selectedFilePathDisplayBox->Location = Point(50, 80);
            selectedFilePathDisplayBox->Size = Drawing::Size(300, 30);
            selectedFilePathDisplayBox->ReadOnly = true;
            selectedFilePathDisplayBox->Text = L"No encrypted file selected."; // Default text

            // New: Decrypted Content Preview Box
            decryptedPreviewBox->Location = Point(50, 120); // Position below path box
            decryptedPreviewBox->Size = Drawing::Size(300, 150); // Larger size for content preview
            decryptedPreviewBox->ReadOnly = true;
            decryptedPreviewBox->ScrollBars = RichTextBoxScrollBars::Vertical; // Add scrollbars
            decryptedPreviewBox->Text = L"Decrypted content preview will appear here.";

            // Add controls to the form
            this->Controls->Add(browseBtn);
            this->Controls->Add(decryptBtn);
            this->Controls->Add(selectedFilePathDisplayBox); // Add renamed control
            this->Controls->Add(decryptedPreviewBox);      // Add new control
            this->ClientSize = Drawing::Size(400, 300); // Adjusted form size
            this->Text = L"Decrypt Menu";
        }
#pragma endregion

        System::Void browseBtn_Click(System::Object^, System::EventArgs^) {
            openFileDialog->Filter = "Encrypted Files (*.enc)|*.enc";
            if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                selectedFilePath = openFileDialog->FileName;
                selectedFilePathDisplayBox->Text = selectedFilePath; // Display selected file path
                decryptedPreviewBox->Text = L"Click 'Decrypt' to see content."; // Reset preview
            }
        }

        System::Void decryptBtn_Click(System::Object^, System::EventArgs^) {
            if (String::IsNullOrEmpty(selectedFilePath)) {
                MessageBox::Show("Please select an encrypted file first.");
                return;
            }

            msclr::interop::marshal_context context;
            std::string path = context.marshal_as<std::string>(selectedFilePath);
            std::ifstream in(path, std::ios::binary);
            if (!in) {
                MessageBox::Show("Failed to open file.");
                return;
            }

            std::vector<unsigned char> enc((std::istreambuf_iterator<char>(in)), {});
            in.close();

            if (enc.size() < AES_BLOCK_SIZE) {
                MessageBox::Show("File too small to contain IV and ciphertext.");
                decryptedPreviewBox->Text = L"Error: Encrypted file is too small.";
                return;
            }

            unsigned char key[16] = { 'e','x','a','m','p','l','e','k','e','y','1','2','3','4','5','6' };
            unsigned char iv[AES_BLOCK_SIZE];

            memcpy(iv, enc.data(), AES_BLOCK_SIZE);

            std::vector<unsigned char> cipher(enc.begin() + AES_BLOCK_SIZE, enc.end());
            std::vector<unsigned char> plain(cipher.size());

            AES_KEY decKey;
            AES_set_encrypt_key(key, 128, &decKey);
            int num = 0;

            AES_cfb128_encrypt(cipher.data(), plain.data(), cipher.size(), &decKey, iv, &num, AES_DECRYPT);

            SaveFileDialog^ saveDialog = gcnew SaveFileDialog();
            saveDialog->Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
            String^ originalFileName = Path::GetFileNameWithoutExtension(selectedFilePath); // Use Path from System::IO
            saveDialog->FileName = originalFileName + "_decrypted.txt";

            if (saveDialog->ShowDialog() != System::Windows::Forms::DialogResult::OK) {
                decryptedPreviewBox->Text = L"Decryption cancelled by user (file not saved).";
                return;
            }

            String^ saveFilePath = saveDialog->FileName;
            std::string savePath = context.marshal_as<std::string>(saveFilePath);
            std::ofstream out(savePath, std::ios::binary);

            out.write(reinterpret_cast<char*>(plain.data()), plain.size());
            out.close();

            // Display decrypted content in the new preview box
            if (!plain.empty()) {
                // Assuming decrypted content is text. Handle encoding if necessary.
                // Using a try-catch for the String conversion as binary data might contain nulls.
                try {
                    String^ decryptedText = gcnew String(reinterpret_cast<char*>(plain.data()), 0, static_cast<int>(plain.size()));

                    // Limit display to prevent UI overload if file is huge
                    int previewLength = Math::Min(decryptedText->Length, 5000); // Max 5000 characters
                    decryptedPreviewBox->Text = decryptedText->Substring(0, previewLength);
                    if (decryptedText->Length > previewLength) {
                        decryptedPreviewBox->AppendText(L"\n\n--- Content truncated ---");
                    }
                }
                catch (Exception^ ex) {
                    decryptedPreviewBox->Text = L"Error displaying decrypted text (possibly binary data): " + ex->Message;
                }
            }
            else {
                decryptedPreviewBox->Text = L"Decrypted file is empty.";
            }

            MessageBox::Show("Decryption complete and file saved.");
            selectedFilePathDisplayBox->Text = L"No encrypted file selected."; // Clear path
            selectedFilePath = nullptr; // Reset selected file path
        }
    };
}
