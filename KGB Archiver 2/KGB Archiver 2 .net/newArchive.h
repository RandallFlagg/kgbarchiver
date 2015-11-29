#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace Microsoft::Win32;

#include "../language.h"

namespace KGBArchiver2net {

	/// <summary>
	/// Summary for newArchive
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class newArchive : public System::Windows::Forms::Form
	{
	public:
		newArchive(void)
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
		~newArchive()
		{
			if (components)
			{
				delete components;
			}
		}


	public: System::Windows::Forms::SaveFileDialog^  dSave;
	public: System::Windows::Forms::Button^  bCancel;
	public: System::Windows::Forms::Button^  bNext;
	public: System::Windows::Forms::Label^  lMemInfo;
	public: System::Windows::Forms::Label^  lMem;
	private: System::Windows::Forms::TabControl^  tabs;
	private: System::Windows::Forms::TabPage^  tGeneral;
	public: System::Windows::Forms::GroupBox^  gOther;
	public: System::Windows::Forms::CheckBox^  cFullpath;
	public: System::Windows::Forms::GroupBox^  gSFX;
	public: System::Windows::Forms::CheckBox^  cSFX;
	public: System::Windows::Forms::GroupBox^  gPassword;
	public: System::Windows::Forms::CheckBox^  cShow;
	public: System::Windows::Forms::TextBox^  tPassword;
	public: System::Windows::Forms::GroupBox^  gLevel;
	public: System::Windows::Forms::ComboBox^  cLevel;
	public: System::Windows::Forms::GroupBox^  gMethod;
	public: System::Windows::Forms::ComboBox^  cAlgorithm;
	public: System::Windows::Forms::GroupBox^  gFormat;
	public: System::Windows::Forms::RadioButton^  rFormatZIP;
	public: System::Windows::Forms::RadioButton^  rFormatKGB;
	public: System::Windows::Forms::GroupBox^  gDestination;
	public: System::Windows::Forms::Button^  bRecent;
	public: System::Windows::Forms::Button^  bBrowse;
	public: System::Windows::Forms::TextBox^  tDestination;
	private: System::Windows::Forms::TabPage^  tFileList;
	public: System::Windows::Forms::ListView^  lFiles;
	private: System::Windows::Forms::ColumnHeader^  cFilename;
	private: System::Windows::Forms::Button^  bDelete;
	private: System::Windows::Forms::Button^  bAddFile;
	private: System::Windows::Forms::Button^  bAddDir;
	public: System::Windows::Forms::FolderBrowserDialog^  dAddDir;
	private: System::Windows::Forms::OpenFileDialog^  dAddFile;
	private: System::Windows::Forms::TabPage^  tSFX;
	private: System::Windows::Forms::GroupBox^  gSFXMsg;
	private: System::Windows::Forms::Label^  lSFXMsgType;
	private: System::Windows::Forms::TextBox^  tSFXMsg;
	private: System::Windows::Forms::RadioButton^  rSFXMsgTypeYesNo;
	private: System::Windows::Forms::RadioButton^  rMsgTypeOK;
	private: System::Windows::Forms::GroupBox^  gSFXDestination;
	private: System::Windows::Forms::TextBox^  tSFXDestination;
	private: System::Windows::Forms::GroupBox^  gSFXCommand;
	private: System::Windows::Forms::TextBox^  tSFXRunFileName;
	private: System::Windows::Forms::Label^  lSFXRunFileName;
	private: System::Windows::Forms::TextBox^  tSFXRunArg;
	private: System::Windows::Forms::Label^  lSFXRunArguments;
	private: System::Windows::Forms::ToolTip^  tSFXCommandLineHelp;
	private: System::Windows::Forms::ContextMenuStrip^  mRecent;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(newArchive::typeid));
			this->dSave = (gcnew System::Windows::Forms::SaveFileDialog());
			this->bCancel = (gcnew System::Windows::Forms::Button());
			this->bNext = (gcnew System::Windows::Forms::Button());
			this->lMemInfo = (gcnew System::Windows::Forms::Label());
			this->lMem = (gcnew System::Windows::Forms::Label());
			this->tabs = (gcnew System::Windows::Forms::TabControl());
			this->tGeneral = (gcnew System::Windows::Forms::TabPage());
			this->gOther = (gcnew System::Windows::Forms::GroupBox());
			this->cFullpath = (gcnew System::Windows::Forms::CheckBox());
			this->gSFX = (gcnew System::Windows::Forms::GroupBox());
			this->cSFX = (gcnew System::Windows::Forms::CheckBox());
			this->gPassword = (gcnew System::Windows::Forms::GroupBox());
			this->cShow = (gcnew System::Windows::Forms::CheckBox());
			this->tPassword = (gcnew System::Windows::Forms::TextBox());
			this->gLevel = (gcnew System::Windows::Forms::GroupBox());
			this->cLevel = (gcnew System::Windows::Forms::ComboBox());
			this->gMethod = (gcnew System::Windows::Forms::GroupBox());
			this->cAlgorithm = (gcnew System::Windows::Forms::ComboBox());
			this->gFormat = (gcnew System::Windows::Forms::GroupBox());
			this->rFormatZIP = (gcnew System::Windows::Forms::RadioButton());
			this->rFormatKGB = (gcnew System::Windows::Forms::RadioButton());
			this->gDestination = (gcnew System::Windows::Forms::GroupBox());
			this->bRecent = (gcnew System::Windows::Forms::Button());
			this->bBrowse = (gcnew System::Windows::Forms::Button());
			this->tDestination = (gcnew System::Windows::Forms::TextBox());
			this->tFileList = (gcnew System::Windows::Forms::TabPage());
			this->bDelete = (gcnew System::Windows::Forms::Button());
			this->bAddFile = (gcnew System::Windows::Forms::Button());
			this->bAddDir = (gcnew System::Windows::Forms::Button());
			this->lFiles = (gcnew System::Windows::Forms::ListView());
			this->cFilename = (gcnew System::Windows::Forms::ColumnHeader());
			this->tSFX = (gcnew System::Windows::Forms::TabPage());
			this->gSFXCommand = (gcnew System::Windows::Forms::GroupBox());
			this->tSFXRunArg = (gcnew System::Windows::Forms::TextBox());
			this->lSFXRunArguments = (gcnew System::Windows::Forms::Label());
			this->tSFXRunFileName = (gcnew System::Windows::Forms::TextBox());
			this->lSFXRunFileName = (gcnew System::Windows::Forms::Label());
			this->gSFXDestination = (gcnew System::Windows::Forms::GroupBox());
			this->tSFXDestination = (gcnew System::Windows::Forms::TextBox());
			this->gSFXMsg = (gcnew System::Windows::Forms::GroupBox());
			this->rSFXMsgTypeYesNo = (gcnew System::Windows::Forms::RadioButton());
			this->rMsgTypeOK = (gcnew System::Windows::Forms::RadioButton());
			this->lSFXMsgType = (gcnew System::Windows::Forms::Label());
			this->tSFXMsg = (gcnew System::Windows::Forms::TextBox());
			this->dAddDir = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->dAddFile = (gcnew System::Windows::Forms::OpenFileDialog());
			this->tSFXCommandLineHelp = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->mRecent = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->tabs->SuspendLayout();
			this->tGeneral->SuspendLayout();
			this->gOther->SuspendLayout();
			this->gSFX->SuspendLayout();
			this->gPassword->SuspendLayout();
			this->gLevel->SuspendLayout();
			this->gMethod->SuspendLayout();
			this->gFormat->SuspendLayout();
			this->gDestination->SuspendLayout();
			this->tFileList->SuspendLayout();
			this->tSFX->SuspendLayout();
			this->gSFXCommand->SuspendLayout();
			this->gSFXDestination->SuspendLayout();
			this->gSFXMsg->SuspendLayout();
			this->SuspendLayout();
			// 
			// dSave
			// 
			resources->ApplyResources(this->dSave, L"dSave");
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
			this->tSFXCommandLineHelp->SetToolTip(this->bCancel, resources->GetString(L"bCancel.ToolTip"));
			this->bCancel->UseVisualStyleBackColor = true;
			this->bCancel->Click += gcnew System::EventHandler(this, &newArchive::bCancel_Click);
			// 
			// bNext
			// 
			this->bNext->AccessibleDescription = nullptr;
			this->bNext->AccessibleName = nullptr;
			resources->ApplyResources(this->bNext, L"bNext");
			this->bNext->BackgroundImage = nullptr;
			this->bNext->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->bNext->Font = nullptr;
			this->bNext->Name = L"bNext";
			this->tSFXCommandLineHelp->SetToolTip(this->bNext, resources->GetString(L"bNext.ToolTip"));
			this->bNext->UseVisualStyleBackColor = true;
			this->bNext->Click += gcnew System::EventHandler(this, &newArchive::bNext_Click);
			// 
			// lMemInfo
			// 
			this->lMemInfo->AccessibleDescription = nullptr;
			this->lMemInfo->AccessibleName = nullptr;
			resources->ApplyResources(this->lMemInfo, L"lMemInfo");
			this->lMemInfo->Font = nullptr;
			this->lMemInfo->Name = L"lMemInfo";
			this->tSFXCommandLineHelp->SetToolTip(this->lMemInfo, resources->GetString(L"lMemInfo.ToolTip"));
			// 
			// lMem
			// 
			this->lMem->AccessibleDescription = nullptr;
			this->lMem->AccessibleName = nullptr;
			resources->ApplyResources(this->lMem, L"lMem");
			this->lMem->Font = nullptr;
			this->lMem->Name = L"lMem";
			this->tSFXCommandLineHelp->SetToolTip(this->lMem, resources->GetString(L"lMem.ToolTip"));
			// 
			// tabs
			// 
			this->tabs->AccessibleDescription = nullptr;
			this->tabs->AccessibleName = nullptr;
			resources->ApplyResources(this->tabs, L"tabs");
			this->tabs->BackgroundImage = nullptr;
			this->tabs->Controls->Add(this->tGeneral);
			this->tabs->Controls->Add(this->tFileList);
			this->tabs->Controls->Add(this->tSFX);
			this->tabs->Font = nullptr;
			this->tabs->Name = L"tabs";
			this->tabs->SelectedIndex = 0;
			this->tSFXCommandLineHelp->SetToolTip(this->tabs, resources->GetString(L"tabs.ToolTip"));
			// 
			// tGeneral
			// 
			this->tGeneral->AccessibleDescription = nullptr;
			this->tGeneral->AccessibleName = nullptr;
			resources->ApplyResources(this->tGeneral, L"tGeneral");
			this->tGeneral->BackgroundImage = nullptr;
			this->tGeneral->Controls->Add(this->gOther);
			this->tGeneral->Controls->Add(this->gSFX);
			this->tGeneral->Controls->Add(this->gPassword);
			this->tGeneral->Controls->Add(this->gLevel);
			this->tGeneral->Controls->Add(this->gMethod);
			this->tGeneral->Controls->Add(this->gFormat);
			this->tGeneral->Controls->Add(this->gDestination);
			this->tGeneral->Font = nullptr;
			this->tGeneral->Name = L"tGeneral";
			this->tSFXCommandLineHelp->SetToolTip(this->tGeneral, resources->GetString(L"tGeneral.ToolTip"));
			this->tGeneral->UseVisualStyleBackColor = true;
			// 
			// gOther
			// 
			this->gOther->AccessibleDescription = nullptr;
			this->gOther->AccessibleName = nullptr;
			resources->ApplyResources(this->gOther, L"gOther");
			this->gOther->BackgroundImage = nullptr;
			this->gOther->Controls->Add(this->cFullpath);
			this->gOther->Font = nullptr;
			this->gOther->Name = L"gOther";
			this->gOther->TabStop = false;
			this->tSFXCommandLineHelp->SetToolTip(this->gOther, resources->GetString(L"gOther.ToolTip"));
			// 
			// cFullpath
			// 
			this->cFullpath->AccessibleDescription = nullptr;
			this->cFullpath->AccessibleName = nullptr;
			resources->ApplyResources(this->cFullpath, L"cFullpath");
			this->cFullpath->BackgroundImage = nullptr;
			this->cFullpath->Font = nullptr;
			this->cFullpath->Name = L"cFullpath";
			this->tSFXCommandLineHelp->SetToolTip(this->cFullpath, resources->GetString(L"cFullpath.ToolTip"));
			this->cFullpath->UseVisualStyleBackColor = true;
			// 
			// gSFX
			// 
			this->gSFX->AccessibleDescription = nullptr;
			this->gSFX->AccessibleName = nullptr;
			resources->ApplyResources(this->gSFX, L"gSFX");
			this->gSFX->BackgroundImage = nullptr;
			this->gSFX->Controls->Add(this->cSFX);
			this->gSFX->Font = nullptr;
			this->gSFX->Name = L"gSFX";
			this->gSFX->TabStop = false;
			this->tSFXCommandLineHelp->SetToolTip(this->gSFX, resources->GetString(L"gSFX.ToolTip"));
			// 
			// cSFX
			// 
			this->cSFX->AccessibleDescription = nullptr;
			this->cSFX->AccessibleName = nullptr;
			resources->ApplyResources(this->cSFX, L"cSFX");
			this->cSFX->BackgroundImage = nullptr;
			this->cSFX->Font = nullptr;
			this->cSFX->Name = L"cSFX";
			this->tSFXCommandLineHelp->SetToolTip(this->cSFX, resources->GetString(L"cSFX.ToolTip"));
			this->cSFX->UseVisualStyleBackColor = true;
			this->cSFX->CheckedChanged += gcnew System::EventHandler(this, &newArchive::rFormatKGB_CheckedChanged);
			// 
			// gPassword
			// 
			this->gPassword->AccessibleDescription = nullptr;
			this->gPassword->AccessibleName = nullptr;
			resources->ApplyResources(this->gPassword, L"gPassword");
			this->gPassword->BackgroundImage = nullptr;
			this->gPassword->Controls->Add(this->cShow);
			this->gPassword->Controls->Add(this->tPassword);
			this->gPassword->Font = nullptr;
			this->gPassword->Name = L"gPassword";
			this->gPassword->TabStop = false;
			this->tSFXCommandLineHelp->SetToolTip(this->gPassword, resources->GetString(L"gPassword.ToolTip"));
			// 
			// cShow
			// 
			this->cShow->AccessibleDescription = nullptr;
			this->cShow->AccessibleName = nullptr;
			resources->ApplyResources(this->cShow, L"cShow");
			this->cShow->BackgroundImage = nullptr;
			this->cShow->Font = nullptr;
			this->cShow->Name = L"cShow";
			this->tSFXCommandLineHelp->SetToolTip(this->cShow, resources->GetString(L"cShow.ToolTip"));
			this->cShow->UseVisualStyleBackColor = true;
			this->cShow->CheckedChanged += gcnew System::EventHandler(this, &newArchive::cShow_CheckedChanged);
			// 
			// tPassword
			// 
			this->tPassword->AccessibleDescription = nullptr;
			this->tPassword->AccessibleName = nullptr;
			resources->ApplyResources(this->tPassword, L"tPassword");
			this->tPassword->BackgroundImage = nullptr;
			this->tPassword->Font = nullptr;
			this->tPassword->Name = L"tPassword";
			this->tSFXCommandLineHelp->SetToolTip(this->tPassword, resources->GetString(L"tPassword.ToolTip"));
			this->tPassword->UseSystemPasswordChar = true;
			// 
			// gLevel
			// 
			this->gLevel->AccessibleDescription = nullptr;
			this->gLevel->AccessibleName = nullptr;
			resources->ApplyResources(this->gLevel, L"gLevel");
			this->gLevel->BackgroundImage = nullptr;
			this->gLevel->Controls->Add(this->cLevel);
			this->gLevel->Font = nullptr;
			this->gLevel->Name = L"gLevel";
			this->gLevel->TabStop = false;
			this->tSFXCommandLineHelp->SetToolTip(this->gLevel, resources->GetString(L"gLevel.ToolTip"));
			// 
			// cLevel
			// 
			this->cLevel->AccessibleDescription = nullptr;
			this->cLevel->AccessibleName = nullptr;
			resources->ApplyResources(this->cLevel, L"cLevel");
			this->cLevel->BackgroundImage = nullptr;
			this->cLevel->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cLevel->Font = nullptr;
			this->cLevel->FormattingEnabled = true;
			this->cLevel->Items->AddRange(gcnew cli::array< System::Object^  >(10) {resources->GetString(L"cLevel.Items"), resources->GetString(L"cLevel.Items1"), 
				resources->GetString(L"cLevel.Items2"), resources->GetString(L"cLevel.Items3"), resources->GetString(L"cLevel.Items4"), resources->GetString(L"cLevel.Items5"), 
				resources->GetString(L"cLevel.Items6"), resources->GetString(L"cLevel.Items7"), resources->GetString(L"cLevel.Items8"), resources->GetString(L"cLevel.Items9")});
			this->cLevel->Name = L"cLevel";
			this->tSFXCommandLineHelp->SetToolTip(this->cLevel, resources->GetString(L"cLevel.ToolTip"));
			this->cLevel->SelectedIndexChanged += gcnew System::EventHandler(this, &newArchive::cLevel_SelectedIndexChanged);
			// 
			// gMethod
			// 
			this->gMethod->AccessibleDescription = nullptr;
			this->gMethod->AccessibleName = nullptr;
			resources->ApplyResources(this->gMethod, L"gMethod");
			this->gMethod->BackgroundImage = nullptr;
			this->gMethod->Controls->Add(this->cAlgorithm);
			this->gMethod->Font = nullptr;
			this->gMethod->Name = L"gMethod";
			this->gMethod->TabStop = false;
			this->tSFXCommandLineHelp->SetToolTip(this->gMethod, resources->GetString(L"gMethod.ToolTip"));
			// 
			// cAlgorithm
			// 
			this->cAlgorithm->AccessibleDescription = nullptr;
			this->cAlgorithm->AccessibleName = nullptr;
			resources->ApplyResources(this->cAlgorithm, L"cAlgorithm");
			this->cAlgorithm->BackgroundImage = nullptr;
			this->cAlgorithm->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cAlgorithm->Font = nullptr;
			this->cAlgorithm->FormattingEnabled = true;
			this->cAlgorithm->Items->AddRange(gcnew cli::array< System::Object^  >(8) {resources->GetString(L"cAlgorithm.Items"), resources->GetString(L"cAlgorithm.Items1"), 
				resources->GetString(L"cAlgorithm.Items2"), resources->GetString(L"cAlgorithm.Items3"), resources->GetString(L"cAlgorithm.Items4"), 
				resources->GetString(L"cAlgorithm.Items5"), resources->GetString(L"cAlgorithm.Items6"), resources->GetString(L"cAlgorithm.Items7")});
			this->cAlgorithm->Name = L"cAlgorithm";
			this->tSFXCommandLineHelp->SetToolTip(this->cAlgorithm, resources->GetString(L"cAlgorithm.ToolTip"));
			this->cAlgorithm->SelectedIndexChanged += gcnew System::EventHandler(this, &newArchive::cAlgorithm_SelectedIndexChanged);
			// 
			// gFormat
			// 
			this->gFormat->AccessibleDescription = nullptr;
			this->gFormat->AccessibleName = nullptr;
			resources->ApplyResources(this->gFormat, L"gFormat");
			this->gFormat->BackgroundImage = nullptr;
			this->gFormat->Controls->Add(this->rFormatZIP);
			this->gFormat->Controls->Add(this->rFormatKGB);
			this->gFormat->Font = nullptr;
			this->gFormat->Name = L"gFormat";
			this->gFormat->TabStop = false;
			this->tSFXCommandLineHelp->SetToolTip(this->gFormat, resources->GetString(L"gFormat.ToolTip"));
			// 
			// rFormatZIP
			// 
			this->rFormatZIP->AccessibleDescription = nullptr;
			this->rFormatZIP->AccessibleName = nullptr;
			resources->ApplyResources(this->rFormatZIP, L"rFormatZIP");
			this->rFormatZIP->BackgroundImage = nullptr;
			this->rFormatZIP->Font = nullptr;
			this->rFormatZIP->Name = L"rFormatZIP";
			this->tSFXCommandLineHelp->SetToolTip(this->rFormatZIP, resources->GetString(L"rFormatZIP.ToolTip"));
			this->rFormatZIP->UseVisualStyleBackColor = true;
			this->rFormatZIP->CheckedChanged += gcnew System::EventHandler(this, &newArchive::rFormatKGB_CheckedChanged);
			// 
			// rFormatKGB
			// 
			this->rFormatKGB->AccessibleDescription = nullptr;
			this->rFormatKGB->AccessibleName = nullptr;
			resources->ApplyResources(this->rFormatKGB, L"rFormatKGB");
			this->rFormatKGB->BackgroundImage = nullptr;
			this->rFormatKGB->Checked = true;
			this->rFormatKGB->Font = nullptr;
			this->rFormatKGB->Name = L"rFormatKGB";
			this->rFormatKGB->TabStop = true;
			this->tSFXCommandLineHelp->SetToolTip(this->rFormatKGB, resources->GetString(L"rFormatKGB.ToolTip"));
			this->rFormatKGB->UseVisualStyleBackColor = true;
			this->rFormatKGB->CheckedChanged += gcnew System::EventHandler(this, &newArchive::rFormatKGB_CheckedChanged);
			// 
			// gDestination
			// 
			this->gDestination->AccessibleDescription = nullptr;
			this->gDestination->AccessibleName = nullptr;
			resources->ApplyResources(this->gDestination, L"gDestination");
			this->gDestination->BackgroundImage = nullptr;
			this->gDestination->Controls->Add(this->bRecent);
			this->gDestination->Controls->Add(this->bBrowse);
			this->gDestination->Controls->Add(this->tDestination);
			this->gDestination->Font = nullptr;
			this->gDestination->Name = L"gDestination";
			this->gDestination->TabStop = false;
			this->tSFXCommandLineHelp->SetToolTip(this->gDestination, resources->GetString(L"gDestination.ToolTip"));
			// 
			// bRecent
			// 
			this->bRecent->AccessibleDescription = nullptr;
			this->bRecent->AccessibleName = nullptr;
			resources->ApplyResources(this->bRecent, L"bRecent");
			this->bRecent->BackgroundImage = nullptr;
			this->bRecent->Font = nullptr;
			this->bRecent->Name = L"bRecent";
			this->tSFXCommandLineHelp->SetToolTip(this->bRecent, resources->GetString(L"bRecent.ToolTip"));
			this->bRecent->UseVisualStyleBackColor = true;
			this->bRecent->Click += gcnew System::EventHandler(this, &newArchive::bRecent_Click);
			// 
			// bBrowse
			// 
			this->bBrowse->AccessibleDescription = nullptr;
			this->bBrowse->AccessibleName = nullptr;
			resources->ApplyResources(this->bBrowse, L"bBrowse");
			this->bBrowse->BackgroundImage = nullptr;
			this->bBrowse->Font = nullptr;
			this->bBrowse->Name = L"bBrowse";
			this->tSFXCommandLineHelp->SetToolTip(this->bBrowse, resources->GetString(L"bBrowse.ToolTip"));
			this->bBrowse->UseVisualStyleBackColor = true;
			this->bBrowse->Click += gcnew System::EventHandler(this, &newArchive::bBrowse_Click);
			// 
			// tDestination
			// 
			this->tDestination->AccessibleDescription = nullptr;
			this->tDestination->AccessibleName = nullptr;
			resources->ApplyResources(this->tDestination, L"tDestination");
			this->tDestination->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Suggest;
			this->tDestination->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::FileSystem;
			this->tDestination->BackgroundImage = nullptr;
			this->tDestination->Font = nullptr;
			this->tDestination->Name = L"tDestination";
			this->tSFXCommandLineHelp->SetToolTip(this->tDestination, resources->GetString(L"tDestination.ToolTip"));
			this->tDestination->TextChanged += gcnew System::EventHandler(this, &newArchive::tDestination_TextChanged);
			// 
			// tFileList
			// 
			this->tFileList->AccessibleDescription = nullptr;
			this->tFileList->AccessibleName = nullptr;
			resources->ApplyResources(this->tFileList, L"tFileList");
			this->tFileList->BackgroundImage = nullptr;
			this->tFileList->Controls->Add(this->bDelete);
			this->tFileList->Controls->Add(this->bAddFile);
			this->tFileList->Controls->Add(this->bAddDir);
			this->tFileList->Controls->Add(this->lFiles);
			this->tFileList->Font = nullptr;
			this->tFileList->Name = L"tFileList";
			this->tSFXCommandLineHelp->SetToolTip(this->tFileList, resources->GetString(L"tFileList.ToolTip"));
			this->tFileList->UseVisualStyleBackColor = true;
			// 
			// bDelete
			// 
			this->bDelete->AccessibleDescription = nullptr;
			this->bDelete->AccessibleName = nullptr;
			resources->ApplyResources(this->bDelete, L"bDelete");
			this->bDelete->BackgroundImage = nullptr;
			this->bDelete->Font = nullptr;
			this->bDelete->Name = L"bDelete";
			this->tSFXCommandLineHelp->SetToolTip(this->bDelete, resources->GetString(L"bDelete.ToolTip"));
			this->bDelete->UseVisualStyleBackColor = true;
			this->bDelete->Click += gcnew System::EventHandler(this, &newArchive::bDelete_Click);
			// 
			// bAddFile
			// 
			this->bAddFile->AccessibleDescription = nullptr;
			this->bAddFile->AccessibleName = nullptr;
			resources->ApplyResources(this->bAddFile, L"bAddFile");
			this->bAddFile->BackgroundImage = nullptr;
			this->bAddFile->Font = nullptr;
			this->bAddFile->Name = L"bAddFile";
			this->tSFXCommandLineHelp->SetToolTip(this->bAddFile, resources->GetString(L"bAddFile.ToolTip"));
			this->bAddFile->UseVisualStyleBackColor = true;
			this->bAddFile->Click += gcnew System::EventHandler(this, &newArchive::bAddFile_Click);
			// 
			// bAddDir
			// 
			this->bAddDir->AccessibleDescription = nullptr;
			this->bAddDir->AccessibleName = nullptr;
			resources->ApplyResources(this->bAddDir, L"bAddDir");
			this->bAddDir->BackgroundImage = nullptr;
			this->bAddDir->Font = nullptr;
			this->bAddDir->Name = L"bAddDir";
			this->tSFXCommandLineHelp->SetToolTip(this->bAddDir, resources->GetString(L"bAddDir.ToolTip"));
			this->bAddDir->UseVisualStyleBackColor = true;
			this->bAddDir->Click += gcnew System::EventHandler(this, &newArchive::bAddDir_Click);
			// 
			// lFiles
			// 
			this->lFiles->AccessibleDescription = nullptr;
			this->lFiles->AccessibleName = nullptr;
			resources->ApplyResources(this->lFiles, L"lFiles");
			this->lFiles->BackgroundImage = nullptr;
			this->lFiles->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(1) {this->cFilename});
			this->lFiles->Font = nullptr;
			this->lFiles->LabelEdit = true;
			this->lFiles->Name = L"lFiles";
			this->tSFXCommandLineHelp->SetToolTip(this->lFiles, resources->GetString(L"lFiles.ToolTip"));
			this->lFiles->UseCompatibleStateImageBehavior = false;
			this->lFiles->View = System::Windows::Forms::View::Details;
			this->lFiles->ItemSelectionChanged += gcnew System::Windows::Forms::ListViewItemSelectionChangedEventHandler(this, &newArchive::lFiles_ItemSelectionChanged);
			// 
			// cFilename
			// 
			resources->ApplyResources(this->cFilename, L"cFilename");
			// 
			// tSFX
			// 
			this->tSFX->AccessibleDescription = nullptr;
			this->tSFX->AccessibleName = nullptr;
			resources->ApplyResources(this->tSFX, L"tSFX");
			this->tSFX->BackgroundImage = nullptr;
			this->tSFX->Controls->Add(this->gSFXCommand);
			this->tSFX->Controls->Add(this->gSFXDestination);
			this->tSFX->Controls->Add(this->gSFXMsg);
			this->tSFX->Font = nullptr;
			this->tSFX->Name = L"tSFX";
			this->tSFXCommandLineHelp->SetToolTip(this->tSFX, resources->GetString(L"tSFX.ToolTip"));
			this->tSFX->UseVisualStyleBackColor = true;
			// 
			// gSFXCommand
			// 
			this->gSFXCommand->AccessibleDescription = nullptr;
			this->gSFXCommand->AccessibleName = nullptr;
			resources->ApplyResources(this->gSFXCommand, L"gSFXCommand");
			this->gSFXCommand->BackgroundImage = nullptr;
			this->gSFXCommand->Controls->Add(this->tSFXRunArg);
			this->gSFXCommand->Controls->Add(this->lSFXRunArguments);
			this->gSFXCommand->Controls->Add(this->tSFXRunFileName);
			this->gSFXCommand->Controls->Add(this->lSFXRunFileName);
			this->gSFXCommand->Font = nullptr;
			this->gSFXCommand->Name = L"gSFXCommand";
			this->gSFXCommand->TabStop = false;
			this->tSFXCommandLineHelp->SetToolTip(this->gSFXCommand, resources->GetString(L"gSFXCommand.ToolTip"));
			// 
			// tSFXRunArg
			// 
			this->tSFXRunArg->AccessibleDescription = nullptr;
			this->tSFXRunArg->AccessibleName = nullptr;
			resources->ApplyResources(this->tSFXRunArg, L"tSFXRunArg");
			this->tSFXRunArg->BackgroundImage = nullptr;
			this->tSFXRunArg->Font = nullptr;
			this->tSFXRunArg->Name = L"tSFXRunArg";
			this->tSFXCommandLineHelp->SetToolTip(this->tSFXRunArg, resources->GetString(L"tSFXRunArg.ToolTip"));
			// 
			// lSFXRunArguments
			// 
			this->lSFXRunArguments->AccessibleDescription = nullptr;
			this->lSFXRunArguments->AccessibleName = nullptr;
			resources->ApplyResources(this->lSFXRunArguments, L"lSFXRunArguments");
			this->lSFXRunArguments->Font = nullptr;
			this->lSFXRunArguments->Name = L"lSFXRunArguments";
			this->tSFXCommandLineHelp->SetToolTip(this->lSFXRunArguments, resources->GetString(L"lSFXRunArguments.ToolTip"));
			// 
			// tSFXRunFileName
			// 
			this->tSFXRunFileName->AccessibleDescription = nullptr;
			this->tSFXRunFileName->AccessibleName = nullptr;
			resources->ApplyResources(this->tSFXRunFileName, L"tSFXRunFileName");
			this->tSFXRunFileName->BackgroundImage = nullptr;
			this->tSFXRunFileName->Font = nullptr;
			this->tSFXRunFileName->Name = L"tSFXRunFileName";
			this->tSFXCommandLineHelp->SetToolTip(this->tSFXRunFileName, resources->GetString(L"tSFXRunFileName.ToolTip"));
			// 
			// lSFXRunFileName
			// 
			this->lSFXRunFileName->AccessibleDescription = nullptr;
			this->lSFXRunFileName->AccessibleName = nullptr;
			resources->ApplyResources(this->lSFXRunFileName, L"lSFXRunFileName");
			this->lSFXRunFileName->Font = nullptr;
			this->lSFXRunFileName->Name = L"lSFXRunFileName";
			this->tSFXCommandLineHelp->SetToolTip(this->lSFXRunFileName, resources->GetString(L"lSFXRunFileName.ToolTip"));
			// 
			// gSFXDestination
			// 
			this->gSFXDestination->AccessibleDescription = nullptr;
			this->gSFXDestination->AccessibleName = nullptr;
			resources->ApplyResources(this->gSFXDestination, L"gSFXDestination");
			this->gSFXDestination->BackgroundImage = nullptr;
			this->gSFXDestination->Controls->Add(this->tSFXDestination);
			this->gSFXDestination->Font = nullptr;
			this->gSFXDestination->Name = L"gSFXDestination";
			this->gSFXDestination->TabStop = false;
			this->tSFXCommandLineHelp->SetToolTip(this->gSFXDestination, resources->GetString(L"gSFXDestination.ToolTip"));
			// 
			// tSFXDestination
			// 
			this->tSFXDestination->AccessibleDescription = nullptr;
			this->tSFXDestination->AccessibleName = nullptr;
			resources->ApplyResources(this->tSFXDestination, L"tSFXDestination");
			this->tSFXDestination->BackgroundImage = nullptr;
			this->tSFXDestination->Font = nullptr;
			this->tSFXDestination->Name = L"tSFXDestination";
			this->tSFXCommandLineHelp->SetToolTip(this->tSFXDestination, resources->GetString(L"tSFXDestination.ToolTip"));
			// 
			// gSFXMsg
			// 
			this->gSFXMsg->AccessibleDescription = nullptr;
			this->gSFXMsg->AccessibleName = nullptr;
			resources->ApplyResources(this->gSFXMsg, L"gSFXMsg");
			this->gSFXMsg->BackgroundImage = nullptr;
			this->gSFXMsg->Controls->Add(this->rSFXMsgTypeYesNo);
			this->gSFXMsg->Controls->Add(this->rMsgTypeOK);
			this->gSFXMsg->Controls->Add(this->lSFXMsgType);
			this->gSFXMsg->Controls->Add(this->tSFXMsg);
			this->gSFXMsg->Font = nullptr;
			this->gSFXMsg->Name = L"gSFXMsg";
			this->gSFXMsg->TabStop = false;
			this->tSFXCommandLineHelp->SetToolTip(this->gSFXMsg, resources->GetString(L"gSFXMsg.ToolTip"));
			// 
			// rSFXMsgTypeYesNo
			// 
			this->rSFXMsgTypeYesNo->AccessibleDescription = nullptr;
			this->rSFXMsgTypeYesNo->AccessibleName = nullptr;
			resources->ApplyResources(this->rSFXMsgTypeYesNo, L"rSFXMsgTypeYesNo");
			this->rSFXMsgTypeYesNo->BackgroundImage = nullptr;
			this->rSFXMsgTypeYesNo->Font = nullptr;
			this->rSFXMsgTypeYesNo->Name = L"rSFXMsgTypeYesNo";
			this->tSFXCommandLineHelp->SetToolTip(this->rSFXMsgTypeYesNo, resources->GetString(L"rSFXMsgTypeYesNo.ToolTip"));
			this->rSFXMsgTypeYesNo->UseVisualStyleBackColor = true;
			// 
			// rMsgTypeOK
			// 
			this->rMsgTypeOK->AccessibleDescription = nullptr;
			this->rMsgTypeOK->AccessibleName = nullptr;
			resources->ApplyResources(this->rMsgTypeOK, L"rMsgTypeOK");
			this->rMsgTypeOK->BackgroundImage = nullptr;
			this->rMsgTypeOK->Checked = true;
			this->rMsgTypeOK->Font = nullptr;
			this->rMsgTypeOK->Name = L"rMsgTypeOK";
			this->rMsgTypeOK->TabStop = true;
			this->tSFXCommandLineHelp->SetToolTip(this->rMsgTypeOK, resources->GetString(L"rMsgTypeOK.ToolTip"));
			this->rMsgTypeOK->UseVisualStyleBackColor = true;
			// 
			// lSFXMsgType
			// 
			this->lSFXMsgType->AccessibleDescription = nullptr;
			this->lSFXMsgType->AccessibleName = nullptr;
			resources->ApplyResources(this->lSFXMsgType, L"lSFXMsgType");
			this->lSFXMsgType->Font = nullptr;
			this->lSFXMsgType->Name = L"lSFXMsgType";
			this->tSFXCommandLineHelp->SetToolTip(this->lSFXMsgType, resources->GetString(L"lSFXMsgType.ToolTip"));
			// 
			// tSFXMsg
			// 
			this->tSFXMsg->AccessibleDescription = nullptr;
			this->tSFXMsg->AccessibleName = nullptr;
			resources->ApplyResources(this->tSFXMsg, L"tSFXMsg");
			this->tSFXMsg->BackgroundImage = nullptr;
			this->tSFXMsg->Font = nullptr;
			this->tSFXMsg->Name = L"tSFXMsg";
			this->tSFXCommandLineHelp->SetToolTip(this->tSFXMsg, resources->GetString(L"tSFXMsg.ToolTip"));
			// 
			// dAddDir
			// 
			resources->ApplyResources(this->dAddDir, L"dAddDir");
			this->dAddDir->ShowNewFolderButton = false;
			// 
			// dAddFile
			// 
			resources->ApplyResources(this->dAddFile, L"dAddFile");
			this->dAddFile->Multiselect = true;
			// 
			// tSFXCommandLineHelp
			// 
			this->tSFXCommandLineHelp->AutoPopDelay = 5000;
			this->tSFXCommandLineHelp->InitialDelay = 0;
			this->tSFXCommandLineHelp->IsBalloon = true;
			this->tSFXCommandLineHelp->ReshowDelay = 100;
			this->tSFXCommandLineHelp->ToolTipIcon = System::Windows::Forms::ToolTipIcon::Info;
			this->tSFXCommandLineHelp->ToolTipTitle = L"Specjalne katalogi";
			// 
			// mRecent
			// 
			this->mRecent->AccessibleDescription = nullptr;
			this->mRecent->AccessibleName = nullptr;
			resources->ApplyResources(this->mRecent, L"mRecent");
			this->mRecent->BackgroundImage = nullptr;
			this->mRecent->Font = nullptr;
			this->mRecent->Name = L"mRecent";
			this->tSFXCommandLineHelp->SetToolTip(this->mRecent, resources->GetString(L"mRecent.ToolTip"));
			this->mRecent->ItemClicked += gcnew System::Windows::Forms::ToolStripItemClickedEventHandler(this, &newArchive::mRecent_ItemClicked);
			// 
			// newArchive
			// 
			this->AcceptButton = this->bNext;
			this->AccessibleDescription = nullptr;
			this->AccessibleName = nullptr;
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = nullptr;
			this->CancelButton = this->bCancel;
			this->Controls->Add(this->lMem);
			this->Controls->Add(this->lMemInfo);
			this->Controls->Add(this->tabs);
			this->Controls->Add(this->bNext);
			this->Controls->Add(this->bCancel);
			this->Font = nullptr;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = nullptr;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"newArchive";
			this->tSFXCommandLineHelp->SetToolTip(this, resources->GetString(L"$this.ToolTip"));
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &newArchive::newArchive_FormClosing);
			this->Load += gcnew System::EventHandler(this, &newArchive::newArchive_Load);
			this->tabs->ResumeLayout(false);
			this->tGeneral->ResumeLayout(false);
			this->gOther->ResumeLayout(false);
			this->gOther->PerformLayout();
			this->gSFX->ResumeLayout(false);
			this->gSFX->PerformLayout();
			this->gPassword->ResumeLayout(false);
			this->gPassword->PerformLayout();
			this->gLevel->ResumeLayout(false);
			this->gMethod->ResumeLayout(false);
			this->gFormat->ResumeLayout(false);
			this->gFormat->PerformLayout();
			this->gDestination->ResumeLayout(false);
			this->gDestination->PerformLayout();
			this->tFileList->ResumeLayout(false);
			this->tSFX->ResumeLayout(false);
			this->gSFXCommand->ResumeLayout(false);
			this->gSFXCommand->PerformLayout();
			this->gSFXDestination->ResumeLayout(false);
			this->gSFXDestination->PerformLayout();
			this->gSFXMsg->ResumeLayout(false);
			this->gSFXMsg->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	public: String ^curDir;
	private: System::Void rFormatKGB_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				 try{
					 if(cSFX->Checked){
						tDestination->Text = tDestination->Text->Substring(0, tDestination->Text->Length-4)+gcnew String(".exe");
					 }else if(rFormatKGB->Checked){
						tDestination->Text = tDestination->Text->Substring(0, tDestination->Text->Length-4)+gcnew String(".kgb");
					 }else
						tDestination->Text = tDestination->Text->Substring(0, tDestination->Text->Length-4)+gcnew String(".zip");
				 }catch(...){}
				 cAlgorithm->Enabled = rFormatKGB->Checked;
				 cAlgorithm_SelectedIndexChanged(NULL, System::EventArgs::Empty);
				 countMem();
				 tSFX->Enabled = cSFX->Checked;
			 }
