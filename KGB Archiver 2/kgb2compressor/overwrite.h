#pragma once

namespace kgb2compressor {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	
	/// <summary>
	/// Summary for overwrite
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class overwrite : public System::Windows::Forms::Form
	{
	public:
		overwrite(void)
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
		~overwrite()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::GroupBox^  gExisting;
	public: System::Windows::Forms::GroupBox^  gNew;
	public: System::Windows::Forms::Label^  lExistModified;
	public: System::Windows::Forms::Label^  lExistSize;
	public: System::Windows::Forms::PictureBox^  pExisting;
	public: System::Windows::Forms::Label^  lNewModified;
	public: System::Windows::Forms::Label^  lNewSize;
	public: System::Windows::Forms::PictureBox^  pNew;
	public: System::Windows::Forms::Button^  bYesAll;
	public: System::Windows::Forms::Button^  bYes;
	public: System::Windows::Forms::Button^  bNo;
	public: System::Windows::Forms::Button^  bNoAll;
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(overwrite::typeid));
			this->gExisting = (gcnew System::Windows::Forms::GroupBox());
			this->lExistModified = (gcnew System::Windows::Forms::Label());
			this->lExistSize = (gcnew System::Windows::Forms::Label());
			this->pExisting = (gcnew System::Windows::Forms::PictureBox());
			this->gNew = (gcnew System::Windows::Forms::GroupBox());
			this->lNewModified = (gcnew System::Windows::Forms::Label());
			this->lNewSize = (gcnew System::Windows::Forms::Label());
			this->pNew = (gcnew System::Windows::Forms::PictureBox());
			this->bYesAll = (gcnew System::Windows::Forms::Button());
			this->bYes = (gcnew System::Windows::Forms::Button());
			this->bNo = (gcnew System::Windows::Forms::Button());
			this->bNoAll = (gcnew System::Windows::Forms::Button());
			this->gExisting->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pExisting))->BeginInit();
			this->gNew->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pNew))->BeginInit();
			this->SuspendLayout();
			// 
			// gExisting
			// 
			this->gExisting->AccessibleDescription = nullptr;
			this->gExisting->AccessibleName = nullptr;
			resources->ApplyResources(this->gExisting, L"gExisting");
			this->gExisting->BackgroundImage = nullptr;
			this->gExisting->Controls->Add(this->lExistModified);
			this->gExisting->Controls->Add(this->lExistSize);
			this->gExisting->Controls->Add(this->pExisting);
			this->gExisting->Font = nullptr;
			this->gExisting->Name = L"gExisting";
			this->gExisting->TabStop = false;
			// 
			// lExistModified
			// 
			this->lExistModified->AccessibleDescription = nullptr;
			this->lExistModified->AccessibleName = nullptr;
			resources->ApplyResources(this->lExistModified, L"lExistModified");
			this->lExistModified->Font = nullptr;
			this->lExistModified->Name = L"lExistModified";
			// 
			// lExistSize
			// 
			this->lExistSize->AccessibleDescription = nullptr;
			this->lExistSize->AccessibleName = nullptr;
			resources->ApplyResources(this->lExistSize, L"lExistSize");
			this->lExistSize->Font = nullptr;
			this->lExistSize->Name = L"lExistSize";
			// 
			// pExisting
			// 
			this->pExisting->AccessibleDescription = nullptr;
			this->pExisting->AccessibleName = nullptr;
			resources->ApplyResources(this->pExisting, L"pExisting");
			this->pExisting->BackgroundImage = nullptr;
			this->pExisting->Font = nullptr;
			this->pExisting->ImageLocation = nullptr;
			this->pExisting->Name = L"pExisting";
			this->pExisting->TabStop = false;
			// 
			// gNew
			// 
			this->gNew->AccessibleDescription = nullptr;
			this->gNew->AccessibleName = nullptr;
			resources->ApplyResources(this->gNew, L"gNew");
			this->gNew->BackgroundImage = nullptr;
			this->gNew->Controls->Add(this->lNewModified);
			this->gNew->Controls->Add(this->lNewSize);
			this->gNew->Controls->Add(this->pNew);
			this->gNew->Font = nullptr;
			this->gNew->Name = L"gNew";
			this->gNew->TabStop = false;
			// 
			// lNewModified
			// 
			this->lNewModified->AccessibleDescription = nullptr;
			this->lNewModified->AccessibleName = nullptr;
			resources->ApplyResources(this->lNewModified, L"lNewModified");
			this->lNewModified->Font = nullptr;
			this->lNewModified->Name = L"lNewModified";
			// 
			// lNewSize
			// 
			this->lNewSize->AccessibleDescription = nullptr;
			this->lNewSize->AccessibleName = nullptr;
			resources->ApplyResources(this->lNewSize, L"lNewSize");
			this->lNewSize->Font = nullptr;
			this->lNewSize->Name = L"lNewSize";
			// 
			// pNew
			// 
			this->pNew->AccessibleDescription = nullptr;
			this->pNew->AccessibleName = nullptr;
			resources->ApplyResources(this->pNew, L"pNew");
			this->pNew->BackgroundImage = nullptr;
			this->pNew->Font = nullptr;
			this->pNew->ImageLocation = nullptr;
			this->pNew->Name = L"pNew";
			this->pNew->TabStop = false;
			// 
			// bYesAll
			// 
			this->bYesAll->AccessibleDescription = nullptr;
			this->bYesAll->AccessibleName = nullptr;
			resources->ApplyResources(this->bYesAll, L"bYesAll");
			this->bYesAll->BackgroundImage = nullptr;
			this->bYesAll->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->bYesAll->Font = nullptr;
			this->bYesAll->Name = L"bYesAll";
			this->bYesAll->UseVisualStyleBackColor = true;
			// 
			// bYes
			// 
			this->bYes->AccessibleDescription = nullptr;
			this->bYes->AccessibleName = nullptr;
			resources->ApplyResources(this->bYes, L"bYes");
			this->bYes->BackgroundImage = nullptr;
			this->bYes->DialogResult = System::Windows::Forms::DialogResult::Yes;
			this->bYes->Font = nullptr;
			this->bYes->Name = L"bYes";
			this->bYes->UseVisualStyleBackColor = true;
			// 
			// bNo
			// 
			this->bNo->AccessibleDescription = nullptr;
			this->bNo->AccessibleName = nullptr;
			resources->ApplyResources(this->bNo, L"bNo");
			this->bNo->BackgroundImage = nullptr;
			this->bNo->DialogResult = System::Windows::Forms::DialogResult::No;
			this->bNo->Font = nullptr;
			this->bNo->Name = L"bNo";
			this->bNo->UseVisualStyleBackColor = true;
			// 
			// bNoAll
			// 
			this->bNoAll->AccessibleDescription = nullptr;
			this->bNoAll->AccessibleName = nullptr;
			resources->ApplyResources(this->bNoAll, L"bNoAll");
			this->bNoAll->BackgroundImage = nullptr;
			this->bNoAll->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->bNoAll->Font = nullptr;
			this->bNoAll->Name = L"bNoAll";
			this->bNoAll->UseVisualStyleBackColor = true;
			// 
			// overwrite
			// 
			this->AcceptButton = this->bYes;
			this->AccessibleDescription = nullptr;
			this->AccessibleName = nullptr;
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = nullptr;
			this->CancelButton = this->bNo;
			this->ControlBox = false;
			this->Controls->Add(this->bNoAll);
			this->Controls->Add(this->bNo);
			this->Controls->Add(this->bYes);
			this->Controls->Add(this->bYesAll);
			this->Controls->Add(this->gNew);
			this->Controls->Add(this->gExisting);
			this->Font = nullptr;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"overwrite";
			this->Shown += gcnew System::EventHandler(this, &overwrite::overwrite_Shown);
			this->Load += gcnew System::EventHandler(this, &overwrite::overwrite_Load);
			this->gExisting->ResumeLayout(false);
			this->gExisting->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pExisting))->EndInit();
			this->gNew->ResumeLayout(false);
			this->gNew->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pNew))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void overwrite_Load(System::Object^  sender, System::EventArgs^  e) {
				 bYes->Select();
				 //this->BringToFront();
			 }
private: System::Void overwrite_Shown(System::Object^  sender, System::EventArgs^  e) {
			 this->TopMost = true;
		 }
};
}
