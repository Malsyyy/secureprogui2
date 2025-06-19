#pragma once  

#include <msclr/marshal_cppstd.h>
#include <fstream>
#include <sstream>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <vector>

namespace WindowsForm {

    public ref class decryptMenu : public System::Windows::Forms::Form
    {
    public:
        decryptMenu(void)
        {
            InitializeComponent();
        }

    protected:
        ~decryptMenu()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        System::ComponentModel::Container^ components;
        System::Windows::Forms::Label^ welcomeMsg;
        System::Windows::Forms::Button^ browseBtn;
        System::Windows::Forms::Button^ decryptBtn;
        System::Windows::Forms::Button^ logoutBtn;
        System::Windows::Forms::Button^ exitBtn;
        System::Windows::Forms::RichTextBox^ fileBox;
        System::Windows::Forms::RichTextBox^ fileContent;
        System::Windows::Forms::OpenFileDialog^ openFileDialog;
        System::Windows::Forms::SaveFileDialog^ saveFileDialog;
        System::String^ selectedFilePath;

#pragma region Windows Form Designer generated code  
        void InitializeComponent(void)
        {
            this->welcomeMsg = (gcnew System::Windows::Forms::Label());
            this->browseBtn = (gcnew System::Windows::Forms::Button());
            this->decryptBtn = (gcnew System::Windows::Forms::Button());
            this->logoutBtn = (gcnew System::Windows::Forms::Button());
            this->exitBtn = (gcnew System::Windows::Forms::Button());
            this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
            this->saveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
            this->fileBox = (gcnew System::Windows::Forms::RichTextBox());
            this->fileContent = (gcnew System::Windows::Forms::RichTextBox());
            this->SuspendLayout();
            //  
            // welcomeMsg  
            //  
            this->welcomeMsg->Location = System::Drawing::Point(58, 220);
            this->welcomeMsg->Name = L"welcomeMsg";
            this->welcomeMsg->Size = System::Drawing::Size(0, 100);
            this->welcomeMsg->TabIndex = 4;
            //  
            // browseBtn  
            //  
            this->browseBtn->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->browseBtn->Location = System::Drawing::Point(472, 188);
            this->browseBtn->Name = L"browseBtn";
            this->browseBtn->Size = System::Drawing::Size(164, 49);
            this->browseBtn->TabIndex = 5;
            this->browseBtn->Text = L"Browse For A file:";
            this->browseBtn->Click += gcnew System::EventHandler(this, &decryptMenu::browseBtn_Click);
            //  
            // decryptBtn  
            //  
            this->decryptBtn->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->decryptBtn->Location = System::Drawing::Point(250, 375);
            this->decryptBtn->Name = L"decryptBtn";
            this->decryptBtn->Size = System::Drawing::Size(164, 49);
            this->decryptBtn->TabIndex = 6;
            this->decryptBtn->Text = L"Decrypt File";
            this->decryptBtn->Click += gcnew System::EventHandler(this, &decryptMenu::decryptBtn_Click);
            //  
            // exitBtn  
            //  
            this->exitBtn->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->exitBtn->Location = System::Drawing::Point(250, 475);
            this->exitBtn->Name = L"exitBtn";
            this->exitBtn->Size = System::Drawing::Size(169, 49);
            this->exitBtn->TabIndex = 8;
            this->exitBtn->Text = L"Exit";
            this->exitBtn->Click += gcnew System::EventHandler(this, &decryptMenu::exitBtn_Click);
            //  
            // fileBox  
            //  
            this->fileBox->Location = System::Drawing::Point(169, 192);
            this->fileBox->Name = L"fileBox";
            this->fileBox->Size = System::Drawing::Size(297, 45);
            this->fileBox->TabIndex = 9;
            this->fileBox->Text = L"";
            //  
            // fileContent  
            //  
            this->fileContent->Location = System::Drawing::Point(169, 243);
            this->fileContent->Name = L"fileContent";
            this->fileContent->Size = System::Drawing::Size(297, 100);
            this->fileContent->TabIndex = 10;
            this->fileContent->Text = L"";
            this->fileContent->ReadOnly = true;
            //  
            // decryptMenu  
            //  
            this->ClientSize = System::Drawing::Size(807, 589);
            this->Controls->Add(this->fileBox);
            this->Controls->Add(this->fileContent);
            this->Controls->Add(this->welcomeMsg);
            this->Controls->Add(this->browseBtn);
            this->Controls->Add(this->decryptBtn);
            this->Controls->Add(this->logoutBtn);
            this->Controls->Add(this->exitBtn);
            this->Name = L"decryptMenu";
            this->Text = L"Decrypt Menu";
            this->ResumeLayout(false);
        }
#pragma endregion  

    private: System::Void browseBtn_Click(System::Object^ sender, System::EventArgs^ e) {
        openFileDialog->Title = "Select a file to upload";
        openFileDialog->Filter = "Enc Files (*.enc)|*.enc";

        if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            selectedFilePath = openFileDialog->FileName;
            fileBox->Text = selectedFilePath;

            try {
                msclr::interop::marshal_context context;
                std::string nativePath = context.marshal_as<std::string>(selectedFilePath);
                std::ifstream inFile(nativePath, std::ios::binary);
                std::stringstream buffer;
                buffer << inFile.rdbuf();
                std::string encContent = buffer.str();
                fileContent->Text = gcnew System::String(encContent.c_str());
            }
            catch (...) {
                System::Windows::Forms::MessageBox::Show("Failed to read file content.");
            }
        }
    }

    private: System::Void decryptBtn_Click(System::Object^ sender, System::EventArgs^ e) {
        if (System::String::IsNullOrEmpty(selectedFilePath)) {
            System::Windows::Forms::MessageBox::Show("Please select a file first.");
            return;
        }

        saveFileDialog->Title = "Save Decrypted File As";
        saveFileDialog->Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
        if (saveFileDialog->ShowDialog() != System::Windows::Forms::DialogResult::OK) {
            return;
        }
        System::String^ savePath = saveFileDialog->FileName;

        unsigned char key[32] = { 0 };
        unsigned char iv[16] = { 0 };

        msclr::interop::marshal_context context;
        std::string nativePath = context.marshal_as<std::string>(selectedFilePath);

        std::ifstream inFile(nativePath, std::ios::binary);
        std::vector<unsigned char> encData;
        if (inFile) {
            inFile.seekg(0, std::ios::end);
            std::streamsize size = inFile.tellg();
            inFile.seekg(0, std::ios::beg);
            if (size > 0) {
                encData.resize(static_cast<size_t>(size));
                inFile.read(reinterpret_cast<char*>(encData.data()), size);
            }
        }
        inFile.close();

        std::vector<unsigned char> decData(encData.size() + AES_BLOCK_SIZE);
        int outLen1 = 0, outLen2 = 0;

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
        EVP_DecryptUpdate(ctx, decData.data(), &outLen1, encData.data(), static_cast<int>(encData.size()));
        EVP_DecryptFinal_ex(ctx, decData.data() + outLen1, &outLen2);
        EVP_CIPHER_CTX_free(ctx);

        std::string nativeSavePath = context.marshal_as<std::string>(savePath);
        std::ofstream outFile(nativeSavePath, std::ios::binary);
        outFile.write(reinterpret_cast<char*>(decData.data()), outLen1 + outLen2);
        outFile.close();

        System::Windows::Forms::MessageBox::Show("File decrypted and saved successfully.");
    }

    private: System::Void exitBtn_Click(System::Object^ sender, System::EventArgs^ e) {
        this->Close();
    }
    };
}