private: System::Void cAlgorithm_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 if(cAlgorithm->SelectedIndex <= 5 && rFormatKGB->Checked)
				 cLevel->Enabled = false;
			 else
				 cLevel->Enabled = true;
			 countMem();
		 }
private: System::Void newArchive_Load(System::Object^  sender, System::EventArgs^  e) {
			 //cAlgorithm->SelectedIndex = 7;
			 //cLevel->SelectedIndex = 3;
			 tSFX->Enabled = false;
			 tSFX->Visible = false;

			 try{
				 cAlgorithm->SelectedIndex = (int)Registry::GetValue(L"HKEY_CURRENT_USER\\Software\\KGB Archiver 2\\NewArchive", L"algorithm", 7);
			 }catch(Exception ^){
				 cAlgorithm->SelectedIndex = 7;
			 }
			 try{
				 cLevel->SelectedIndex = (int)Registry::GetValue(L"HKEY_CURRENT_USER\\Software\\KGB Archiver 2\\NewArchive", L"level", 3);
			 }catch(Exception ^){
				 cLevel->SelectedIndex = 3;
			 }
		 }
private: System::Void bBrowse_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(dSave->ShowDialog() == Windows::Forms::DialogResult::OK)
				 tDestination->Text = dSave->FileName;
		 }
