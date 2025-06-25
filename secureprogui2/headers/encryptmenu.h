// encryptMenu.h
#pragma once

// Removed: constexpr long long OPENSSL_API_COMPAT_VALUE = 0x10100000L;
// Removed: #define OPENSSL_API_COMPAT OPENSSL_API_COMPAT_VALUE
// This macro often conflicts with the OpenSSL library's own version checks
// and is generally not needed for basic AES operations.

#include <openssl/aes.h>
#include <openssl/rand.h>
#include <fstream>
#include <vector>
#include <string> // For std::string (if used in C++ part)
#include <msclr/marshal_cppstd.h>

// Correct way to use .NET types in C++/CLI:
// Instead of #include <System.IO.File.h>,
// use 'using namespace System::IO;' to bring the namespace into scope.
using namespace System::IO; // For File and Path classes

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing; // For Point, Size

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
        RichTextBox^ previewTextBox; // New: To preview file content
        OpenFileDialog^ openFileDialog;
        String^ selectedFilePath;

#pragma region Designer Code
        void InitializeComponent(void) {
            browseBtn = gcnew Button();
            encryptBtn = gcnew Button();
            fileBox = gcnew RichTextBox();
            previewTextBox = gcnew RichTextBox(); // Initialize new preview text box
            openFileDialog = gcnew OpenFileDialog();

            // Browse Button
            browseBtn->Text = L"Browse";
            browseBtn->Location = Point(50, 30);
            browseBtn->Size = Drawing::Size(80, 30); // Added size for consistency
            browseBtn->Click += gcnew EventHandler(this, &encryptMenu::browseBtn_Click);

            // Encrypt Button
            encryptBtn->Text = L"Encrypt";
            encryptBtn->Location = Point(140, 30); // Adjusted location
            encryptBtn->Size = Drawing::Size(80, 30); // Added size for consistency
            encryptBtn->Click += gcnew EventHandler(this, &encryptMenu::encryptBtn_Click);

            // File Path Display Box
            fileBox->Location = Point(50, 80);
            fileBox->Size = Drawing::Size(300, 30);
            fileBox->ReadOnly = true;
            fileBox->Text = L"No file selected."; // Default text

            // New: Preview Text Box
            previewTextBox->Location = Point(50, 120); // Position below fileBox
            previewTextBox->Size = Drawing::Size(300, 150); // Larger size for content preview
            previewTextBox->ReadOnly = true;
            previewTextBox->ScrollBars = RichTextBoxScrollBars::Vertical; // Add scrollbars
            previewTextBox->Text = L"File content preview will appear here.";

            // Add controls to the form
            this->Controls->Add(browseBtn);
            this->Controls->Add(encryptBtn);
            this->Controls->Add(fileBox);
            this->Controls->Add(previewTextBox); // Add new control
            this->ClientSize = Drawing::Size(400, 300); // Adjusted form size
            this->Text = L"Encrypt Menu";
        }
#pragma endregion

        System::Void browseBtn_Click(System::Object^, System::EventArgs^) {
            openFileDialog->Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
            if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                selectedFilePath = openFileDialog->FileName;
                fileBox->Text = selectedFilePath;

                // Read and display file content in previewTextBox
                try {
                    // Using ReadAllText for simplicity for text files.
                    // For very large files, consider reading in chunks or showing a warning.
                    // If files might be binary, displaying them directly as text can show garbage.
                    String^ fileContent = File::ReadAllText(selectedFilePath); // Use File from System::IO

                    // Limit the preview size to avoid UI slowdowns for huge files
                    int previewLength = Math::Min(fileContent->Length, 5000); // Max 5000 characters
                    previewTextBox->Text = fileContent->Substring(0, previewLength);
                    if (fileContent->Length > previewLength) {
                        previewTextBox->AppendText(L"\n\n--- Content truncated ---");
                    }

                }
                catch (Exception^ ex) {
                    previewTextBox->Text = L"Error reading file content: " + ex->Message;
                }
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

            if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
                MessageBox::Show("IV generation failed.");
                return;
            }

            unsigned char originalIvToSave[AES_BLOCK_SIZE];
            memcpy(originalIvToSave, iv, AES_BLOCK_SIZE);

            AES_KEY encKey;
            AES_set_encrypt_key(key, 128, &encKey);
            std::vector<unsigned char> cipher(plain.size());
            int num = 0;

            AES_cfb128_encrypt(plain.data(), cipher.data(), plain.size(), &encKey, iv, &num, AES_ENCRYPT);

            SaveFileDialog^ saveDialog = gcnew SaveFileDialog();
            saveDialog->Filter = "Encrypted Files (*.enc)|*.enc";
            // Set default filename to original filename with a _enc suffix
            String^ originalFileName = Path::GetFileNameWithoutExtension(selectedFilePath); // Use Path from System::IO
            saveDialog->FileName = originalFileName + "_encrypted.enc";

            if (saveDialog->ShowDialog() != System::Windows::Forms::DialogResult::OK) return;

            std::string savePath = context.marshal_as<std::string>(saveDialog->FileName);
            std::ofstream out(savePath, std::ios::binary);

            out.write(reinterpret_cast<char*>(originalIvToSave), AES_BLOCK_SIZE);
            out.write(reinterpret_cast<char*>(cipher.data()), cipher.size());
            out.close();

            MessageBox::Show("File encrypted successfully.");
            previewTextBox->Text = L"Encryption complete. Preview cleared."; // Clear preview after encryption
            fileBox->Text = L"No file selected."; // Clear selected file path
            selectedFilePath = nullptr; // Reset selected file path
        }
    };
}
