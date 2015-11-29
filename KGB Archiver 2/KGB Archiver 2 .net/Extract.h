#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "../language.h"


namespace KGBArchiver2net {

	/// <summary>
	/// Summary for Extract
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Extract : public System::Windows::Forms::Form
	{
	public:
		Extract(void)
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
		~Extract()
		{
			if (components)
			{
				delete components;
			}
		}
	public: static ref class format{
				public: static char type;
					static char algorithm;
					static char level;
			};
	public: System::Windows::Forms::FolderBrowserDialog^  dBrowser;
	public: System::Windows::Forms::Button^  bCancel;
	public: System::Windows::Forms::Button^  bNext;

	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tGeneral;
	private: System::Windows::Forms::TabPage^  tabPage2;
	public: System::Windows::Forms::GroupBox^  gOther;

	public: System::Windows::Forms::CheckBox^  cRunBackground;
	public: System::Windows::Forms::CheckBox^  cDelete;
	public: System::Windows::Forms::CheckBox^  cOpenDir;
	public: System::Windows::Forms::GroupBox^  gPassword;
	public: System::Windows::Forms::CheckBox^  cShow;
	public: System::Windows::Forms::TextBox^  tPasswd;
	public: System::Windows::Forms::GroupBox^  gOverwrite;
	private: System::Windows::Forms::RadioButton^  rNever;

	public: System::Windows::Forms::RadioButton^  rForce;

	public: System::Windows::Forms::RadioButton^  rAsk;



	public: System::Windows::Forms::GroupBox^  gDestination;
	public: System::Windows::Forms::Button^  bRecent;
	public: System::Windows::Forms::Button^  bBrowse;
	public: System::Windows::Forms::TextBox^  tDestination;
	public: System::Windows::Forms::ListView^  lFiles;


	private: System::Windows::Forms::ColumnHeader^  cFilename;
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Extract::typeid));
			this->dBrowser = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->bCancel = (gcnew System::Windows::Forms::Button());
			this->bNext = (gcnew System::Windows::Forms::Button());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tGeneral = (gcnew System::Windows::Forms::TabPage());
			this->gOther = (gcnew System::Windows::Forms::GroupBox());
			this->cRunBackground = (gcnew System::Windows::Forms::CheckBox());
			this->cDelete = (gcnew System::Windows::Forms::CheckBox());
			this->cOpenDir = (gcnew System::Windows::Forms::CheckBox());
			this->gPassword = (gcnew System::Windows::Forms::GroupBox());
			this->cShow = (gcnew System::Windows::Forms::CheckBox());
			this->tPasswd = (gcnew System::Windows::Forms::TextBox());
			this->gOverwrite = (gcnew System::Windows::Forms::GroupBox());
			this->rNever = (gcnew System::Windows::Forms::RadioButton());
			this->rForce = (gcnew System::Windows::Forms::RadioButton());
			this->rAsk = (gcnew System::Windows::Forms::RadioButton());
			this->gDestination = (gcnew System::Windows::Forms::GroupBox());
			this->bRecent = (gcnew System::Windows::Forms::Button());
			this->bBrowse = (gcnew System::Windows::Forms::Button());
			this->tDestination = (gcnew System::Windows::Forms::TextBox());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->lFiles = (gcnew System::Windows::Forms::ListView());
			this->cFilename = (gcnew System::Windows::Forms::ColumnHeader());
			this->mRecent = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->tabControl1->SuspendLayout();
			this->tGeneral->SuspendLayout();
			this->gOther->SuspendLayout();
			this->gPassword->SuspendLayout();
			this->gOverwrite->SuspendLayout();
			this->gDestination->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->SuspendLayout();
			// 
			// dBrowser
			// 
			resources->ApplyResources(this->dBrowser, L"dBrowser");
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
			// bNext
			// 
			this->bNext->AccessibleDescription = nullptr;
			this->bNext->AccessibleName = nullptr;
			resources->ApplyResources(this->bNext, L"bNext");
			this->bNext->BackgroundImage = nullptr;
			this->bNext->Font = nullptr;
			this->bNext->Name = L"bNext";
			this->bNext->UseVisualStyleBackColor = true;
			this->bNext->Click += gcnew System::EventHandler(this, &Extract::bNext_Click);
			// 
			// tabControl1
			// 
			this->tabControl1->AccessibleDescription = nullptr;
			this->tabControl1->AccessibleName = nullptr;
			resources->ApplyResources(this->tabControl1, L"tabControl1");
			this->tabControl1->BackgroundImage = nullptr;
			this->tabControl1->Controls->Add(this->tGeneral);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Font = nullptr;
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			// 
			// tGeneral
			// 
			this->tGeneral->AccessibleDescription = nullptr;
			this->tGeneral->AccessibleName = nullptr;
			resources->ApplyResources(this->tGeneral, L"tGeneral");
			this->tGeneral->BackgroundImage = nullptr;
			this->tGeneral->Controls->Add(this->gOther);
			this->tGeneral->Controls->Add(this->gPassword);
			this->tGeneral->Controls->Add(this->gOverwrite);
			this->tGeneral->Controls->Add(this->gDestination);
			this->tGeneral->Font = nullptr;
			this->tGeneral->Name = L"tGeneral";
			this->tGeneral->UseVisualStyleBackColor = true;
			// 
			// gOther
			// 
			this->gOther->AccessibleDescription = nullptr;
			this->gOther->AccessibleName = nullptr;
			resources->ApplyResources(this->gOther, L"gOther");
			this->gOther->BackgroundImage = nullptr;
			this->gOther->Controls->Add(this->cRunBackground);
			this->gOther->Controls->Add(this->cDelete);
			this->gOther->Controls->Add(this->cOpenDir);
			this->gOther->Font = nullptr;
			this->gOther->Name = L"gOther";
			this->gOther->TabStop = false;
			// 
			// cRunBackground
			// 
			this->cRunBackground->AccessibleDescription = nullptr;
			this->cRunBackground->AccessibleName = nullptr;
			resources->ApplyResources(this->cRunBackground, L"cRunBackground");
			this->cRunBackground->BackgroundImage = nullptr;
			this->cRunBackground->Font = nullptr;
			this->cRunBackground->Name = L"cRunBackground";
			this->cRunBackground->UseVisualStyleBackColor = true;
			// 
			// cDelete
			// 
			this->cDelete->AccessibleDescription = nullptr;
			this->cDelete->AccessibleName = nullptr;
			resources->ApplyResources(this->cDelete, L"cDelete");
			this->cDelete->BackgroundImage = nullptr;
			this->cDelete->Font = nullptr;
			this->cDelete->Name = L"cDelete";
			this->cDelete->UseVisualStyleBackColor = true;
			// 
			// cOpenDir
			// 
			this->cOpenDir->AccessibleDescription = nullptr;
			this->cOpenDir->AccessibleName = nullptr;
			resources->ApplyResources(this->cOpenDir, L"cOpenDir");
			this->cOpenDir->BackgroundImage = nullptr;
			this->cOpenDir->Font = nullptr;
			this->cOpenDir->Name = L"cOpenDir";
			this->cOpenDir->UseVisualStyleBackColor = true;
			// 
			// gPassword
			// 
			this->gPassword->AccessibleDescription = nullptr;
			this->gPassword->AccessibleName = nullptr;
			resources->ApplyResources(this->gPassword, L"gPassword");
			this->gPassword->BackgroundImage = nullptr;
			this->gPassword->Controls->Add(this->cShow);
			this->gPassword->Controls->Add(this->tPasswd);
			this->gPassword->Font = nullptr;
			this->gPassword->Name = L"gPassword";
			this->gPassword->TabStop = false;
			// 
			// cShow
			// 
			this->cShow->AccessibleDescription = nullptr;
			this->cShow->AccessibleName = nullptr;
			resources->ApplyResources(this->cShow, L"cShow");
			this->cShow->BackgroundImage = nullptr;
			this->cShow->Font = nullptr;
			this->cShow->Name = L"cShow";
			this->cShow->UseVisualStyleBackColor = true;
			this->cShow->CheckedChanged += gcnew System::EventHandler(this, &Extract::cShow_CheckedChanged);
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
			// gOverwrite
			// 
			this->gOverwrite->AccessibleDescription = nullptr;
			this->gOverwrite->AccessibleName = nullptr;
			resources->ApplyResources(this->gOverwrite, L"gOverwrite");
			this->gOverwrite->BackgroundImage = nullptr;
			this->gOverwrite->Controls->Add(this->rNever);
			this->gOverwrite->Controls->Add(this->rForce);
			this->gOverwrite->Controls->Add(this->rAsk);
			this->gOverwrite->Font = nullptr;
			this->gOverwrite->Name = L"gOverwrite";
			this->gOverwrite->TabStop = false;
			// 
			// rNever
			// 
			this->rNever->AccessibleDescription = nullptr;
			this->rNever->AccessibleName = nullptr;
			resources->ApplyResources(this->rNever, L"rNever");
			this->rNever->BackgroundImage = nullptr;
			this->rNever->Font = nullptr;
			this->rNever->Name = L"rNever";
			this->rNever->TabStop = true;
			this->rNever->UseVisualStyleBackColor = true;
			// 
			// rForce
			// 
			this->rForce->AccessibleDescription = nullptr;
			this->rForce->AccessibleName = nullptr;
			resources->ApplyResources(this->rForce, L"rForce");
			this->rForce->BackgroundImage = nullptr;
			this->rForce->Font = nullptr;
			this->rForce->Name = L"rForce";
			this->rForce->TabStop = true;
			this->rForce->UseVisualStyleBackColor = true;
			// 
			// rAsk
			// 
			this->rAsk->AccessibleDescription = nullptr;
			this->rAsk->AccessibleName = nullptr;
			resources->ApplyResources(this->rAsk, L"rAsk");
			this->rAsk->BackgroundImage = nullptr;
			this->rAsk->Checked = true;
			this->rAsk->Font = nullptr;
			this->rAsk->Name = L"rAsk";
			this->rAsk->TabStop = true;
			this->rAsk->UseVisualStyleBackColor = true;
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
			// 
			// bRecent
			// 
			this->bRecent->AccessibleDescription = nullptr;
			this->bRecent->AccessibleName = nullptr;
			resources->ApplyResources(this->bRecent, L"bRecent");
			this->bRecent->BackgroundImage = nullptr;
			this->bRecent->Font = nullptr;
			this->bRecent->Name = L"bRecent";
			this->bRecent->UseVisualStyleBackColor = true;
			this->bRecent->Click += gcnew System::EventHandler(this, &Extract::bRecent_Click);
			// 
			// bBrowse
			// 
			this->bBrowse->AccessibleDescription = nullptr;
			this->bBrowse->AccessibleName = nullptr;
			resources->ApplyResources(this->bBrowse, L"bBrowse");
			this->bBrowse->BackgroundImage = nullptr;
			this->bBrowse->Font = nullptr;
			this->bBrowse->Name = L"bBrowse";
			this->bBrowse->UseVisualStyleBackColor = true;
			this->bBrowse->Click += gcnew System::EventHandler(this, &Extract::bBrowse_Click);
			// 
			// tDestination
			// 
			this->tDestination->AccessibleDescription = nullptr;
			this->tDestination->AccessibleName = nullptr;
			resources->ApplyResources(this->tDestination, L"tDestination");
			this->tDestination->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::Suggest;
			this->tDestination->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::FileSystemDirectories;
			this->tDestination->BackgroundImage = nullptr;
			this->tDestination->Font = nullptr;
			this->tDestination->Name = L"tDestination";
			// 
			// tabPage2
			// 
			this->tabPage2->AccessibleDescription = nullptr;
			this->tabPage2->AccessibleName = nullptr;
			resources->ApplyResources(this->tabPage2, L"tabPage2");
			this->tabPage2->BackgroundImage = nullptr;
			this->tabPage2->Controls->Add(this->lFiles);
			this->tabPage2->Font = nullptr;
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// lFiles
			// 
			this->lFiles->AccessibleDescription = nullptr;
			this->lFiles->AccessibleName = nullptr;
			resources->ApplyResources(this->lFiles, L"lFiles");
			this->lFiles->BackgroundImage = nullptr;
			this->lFiles->CheckBoxes = true;
			this->lFiles->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(1) {this->cFilename});
			this->lFiles->Cursor = System::Windows::Forms::Cursors::Default;
			this->lFiles->Font = nullptr;
			this->lFiles->Name = L"lFiles";
			this->lFiles->UseCompatibleStateImageBehavior = false;
			this->lFiles->View = System::Windows::Forms::View::Details;
			// 
			// cFilename
			// 
			resources->ApplyResources(this->cFilename, L"cFilename");
			// 
			// mRecent
			// 
			this->mRecent->AccessibleDescription = nullptr;
			this->mRecent->AccessibleName = nullptr;
			resources->ApplyResources(this->mRecent, L"mRecent");
			this->mRecent->BackgroundImage = nullptr;
			this->mRecent->Font = nullptr;
			this->mRecent->Name = L"contextMenuStrip1";
			this->mRecent->ItemClicked += gcnew System::Windows::Forms::ToolStripItemClickedEventHandler(this, &Extract::mRecent_ItemClicked);
			// 
			// Extract
			// 
			this->AcceptButton = this->bNext;
			this->AccessibleDescription = nullptr;
			this->AccessibleName = nullptr;
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = nullptr;
			this->CancelButton = this->bCancel;
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->bNext);
			this->Controls->Add(this->bCancel);
			this->Font = nullptr;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = nullptr;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"Extract";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Extract::Extract_FormClosing);
			this->tabControl1->ResumeLayout(false);
			this->tGeneral->ResumeLayout(false);
			this->gOther->ResumeLayout(false);
			this->gOther->PerformLayout();
			this->gPassword->ResumeLayout(false);
			this->gPassword->PerformLayout();
			this->gOverwrite->ResumeLayout(false);
			this->gOverwrite->PerformLayout();
			this->gDestination->ResumeLayout(false);
			this->gDestination->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void cShow_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				 tPasswd->UseSystemPasswordChar = !cShow->Checked;
			 }
