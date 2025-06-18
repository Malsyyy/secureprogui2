#pragma once

using namespace System::Windows::Forms;
using namespace System;

namespace WindowsForm {

	public ref class encryptMenu : public Form
	{
	public:
		encryptMenu(void)
		{
			InitializeComponent();
		}

	protected:
		~encryptMenu()
		{
			if (components)
			{
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
		OpenFileDialog^ openFileDialog;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->welcomeMsg = gcnew Label();
			this->browseBtn = gcnew Button();
			this->encryptBtn = gcnew Button();
			this->logoutBtn = gcnew Button();
			this->exitBtn = gcnew Button();
			this->fileBox = gcnew RichTextBox();
			this->openFileDialog = gcnew OpenFileDialog();
			this->SuspendLayout();

			// welcomeMsg
			this->welcomeMsg->Location = Drawing::Point(58, 220);
			this->welcomeMsg->Name = L"welcomeMsg";
			this->welcomeMsg->Size = Drawing::Size(0, 100);
			this->welcomeMsg->TabIndex = 4;

			// browseBtn
			this->browseBtn->Anchor = AnchorStyles::Top;
			this->browseBtn->Location = Drawing::Point(472, 188);
			this->browseBtn->Name = L"browseBtn";
			this->browseBtn->Size = Drawing::Size(164, 49);
			this->browseBtn->TabIndex = 5;
			this->browseBtn->Text = L"Browse For A File:";
			this->browseBtn->Click += gcnew EventHandler(this, &encryptMenu::browseBtn_Click);

			// encryptBtn
			this->encryptBtn->Anchor = AnchorStyles::Top;
			this->encryptBtn->Location = Drawing::Point(250, 275);
			this->encryptBtn->Name = L"encryptBtn";
			this->encryptBtn->Size = Drawing::Size(164, 49);
			this->encryptBtn->TabIndex = 6;
			this->encryptBtn->Text = L"Upload File";

			// logoutBtn
			this->logoutBtn->Anchor = AnchorStyles::Top;
			this->logoutBtn->Location = Drawing::Point(169, 475);
			this->logoutBtn->Name = L"logoutBtn";
			this->logoutBtn->Size = Drawing::Size(169, 49);
			this->logoutBtn->TabIndex = 7;
			this->logoutBtn->Text = L"Logout";

			// exitBtn
			this->exitBtn->Anchor = AnchorStyles::Top;
			this->exitBtn->Location = Drawing::Point(369, 475);
			this->exitBtn->Name = L"exitBtn";
			this->exitBtn->Size = Drawing::Size(169, 49);
			this->exitBtn->TabIndex = 8;
			this->exitBtn->Text = L"Exit";

			// fileBox
			this->fileBox->Location = Drawing::Point(169, 192);
			this->fileBox->Name = L"fileBox";
			this->fileBox->Size = Drawing::Size(297, 45);
			this->fileBox->TabIndex = 9;
			this->fileBox->Text = L"";

			// encryptMenu Form
			this->ClientSize = Drawing::Size(807, 589);
			this->Controls->Add(this->fileBox);
			this->Controls->Add(this->welcomeMsg);
			this->Controls->Add(this->browseBtn);
			this->Controls->Add(this->encryptBtn);
			this->Controls->Add(this->logoutBtn);
			this->Controls->Add(this->exitBtn);
			this->Name = L"encryptMenu";
			this->Text = L"Encrypt Menu";
			this->ResumeLayout(false);
		}
#pragma endregion

	private:
		void SetFileBoxPath(String^ filePath) {
			fileBox->ReadOnly = true; // Lock the fileBox from being edited
			fileBox->Text = filePath; // Set the file path in the fileBox
			// Copy the path to clipboard
			Clipboard::SetText(filePath);
		}

	private:
		System::Void browseBtn_Click(System::Object^ sender, System::EventArgs^ e) {
			openFileDialog->Title = "Select a file to upload";
			openFileDialog->Filter = "All Files (*.*)|*.*";

			if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				String^ filePath = openFileDialog->FileName;
				SetFileBoxPath(filePath);
			}
		}
	};
}