private: System::Void tDestination_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 //MessageBox::Show(tDestination->Text->Substring(tDestination->Text->Length-4, 4)->ToLower());
			 try{
				 if(tDestination->Text->Substring(tDestination->Text->Length-4, 4)->ToLower() == gcnew String(".kgb")){
					 rFormatKGB->Checked = true;
					 cSFX->Checked = false;
				 }else if(tDestination->Text->Substring(tDestination->Text->Length-4, 4)->ToLower() == gcnew String(".zip")){
					 rFormatZIP->Checked = true;
					 cSFX->Checked = false;
				 }else if(tDestination->Text->Substring(tDestination->Text->Length-4, 4)->ToLower() == gcnew String(".zip")){
					 cSFX->Checked = true;
				 }
			 }catch(...){}
		 }
private: System::Void cShow_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 tPassword->UseSystemPasswordChar = !cShow->Checked;
		 }

private: System::Void countMem(){
			 if(rFormatZIP->Checked){
				lMem->Text = "17MB";
				return;
			 }
			 switch(cAlgorithm->SelectedIndex){
				case 0: lMem->Text = "12MB"; break;
				case 1: lMem->Text = "58MB"; break;
				case 2: lMem->Text = "60MB"; break;
				case 3: lMem->Text = "201MB"; break;
				case 4: lMem->Text = "344MB"; break;
				case 5: lMem->Text = "367MB"; break;
				case 6: 
					switch(cLevel->SelectedIndex){
						case 0: lMem->Text = "13MB"; break;
						case 1: lMem->Text = "15MB"; break;
						case 2: lMem->Text = "19MB"; break;
						case 3: lMem->Text = "31MB"; break;
						case 4: lMem->Text = "70MB"; break;
						case 5: lMem->Text = "158MB"; break;
						case 6: lMem->Text = "206MB"; break;
						case 7: lMem->Text = "406MB"; break;
						case 8: lMem->Text = "794MB"; break;
						case 9: lMem->Text = "1604MB"; break;
					}
					break;
				case 7: 
					switch(cLevel->SelectedIndex){
						case 0: lMem->Text = "31MB"; break;
						case 1: lMem->Text = "33MB"; break;
						case 2: lMem->Text = "37MB"; break;
						case 3: lMem->Text = "44MB"; break;
						case 4: lMem->Text = "59MB"; break;
						case 5: lMem->Text = "89MB"; break;
						case 6: lMem->Text = "145MB"; break;
						case 7: lMem->Text = "261MB"; break;
						case 8: lMem->Text = "492MB"; break;
						case 9: lMem->Text = "949MB"; break;
					}
					break;
			 }
		 }
