#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace KGBArchiver2net {

	/// <summary>
	/// Summary for About
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class About : public System::Windows::Forms::Form
	{
	public:
		About(void)
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
		~About()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  lVer;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tDonations;
	private: System::Windows::Forms::WebBrowser^  bDonations;
	private: System::Windows::Forms::TabPage^  tLicense;
	private: System::Windows::Forms::LinkLabel^  lHomepage;
	private: System::Windows::Forms::Button^  bOk;
	private: System::Windows::Forms::TextBox^  textBox1;


	protected: 

	protected: 

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(About::typeid));
			this->lVer = (gcnew System::Windows::Forms::Label());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tDonations = (gcnew System::Windows::Forms::TabPage());
			this->bDonations = (gcnew System::Windows::Forms::WebBrowser());
			this->tLicense = (gcnew System::Windows::Forms::TabPage());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->lHomepage = (gcnew System::Windows::Forms::LinkLabel());
			this->bOk = (gcnew System::Windows::Forms::Button());
			this->tabControl1->SuspendLayout();
			this->tDonations->SuspendLayout();
			this->tLicense->SuspendLayout();
			this->SuspendLayout();
			// 
			// lVer
			// 
			this->lVer->AccessibleDescription = nullptr;
			this->lVer->AccessibleName = nullptr;
			resources->ApplyResources(this->lVer, L"lVer");
			this->lVer->Font = nullptr;
			this->lVer->Name = L"lVer";
			// 
			// tabControl1
			// 
			this->tabControl1->AccessibleDescription = nullptr;
			this->tabControl1->AccessibleName = nullptr;
			resources->ApplyResources(this->tabControl1, L"tabControl1");
			this->tabControl1->BackgroundImage = nullptr;
			this->tabControl1->Controls->Add(this->tDonations);
			this->tabControl1->Controls->Add(this->tLicense);
			this->tabControl1->Font = nullptr;
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			// 
			// tDonations
			// 
			this->tDonations->AccessibleDescription = nullptr;
			this->tDonations->AccessibleName = nullptr;
			resources->ApplyResources(this->tDonations, L"tDonations");
			this->tDonations->BackgroundImage = nullptr;
			this->tDonations->Controls->Add(this->bDonations);
			this->tDonations->Font = nullptr;
			this->tDonations->Name = L"tDonations";
			this->tDonations->UseVisualStyleBackColor = true;
			// 
			// bDonations
			// 
			this->bDonations->AccessibleDescription = nullptr;
			this->bDonations->AccessibleName = nullptr;
			resources->ApplyResources(this->bDonations, L"bDonations");
			this->bDonations->IsWebBrowserContextMenuEnabled = false;
			this->bDonations->MinimumSize = System::Drawing::Size(20, 20);
			this->bDonations->Name = L"bDonations";
			this->bDonations->Url = (gcnew System::Uri(L"", System::UriKind::Relative));
			this->bDonations->WebBrowserShortcutsEnabled = false;
			// 
			// tLicense
			// 
			this->tLicense->AccessibleDescription = nullptr;
			this->tLicense->AccessibleName = nullptr;
			resources->ApplyResources(this->tLicense, L"tLicense");
			this->tLicense->BackgroundImage = nullptr;
			this->tLicense->Controls->Add(this->textBox1);
			this->tLicense->Font = nullptr;
			this->tLicense->Name = L"tLicense";
			this->tLicense->UseVisualStyleBackColor = true;
			// 
			// textBox1
			// 
			this->textBox1->AccessibleDescription = nullptr;
			this->textBox1->AccessibleName = nullptr;
			resources->ApplyResources(this->textBox1, L"textBox1");
			this->textBox1->BackColor = System::Drawing::SystemColors::Window;
			this->textBox1->BackgroundImage = nullptr;
			this->textBox1->Font = nullptr;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			// 
			// lHomepage
			// 
			this->lHomepage->AccessibleDescription = nullptr;
			this->lHomepage->AccessibleName = nullptr;
			resources->ApplyResources(this->lHomepage, L"lHomepage");
			this->lHomepage->Font = nullptr;
			this->lHomepage->Name = L"lHomepage";
			this->lHomepage->TabStop = true;
			this->lHomepage->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &About::lHomepage_LinkClicked);
			// 
			// bOk
			// 
			this->bOk->AccessibleDescription = nullptr;
			this->bOk->AccessibleName = nullptr;
			resources->ApplyResources(this->bOk, L"bOk");
			this->bOk->BackgroundImage = nullptr;
			this->bOk->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->bOk->Font = nullptr;
			this->bOk->Name = L"bOk";
			this->bOk->UseVisualStyleBackColor = true;
			// 
			// About
			// 
			this->AccessibleDescription = nullptr;
			this->AccessibleName = nullptr;
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = nullptr;
			this->Controls->Add(this->bOk);
			this->Controls->Add(this->lHomepage);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->lVer);
			this->Font = nullptr;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Icon = nullptr;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"About";
			this->Load += gcnew System::EventHandler(this, &About::About_Load);
			this->tabControl1->ResumeLayout(false);
			this->tDonations->ResumeLayout(false);
			this->tLicense->ResumeLayout(false);
			this->tLicense->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void About_Load(System::Object^  sender, System::EventArgs^  e) {
				 lVer->Text = gcnew String(L"KGB Archiver 2 v") + Application::ProductVersion;
				 bDonations->Navigate(IO::Path::GetDirectoryName(Application::ExecutablePath) + L"\\donations.htm");
			 }
	private: System::Void lHomepage_LinkClicked(System::Object^  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^  e) {
				 Diagnostics::Process::Start(L"http://kgbarchiver.sourceforge.net/");
			 }
};
}
