#include "update.cpp"

#pragma once

namespace KGBArchiver2net {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	/// <summary>
	/// Summary for Options
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Options : public System::Windows::Forms::Form
	{
	public:
		Options(void)
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
		~Options()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::TabControl^  tMain;
	public: System::Windows::Forms::TabPage^  tGeneral;
	public: System::Windows::Forms::TabPage^  tShellExtension;
	public: System::Windows::Forms::Button^  bOK;
	public: System::Windows::Forms::Button^  bCancel;
	public: System::Windows::Forms::GroupBox^  groupBox1;
	public: System::Windows::Forms::ComboBox^  cLang;
	public: System::Windows::Forms::Button^  bCheckNow;
	public: System::Windows::Forms::CheckBox^  cUpdate;
	public: System::Windows::Forms::CheckBox^  cSaveState;
	public: System::Windows::Forms::GroupBox^  groupBox2;
	public: System::Windows::Forms::CheckedListBox^  clAssoc;
	public: System::Windows::Forms::CheckBox^  cCompressTo;
	public: System::Windows::Forms::CheckBox^  cExtractTo;
	public: System::Windows::Forms::TabPage^  tFileManager;
	public: System::Windows::Forms::CheckBox^  cArchiveTree;
	public: System::Windows::Forms::CheckBox^  cHidden;
	public: System::Windows::Forms::CheckBox^  cFileGroups;
	private: System::Windows::Forms::Label^  lRecentItemsCount;
	public: System::Windows::Forms::NumericUpDown^  nRecentItemsCount;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Options::typeid));
			this->tMain = (gcnew System::Windows::Forms::TabControl());
			this->tGeneral = (gcnew System::Windows::Forms::TabPage());
			this->nRecentItemsCount = (gcnew System::Windows::Forms::NumericUpDown());
			this->lRecentItemsCount = (gcnew System::Windows::Forms::Label());
			this->cSaveState = (gcnew System::Windows::Forms::CheckBox());
			this->bCheckNow = (gcnew System::Windows::Forms::Button());
			this->cUpdate = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->cLang = (gcnew System::Windows::Forms::ComboBox());
			this->tFileManager = (gcnew System::Windows::Forms::TabPage());
			this->cFileGroups = (gcnew System::Windows::Forms::CheckBox());
			this->cArchiveTree = (gcnew System::Windows::Forms::CheckBox());
			this->cHidden = (gcnew System::Windows::Forms::CheckBox());
			this->tShellExtension = (gcnew System::Windows::Forms::TabPage());
			this->cExtractTo = (gcnew System::Windows::Forms::CheckBox());
			this->cCompressTo = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->clAssoc = (gcnew System::Windows::Forms::CheckedListBox());
			this->bOK = (gcnew System::Windows::Forms::Button());
			this->bCancel = (gcnew System::Windows::Forms::Button());
			this->tMain->SuspendLayout();
			this->tGeneral->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nRecentItemsCount))->BeginInit();
			this->groupBox1->SuspendLayout();
			this->tFileManager->SuspendLayout();
			this->tShellExtension->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// tMain
			// 
			this->tMain->AccessibleDescription = nullptr;
			this->tMain->AccessibleName = nullptr;
			resources->ApplyResources(this->tMain, L"tMain");
			this->tMain->BackgroundImage = nullptr;
			this->tMain->Controls->Add(this->tGeneral);
			this->tMain->Controls->Add(this->tFileManager);
			this->tMain->Controls->Add(this->tShellExtension);
			this->tMain->Font = nullptr;
			this->tMain->Name = L"tMain";
			this->tMain->SelectedIndex = 0;
			// 
			// tGeneral
			// 
			this->tGeneral->AccessibleDescription = nullptr;
			this->tGeneral->AccessibleName = nullptr;
			resources->ApplyResources(this->tGeneral, L"tGeneral");
			this->tGeneral->BackgroundImage = nullptr;
			this->tGeneral->Controls->Add(this->nRecentItemsCount);
			this->tGeneral->Controls->Add(this->lRecentItemsCount);
			this->tGeneral->Controls->Add(this->cSaveState);
			this->tGeneral->Controls->Add(this->bCheckNow);
			this->tGeneral->Controls->Add(this->cUpdate);
			this->tGeneral->Controls->Add(this->groupBox1);
			this->tGeneral->Font = nullptr;
			this->tGeneral->Name = L"tGeneral";
			this->tGeneral->UseVisualStyleBackColor = true;
			// 
			// nRecentItemsCount
			// 
			this->nRecentItemsCount->AccessibleDescription = nullptr;
			this->nRecentItemsCount->AccessibleName = nullptr;
			resources->ApplyResources(this->nRecentItemsCount, L"nRecentItemsCount");
			this->nRecentItemsCount->Font = nullptr;
			this->nRecentItemsCount->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {20, 0, 0, 0});
			this->nRecentItemsCount->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {3, 0, 0, 0});
			this->nRecentItemsCount->Name = L"nRecentItemsCount";
			this->nRecentItemsCount->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
			// 
			// lRecentItemsCount
			// 
			this->lRecentItemsCount->AccessibleDescription = nullptr;
			this->lRecentItemsCount->AccessibleName = nullptr;
			resources->ApplyResources(this->lRecentItemsCount, L"lRecentItemsCount");
			this->lRecentItemsCount->Font = nullptr;
			this->lRecentItemsCount->Name = L"lRecentItemsCount";
			// 
			// cSaveState
			// 
			this->cSaveState->AccessibleDescription = nullptr;
			this->cSaveState->AccessibleName = nullptr;
			resources->ApplyResources(this->cSaveState, L"cSaveState");
			this->cSaveState->BackgroundImage = nullptr;
			this->cSaveState->Font = nullptr;
			this->cSaveState->Name = L"cSaveState";
			this->cSaveState->UseVisualStyleBackColor = true;
			// 
			// bCheckNow
			// 
			this->bCheckNow->AccessibleDescription = nullptr;
			this->bCheckNow->AccessibleName = nullptr;
			resources->ApplyResources(this->bCheckNow, L"bCheckNow");
			this->bCheckNow->BackgroundImage = nullptr;
			this->bCheckNow->Font = nullptr;
			this->bCheckNow->Name = L"bCheckNow";
			this->bCheckNow->UseVisualStyleBackColor = true;
			this->bCheckNow->Click += gcnew System::EventHandler(this, &Options::bCheckNow_Click);
			// 
			// cUpdate
			// 
			this->cUpdate->AccessibleDescription = nullptr;
			this->cUpdate->AccessibleName = nullptr;
			resources->ApplyResources(this->cUpdate, L"cUpdate");
			this->cUpdate->BackgroundImage = nullptr;
			this->cUpdate->Font = nullptr;
			this->cUpdate->Name = L"cUpdate";
			this->cUpdate->UseVisualStyleBackColor = true;
			// 
			// groupBox1
			// 
			this->groupBox1->AccessibleDescription = nullptr;
			this->groupBox1->AccessibleName = nullptr;
			resources->ApplyResources(this->groupBox1, L"groupBox1");
			this->groupBox1->BackgroundImage = nullptr;
			this->groupBox1->Controls->Add(this->cLang);
			this->groupBox1->Font = nullptr;
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->TabStop = false;
			// 
			// cLang
			// 
			this->cLang->AccessibleDescription = nullptr;
			this->cLang->AccessibleName = nullptr;
			resources->ApplyResources(this->cLang, L"cLang");
			this->cLang->BackgroundImage = nullptr;
			this->cLang->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cLang->Font = nullptr;
			this->cLang->FormattingEnabled = true;
			this->cLang->Name = L"cLang";
			// 
			// tFileManager
			// 
			this->tFileManager->AccessibleDescription = nullptr;
			this->tFileManager->AccessibleName = nullptr;
			resources->ApplyResources(this->tFileManager, L"tFileManager");
			this->tFileManager->BackgroundImage = nullptr;
			this->tFileManager->Controls->Add(this->cFileGroups);
			this->tFileManager->Controls->Add(this->cArchiveTree);
			this->tFileManager->Controls->Add(this->cHidden);
			this->tFileManager->Font = nullptr;
			this->tFileManager->Name = L"tFileManager";
			this->tFileManager->UseVisualStyleBackColor = true;
			// 
			// cFileGroups
			// 
			this->cFileGroups->AccessibleDescription = nullptr;
			this->cFileGroups->AccessibleName = nullptr;
			resources->ApplyResources(this->cFileGroups, L"cFileGroups");
			this->cFileGroups->BackgroundImage = nullptr;
			this->cFileGroups->Font = nullptr;
			this->cFileGroups->Name = L"cFileGroups";
			this->cFileGroups->UseVisualStyleBackColor = true;
			// 
			// cArchiveTree
			// 
			this->cArchiveTree->AccessibleDescription = nullptr;
			this->cArchiveTree->AccessibleName = nullptr;
			resources->ApplyResources(this->cArchiveTree, L"cArchiveTree");
			this->cArchiveTree->BackgroundImage = nullptr;
			this->cArchiveTree->Font = nullptr;
			this->cArchiveTree->Name = L"cArchiveTree";
			this->cArchiveTree->UseVisualStyleBackColor = true;
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
			// tShellExtension
			// 
			this->tShellExtension->AccessibleDescription = nullptr;
			this->tShellExtension->AccessibleName = nullptr;
			resources->ApplyResources(this->tShellExtension, L"tShellExtension");
			this->tShellExtension->BackgroundImage = nullptr;
			this->tShellExtension->Controls->Add(this->cExtractTo);
			this->tShellExtension->Controls->Add(this->cCompressTo);
			this->tShellExtension->Controls->Add(this->groupBox2);
			this->tShellExtension->Font = nullptr;
			this->tShellExtension->Name = L"tShellExtension";
			this->tShellExtension->UseVisualStyleBackColor = true;
			// 
			// cExtractTo
			// 
			this->cExtractTo->AccessibleDescription = nullptr;
			this->cExtractTo->AccessibleName = nullptr;
			resources->ApplyResources(this->cExtractTo, L"cExtractTo");
			this->cExtractTo->BackgroundImage = nullptr;
			this->cExtractTo->Font = nullptr;
			this->cExtractTo->Name = L"cExtractTo";
			this->cExtractTo->UseVisualStyleBackColor = true;
			// 
			// cCompressTo
			// 
			this->cCompressTo->AccessibleDescription = nullptr;
			this->cCompressTo->AccessibleName = nullptr;
			resources->ApplyResources(this->cCompressTo, L"cCompressTo");
			this->cCompressTo->BackgroundImage = nullptr;
			this->cCompressTo->Font = nullptr;
			this->cCompressTo->Name = L"cCompressTo";
			this->cCompressTo->UseVisualStyleBackColor = true;
			// 
			// groupBox2
			// 
			this->groupBox2->AccessibleDescription = nullptr;
			this->groupBox2->AccessibleName = nullptr;
			resources->ApplyResources(this->groupBox2, L"groupBox2");
			this->groupBox2->BackgroundImage = nullptr;
			this->groupBox2->Controls->Add(this->clAssoc);
			this->groupBox2->Font = nullptr;
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->TabStop = false;
			// 
			// clAssoc
			// 
			this->clAssoc->AccessibleDescription = nullptr;
			this->clAssoc->AccessibleName = nullptr;
			resources->ApplyResources(this->clAssoc, L"clAssoc");
			this->clAssoc->BackgroundImage = nullptr;
			this->clAssoc->CheckOnClick = true;
			this->clAssoc->Font = nullptr;
			this->clAssoc->FormattingEnabled = true;
			this->clAssoc->Items->AddRange(gcnew cli::array< System::Object^  >(2) {resources->GetString(L"clAssoc.Items"), resources->GetString(L"clAssoc.Items1")});
			this->clAssoc->Name = L"clAssoc";
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
			// Options
			// 
			this->AcceptButton = this->bOK;
			this->AccessibleDescription = nullptr;
			this->AccessibleName = nullptr;
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = nullptr;
			this->CancelButton = this->bCancel;
			this->Controls->Add(this->bCancel);
			this->Controls->Add(this->bOK);
			this->Controls->Add(this->tMain);
			this->Font = nullptr;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = nullptr;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"Options";
			this->Load += gcnew System::EventHandler(this, &Options::Options_Load);
			this->tMain->ResumeLayout(false);
			this->tGeneral->ResumeLayout(false);
			this->tGeneral->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->nRecentItemsCount))->EndInit();
			this->groupBox1->ResumeLayout(false);
			this->tFileManager->ResumeLayout(false);
			this->tFileManager->PerformLayout();
			this->tShellExtension->ResumeLayout(false);
			this->tShellExtension->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void bCheckNow_Click(System::Object^  sender, System::EventArgs^  e) {
				 checkForUpdates();
			 }
private: System::Void Options_Load(System::Object^  sender, System::EventArgs^  e) {
			 cFileGroups->Enabled = LOBYTE(LOWORD(GetVersion()))*10 + HIBYTE(LOWORD(GetVersion())) >= 51; /*XP/Vista*/
		 }
};
}
