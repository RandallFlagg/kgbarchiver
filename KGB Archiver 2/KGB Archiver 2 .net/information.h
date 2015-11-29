#pragma once

namespace KGBArchiver2net {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	/// <summary>
	/// Summary for information
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class information : public System::Windows::Forms::Form
	{
	public:
		information(void)
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
		~information()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::Label^  label1;
	public: System::Windows::Forms::Label^  lType;
	public: System::Windows::Forms::Label^  label3;
	public: System::Windows::Forms::Label^  lVersion;
	public: System::Windows::Forms::GroupBox^  gMain;
	public: System::Windows::Forms::GroupBox^  gFiles;
	public: System::Windows::Forms::Label^  lBeforeSize;
	public: System::Windows::Forms::Label^  label4;
	public: System::Windows::Forms::Label^  lNumFiles;
	public: System::Windows::Forms::Label^  label2;
	public: System::Windows::Forms::Label^  lCompressed;
	public: System::Windows::Forms::Label^  label5;
	public: System::Windows::Forms::Label^  lRatio;
	public: System::Windows::Forms::Label^  label6;
	public: System::Windows::Forms::Label^  lAlgorithm;
	public: System::Windows::Forms::Label^  label7;
	public: System::Windows::Forms::Label^  lLevel;
	public: System::Windows::Forms::Label^  label8;
	public: System::Windows::Forms::Label^  lPassword;
	public: System::Windows::Forms::Label^  label9;
	public: System::Windows::Forms::Button^  bOK;

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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->lType = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->lVersion = (gcnew System::Windows::Forms::Label());
			this->gMain = (gcnew System::Windows::Forms::GroupBox());
			this->lPassword = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->lLevel = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->lAlgorithm = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->gFiles = (gcnew System::Windows::Forms::GroupBox());
			this->lRatio = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->lCompressed = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->lBeforeSize = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->lNumFiles = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->bOK = (gcnew System::Windows::Forms::Button());
			this->gMain->SuspendLayout();
			this->gFiles->SuspendLayout();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<unsigned char>(238)));
			this->label1->Location = System::Drawing::Point(6, 16);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(82, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Archive type:";
			// 
			// lType
			// 
			this->lType->AutoSize = true;
			this->lType->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<unsigned char>(238)));
			this->lType->Location = System::Drawing::Point(138, 16);
			this->lType->Name = L"lType";
			this->lType->Size = System::Drawing::Size(32, 13);
			this->lType->TabIndex = 1;
			this->lType->Text = L"KGB";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(6, 38);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(83, 13);
			this->label3->TabIndex = 2;
			this->label3->Text = L"Archive version:";
			// 
			// lVersion
			// 
			this->lVersion->AutoSize = true;
			this->lVersion->Location = System::Drawing::Point(138, 38);
			this->lVersion->Name = L"lVersion";
			this->lVersion->Size = System::Drawing::Size(19, 13);
			this->lVersion->TabIndex = 3;
			this->lVersion->Text = L"v2";
			// 
			// gMain
			// 
			this->gMain->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->gMain->Controls->Add(this->lPassword);
			this->gMain->Controls->Add(this->label9);
			this->gMain->Controls->Add(this->lLevel);
			this->gMain->Controls->Add(this->label8);
			this->gMain->Controls->Add(this->lAlgorithm);
			this->gMain->Controls->Add(this->label7);
			this->gMain->Controls->Add(this->label1);
			this->gMain->Controls->Add(this->lVersion);
			this->gMain->Controls->Add(this->lType);
			this->gMain->Controls->Add(this->label3);
			this->gMain->Location = System::Drawing::Point(12, 12);
			this->gMain->Name = L"gMain";
			this->gMain->Size = System::Drawing::Size(308, 127);
			this->gMain->TabIndex = 4;
			this->gMain->TabStop = false;
			this->gMain->Text = L"Main information";
			// 
			// lPassword
			// 
			this->lPassword->AutoSize = true;
			this->lPassword->Location = System::Drawing::Point(138, 105);
			this->lPassword->Name = L"lPassword";
			this->lPassword->Size = System::Drawing::Size(25, 13);
			this->lPassword->TabIndex = 9;
			this->lPassword->Text = L"Yes";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(6, 105);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(106, 13);
			this->label9->TabIndex = 8;
			this->label9->Text = L"Password protection:";
			// 
			// lLevel
			// 
			this->lLevel->AutoSize = true;
			this->lLevel->Location = System::Drawing::Point(138, 83);
			this->lLevel->Name = L"lLevel";
			this->lLevel->Size = System::Drawing::Size(69, 13);
			this->lLevel->TabIndex = 7;
			this->lLevel->Text = L"Not available";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(6, 83);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(95, 13);
			this->label8->TabIndex = 6;
			this->label8->Text = L"Compression level:";
			// 
			// lAlgorithm
			// 
			this->lAlgorithm->AutoSize = true;
			this->lAlgorithm->Location = System::Drawing::Point(138, 61);
			this->lAlgorithm->Name = L"lAlgorithm";
			this->lAlgorithm->Size = System::Drawing::Size(83, 13);
			this->lAlgorithm->TabIndex = 5;
			this->lAlgorithm->Text = L"No compression";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(6, 61);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(115, 13);
			this->label7->TabIndex = 4;
			this->label7->Text = L"Compression algorithm:";
			// 
			// gFiles
			// 
			this->gFiles->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->gFiles->Controls->Add(this->lRatio);
			this->gFiles->Controls->Add(this->label6);
			this->gFiles->Controls->Add(this->lCompressed);
			this->gFiles->Controls->Add(this->label5);
			this->gFiles->Controls->Add(this->lBeforeSize);
			this->gFiles->Controls->Add(this->label4);
			this->gFiles->Controls->Add(this->lNumFiles);
			this->gFiles->Controls->Add(this->label2);
			this->gFiles->Location = System::Drawing::Point(12, 145);
			this->gFiles->Name = L"gFiles";
			this->gFiles->Size = System::Drawing::Size(308, 103);
			this->gFiles->TabIndex = 5;
			this->gFiles->TabStop = false;
			this->gFiles->Text = L"Files";
			// 
			// lRatio
			// 
			this->lRatio->AutoSize = true;
			this->lRatio->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<unsigned char>(238)));
			this->lRatio->Location = System::Drawing::Point(138, 82);
			this->lRatio->Name = L"lRatio";
			this->lRatio->Size = System::Drawing::Size(48, 13);
			this->lRatio->TabIndex = 7;
			this->lRatio->Text = L"100,0%";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<unsigned char>(238)));
			this->label6->Location = System::Drawing::Point(6, 82);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(41, 13);
			this->label6->TabIndex = 6;
			this->label6->Text = L"Ratio:";
			// 
			// lCompressed
			// 
			this->lCompressed->AutoSize = true;
			this->lCompressed->Location = System::Drawing::Point(138, 60);
			this->lCompressed->Name = L"lCompressed";
			this->lCompressed->Size = System::Drawing::Size(120, 13);
			this->lCompressed->TabIndex = 5;
			this->lCompressed->Text = L"175,3MB (183815372B)";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(6, 60);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(89, 13);
			this->label5->TabIndex = 4;
			this->label5->Text = L"Compressed size:";
			// 
			// lBeforeSize
			// 
			this->lBeforeSize->AutoSize = true;
			this->lBeforeSize->Location = System::Drawing::Point(138, 38);
			this->lBeforeSize->Name = L"lBeforeSize";
			this->lBeforeSize->Size = System::Drawing::Size(120, 13);
			this->lBeforeSize->TabIndex = 3;
			this->lBeforeSize->Text = L"175,3MB (183815372B)";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(6, 38);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(125, 13);
			this->label4->TabIndex = 2;
			this->label4->Text = L"Size before compression:";
			// 
			// lNumFiles
			// 
			this->lNumFiles->AutoSize = true;
			this->lNumFiles->Location = System::Drawing::Point(138, 16);
			this->lNumFiles->Name = L"lNumFiles";
			this->lNumFiles->Size = System::Drawing::Size(31, 13);
			this->lNumFiles->TabIndex = 1;
			this->lNumFiles->Text = L"1000";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(6, 16);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(80, 13);
			this->label2->TabIndex = 0;
			this->label2->Text = L"Number of files:";
			// 
			// bOK
			// 
			this->bOK->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->bOK->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->bOK->Location = System::Drawing::Point(245, 254);
			this->bOK->Name = L"bOK";
			this->bOK->Size = System::Drawing::Size(75, 23);
			this->bOK->TabIndex = 6;
			this->bOK->Text = L"OK";
			this->bOK->UseVisualStyleBackColor = true;
			// 
			// information
			// 
			this->AcceptButton = this->bOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(332, 282);
			this->Controls->Add(this->bOK);
			this->Controls->Add(this->gFiles);
			this->Controls->Add(this->gMain);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"information";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"Information";
			this->gMain->ResumeLayout(false);
			this->gMain->PerformLayout();
			this->gFiles->ResumeLayout(false);
			this->gFiles->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
