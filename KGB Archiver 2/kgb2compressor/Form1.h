#pragma once

//#include "fShutdown.h"
#include <time.h>

namespace kgb2compressor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

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
		bool loaded;
		Form1(void)
		{
			loaded = false;
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
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
	private: System::Windows::Forms::GroupBox^  gFilename;
	public: System::Windows::Forms::Label^  lFilename;

	private: System::Windows::Forms::GroupBox^  gCurrentProgress;
	public: System::Windows::Forms::ProgressBar^  pCurrent;

	private: System::Windows::Forms::GroupBox^  gTotalProgress;
	public: System::Windows::Forms::ProgressBar^  pTotal;

	private: System::Windows::Forms::GroupBox^  gStatistics;
	public: System::Windows::Forms::Label^  lProjectedDesc;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label6;
	public: System::Windows::Forms::Label^  lBefore;
	private: 
	public: System::Windows::Forms::Label^  lRatio;
	public: System::Windows::Forms::Label^  lProjected;
	public: System::Windows::Forms::Label^  lTime;
	public: System::Windows::Forms::Label^  lRemaining;

	private: System::Windows::Forms::ComboBox^  cPriority;
	public: System::Windows::Forms::CheckBox^  cShutdown;

	private: System::Windows::Forms::Button^  bCancel;
	private: System::Diagnostics::Process^  pThis;
	public: System::Windows::Forms::NotifyIcon^  iTray;
	public: System::Windows::Forms::ImageList^  iTrayList;

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
			this->gFilename = (gcnew System::Windows::Forms::GroupBox());
			this->lFilename = (gcnew System::Windows::Forms::Label());
			this->gCurrentProgress = (gcnew System::Windows::Forms::GroupBox());
			this->pCurrent = (gcnew System::Windows::Forms::ProgressBar());
			this->gTotalProgress = (gcnew System::Windows::Forms::GroupBox());
			this->pTotal = (gcnew System::Windows::Forms::ProgressBar());
			this->gStatistics = (gcnew System::Windows::Forms::GroupBox());
			this->lRemaining = (gcnew System::Windows::Forms::Label());
			this->lTime = (gcnew System::Windows::Forms::Label());
			this->lRatio = (gcnew System::Windows::Forms::Label());
			this->lProjected = (gcnew System::Windows::Forms::Label());
			this->lBefore = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->lProjectedDesc = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->cPriority = (gcnew System::Windows::Forms::ComboBox());
			this->cShutdown = (gcnew System::Windows::Forms::CheckBox());
			this->bCancel = (gcnew System::Windows::Forms::Button());
			this->pThis = (gcnew System::Diagnostics::Process());
			this->iTray = (gcnew System::Windows::Forms::NotifyIcon(this->components));
			this->iTrayList = (gcnew System::Windows::Forms::ImageList(this->components));
			this->gFilename->SuspendLayout();
			this->gCurrentProgress->SuspendLayout();
			this->gTotalProgress->SuspendLayout();
			this->gStatistics->SuspendLayout();
			this->SuspendLayout();
			// 
			// gFilename
			// 
			this->gFilename->AccessibleDescription = nullptr;
			this->gFilename->AccessibleName = nullptr;
			resources->ApplyResources(this->gFilename, L"gFilename");
			this->gFilename->BackgroundImage = nullptr;
			this->gFilename->Controls->Add(this->lFilename);
			this->gFilename->Font = nullptr;
			this->gFilename->Name = L"gFilename";
			this->gFilename->TabStop = false;
			// 
			// lFilename
			// 
			this->lFilename->AccessibleDescription = nullptr;
			this->lFilename->AccessibleName = nullptr;
			resources->ApplyResources(this->lFilename, L"lFilename");
			this->lFilename->AutoEllipsis = true;
			this->lFilename->Font = nullptr;
			this->lFilename->Name = L"lFilename";
			// 
			// gCurrentProgress
			// 
			this->gCurrentProgress->AccessibleDescription = nullptr;
			this->gCurrentProgress->AccessibleName = nullptr;
			resources->ApplyResources(this->gCurrentProgress, L"gCurrentProgress");
			this->gCurrentProgress->BackgroundImage = nullptr;
			this->gCurrentProgress->Controls->Add(this->pCurrent);
			this->gCurrentProgress->Font = nullptr;
			this->gCurrentProgress->Name = L"gCurrentProgress";
			this->gCurrentProgress->TabStop = false;
			// 
			// pCurrent
			// 
			this->pCurrent->AccessibleDescription = nullptr;
			this->pCurrent->AccessibleName = nullptr;
			resources->ApplyResources(this->pCurrent, L"pCurrent");
			this->pCurrent->BackgroundImage = nullptr;
			this->pCurrent->Font = nullptr;
			this->pCurrent->Name = L"pCurrent";
			// 
			// gTotalProgress
			// 
			this->gTotalProgress->AccessibleDescription = nullptr;
			this->gTotalProgress->AccessibleName = nullptr;
			resources->ApplyResources(this->gTotalProgress, L"gTotalProgress");
			this->gTotalProgress->BackgroundImage = nullptr;
			this->gTotalProgress->Controls->Add(this->pTotal);
			this->gTotalProgress->Font = nullptr;
			this->gTotalProgress->Name = L"gTotalProgress";
			this->gTotalProgress->TabStop = false;
			// 
			// pTotal
			// 
			this->pTotal->AccessibleDescription = nullptr;
			this->pTotal->AccessibleName = nullptr;
			resources->ApplyResources(this->pTotal, L"pTotal");
			this->pTotal->BackgroundImage = nullptr;
			this->pTotal->Font = nullptr;
			this->pTotal->Name = L"pTotal";
			// 
			// gStatistics
			// 
			this->gStatistics->AccessibleDescription = nullptr;
			this->gStatistics->AccessibleName = nullptr;
			resources->ApplyResources(this->gStatistics, L"gStatistics");
			this->gStatistics->BackgroundImage = nullptr;
			this->gStatistics->Controls->Add(this->lRemaining);
			this->gStatistics->Controls->Add(this->lTime);
			this->gStatistics->Controls->Add(this->lRatio);
			this->gStatistics->Controls->Add(this->lProjected);
			this->gStatistics->Controls->Add(this->lBefore);
			this->gStatistics->Controls->Add(this->label5);
			this->gStatistics->Controls->Add(this->label4);
			this->gStatistics->Controls->Add(this->label3);
			this->gStatistics->Controls->Add(this->lProjectedDesc);
			this->gStatistics->Controls->Add(this->label1);
			this->gStatistics->Font = nullptr;
			this->gStatistics->Name = L"gStatistics";
			this->gStatistics->TabStop = false;
			// 
			// lRemaining
			// 
			this->lRemaining->AccessibleDescription = nullptr;
			this->lRemaining->AccessibleName = nullptr;
			resources->ApplyResources(this->lRemaining, L"lRemaining");
			this->lRemaining->Font = nullptr;
			this->lRemaining->Name = L"lRemaining";
			// 
			// lTime
			// 
			this->lTime->AccessibleDescription = nullptr;
			this->lTime->AccessibleName = nullptr;
			resources->ApplyResources(this->lTime, L"lTime");
			this->lTime->Font = nullptr;
			this->lTime->Name = L"lTime";
			// 
			// lRatio
			// 
			this->lRatio->AccessibleDescription = nullptr;
			this->lRatio->AccessibleName = nullptr;
			resources->ApplyResources(this->lRatio, L"lRatio");
			this->lRatio->Font = nullptr;
			this->lRatio->Name = L"lRatio";
			// 
			// lProjected
			// 
			this->lProjected->AccessibleDescription = nullptr;
			this->lProjected->AccessibleName = nullptr;
			resources->ApplyResources(this->lProjected, L"lProjected");
			this->lProjected->Font = nullptr;
			this->lProjected->Name = L"lProjected";
			// 
			// lBefore
			// 
			this->lBefore->AccessibleDescription = nullptr;
			this->lBefore->AccessibleName = nullptr;
			resources->ApplyResources(this->lBefore, L"lBefore");
			this->lBefore->Font = nullptr;
			this->lBefore->Name = L"lBefore";
			// 
			// label5
			// 
			this->label5->AccessibleDescription = nullptr;
			this->label5->AccessibleName = nullptr;
			resources->ApplyResources(this->label5, L"label5");
			this->label5->Font = nullptr;
			this->label5->Name = L"label5";
			// 
			// label4
			// 
			this->label4->AccessibleDescription = nullptr;
			this->label4->AccessibleName = nullptr;
			resources->ApplyResources(this->label4, L"label4");
			this->label4->Font = nullptr;
			this->label4->Name = L"label4";
			// 
			// label3
			// 
			this->label3->AccessibleDescription = nullptr;
			this->label3->AccessibleName = nullptr;
			resources->ApplyResources(this->label3, L"label3");
			this->label3->Font = nullptr;
			this->label3->Name = L"label3";
			// 
			// lProjectedDesc
			// 
			this->lProjectedDesc->AccessibleDescription = nullptr;
			this->lProjectedDesc->AccessibleName = nullptr;
			resources->ApplyResources(this->lProjectedDesc, L"lProjectedDesc");
			this->lProjectedDesc->Font = nullptr;
			this->lProjectedDesc->Name = L"lProjectedDesc";
			// 
			// label1
			// 
			this->label1->AccessibleDescription = nullptr;
			this->label1->AccessibleName = nullptr;
			resources->ApplyResources(this->label1, L"label1");
			this->label1->Font = nullptr;
			this->label1->Name = L"label1";
			// 
			// label6
			// 
			this->label6->AccessibleDescription = nullptr;
			this->label6->AccessibleName = nullptr;
			resources->ApplyResources(this->label6, L"label6");
			this->label6->Font = nullptr;
			this->label6->Name = L"label6";
			// 
			// cPriority
			// 
			this->cPriority->AccessibleDescription = nullptr;
			this->cPriority->AccessibleName = nullptr;
			resources->ApplyResources(this->cPriority, L"cPriority");
			this->cPriority->BackgroundImage = nullptr;
			this->cPriority->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cPriority->Font = nullptr;
			this->cPriority->FormattingEnabled = true;
			this->cPriority->Items->AddRange(gcnew cli::array< System::Object^  >(5) {resources->GetString(L"cPriority.Items"), resources->GetString(L"cPriority.Items1"), 
				resources->GetString(L"cPriority.Items2"), resources->GetString(L"cPriority.Items3"), resources->GetString(L"cPriority.Items4")});
			this->cPriority->Name = L"cPriority";
			this->cPriority->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::cPriority_SelectedIndexChanged);
			// 
			// cShutdown
			// 
			this->cShutdown->AccessibleDescription = nullptr;
			this->cShutdown->AccessibleName = nullptr;
			resources->ApplyResources(this->cShutdown, L"cShutdown");
			this->cShutdown->BackgroundImage = nullptr;
			this->cShutdown->Font = nullptr;
			this->cShutdown->Name = L"cShutdown";
			this->cShutdown->UseVisualStyleBackColor = true;
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
			this->bCancel->Click += gcnew System::EventHandler(this, &Form1::bCancel_Click);
			// 
			// pThis
			// 
			this->pThis->StartInfo->Domain = L"";
			this->pThis->StartInfo->LoadUserProfile = false;
			this->pThis->StartInfo->Password = nullptr;
			this->pThis->StartInfo->StandardErrorEncoding = nullptr;
			this->pThis->StartInfo->StandardOutputEncoding = nullptr;
			this->pThis->StartInfo->UserName = L"";
			this->pThis->SynchronizingObject = this;
			// 
			// iTray
			// 
			this->iTray->BalloonTipIcon = System::Windows::Forms::ToolTipIcon::Info;
			resources->ApplyResources(this->iTray, L"iTray");
			this->iTray->Icon = nullptr;
			this->iTray->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::iTray_MouseMove);
			this->iTray->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::iTray_MouseDoubleClick);
			// 
			// iTrayList
			// 
			this->iTrayList->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"iTrayList.ImageStream")));
			this->iTrayList->TransparentColor = System::Drawing::Color::Transparent;
			this->iTrayList->Images->SetKeyName(0, L"tray1.ico");
			this->iTrayList->Images->SetKeyName(1, L"tray2.ico");
			this->iTrayList->Images->SetKeyName(2, L"tray3.ico");
			this->iTrayList->Images->SetKeyName(3, L"tray4.ico");
			this->iTrayList->Images->SetKeyName(4, L"tray5.ico");
			this->iTrayList->Images->SetKeyName(5, L"tray6.ico");
			// 
			// Form1
			// 
			this->AccessibleDescription = nullptr;
			this->AccessibleName = nullptr;
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = nullptr;
			this->CancelButton = this->bCancel;
			this->Controls->Add(this->bCancel);
			this->Controls->Add(this->cShutdown);
			this->Controls->Add(this->cPriority);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->gStatistics);
			this->Controls->Add(this->gTotalProgress);
			this->Controls->Add(this->gCurrentProgress);
			this->Controls->Add(this->gFilename);
			this->Font = nullptr;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = nullptr;
			this->MaximizeBox = false;
			this->Name = L"Form1";
			this->LocationChanged += gcnew System::EventHandler(this, &Form1::Form1_LocationChanged);
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->gFilename->ResumeLayout(false);
			this->gCurrentProgress->ResumeLayout(false);
			this->gTotalProgress->ResumeLayout(false);
			this->gStatistics->ResumeLayout(false);
			this->gStatistics->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: clock_t BalloonTipTime;
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
				 this->DialogResult = Windows::Forms::DialogResult::Cancel;

				 pThis = Diagnostics::Process::GetCurrentProcess();
				 pThis->PriorityBoostEnabled = true;
				 cPriority->SelectedIndex = 2;
				 BalloonTipTime = clock();
				 iTray->Visible = true;

				 loaded = true;
			 }