private: System::Void bBrowse_Click(System::Object^  sender, System::EventArgs^  e) {
			 try{
				 dBrowser->SelectedPath = tDestination->Text;
			 }catch(...){}
			 if(dBrowser->ShowDialog() == Windows::Forms::DialogResult::OK)
				 tDestination->Text = dBrowser->SelectedPath;
		 }
private: System::Void bNext_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(tDestination->Text->Length == 0)
				 MessageBox::Show(getString("ProvideExtractDir"), L"KGB Archiver 2");
			 else if(tPasswd->Enabled && tPasswd->Text->Length == 0)
				 MessageBox::Show(getString("ProvidePassword"), L"KGB Archiver 2");
			 else{
				 this->DialogResult = Windows::Forms::DialogResult::OK;
				 Close();
			 }
		 }
private: System::Void Extract_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
			 if(this->DialogResult == Windows::Forms::DialogResult::OK){			
				MEMORYSTATUSEX RAM;
				 unsigned int neededMem = 17; //for ZIP
				 unsigned int kgbMem[8][10] = {{12}, {58}, {60}, {201}, {344}, {367}, 
									{13, 15, 19, 31, 70, 158, 206, 406, 794, 1604}, 
									{31, 33, 37, 44, 59, 89, 145, 261, 492, 949}};

				 RAM.dwLength = sizeof(RAM);

				 if(format::type == 'k'){
					 if(format::algorithm < 6)
						 neededMem = kgbMem[format::algorithm][0];
					 else
						 neededMem = kgbMem[format::algorithm][format::level];
				 }

				 GlobalMemoryStatusEx(&RAM);

				 if(RAM.ullAvailPageFile < (neededMem<<20)){
					 MessageBox::Show(getString("NotEnoughMem"), L"KGB Archiver 2", MessageBoxButtons::OK, MessageBoxIcon::Stop);
					 //MessageBox::Show(L"There is not enough free memory to continue. Please close unused windows or programs and try again.", L"KGB Archiver 2", MessageBoxButtons::OK, MessageBoxIcon::Stop);
					 e->Cancel = true;
					 return;
				 }else if(RAM.ullTotalPhys *3/4 < (neededMem<<20)){
					 if(MessageBox::Show(getString("DecompressionCauseSystemSlowdown"), L"KGB Archiver 2", MessageBoxButtons::YesNo, MessageBoxIcon::Warning) == Windows::Forms::DialogResult::No){
						 e->Cancel = true;
						 return;
					 }
				 }

				 String ^hook = gcnew String(L"HKEY_CURRENT_USER\\Software\\KGB Archiver 2\\recentDirectories");
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
			 }
		 }
private: System::Void bRecent_Click(System::Object^  sender, System::EventArgs^  e) {
			 mRecent->Items->Clear();

			 String ^hook = gcnew String(L"HKEY_CURRENT_USER\\Software\\KGB Archiver 2\\recentDirectories");
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