private: System::Void cLevel_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 countMem();
		 }

private: System::Void newArchive_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
			 if(this->DialogResult == Windows::Forms::DialogResult::OK){			
				if(tDestination->Text->Length < 4){
					 MessageBox::Show(getString("ProvideArchName"), L"KGB Archiver");
					 e->Cancel = true;
					 tabs->SelectedTab = tGeneral;
					 tDestination->Focus();
					 return;
				 }
				 if(lFiles->Items->Count == 0){
					 MessageBox::Show(getString("SelectFilesCompress"), L"KGB Archiver");
					 tabs->SelectedTab = tFileList;
					 e->Cancel = true;
					 return;
				 }

				 MEMORYSTATUSEX RAM;
				 unsigned int neededMem = 17; //for ZIP
				 unsigned int kgbMem[8][10] = {{12}, {58}, {60}, {201}, {344}, {367}, 
									{13, 15, 19, 31, 70, 158, 206, 406, 794, 1604}, 
									{31, 33, 37, 44, 59, 89, 145, 261, 492, 949}};

				 RAM.dwLength = sizeof(RAM);

				 if(rFormatKGB->Checked){
					 if(cAlgorithm->SelectedIndex < 6)
						 neededMem = kgbMem[cAlgorithm->SelectedIndex][0];
					 else
						 neededMem = kgbMem[cAlgorithm->SelectedIndex][cLevel->SelectedIndex];
				 }

				 GlobalMemoryStatusEx(&RAM);

				 if(RAM.ullAvailPageFile < (neededMem<<20)){
					 MessageBox::Show(getString("NotEnoughMem"), L"KGB Archiver 2", MessageBoxButtons::OK, MessageBoxIcon::Stop);
					 e->Cancel = true;
					 return;
				 }else if(RAM.ullTotalPhys *3/4 < (neededMem<<20)){
					 if(MessageBox::Show(getString("CompressionCauseSystemSlowdown"), L"KGB Archiver 2", MessageBoxButtons::YesNo, MessageBoxIcon::Warning) == Windows::Forms::DialogResult::No){
						 e->Cancel = true;
						 return;
					 }
				 }
				 
				 String ^hook = gcnew String(L"HKEY_CURRENT_USER\\Software\\KGB Archiver 2\\recentArchives");
				 String ^num, ^num2;
				 int i;

				 try{
					  for(i = 0;i<settings::general::recentItemsCount;++i){
						 num = String::Format(L"{0}", i);
						 
						 if(tDestination->Text == (String^)Registry::GetValue(hook, num, L"")){
							delete num;
							break;
						 }
						 
						 delete num;
					 }
				 }catch(Exception ^){}

				 try{
					 for(--i;i>=0;--i){
						 num = String::Format(L"{0}", i);
						 num2 = String::Format(L"{0}", i+1);
						 
						 Registry::SetValue(hook, num2, Registry::GetValue(hook, num, L""));
						 
						 delete num;
						 delete num2;
					 }
				 }catch(Exception ^){}
				 try{
					 Registry::SetValue(hook, L"0", tDestination->Text); 
				 }catch(Exception ^){}

				 delete hook;

				 try{
					 Registry::SetValue(L"HKEY_CURRENT_USER\\Software\\KGB Archiver 2\\NewArchive", L"algorithm", cAlgorithm->SelectedIndex);
					 Registry::SetValue(L"HKEY_CURRENT_USER\\Software\\KGB Archiver 2\\NewArchive", L"level", cLevel->SelectedIndex);
				 }catch(Exception ^){

				 }
			 }
			 //Close();
		 }
