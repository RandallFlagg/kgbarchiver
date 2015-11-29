//#include "stdafx.h"
#include "../language.h"

namespace KGBArchiver2net{
	using namespace System;
	using namespace System::IO;
	using namespace System::Net;
	using namespace System::Text;
	using namespace System::Diagnostics;
	
	bool checkForUpdates(bool silent=false){
		HttpWebRequest ^http = dynamic_cast<HttpWebRequest^>(WebRequest::Create("http://kgbarchiver.sourceforge.net/update.txt"));
		Stream ^stream;
		String ^buffer;
		HttpWebResponse ^res;
		StreamReader ^sr;

		http->MaximumResponseHeadersLength = 4;

		//http->Timeout = 10000;//10sec
		http->CachePolicy = gcnew Cache::RequestCachePolicy(Cache::RequestCacheLevel::NoCacheNoStore);
		http->UserAgent = String::Format(L"KGB Archiver v{0}", Application::ProductVersion);
		http->Credentials = CredentialCache::DefaultCredentials;
		http->KeepAlive = false;
		//http->ServicePoint->ConnectionLeaseTimeout = 1;
		http->ServicePoint->UseNagleAlgorithm = false;

		try{
			res = dynamic_cast<HttpWebResponse^>(http->GetResponse());
			stream = res->GetResponseStream();
		}catch(Exception ^e){
			if(!silent)
				MessageBox::Show(e->Message, L"KGB Archiver 2");
			delete http;
			delete stream;
			delete res;
			return false;
		}

		sr = gcnew StreamReader(stream, Encoding::UTF8);
		buffer = sr->ReadToEnd();

		if(buffer->Substring(0, 3) != gcnew String(L"KGB")){
			if(!silent)
				MessageBox::Show(getString("InternalServerError"), L"KGB Archiver 2");
			res->Close();
			stream->Close();
			sr->Close();
			delete http;
			delete stream;
			delete res;
			delete sr;
			delete buffer;
			return false;
		}else{
			if((gcnew Version(buffer->Substring(3))) > (gcnew Version(Application::ProductVersion))){
				if(MessageBox::Show(getString("UpdateFound"), L"KGB Archiver 2", MessageBoxButtons::YesNo) == DialogResult::Yes){
					
					//TODO: AUTOMATIC UPDATE

					Process::Start(L"http://sourceforge.net/project/showfiles.php?group_id=162546");
				}
			}else if(!silent){
					MessageBox::Show(getString("NoUpdates"), L"KGB Archiver 2");
			}
			res->Close();
			stream->Close();
			sr->Close();
			delete http;
			delete stream;
			delete res;
			delete sr;
			delete buffer;
			return true;
		}
	}
};