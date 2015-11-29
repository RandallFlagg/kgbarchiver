// kgb2compressor.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"
#include "fShutdown.h"
#include "../compress.cpp"
//#include "../KGB Archiver 2 .net/units.cpp"
//#include "../KGB Archiver 2 .net/file_management.cpp"
#include "shutdown.cpp"
#include "../ziparchive/ziparchive/ziparchive.h"
//#pragma comment (lib, "../ziparchive/ZipArchive/x64/Release/ZipArchive.lib")
#include "../language.h"

#define FORMAT_KGB 0
#define FORMAT_ZIP 1

#define ZIP_ERROR_BROKEN_ARCHIVE_OR_PASS -10
#define ZIP_ERROR_BROKEN_ARCHIVE KGB_ERROR_BROKEN_ARCHIVE
#define ERROR_INTERNAL -20

using namespace kgb2compressor;

bool isKGBArchive(wstring file){
	//return true;
	compressInfo cmsi;
	return (readHeader(file, &cmsi) == 0);
}

bool isZIPArchive(wstring file){
	CZipArchive zip;
	try{
		zip.Open(file.c_str(), CZipArchive::OpenMode::zipOpenReadOnly);
		zip[0]->GetFileName();
		zip.Close();
	}catch(Exception ^){
		return false;
	}
	return true;
}