private: System::Void bNext_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->DialogResult = Windows::Forms::DialogResult::OK;
		 }
private: System::Void bCancel_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->DialogResult = Windows::Forms::DialogResult::Cancel;
		 }
private: System::Void bAddDir_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(dAddDir->ShowDialog() == Windows::Forms::DialogResult::OK){
				 String ^dir = dAddDir->SelectedPath + L"/";
				 if(dir->Length > curDir->Length){
					 if(dir->Substring(0, curDir->Length-1) == curDir->Substring(0, curDir->Length-1)){
						 //Windows::Forms::MessageBox::Show(dir->Substring(0, curDir->Length-1) + L"\n" + curDir->Substring(0, curDir->Length-1));
						 dir = dir->Substring(curDir->Length);
					 
					 }
				 }

				 bool found = false;
				 for(int i=0;i<lFiles->Items->Count;++i){
					 if(lFiles->Items[i]->Text->Length <= dir->Length){
						 if(lFiles->Items[i]->Text == dir->Substring(0, lFiles->Items[i]->Text->Length)){
							 found = true;
							 break;
						 }
					 }else{
						 if(lFiles->Items[i]->Text->Substring(0, dir->Length) == dir){
							 found = true;
							 break;
						 }
					 }
				 }
				 if(!found){
					 lFiles->Items->Add(dir);
				 }

				 delete dir;
			 }
		 }
