
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

	private: System::Windows::Forms::Button^ EncryptBtn;
	private: System::Windows::Forms::Button^ decryptBtn;
	private: System::Windows::Forms::Button^ exitBtn;

	private:
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->EncryptBtn = (gcnew System::Windows::Forms::Button()); // Button to trigger encryption
			this->decryptBtn = (gcnew System::Windows::Forms::Button()); // Button to trigger decryption
			this->exitBtn = (gcnew System::Windows::Forms::Button()); // Button to exit the application

			this->SuspendLayout();

			//
			// EncryptBtn
			//
			this->EncryptBtn->Location = System::Drawing::Point(172, 58); // Set location of the button
			this->EncryptBtn->Name = L"EncryptBtn"; // Name of the button
			this->EncryptBtn->Size = System::Drawing::Size(164, 49); // Set size of the button
			this->EncryptBtn->TabIndex = 5; // Tab index for focus order
			this->EncryptBtn->Text = L"Encrypt"; // Text displayed on the button
			this->EncryptBtn->UseVisualStyleBackColor = true; // Use the default button style
			this->EncryptBtn->Click += gcnew System::EventHandler(this, &mainMenu::EncryptBtn_Click); // Event handler for button click
			//
			// DecryptBtn
			//
			this->decryptBtn->Location = System::Drawing::Point(172, 127); // Set location of the button
			this->decryptBtn->Name = L"DecryptBtn"; // Name of the button
			this->decryptBtn->Size = System::Drawing::Size(164, 49); // Set size of the button
			this->decryptBtn->TabIndex = 6; // Tab index for focus order
			this->decryptBtn->Text = L"Decrypt"; // Text displayed on the button
			this->decryptBtn->UseVisualStyleBackColor = true; // Use the default button style
			this->decryptBtn->Click += gcnew System::EventHandler(this, &mainMenu::DecryptBtn_Click); // Event handler for button click
			//
			// exitBtn
			//
			this->exitBtn->Location = System::Drawing::Point(172, 200); // Set location of the button
			this->exitBtn->Name = L"exitBtn"; // Name of the button
			this->exitBtn->Size = System::Drawing::Size(164, 49); // Set size of the button
			this->exitBtn->TabIndex = 9; // Tab index for focus order
			this->exitBtn->Text = L"Exit"; // Text displayed on the button
			this->exitBtn->UseVisualStyleBackColor = true; // Use the default button style
			this->exitBtn->Click += gcnew System::EventHandler(this, &mainMenu::exitBtn_Click); // Event handler for button click 
			//
			// mainMenu
			//
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13); // Set the scaling dimensions for the form
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font; // Set the scaling mode for the form
			this->ClientSize = System::Drawing::Size(537, 436); // Set the size of the form
			this->Controls->Add(this->exitBtn); // Add the exit button to the form
			this->Controls->Add(this->decryptBtn); // Add the decrypt button to the form
			this->Controls->Add(this->EncryptBtn); // Add the encrypt button to the form
			this->MinimumSize = System::Drawing::Size(553, 475); // Set the minimum size of the form
			this->Name = L"mainMenu"; // Name of the form
			this->Text = L"Main Menu"; // Title of the form
			this->ResumeLayout(false); // Resume layout of the form

		}
#pragma endregion	

	private: System::Void EncryptBtn_Click(System::Object^ sender, System::EventArgs^ e) {
		WindowsForm::encryptMenu^ menu = gcnew WindowsForm::encryptMenu();
		menu->ShowDialog(); 
	}
	private: System::Void DecryptBtn_Click(System::Object^ sender, System::EventArgs^ e) {
		WindowsForm::decryptMenu^ menu = gcnew WindowsForm::decryptMenu();
		menu->ShowDialog(); 
	}
	private: System::Void exitBtn_Click(System::Object^ sender, System::EventArgs^ e) {
		this->Close(); 
	}
	};
}
