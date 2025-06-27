// encryptMenu.h
#pragma once

#define OPENSSL_API_COMPAT 0x10100000L
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <fstream>
#include <vector>
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Windows::Forms;

namespace WindowsForm {

    public ref class encryptMenu : public Form {
    public:
        encryptMenu() {
            InitializeComponent();
        }

    protected:
        ~encryptMenu() {
            if (components) delete components;
        }

    private:
        System::ComponentModel::Container^ components;
        Button^ browseBtn;
        Button^ encryptBtn;
        RichTextBox^ fileBox;
        OpenFileDialog^ openFileDialog;
        String^ selectedFilePath;

#pragma region Designer Code
        void InitializeComponent(void) {
            browseBtn = gcnew Button();
            encryptBtn = gcnew Button();
            fileBox = gcnew RichTextBox();
            openFileDialog = gcnew OpenFileDialog();

            browseBtn->Text = L"Browse";
            browseBtn->Location = Drawing::Point(50, 30);
            browseBtn->Click += gcnew EventHandler(this, &encryptMenu::browseBtn_Click);

            encryptBtn->Text = L"Encrypt";
            encryptBtn->Location = Drawing::Point(150, 30);
            encryptBtn->Click += gcnew EventHandler(this, &encryptMenu::encryptBtn_Click);

            fileBox->Location = Drawing::Point(50, 80);
            fileBox->Size = Drawing::Size(300, 30);
            fileBox->ReadOnly = true;

            this->Controls->Add(browseBtn);
            this->Controls->Add(encryptBtn);
            this->Controls->Add(fileBox);
            this->ClientSize = Drawing::Size(400, 200);
            this->Text = L"Encrypt Menu";
        }
#pragma endregion

        System::Void browseBtn_Click(System::Object^, System::EventArgs^) {
            openFileDialog->Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
            if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                selectedFilePath = openFileDialog->FileName;
                fileBox->Text = selectedFilePath;
            }
        }

        System::Void encryptBtn_Click(System::Object^, System::EventArgs^) {
            if (String::IsNullOrEmpty(selectedFilePath)) {
                MessageBox::Show("Select a file first.");
                return;
            }

            msclr::interop::marshal_context context;
            std::string path = context.marshal_as<std::string>(selectedFilePath);
            std::ifstream in(path, std::ios::binary);
            if (!in) {
                MessageBox::Show("Failed to open file.");
                return;
            }

            std::vector<unsigned char> plain((std::istreambuf_iterator<char>(in)), {});
            in.close();

            unsigned char key[16] = { 'e','x','a','m','p','l','e','k','e','y','1','2','3','4','5','6' };
            unsigned char iv[AES_BLOCK_SIZE];

            // Generate a random IV
            if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
                MessageBox::Show("IV generation failed.");
                return;
            }

            // IMPORTANT: Make a copy of the original IV to save to file
            // AES_cfb128_encrypt modifies the IV in place, so we need the original for decryption
            unsigned char originalIvToSave[AES_BLOCK_SIZE];
            memcpy(originalIvToSave, iv, AES_BLOCK_SIZE);

            AES_KEY encKey;
            AES_set_encrypt_key(key, 128, &encKey);
            std::vector<unsigned char> cipher(plain.size());
            int num = 0; // This 'num' parameter tracks the offset within the current block, initialize to 0 for the start of the stream

            // Perform encryption; 'iv' will be modified by this call
            AES_cfb128_encrypt(plain.data(), cipher.data(), plain.size(), &encKey, iv, &num, AES_ENCRYPT);

            SaveFileDialog^ saveDialog = gcnew SaveFileDialog();
            saveDialog->Filter = "Encrypted Files (*.enc)|*.enc";
            if (saveDialog->ShowDialog() != System::Windows::Forms::DialogResult::OK) return;

            std::string savePath = context.marshal_as<std::string>(saveDialog->FileName);
            std::ofstream out(savePath, std::ios::binary);

            // Write the ORIGINAL IV to the file, followed by the ciphertext
            out.write(reinterpret_cast<char*>(originalIvToSave), AES_BLOCK_SIZE);
            out.write(reinterpret_cast<char*>(cipher.data()), cipher.size());
            out.close();

            MessageBox::Show("File encrypted successfully.");
        }
    };
}