private: System::Void cPriority_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 switch(cPriority->SelectedIndex){
				case 0: pThis->PriorityClass = Diagnostics::ProcessPriorityClass::High; break;
				case 1: pThis->PriorityClass = Diagnostics::ProcessPriorityClass::AboveNormal; break;
				case 2: pThis->PriorityClass = Diagnostics::ProcessPriorityClass::Normal; break;
				case 3: pThis->PriorityClass = Diagnostics::ProcessPriorityClass::BelowNormal; break;
				case 4: pThis->PriorityClass = Diagnostics::ProcessPriorityClass::Idle; break;
			 }
		 }
private: System::Void iTray_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if(clock()-BalloonTipTime > 1000){
				 iTray->ShowBalloonTip(1000);
				 BalloonTipTime = clock();
			 }
		 }
public: System::Void Form1_LocationChanged(System::Object^  sender, System::EventArgs^  e) {
			 if(this->WindowState == FormWindowState::Minimized){
				 //this->DialogResult = Windows::Forms::DialogResult::None;
				 //this->WindowState = FormWindowState::Normal;
				 this->Visible = false;
				 
			 }
		 }
private: System::Void iTray_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 this->Visible = true;
			 this->WindowState = FormWindowState::Normal;
			 //this->ShowInTaskbar = true;
		 }
private: System::Void bCancel_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->loaded = false;
			 this->DialogResult = Windows::Forms::DialogResult::Cancel;
			 this->Close();
		 }
};
}

