#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace KGBArchiver2net {

	/// <summary>
	/// Summary for ColumnSelect
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ColumnSelect : public System::Windows::Forms::Form
	{
	public:
		ColumnSelect(void)
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
		~ColumnSelect()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  bOK;
	private: System::Windows::Forms::Button^  bCancel;
	public protected: System::Windows::Forms::CheckBox^  cFilename; 
	public protected: System::Windows::Forms::CheckBox^  cSize;
	public protected: System::Windows::Forms::CheckBox^  cCreated;
	public protected: System::Windows::Forms::CheckBox^  cLastmodified;
	public protected: System::Windows::Forms::CheckBox^  cLastaccessed;
	public protected: System::Windows::Forms::CheckBox^  cAttributes;

	private: System::Windows::Forms::GroupBox^  groupBox1;
	public: System::Windows::Forms::CheckBox^  cType;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(ColumnSelect::typeid));
			this->bOK = (gcnew System::Windows::Forms::Button());
			this->bCancel = (gcnew System::Windows::Forms::Button());
			this->cFilename = (gcnew System::Windows::Forms::CheckBox());
			this->cSize = (gcnew System::Windows::Forms::CheckBox());
			this->cCreated = (gcnew System::Windows::Forms::CheckBox());
			this->cLastmodified = (gcnew System::Windows::Forms::CheckBox());
			this->cLastaccessed = (gcnew System::Windows::Forms::CheckBox());
			this->cAttributes = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->cType = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
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
			// cFilename
			// 
			this->cFilename->AccessibleDescription = nullptr;
			this->cFilename->AccessibleName = nullptr;
			resources->ApplyResources(this->cFilename, L"cFilename");
			this->cFilename->BackgroundImage = nullptr;
			this->cFilename->Font = nullptr;
			this->cFilename->Name = L"cFilename";
			this->cFilename->UseVisualStyleBackColor = true;
			// 
			// cSize
			// 
			this->cSize->AccessibleDescription = nullptr;
			this->cSize->AccessibleName = nullptr;
			resources->ApplyResources(this->cSize, L"cSize");
			this->cSize->BackgroundImage = nullptr;
			this->cSize->Font = nullptr;
			this->cSize->Name = L"cSize";
			this->cSize->UseVisualStyleBackColor = true;
			// 
			// cCreated
			// 
			this->cCreated->AccessibleDescription = nullptr;
			this->cCreated->AccessibleName = nullptr;
			resources->ApplyResources(this->cCreated, L"cCreated");
			this->cCreated->BackgroundImage = nullptr;
			this->cCreated->Font = nullptr;
			this->cCreated->Name = L"cCreated";
			this->cCreated->UseVisualStyleBackColor = true;
			// 
			// cLastmodified
			// 
			this->cLastmodified->AccessibleDescription = nullptr;
			this->cLastmodified->AccessibleName = nullptr;
			resources->ApplyResources(this->cLastmodified, L"cLastmodified");
			this->cLastmodified->BackgroundImage = nullptr;
			this->cLastmodified->Font = nullptr;
			this->cLastmodified->Name = L"cLastmodified";
			this->cLastmodified->UseVisualStyleBackColor = true;
			// 
			// cLastaccessed
			// 
			this->cLastaccessed->AccessibleDescription = nullptr;
			this->cLastaccessed->AccessibleName = nullptr;
			resources->ApplyResources(this->cLastaccessed, L"cLastaccessed");
			this->cLastaccessed->BackgroundImage = nullptr;
			this->cLastaccessed->Font = nullptr;
			this->cLastaccessed->Name = L"cLastaccessed";
			this->cLastaccessed->UseVisualStyleBackColor = true;
			// 
			// cAttributes
			// 
			this->cAttributes->AccessibleDescription = nullptr;
			this->cAttributes->AccessibleName = nullptr;
			resources->ApplyResources(this->cAttributes, L"cAttributes");
			this->cAttributes->BackgroundImage = nullptr;
			this->cAttributes->Font = nullptr;
			this->cAttributes->Name = L"cAttributes";
			this->cAttributes->UseVisualStyleBackColor = true;
			// 
			// groupBox1
			// 
			this->groupBox1->AccessibleDescription = nullptr;
			this->groupBox1->AccessibleName = nullptr;
			resources->ApplyResources(this->groupBox1, L"groupBox1");
			this->groupBox1->BackgroundImage = nullptr;
			this->groupBox1->Controls->Add(this->cType);
			this->groupBox1->Controls->Add(this->cFilename);
			this->groupBox1->Controls->Add(this->cAttributes);
			this->groupBox1->Controls->Add(this->cSize);
			this->groupBox1->Controls->Add(this->cLastaccessed);
			this->groupBox1->Controls->Add(this->cCreated);
			this->groupBox1->Controls->Add(this->cLastmodified);
			this->groupBox1->Font = nullptr;
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->TabStop = false;
			// 
			// cType
			// 
			this->cType->AccessibleDescription = nullptr;
			this->cType->AccessibleName = nullptr;
			resources->ApplyResources(this->cType, L"cType");
			this->cType->BackgroundImage = nullptr;
			this->cType->Font = nullptr;
			this->cType->Name = L"cType";
			this->cType->UseVisualStyleBackColor = true;
			// 
			// ColumnSelect
			// 
			this->AcceptButton = this->bOK;
			this->AccessibleDescription = nullptr;
			this->AccessibleName = nullptr;
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = nullptr;
			this->CancelButton = this->bCancel;
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->bCancel);
			this->Controls->Add(this->bOK);
			this->Font = nullptr;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = nullptr;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"ColumnSelect";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
};
}
