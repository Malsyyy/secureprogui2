
#pragma once

#include <openssl/aes.h>
#include <fstream>
#include <vector>
#include <string> 
#include <msclr/marshal_cppstd.h>



using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing; 

namespace WindowsForm {

    public ref class decryptMenu : public Form {
    public:

		// Constructor to initialize the form and its components

        decryptMenu() {
            InitializeComponent();
        }

    protected:

		// Destructor to clean up resources

        ~decryptMenu() {
            if (components) delete components;
        }

    private:

		// Declare UI components

        System::ComponentModel::Container^ components;
        Button^ browseBtn;
        Button^ decryptBtn;
        RichTextBox^ selectedFilePathDisplayBox; 
        RichTextBox^ decryptedPreviewBox;      
        OpenFileDialog^ openFileDialog;
        String^ selectedFilePath;

#pragma region Designer Code
        void InitializeComponent(void) {
            browseBtn = gcnew Button();
            decryptBtn = gcnew Button();
            selectedFilePathDisplayBox = gcnew RichTextBox(); 
            decryptedPreviewBox = gcnew RichTextBox();     
            openFileDialog = gcnew OpenFileDialog();

            // Browse Button
			browseBtn->Text = L"Browse Encrypted File"; // Button to select an encrypted file
			browseBtn->Location = Point(50, 30); // Set location of the button
			browseBtn->Size = Drawing::Size(150, 30); // Set size of the button
			browseBtn->Click += gcnew EventHandler(this, &decryptMenu::browseBtn_Click); // Event handler for button click 

            // Decrypt Button
			decryptBtn->Text = L"Decrypt"; // Button to trigger decryption
			decryptBtn->Location = Point(210, 30);  // Set location of the button
			decryptBtn->Size = Drawing::Size(80, 30); // Set size of the button
			decryptBtn->Click += gcnew EventHandler(this, &decryptMenu::decryptBtn_Click); // Event handler for button click

            // Selected File Path Display Box
			selectedFilePathDisplayBox->Location = Point(50, 80); // Set location of the file path display box
			selectedFilePathDisplayBox->Size = Drawing::Size(300, 30); // Set size of the file path display box
			selectedFilePathDisplayBox->ReadOnly = true; // Make the box read-only
			selectedFilePathDisplayBox->Text = L"No encrypted file selected."; // Initial text in the box

            // New: Decrypted Content Preview Box
			decryptedPreviewBox->Location = Point(50, 120); // Set location of the decrypted content preview box
			decryptedPreviewBox->Size = Drawing::Size(300, 150); // Set size of the decrypted content preview box
			decryptedPreviewBox->ReadOnly = true; // Make the box read-only
			decryptedPreviewBox->ScrollBars = RichTextBoxScrollBars::Vertical; // Enable vertical scroll bars 
			decryptedPreviewBox->Text = L"Decrypted content preview will appear here."; // Text is shown in the preview box until file has been chosen by the user

            // Add controls to the form
			this->Controls->Add(browseBtn); // Add the browse button to the form
			this->Controls->Add(decryptBtn); // Add the decrypt button to the form
			this->Controls->Add(selectedFilePathDisplayBox); // Add the selected file path display box to the form
			this->Controls->Add(decryptedPreviewBox);     // Add the decrypted content preview box to the form
			this->ClientSize = Drawing::Size(400, 300); // Set the size of the form
            this->Text = L"Decrypt Menu";
        }
#pragma endregion

		// Browse Button Click Event Handler

        System::Void browseBtn_Click(System::Object^, System::EventArgs^) {
            openFileDialog->Filter = "Encrypted Files (*.enc)|*.enc";
            if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                selectedFilePath = openFileDialog->FileName;
                selectedFilePathDisplayBox->Text = selectedFilePath; 
                decryptedPreviewBox->Text = L"Click 'Decrypt' to see content."; 
            }
        }

		// Decrypt Button Click Event Handler

        System::Void decryptBtn_Click(System::Object^, System::EventArgs^) {
            if (String::IsNullOrEmpty(selectedFilePath)) {
                MessageBox::Show("Please select an encrypted file first.");
                return;
            }

			// Convert the selected file path to a standard string format for file operations

            msclr::interop::marshal_context context;
            std::string path = context.marshal_as<std::string>(selectedFilePath);
            std::ifstream in(path, std::ios::binary);
            if (!in) {
                MessageBox::Show("Failed to open file.");
                return;
            }

            std::vector<unsigned char> enc((std::istreambuf_iterator<char>(in)), {});
            in.close();


			// Check if the file is large enough to contain IV and ciphertext

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
            String^ originalFileName = Path::GetFileNameWithoutExtension(selectedFilePath); 
            saveDialog->FileName = originalFileName + "_decrypted.txt";

			// Show save dialog to user

            if (saveDialog->ShowDialog() != System::Windows::Forms::DialogResult::OK) {
                decryptedPreviewBox->Text = L"Decryption cancelled by user (file not saved).";
                return;
            }

            String^ saveFilePath = saveDialog->FileName;
            std::string savePath = context.marshal_as<std::string>(saveFilePath);
            std::ofstream out(savePath, std::ios::binary);

            out.write(reinterpret_cast<char*>(plain.data()), plain.size());
            out.close();


            if (!plain.empty()) {

				// Attempt to display decrypted content in the preview box

                try {
                    String^ decryptedText = gcnew String(reinterpret_cast<char*>(plain.data()), 0, static_cast<int>(plain.size()));

                    int previewLength = Math::Min(decryptedText->Length, 5000); 
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
            selectedFilePathDisplayBox->Text = L"No encrypted file selected."; 
            selectedFilePath = nullptr; 
        }
    };
}
