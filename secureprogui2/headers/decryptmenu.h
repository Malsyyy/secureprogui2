#pragma once

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
	private: System::Windows::Forms::Button^ encryptBtn;

		   System::Windows::Forms::Button^ logoutBtn;
		   System::Windows::Forms::Button^ exitBtn;
	private: System::Windows::Forms::RichTextBox^ fileBox;


		   System::Windows::Forms::OpenFileDialog^ openFileDialog;

#pragma region Windows Form Designer generated code
		   void InitializeComponent(void)
		   {
			   this->welcomeMsg = (gcnew System::Windows::Forms::Label());
			   this->browseBtn = (gcnew System::Windows::Forms::Button());
			   this->encryptBtn = (gcnew System::Windows::Forms::Button());
			   this->logoutBtn = (gcnew System::Windows::Forms::Button());
			   this->exitBtn = (gcnew System::Windows::Forms::Button());
			   this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			   this->fileBox = (gcnew System::Windows::Forms::RichTextBox());
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
			   // encryptBtn
			   // 
			   this->encryptBtn->Anchor = System::Windows::Forms::AnchorStyles::Top;
			   this->encryptBtn->Location = System::Drawing::Point(250, 275);
			   this->encryptBtn->Name = L"encryptBtn";
			   this->encryptBtn->Size = System::Drawing::Size(164, 49);
			   this->encryptBtn->TabIndex = 6;
			   this->encryptBtn->Text = L"Upload File";
			   // 
			   // logoutBtn
			   // 
			   this->logoutBtn->Anchor = System::Windows::Forms::AnchorStyles::Top;
			   this->logoutBtn->Location = System::Drawing::Point(169, 475);
			   this->logoutBtn->Name = L"logoutBtn";
			   this->logoutBtn->Size = System::Drawing::Size(169, 49);
			   this->logoutBtn->TabIndex = 7;
			   this->logoutBtn->Text = L"Logout";
			   // 
			   // exitBtn
			   // 
			   this->exitBtn->Anchor = System::Windows::Forms::AnchorStyles::Top;
			   this->exitBtn->Location = System::Drawing::Point(369, 475);
			   this->exitBtn->Name = L"exitBtn";
			   this->exitBtn->Size = System::Drawing::Size(169, 49);
			   this->exitBtn->TabIndex = 8;
			   this->exitBtn->Text = L"Exit";
			   // 
			   // fileBox
			   // 
			   this->fileBox->Location = System::Drawing::Point(169, 192);
			   this->fileBox->Name = L"fileBox";
			   this->fileBox->Size = System::Drawing::Size(297, 45);
			   this->fileBox->TabIndex = 9;
			   this->fileBox->Text = L"";
			   // 
			   // encryptMenu
			   // 
			   this->ClientSize = System::Drawing::Size(807, 589);
			   this->Controls->Add(this->fileBox);
			   this->Controls->Add(this->welcomeMsg);
			   this->Controls->Add(this->browseBtn);
			   this->Controls->Add(this->encryptBtn);
			   this->Controls->Add(this->logoutBtn);
			   this->Controls->Add(this->exitBtn);
			   this->Name = L"decryptMenu";
			   this->Text = L"Decrypt Menu";
			   this->ResumeLayout(false);

		   }
#pragma endregion

	private: System::Void browseBtn_Click(System::Object^ sender, System::EventArgs^ e) {
		openFileDialog->Title = "Select a file to upload";
		openFileDialog->Filter = "All Files (*.*)|*.*";

		if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			System::String^ filePath = openFileDialog->FileName;
			System::Windows::Forms::MessageBox::Show("Selected file: " + filePath);
		}
	}


	};
}
