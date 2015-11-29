#pragma once

#include <shlobj.h>
#include "file_management.cpp"
//#include "../compress.cpp"
#include "columnselect.h"
#include "newArchive.h"
#include "options.h"
#include "about.h"
#include "information.h"
#include "extract.h"
#include "properties.h"
#include "passwd.h"
//#include "settings.cpp"

#include "overwrite.h"
#include "../language.h"

#define OVERWRITE_ALL 3
#define OVERWRITE_YES 2
#define OVERWRITE_NO 1
#define OVERWRITE_NEVER 0

int overwriteMode;

wstring curDir;

#define backListSender 0x0001

namespace KGBArchiver2net {

	using namespace System;
	using namespace System::IO;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Diagnostics;
	using namespace System::Threading;
	using namespace System::Globalization;
	using namespace Microsoft::Win32;

	vector<wstring>backList;
	vector<wstring>forwardList;

	bool overwriteFile(String ^oldFile, String ^newFile){
		if(overwriteMode == OVERWRITE_ALL || overwriteMode == OVERWRITE_NEVER)
			return overwriteMode;
		
		Windows::Forms::DialogResult tmp;
		overwrite ^fOverwrite = gcnew overwrite();
		IO::FileInfo ^fiOld = gcnew IO::FileInfo(oldFile);
		IO::FileInfo ^fiNew = gcnew IO::FileInfo(newFile);
		SHFILEINFO shfi;
		memset(&shfi,0,sizeof(shfi));
		SHGetFileInfo((wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(oldFile).ToPointer(), FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(shfi), SHGFI_ICON|SHGFI_USEFILEATTRIBUTES|/*SHGFI_SHELLICONSIZE|*/SHGFI_LARGEICON/*|SHGFI_TYPENAME*/);
		

		fOverwrite->gExisting->Text += IO::Path::GetFileName(oldFile);
		fOverwrite->lExistSize->Text = gcnew String(convertUnits(fiOld->Length).c_str());
		fOverwrite->lExistModified->Text = fiOld->LastWriteTime.ToShortDateString() + L" " + fiOld->LastWriteTime.ToShortTimeString();
		if(shfi.hIcon != NULL)
			fOverwrite->pExisting->Image = System::Drawing::Icon::FromHandle(IntPtr(shfi.hIcon))->ToBitmap();

		fOverwrite->lNewSize->Text = gcnew String(convertUnits(fiNew->Length).c_str());
		fOverwrite->lNewModified->Text = fiNew->LastWriteTime.ToShortDateString() + L" " + fiNew->LastWriteTime.ToShortTimeString();
		fOverwrite->pNew->Image = fOverwrite->pExisting->Image;

		tmp = fOverwrite->ShowDialog();

		delete fOverwrite;
			
		switch(tmp){
			case System::Windows::Forms::DialogResult::OK: overwriteMode = OVERWRITE_ALL;
			case System::Windows::Forms::DialogResult::Yes: return true; break;
			case System::Windows::Forms::DialogResult::Cancel: overwriteMode = OVERWRITE_NEVER;
			case System::Windows::Forms::DialogResult::No: return false; break;
		}

		return true;
	}

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		bool firstrun;
		bool restartRequied;
	private: System::Windows::Forms::ToolStripMenuItem^  mHomepage;
	private: System::Windows::Forms::ToolStripMenuItem^  mShellCut;
	private: System::Windows::Forms::ToolStripMenuItem^  mShellCopy;
	private: System::Windows::Forms::ToolStripMenuItem^  mShellPaste;
	private: System::Windows::Forms::ToolStripSeparator^  mShellSeparator3;
	private: System::Windows::Forms::Timer^  clipboardWatcher;

	public: 
		array<String^>^args;
		Form1(void)
		{
			//language settings
			try{
				Thread::CurrentThread::get()->CurrentUICulture = gcnew CultureInfo(settings::general::language);
			}catch(Exception ^){}

			//System::Windows::Forms::MessageBox::Show(String::Format("{0}", settings::general::language));

			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			firstrun = false;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ToolStripContainer^  tContainer;
	public: System::Windows::Forms::ToolStrip^  tAddr;

	private: System::Windows::Forms::ToolStripButton^  bUp;
	public: System::Windows::Forms::ToolStripComboBox^  cAddress;

	private: System::Windows::Forms::ToolStripButton^  bGo;
	public: System::Windows::Forms::MenuStrip^  mMain;

	private: System::Windows::Forms::ToolStripMenuItem^  mFile;

	private: System::Windows::Forms::ToolStripMenuItem^  mNew;

	private: System::Windows::Forms::ToolStripMenuItem^  mOpen;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  mHelp;
	private: System::Windows::Forms::ToolStripMenuItem^  mAbout;


	private: System::Windows::Forms::ToolStripMenuItem^  mEdit;

	private: System::Windows::Forms::ToolStripMenuItem^  mCut;

	private: System::Windows::Forms::ToolStripMenuItem^  mCopy;

	private: System::Windows::Forms::ToolStripMenuItem^  mPaste;

	private: System::Windows::Forms::ToolStripMenuItem^  mDelete;
	private: System::Windows::Forms::ToolStripMenuItem^  mView;
	public: System::Windows::Forms::ToolStrip^  tMain;

	private: System::Windows::Forms::ToolStripButton^  bNew;
	private: System::Windows::Forms::ToolStripButton^  bOpen;
	private: System::Windows::Forms::ToolStripSeparator^  sMain1;

	private: System::Windows::Forms::ToolStripButton^  bExtract;
	private: System::Windows::Forms::ToolStripButton^  bDel;
	private: System::Windows::Forms::ToolStripButton^  bInfo;
	private: System::Windows::Forms::ToolStripButton^  bTest;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::ListView^  lFiles;

	private: System::Windows::Forms::TreeView^  tFiles;
	private: System::Windows::Forms::ToolStripMenuItem^  mTools;
	private: System::Windows::Forms::ToolStripMenuItem^  mOptions;
	public: System::Windows::Forms::StatusStrip^  sStatusbar;

	private: System::Windows::Forms::ToolStripStatusLabel^  sStatus;
	private: System::Windows::Forms::ToolStripProgressBar^  pStatus;
	private: System::Windows::Forms::ColumnHeader^  cFilename;
	private: System::Windows::Forms::ColumnHeader^  cSize;
	private: System::Windows::Forms::ColumnHeader^  cCreated;
	private: System::Windows::Forms::ColumnHeader^  cModified;
	private: System::Windows::Forms::ColumnHeader^  cAccessed;
	private: System::Windows::Forms::ColumnHeader^  cAttributes;
	private: System::Windows::Forms::ToolStripMenuItem^  skinToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  mSkinProfessional;
	private: System::Windows::Forms::ToolStripMenuItem^  mSkinSystem;
	private: System::Windows::Forms::ImageList^  icoLst;
	private: System::Windows::Forms::ToolStripMenuItem^  mDirTree;
	private: System::Windows::Forms::ToolStripMenuItem^  mSkinProLight;
	private: System::Windows::Forms::ImageList^  treeIcoLst;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem2;
	private: System::Windows::Forms::ToolStripMenuItem^  mExtract;
	private: System::Windows::Forms::ToolStripMenuItem^  mTest;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem3;
	private: System::Windows::Forms::ToolStripMenuItem^  mInformation;
	private: System::Windows::Forms::OpenFileDialog^  dOpen;
	private: System::Windows::Forms::ToolStripMenuItem^  mSelectColumns;
	private: System::Windows::Forms::ContextMenuStrip^  mFileList;

	private: System::Windows::Forms::ToolStripMenuItem^  mShellOpen;
	private: System::Windows::Forms::ToolStripMenuItem^  mShellCompress;
private: System::Windows::Forms::ToolStripMenuItem^  mShellProperties;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem4;
private: System::Windows::Forms::ToolStripMenuItem^  mShellRename;

private: System::Windows::Forms::ToolStripMenuItem^  mShellDelete;
private: System::Windows::Forms::ToolStripMenuItem^  mShellWipe;
private: System::Windows::Forms::ToolStripSeparator^  mShellSeparator4;

	private: System::Windows::Forms::ToolStripMenuItem^  mWipe;

private: System::Windows::Forms::ToolStripSeparator^  bSeparator1;
private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem6;
private: System::Windows::Forms::ToolStripMenuItem^  mShellCreateDir;
private: System::IO::FileSystemWatcher^  fsWatcher;
private: System::Windows::Forms::ToolStripButton^  bWipe;
private: System::Windows::Forms::ToolStripButton^  bCut;
private: System::Windows::Forms::ToolStripButton^  bCopy;
private: System::Windows::Forms::ToolStripButton^  bPaste;

private: System::Windows::Forms::ToolStripSeparator^  sMain2;
private: System::Windows::Forms::ColumnHeader^  cType;
private: System::Windows::Forms::ToolStripMenuItem^  mProperties;
private: System::Windows::Forms::ToolStripSplitButton^  bBack;
private: System::Windows::Forms::ToolStripSplitButton^  bForward;
private: System::Windows::Forms::ToolStripMenuItem^  mCheckUpdates;
private: System::ComponentModel::BackgroundWorker^  bwUpdate;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->tContainer = (gcnew System::Windows::Forms::ToolStripContainer());
			this->sStatusbar = (gcnew System::Windows::Forms::StatusStrip());
			this->sStatus = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->pStatus = (gcnew System::Windows::Forms::ToolStripProgressBar());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->tFiles = (gcnew System::Windows::Forms::TreeView());
			this->treeIcoLst = (gcnew System::Windows::Forms::ImageList(this->components));
			this->lFiles = (gcnew System::Windows::Forms::ListView());
			this->cFilename = (gcnew System::Windows::Forms::ColumnHeader());
			this->cSize = (gcnew System::Windows::Forms::ColumnHeader());
			this->cType = (gcnew System::Windows::Forms::ColumnHeader());
			this->cCreated = (gcnew System::Windows::Forms::ColumnHeader());
			this->cModified = (gcnew System::Windows::Forms::ColumnHeader());
			this->cAccessed = (gcnew System::Windows::Forms::ColumnHeader());
			this->cAttributes = (gcnew System::Windows::Forms::ColumnHeader());
			this->mFileList = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->mShellOpen = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mShellCompress = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem6 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->mShellCreateDir = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem4 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->mShellCut = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mShellCopy = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mShellPaste = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mShellSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->mShellRename = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mShellDelete = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mShellWipe = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mShellSeparator4 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->mShellProperties = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->icoLst = (gcnew System::Windows::Forms::ImageList(this->components));
			this->tMain = (gcnew System::Windows::Forms::ToolStrip());
			this->bNew = (gcnew System::Windows::Forms::ToolStripButton());
			this->bOpen = (gcnew System::Windows::Forms::ToolStripButton());
			this->sMain1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->bCut = (gcnew System::Windows::Forms::ToolStripButton());
			this->bCopy = (gcnew System::Windows::Forms::ToolStripButton());
			this->bPaste = (gcnew System::Windows::Forms::ToolStripButton());
			this->sMain2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->bExtract = (gcnew System::Windows::Forms::ToolStripButton());
			this->bTest = (gcnew System::Windows::Forms::ToolStripButton());
			this->bDel = (gcnew System::Windows::Forms::ToolStripButton());
			this->bWipe = (gcnew System::Windows::Forms::ToolStripButton());
			this->bInfo = (gcnew System::Windows::Forms::ToolStripButton());
			this->mMain = (gcnew System::Windows::Forms::MenuStrip());
			this->mFile = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mNew = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mOpen = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mEdit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mCut = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mCopy = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mPaste = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mDelete = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mWipe = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->mExtract = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mTest = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mView = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mDirTree = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->skinToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mSkinProfessional = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mSkinProLight = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mSkinSystem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mSelectColumns = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->mInformation = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mProperties = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mTools = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mOptions = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mHelp = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mCheckUpdates = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mHomepage = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mAbout = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tAddr = (gcnew System::Windows::Forms::ToolStrip());
			this->bBack = (gcnew System::Windows::Forms::ToolStripSplitButton());
			this->bForward = (gcnew System::Windows::Forms::ToolStripSplitButton());
			this->bSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->bUp = (gcnew System::Windows::Forms::ToolStripButton());
			this->cAddress = (gcnew System::Windows::Forms::ToolStripComboBox());
			this->bGo = (gcnew System::Windows::Forms::ToolStripButton());
			this->dOpen = (gcnew System::Windows::Forms::OpenFileDialog());
			this->fsWatcher = (gcnew System::IO::FileSystemWatcher());
			this->bwUpdate = (gcnew System::ComponentModel::BackgroundWorker());
			this->clipboardWatcher = (gcnew System::Windows::Forms::Timer(this->components));
			this->tContainer->BottomToolStripPanel->SuspendLayout();
			this->tContainer->ContentPanel->SuspendLayout();
			this->tContainer->LeftToolStripPanel->SuspendLayout();
			this->tContainer->TopToolStripPanel->SuspendLayout();
			this->tContainer->SuspendLayout();
			this->sStatusbar->SuspendLayout();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->mFileList->SuspendLayout();
			this->tMain->SuspendLayout();
			this->mMain->SuspendLayout();
			this->tAddr->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->fsWatcher))->BeginInit();
			this->SuspendLayout();
			// 
			// tContainer
			// 
			// 
			// tContainer.BottomToolStripPanel
			// 
			this->tContainer->BottomToolStripPanel->Controls->Add(this->sStatusbar);
			// 
			// tContainer.ContentPanel
			// 
			this->tContainer->ContentPanel->Controls->Add(this->splitContainer1);
			this->tContainer->ContentPanel->RenderMode = System::Windows::Forms::ToolStripRenderMode::ManagerRenderMode;
			resources->ApplyResources(this->tContainer->ContentPanel, L"tContainer.ContentPanel");
			resources->ApplyResources(this->tContainer, L"tContainer");
			// 
			// tContainer.LeftToolStripPanel
			// 
			this->tContainer->LeftToolStripPanel->Controls->Add(this->tMain);
			this->tContainer->Name = L"tContainer";
			// 
			// tContainer.TopToolStripPanel
			// 
			this->tContainer->TopToolStripPanel->Controls->Add(this->tAddr);
			this->tContainer->TopToolStripPanel->Controls->Add(this->mMain);
			this->tContainer->TopToolStripPanel->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			// 
			// sStatusbar
			// 
			resources->ApplyResources(this->sStatusbar, L"sStatusbar");
			this->sStatusbar->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->sStatus, this->pStatus});
			this->sStatusbar->Name = L"sStatusbar";
			this->sStatusbar->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			// 
			// sStatus
			// 
			this->sStatus->Name = L"sStatus";
			resources->ApplyResources(this->sStatus, L"sStatus");
			// 
			// pStatus
			// 
			this->pStatus->Name = L"pStatus";
			resources->ApplyResources(this->pStatus, L"pStatus");
			// 
			// splitContainer1
			// 
			resources->ApplyResources(this->splitContainer1, L"splitContainer1");
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->tFiles);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->lFiles);
			// 
			// tFiles
			// 
			resources->ApplyResources(this->tFiles, L"tFiles");
			this->tFiles->ImageList = this->treeIcoLst;
			this->tFiles->Name = L"tFiles";
			this->tFiles->ShowNodeToolTips = true;
			this->tFiles->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &Form1::tFiles_AfterSelect);
			// 
			// treeIcoLst
			// 
			this->treeIcoLst->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"treeIcoLst.ImageStream")));
			this->treeIcoLst->TransparentColor = System::Drawing::Color::Transparent;
			this->treeIcoLst->Images->SetKeyName(0, L"folderopen.ico");
			this->treeIcoLst->Images->SetKeyName(1, L"drive.ico");
			this->treeIcoLst->Images->SetKeyName(2, L"folderclose.ico");
			this->treeIcoLst->Images->SetKeyName(3, L"kgb_small.ico");
			this->treeIcoLst->Images->SetKeyName(4, L"zip_small.ico");
			// 
			// lFiles
			// 
			this->lFiles->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(7) {this->cFilename, this->cSize, 
				this->cType, this->cCreated, this->cModified, this->cAccessed, this->cAttributes});
			this->lFiles->ContextMenuStrip = this->mFileList;
			resources->ApplyResources(this->lFiles, L"lFiles");
			this->lFiles->Groups->AddRange(gcnew cli::array< System::Windows::Forms::ListViewGroup^  >(3) {(cli::safe_cast<System::Windows::Forms::ListViewGroup^  >(resources->GetObject(L"lFiles.Groups"))), 
				(cli::safe_cast<System::Windows::Forms::ListViewGroup^  >(resources->GetObject(L"lFiles.Groups1"))), (cli::safe_cast<System::Windows::Forms::ListViewGroup^  >(resources->GetObject(L"lFiles.Groups2")))});
			this->lFiles->LabelEdit = true;
			this->lFiles->Name = L"lFiles";
			this->lFiles->SmallImageList = this->icoLst;
			this->lFiles->UseCompatibleStateImageBehavior = false;
			this->lFiles->View = System::Windows::Forms::View::Details;
			this->lFiles->ItemActivate += gcnew System::EventHandler(this, &Form1::lFiles_ItemActivate);
			this->lFiles->ColumnClick += gcnew System::Windows::Forms::ColumnClickEventHandler(this, &Form1::lFiles_ColumnClick);
			this->lFiles->ItemSelectionChanged += gcnew System::Windows::Forms::ListViewItemSelectionChangedEventHandler(this, &Form1::lFiles_ItemSelectionChanged);
			this->lFiles->AfterLabelEdit += gcnew System::Windows::Forms::LabelEditEventHandler(this, &Form1::lFiles_AfterLabelEdit);
			// 
			// cFilename
			// 
			resources->ApplyResources(this->cFilename, L"cFilename");
			// 
			// cSize
			// 
			resources->ApplyResources(this->cSize, L"cSize");
			// 
			// cType
			// 
			resources->ApplyResources(this->cType, L"cType");
			// 
			// cCreated
			// 
			resources->ApplyResources(this->cCreated, L"cCreated");
			// 
			// cModified
			// 
			resources->ApplyResources(this->cModified, L"cModified");
			// 
			// cAccessed
			// 
			resources->ApplyResources(this->cAccessed, L"cAccessed");
			// 
			// cAttributes
			// 
			resources->ApplyResources(this->cAttributes, L"cAttributes");
			// 
			// mFileList
			// 
			this->mFileList->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(14) {this->mShellOpen, this->mShellCompress, 
				this->toolStripMenuItem6, this->mShellCreateDir, this->toolStripMenuItem4, this->mShellCut, this->mShellCopy, this->mShellPaste, 
				this->mShellSeparator3, this->mShellRename, this->mShellDelete, this->mShellWipe, this->mShellSeparator4, this->mShellProperties});
			this->mFileList->Name = L"mFileList";
			resources->ApplyResources(this->mFileList, L"mFileList");
			this->mFileList->Opening += gcnew System::ComponentModel::CancelEventHandler(this, &Form1::mFileList_Opening);
			// 
			// mShellOpen
			// 
			resources->ApplyResources(this->mShellOpen, L"mShellOpen");
			this->mShellOpen->Name = L"mShellOpen";
			this->mShellOpen->Click += gcnew System::EventHandler(this, &Form1::lFiles_ItemActivate);
			// 
			// mShellCompress
			// 
			this->mShellCompress->Name = L"mShellCompress";
			resources->ApplyResources(this->mShellCompress, L"mShellCompress");
			this->mShellCompress->Click += gcnew System::EventHandler(this, &Form1::mNew_Click);
			// 
			// toolStripMenuItem6
			// 
			this->toolStripMenuItem6->Name = L"toolStripMenuItem6";
			resources->ApplyResources(this->toolStripMenuItem6, L"toolStripMenuItem6");
			// 
			// mShellCreateDir
			// 
			this->mShellCreateDir->Name = L"mShellCreateDir";
			resources->ApplyResources(this->mShellCreateDir, L"mShellCreateDir");
			this->mShellCreateDir->Click += gcnew System::EventHandler(this, &Form1::mShellCreateDir_Click);
			// 
			// toolStripMenuItem4
			// 
			this->toolStripMenuItem4->Name = L"toolStripMenuItem4";
			resources->ApplyResources(this->toolStripMenuItem4, L"toolStripMenuItem4");
			// 
			// mShellCut
			// 
			this->mShellCut->Name = L"mShellCut";
			resources->ApplyResources(this->mShellCut, L"mShellCut");
			this->mShellCut->Click += gcnew System::EventHandler(this, &Form1::mCut_Click);
			// 
			// mShellCopy
			// 
			this->mShellCopy->Name = L"mShellCopy";
			resources->ApplyResources(this->mShellCopy, L"mShellCopy");
			this->mShellCopy->Click += gcnew System::EventHandler(this, &Form1::mCopy_Click);
			// 
			// mShellPaste
			// 
			this->mShellPaste->Name = L"mShellPaste";
			resources->ApplyResources(this->mShellPaste, L"mShellPaste");
			this->mShellPaste->Click += gcnew System::EventHandler(this, &Form1::mPaste_Click);
			// 
			// mShellSeparator3
			// 
			this->mShellSeparator3->Name = L"mShellSeparator3";
			resources->ApplyResources(this->mShellSeparator3, L"mShellSeparator3");
			// 
			// mShellRename
			// 
			this->mShellRename->Name = L"mShellRename";
			resources->ApplyResources(this->mShellRename, L"mShellRename");
			this->mShellRename->Click += gcnew System::EventHandler(this, &Form1::mShellRename_Click);
			// 
			// mShellDelete
			// 
			this->mShellDelete->Name = L"mShellDelete";
			resources->ApplyResources(this->mShellDelete, L"mShellDelete");
			this->mShellDelete->Click += gcnew System::EventHandler(this, &Form1::mDelete_Click);
			// 
			// mShellWipe
			// 
			this->mShellWipe->Name = L"mShellWipe";
			resources->ApplyResources(this->mShellWipe, L"mShellWipe");
			this->mShellWipe->Click += gcnew System::EventHandler(this, &Form1::mWipe_Click);
			// 
			// mShellSeparator4
			// 
			this->mShellSeparator4->Name = L"mShellSeparator4";
			resources->ApplyResources(this->mShellSeparator4, L"mShellSeparator4");
			// 
			// mShellProperties
			// 
			this->mShellProperties->Name = L"mShellProperties";
			resources->ApplyResources(this->mShellProperties, L"mShellProperties");
			this->mShellProperties->Click += gcnew System::EventHandler(this, &Form1::mProperties_Click);
			// 
			// icoLst
			// 
			this->icoLst->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"icoLst.ImageStream")));
			this->icoLst->TransparentColor = System::Drawing::Color::Transparent;
			this->icoLst->Images->SetKeyName(0, L"folderclose.ico");
			// 
			// tMain
			// 
			resources->ApplyResources(this->tMain, L"tMain");
			this->tMain->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(12) {this->bNew, this->bOpen, this->sMain1, 
				this->bCut, this->bCopy, this->bPaste, this->sMain2, this->bExtract, this->bTest, this->bDel, this->bWipe, this->bInfo});
			this->tMain->Name = L"tMain";
			this->tMain->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			// 
			// bNew
			// 
			this->bNew->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->bNew, L"bNew");
			this->bNew->Name = L"bNew";
			this->bNew->Click += gcnew System::EventHandler(this, &Form1::mNew_Click);
			// 
			// bOpen
			// 
			this->bOpen->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->bOpen, L"bOpen");
			this->bOpen->Name = L"bOpen";
			this->bOpen->Click += gcnew System::EventHandler(this, &Form1::mOpen_Click);
			// 
			// sMain1
			// 
			this->sMain1->Name = L"sMain1";
			resources->ApplyResources(this->sMain1, L"sMain1");
			// 
			// bCut
			// 
			this->bCut->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->bCut, L"bCut");
			this->bCut->Name = L"bCut";
			this->bCut->Click += gcnew System::EventHandler(this, &Form1::mCut_Click);
			// 
			// bCopy
			// 
			this->bCopy->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->bCopy, L"bCopy");
			this->bCopy->Name = L"bCopy";
			this->bCopy->Click += gcnew System::EventHandler(this, &Form1::mCopy_Click);
			// 
			// bPaste
			// 
			this->bPaste->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->bPaste, L"bPaste");
			this->bPaste->Name = L"bPaste";
			this->bPaste->Click += gcnew System::EventHandler(this, &Form1::mPaste_Click);
			// 
			// sMain2
			// 
			this->sMain2->Name = L"sMain2";
			resources->ApplyResources(this->sMain2, L"sMain2");
			// 
			// bExtract
			// 
			this->bExtract->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->bExtract, L"bExtract");
			this->bExtract->Name = L"bExtract";
			this->bExtract->Click += gcnew System::EventHandler(this, &Form1::mExtract_Click);
			// 
			// bTest
			// 
			this->bTest->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->bTest, L"bTest");
			this->bTest->Name = L"bTest";
			this->bTest->Click += gcnew System::EventHandler(this, &Form1::mTest_Click);
			// 
			// bDel
			// 
			this->bDel->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->bDel, L"bDel");
			this->bDel->Name = L"bDel";
			this->bDel->Click += gcnew System::EventHandler(this, &Form1::mDelete_Click);
			// 
			// bWipe
			// 
			this->bWipe->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->bWipe, L"bWipe");
			this->bWipe->Name = L"bWipe";
			this->bWipe->Click += gcnew System::EventHandler(this, &Form1::mWipe_Click);
			// 
			// bInfo
			// 
			this->bInfo->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->bInfo, L"bInfo");
			this->bInfo->Name = L"bInfo";
			this->bInfo->Click += gcnew System::EventHandler(this, &Form1::mInformation_Click);
			// 
			// mMain
			// 
			resources->ApplyResources(this->mMain, L"mMain");
			this->mMain->GripStyle = System::Windows::Forms::ToolStripGripStyle::Visible;
			this->mMain->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->mFile, this->mEdit, this->mView, 
				this->mTools, this->mHelp});
			this->mMain->LayoutStyle = System::Windows::Forms::ToolStripLayoutStyle::Flow;
			this->mMain->Name = L"mMain";
			this->mMain->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			// 
			// mFile
			// 
			this->mFile->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->mNew, this->mOpen, 
				this->toolStripMenuItem1, this->exitToolStripMenuItem});
			this->mFile->Name = L"mFile";
			resources->ApplyResources(this->mFile, L"mFile");
			// 
			// mNew
			// 
			this->mNew->Name = L"mNew";
			resources->ApplyResources(this->mNew, L"mNew");
			this->mNew->Click += gcnew System::EventHandler(this, &Form1::mNew_Click);
			// 
			// mOpen
			// 
			this->mOpen->Name = L"mOpen";
			resources->ApplyResources(this->mOpen, L"mOpen");
			this->mOpen->Click += gcnew System::EventHandler(this, &Form1::mOpen_Click);
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			resources->ApplyResources(this->toolStripMenuItem1, L"toolStripMenuItem1");
			// 
			// exitToolStripMenuItem
			// 
			resources->ApplyResources(this->exitToolStripMenuItem, L"exitToolStripMenuItem");
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::exitToolStripMenuItem_Click);
			// 
			// mEdit
			// 
			this->mEdit->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(8) {this->mCut, this->mCopy, 
				this->mPaste, this->mDelete, this->mWipe, this->toolStripMenuItem2, this->mExtract, this->mTest});
			this->mEdit->Name = L"mEdit";
			resources->ApplyResources(this->mEdit, L"mEdit");
			this->mEdit->DropDownOpening += gcnew System::EventHandler(this, &Form1::mEdit_DropDownOpening);
			// 
			// mCut
			// 
			this->mCut->Name = L"mCut";
			resources->ApplyResources(this->mCut, L"mCut");
			this->mCut->Click += gcnew System::EventHandler(this, &Form1::mCut_Click);
			// 
			// mCopy
			// 
			this->mCopy->Name = L"mCopy";
			resources->ApplyResources(this->mCopy, L"mCopy");
			this->mCopy->Click += gcnew System::EventHandler(this, &Form1::mCopy_Click);
			// 
			// mPaste
			// 
			this->mPaste->Name = L"mPaste";
			resources->ApplyResources(this->mPaste, L"mPaste");
			this->mPaste->Click += gcnew System::EventHandler(this, &Form1::mPaste_Click);
			// 
			// mDelete
			// 
			this->mDelete->Name = L"mDelete";
			resources->ApplyResources(this->mDelete, L"mDelete");
			this->mDelete->Click += gcnew System::EventHandler(this, &Form1::mDelete_Click);
			// 
			// mWipe
			// 
			this->mWipe->Name = L"mWipe";
			resources->ApplyResources(this->mWipe, L"mWipe");
			this->mWipe->Click += gcnew System::EventHandler(this, &Form1::mWipe_Click);
			// 
			// toolStripMenuItem2
			// 
			this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
			resources->ApplyResources(this->toolStripMenuItem2, L"toolStripMenuItem2");
			// 
			// mExtract
			// 
			this->mExtract->Name = L"mExtract";
			resources->ApplyResources(this->mExtract, L"mExtract");
			this->mExtract->Click += gcnew System::EventHandler(this, &Form1::mExtract_Click);
			// 
			// mTest
			// 
			this->mTest->Name = L"mTest";
			resources->ApplyResources(this->mTest, L"mTest");
			this->mTest->Click += gcnew System::EventHandler(this, &Form1::mTest_Click);
			// 
			// mView
			// 
			this->mView->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->mDirTree, this->skinToolStripMenuItem, 
				this->mSelectColumns, this->toolStripMenuItem3, this->mInformation, this->mProperties});
			this->mView->Name = L"mView";
			resources->ApplyResources(this->mView, L"mView");
			this->mView->DropDownOpening += gcnew System::EventHandler(this, &Form1::mView_DropDownOpening);
			// 
			// mDirTree
			// 
			this->mDirTree->Checked = true;
			this->mDirTree->CheckState = System::Windows::Forms::CheckState::Checked;
			this->mDirTree->Name = L"mDirTree";
			resources->ApplyResources(this->mDirTree, L"mDirTree");
			this->mDirTree->Click += gcnew System::EventHandler(this, &Form1::mDirTree_Click);
			// 
			// skinToolStripMenuItem
			// 
			this->skinToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->mSkinProfessional, 
				this->mSkinProLight, this->mSkinSystem});
			this->skinToolStripMenuItem->Name = L"skinToolStripMenuItem";
			resources->ApplyResources(this->skinToolStripMenuItem, L"skinToolStripMenuItem");
			// 
			// mSkinProfessional
			// 
			this->mSkinProfessional->Checked = true;
			this->mSkinProfessional->CheckState = System::Windows::Forms::CheckState::Checked;
			this->mSkinProfessional->Name = L"mSkinProfessional";
			resources->ApplyResources(this->mSkinProfessional, L"mSkinProfessional");
			this->mSkinProfessional->Click += gcnew System::EventHandler(this, &Form1::mSkinProfessional_Click);
			// 
			// mSkinProLight
			// 
			this->mSkinProLight->Name = L"mSkinProLight";
			resources->ApplyResources(this->mSkinProLight, L"mSkinProLight");
			this->mSkinProLight->Click += gcnew System::EventHandler(this, &Form1::mSkinProLight_Click);
			// 
			// mSkinSystem
			// 
			this->mSkinSystem->Name = L"mSkinSystem";
			resources->ApplyResources(this->mSkinSystem, L"mSkinSystem");
			this->mSkinSystem->Click += gcnew System::EventHandler(this, &Form1::mSkinSystem_Click);
			// 
			// mSelectColumns
			// 
			this->mSelectColumns->Name = L"mSelectColumns";
			resources->ApplyResources(this->mSelectColumns, L"mSelectColumns");
			this->mSelectColumns->Click += gcnew System::EventHandler(this, &Form1::mSelectColumns_Click);
			// 
			// toolStripMenuItem3
			// 
			this->toolStripMenuItem3->Name = L"toolStripMenuItem3";
			resources->ApplyResources(this->toolStripMenuItem3, L"toolStripMenuItem3");
			// 
			// mInformation
			// 
			this->mInformation->Name = L"mInformation";
			resources->ApplyResources(this->mInformation, L"mInformation");
			this->mInformation->Click += gcnew System::EventHandler(this, &Form1::mInformation_Click);
			// 
			// mProperties
			// 
			this->mProperties->Name = L"mProperties";
			resources->ApplyResources(this->mProperties, L"mProperties");
			this->mProperties->Click += gcnew System::EventHandler(this, &Form1::mProperties_Click);
			// 
			// mTools
			// 
			this->mTools->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->mOptions});
			this->mTools->Name = L"mTools";
			resources->ApplyResources(this->mTools, L"mTools");
			// 
			// mOptions
			// 
			this->mOptions->Name = L"mOptions";
			resources->ApplyResources(this->mOptions, L"mOptions");
			this->mOptions->Click += gcnew System::EventHandler(this, &Form1::mOptions_Click);
			// 
			// mHelp
			// 
			this->mHelp->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->mCheckUpdates, 
				this->mHomepage, this->mAbout});
			this->mHelp->Name = L"mHelp";
			resources->ApplyResources(this->mHelp, L"mHelp");
			// 
			// mCheckUpdates
			// 
			this->mCheckUpdates->Name = L"mCheckUpdates";
			resources->ApplyResources(this->mCheckUpdates, L"mCheckUpdates");
			this->mCheckUpdates->Click += gcnew System::EventHandler(this, &Form1::mCheckUpdates_Click);
			// 
			// mHomepage
			// 
			this->mHomepage->Name = L"mHomepage";
			resources->ApplyResources(this->mHomepage, L"mHomepage");
			this->mHomepage->Click += gcnew System::EventHandler(this, &Form1::homepageToolStripMenuItem_Click);
			// 
			// mAbout
			// 
			this->mAbout->Name = L"mAbout";
			resources->ApplyResources(this->mAbout, L"mAbout");
			this->mAbout->Click += gcnew System::EventHandler(this, &Form1::mAbout_Click);
			// 
			// tAddr
			// 
			this->tAddr->CanOverflow = false;
			resources->ApplyResources(this->tAddr, L"tAddr");
			this->tAddr->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->bBack, this->bForward, 
				this->bSeparator1, this->bUp, this->cAddress, this->bGo});
			this->tAddr->Name = L"tAddr";
			this->tAddr->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			this->tAddr->Stretch = true;
			// 
			// bBack
			// 
			this->bBack->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->bBack, L"bBack");
			this->bBack->Name = L"bBack";
			this->bBack->ButtonClick += gcnew System::EventHandler(this, &Form1::bBack_Click);
			this->bBack->DropDownOpening += gcnew System::EventHandler(this, &Form1::bBack_DropDownOpening);
			this->bBack->DropDownItemClicked += gcnew System::Windows::Forms::ToolStripItemClickedEventHandler(this, &Form1::bBack_DropDownItemClicked);
			// 
			// bForward
			// 
			this->bForward->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->bForward, L"bForward");
			this->bForward->Name = L"bForward";
			this->bForward->ButtonClick += gcnew System::EventHandler(this, &Form1::bForward_ButtonClick);
			this->bForward->DropDownOpening += gcnew System::EventHandler(this, &Form1::bForward_DropDownOpening);
			this->bForward->DropDownItemClicked += gcnew System::Windows::Forms::ToolStripItemClickedEventHandler(this, &Form1::bForward_DropDownItemClicked);
			// 
			// bSeparator1
			// 
			this->bSeparator1->Name = L"bSeparator1";
			resources->ApplyResources(this->bSeparator1, L"bSeparator1");
			// 
			// bUp
			// 
			this->bUp->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->bUp, L"bUp");
			this->bUp->Name = L"bUp";
			this->bUp->Click += gcnew System::EventHandler(this, &Form1::bUp_Click);
			// 
			// cAddress
			// 
			this->cAddress->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Suggest;
			this->cAddress->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::FileSystemDirectories;
			resources->ApplyResources(this->cAddress, L"cAddress");
			this->cAddress->Name = L"cAddress";
			this->cAddress->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::cAddress_KeyDown);
			// 
			// bGo
			// 
			resources->ApplyResources(this->bGo, L"bGo");
			this->bGo->Name = L"bGo";
			this->bGo->Click += gcnew System::EventHandler(this, &Form1::bGo_Click);
			// 
			// dOpen
			// 
			resources->ApplyResources(this->dOpen, L"dOpen");
			// 
			// fsWatcher
			// 
			this->fsWatcher->EnableRaisingEvents = true;
			this->fsWatcher->NotifyFilter = static_cast<System::IO::NotifyFilters>(((((System::IO::NotifyFilters::FileName | System::IO::NotifyFilters::DirectoryName) 
				| System::IO::NotifyFilters::Attributes) 
				| System::IO::NotifyFilters::Size) 
				| System::IO::NotifyFilters::LastWrite));
			this->fsWatcher->SynchronizingObject = this;
			this->fsWatcher->Created += gcnew System::IO::FileSystemEventHandler(this, &Form1::fsWatcher_Created);
			this->fsWatcher->Deleted += gcnew System::IO::FileSystemEventHandler(this, &Form1::fsWatcher_Deleted);
			this->fsWatcher->Renamed += gcnew System::IO::RenamedEventHandler(this, &Form1::fsWatcher_Renamed);
			// 
			// bwUpdate
			// 
			this->bwUpdate->WorkerSupportsCancellation = true;
			this->bwUpdate->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::bwUpdate_DoWork);
			// 
			// clipboardWatcher
			// 
			this->clipboardWatcher->Enabled = true;
			this->clipboardWatcher->Tick += gcnew System::EventHandler(this, &Form1::clipboardWatcher_Tick);
			// 
			// Form1
			// 
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->tContainer);
			this->MainMenuStrip = this->mMain;
			this->Name = L"Form1";
			this->Resize += gcnew System::EventHandler(this, &Form1::Form1_Resize);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->tContainer->BottomToolStripPanel->ResumeLayout(false);
			this->tContainer->BottomToolStripPanel->PerformLayout();
			this->tContainer->ContentPanel->ResumeLayout(false);
			this->tContainer->LeftToolStripPanel->ResumeLayout(false);
			this->tContainer->LeftToolStripPanel->PerformLayout();
			this->tContainer->TopToolStripPanel->ResumeLayout(false);
			this->tContainer->TopToolStripPanel->PerformLayout();
			this->tContainer->ResumeLayout(false);
			this->tContainer->PerformLayout();
			this->sStatusbar->ResumeLayout(false);
			this->sStatusbar->PerformLayout();
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			this->mFileList->ResumeLayout(false);
			this->tMain->ResumeLayout(false);
			this->tMain->PerformLayout();
			this->mMain->ResumeLayout(false);
			this->mMain->PerformLayout();
			this->tAddr->ResumeLayout(false);
			this->tAddr->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->fsWatcher))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

	ref class ListViewItemComparer: public IComparer
	{
	private:
	   int col;

	public:
	   ListViewItemComparer()
	   {
		  col = 0;
	   }

	   ListViewItemComparer( int column )
	   {
		  col = column;
	   }

	   virtual int Compare( Object^ x, Object^ y )
	   {
		   try{
				return String::Compare( (dynamic_cast<ListViewItem^>(x))->SubItems[ col ]->Text,
								  (dynamic_cast<ListViewItem^>(y))->SubItems[ col ]->Text );
		   }catch(Exception ^e){
				return 0;
		   }
	   }
	};

	int lastCol;

	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
				 this->SuspendLayout();
				 
				 curDir = L"C:\\";
				 /*for(wchar_t c[]=L"C:";c[0] < L'Z';c[0] += L'\1'){
					 if(SetCurrentDirectory(c)){
						tFiles->Nodes->Add(gcnew String(c), gcnew String(c), 1);
						tFiles->Nodes[gcnew String(c)]->SelectedImageIndex = 1;
						//FillTree(tFiles, c);
					 }	
				 }*/
				 cli::array<IO::DriveInfo^>^drives = IO::DriveInfo::GetDrives();
				 for(int i=0;i<drives->Length;i++){
					 tFiles->Nodes->Add(drives[i]->RootDirectory->FullName->Substring(0, drives[i]->RootDirectory->FullName->Length-1), drives[i]->RootDirectory->FullName->Substring(0, drives[i]->RootDirectory->FullName->Length-1), 1, 1);
				 }
				 //FillDir(lFiles, curDir);
				 //FillTree(tFiles, curDir);

				 mDelete->Image = bDel->Image;
				 mShellDelete->Image = bDel->Image;
				 mWipe->Image = bWipe->Image;
				 mShellWipe->Image = bWipe->Image;
				 mNew->Image = bNew->Image;
				 mShellCompress->Image = bNew->Image;
				 mOpen->Image = bOpen->Image;
				 mShellOpen->Image = bOpen->Image;
				 mInformation->Image = bInfo->Image;
				 mExtract->Image = bExtract->Image;
				 mTest->Image = bTest->Image;
				 mCut->Image = bCut->Image;
				 mCopy->Image = bCopy->Image;
				 mPaste->Image = bPaste->Image;
				 mShellCut->Image = bCut->Image;
				 mShellCopy->Image = bCopy->Image;
				 mShellPaste->Image = bPaste->Image;

				 lastCol = -1;

				 Form1_Resize(nullptr, EventArgs::Empty);

				 makeColumns();

				 switch(settings::skin){
					case 0: mSkinProfessional_Click(nullptr, EventArgs::Empty); break;
					case 1: mSkinProLight_Click(nullptr, EventArgs::Empty); break;
					case 2: mSkinSystem_Click(nullptr, EventArgs::Empty); break;
				 }

				 if(firstrun)
					 mOptions_Click(nullptr, EventArgs::Empty);

				 if(args->Length > 0){
					 if(args[0]->Length >= 2){
						 if(args[0] == L"-c" && args->Length == 3){
							 this->Visible = false;
							 if(args[1][0] == L'"')
								args[1] = args[1]->Substring(1, args[1]->Length-2);
							cAddress->Text = Path::GetFullPath(args[1]+L"\\");
							//this->Visible = true;
							bGo_Click(nullptr, EventArgs::Empty);
							
							if(args[2][0] == L'"')
								args[2] = args[2]->Substring(1, args[2]->Length-2);
							
							StreamReader ^strReader = gcnew StreamReader(args[2]);
							String ^s;

							while(1){
								s = strReader->ReadLine();
								if(!s)
									break;
								for each(ListViewItem ^item in lFiles->Items){
									if(item->Text->ToLower() == s->ToLower()){
										item->Selected = true;
										break;
									}
								}
								delete s;
							}

							strReader->Close();
							delete strReader;

							File::Delete(args[2]);
							
							mNew_Click(nullptr, EventArgs::Empty);
							Close();

						 }else{
							 if(args[0][0] == L'"')
								args[0] = args[0]->Substring(1, args[0]->Length-2);
							 cAddress->Text = Path::GetFullPath(args[0]);
							 bGo_Click(nullptr, EventArgs::Empty);
						 }
					 }
				 }else{
					 bGo_Click(nullptr, EventArgs::Empty);
				 }

				 overwriteMode = OVERWRITE_NO;

				 lFiles->ShowGroups = settings::fileManager::showFileGroups;

				 this->Text += L"        Build date: " + __DATE__ + L" " + __TIME__;

				 if(settings::general::checkUpdates)
					 bwUpdate->RunWorkerAsync(true);

				 this->ResumeLayout();
			 }