ref class statsThread{
public:
	Form1 ^fMain;
	HANDLE *hThread;
	char mode;
	char format;
	bool runBackground;
	statsThread(){fMain = nullptr;mode=0;runBackground=false;}
	void start(){
		__int64 ratio;
		__int64 completion;
		array<Drawing::Icon^> ^ico = gcnew array<Drawing::Icon^>(6);

		while(!fMain->loaded)
			Sleep(10);

		ico[0] = Drawing::Icon::FromHandle(((Drawing::Bitmap^)fMain->iTrayList->Images[0])->GetHicon());
		ico[1] = Drawing::Icon::FromHandle(((Drawing::Bitmap^)fMain->iTrayList->Images[1])->GetHicon());
		ico[2] = Drawing::Icon::FromHandle(((Drawing::Bitmap^)fMain->iTrayList->Images[2])->GetHicon());
		ico[3] = Drawing::Icon::FromHandle(((Drawing::Bitmap^)fMain->iTrayList->Images[3])->GetHicon());
		ico[4] = Drawing::Icon::FromHandle(((Drawing::Bitmap^)fMain->iTrayList->Images[4])->GetHicon());
		ico[5] = Drawing::Icon::FromHandle(((Drawing::Bitmap^)fMain->iTrayList->Images[5])->GetHicon());

		if(mode == 0)
			fMain->lProjectedDesc->Text = getString("ProjectedSize");//L"Projected archive size:";

		if(runBackground)
			fMain->Form1_LocationChanged(NULL, EventArgs::Empty);

		if(format == FORMAT_ZIP){
			fMain->pCurrent->Style = ProgressBarStyle::Marquee;
		}
		

		/*fMain->pCurrent->Maximum = 100;
		fMain->pTotal->Maximum = 100;*/
		
		do{
			try{
				//throw 0;
				if(compressStatus.filename.length() >= 1){
					switch(compressStatus.filename.c_str()[0]){
						case L'\1': 
							fMain->lFilename->Text = getString("Encrypting");//gcnew String(L"Encrypting..."); 
							//fMain->pCurrent->Maximum = compressStatus.compressed_all;
							fMain->pCurrent->Value = compressStatus.encrypted*100/compressStatus.compressed_all;
							break;
						case L'\2': 
							fMain->lFilename->Text = getString("Decrypting");//gcnew String(L"Decrypting...");
							//fMain->pCurrent->Maximum = compressStatus.compressed_all;
							fMain->pCurrent->Value = compressStatus.encrypted*100/compressStatus.compressed_all;
							break;
						case L'\3':
							fMain->lFilename->Text = getString("CannotCreate");//gcnew String(L"Cannot create, skipping...");
							//fMain->pCurrent->Maximum = compressStatus.uncompressed_cur;
							fMain->pCurrent->Value = compressStatus.done_cur*100/compressStatus.uncompressed_cur;
							break;
						default: 
							fMain->lFilename->Text = gcnew String(compressStatus.filename.c_str()); 
							//fMain->pCurrent->Maximum = rand();//compressStatus.uncompressed_cur;
							fMain->pCurrent->Value = compressStatus.done_cur*100/compressStatus.uncompressed_cur;
							break;
					}
				}

				//fMain->pTotal->Maximum = compressStatus.uncompressed_all;
				fMain->pTotal->Value = compressStatus.done_all*100/compressStatus.uncompressed_all;

				fMain->Text = String::Format(L"{0:F1}% {1}...", (float)(compressStatus.done_all*1000/compressStatus.uncompressed_all)/10.0f, (mode==0 ? L"compressing" : (mode==1 ? L"decompressing" : L"testing")));
				fMain->iTray->Text = fMain->Text;
				fMain->iTray->BalloonTipTitle = fMain->Text;
				
				fMain->lBefore->Text = gcnew String(convertUnits(compressStatus.uncompressed_all).c_str());

				if(mode == 0)
					ratio = compressStatus.compressed_all*1000/(compressStatus.done_all == 0?1:compressStatus.done_all);
				else
					ratio = compressStatus.compressed_all*1000/(compressStatus.uncompressed_all == 0?1:compressStatus.uncompressed_all);

				fMain->lProjected->Text = gcnew String(convertUnits(ratio * compressStatus.uncompressed_all/1000).c_str());
				fMain->lRatio->Text = String::Format(L"{0:F1}%", (float)ratio/10.0f);
				
				fMain->lTime->Text = gcnew String(convertTime(_time64(NULL)-compressStatus.startTime).c_str());
				fMain->lRemaining->Text = gcnew String(convertTime((_time64(NULL)-compressStatus.startTime)*compressStatus.uncompressed_all/(compressStatus.done_all==0?1:compressStatus.done_all)-_time64(NULL)+compressStatus.startTime).c_str());

				if(mode == 0){
					
					fMain->iTray->BalloonTipText = String::Format(getString("CompressBalloonTip"),
					fMain->lProjected->Text, fMain->lRatio->Text,
					fMain->lTime->Text, fMain->lRemaining->Text);
				}else{ //extract or test
					fMain->iTray->BalloonTipText = String::Format(getString("DecompressBallonTip"),
					fMain->lProjected->Text, fMain->lRatio->Text,
					fMain->lTime->Text, fMain->lRemaining->Text);
				}

				
				
				completion = compressStatus.done_all*100/compressStatus.uncompressed_all;
				if(completion < 17){
					fMain->iTray->Icon = ico[0];
					fMain->Icon = ico[0];
				}else if(completion < 33){
					fMain->iTray->Icon = ico[1];
					fMain->Icon = ico[1];
				}else if(completion < 50){
					fMain->iTray->Icon = ico[2];
					fMain->Icon = ico[2];
				}else if(completion < 66){
					fMain->iTray->Icon = ico[3];
					fMain->Icon = ico[3];
				}else if(completion < 83){
					fMain->iTray->Icon = ico[4];
					fMain->Icon = ico[4];
				}else{
					fMain->iTray->Icon = ico[5];
					fMain->Icon = ico[5];
				}

				if(fMain->pCurrent->Style == ProgressBarStyle::Marquee)
					fMain->pCurrent->Refresh();

			}catch(Exception ^e){
				//MessageBox::Show(e->ToString()/*String::Format(L"Error code: {0}", err)*/);
			}
		}while(fMain->loaded && WaitForSingleObject(*hThread, 200) == WAIT_TIMEOUT/*compressStatus.done_all!=compressStatus.uncompressed_all*/);

		if(fMain->loaded)
			fMain->DialogResult = DialogResult::OK;
		fMain->Close();
	}
};
namespace kgb2compressor {
	using namespace System;
	using namespace System::IO;
	using namespace System::Threading;
	using namespace System::Globalization;
	using namespace Microsoft::Win32;

