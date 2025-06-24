// mainMenu.h
#pragma once

#include "encryptMenu.h"
#include "decryptMenu.h"


namespace WindowsForm {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class mainMenu : public System::Windows::Forms::Form
	{
	public:
		mainMenu(void)
		{
			InitializeComponent();
		}

	protected:
		~mainMenu()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Label^ label3; // This label is declared but not used or initialized
	private: System::Windows::Forms::Button^ EncryptBtn;
	private: System::Windows::Forms::Button^ decryptBtn;
	private: System::Windows::Forms::Button^ exitBtn; // Corrected button name for clarity

	private:
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->EncryptBtn = (gcnew System::Windows::Forms::Button());
			this->decryptBtn = (gcnew System::Windows::Forms::Button());
			this->exitBtn = (gcnew System::Windows::Forms::Button()); // Renamed from SaveBtn/LogoutBtn placeholder

			this->SuspendLayout();

			//
			// EncryptBtn
			//
			this->EncryptBtn->Location = System::Drawing::Point(172, 58);
			this->EncryptBtn->Name = L"EncryptBtn";
			this->EncryptBtn->Size = System::Drawing::Size(164, 49);
			this->EncryptBtn->TabIndex = 5;
			this->EncryptBtn->Text = L"Encrypt";
			this->EncryptBtn->UseVisualStyleBackColor = true;
			this->EncryptBtn->Click += gcnew System::EventHandler(this, &mainMenu::EncryptBtn_Click);
			//
			// DecryptBtn
			//
			this->decryptBtn->Location = System::Drawing::Point(172, 127);
			this->decryptBtn->Name = L"DecryptBtn";
			this->decryptBtn->Size = System::Drawing::Size(164, 49);
			this->decryptBtn->TabIndex = 6;
			this->decryptBtn->Text = L"Decrypt";
			this->decryptBtn->UseVisualStyleBackColor = true;
			this->decryptBtn->Click += gcnew System::EventHandler(this, &mainMenu::DecryptBtn_Click);
			//
			// exitBtn
			//
			this->exitBtn->Location = System::Drawing::Point(172, 200); // Adjusted location
			this->exitBtn->Name = L"exitBtn";
			this->exitBtn->Size = System::Drawing::Size(164, 49);
			this->exitBtn->TabIndex = 9;
			this->exitBtn->Text = L"Exit";
			this->exitBtn->UseVisualStyleBackColor = true;
			this->exitBtn->Click += gcnew System::EventHandler(this, &mainMenu::exitBtn_Click); // Added click event handler
			//
			// mainMenu
			//
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(537, 436);
			this->Controls->Add(this->exitBtn);
			this->Controls->Add(this->decryptBtn);
			this->Controls->Add(this->EncryptBtn);
			this->MinimumSize = System::Drawing::Size(553, 475);
			this->Name = L"mainMenu";
			this->Text = L"Main Menu"; // Changed title for clarity
			this->ResumeLayout(false);
			// Removed this->PerformLayout(); as label3 is removed from controls
		}
#pragma endregion	

	private: System::Void EncryptBtn_Click(System::Object^ sender, System::EventArgs^ e) {
		WindowsForm::encryptMenu^ menu = gcnew WindowsForm::encryptMenu();
		menu->ShowDialog(); // Show as modal dialog
	}
	private: System::Void DecryptBtn_Click(System::Object^ sender, System::EventArgs^ e) {
		WindowsForm::decryptMenu^ menu = gcnew WindowsForm::decryptMenu();
		menu->ShowDialog(); // Show as modal dialog
	}
	private: System::Void exitBtn_Click(System::Object^ sender, System::EventArgs^ e) {
		this->Close(); // Close the main menu form, which will exit the application
	}
	};
}