private: System::Void bAddFile_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(dAddFile->ShowDialog() == Windows::Forms::DialogResult::OK){
				 lFiles->BeginUpdate();
				 bool found;

				 for(int i=0;i<dAddFile->FileNames->Length;++i){
					 found = false;
					 for(int j=0;j<lFiles->Items->Count;++j){
						 if(dAddFile->FileNames[i] == lFiles->Items[j]->Text){
							found = true;
							break;
						 }
					 }
					 if(!found){
						 String ^fname = dAddFile->FileNames[i];
						 if(fname->Length >= curDir->Length){
							 if(fname->Substring(0, curDir->Length) == curDir)
								 fname = fname->Substring(curDir->Length);
						 }
						 lFiles->Items->Add(fname);		
						 delete fname;
					 }
				 }

				 lFiles->EndUpdate();
			 }
		 }
private: System::Void bDelete_Click(System::Object^  sender, System::EventArgs^  e) {
			 lFiles->BeginUpdate();
			 for(int i=lFiles->SelectedIndices->Count-1;i>=0;--i){
				 lFiles->Items->RemoveAt(lFiles->SelectedIndices[i]);
			 }
			 lFiles->EndUpdate();
		 }
private: System::Void lFiles_ItemSelectionChanged(System::Object^  sender, System::Windows::Forms::ListViewItemSelectionChangedEventArgs^  e) {
			 bDelete->Enabled = lFiles->SelectedItems->Count > 0;
		 }


private: System::Void bRecent_Click(System::Object^  sender, System::EventArgs^  e) {
			 mRecent->Items->Clear();

			 String ^hook = gcnew String(L"HKEY_CURRENT_USER\\Software\\KGB Archiver 2\\recentArchives");
			 String ^num;
			 String ^tmp;

			 try{
				 for(int i=0;i<settings::general::recentItemsCount;++i){
					 num = String::Format(L"{0}", i);
					 tmp = (String^)Registry::GetValue(hook, num, L"");
					 if(tmp->Length != 0)
						mRecent->Items->Add(tmp);
					 delete num;
					 delete tmp;
				 }
			 }catch(Exception ^){}
			
			 mRecent->Show(bRecent, bRecent->Width, 0);
			
			 delete hook;
		 }
private: System::Void mRecent_ItemClicked(System::Object^  sender, System::Windows::Forms::ToolStripItemClickedEventArgs^  e) {
			 tDestination->Text = e->ClickedItem->Text;
		 }
};
}