	DWORD WINAPI createZIPArchive(LPVOID lpParam){
		compressInfo cmsi = *(compressInfo*)lpParam;
		compressStatus.init(cmsi);

		CZipArchive zip;
		CZipFileHeader header;
		wchar_t pass[32];
		wstring orgArchName;

		if(cmsi.sfx){
			wchar_t temp[MAX_PATH];
			wchar_t tempArch[MAX_PATH];
			orgArchName = cmsi.archiveName;

			GetTempPath(MAX_PATH, temp);
			GetTempFileName(temp, L"", 0, tempArch);
			cmsi.archiveName = tempArch;
		}

		try{
			zip.Open(cmsi.archiveName.c_str(), CZipArchive::OpenMode::zipCreate);
		}catch(Exception^){
			return GetLastError();
		}

		if(cmsi.encryption.enabled){
			mbstowcs(pass, (char*)cmsi.encryption.pass, 32);
			zip.SetPassword(pass);
		}
		
		for(__int64 i=0;i<cmsi.files.size();i++){
			compressStatus.filename = cmsi.files[i].name;
			compressStatus.uncompressed_cur = cmsi.files[i].size;
			compressStatus.done_cur = 0;
			
			if(!zip.AddNewFile(cmsi.files[i].name, cmsi.algoMode)){
				Windows::Forms::MessageBox::Show(getString("AccessDeniedSkipping"), L"KGB Archiver");
			}

			zip.GetFileInfo(header, i);

			compressStatus.done_cur += cmsi.files[i].size;
			compressStatus.done_all += cmsi.files[i].size;
			compressStatus.compressed_all += header.m_uComprSize;
		}

		zip.Close();

		if(cmsi.sfx){
			char buffer[65536];
			int len;
			try{
				if(cmsi.sfx){
					File::Copy(gcnew String(getHome().c_str())+L"\\stubzip.exe", gcnew String(orgArchName.c_str()), true);
				}
			}catch(Exception^){
				return ERROR_INTERNAL;
			}
			FILE *ifile = _wfopen(cmsi.archiveName.c_str(), L"rb");
			if(ifile == NULL){
				return ERROR_ACCESS_DENIED;
			}
			FILE *ofile = _wfopen(orgArchName.c_str(), L"ab");
			if(ofile == NULL){
				fclose(ifile);
				return ERROR_ACCESS_DENIED;
			}
			
			while(!feof(ifile)){
				len = fread(buffer, 1, 65536, ifile);
				fwrite(buffer, 1, len, ofile);
			}

			fclose(ifile);
			fclose(ofile);
		}

		return 0;
	}