private: System::Void bGo_Click(System::Object^  sender, System::EventArgs^  e) {
			 pStatus->Visible = true;
			 pStatus->Maximum = 4;
			 sStatus->Text = getString("ListingDir");//gcnew String(L"Listing directory...");
			 Update();
			 //pStatus->Value = 0;
			 
			 wstring archive_name;

			 cAddress->Text = gcnew String(filterAddress((wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(cAddress->Text).ToPointer()).c_str());
			 curDir = (wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(cAddress->Text).ToPointer();

			 pStatus->Value = 1;
			 
			 FillDir(lFiles, (wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(cAddress->Text).ToPointer());
			 pStatus->Value = 2;
			 FillTree(tFiles, (wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(cAddress->Text).ToPointer());
			 pStatus->Value = 3;

			 mShellRename->Enabled = false;

			 mNew->Enabled = false;
			 bNew->Enabled = false;
			
			 mDelete->Enabled = false;
			 bDel->Enabled = false;

			 mWipe->Enabled = false;
			 bWipe->Enabled = false;

			 bCut->Enabled = false;
			 bCopy->Enabled = false;

			 if(!isKGBArchive(curDir.c_str(), archive_name) && !isZIPArchive(curDir.c_str(), archive_name)){
				 try{
					 fsWatcher->Path = cAddress->Text;
					 fsWatcher->EnableRaisingEvents = true;
				 }catch(Exception ^e){
					 MessageBox::Show(e->Message);
				 }
				

				lFiles->LabelEdit = true;
				

				mExtract->Enabled = false;
				bExtract->Enabled = false;

				mTest->Enabled = false;
				bTest->Enabled = false;

				mInformation->Enabled = false;
				bInfo->Enabled = false;


			 }else{
				//fsWatcher->Path = cAddress->Text;
				fsWatcher->EnableRaisingEvents = false;

				lFiles->LabelEdit = false;

				mExtract->Enabled = true;
				bExtract->Enabled = true;

				mTest->Enabled = true;
				bTest->Enabled = true;

				mInformation->Enabled = true;
				bInfo->Enabled = true;
			 }

			 backList.push_back(curDir);
			 if(backList.size() > 1)
				bBack->Enabled = true;
			 if(sender != backListSender){
				 forwardList.clear();
				 bForward->Enabled = false;
			 }else if(forwardList.size() != 0){
				 bForward->Enabled = true;
			 }
			 
			 sStatus->Text = getString("Done");//gcnew String(L"Done");
			 pStatus->Value = 0;
			 pStatus->Visible = false;
			 /*FillDirList(cAddress, (wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(cAddress->Text).ToPointer());
			 pStatus->Value = 0;*/
			 //MessageBox::Show(tFiles->SelectedNode->ToString());
			 //fsWatcher->EnableRaisingEvents = false;
		 }


private: System::Void mSkinProfessional_Click(System::Object^  sender, System::EventArgs^  e) {
			 tAddr->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;			 
			 tMain->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;	
			 mMain->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			 sStatusbar->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			 tContainer->TopToolStripPanel->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			 tContainer->LeftToolStripPanel->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			 tContainer->RightToolStripPanel->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			 tContainer->BottomToolStripPanel->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			 
			 Application::VisualStyleState = System::Windows::Forms::VisualStyles::VisualStyleState::ClientAndNonClientAreasEnabled;

			 mSkinProfessional->Checked = true;
			 mSkinProLight->Checked = false;
			 mSkinSystem->Checked = false;

			 settings::skin = 0;
		 }
private: System::Void mSkinSystem_Click(System::Object^  sender, System::EventArgs^  e) {
			 tAddr->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;			 
			 tMain->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;	
			 mMain->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
			 sStatusbar->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
			 tContainer->TopToolStripPanel->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
			 tContainer->LeftToolStripPanel->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
			 tContainer->RightToolStripPanel->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
			 tContainer->BottomToolStripPanel->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
			 
			 Application::VisualStyleState = System::Windows::Forms::VisualStyles::VisualStyleState::ClientAndNonClientAreasEnabled;

			 mSkinProfessional->Checked = false;
			 mSkinProLight->Checked = false;
			 mSkinSystem->Checked = true;

			 settings::skin = 2;
		 }
private: System::Void Form1_Resize(System::Object^  sender, System::EventArgs^  e) {
			 cAddress->Width = this->Width - bUp->Width - bGo->Width - bBack->Width - bForward->Width - bSeparator1->Width - (LOBYTE(LOWORD(GetVersion())) >= 6 /*VISTA*/? 28:20);
		 }
private: System::Void mDirTree_Click(System::Object^  sender, System::EventArgs^  e) {
			 mDirTree->Checked = !mDirTree->Checked;
			 splitContainer1->Panel1Collapsed = !mDirTree->Checked;
		 }
private: System::Void mSkinProLight_Click(System::Object^  sender, System::EventArgs^  e) {
			 tAddr->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;			 
			 tMain->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;	
			 mMain->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			 sStatusbar->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			 tContainer->TopToolStripPanel->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			 tContainer->LeftToolStripPanel->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			 tContainer->RightToolStripPanel->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			 tContainer->BottomToolStripPanel->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			 
			 Application::VisualStyleState = System::Windows::Forms::VisualStyles::VisualStyleState::NonClientAreaEnabled;

			 mSkinProfessional->Checked = false;
			 mSkinProLight->Checked = true;
			 mSkinSystem->Checked = false;

			 settings::skin = 1;
		 }
private: System::Void lFiles_ItemActivate(System::Object^  sender, System::EventArgs^  e) {
			 long hFile;
			 struct _wfinddatai64_t c_file;
			 wstring arch_name;

			 if(lFiles->SelectedItems->Count == 0)
				 return;
			 
			 hFile = _wfindfirsti64((curDir + (wchar_t*)System::Runtime::InteropServices::Marshal::StringToHGlobalUni(lFiles->SelectedItems[0]->Text).ToPointer()).c_str(), &c_file);
			 _findclose(hFile);

			 if((c_file.attrib & _A_SUBDIR) || lFiles->SelectedItems[0]->Text == gcnew String(L"..") || isKGBArchive(curDir + (wchar_t*)System::Runtime::InteropServices::Marshal::StringToHGlobalUni(lFiles->SelectedItems[0]->Text).ToPointer() + L"\\", arch_name) || isZIPArchive(curDir + (wchar_t*)System::Runtime::InteropServices::Marshal::StringToHGlobalUni(lFiles->SelectedItems[0]->Text).ToPointer() + L"\\", arch_name)){
				curDir = curDir + (wchar_t*)System::Runtime::InteropServices::Marshal::StringToHGlobalUni(lFiles->SelectedItems[0]->Text).ToPointer();
				cAddress->Text = gcnew String(curDir.c_str());
				bGo_Click(NULL, EventArgs::Empty);
			 }else{
				Diagnostics::ProcessStartInfo ^psi = gcnew Diagnostics::ProcessStartInfo;
				psi->FileName = gcnew String(curDir.c_str()) /*+ L"\\"*/ +  lFiles->SelectedItems[0]->Text;
				Diagnostics::Process::Start(psi);
			 }
		 }

private: System::Void tFiles_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
			 //MessageBox::Show();
			 switch(e->Action){
				 case TreeViewAction::ByKeyboard:
				 case TreeViewAction::ByMouse:
				 //case TreeViewAction::Unknown:
					cAddress->Text = e->Node->FullPath;
					bGo_Click(NULL, EventArgs::Empty);
					break;
				 
			  }

		 }

private: System::Void cAddress_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
			 if(e->KeyValue == VK_RETURN)
				bGo_Click(NULL, System::EventArgs::Empty);
		 }
private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }
private: System::Void bUp_Click(System::Object^  sender, System::EventArgs^  e) {
			 curDir = curDir + L"..\\";
			 cAddress->Text = gcnew String(curDir.c_str());
			 bGo_Click(NULL, EventArgs::Empty);
		 }
private: System::Void mOpen_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(dOpen->ShowDialog() == Windows::Forms::DialogResult::OK){
				cAddress->Text = dOpen->FileName;
				bGo_Click(NULL, EventArgs::Empty);
			 }
		 }
