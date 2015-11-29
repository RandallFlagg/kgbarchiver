#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace KGBArchiver2net {

	/// <summary>
	/// Summary for properties
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class properties : public System::Windows::Forms::Form
	{
	public:
		properties(void)
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
		~properties()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::GroupBox^  gDates;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	public: System::Windows::Forms::DateTimePicker^  dtAccessed;
	public: System::Windows::Forms::DateTimePicker^  dtModified;
	public: System::Windows::Forms::DateTimePicker^  dtCreated;
	public: System::Windows::Forms::GroupBox^  gAttributes;

	public: System::Windows::Forms::CheckBox^  cSystem;
	public: System::Windows::Forms::CheckBox^  cHidden;
	public: System::Windows::Forms::CheckBox^  cArchive;
	public: System::Windows::Forms::CheckBox^  cReadOnly;
	public: System::Windows::Forms::GroupBox^  gFile;

	public: System::Windows::Forms::Label^  lType;
	public: System::Windows::Forms::TextBox^  tFilename;
	public: System::Windows::Forms::PictureBox^  pIcon;
	private: System::Windows::Forms::Button^  bCancel;
	private: System::Windows::Forms::Button^  bOK;
	public: System::Windows::Forms::CheckBox^  cEncrypted;
	public: System::Windows::Forms::CheckBox^  cCompressed;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(properties::typeid));
			this->gDates = (gcnew System::Windows::Forms::GroupBox());
			this->dtAccessed = (gcnew System::Windows::Forms::DateTimePicker());
			this->dtModified = (gcnew System::Windows::Forms::DateTimePicker());
			this->dtCreated = (gcnew System::Windows::Forms::DateTimePicker());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->gAttributes = (gcnew System::Windows::Forms::GroupBox());
			this->cEncrypted = (gcnew System::Windows::Forms::CheckBox());
			this->cCompressed = (gcnew System::Windows::Forms::CheckBox());
			this->cSystem = (gcnew System::Windows::Forms::CheckBox());
			this->cHidden = (gcnew System::Windows::Forms::CheckBox());
			this->cArchive = (gcnew System::Windows::Forms::CheckBox());
			this->cReadOnly = (gcnew System::Windows::Forms::CheckBox());
			this->gFile = (gcnew System::Windows::Forms::GroupBox());
			this->lType = (gcnew System::Windows::Forms::Label());
			this->tFilename = (gcnew System::Windows::Forms::TextBox());
			this->pIcon = (gcnew System::Windows::Forms::PictureBox());
			this->bCancel = (gcnew System::Windows::Forms::Button());
			this->bOK = (gcnew System::Windows::Forms::Button());
			this->gDates->SuspendLayout();
			this->gAttributes->SuspendLayout();
			this->gFile->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pIcon))->BeginInit();
			this->SuspendLayout();
			// 
			// gDates
			// 
			this->gDates->AccessibleDescription = nullptr;
			this->gDates->AccessibleName = nullptr;
			resources->ApplyResources(this->gDates, L"gDates");
			this->gDates->BackgroundImage = nullptr;
			this->gDates->Controls->Add(this->dtAccessed);
			this->gDates->Controls->Add(this->dtModified);
			this->gDates->Controls->Add(this->dtCreated);
			this->gDates->Controls->Add(this->label3);
			this->gDates->Controls->Add(this->label2);
			this->gDates->Controls->Add(this->label1);
			this->gDates->Font = nullptr;
			this->gDates->Name = L"gDates";
			this->gDates->TabStop = false;
			// 
			// dtAccessed
			// 
			this->dtAccessed->AccessibleDescription = nullptr;
			this->dtAccessed->AccessibleName = nullptr;
			resources->ApplyResources(this->dtAccessed, L"dtAccessed");
			this->dtAccessed->BackgroundImage = nullptr;
			this->dtAccessed->CalendarFont = nullptr;
			this->dtAccessed->Font = nullptr;
			this->dtAccessed->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dtAccessed->Name = L"dtAccessed";
			// 
			// dtModified
			// 
			this->dtModified->AccessibleDescription = nullptr;
			this->dtModified->AccessibleName = nullptr;
			resources->ApplyResources(this->dtModified, L"dtModified");
			this->dtModified->BackgroundImage = nullptr;
			this->dtModified->CalendarFont = nullptr;
			this->dtModified->Font = nullptr;
			this->dtModified->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dtModified->Name = L"dtModified";
			// 
			// dtCreated
			// 
			this->dtCreated->AccessibleDescription = nullptr;
			this->dtCreated->AccessibleName = nullptr;
			resources->ApplyResources(this->dtCreated, L"dtCreated");
			this->dtCreated->BackgroundImage = nullptr;
			this->dtCreated->CalendarFont = nullptr;
			this->dtCreated->Font = nullptr;
			this->dtCreated->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dtCreated->Name = L"dtCreated";
			// 
			// label3
			// 
			this->label3->AccessibleDescription = nullptr;
			this->label3->AccessibleName = nullptr;
			resources->ApplyResources(this->label3, L"label3");
			this->label3->Font = nullptr;
			this->label3->Name = L"label3";
			// 
			// label2
			// 
			this->label2->AccessibleDescription = nullptr;
			this->label2->AccessibleName = nullptr;
			resources->ApplyResources(this->label2, L"label2");
			this->label2->Font = nullptr;
			this->label2->Name = L"label2";
			// 
			// label1
			// 
			this->label1->AccessibleDescription = nullptr;
			this->label1->AccessibleName = nullptr;
			resources->ApplyResources(this->label1, L"label1");
			this->label1->Font = nullptr;
			this->label1->Name = L"label1";
			// 
			// gAttributes
			// 
			this->gAttributes->AccessibleDescription = nullptr;
			this->gAttributes->AccessibleName = nullptr;
			resources->ApplyResources(this->gAttributes, L"gAttributes");
			this->gAttributes->BackgroundImage = nullptr;
			this->gAttributes->Controls->Add(this->cEncrypted);
			this->gAttributes->Controls->Add(this->cCompressed);
			this->gAttributes->Controls->Add(this->cSystem);
			this->gAttributes->Controls->Add(this->cHidden);
			this->gAttributes->Controls->Add(this->cArchive);
			this->gAttributes->Controls->Add(this->cReadOnly);
			this->gAttributes->Font = nullptr;
			this->gAttributes->Name = L"gAttributes";
			this->gAttributes->TabStop = false;
			// 
			// cEncrypted
			// 
			this->cEncrypted->AccessibleDescription = nullptr;
			this->cEncrypted->AccessibleName = nullptr;
			resources->ApplyResources(this->cEncrypted, L"cEncrypted");
			this->cEncrypted->BackgroundImage = nullptr;
			this->cEncrypted->Font = nullptr;
			this->cEncrypted->Name = L"cEncrypted";
			this->cEncrypted->UseVisualStyleBackColor = true;
			// 
			// cCompressed
			// 
			this->cCompressed->AccessibleDescription = nullptr;
			this->cCompressed->AccessibleName = nullptr;
			resources->ApplyResources(this->cCompressed, L"cCompressed");
			this->cCompressed->BackgroundImage = nullptr;
			this->cCompressed->Font = nullptr;
			this->cCompressed->Name = L"cCompressed";
			this->cCompressed->UseVisualStyleBackColor = true;
			// 
			// cSystem
			// 
			this->cSystem->AccessibleDescription = nullptr;
			this->cSystem->AccessibleName = nullptr;
			resources->ApplyResources(this->cSystem, L"cSystem");
			this->cSystem->BackgroundImage = nullptr;
			this->cSystem->Font = nullptr;
			this->cSystem->Name = L"cSystem";
			this->cSystem->UseVisualStyleBackColor = true;
			// 
			// cHidden
			// 
			this->cHidden->AccessibleDescription = nullptr;
			this->cHidden->AccessibleName = nullptr;
			resources->ApplyResources(this->cHidden, L"cHidden");
			this->cHidden->BackgroundImage = nullptr;
			this->cHidden->Font = nullptr;
			this->cHidden->Name = L"cHidden";
			this->cHidden->UseVisualStyleBackColor = true;
			// 
			// cArchive
			// 
			this->cArchive->AccessibleDescription = nullptr;
			this->cArchive->AccessibleName = nullptr;
			resources->ApplyResources(this->cArchive, L"cArchive");
			this->cArchive->BackgroundImage = nullptr;
			this->cArchive->Font = nullptr;
			this->cArchive->Name = L"cArchive";
			this->cArchive->UseVisualStyleBackColor = true;
			// 
			// cReadOnly
			// 
			this->cReadOnly->AccessibleDescription = nullptr;
			this->cReadOnly->AccessibleName = nullptr;
			resources->ApplyResources(this->cReadOnly, L"cReadOnly");
			this->cReadOnly->BackgroundImage = nullptr;
			this->cReadOnly->Font = nullptr;
			this->cReadOnly->Name = L"cReadOnly";
			this->cReadOnly->UseVisualStyleBackColor = true;
			// 
			// gFile
			// 
			this->gFile->AccessibleDescription = nullptr;
			this->gFile->AccessibleName = nullptr;
			resources->ApplyResources(this->gFile, L"gFile");
			this->gFile->BackgroundImage = nullptr;
			this->gFile->Controls->Add(this->lType);
			this->gFile->Controls->Add(this->tFilename);
			this->gFile->Controls->Add(this->pIcon);
			this->gFile->Font = nullptr;
			this->gFile->Name = L"gFile";
			this->gFile->TabStop = false;
			// 
			// lType
			// 
			this->lType->AccessibleDescription = nullptr;
			this->lType->AccessibleName = nullptr;
			resources->ApplyResources(this->lType, L"lType");
			this->lType->Font = nullptr;
			this->lType->Name = L"lType";
			// 
			// tFilename
			// 
			this->tFilename->AccessibleDescription = nullptr;
			this->tFilename->AccessibleName = nullptr;
			resources->ApplyResources(this->tFilename, L"tFilename");
			this->tFilename->BackgroundImage = nullptr;
			this->tFilename->Font = nullptr;
			this->tFilename->Name = L"tFilename";
			// 
			// pIcon
			// 
			this->pIcon->AccessibleDescription = nullptr;
			this->pIcon->AccessibleName = nullptr;
			resources->ApplyResources(this->pIcon, L"pIcon");
			this->pIcon->BackgroundImage = nullptr;
			this->pIcon->Font = nullptr;
			this->pIcon->ImageLocation = nullptr;
			this->pIcon->Name = L"pIcon";
			this->pIcon->TabStop = false;
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
			// properties
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
			this->Controls->Add(this->gFile);
			this->Controls->Add(this->gAttributes);
			this->Controls->Add(this->gDates);
			this->Font = nullptr;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = nullptr;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"properties";
			this->gDates->ResumeLayout(false);
			this->gDates->PerformLayout();
			this->gAttributes->ResumeLayout(false);
			this->gAttributes->PerformLayout();
			this->gFile->ResumeLayout(false);
			this->gFile->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pIcon))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion


};
}