	DWORD WINAPI extractZIPArchive(LPVOID lpParam){
		compressInfo cmsi = *(compressInfo*)lpParam;
		compressStatus.init(cmsi);

		CZipArchive zip;
		CZipFileHeader header;
		_wfinddatai64_t fi;
		char tmpName[MAX_PATH];
		
		zip.Open(cmsi.archiveName.c_str(), CZipArchive::OpenMode::zipOpenReadOnly);
			for(int i=0;i<zip.GetCount(false);i++){
				if(zip[i]->IsDirectory())
					continue;
				compressStatus.uncompressed_all += zip[i]->m_uUncomprSize;
			}
		zip.Close();


		zip.Open(cmsi.archiveName.c_str(), CZipArchive::OpenMode::zipOpenReadOnly);
		
		if(cmsi.encryption.enabled){
			wchar_t pass[32];
			mbstowcs(pass, (char *)cmsi.encryption.pass, 32);
			
			zip.SetPassword(pass);
		}
		
		try{
			for(int i=0;i<zip.GetCount(false);i++){
				if(zip[i]->IsDirectory())
					continue;
				
				compressStatus.filename = zip[i]->GetFileName();
				compressStatus.uncompressed_cur = zip[i]->m_uUncomprSize;
				compressStatus.done_cur = 0;

				bool onList = true;
				bool ow = true;
				if(cmsi.files2decompress.size() != 0){
					onList = false;
					for(unsigned __int64 j=0;j<cmsi.files2decompress.size();++j){
						if(cmsi.files2decompress[j] == zip[i]->GetFileName()){
							onList = true;
							break;
						}
					}
				}

				wcstombs(tmpName, zip[i]->GetFileName().c_str(), MAX_PATH);
				if(onList && _access(tmpName, 0) == 0){
					wcscpy(fi.name, zip[i]->GetFileName().c_str());
					fi.size = zip[i]->m_uUncomprSize;
					fi.time_write = zip[i]->GetTime();
					ow = overwriteFile(&fi);
				}

				if(onList && ow)
					zip.ExtractFile(i, cmsi.destination.c_str());

				compressStatus.done_cur += zip[i]->m_uUncomprSize;
				compressStatus.done_all += zip[i]->m_uUncomprSize;
			}
		}catch(Exception^){
			if(zip[0]->IsEncrypted()){
				zip.Close();
				return ZIP_ERROR_BROKEN_ARCHIVE_OR_PASS;
			}
			zip.Close();
			return ZIP_ERROR_BROKEN_ARCHIVE;
		}

		zip.Close();

		return 0;
	}

	DWORD WINAPI testZIPArchive(LPVOID lpParam){
		compressInfo cmsi = *(compressInfo*)lpParam;
		compressStatus.init(cmsi);
		//MessageBox::Show("");
		CZipArchive zip;
		CZipFileHeader header;
		_wfinddatai64_t fi;
		char tmpName[MAX_PATH];
		
		zip.Open(cmsi.archiveName.c_str(), CZipArchive::OpenMode::zipOpenReadOnly);
			for(int i=0;i<zip.GetCount(false);i++){
				if(zip[i]->IsDirectory())
					continue;
				compressStatus.uncompressed_all += zip[i]->m_uUncomprSize;
			}
		zip.Close();


		zip.Open(cmsi.archiveName.c_str(), CZipArchive::OpenMode::zipOpenReadOnly);
		
		if(cmsi.encryption.enabled){
			wchar_t pass[32];
			mbstowcs(pass, (char *)cmsi.encryption.pass, 32);
			
			zip.SetPassword(pass);
		}
		try{
			for(int i=0;i<zip.GetCount(false);i++){
				if(zip[i]->IsDirectory())
					continue;
				
				compressStatus.filename = zip[i]->GetFileName();
				compressStatus.uncompressed_cur = zip[i]->m_uUncomprSize;
				compressStatus.done_cur = 0;

				
					zip.TestFile(i);
					//MessageBox::Show("");
				

				compressStatus.done_cur += zip[i]->m_uUncomprSize;
				compressStatus.done_all += zip[i]->m_uUncomprSize;
			}
		}catch(Exception^){
			if(zip[0]->IsEncrypted()){
				zip.Close();
				return ZIP_ERROR_BROKEN_ARCHIVE_OR_PASS;
			}
			zip.Close();
			return ZIP_ERROR_BROKEN_ARCHIVE;
		}

		zip.Close();

		return 0;
	}
}