private: System::Void makeColumns(){
			if(settings::columns::cAttributes){
				 lFiles->Columns[6]->Width = (lFiles->Columns[6]->Width == 0 ? 70 : lFiles->Columns[6]->Width);
			 }else{
				 lFiles->Columns[6]->Width = 0;}

			 if(settings::columns::cAccessed){
				 lFiles->Columns[5]->Width = (lFiles->Columns[5]->Width == 0 ? 70 : lFiles->Columns[5]->Width);
			 }else{
				 lFiles->Columns[5]->Width = 0;}

			 if(settings::columns::cModified){
				 lFiles->Columns[4]->Width = (lFiles->Columns[4]->Width == 0 ? 70 : lFiles->Columns[4]->Width);
			 }else{
				 lFiles->Columns[4]->Width = 0;}

			 if(settings::columns::cCreated){
				 lFiles->Columns[3]->Width = (lFiles->Columns[3]->Width == 0 ? 70 : lFiles->Columns[3]->Width);
			 }else{
				 lFiles->Columns[3]->Width = 0;}

			 if(settings::columns::cType){
				 lFiles->Columns[2]->Width = (lFiles->Columns[2]->Width == 0 ? 70 : lFiles->Columns[2]->Width);
			 }else{
				 lFiles->Columns[2]->Width = 0;}

			 if(settings::columns::cSize){
				 lFiles->Columns[1]->Width = (lFiles->Columns[1]->Width == 0 ? 70 : lFiles->Columns[1]->Width);
			 }else{
				 lFiles->Columns[1]->Width = 0;}
			 
			 if(settings::columns::cFilename){
				 lFiles->Columns[0]->Width = (lFiles->Columns[0]->Width == 0 ? 140 : lFiles->Columns[0]->Width);
			 }else{
				 lFiles->Columns[0]->Width = 0;}
		 }

