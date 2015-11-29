#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace KGBArchiver2net {

	/// <summary>
	/// Summary for passwd
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class passwd : public System::Windows::Forms::Form
	{
	public:
		passwd(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~passwd()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::GroupBox^  gPasswd;
	public: System::Windows::Forms::TextBox^  tPasswd;
	private: 
	protected: 

	private: System::Windows::Forms::Button^  bCancel;
	private: System::Windows::Forms::Button^  bOK;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(passwd::typeid));
			this->gPasswd = (gcnew System::Windows::Forms::GroupBox());
			this->tPasswd = (gcnew System::Windows::Forms::TextBox());
			this->bCancel = (gcnew System::Windows::Forms::Button());
			this->bOK = (gcnew System::Windows::Forms::Button());
			this->gPasswd->SuspendLayout();
			this->SuspendLayout();
			// 
			// gPasswd
			// 
			this->gPasswd->AccessibleDescription = nullptr;
			this->gPasswd->AccessibleName = nullptr;
			resources->ApplyResources(this->gPasswd, L"gPasswd");
			this->gPasswd->BackgroundImage = nullptr;
			this->gPasswd->Controls->Add(this->tPasswd);
			this->gPasswd->Font = nullptr;
			this->gPasswd->Name = L"gPasswd";
			this->gPasswd->TabStop = false;
			// 
			// tPasswd
			// 
			this->tPasswd->AccessibleDescription = nullptr;
			this->tPasswd->AccessibleName = nullptr;
			resources->ApplyResources(this->tPasswd, L"tPasswd");
			this->tPasswd->BackgroundImage = nullptr;
			this->tPasswd->Font = nullptr;
			this->tPasswd->Name = L"tPasswd";
			this->tPasswd->UseSystemPasswordChar = true;
			// 
			// bCancel
			// 
			this->bCancel->AccessibleDescription = nullptr;
			this->bCancel->AccessibleName = nullptr;
			resources->ApplyResources(this->bCancel, L"bCancel");
			this->bCancel->BackgroundImage = nullptr;
			this->bCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->bCancel->Font = nullptr;
			this->bCancel->Name = L"bCancel";
			this->bCancel->UseVisualStyleBackColor = true;
			// 
			// bOK
			// 
			this->bOK->AccessibleDescription = nullptr;
			this->bOK->AccessibleName = nullptr;
			resources->ApplyResources(this->bOK, L"bOK");
			this->bOK->BackgroundImage = nullptr;
			this->bOK->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->bOK->Font = nullptr;
			this->bOK->Name = L"bOK";
			this->bOK->UseVisualStyleBackColor = true;
			// 
			// passwd
			// 
			this->AcceptButton = this->bOK;
			this->AccessibleDescription = nullptr;
			this->AccessibleName = nullptr;
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = nullptr;
			this->CancelButton = this->bCancel;
			this->Controls->Add(this->bOK);
			this->Controls->Add(this->bCancel);
			this->Controls->Add(this->gPasswd);
			this->Font = nullptr;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = nullptr;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"passwd";
			this->gPasswd->ResumeLayout(false);
			this->gPasswd->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
