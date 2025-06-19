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
        encryptMenu(void) {
            InitializeComponent();
        }

    protected:
        ~encryptMenu() {
            if (components) {
                delete components;
            }
        }

    private:
        System::ComponentModel::Container^ components;
        Label^ welcomeMsg;
        Button^ browseBtn;
        Button^ encryptBtn;
        Button^ logoutBtn;
        Button^ exitBtn;
        RichTextBox^ fileBox;
        RichTextBox^ fileContent;
        OpenFileDialog^ openFileDialog;

#pragma region Windows Form Designer generated code  
        void InitializeComponent(void) {
            this->welcomeMsg = gcnew Label();
            this->browseBtn = gcnew Button();
            this->encryptBtn = gcnew Button();
            this->logoutBtn = gcnew Button();
            this->exitBtn = gcnew Button();
            this->fileBox = gcnew RichTextBox();
            this->fileContent = gcnew RichTextBox();
            this->openFileDialog = gcnew OpenFileDialog();
            this->SuspendLayout();
            //
            // welcome msg
            //
            this->welcomeMsg->Location = Drawing::Point(58, 220);
            this->welcomeMsg->Name = L"welcomeMsg";
            this->welcomeMsg->Size = Drawing::Size(0, 100);
            this->welcomeMsg->TabIndex = 4;
            //
            // browseBtn
            //
            this->browseBtn->Location = Drawing::Point(472, 188);
            this->browseBtn->Name = L"browseBtn";
            this->browseBtn->Size = Drawing::Size(164, 49);
            this->browseBtn->Text = L"Browse File";
            this->browseBtn->Click += gcnew EventHandler(this, &encryptMenu::browseBtn_Click);
            //
            // encryptBtn
            //
            this->encryptBtn->Location = Drawing::Point(250, 375);
            this->encryptBtn->Name = L"encryptBtn";
            this->encryptBtn->Size = Drawing::Size(164, 49);
            this->encryptBtn->Text = L"Encrypt";
            this->encryptBtn->Click += gcnew EventHandler(this, &encryptMenu::encryptBtn_Click);
            //
            // exitBtn
            //
            this->exitBtn->Location = Drawing::Point(250, 475);
            this->exitBtn->Name = L"exitBtn";
            this->exitBtn->Size = Drawing::Size(169, 49);
            this->exitBtn->Text = L"Exit";
			this->exitBtn->Click += gcnew EventHandler(this, &encryptMenu::exitBtn_Click);
            //
            // fileBox
            //
            this->fileBox->Location = Drawing::Point(169, 192);
            this->fileBox->Size = Drawing::Size(297, 45);
            this->fileBox->ReadOnly = true;
            //
            // fileContent
            //
            this->fileContent->Location = Drawing::Point(169, 243);
            this->fileContent->Size = Drawing::Size(297, 100);
            this->fileContent->ReadOnly = true;
            //
            // Form
            //
            this->ClientSize = Drawing::Size(807, 589);
            this->Controls->Add(this->fileBox);
            this->Controls->Add(this->fileContent);
            this->Controls->Add(this->welcomeMsg);
            this->Controls->Add(this->browseBtn);
            this->Controls->Add(this->encryptBtn);
            this->Controls->Add(this->exitBtn);
            this->Name = L"encryptMenu";
            this->Text = L"Encrypt Menu";
            this->ResumeLayout(false);
        }
#pragma endregion

    private:
        void SetFileBoxPath(String^ filePath) {
            fileBox->Text = filePath;
            Clipboard::SetText(filePath);
        }

        void ShowSelectedFileContent(String^ filePath) {
            try {
                std::ifstream inputFile(msclr::interop::marshal_as<std::string>(filePath), std::ios::binary);
                if (!inputFile) {
                    fileContent->Text = "Failed to open file.";
                    return;
                }
                std::string content((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
                inputFile.close();
                fileContent->Text = gcnew String(content.c_str());
            }
            catch (...) {
                fileContent->Text = "Error reading file.";
            }
        }
            
        bool SaveEncryptedFileToUserLocation(const std::vector<unsigned char>& encryptedData) {
            SaveFileDialog^ saveFileDialog = gcnew SaveFileDialog();
            saveFileDialog->Title = "Save Encrypted File As";
            saveFileDialog->Filter = "Encrypted Files (*.enc)|*.enc|All Files (*.*)|*.*";
            saveFileDialog->DefaultExt = "enc";
            if (saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                std::string savePath = msclr::interop::marshal_as<std::string>(saveFileDialog->FileName);
                std::ofstream outputFile(savePath, std::ios::binary);
                if (!outputFile) {
                    MessageBox::Show("Failed to save file.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
                    return false;
                }
                outputFile.write(reinterpret_cast<const char*>(encryptedData.data()), encryptedData.size());
                outputFile.close();
                return true;
            }
            return false;
        }

        System::Void browseBtn_Click(System::Object^ sender, System::EventArgs^ e) {
            openFileDialog->Title = "Select a file to upload";
            openFileDialog->Filter = "Text Files (*.txt)|*.txt|JSON Files (*.json)|*.json|CSV Files (*.csv)|*.csv|All Files (*.*)|*.*";

            if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                String^ filePath = openFileDialog->FileName;
                SetFileBoxPath(filePath);
                ShowSelectedFileContent(filePath);
            }
        }

        System::Void encryptBtn_Click(System::Object^ sender, System::EventArgs^ e) {
            String^ filePath = fileBox->Text;
            if (String::IsNullOrEmpty(filePath)) {
                MessageBox::Show("Please select a file.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
                return;
            }

            std::ifstream inputFile(msclr::interop::marshal_as<std::string>(filePath), std::ios::binary);
            if (!inputFile) {
                MessageBox::Show("Could not open file.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
                return;
            }

            std::vector<unsigned char> fileData((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
            inputFile.close();

            unsigned char key[16];
            memcpy(key, "examplekey123456", 16);

            unsigned char iv[AES_BLOCK_SIZE];
            if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
                MessageBox::Show("IV generation failed.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
                return;
            }

            AES_KEY encryptKey;
            AES_set_encrypt_key(key, 128, &encryptKey);

            std::vector<unsigned char> encryptedData(fileData.size());
            int num = 0;
            AES_cfb128_encrypt(fileData.data(), encryptedData.data(), fileData.size(), &encryptKey, iv, &num, AES_ENCRYPT);

            if (SaveEncryptedFileToUserLocation(encryptedData)) {
                MessageBox::Show("File encrypted and saved!", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }


        }

	private: System::Void exitBtn_Click(System::Object^ sender, System::EventArgs^ e) {
		this->Close();
	}

    };
}