//BECAUSE OF _fcloseall() SOMETIMES HANGS
DWORD WINAPI deleteArchive(LPVOID lpParam){
	//wstring arch = *(wstring*)lpParam;
	_fcloseall();
	
	if(lpParam){
		//MessageBox::Show(gcnew String(arch.c_str()));
		DeleteFile(/*((wstring*)lpParam)->c_str()*/((wstring*)lpParam)->c_str());
	}

	return 0;
}

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	//Application::Run(gcnew Form1());

	try{
		Thread::CurrentThread::get()->CurrentUICulture = gcnew CultureInfo((int)Registry::GetValue("HKEY_CURRENT_USER\\Software\\KGB Archiver 2\\general", "language", 0x0009));
	}catch(Exception ^){}
	
	char mode = 0; //0 - compress, 1 - decompress, 2 - test
	char format = FORMAT_KGB; //0 - kgb, 1 - zip
	wchar_t passwd[32];
	compressInfo cmsi;
	//wstring destination;
	bool openDir = false;
	bool removeArch = false;
	bool runBackground = false;
	
	overwriteMode = OVERWRITE_YES;

	for(int i=0;i<args->Length;i++){
		//MessageBox::Show(args[i]);
		//MessageBox::Show(/*gcnew String(cmsi.archiveName.c_str())*/args[i]->ToString());
		if(args[i][0] == L'-' && args[i]->Length > 1){
			switch(args[i][1]){
				case L'm': 
					switch(args[i][2]){
						case L'c': mode = 0; break;
						case L'd': mode = 1; break;
						case L't': mode = 2; break;
					}
					break;
				case L'f': format = (args[i][2] == L'k'? FORMAT_KGB : FORMAT_ZIP); break;
				case L'p':
					wcscpy(passwd, (wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(args[i]->Substring(2, (args[i]->Length > 34? 32 : args[i]->Length-2))).ToPointer()); 
					cmsi.encryption.enabled = true;
					cmsi.encryption.setPassword(passwd);
					//MessageBox::Show(gcnew String((char*)cmsi.encryption.pass));
					break;
				case L'a': cmsi.algorithm = args[i][2] - L'0'; break;
				case L'l': cmsi.algoMode = args[i][2] - L'0'; break;
				case L's': cmsi.sfx = true; break;
				case L'o': overwriteMode = (args[i][2] == L'f' ? OVERWRITE_ALL : (args[i][2] == L'a' ? OVERWRITE_YES : OVERWRITE_NEVER)); break;
				case L'd': openDir = true; break;
				case L'r': removeArch = true; break;
				case L'b': runBackground = true; break;
				case L't': 
					StreamReader ^strReader = gcnew StreamReader(args[i]->Substring(2, args[i]->Length-2));
					String ^s;
					if(mode == 0){ //compress
						try{
							while(1){
								s = strReader->ReadLine();
								
								if(!s)
									break;
								if(s->Substring(s->Length-1) != gcnew String(L"\\") && s->Substring(s->Length-1) != gcnew String(L"/"))
									cmsi.insertFile((wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(s).ToPointer());
								else
									cmsi.insertDir((wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(s->Substring(0, s->Length-1)).ToPointer());
							}
						}catch(Exception ^e){Windows::Forms::MessageBox::Show(e->ToString());}
					}else{ //decompress or test
						try{
							while(1){
								s = strReader->ReadLine();
								
								if(!s)
									break;
								cmsi.files2decompress.push_back((wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(s).ToPointer());
							}
						}catch(Exception ^e){Windows::Forms::MessageBox::Show(e->ToString());}
					}
					delete strReader;
					//delete s;
					break;
			}
			continue;
		}else if(args[i][0] == L'"'){
			args[i] = args[i]->Substring(1, args[i]->Length-2);
		}
		if(cmsi.archiveName.length() == 0){
			cmsi.archiveName = (wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(args[i]).ToPointer();
			continue;
		}
		if(mode == 0){//compress
			if(args[i]->Substring(args[i]->Length-1) != gcnew String(L"\\") && args[i]->Substring(args[i]->Length-1) != gcnew String(L"/"))
				cmsi.insertFile((wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(args[i]).ToPointer());
			else
				cmsi.insertDir((wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(args[i]->Substring(0, args[i]->Length-1)).ToPointer());
		}else{//decompress
			if(cmsi.destination.length() == 0)
				cmsi.destination = (wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(args[i]).ToPointer();
		}
	}
	
	if(cmsi.archiveName.length() == 0 || (mode == 0 && cmsi.files.size() == 0) || (mode == 1 && cmsi.destination.length() == 0)){ //|| (mode == 2 && cmsi.archiveName.length() == 0) - no needed
		MessageBox::Show(getString("WrongArguments"), L"KGB Archiver 2");
		return 0;
	}

	if(mode >= 1){//decompress or test
		if(isKGBArchive(cmsi.archiveName))
			format = FORMAT_KGB;
		else if(isZIPArchive(cmsi.archiveName))
			format = FORMAT_ZIP;
		else{
			MessageBox::Show(getString("UnrecognizedArch"), L"KGB Archiver 2");
			return 0;
		}
	}

	//MessageBox::Show(gcnew String(passwd));

	Form1 ^fMain = gcnew Form1;
	Form1 ^fMain2;
	HANDLE hThread;
	Threading::Thread ^hStatsThread;
	statsThread ^stats = gcnew statsThread;
	DWORD exitCode;

	//MessageBox::Show(gcnew String(destination.c_str()));
	
	if(format == FORMAT_KGB){//kgb
		if(mode == 0)
			hThread = CreateThread(NULL, NULL, &createArchive, &cmsi, NULL, NULL);
		else if(mode == 1){
			wchar_t tmp[1024];
			int err;
			cmsi.destination = cmsi.destination + L'\\';
			if(cmsi.destination.length() >= 2){
				for(int j=(cmsi.destination.c_str()[1] == L':'?3:0);j<cmsi.destination.length();j++){
					if(cmsi.destination.c_str()[j] == L'\\' || cmsi.destination.c_str()[j] == L'/'){
						wcscpy_s(tmp, sizeof(tmp)/sizeof(*tmp), cmsi.destination.c_str());
						tmp[j] = L'\0';
						//MessageBox::Show(gcnew String(tmp));
						err = NULL;
						if(CreateDirectory(tmp, NULL) == NULL && (err=GetLastError()) != ERROR_ALREADY_EXISTS){
							MessageBox::Show(getString("CannotCreateExtractDir"), L"KGB Archiver 2");
							return 0;
						}
					}
				}
			}
			SetCurrentDirectory(cmsi.destination.c_str());
			hThread = CreateThread(NULL, NULL, &extractArchive, &cmsi, NULL, NULL);
		}else{
			hThread = CreateThread(NULL, NULL, &testArchive, &cmsi, NULL, NULL);
		}
	}else if(format == FORMAT_ZIP){ //zip
		if(mode == 0)
			hThread = CreateThread(NULL, NULL, &createZIPArchive, &cmsi, NULL, NULL);
		else if(mode == 1){
			SetCurrentDirectory(cmsi.destination.c_str());
			hThread = CreateThread(NULL, NULL, &extractZIPArchive, &cmsi, NULL, NULL);
		}else{
			hThread = CreateThread(NULL, NULL, &testZIPArchive, &cmsi, NULL, NULL);
		}
	}

	stats->fMain = fMain;
	stats->hThread = &hThread;
	stats->mode = mode;
	stats->format = format;
	stats->runBackground = runBackground;
	hStatsThread = gcnew Threading::Thread(gcnew Threading::ThreadStart(stats, &statsThread::start));
	hStatsThread->Start();

	if(runBackground)
		fMain->WindowState = FormWindowState::Minimized;

	Application::Run(fMain);

	if(fMain->DialogResult == DialogResult::Cancel){
		hStatsThread->Abort();
		TerminateThread(hThread, 0);
		
		//MessageBox::Show(gcnew String(cmsi.archiveName.c_str()));
		//BECAUSE OF _fcloseall() HANGS SOMETIMES
		if(mode == 0)
			CreateThread(NULL, NULL, &deleteArchive, &cmsi.archiveName, NULL, NULL);
		else
			CreateThread(NULL, NULL, &deleteArchive, NULL, NULL, NULL);
		/*_fcloseall();
		if(mode==0)
			DeleteFile(destination.c_str());*/

		MessageBox::Show(getString("Aborted"), "KGB Archiver 2");
	}else{
		GetExitCodeThread(hThread, &exitCode);
		switch(exitCode){
			case NULL: 
				fMain2 = gcnew Form1;
				//fMain->iTray = gcnew NotifyIcon;
				fMain2->iTray->Icon = Drawing::Icon::FromHandle(((Drawing::Bitmap^)fMain2->iTrayList->Images[5])->GetHicon());
				fMain2->iTray->BalloonTipTitle = String::Format(getString("DoneBalloonTitle"), (mode==0 ? getString("Compression"): (mode==1 ? getString("Decompression") : getString("Test"))));
				//Windows::Forms::MessageBox::Show("1");
				if(mode == 0)
					fMain2->iTray->BalloonTipText = String::Format(getString("DoneCompressionBalloonTip"),
					gcnew String(convertUnits(compressStatus.compressed_all).c_str()),
					(float)(compressStatus.compressed_all*1000/compressStatus.uncompressed_all)/10.0f,
					gcnew String(convertTime(_time64(NULL)-compressStatus.startTime).c_str()));
				else if(mode == 1)
					fMain2->iTray->BalloonTipText = String::Format(getString("DoneDecompressionBallonTip"),
					gcnew String(cmsi.destination.c_str()),
					gcnew String(convertTime(_time64(NULL)-compressStatus.startTime).c_str()));
				else
					fMain2->iTray->BalloonTipText = String::Format(getString("DoneTestingBallonTip"), gcnew String(cmsi.archiveName.c_str()));
				//Windows::Forms::MessageBox::Show("2");
				fMain2->iTray->ShowBalloonTip(7500);
				
				if(openDir)
					ShellExecute(0, L"open", cmsi.destination.c_str(), L"", cmsi.destination.c_str(), SW_SHOW);
				if(removeArch)
					DeleteFile(cmsi.archiveName.c_str());

				if(fMain->cShutdown->Checked){
					fShutdown ^fs = gcnew fShutdown;
					if(fs->ShowDialog() == DialogResult::Cancel)
						return 0;
					
					shutdown();
					//MessageBox::Show(L"shutdown!");
				}else
					Sleep(7500);//for balloon tip
				
				fMain2->iTray->Visible = false;
				
				break;
			case KGB_ERROR_WRONG_PASSWORD: MessageBox::Show(getString("WrongPasswd"), L"KGB Archiver 2"); break;
			case KGB_ERROR_BROKEN_ARCHIVE: MessageBox::Show(getString("BrokenArch"), L"KGB Archiver 2"); break;
			case KGB_ERROR_SFX_NOT_FOUND: MessageBox::Show(getString("SFXNotFound"), L"KGB Archiver 2"); break;
			case ZIP_ERROR_BROKEN_ARCHIVE_OR_PASS: MessageBox::Show(getString("BrokenArchOrPasswd"), L"KGB Archiver 2"); break; 
			case ERROR_ACCESS_DENIED:
			case ERROR_SHARING_VIOLATION: MessageBox::Show(getString("AccessDenied"), L"KGB Archiver 2"); break; //occurs sometimes on vista 
			case ERROR_INTERNAL: MessageBox::Show(getString("InternalError"), L"KGB Archiver 2"); break;
			default: MessageBox::Show(getString("UndefiniedError") + String::Format(L"{0}", exitCode)); break;
		}

	}
	
	//Application::Run(fMain);

	return 0;
}
