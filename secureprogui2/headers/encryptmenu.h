// encryptMenu.h
#pragma once



#include <openssl/aes.h> // for AES encryption
#include <openssl/rand.h> // for random number generation
#include <fstream> // for file operations
#include <vector>  // for byte arrays
#include <string>  // for string operations
#include <msclr/marshal_cppstd.h> // for string conversion


using namespace System::IO; 

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing; 

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
        RichTextBox^ previewTextBox; 
        OpenFileDialog^ openFileDialog;
        String^ selectedFilePath;

#pragma region Designer Code

		// Initialize the components of the form

        void InitializeComponent(void) {
            browseBtn = gcnew Button();
            encryptBtn = gcnew Button();
            fileBox = gcnew RichTextBox();
            previewTextBox = gcnew RichTextBox(); 
            openFileDialog = gcnew OpenFileDialog();

            // Browse Button
			browseBtn->Text = L"Browse"; // Button to select a file
			browseBtn->Location = Point(50, 30); // Set location of the button
			browseBtn->Size = Drawing::Size(80, 30); // Set size of the button
			browseBtn->Click += gcnew EventHandler(this, &encryptMenu::browseBtn_Click); // Event handler for button click

            // Encrypt Button
			encryptBtn->Text = L"Encrypt"; // Button to trigger encryption
			encryptBtn->Location = Point(140, 30); // Set location of the button  
			encryptBtn->Size = Drawing::Size(80, 30); // Set size of the button
			encryptBtn->Click += gcnew EventHandler(this, &encryptMenu::encryptBtn_Click); // Event handler for button click

            // File Path Display Box
			fileBox->Location = Point(50, 80); // Set location of the file path display box
			fileBox->Size = Drawing::Size(300, 30); // Set size of the file path display box
			fileBox->ReadOnly = true; // Make the box read-only
			fileBox->Text = L"No file selected."; // Initial text in the box

            // New: Preview Text Box
			previewTextBox->Location = Point(50, 120); // Set location of the preview text box
			previewTextBox->Size = Drawing::Size(300, 150); // Set size of the preview text box
			previewTextBox->ReadOnly = true; // Make the box read-only
			previewTextBox->ScrollBars = RichTextBoxScrollBars::Vertical; // Enable vertical scroll bars
			previewTextBox->Text = L"File content preview will appear here."; // Text is shown in the preview box until file has been chosen by the user

            // Add controls to the form
			this->Controls->Add(browseBtn); // Add the browse button to the form
			this->Controls->Add(encryptBtn); // Add the encrypt button to the form
			this->Controls->Add(fileBox); // Add the file path display box to the form
			this->Controls->Add(previewTextBox); // Add the preview text box to the form
			this->ClientSize = Drawing::Size(400, 300); // Set the size of the form
            this->Text = L"Encrypt Menu"; // Set the title of the form
        }
#pragma endregion

        System::Void browseBtn_Click(System::Object^, System::EventArgs^) {
            openFileDialog->Filter = "Text Files (*.txt)|*.txt|CSV Files (*.csv)|*.csv| JSON Files (*.json)|*.json";
            if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                selectedFilePath = openFileDialog->FileName;
                fileBox->Text = selectedFilePath;


                try {

                    String^ fileContent = File::ReadAllText(selectedFilePath); 


                    int previewLength = Math::Min(fileContent->Length, 5000); 
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
            String^ originalFileName = Path::GetFileNameWithoutExtension(selectedFilePath); 
            saveDialog->FileName = originalFileName + "_encrypted.enc";

            if (saveDialog->ShowDialog() != System::Windows::Forms::DialogResult::OK) return;

            std::string savePath = context.marshal_as<std::string>(saveDialog->FileName);
            std::ofstream out(savePath, std::ios::binary);

            out.write(reinterpret_cast<char*>(originalIvToSave), AES_BLOCK_SIZE);
            out.write(reinterpret_cast<char*>(cipher.data()), cipher.size());
            out.close();

            MessageBox::Show("File encrypted successfully.");
            previewTextBox->Text = L"Encryption complete. Preview cleared."; 
            fileBox->Text = L"No file selected.";
            selectedFilePath = nullptr; 
        }
    };
}
