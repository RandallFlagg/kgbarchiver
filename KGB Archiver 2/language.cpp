
#include <stdafx.h>

#ifndef _kgb2compressor
namespace KGBArchiver2net{
#else
namespace kgb2compressor{
#endif

	using namespace System;
	using namespace System::IO;
	using namespace System::Resources;
	using namespace System::Reflection;
	using namespace System::Collections;
	using namespace System::Collections::Specialized;
	using namespace System::Globalization;
	using namespace System::Diagnostics;
	//using namespace System::Windows::Forms;

	String ^getString(String ^name){
		//System::Windows::Forms::MessageBox::Show(caller->GetType()->ToString());
#ifndef _kgb2compressor
		ResourceManager ^LocRM = gcnew ResourceManager("KGBArchiver2net.strings", Assembly::GetExecutingAssembly());
#else
		ResourceManager ^LocRM = gcnew ResourceManager("kgb2compressor.strings", Assembly::GetExecutingAssembly());
#endif
		String ^str = LocRM->GetString(name);
		delete LocRM;
		return str;
	}

	ArrayList ^getLanguages(){
		ArrayList ^result = gcnew ArrayList();
		result->Add(CultureInfo::GetCultureInfo(0x0009));
		//MessageBox::Show(Path::GetDirectoryName(Process::GetCurrentProcess()->MainModule->FileName));
		DirectoryInfo ^appDir = gcnew DirectoryInfo(Path::GetDirectoryName(Process::GetCurrentProcess()->MainModule->FileName));
		cli::array<DirectoryInfo^>^dirs = appDir->GetDirectories();
		for(int i=0;i<dirs->Length;++i){
			try{
				//MessageBox::Show(dirs[i]->Name);
				result->Add(CultureInfo::GetCultureInfo(dirs[i]->Name));
			}catch(Exception ^/*e*/){
				//Windows::Forms::MessageBox::Show(e->ToString());
			}
		}

		return result;
	}
}