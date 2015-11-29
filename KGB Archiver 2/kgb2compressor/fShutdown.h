#pragma once


namespace kgb2compressor {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	/// <summary>
	/// Summary for fShutdown
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class fShutdown : public System::Windows::Forms::Form
	{
	public:
		fShutdown(void)
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
		~fShutdown()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  lPrompt;
	protected: 

	private: System::Windows::Forms::Button^  bCancel;
	private: System::Windows::Forms::Timer^  tCountdown;
	private: System::ComponentModel::IContainer^  components;
	private: int secs;
	protected: 

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(fShutdown::typeid));
			this->lPrompt = (gcnew System::Windows::Forms::Label());
			this->bCancel = (gcnew System::Windows::Forms::Button());
			this->tCountdown = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// lPrompt
			// 
			this->lPrompt->AccessibleDescription = nullptr;
			this->lPrompt->AccessibleName = nullptr;
			resources->ApplyResources(this->lPrompt, L"lPrompt");
			this->lPrompt->Font = nullptr;
			this->lPrompt->Name = L"lPrompt";
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
			// tCountdown
			// 
			this->tCountdown->Interval = 1000;
			this->tCountdown->Tick += gcnew System::EventHandler(this, &fShutdown::tCountdown_Tick);
			// 
			// fShutdown
			// 
			this->AccessibleDescription = nullptr;
			this->AccessibleName = nullptr;
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = nullptr;
			this->CancelButton = this->bCancel;
			this->Controls->Add(this->bCancel);
			this->Controls->Add(this->lPrompt);
			this->Font = nullptr;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = nullptr;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"fShutdown";
			this->TopMost = true;
			this->Load += gcnew System::EventHandler(this, &fShutdown::fShutdown_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void tCountdown_Tick(System::Object^  sender, System::EventArgs^  e) {
				secs--;
				lPrompt->Text = String::Format(L"Automatic shutdown for {0}s...", secs);
				if(secs == 0){
					this->DialogResult = Windows::Forms::DialogResult::OK;
					tCountdown->Enabled = false;
					Close();
				}
			 }
	private: System::Void fShutdown_Load(System::Object^  sender, System::EventArgs^  e) {
				 secs = 10;
				 tCountdown->Enabled = true;
			 }
	};
}