private: System::Void mSelectColumns_Click(System::Object^  sender, System::EventArgs^  e) {
			 ColumnSelect ^cs = gcnew ColumnSelect;
			 cs->cFilename->Checked = (lFiles->Columns[0]->Width > 0);
			 cs->cSize->Checked = (lFiles->Columns[1]->Width > 0);
			 cs->cType->Checked = (lFiles->Columns[2]->Width > 0);
			 cs->cCreated->Checked = (lFiles->Columns[3]->Width > 0);
			 cs->cLastmodified->Checked = (lFiles->Columns[4]->Width > 0);
			 cs->cLastaccessed->Checked = (lFiles->Columns[5]->Width > 0);
			 cs->cAttributes->Checked = (lFiles->Columns[6]->Width > 0);

			 if(cs->ShowDialog() == Windows::Forms::DialogResult::OK){
				 settings::columns::cFilename = cs->cFilename->Checked;
				 settings::columns::cSize = cs->cSize->Checked;
				 settings::columns::cType = cs->cType->Checked;
				 settings::columns::cCreated = cs->cCreated->Checked;
				 settings::columns::cModified = cs->cLastmodified->Checked;
				 settings::columns::cAccessed = cs->cLastaccessed->Checked;
				 settings::columns::cAttributes = cs->cAttributes->Checked;

				 makeColumns();
			 }

			 delete cs;
		 }

private: System::Void mNew_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(lFiles->SelectedItems->Count == 0){
				 MessageBox::Show(getString("selectFilesCompress"), L"KGB Archiver 2");
				 //MessageBox::Show(L"Please select files to compress!", L"KGB Archiver 2");
				 return;
			 }

			 long hFile;
			 struct _wfinddatai64_t c_file;

			 newArchive ^nArch = gcnew newArchive;
			 nArch->Icon = Drawing::Icon::FromHandle(((Drawing::Bitmap^)bNew->Image)->GetHicon());
			 
			 if(lFiles->SelectedItems->Count == 1){
				 nArch->tDestination->Text = gcnew String(curDir.c_str()) + lFiles->SelectedItems[0]->Text->Substring(0,lFiles->SelectedItems[0]->Text->Length-IO::Path::GetExtension(lFiles->SelectedItems[0]->Text)->Length) + gcnew String(L".kgb"); 
			 }else{
				 nArch->tDestination->Text = gcnew String((curDir.c_str()[curDir.length()-2] == L':' ? (wstring)(curDir + L"archive") : curDir.substr(0, curDir.size()-1)).c_str()) + gcnew String(".kgb");
			 }
			 
			 for(int i=0;i<lFiles->SelectedItems->Count;++i){
				 if(lFiles->SelectedItems[i]->Text == L"..")
					 continue;
				 else if(File::Exists(gcnew String(curDir.c_str())+lFiles->SelectedItems[i]->Text))
					 nArch->lFiles->Items->Add(lFiles->SelectedItems[i]->Text);
				 else if(Directory::Exists(gcnew String(curDir.c_str())+lFiles->SelectedItems[i]->Text))
					 nArch->lFiles->Items->Add(lFiles->SelectedItems[i]->Text+L"/");
				//else - somebody deleted our file/dir :(
			 }
			
			 nArch->curDir = gcnew String(curDir.c_str());

			 if(nArch->ShowDialog() == Windows::Forms::DialogResult::Cancel){
				 delete nArch->curDir;
				 delete nArch;
				 return;
			 }
			Diagnostics::ProcessStartInfo ^psi = gcnew Diagnostics::ProcessStartInfo;
			
			psi->WorkingDirectory = gcnew String(curDir.c_str());
			psi->FileName = IO::Path::GetDirectoryName(Application::ExecutablePath)+L"\\kgb2compressor.exe";
			psi->Arguments = String::Format(L"-mc -f{0}{1} -a{2} -l{3}{4} \"{5}\"",
				(nArch->rFormatKGB->Checked ? L'k' : L'z'), 
				(nArch->tPassword->Text->Length > 0 ? (String::Format(L" -p\"{0}\"", nArch->tPassword->Text)) : L""),
				nArch->cAlgorithm->SelectedIndex,
				nArch->cLevel->SelectedIndex,
				(nArch->cSFX->Checked ? L" -s" : L""),
				/*(nArch->cFullpath->Checked ? L" -b": L""),*/
				nArch->tDestination->Text);

			
			String ^tmpStr;
			FileStream ^tmpFile = gcnew FileStream(tmpStr = IO::Path::GetTempFileName(), FileMode::Create);
			StreamWriter ^strWriter = gcnew StreamWriter(tmpFile);

			
			psi->Arguments += L" -t\"" + tmpStr + L"\"";
			for(int i=0;i<nArch->lFiles->Items->Count;i++){
				
				/*psi->Arguments += L" \"";
				
				if(nArch->lFiles->Items[i]->Text->Length > 1)
					psi->Arguments += gcnew String(nArch->cFullpath->Checked && nArch->lFiles->Items[i]->Text[1] != L':' && nArch->lFiles->Items[i]->Text->Substring(0,2) != L"\\\\" ? curDir.c_str(): L"")+nArch->lFiles->Items[i]->Text;
				else
					psi->Arguments += gcnew String(nArch->cFullpath->Checked ? curDir.c_str(): L"")+nArch->lFiles->Items[i]->Text;
				
				psi->Arguments += L"\"";*/
				
				
				if(nArch->lFiles->Items[i]->Text->Length > 1)
					tmpStr = gcnew String(nArch->cFullpath->Checked && nArch->lFiles->Items[i]->Text[1] != L':' && nArch->lFiles->Items[i]->Text->Substring(0,2) != L"\\\\" ? curDir.c_str(): L"")+nArch->lFiles->Items[i]->Text;
				else
					tmpStr = gcnew String(nArch->cFullpath->Checked ? curDir.c_str(): L"")+nArch->lFiles->Items[i]->Text;
				
				strWriter->WriteLine(tmpStr);

				delete tmpStr;
			}
			strWriter->Flush();
			tmpFile->Close();

			//MessageBox::Show(psi->Arguments);

			Diagnostics::Process::Start(psi);
			
			delete nArch->curDir;
			delete nArch;
			delete psi;
			delete tmpFile;
			//delete strWriter;
		 }
private: System::Void mDelete_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(lFiles->SelectedItems->Count == 0){
				 MessageBox::Show(getString("SelectFilesDelete"), L"KGB Archiver 2");
				 //MessageBox::Show(L"Please select file(s) to delete!", L"KGB Archiver 2");
				 return;
			 }
			 if(MessageBox::Show(getString("ConfirmDel"), L"KGB Archiver 2", MessageBoxButtons::YesNo) == Windows::Forms::DialogResult::Yes){
				 //SetCurrentDirectory(curDir.c_str());//make sure if subdirectory is not used by KGB
				 fsWatcher->EnableRaisingEvents = false;
				 //fsWatcher->Path = gcnew String(L"c:\\");
				 //IO::Directory::SetCurrentDirectory(gcnew String(L"c:\\"));
				 long hFile;
				 struct _wfinddatai64_t c_file;

				 for(int i=0;i<lFiles->SelectedItems->Count;i++){
					if((hFile = _wfindfirsti64((curDir + (wchar_t*)System::Runtime::InteropServices::Marshal::StringToHGlobalUni(lFiles->SelectedItems[i]->Text).ToPointer()).c_str(), &c_file)) == -1L){
						_findclose(hFile);
						continue;
					}
					_findclose(hFile);

					try{
						if(c_file.attrib & _A_SUBDIR){
							IO::Directory::Delete(gcnew String(curDir.c_str()) + lFiles->SelectedItems[i]->Text, true);
						}else{
							IO::File::Delete(gcnew String(curDir.c_str()) + lFiles->SelectedItems[i]->Text);
						}
					}catch(Exception^ e){
						MessageBox::Show(e->Message, L"KGB Archiver 2");
					}					
					//}
				 }
				 cAddress->Text = gcnew String(curDir.c_str());
				 bGo_Click(NULL, EventArgs::Empty);
			}
		 }

private: System::Void mFileList_Opening(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {
			 wstring arch_name;
			 if(lFiles->SelectedItems->Count == 0){
				 mShellCut->Enabled = false;
				 mShellCopy->Enabled = false;
				
			 }else{
				 mShellCut->Enabled = true;
				 mShellCopy->Enabled = true;
				 //mShellPaste->Enabled = false;
			 }
			 mShellPaste->Enabled = Clipboard::ContainsFileDropList();
			 if(mShellPaste->Enabled){
				 Specialized::StringCollection ^str = Clipboard::GetFileDropList();
				 for(int i=0;i<str->Count;i++){
					 if((!File::Exists(str[i]) && !Directory::Exists(str[i])) || gcnew String(curDir.c_str()) == FileInfo(str[i]).DirectoryName + L"\\"){
						 mShellPaste->Enabled = false;
						 break;
					 }
				 }
				 delete str;
			 }

			 if(lFiles->SelectedItems->Count){
				 mShellRename->Visible = true;
				 mShellDelete->Visible = true;
				 mShellWipe->Visible = true;
				 mShellCompress->Visible = true;
				 mShellProperties->Visible = true;
				 mShellSeparator3->Visible = true;
				 mShellSeparator4->Visible = true;
			 }else{
				 mShellRename->Visible = false;
				 mShellDelete->Visible = false;
				 mShellWipe->Visible = false;
				 mShellCompress->Visible = false;
				 mShellProperties->Visible = false;
				 mShellSeparator3->Visible = false;
				 mShellSeparator4->Visible = false;
			 }

			 if(isKGBArchive(curDir, arch_name) || isZIPArchive(curDir, arch_name)){
				 mShellCompress->Enabled = false;
				 mShellCreateDir->Enabled = false;
				 mShellRename->Enabled = false;
				 mShellDelete->Enabled = false;
				 mShellWipe->Enabled = false;
				 mShellCut->Enabled = false;
				 mShellCopy->Enabled = false;
				 mShellPaste->Enabled = false;
			 }else{
				 mShellCompress->Enabled = true;
				 mShellCreateDir->Enabled = true;
				 mShellRename->Enabled = true;
				 mShellDelete->Enabled = true;
				 mShellWipe->Enabled = true;
			 }
		 }
/*private: System::Void fsWatcher_Changed(System::Object^  sender, System::IO::FileSystemEventArgs^  e) {
			 /*if(e->ChangeType & IO::WatcherChangeTypes::Changed){
				 String ^file = IO::Path::GetFileName(e->FullPath);
				 IO::FileInfo ^fi = gcnew IO::FileInfo(e->FullPath);
				 SHFILEINFO shfi = getIcon((wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(e->FullPath).ToPointer());
				 String ^attr = gcnew String(L"");
				 int idx;

				 for(idx = 0; idx < lFiles->Items->Count; idx++){
					 if(lFiles->Items[idx]->Text == file)
						break;
				 }
				  MessageBox::Show(file);
				 /*if(e->ChangeType == IO::WatcherChangeTypes::Renamed)
					 MessageBox::Show(e->FullPath);* /
				 
				 if(fi->Attributes != IO::FileAttributes::Directory)
					lFiles->Items[idx]->SubItems[0]->Text = gcnew String(convertUnits(fi->Length).c_str());
				 lFiles->Items[idx]->SubItems[1]->Text = gcnew String(shfi.szTypeName);
				 lFiles->Items[idx]->SubItems[2]->Text = fi->CreationTime.ToShortDateString();
				 lFiles->Items[idx]->SubItems[3]->Text = fi->LastWriteTime.ToShortDateString();
				 lFiles->Items[idx]->SubItems[4]->Text = fi->LastAccessTime.ToShortDateString();
				 
				 attr = attr + (fi->Attributes == IO::FileAttributes::Directory?L"d":L"-");
				 attr = attr + L"r";
				 attr = attr + (!(fi->Attributes == IO::FileAttributes::ReadOnly)?L"w":L"-");
				 attr = attr + (fi->Attributes == IO::FileAttributes::Archive?L"a":L"-");
				 attr = attr + (fi->Attributes == IO::FileAttributes::Hidden?L"h":L"-");
				 attr = attr + (fi->Attributes == IO::FileAttributes::System?L"s":L"-");

				 lFiles->Items[idx]->SubItems[5]->Text = attr;

			 }
			 
			 /*cAddress->Text = gcnew String(curDir.c_str());
			 bGo_Click(NULL, EventArgs::Empty);* /
		 }*/

private: System::Void mOptions_Click(System::Object^  sender, System::EventArgs^  e) {
			 Options ^fOptions = gcnew Options();
			 restartRequied = false;
			 
			 fOptions->cUpdate->Checked = settings::general::checkUpdates;
			 fOptions->cSaveState->Checked = settings::general::saveWindowState;
			 fOptions->nRecentItemsCount->Value = settings::general::recentItemsCount;

			 fOptions->cArchiveTree->Checked = settings::fileManager::showArchiveTree;
			 fOptions->cHidden->Checked = settings::fileManager::showHidden;
			 fOptions->cFileGroups->Checked = settings::fileManager::showFileGroups;

			 ArrayList ^languages = getLanguages();
			 for(int i=0;i<languages->Count;++i){
				fOptions->cLang->Items->Add(dynamic_cast<CultureInfo^>(languages[i])->NativeName);
			 }

			 CultureInfo ^ci;
			 try{
				 ci = gcnew CultureInfo(settings::general::language);
			 }catch(Exception ^){
				 ci = gcnew CultureInfo(0x09);
			 }
			 
			 fOptions->cLang->Text = ci->NativeName;
			 delete ci;

			 try{
				 //MessageBox::Show((String^)Registry::GetValue(L"HKEY_CLASSES_ROOT\\.kgb", L"", L""));
				 if((String^)Registry::GetValue(L"HKEY_CLASSES_ROOT\\.kgb", L"", L"") == L"KGBArchiver2_kgb"){
					 fOptions->clAssoc->SetItemChecked(0, true);
				 }
			 }catch(Exception^){}
			 try{
				 if((String^)Registry::GetValue(L"HKEY_CLASSES_ROOT\\.zip", L"", L"") == L"KGBArchiver2_zip"){
					 fOptions->clAssoc->SetItemChecked(1, true);
				 }
			 }catch(Exception^){}

			 fOptions->cCompressTo->Checked = settings::explorerExtension::Compress;
			 fOptions->cExtractTo->Checked = settings::explorerExtension::Decompress;

			 if(fOptions->ShowDialog() == Windows::Forms::DialogResult::OK){
				 if(settings::general::language != dynamic_cast<CultureInfo^>(languages[fOptions->cLang->SelectedIndex])->LCID)
					 restartRequied = true;
				 settings::general::language = dynamic_cast<CultureInfo^>(languages[fOptions->cLang->SelectedIndex])->LCID;
				 settings::general::checkUpdates = fOptions->cUpdate->Checked;
				 settings::general::saveWindowState = fOptions->cSaveState->Checked;
				 settings::general::recentItemsCount = (int)fOptions->nRecentItemsCount->Value;

				 settings::fileManager::showArchiveTree = fOptions->cArchiveTree->Checked;
				 settings::fileManager::showHidden = fOptions->cHidden->Checked;
				 settings::fileManager::showFileGroups = fOptions->cFileGroups->Checked;

				 settings::explorerExtension::Compress = fOptions->cCompressTo->Checked;
				 settings::explorerExtension::Decompress = fOptions->cExtractTo->Checked;
				 Registry::SetValue(L"HKEY_CURRENT_USER\\Software\\KGB Archiver 2\\explorerExtension", L"compress", (int)settings::explorerExtension::Compress);
				 Registry::SetValue(L"HKEY_CURRENT_USER\\Software\\KGB Archiver 2\\explorerExtension", L"decompress", (int)settings::explorerExtension::Decompress);

				 for(int i=0;i<fOptions->clAssoc->CheckedItems->Count;i++){
					 Registry::SetValue(gcnew String(L"HKEY_CLASSES_ROOT\\") + fOptions->clAssoc->CheckedItems[i]->ToString()->Substring(1), L"", gcnew String(L"KGBArchiver2_")+fOptions->clAssoc->CheckedItems[i]->ToString()->Substring(2));
					 Registry::SetValue(gcnew String(L"HKEY_CLASSES_ROOT\\KGBArchiver2_") + fOptions->clAssoc->CheckedItems[i]->ToString()->Substring(2) + L"\\DefaultIcon", L"", gcnew String(getHome().c_str())+fOptions->clAssoc->CheckedItems[i]->ToString()->Substring(2)+L".ico");
					 Registry::SetValue(gcnew String(L"HKEY_CLASSES_ROOT\\KGBArchiver2_") + fOptions->clAssoc->CheckedItems[i]->ToString()->Substring(2) + L"\\shell\\open\\command", L"", gcnew String(getHome().c_str())+L"KGB Archiver 2 .net.exe \"%1\"");
				 }
				 SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
			 }

			 delete languages;
			 delete fOptions;

			 lFiles->ShowGroups = settings::fileManager::showFileGroups;

			 bGo_Click(NULL, EventArgs::Empty); //update filemanager settings

			 if(restartRequied)
				 this->Close();
		 }
private: System::Void mAbout_Click(System::Object^  sender, System::EventArgs^  e) {
			 About ^fAbout = gcnew About();
			 fAbout->ShowDialog();
		 }
private: System::Void lFiles_ColumnClick(System::Object^  sender, System::Windows::Forms::ColumnClickEventArgs^  e) {
			 if(lastCol == e->Column)
				 lFiles->Sorting = SortOrder::Descending;
			 else
				 lFiles->Sorting = SortOrder::Ascending;

			 lFiles->ListViewItemSorter = gcnew ListViewItemComparer(e->Column);

			 lFiles->Sorting = SortOrder::None;
		 }
private: System::Void mWipe_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(lFiles->SelectedItems->Count == 0){
				 MessageBox::Show(getString("SelectFilesWipe"), L"KGB Archiver 2");
				 //MessageBox::Show(L"Please select file(s) to wipe!", L"KGB Archiver 2");
				 return;
			 }
			 if(MessageBox::Show(getString("ConfirmWipe"), L"KGB Archiver 2", MessageBoxButtons::YesNo) == Windows::Forms::DialogResult::No)
				 return;
			 
			 pStatus->Visible = true;
			 pStatus->Maximum = lFiles->SelectedItems->Count << 1;
			 sStatus->Text = getString("wiping");//gcnew String(L"Wiping files...");
			 Update();

			 int err;
			 fsWatcher->EnableRaisingEvents = false;
			 //fsWatcher->Path = gcnew String(L"c:\\");
			 
			 for(int i=0;i<lFiles->SelectedItems->Count;i++){
				 if(err = wipe(curDir + (wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(lFiles->SelectedItems[i]->Text).ToPointer(), 0)){
					 MessageBox::Show(getString("CannotWipe") + lFiles->SelectedItems[i]->Text + getString("WipeAccessDenied") + String::Format(L"{0}", err), L"KGB Archiver 2");
					 pStatus->Value += 2;
					 continue;
				 }
				 pStatus->Value++;
				 Update();
				 if(err = wipe(curDir + (wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(lFiles->SelectedItems[i]->Text).ToPointer())){
					 MessageBox::Show(getString("CannotWipe") + lFiles->SelectedItems[i]->Text + getString("WipeAccessDenied") + String::Format(L"{0}", err), L"KGB Archiver 2");
				 }
				 pStatus->Value++;
			 }

			 //fsWatcher->EnableRaisingEvents = true;

			 pStatus->Value = 0;

			 cAddress->Text = gcnew String(curDir.c_str());
			 bGo_Click(NULL, EventArgs::Empty);
		 }
private: System::Void fsWatcher_Created(System::Object^  sender, System::IO::FileSystemEventArgs^  e) {
			 try{
				 String ^file = IO::Path::GetFileName(e->FullPath);
				IO::FileInfo ^fi = gcnew IO::FileInfo(e->FullPath);
				SHFILEINFO shfi;
				String ^attr = gcnew String(L"");

				shfi = getIcon((wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(e->FullPath).ToPointer());
				 
				if(fi->Attributes != IO::FileAttributes::Directory){
					if(shfi.hIcon != NULL){
						lFiles->SmallImageList->Images->Add(System::Drawing::Icon::FromHandle(System::IntPtr(shfi.hIcon)));
						lFiles->Items->Add(file, file, lFiles->SmallImageList->Images->Count-1);
					}else
						lFiles->Items->Add(file, file);
				 
				
					lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(gcnew String(convertUnits(fi->Length).c_str()));
					lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(gcnew String(shfi.szTypeName));
					
					if(file->Length > 3){
						/*heuristics*/
						if(file->ToLower()->Substring(file->Length-3) == L"kgb"){
							if(isKGBArchive((wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(e->FullPath).ToPointer()))
								lFiles->Items[lFiles->Items->Count-1]->Group = lFiles->Groups[L"lvgArch"];
							
						}else if(file->ToLower()->Substring(file->Length-3) == L"exe"){
							if(isKGBArchive((wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(e->FullPath).ToPointer()) || isZIPArchive((wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(e->FullPath).ToPointer()))
								lFiles->Items[lFiles->Items->Count-1]->Group = lFiles->Groups[L"lvgArch"];
							
						}else if(file->ToLower()->Substring(file->Length-3) == L"zip"){
							if(isZIPArchive((wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(e->FullPath).ToPointer()))
								lFiles->Items[lFiles->Items->Count-1]->Group = lFiles->Groups[L"lvgArch"];
							
						}
					}

					if(lFiles->Items[lFiles->Items->Count-1]->Group != lFiles->Groups[L"lvgArch"])
						lFiles->Items[lFiles->Items->Count-1]->Group = lFiles->Groups[L"lvgFile"];

				}else{
					lFiles->Items->Add(file, file, 0);
					lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(L"");
					lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(getString("TypeDir"));

					lFiles->Items[lFiles->Items->Count-1]->Group = lFiles->Groups[L"lvgDir"];
				}
				
				lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(fi->CreationTime.ToShortDateString());
				lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(fi->LastAccessTime.ToShortDateString());
				lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(fi->LastWriteTime.ToShortDateString());
				
				attr = attr + (fi->Attributes == IO::FileAttributes::Directory?L"d":L"-");
				attr = attr + L"r";
				attr = attr + (!(fi->Attributes == IO::FileAttributes::ReadOnly)?L"w":L"-");
				attr = attr + (fi->Attributes == IO::FileAttributes::Archive?L"a":L"-");
				attr = attr + (fi->Attributes == IO::FileAttributes::Hidden?L"h":L"-");
				attr = attr + (fi->Attributes == IO::FileAttributes::System?L"s":L"-");

				lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(attr);
			 }catch(Exception ^){
			 
			 }
		 }
private: System::Void fsWatcher_Deleted(System::Object^  sender, System::IO::FileSystemEventArgs^  e) {
			String ^file = IO::Path::GetFileName(e->FullPath);
			
			for(int i=0;i<lFiles->Items->Count;i++){
				if(lFiles->Items[i]->Text == file){
					lFiles->Items[i]->Remove();
					break;
				}
			}
		 }
private: System::Void fsWatcher_Renamed(System::Object^  sender, System::IO::RenamedEventArgs^  e) {
			 String ^oldFile = IO::Path::GetFileName(e->OldFullPath);
			 String ^newFile = IO::Path::GetFileName(e->FullPath);

			 if(IO::Path::GetDirectoryName(e->OldFullPath) != IO::Path::GetDirectoryName(e->FullPath)){
				for(int i=0;i<lFiles->Items->Count;i++){
					if(lFiles->Items[i]->Text == oldFile){
						lFiles->Items[i]->Remove();
						return;
					}
				}
			 }
			
			for(int i=0;i<lFiles->Items->Count;i++){
				if(lFiles->Items[i]->Text == oldFile){
					lFiles->Items[i]->Text = newFile;
					break;
				}
			} 
		 }
private: System::Void mInformation_Click(System::Object^  sender, System::EventArgs^  e) {
			wstring arch_name;
			if(isKGBArchive(curDir, arch_name)){
				compressInfo cmsi;
				information ^fInfo = gcnew information();
				newArchive ^nArch = gcnew newArchive();
				IO::FileInfo ^fi = gcnew IO::FileInfo(gcnew String(arch_name.c_str()));
				__int64 size=0;
				__int64 compressed = fi->Length;

				readHeader(arch_name, &cmsi);

				for(__int64 i=0;i<cmsi.files.size();i++){
					size += cmsi.files[i].size;
				}

				fInfo->Icon = Drawing::Icon::FromHandle(((Drawing::Bitmap^)bInfo->Image)->GetHicon());
				fInfo->lVersion->Text = String::Format(L"v{0}", cmsi.version);
				
				fInfo->lLevel->Text = nArch->cLevel->Items[cmsi.algoMode]->ToString();
				if(cmsi.algorithm == 0){
					fInfo->lLevel->Text = getString("NoCompression");//gcnew String(L"No compression"); 
					fInfo->lAlgorithm->Text = getString("NoCompression");//gcnew String(L"No compression");
				}else{
					fInfo->lAlgorithm->Text = String::Format(L"PAQ{0}", cmsi.algorithm);
				}
				if(cmsi.encryption.enabled)
					fInfo->lPassword->Text = getString("Yes");//gcnew String(L"Yes");
				else
					fInfo->lPassword->Text = getString("No");//gcnew String(L"No");
				fInfo->lNumFiles->Text = String::Format(L"{0}", cmsi.files.size());
				fInfo->lBeforeSize->Text = String::Format(L"{0} ({1}B)", gcnew String(convertUnits(size).c_str()), size);
				fInfo->lCompressed->Text = String::Format(L"{0} ({1}B)", gcnew String(convertUnits(compressed).c_str()), compressed);
				fInfo->lRatio->Text = String::Format(L"{0:F1}%", (float)(compressed*1000/size)/10.0f);
				
				delete fi;
				delete nArch;
				fInfo->ShowDialog();
				delete fInfo;
			}else if(isZIPArchive(curDir, arch_name)){
				information ^fInfo = gcnew information();
				newArchive ^nArch = gcnew newArchive();
				IO::FileInfo ^fi = gcnew IO::FileInfo(gcnew String(arch_name.c_str()));
				CZipArchive zip;
				CZipFileHeader header;
				__int64 size = 0;
				__int64 level = 0;

				zip.Open(arch_name.c_str(), CZipArchive::OpenMode::zipOpenReadOnly);
				//zip.GetFileInfo(header, 0);

				for(__int64 i=0;i<zip.GetCount(false);i++){
					zip.GetFileInfo(header, i);
					if(header.IsDirectory())
						continue;

					size += header.m_uUncomprSize;
					level += header.GetCompressionLevel();
				}
				level = abs((int)level) / zip.GetCount(true);

				fInfo->Icon = Drawing::Icon::FromHandle(((Drawing::Bitmap^)bInfo->Image)->GetHicon());
				fInfo->lType->Text = gcnew String(L"ZIP");
				
				fInfo->lNumFiles->Text = String::Format(L"{0}", zip.GetCount(true));
				if(header.IsEncrypted())
					fInfo->lPassword->Text = gcnew String(L"Yes");
				else
					fInfo->lPassword->Text = gcnew String(L"No");
				fInfo->lBeforeSize->Text = String::Format(L"{0} ({1}B)", gcnew String(convertUnits(size).c_str()), size);
				fInfo->lCompressed->Text = String::Format(L"{0} ({1}B)", gcnew String(convertUnits(fi->Length).c_str()), fi->Length);
				fInfo->lRatio->Text = String::Format(L"{0:F1}%", (float)(fi->Length*1000/size)/10.0f);
				
				fInfo->lLevel->Text = nArch->cLevel->Items[level]->ToString();

				if(header.m_uMethod == 0){
					fInfo->lLevel->Text = getString("NoCompression");//gcnew String(L"No compression");
					fInfo->lAlgorithm->Text = getString("NoCompression");//gcnew String(L"No compression");
				}else if(header.m_uMethod == 8)
					fInfo->lAlgorithm->Text = gcnew String(L"Deflate");
				else if(header.m_uMethod == 9)
					fInfo->lAlgorithm->Text = gcnew String(L"Deflate64");
				else
					fInfo->lAlgorithm->Text = String::Format(getString("UnknownAlgorithm"), header.m_uMethod);

				
				fInfo->lVersion->Text = String::Format(L"v{0}", (float)header.m_uVersionMadeBy/10.0f);

				zip.Close();
				delete fi;
				delete nArch;
				fInfo->ShowDialog();
				delete fInfo;
			}
		 }
private: System::Void mExtract_Click(System::Object^  sender, System::EventArgs^  e) {
			wstring arch_name;
			Extract ^fExtract;
			if(isKGBArchive(curDir, arch_name)){//make sure if it's a archive
				compressInfo cmsi;
				fExtract = gcnew Extract();
				fExtract->Icon = Drawing::Icon::FromHandle(((Drawing::Bitmap^)bExtract->Image)->GetHicon());
				fExtract->tDestination->Text = gcnew String(arch_name.c_str());
				fExtract->tDestination->Text = fExtract->tDestination->Text->Substring(0, fExtract->tDestination->Text->LastIndexOf(L'\\', fExtract->tDestination->Text->Length-1)+1);
				
				readHeader(arch_name, &cmsi);
				
				fExtract->gPassword->Enabled = cmsi.encryption.enabled;

				for(int i=0;i<cmsi.files.size();++i){
					fExtract->lFiles->Items->Add(gcnew String(cmsi.files[i].name));
					fExtract->lFiles->Items[i]->Checked = true;
				}

				fExtract->format::type = 'k';
				fExtract->format::algorithm = cmsi.algorithm;
				fExtract->format::level = cmsi.algoMode;
	
			}else if(isZIPArchive(curDir, arch_name)){//make sure if it's a archive
				fExtract = gcnew Extract();
				fExtract->Icon = Drawing::Icon::FromHandle(((Drawing::Bitmap^)bExtract->Image)->GetHicon());
				fExtract->tDestination->Text = gcnew String(arch_name.c_str());
				fExtract->tDestination->Text = fExtract->tDestination->Text->Substring(0, fExtract->tDestination->Text->LastIndexOf(L'\\', fExtract->tDestination->Text->Length-1)+1);
				
				CZipArchive zip;
				CZipFileHeader zipHeader;
				zip.Open(arch_name.c_str(), CZipArchive::OpenMode::zipOpenReadOnly);
				
				//MessageBox::Show(String::Format("{0}", zip.GetEncryptionMethod()));
				/*if()
					fExtract->gPassword->Enabled = false;
				else
					fExtract->gPassword->Enabled = true;*/
				fExtract->gPassword->Enabled = zip[0]->IsEncrypted();

				for(int i=0;i<zip.GetCount(false);++i){
					zip.GetFileInfo(zipHeader, i);

					fExtract->lFiles->Items->Add(gcnew String(zipHeader.GetFileName().c_str()));
					fExtract->lFiles->Items[i]->Checked = true;
				}
				
				zip.Close();

				fExtract->format::type = 'z';
	
			}else
				return;
			
			if(fExtract->ShowDialog() != Windows::Forms::DialogResult::OK){
				delete fExtract;
				return;
			}

			if(fExtract->cDelete->Checked){
				for(int i=arch_name.length()-1;i>=0;i--){
					if(arch_name[i] == L'\\' || arch_name[i] == L'/'){
						cAddress->Text = gcnew String(arch_name.substr(0, i).c_str());
						bGo_Click(NULL, EventArgs::Empty);
						break;
					}
				}
			}
			Diagnostics::ProcessStartInfo ^psi = gcnew Diagnostics::ProcessStartInfo;
				
			psi->WorkingDirectory = gcnew String(curDir.c_str());
			psi->FileName = IO::Path::GetDirectoryName(Application::ExecutablePath)+L"\\kgb2compressor.exe";
			psi->Arguments = String::Format(L"-md{0} -o{1} {2}{3}{4}\"{5}\" \"{6}\"",
				(fExtract->tPasswd->Text->Length > 0 ? (String::Format(L" -p\"{0}\"", fExtract->tPasswd->Text)) : L""),
				(fExtract->rAsk->Checked ? L'a' : (fExtract->rForce->Checked ? L'f' : L'n')),
				(fExtract->cOpenDir->Checked ? L"-d ": L""),
				(fExtract->cDelete->Checked ? L"-r ": L""),
				(fExtract->cRunBackground->Checked ? L"-b ":L""),
				gcnew String(arch_name.c_str()),
				(fExtract->tDestination->Text[fExtract->tDestination->Text->Length-1] == L'\\' ? fExtract->tDestination->Text->Substring(0, fExtract->tDestination->Text->Length-1) : fExtract->tDestination->Text));

			String ^tmpStr;
			FileStream ^tmpFile = gcnew FileStream(tmpStr=Path::GetTempFileName(), FileMode::Create);
			StreamWriter ^strWriter = gcnew StreamWriter(tmpFile);
			psi->Arguments += L" -t\"" + tmpStr + L"\"";
			for(int i=0;i<fExtract->lFiles->CheckedItems->Count;++i){
				//Windows::Forms::MessageBox::Show(fExtract->lFiles->CheckedItems[i]->Text);
				strWriter->WriteLine(fExtract->lFiles->CheckedItems[i]->Text);
			}
			strWriter->Flush();
			tmpFile->Close();

			Diagnostics::Process::Start(psi);
			delete psi;
			delete fExtract;
			delete tmpStr;
			delete tmpFile;
		 }
private: System::Void lFiles_AfterLabelEdit(System::Object^  sender, System::Windows::Forms::LabelEditEventArgs^  e) {
			 if(e->Label == nullptr)
				 return;

			 e->CancelEdit = true; //force use file system watcher

			 try{
				 if(IO::File::Exists(gcnew String(curDir.c_str())+lFiles->Items[e->Item]->Text))
					 IO::File::Move(gcnew String(curDir.c_str())+lFiles->Items[e->Item]->Text, gcnew String(curDir.c_str())+e->Label->Trim());
				 else if(IO::Directory::Exists(gcnew String(curDir.c_str())+lFiles->Items[e->Item]->Text))
					 IO::Directory::Move(gcnew String(curDir.c_str())+lFiles->Items[e->Item]->Text, gcnew String(curDir.c_str())+e->Label->Trim());
			 }catch(Exception ^ex){
				 MessageBox::Show(ex->Message, L"KGB Archiver 2");
			 }
		 }
private: System::Void mShellRename_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(lFiles->SelectedItems->Count > 0)
				lFiles->SelectedItems[0]->BeginEdit();
		 }
private: System::Void mShellCreateDir_Click(System::Object^  sender, System::EventArgs^  e) {
			 String ^dirName = getString("NewDirectory");//gcnew String(L"New directory");

			 for(int i=2;IO::Directory::Exists(gcnew String(curDir.c_str()) + dirName);i++){
				delete dirName;
				dirName = gcnew String(curDir.c_str()) + String::Format(getString("NewDirectoryN"), i);
			 }
			 
			 IO::FileSystemEventArgs ^ea = gcnew IO::FileSystemEventArgs(IO::WatcherChangeTypes::Created, gcnew String(curDir.c_str()), dirName);

			 fsWatcher->EnableRaisingEvents = false;

			 IO::Directory::CreateDirectory(gcnew String(curDir.c_str()) + dirName);
			 //ea->FullName = gcnew String(L"New directory");

			 fsWatcher_Created(NULL, ea);

			 fsWatcher->EnableRaisingEvents = true;

			 lFiles->Items[lFiles->Items->Count-1]->BeginEdit();

			 delete dirName;
		 }
private: System::Void bBack_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(backList.size() > 1){
				 cAddress->Text = gcnew String(backList[backList.size()-2].c_str());
				 
				 forwardList.push_back(backList[backList.size()-1]);
				 backList.pop_back();
				 backList.pop_back();
				 if(backList.size() == 0)
					bBack->Enabled = false;

				 bGo_Click(backListSender, EventArgs::Empty);
			 }
		 }
private: System::Void bBack_DropDownItemClicked(System::Object^  sender, System::Windows::Forms::ToolStripItemClickedEventArgs^  e) {
			 //MessageBox::Show(e->ClickedItem->ToString());
			 cAddress->Text = e->ClickedItem->Text;
			 
			 for(int i = 0;bBack->DropDownItems[i] != e->ClickedItem;i++){
				 forwardList.push_back(backList[backList.size()-1]);
				 backList.pop_back();
			 }
			 backList.pop_back();
			 if(backList.size() == 0)
				 bBack->Enabled = false;

			 bGo_Click(backListSender, EventArgs::Empty);
		 }
private: System::Void bForward_ButtonClick(System::Object^  sender, System::EventArgs^  e) {
			 if(forwardList.size() > 0){
				cAddress->Text = gcnew String(forwardList[forwardList.size()-1].c_str());
				 
				//backList.push_back(forwardList[forwardList.size()-1]);
				forwardList.pop_back();
				if(forwardList.size() == 0)
					bForward->Enabled = false;

				bGo_Click(backListSender, EventArgs::Empty);
			 }
		 }
private: System::Void bBack_DropDownOpening(System::Object^  sender, System::EventArgs^  e) {
			bBack->DropDownItems->Clear();

			 for(int i=backList.size()-2;i>=0;i--){
				 bBack->DropDownItems->Add(gcnew String(backList[i].c_str()));
			 }
		 }
private: System::Void bForward_DropDownOpening(System::Object^  sender, System::EventArgs^  e) {
			 bForward->DropDownItems->Clear();
	
			 for(int i=forwardList.size()-1;i>=0;i--){
				 bForward->DropDownItems->Add(gcnew String(forwardList[i].c_str()));
			 }
		 }
private: System::Void bForward_DropDownItemClicked(System::Object^  sender, System::Windows::Forms::ToolStripItemClickedEventArgs^  e) {
			 cAddress->Text = e->ClickedItem->Text;
			 
			 for(int i = 0;bForward->DropDownItems[i] != e->ClickedItem;i++){
				 backList.push_back(forwardList[forwardList.size()-1]);
				 forwardList.pop_back();
			 }
			 forwardList.pop_back();
			 if(forwardList.size() == 0)
				 bForward->Enabled = false;

			 bGo_Click(backListSender, EventArgs::Empty);
		 }
private: System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
			 String ^mainHook = gcnew String(L"HKEY_CURRENT_USER\\Software\\KGB Archiver 2");
			 String ^hook;
			 try{
				 if(settings::general::saveWindowState){
					hook = mainHook + L"\\windowState";
					Registry::SetValue(hook, L"", WindowState, RegistryValueKind::DWord);
					Registry::SetValue(hook, L"left", Left, RegistryValueKind::DWord);
					Registry::SetValue(hook, L"top", Top, RegistryValueKind::DWord);
					Registry::SetValue(hook, L"width", Width, RegistryValueKind::DWord);
					Registry::SetValue(hook, L"height", Height, RegistryValueKind::DWord);

					hook = mainHook + L"\\windowState\\menubar";
					Registry::SetValue(hook, L"left", mMain->Left, RegistryValueKind::DWord);
					Registry::SetValue(hook, L"top", mMain->Top, RegistryValueKind::DWord);
					
					hook = mainHook + L"\\windowState\\addressbar";
					Registry::SetValue(hook, L"left", tAddr->Left, RegistryValueKind::DWord);
					Registry::SetValue(hook, L"top", tAddr->Top, RegistryValueKind::DWord);
					
					hook = mainHook + L"\\windowState\\toolbar";
					Registry::SetValue(hook, L"left", tMain->Left, RegistryValueKind::DWord);
					Registry::SetValue(hook, L"top", tMain->Top, RegistryValueKind::DWord);
					
				 }
			}catch(Exception ^e){
				MessageBox::Show(e->Message, L"KGB Archiver 2");
			}
			delete mainHook;
	}
private: System::Void mCheckUpdates_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(!bwUpdate->IsBusy)
				bwUpdate->RunWorkerAsync(false);
		 }
private: System::Void bwUpdate_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
			 sStatus->Text = getString("CheckingUpdates");//gcnew String(L"Checking for updates...");
			 if(!checkForUpdates((bool)e->Argument)){
				 sStatus->Text = getString("NoUpdates");//gcnew String(L"No updates have been found");
				 //Sleep(10000);
			 }
			 //sStatus->Text = gcnew String(L"Done");
		 }
private: System::Void setattr(FileAttributes fa, properties ^fProperties, String ^filename){
			//FileAttributes fa = FileAttributes::Normal;
			if(fProperties->cArchive->CheckState == CheckState::Checked)
				fa = fa | FileAttributes::Archive;
			else if(fProperties->cArchive->CheckState == CheckState::Unchecked)
				fa = fa & ~FileAttributes::Archive;

			if(fProperties->cHidden->CheckState == CheckState::Checked)
				fa = fa | FileAttributes::Hidden;
			else if(fProperties->cHidden->CheckState == CheckState::Unchecked)
				fa = fa & ~FileAttributes::Hidden;

			if(fProperties->cReadOnly->CheckState == CheckState::Checked)
				fa = fa | FileAttributes::ReadOnly;
			else if(fProperties->cReadOnly->CheckState == CheckState::Unchecked)
				fa = fa & ~FileAttributes::ReadOnly;

			if(fProperties->cSystem->CheckState == CheckState::Checked)
				fa = fa | FileAttributes::System;
			else if(fProperties->cSystem->CheckState == CheckState::Unchecked)
				fa = fa & ~FileAttributes::System;

			//TODO
			if(fProperties->cCompressed->CheckState == CheckState::Checked)
				fa = fa | FileAttributes::Compressed;
			else if(fProperties->cCompressed->CheckState == CheckState::Unchecked)
				fa = fa & ~FileAttributes::Compressed;

			if(fProperties->cEncrypted->CheckState == CheckState::Checked)
				File::Encrypt(filename);//fa = fa | FileAttributes::Encrypted;
			else if(fProperties->cEncrypted->CheckState == CheckState::Unchecked)
				File::Decrypt(filename);//fa = fa & ~FileAttributes::Encrypted;

			File::SetAttributes(filename, fa);
		 }
private: FileAttributes getAttrFromlFiles(int i){
			 FileAttributes fa = FileAttributes::Normal;
			 try{
				 if(lFiles->SelectedItems[i]->SubItems[6]->Text[0] == L'd')
					 fa = fa | FileAttributes::Directory;
				 if(lFiles->SelectedItems[i]->SubItems[6]->Text[2] == L'-') //w - writable
					 fa = fa | FileAttributes::ReadOnly;
				 if(lFiles->SelectedItems[i]->SubItems[6]->Text[3] == L'a')
					 fa = fa | FileAttributes::Archive;
				 if(lFiles->SelectedItems[i]->SubItems[6]->Text[4] == L'h')
					 fa = fa | FileAttributes::Hidden;
				 if(lFiles->SelectedItems[i]->SubItems[6]->Text[5] == L's')
					 fa = fa | FileAttributes::System;
			 }catch(Exception ^e){
					
			 }
			 return fa;
		 }
private: System::Void mProperties_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(lFiles->SelectedItems->Count == 0)
				 return;

			 properties ^fProperties = gcnew properties();
			 wstring arch_name;
			 bool isArch;
			 //TODO: HEADER OVERWRITE
			 if(isArch=isKGBArchive(curDir, arch_name)){ //KGB archive
				 compressInfo cmsi;
				 FileAttributes fa1, fa2;
				 readHeader(arch_name, &cmsi);

				 fProperties->tFilename->ReadOnly = true;
				 fProperties->gDates->Enabled = false;
				 fProperties->gAttributes->Enabled = false;

				 if(cmsi.algorithm != 0)
					 fProperties->cCompressed->CheckState = CheckState::Checked;

				 if(cmsi.encryption.enabled)
					 fProperties->cEncrypted->CheckState = CheckState::Checked;

				 fProperties->tFilename->Text = lFiles->SelectedItems[0]->Text;
				 fa1 = getAttrFromlFiles(0);
				 fa2 = getAttrFromlFiles(0);
				 for(int i=1;i<lFiles->SelectedItems->Count;i++){
					 fProperties->tFilename->Text += L" " + lFiles->SelectedItems[i]->Text;
					 fa1 = fa1 & getAttrFromlFiles(i);
					 fa2 = fa2 | getAttrFromlFiles(i);
				 }

				 if((fa1 & FileAttributes::ReadOnly) == FileAttributes::ReadOnly)
					 fProperties->cReadOnly->CheckState = CheckState::Checked;
				 else if((fa2 & FileAttributes::ReadOnly) == FileAttributes::ReadOnly)
					 fProperties->cReadOnly->CheckState = CheckState::Indeterminate;

				 if((fa1 & FileAttributes::Archive) == FileAttributes::Archive)
					 fProperties->cArchive->CheckState = CheckState::Checked;
				 else if((fa2 & FileAttributes::Archive) == FileAttributes::Archive)
					 fProperties->cArchive->CheckState = CheckState::Indeterminate;

				 if((fa1 & FileAttributes::Hidden) == FileAttributes::Hidden)
					 fProperties->cHidden->CheckState = CheckState::Checked;
				 else if((fa2 & FileAttributes::Hidden) == FileAttributes::Hidden)
					 fProperties->cHidden->CheckState = CheckState::Indeterminate;

				 if((fa1 & FileAttributes::System) == FileAttributes::System)
					 fProperties->cSystem->CheckState = CheckState::Checked;
				 else if((fa2 & FileAttributes::System) == FileAttributes::System)
					 fProperties->cSystem->CheckState = CheckState::Indeterminate;

				 if(lFiles->SelectedItems->Count == 1){
					 SHFILEINFO shfi;
					 memset(&shfi,0,sizeof(shfi));
					 SHGetFileInfo((wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(lFiles->SelectedItems[0]->Text).ToPointer(), FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(shfi), SHGFI_ICON|SHGFI_USEFILEATTRIBUTES|/*SHGFI_SHELLICONSIZE|*/SHGFI_LARGEICON|SHGFI_TYPENAME);

					 if(shfi.hIcon != NULL)
						 fProperties->pIcon->Image = System::Drawing::Icon::FromHandle(IntPtr(shfi.hIcon))->ToBitmap();
					 
					 wstring filename = curDir.substr(arch_name.length()+1) + (wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(lFiles->SelectedItems[0]->Text).ToPointer();
					 int i = 0;
					 for(i=0;i<cmsi.files.size();i++){
						 //Windows::Forms::MessageBox::Show(gcnew String(cmsi.files[i].name) + L"\n" + gcnew String(filename.c_str()));
						 if(filename == (wstring)cmsi.files[i].name)
							break;
					 }
					 //Windows::Forms::MessageBox::Show(gcnew String(cmsi.files[i].name) + L"\n" + gcnew String(filename.c_str()));
						 
					 if(i < cmsi.files.size()){ //file
						 fProperties->gDates->Enabled = true;
						 fProperties->dtCreated->Value = DateTime(1970, 1, 1).AddSeconds(cmsi.files[i].time_create);
						 fProperties->dtModified->Value = DateTime(1970, 1, 1).AddSeconds(cmsi.files[i].time_write);
						 fProperties->dtAccessed->Value = DateTime(1970, 1, 1).AddSeconds(cmsi.files[i].time_access);
						 fProperties->lType->Text = getString("FileType") + gcnew String((wchar_t*)shfi.szTypeName);
					 }else{ //directory
						 fProperties->lType->Text = getString("FileType") + getString("TypeDirectory");//gcnew String(L"Type: Directory");
					 }
				}
			 }else if(isArch = isZIPArchive(curDir, arch_name)){
				 CZipArchive zip;
				 CZipFileHeader zh;
				 FileAttributes fa1, fa2;
				 int idx = 0;
				 __int64 level = 0;
				 bool contains;

				 zip.Open(arch_name.c_str(), CZipArchive::OpenMode::zipOpenReadOnly);

				 for(idx = 0;idx<zip.GetCount(true);++idx){
					 zip.GetFileInfo(zh, idx);
					 contains = false;
					 //Windows::Forms::MessageBox::Show(gcnew String(zh.GetFileName().c_str()));
					 for(int i=0;i<lFiles->SelectedItems->Count;++i){
						 if(zh.GetFileName() == (wstring)(curDir.substr(arch_name.length()+1) + (wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(lFiles->SelectedItems[i]->Text).ToPointer())){
							 contains = true;
							 break;
						 }
					 }
					 if(!contains)
						 continue;

					 level += zh.GetCompressionLevel();
				 }
				 level = abs((int)level)/lFiles->SelectedItems->Count;

				 fProperties->tFilename->ReadOnly = true;
				 fProperties->gDates->Enabled = false;
				 fProperties->gAttributes->Enabled = false;

				 if(level != 0)
					 fProperties->cCompressed->CheckState = CheckState::Checked;

				 if(zh.IsEncrypted())
					 fProperties->cEncrypted->CheckState = CheckState::Checked;

				 fProperties->tFilename->Text = lFiles->SelectedItems[0]->Text;
				 fa1 = getAttrFromlFiles(0);
				 fa2 = getAttrFromlFiles(0);
				 for(int i=1;i<lFiles->SelectedItems->Count;i++){
					 fProperties->tFilename->Text += L" " + lFiles->SelectedItems[i]->Text;
					 fa1 = fa1 & getAttrFromlFiles(i);
					 fa2 = fa2 | getAttrFromlFiles(i);
				 }

				 if((fa1 & FileAttributes::ReadOnly) == FileAttributes::ReadOnly)
					 fProperties->cReadOnly->CheckState = CheckState::Checked;
				 else if((fa2 & FileAttributes::ReadOnly) == FileAttributes::ReadOnly)
					 fProperties->cReadOnly->CheckState = CheckState::Indeterminate;

				 if((fa1 & FileAttributes::Archive) == FileAttributes::Archive)
					 fProperties->cArchive->CheckState = CheckState::Checked;
				 else if((fa2 & FileAttributes::Archive) == FileAttributes::Archive)
					 fProperties->cArchive->CheckState = CheckState::Indeterminate;

				 if((fa1 & FileAttributes::Hidden) == FileAttributes::Hidden)
					 fProperties->cHidden->CheckState = CheckState::Checked;
				 else if((fa2 & FileAttributes::Hidden) == FileAttributes::Hidden)
					 fProperties->cHidden->CheckState = CheckState::Indeterminate;

				 if((fa1 & FileAttributes::System) == FileAttributes::System)
					 fProperties->cSystem->CheckState = CheckState::Checked;
				 else if((fa2 & FileAttributes::System) == FileAttributes::System)
					 fProperties->cSystem->CheckState = CheckState::Indeterminate;
				

				zip.Close();
			 }else{

				 if(lFiles->SelectedItems->Count > 1){
					 fProperties->tFilename->ReadOnly = true;
					 fProperties->gDates->Enabled = false;
					 /*fProperties->cReadOnly->ThreeState = true;
					 fProperties->cArchive->ThreeState = true;
					 fProperties->cHidden->ThreeState = true;
					 fProperties->cSystem->ThreeState = true;*/

					 FileAttributes fa1;// = gcnew FileAttributes();
					 FileAttributes fa2;// = gcnew FileAttributes();

					FileInfo ^fi = gcnew FileInfo(gcnew String(curDir.c_str()) + lFiles->SelectedItems[0]->Text);
					fa1 = fi->Attributes;
					fa2 = fi->Attributes;
					delete fi;

					 for(int i=0;i<lFiles->SelectedItems->Count;i++){
						 fProperties->tFilename->Text += lFiles->SelectedItems[i]->Text + L"; ";
						 
						FileInfo ^fi = gcnew FileInfo(gcnew String(curDir.c_str()) + lFiles->SelectedItems[i]->Text);
						
						fa1 = fa1 & fi->Attributes;
						fa2 = fa2 | fi->Attributes;

						delete fi;
					 }
					 
					 if((fa1 & FileAttributes::ReadOnly) == FileAttributes::ReadOnly)
						 fProperties->cReadOnly->CheckState = CheckState::Checked;
					 else if((fa2 & FileAttributes::ReadOnly) == FileAttributes::ReadOnly)
						 fProperties->cReadOnly->CheckState = CheckState::Indeterminate;

					 if((fa1 & FileAttributes::Archive) == FileAttributes::Archive)
						 fProperties->cArchive->CheckState = CheckState::Checked;
					 else if((fa2 & FileAttributes::Archive) == FileAttributes::Archive)
						 fProperties->cArchive->CheckState = CheckState::Indeterminate;

					 if((fa1 & FileAttributes::Hidden) == FileAttributes::Hidden)
						 fProperties->cHidden->CheckState = CheckState::Checked;
					 else if((fa2 & FileAttributes::Hidden) == FileAttributes::Hidden)
						 fProperties->cHidden->CheckState = CheckState::Indeterminate;

					 if((fa1 & FileAttributes::System) == FileAttributes::System)
						 fProperties->cSystem->CheckState = CheckState::Checked;
					 else if((fa2 & FileAttributes::System) == FileAttributes::System)
						 fProperties->cSystem->CheckState = CheckState::Indeterminate;

					 if((fa1 & FileAttributes::Compressed) == FileAttributes::Compressed)
						 fProperties->cCompressed->CheckState = CheckState::Checked;
					 else if((fa2 & FileAttributes::Compressed) == FileAttributes::Compressed)
						 fProperties->cCompressed->CheckState = CheckState::Indeterminate;

					 if((fa1 & FileAttributes::Encrypted) == FileAttributes::Encrypted)
						 fProperties->cEncrypted->CheckState = CheckState::Checked;
					 else if((fa2 & FileAttributes::Encrypted) == FileAttributes::Encrypted)
						 fProperties->cEncrypted->CheckState = CheckState::Indeterminate;


				 }else{
					 SHFILEINFO shfi;
					 fProperties->tFilename->Text = lFiles->SelectedItems[0]->Text;

					 //shfi = getIcon(curDir + (wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(lFiles->SelectedItems[0]->Text).ToPointer());
					 memset(&shfi,0,sizeof(shfi));
					 SHGetFileInfo((wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(lFiles->SelectedItems[0]->Text).ToPointer(), FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(shfi), SHGFI_ICON|SHGFI_USEFILEATTRIBUTES|/*SHGFI_SHELLICONSIZE|*/SHGFI_LARGEICON|SHGFI_TYPENAME);

					 if(shfi.hIcon != NULL)
						 fProperties->pIcon->Image = System::Drawing::Icon::FromHandle(IntPtr(shfi.hIcon))->ToBitmap();


					 FileInfo ^fi = gcnew FileInfo(gcnew String(curDir.c_str()) + lFiles->SelectedItems[0]->Text);
					 //fa = fi->Attributes;
					 fProperties->dtCreated->Value = fi->CreationTime;
					 fProperties->dtModified->Value = fi->LastWriteTime;
					 fProperties->dtAccessed->Value = fi->LastAccessTime;

					 DirectoryInfo ^di = gcnew DirectoryInfo(gcnew String(curDir.c_str()) + lFiles->SelectedItems[0]->Text);
					 if(di->Exists)
						fProperties->lType->Text = getString("FileType") + getString("TypeDirectory");//gcnew String(L"Type: Directory");
					 else
						fProperties->lType->Text = getString("FileType") + gcnew String((wchar_t*)shfi.szTypeName);
					 

					 if((fi->Attributes & FileAttributes::ReadOnly) == FileAttributes::ReadOnly)
						 fProperties->cReadOnly->CheckState = CheckState::Checked;

					 if((fi->Attributes & FileAttributes::Archive) == FileAttributes::Archive)
						 fProperties->cArchive->CheckState = CheckState::Checked;
					 
					 if((fi->Attributes & FileAttributes::Hidden) == FileAttributes::Hidden)
						 fProperties->cHidden->CheckState = CheckState::Checked;
					 
					 if((fi->Attributes & FileAttributes::System) == FileAttributes::System)
						 fProperties->cSystem->CheckState = CheckState::Checked;

					 if((fi->Attributes & FileAttributes::Compressed) == FileAttributes::Compressed)
						 fProperties->cCompressed->CheckState = CheckState::Checked;

					 if((fi->Attributes & FileAttributes::Encrypted) == FileAttributes::Encrypted)
						 fProperties->cEncrypted->CheckState = CheckState::Checked;

					 fProperties->dtCreated->Value = fi->CreationTime;
					 fProperties->dtModified->Value = fi->LastWriteTime;
					 fProperties->dtAccessed->Value = fi->LastAccessTime;

					 delete fi;
					 delete di;
				 }

			 }
			 
			 fProperties->Text = getString("Properties") + fProperties->tFilename->Text;
			 if(fProperties->ShowDialog() == Windows::Forms::DialogResult::OK){
				 if(isArch){
					 //TODO: HEADER OVERWRITE
				 }else{
					 FileAttributes fa;
					 if(lFiles->SelectedItems->Count == 1){
						 
						fa = File::GetAttributes(gcnew String(curDir.c_str())+lFiles->SelectedItems[0]->Text);
						File::SetAttributes(gcnew String(curDir.c_str())+lFiles->SelectedItems[0]->Text, FileAttributes::Normal);
						if(File::Exists(gcnew String(curDir.c_str())+lFiles->SelectedItems[0]->Text)){	 
							 File::SetCreationTime(gcnew String(curDir.c_str())+lFiles->SelectedItems[0]->Text, fProperties->dtCreated->Value);
							 File::SetLastWriteTime(gcnew String(curDir.c_str())+lFiles->SelectedItems[0]->Text, fProperties->dtModified->Value);
							 File::SetLastAccessTime(gcnew String(curDir.c_str())+lFiles->SelectedItems[0]->Text, fProperties->dtAccessed->Value);
							 File::Move(gcnew String(curDir.c_str())+lFiles->SelectedItems[0]->Text, gcnew String(curDir.c_str())+fProperties->tFilename->Text);
						}else if(Directory::Exists(gcnew String(curDir.c_str())+lFiles->SelectedItems[0]->Text)){
							 Directory::SetCreationTime(gcnew String(curDir.c_str())+lFiles->SelectedItems[0]->Text, fProperties->dtCreated->Value);
							 Directory::SetLastWriteTime(gcnew String(curDir.c_str())+lFiles->SelectedItems[0]->Text, fProperties->dtModified->Value);
							 Directory::SetLastAccessTime(gcnew String(curDir.c_str())+lFiles->SelectedItems[0]->Text, fProperties->dtAccessed->Value);
							 try{
								 if(gcnew String(curDir.c_str())+lFiles->SelectedItems[0]->Text != gcnew String(curDir.c_str())+fProperties->tFilename->Text)
									 Directory::Move(gcnew String(curDir.c_str())+lFiles->SelectedItems[0]->Text, gcnew String(curDir.c_str())+fProperties->tFilename->Text);
							 }catch(Exception^ e){
								 MessageBox::Show(e->Message, L"KGB Archiver 2");
								 return;
							 }
						}
						setattr(fa, fProperties, gcnew String(curDir.c_str())+fProperties->tFilename->Text);
						 
					 }else{
						 for(int i=0;i<lFiles->SelectedItems->Count;i++){
							 fa = File::GetAttributes(gcnew String(curDir.c_str())+lFiles->SelectedItems[i]->Text);
							 File::SetAttributes(gcnew String(curDir.c_str())+lFiles->SelectedItems[i]->Text, FileAttributes::Normal);
							 setattr(fa, fProperties, gcnew String(curDir.c_str())+lFiles->SelectedItems[i]->Text);
						 }
					 }
				 }
			 }

			 delete fProperties;
		 }
private: System::Void mTest_Click(System::Object^  sender, System::EventArgs^  e) {
			 wstring arch_name;
			 compressInfo cmsi;
			 ProcessStartInfo ^psi;

			 psi = gcnew ProcessStartInfo();

			 psi->WorkingDirectory = gcnew String(curDir.c_str());
			 psi->FileName = IO::Path::GetDirectoryName(Application::ExecutablePath)+L"\\kgb2compressor.exe";
			 psi->Arguments = gcnew String("-mt");

			 if(isKGBArchive(curDir, arch_name)){
				 readHeader(arch_name, &cmsi);

				 if(cmsi.encryption.enabled){
					 passwd ^pass = gcnew passwd();
					 
					 if(pass->ShowDialog() == Windows::Forms::DialogResult::OK){
						 psi->Arguments += L" -p\"" + pass->tPasswd->Text + L"\"";

						 delete pass;
					 }else{
						 delete pass;
						 delete psi;
						 return;
					 } 
				 }
				 psi->Arguments += L" \"" + gcnew String(arch_name.c_str()) + "\"";
			 }else if(isZIPArchive(curDir, arch_name)){
				 CZipArchive zip;
				 zip.Open(arch_name.c_str(), CZipArchive::OpenMode::zipOpenReadOnly);

				 if(zip[0]->IsEncrypted()){
					 passwd ^pass = gcnew passwd();
					 
					 if(pass->ShowDialog() == Windows::Forms::DialogResult::OK){
						 psi->Arguments += L" -p\"" + pass->tPasswd->Text + L"\"";

						 delete pass;
					 }else{
						 delete pass;
						 delete psi;
						 return;
					 } 
				 }
				 zip.Close();
				 psi->Arguments += L" \"" + gcnew String(arch_name.c_str()) + "\"";
			 }else
				 return;

			 Process::Start(psi);
			 delete psi;
		 }
private: System::Void homepageToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 Diagnostics::Process::Start(L"http://kgbarchiver.sourceforge.net/");
		 }
private: System::Void mPaste_Click(System::Object^  sender, System::EventArgs^  e) {
			 pStatus->Value = 0;
			 pStatus->Visible = true;

			 int mode = -1; //0-copy, 1-cut
			 Specialized::StringCollection ^str = Clipboard::GetFileDropList();
			 pStatus->Maximum = str->Count+1;

			 if(Clipboard::GetDataObject()->GetDataPresent("Preferred DropEffect")){
				 //array<unsigned char> ^dde = /*static_cast<array<unsigned char>^>*/(array<unsigned char>^)(Clipboard::GetDataObject()->GetData("Preferred DropEffect", false));
				 DragDropEffects value;
				 try{
					MemoryStream ^mem = (MemoryStream^)(Clipboard::GetDataObject()->GetData("Preferred DropEffect",true));
					value = (DragDropEffects)mem->ReadByte();
				 }catch(Exception^){
					 try{
						 array<unsigned char> ^dde = (array<unsigned char>^)(Clipboard::GetDataObject()->GetData("Preferred DropEffect", true));
						 value = (DragDropEffects)dde[0];
						 delete dde;
					 }catch(Exception^){
						 pStatus->Visible = false;
						 return;
					 }
				 }
				 //MessageBox::Show(String::Format("{0}", (unsigned char)value));
				 if ((value/*(DragDropEffects)dde[0]*/ & DragDropEffects::Move) == DragDropEffects::Move){
					mode = 1;
					sStatus->Text = getString("MovingFiles");//gcnew String(L"Moving files...");
				 }else if((value/*(DragDropEffects)dde[0]*/ & DragDropEffects::Copy) == DragDropEffects::Copy){
				    mode = 0;
					sStatus->Text = getString("CopyingFiles");//gcnew String(L"Copying files...");
				}
				 //delete dde;
				 Update();
			 }
			 pStatus->Value++;
			 switch(mode){
				 case 0:
					for(int i=0;i<str->Count;i++){
						if(File::Exists(str[i])){
							if(File::Exists(gcnew String(curDir.c_str())+Path::GetFileName(str[i]))){
								if(!overwriteFile(gcnew String(curDir.c_str())+Path::GetFileName(str[i]), str[i]))
									continue;
							}
							File::Copy(str[i], gcnew String(curDir.c_str())+Path::GetFileName(str[i]), true);
						}else if(Directory::Exists(str[i])){
							if(Directory::Exists(gcnew String(curDir.c_str())+Path::GetFileName(str[i]))){
								if(!overwriteFile(gcnew String(curDir.c_str())+Path::GetFileName(str[i]), str[i]))
									continue;
							}
							//!!!!!!!!!
							//Directory::Copy(str[i], gcnew String(curDir.c_str())+Path::GetFileName(str[i]), true);
							Microsoft::VisualBasic::FileIO::FileSystem::CopyDirectory(str[i], gcnew String(curDir.c_str())+Path::GetFileName(str[i]), true);
						}
						pStatus->Value++;
					}
					break;
				 case 1:
					for(int i=0;i<Clipboard::GetFileDropList()->Count;i++){
						if(File::Exists(str[i])){
							if(File::Exists(gcnew String(curDir.c_str())+Path::GetFileName(str[i]))){
								if(!overwriteFile(gcnew String(curDir.c_str())+Path::GetFileName(str[i]), str[i]))
									continue;
								File::Delete(gcnew String(curDir.c_str())+Path::GetFileName(str[i]));
							}
							File::Move(str[i], gcnew String(curDir.c_str())+Path::GetFileName(str[i]));
						}else if(Directory::Exists(str[i])){
							if(Directory::Exists(gcnew String(curDir.c_str())+Path::GetFileName(str[i]))){
								if(!overwriteFile(gcnew String(curDir.c_str())+Path::GetFileName(str[i]), str[i]))
									continue;
								//Directory::Delete(gcnew String(curDir.c_str())+Path::GetFileName(str[i]), true);
							}
							Directory::Move(str[i], gcnew String(curDir.c_str())+Path::GetFileName(str[i]));
						}
						pStatus->Value++;
					}
					break;
			 }
			 overwriteMode = OVERWRITE_NO;
			 pStatus->Visible = false;
			 pStatus->Value = 0;
			 sStatus->Text = getString("Done");//gcnew String(L"Done");
		 }
private: System::Void mCopy_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(lFiles->SelectedItems->Count == 0)
				return;

			 array<String^>^lst = gcnew array<String^>(lFiles->SelectedItems->Count);
			 for(int i=0;i<lFiles->SelectedItems->Count;i++)
				lst[i] = gcnew String(curDir.c_str())+lFiles->SelectedItems[i]->Text;
				
			 Windows::Forms::IDataObject^ data = gcnew DataObject(DataFormats::FileDrop, lst);
			 data->SetData("Preferred DropEffect", gcnew MemoryStream(gcnew array<unsigned char>{5/*(unsigned char)DragDropEffects::Copy*/,0,0,0}));
			 /*MemoryStream ^mem = gcnew MemoryStream(gcnew array<unsigned char>{5,0,0,0});
			 data->SetData("Preferred DropEffect", mem);*/

			 Clipboard::SetDataObject(data);

			 //delete mem;
			 delete data;
			 delete lst;
		 }
private: System::Void mCut_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(lFiles->SelectedItems->Count == 0)
				return;

			 array<String^>^lst = gcnew array<String^>(lFiles->SelectedItems->Count);
			 for(int i=0;i<lFiles->SelectedItems->Count;i++)
				lst[i] = gcnew String(curDir.c_str())+lFiles->SelectedItems[i]->Text;
				
			 Windows::Forms::IDataObject^ data = gcnew DataObject(DataFormats::FileDrop, lst);
			 data->SetData("Preferred DropEffect", gcnew MemoryStream(gcnew array<unsigned char>{2/*(unsigned char)DragDropEffects::Move*/,0,0,0}));
			 /*MemoryStream ^mem = gcnew MemoryStream(gcnew array<unsigned char>{2,0,0,0});
			 data->SetData("Preferred DropEffect", mem);*/

			 Clipboard::SetDataObject(data);

			 //delete mem;
			 delete data;
			 delete lst;
		 }
private: System::Void mEdit_DropDownOpening(System::Object^  sender, System::EventArgs^  e) {
			 mPaste->Enabled = Clipboard::ContainsFileDropList() && !isKGBArchive(curDir) && !isZIPArchive(curDir);
			 if(mPaste->Enabled){
				 Specialized::StringCollection ^str = Clipboard::GetFileDropList();
				 for(int i=0;i<str->Count;i++){
					 if((!File::Exists(str[i]) && !Directory::Exists(str[i])) || gcnew String(curDir.c_str()) == FileInfo(str[i]).DirectoryName + L"\\"){
						 mPaste->Enabled = false;
						 break;
					 }
				 }
				 delete str;
			 }
			 //mCopy->Enabled = (lFiles->SelectedItems->Count?true:false);
			 //mCut->Enabled = (lFiles->SelectedItems->Count?true:false);

			 if(lFiles->SelectedItems->Count){
				 //mShellRename->Visible = false;
				 mDelete->Enabled = true;
				 mWipe->Enabled = true;
				 mCopy->Enabled = true;
				 mCut->Enabled = true;
			 }else{
				 //mShellRename->Visible = true;
				 mDelete->Enabled = false;
				 mWipe->Enabled = false;
				 mCopy->Enabled = false;
				 mCut->Enabled = false;
			 }
		 }
private: System::Void mView_DropDownOpening(System::Object^  sender, System::EventArgs^  e) {
			 if(lFiles->SelectedItems->Count){
				mProperties->Enabled = true;
			 }else{
				mProperties->Enabled = false;
			 }
		 }
private: System::Void lFiles_ItemSelectionChanged(System::Object^  sender, System::Windows::Forms::ListViewItemSelectionChangedEventArgs^  e) {
			 //wstring arch_name;
			 if(isKGBArchive(curDir) || isZIPArchive(curDir))
				 return;

			 if(lFiles->SelectedItems->Count){
				 bCut->Enabled = true;
				 bCopy->Enabled = true;
				 bNew->Enabled = true;
				 bDel->Enabled = true;
				 bWipe->Enabled = true;
			 }else{
				 bCut->Enabled = false;
				 bCopy->Enabled = false;
				 bNew->Enabled = false;
				 bDel->Enabled = false;
				 bWipe->Enabled = false;
			 }
		 }
private: System::Void clipboardWatcher_Tick(System::Object^  sender, System::EventArgs^  e) {
			 bPaste->Enabled = Clipboard::ContainsFileDropList() && !isKGBArchive(curDir) && !isZIPArchive(curDir);
			 if(bPaste->Enabled){
				 Specialized::StringCollection ^str = Clipboard::GetFileDropList();
				 for(int i=0;i<str->Count;i++){
					 if((!File::Exists(str[i]) && !Directory::Exists(str[i])) || gcnew String(curDir.c_str()) == FileInfo(str[i]).DirectoryName + L"\\"){
						 bPaste->Enabled = false;
						 break;
					 }
				 }
				 delete str;
			 }
			 
		 }

};

}

