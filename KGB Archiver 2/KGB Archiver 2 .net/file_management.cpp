#include <io.h>
#include <string>
#include <windows.h>
#include <shellapi.h>
#include <vector>

#ifndef _kgb2compressor
#include "settings.cpp"
#endif

#include "units.cpp"
#include "../compress.cpp"

#include "../ziparchive/ziparchive/ziparchive.h"

using namespace std;
/*#ifndef _kgb2compressor
using namespace KGBArchiver2net;
#endif*/

namespace KGBArchiver2net{
	using namespace System::IO;
	using namespace System::Windows::Forms;
	/*using namespace ICSharpCode::SharpZipLib;
	using namespace ICSharpCode::SharpZipLib::Zip;*/

	SHFILEINFO getIcon(wstring file){
		SHFILEINFO shfi;
		memset(&shfi,0,sizeof(shfi));
		SHGetFileInfo(file.c_str(), FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(shfi), SHGFI_ICON|SHGFI_USEFILEATTRIBUTES|/*SHGFI_SHELLICONSIZE|*/SHGFI_SMALLICON|SHGFI_TYPENAME);
		return shfi;
	}

	bool isKGBArchive(wstring file){
		//return true;
		compressInfo cmsi;
		return (readHeader(file, &cmsi) == 0);
	}
	vector<wstring> toDirVector(wstring dir){
		for(int i=0;i<dir.size();i++)
			if(dir[i] == L'/')
				dir[i] = L'\\';

		vector<wstring>r;
		int j=0;
		for(int i=0;(i = dir.find(L"\\", i)) != wstring::npos;j=++i){
			r.push_back(dir.substr(j, i-j));
		}
		return r;
	}

	bool isKGBArchive(wstring dir, wstring &arch_name){
		vector<wstring>dirs;
		bool isArch = false;
		
		dirs = toDirVector(dir);
		arch_name = dirs[0];

		for(int i=1;i<dirs.size();i++){
			arch_name = arch_name + L"\\" + dirs[i];
			if(isKGBArchive(arch_name)){
				isArch = true;
				break;
			}
		}
		return isArch;
	}

	bool isZIPArchive(wstring dir, wstring &arch_name){
		vector<wstring>dirs;
		bool isArch = false;
		
		dirs = toDirVector(dir);
		arch_name = dirs[0];

		CZipArchive zip;
		for(int i=1;i<dirs.size();i++){
			arch_name = arch_name + L"\\" + dirs[i];
			
			
			try{
				
				zip.Open(arch_name.c_str(), CZipArchive::OpenMode::zipOpenReadOnly);
				isArch = true;
				zip.Close();
				break;
			}catch(Exception ^){
				zip.CloseFile();
				//continue
			}
		}
		/*if(!zip.IsClosed())
			zip.Close();*/
		//TODO: check if there is opened file
		_fcloseall();
		return isArch;
	}

	bool isZIPArchive(wstring dir){
		wstring arch_name;
		if(dir[dir.length()-1] != L'/' && dir[dir.length()-1] != L'\\')
			dir += L'\\';
		return isZIPArchive(dir, arch_name);
	}

	void FillDir(ListView^ lFiles, wstring dir=L""){
		struct _wfinddatai64_t c_file;
		long hFile;
		wstring arch_name;
		//int dirTreeIdx;
		wstring tmp,tmp2;

		SHFILEINFO shfi;
		//HICON ico = NULL;
		
		lFiles->BeginUpdate();
		lFiles->Items->Clear();

		for(int i=1;i<lFiles->SmallImageList->Images->Count;i++){
			lFiles->SmallImageList->Images->RemoveAt(i);
		}

		if(isKGBArchive(dir, arch_name)){
			compressInfo cmsi;
			vector<wstring>adirs;
			
			readHeader(arch_name, &cmsi);
			adirs = toDirVector(dir.substr(arch_name.length()+1));

			lFiles->Items->Insert(lFiles->Items->Count, gcnew System::String(L".."), 0);
			lFiles->Items[lFiles->Items->Count-1]->Group = lFiles->Groups[L"lvgDir"];

			for(int i=0;i<cmsi.files.size();i++){

				vector<wstring> fdirs;
				fdirs = toDirVector(cmsi.files[i].name);
				
				if(fdirs.size() == adirs.size()){
					bool contains = false;
					int j=fdirs.size()-1, k=adirs.size()-1;
					for(;j >= 0 && k >= 0;){
						if(fdirs[j] != adirs[k]){
							contains = true;
							break;
						}
						j--;
						k--;
					}
					if(contains)
						continue;

					shfi = getIcon(cmsi.files[i].name);
					
					tmp2 = L"";
					for(int j=0;j<fdirs.size();j++)
						tmp2 = tmp2 + L"\\" + fdirs[j];

					tmp = (wstring)cmsi.files[i].name;
					tmp = tmp.substr(tmp2.length());
					

					if(shfi.hIcon != NULL){
						lFiles->SmallImageList->Images->Add(System::Drawing::Icon::FromHandle(System::IntPtr(shfi.hIcon)));
						lFiles->Items->Add(gcnew System::String(tmp.c_str()), gcnew System::String(tmp.c_str()), lFiles->SmallImageList->Images->Count-1);
					}else
						lFiles->Items->Add(gcnew System::String(tmp.c_str()), gcnew System::String(tmp.c_str()));

					lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(System::String(convertUnits(cmsi.files[i].size).c_str()).ToString());
					lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(gcnew System::String(shfi.szTypeName));
					lFiles->Items[lFiles->Items->Count-1]->Group = lFiles->Groups[L"lvgFile"];

				}else{
					bool contains = false;
					
					if(adirs.size() >= fdirs.size())
						continue;
					if(adirs.size() > 0){
						for(int j=adirs.size()-1;j>=0;j--){
							if(fdirs[j] != adirs[j]){
								contains = true;
								break;
							}
						}
						if(contains)
							continue;
					}

					contains = false;
					for(int j=0;j<lFiles->Items->Count;j++){
						if(lFiles->Items[j]->Text == gcnew System::String(fdirs[adirs.size()].c_str())){
							contains = true;
							break;
						}
					}
					
					if(!contains){
						lFiles->Items->Insert(lFiles->Items->Count, System::String(fdirs[adirs.size()].c_str()).ToString(), 0);
						lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(L"");
						lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(L"Directory");
						lFiles->Items[lFiles->Items->Count-1]->Group = lFiles->Groups[L"lvgDir"];
					}

					continue;
				}
				
				if(cmsi.version >= 2){
					lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(System::DateTime(1970, 1, 1).AddSeconds(cmsi.files[i].time_create).ToShortDateString()/*System::String(_wctime64(&cmsi.files[i].time_create)).ToString()->Substring(0, 24)*/);
					lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(System::DateTime(1970, 1, 1).AddSeconds(cmsi.files[i].time_write).ToShortDateString()/*System::String(_wctime64(&cmsi.files[i].time_access)).ToString()->Substring(0, 24)*/);
					lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(System::DateTime(1970, 1, 1).AddSeconds(cmsi.files[i].time_access).ToShortDateString()/*System::String(_wctime64(&cmsi.files[i].time_write)).ToString()->Substring(0, 24)*/);
					lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(System::String(convertAttributes(cmsi.files[i].attrib).c_str()).ToString());
				}
			}

		}else if(isZIPArchive(dir.c_str(), arch_name)){
			CZipArchive zip;
			CZipFileHeader header;
			wstring cur;
			zip.Open(arch_name.c_str(), CZipArchive::OpenMode::zipOpenReadOnly);

			lFiles->Items->Insert(lFiles->Items->Count, gcnew System::String(L".."), 0);
			lFiles->Items[lFiles->Items->Count-1]->Group = lFiles->Groups[L"lvgDir"];

			vector<wstring>adirs;
			adirs = toDirVector(dir.substr(arch_name.length()+1));

			for(__int64 i=0;i<zip.GetCount(false);i++){
				zip.GetFileInfo(header, i);
				if(header.IsDirectory())
					continue;
				//zip.
				//header = *zip[i];
				cur = header.GetFileName();
				

				vector<wstring> fdirs;
				fdirs = toDirVector(cur);

				if(fdirs.size() == adirs.size()){
					bool contains = false;
					int j=fdirs.size()-1, k=adirs.size()-1;
					for(;j >= 0 && k >= 0;){
						if(fdirs[j] != adirs[k]){
							contains = true;
							break;
						}
						j--;
						k--;
					}
					if(contains)
						continue;

					shfi = getIcon(cur.c_str());
					
					tmp2 = L"";
					for(int j=0;j<fdirs.size();j++)
						tmp2 = tmp2 + L"\\" + fdirs[j];

					tmp = cur;
					tmp = tmp.substr(tmp2.length());
					

					if(shfi.hIcon != NULL){
						lFiles->SmallImageList->Images->Add(System::Drawing::Icon::FromHandle(System::IntPtr(shfi.hIcon)));
						lFiles->Items->Add(gcnew System::String(tmp.c_str()), gcnew System::String(tmp.c_str()), lFiles->SmallImageList->Images->Count-1);
					}else
						lFiles->Items->Add(gcnew System::String(tmp.c_str()), gcnew System::String(tmp.c_str()));

					lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(System::String(convertUnits(header.m_uUncomprSize).c_str()).ToString());
					lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(gcnew System::String(shfi.szTypeName));
					lFiles->Items[lFiles->Items->Count-1]->Group = lFiles->Groups[L"lvgFile"];
				}else{
					bool contains = false;
					
					if(adirs.size() >= fdirs.size())
						continue;
					if(adirs.size() > 0){
						for(int j=adirs.size()-1;j>=0;j--){
							if(fdirs[j] != adirs[j]){
								contains = true;
								break;
							}
						}
						if(contains)
							continue;
					}

					contains = false;
					for(int j=0;j<lFiles->Items->Count;j++){
						if(lFiles->Items[j]->Text == gcnew System::String(fdirs[adirs.size()].c_str())){
							contains = true;
							break;
						}
					}
					
					if(!contains){
						lFiles->Items->Insert(lFiles->Items->Count, System::String(fdirs[adirs.size()].c_str()).ToString(), 0);
						lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(L"");
						lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(L"Directory");
						lFiles->Items[lFiles->Items->Count-1]->Group = lFiles->Groups[L"lvgDir"];
					}

					continue;
				}
				lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(gcnew String(L""));
				lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(System::DateTime(1970, 1, 1).AddSeconds(header.GetTime()).ToShortDateString());
				lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(gcnew String(L""));
				lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(System::String(convertAttributes(header.GetOriginalAttributes()).c_str()).ToString());
			}
			zip.Close();
		}else if(SetCurrentDirectory(dir.c_str())){

			if((hFile = _wfindfirsti64(L"*.*", &c_file)) == -1L){
				//goto end //return;
			}else{
				FileInfo ^fi;

				do{
					if(/*(c_file.name[0] == L'.' && c_file.name[1] == L'.') || */c_file.name[0] == L'.' && c_file.name[1] == L'\0')
						continue;
	//#ifndef _kgb2compressor
					if(c_file.attrib & _A_HIDDEN && !settings::fileManager::showHidden)
						continue;
	//#endif
					
					fi = gcnew System::IO::FileInfo(gcnew System::String((dir+c_file.name).c_str()));

					if(c_file.attrib & _A_SUBDIR){
						lFiles->Items->Insert(lFiles->Items->Count, System::String(c_file.name).ToString(), 0);
						lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(L"");
						lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(L"Directory");
						lFiles->Items[lFiles->Items->Count-1]->Group = lFiles->Groups[L"lvgDir"];
					}else{
						shfi = getIcon(c_file.name);

						if(shfi.hIcon != NULL){
							lFiles->SmallImageList->Images->Add(System::Drawing::Icon::FromHandle(System::IntPtr(shfi.hIcon)));
							lFiles->Items->Add(gcnew System::String(c_file.name), gcnew System::String(c_file.name), lFiles->SmallImageList->Images->Count-1);
						}else
							lFiles->Items->Add(gcnew System::String(c_file.name), gcnew System::String(c_file.name));

						lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(System::String(convertUnits(c_file.size).c_str()).ToString());
						lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(gcnew System::String(shfi.szTypeName));
						
						if(wcslen(c_file.name) > 3){
							/*heuristics*/
							if(tolower(c_file.name[wcslen(c_file.name)-3]) == L'k' && tolower(c_file.name[wcslen(c_file.name)-2]) == L'g' && tolower(c_file.name[wcslen(c_file.name)-1]) == L'b'){
								if(isKGBArchive(c_file.name))
									lFiles->Items[lFiles->Items->Count-1]->Group = lFiles->Groups[L"lvgArch"];
								
							}else if(tolower(c_file.name[wcslen(c_file.name)-3]) == L'e' && tolower(c_file.name[wcslen(c_file.name)-2]) == L'x' && tolower(c_file.name[wcslen(c_file.name)-1]) == L'e'){
								if(isKGBArchive(c_file.name) || isZIPArchive((dir+c_file.name).c_str()))
									lFiles->Items[lFiles->Items->Count-1]->Group = lFiles->Groups[L"lvgArch"];
								
							}else if(tolower(c_file.name[wcslen(c_file.name)-3]) == L'z' && tolower(c_file.name[wcslen(c_file.name)-2]) == L'i' && tolower(c_file.name[wcslen(c_file.name)-1]) == L'p'){
								if(isZIPArchive((dir+c_file.name).c_str()))
									lFiles->Items[lFiles->Items->Count-1]->Group = lFiles->Groups[L"lvgArch"];
								
							}
						}
						
						if(lFiles->Items[lFiles->Items->Count-1]->Group != lFiles->Groups[L"lvgArch"])
							lFiles->Items[lFiles->Items->Count-1]->Group = lFiles->Groups[L"lvgFile"];
					}
					
					
					lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(fi->CreationTime.ToShortDateString()/*System::String(_wctime64(&c_file.time_create)).ToString()->Substring(0, 24)*/);
					lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(fi->LastWriteTime.ToShortDateString()/*System::String(_wctime64(&c_file.time_access)).ToString()->Substring(0, 24)*/);
					lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(fi->LastAccessTime.ToShortDateString()/*::String(_wctime64(&c_file.time_write)).ToString()->Substring(0, 24)*/);
					lFiles->Items[lFiles->Items->Count-1]->SubItems->Add(System::String(convertAttributes(c_file.attrib).c_str()).ToString());

					delete fi;

				}while(_wfindnexti64(hFile, &c_file) == 0);

			}
			_findclose(hFile);
		}else{
			//if(!isKGBArchive(dir.substr(0, dir.length()-1)) && SetCurrentDirectory(dir.c_str()) == NULL){
			//System::Windows::Forms::MessageBox::Show("Cannot access directory!", "KGB Archiver", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Warning);
			//return;
			//}
		}
		
		lFiles->EndUpdate();
	}

	void _FillTree(System::Windows::Forms::TreeNode ^tn, bool recurrency = 1){
		struct _wfinddatai64_t c_file;
		long hFile;

		tn->Nodes->Clear();
		
		if((hFile = _wfindfirsti64(L"*.*", &c_file)) == -1L){
			//goto end
		}else{
			
			do{
				if((c_file.name[0] == L'.' && c_file.name[1] == L'.') || c_file.name[0] == L'.'/* && c_file.name[1] == L'.'*/)
					continue;

					if(c_file.attrib & _A_HIDDEN && !settings::fileManager::showHidden)
						continue;
				
				if(c_file.attrib & _A_SUBDIR){
					tn->Nodes->Add(gcnew String(c_file.name), gcnew String(c_file.name), 2);
					if(recurrency){
						SetCurrentDirectory((wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(tn->Nodes[gcnew String(c_file.name)]->FullPath).ToPointer());
						_FillTree(tn->Nodes[gcnew String(c_file.name)], 0);
						SetCurrentDirectory((wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(tn->FullPath).ToPointer());
					}
				}else if(wcslen(c_file.name) > 3 && settings::fileManager::showArchiveTree){
					/*heuristics*/
					if(tolower(c_file.name[wcslen(c_file.name)-3]) == L'k' && tolower(c_file.name[wcslen(c_file.name)-2]) == L'g' && tolower(c_file.name[wcslen(c_file.name)-1]) == L'b'){
						if(isKGBArchive((wstring)(wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(tn->FullPath).ToPointer() + L"\\" + c_file.name))
							tn->Nodes->Add(gcnew String(c_file.name), gcnew String(c_file.name), 3);

					}else if(tolower(c_file.name[wcslen(c_file.name)-3]) == L'e' && tolower(c_file.name[wcslen(c_file.name)-2]) == L'x' && tolower(c_file.name[wcslen(c_file.name)-1]) == L'e'){
						if(isKGBArchive((wstring)(wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(tn->FullPath).ToPointer() + L"\\" + c_file.name))
							tn->Nodes->Add(gcnew String(c_file.name), gcnew String(c_file.name), 3);
						else if(isZIPArchive((wstring)(wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(tn->FullPath).ToPointer() + L"\\" + c_file.name))
							tn->Nodes->Add(gcnew String(c_file.name), gcnew String(c_file.name), 4);	

					}else if(tolower(c_file.name[wcslen(c_file.name)-3]) == L'z' && tolower(c_file.name[wcslen(c_file.name)-2]) == L'i' && tolower(c_file.name[wcslen(c_file.name)-1]) == L'p'){
						if(isZIPArchive((wstring)(wchar_t*)Runtime::InteropServices::Marshal::StringToHGlobalUni(tn->FullPath).ToPointer() + L"\\" + c_file.name))
							tn->Nodes->Add(gcnew String(c_file.name), gcnew String(c_file.name), 4);
					}
				}


			}while(_wfindnexti64(hFile, &c_file) == 0);
		}
		_findclose(hFile);
	}

	void addNode(System::Windows::Forms::TreeNode ^tn, vector<wstring>dirs, wstring full, System::Windows::Forms::TreeView ^tFiles, int n=0, bool recursive=1){
		if(n >= dirs.size())
			return;
		//tn->Nodes->Clear();
		bool contains = false;
		String ^str = gcnew String(dirs[n].c_str());
		int i;

		for(i=0;i<tn->Nodes->Count;i++){
			if(tn->Nodes[i]->Text == str){
				contains = true;
				break;
			}
		}

		if(!contains){
			tn->Nodes->Add(str);
			//i = tn->Nodes->Count-1;
		}
		
		if(gcnew String(full.c_str()) == tn->Nodes[tn->Nodes->Count-1]->FullPath)
			tFiles->SelectedNode = tn->Nodes[tn->Nodes->Count-1];

		if(recursive)
			addNode(tn->Nodes[/*tn->Nodes->Count-1*/i], dirs, full, tFiles, n+1);

		delete str;
	}

	void FillTree(TreeView ^tFiles, wstring dir=L""){
		wstring arch_name;
		//bool isArch;

		vector<wstring> dirVector = toDirVector(dir);
		TreeNode ^tn = tFiles->Nodes[gcnew String(dirVector[0].c_str())];

		wstring curPath = dirVector[0] + L"\\";
		//isArch = isKGBArchive(dir, arch_name);

		tFiles->BeginUpdate();

		if(/*isArch*/isKGBArchive(dir, arch_name) && settings::fileManager::showArchiveTree){

			for(int i=1;curPath != arch_name + L"\\" && i < dirVector.size();i++){
				try{
					SetCurrentDirectory(curPath.c_str());
					_FillTree(tn);
					curPath = curPath + dirVector[i] + L"\\";
					//MessageBox(0, curPath.c_str(), L"", 0);
					tn = tn->Nodes[gcnew String(dirVector[i].c_str())];
				}catch(int err){
					break;
				}
			}

			//System::Windows::Forms::MessageBox::Show(tn->FullPath);

			compressInfo cmsi;
			vector<wstring>adirs;
			
			readHeader(arch_name/*dir.substr(0, dir.length()-1)*/, &cmsi);
			adirs = toDirVector(dir.substr(arch_name.length()+1));
			
			
			for(int i=0;i<cmsi.files.size();i++){
				vector<wstring> fdirs;
				fdirs = toDirVector(cmsi.files[i].name);

				addNode(tn, fdirs, dir.substr(0, dir.length()-1), tFiles);
			}
			if(dir.substr(0, dir.length()-1) == arch_name){
				tFiles->SelectedNode = tn;
				tn->SelectedImageIndex = 3;
			}

		}else if(isZIPArchive(dir, arch_name) && settings::fileManager::showArchiveTree){
			for(int i=1;curPath != arch_name + L"\\" && i < dirVector.size();i++){
				try{
					SetCurrentDirectory(curPath.c_str());
					_FillTree(tn);
					curPath = curPath + dirVector[i] + L"\\";
					//MessageBox(0, curPath.c_str(), L"", 0);
					tn = tn->Nodes[gcnew String(dirVector[i].c_str())];
				}catch(int err){
					break;
				}
			}

			CZipArchive zip;
			CZipFileHeader header;
			wstring cur;
			zip.Open(arch_name.c_str(), CZipArchive::OpenMode::zipOpenReadOnly);

			vector<wstring>adirs;
			adirs = toDirVector(dir.substr(arch_name.length()+1));
			
			for(int i=0;i<zip.GetCount(false);i++){
				vector<wstring> fdirs;
				zip.GetFileInfo(header, i);
				if(header.IsDirectory())
					continue;

				cur = header.GetFileName();
				fdirs = toDirVector(cur);

				addNode(tn, fdirs, dir.substr(0, dir.length()-1), tFiles);
			}
			if(dir.substr(0, dir.length()-1) == arch_name){
				tFiles->SelectedNode = tn;
				tn->SelectedImageIndex = 4;
			}
			zip.Close();
		}else if(SetCurrentDirectory(dir.c_str())){
			//MessageBox(0, L"", L"", 0);
			int i;
			for(i=1;i<dirVector.size();i++){
				try{
					
					SetCurrentDirectory(curPath.c_str());
					_FillTree(tn);		

					curPath = curPath + dirVector[i] + L"\\";

					tn = tn->Nodes[gcnew System::String(dirVector[i].c_str())];
				}catch(int err){
					break;
				}
			}
			SetCurrentDirectory(curPath.c_str());
			_FillTree(tn);

			tFiles->SelectedNode = tn;
		}else{
			//MessageBox::Show("Cannot access directory!", "KGB Archiver", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			//tFiles->EndUpdate();
			//return;
		}
		tFiles->EndUpdate();
	}

	wstring filterAddress(wstring addr){
		int j;
		int i=0;
		while((i=addr.find(L"\\..", i)) != wstring::npos){
			j = addr.find_last_of(L'\\',i-1);
			if(j == wstring::npos){ //C:\..
				addr = addr.substr(0, i);
				break;
			}
			addr = addr.substr(0, j) + addr.substr(i+3);
			i = j;
		}

		i = 0;
		while((i=addr.find(L"\\\\", i)) != wstring::npos){
			addr = addr.substr(0, i) + addr.substr(i+1);
		}
		if(addr.substr(addr.size()-1).c_str()[0] != L'\\')
			addr = addr + L"\\";

		return addr;
	}

	#define WIPE_BUFFER_SIZE 65536*2

	int _wipe(wstring filename, bool del=1){
		char buffer[WIPE_BUFFER_SIZE];
		__int64 len;
		__int64 BUFF_SIZE;

		srand(time(NULL));
		BUFF_SIZE = rand()%(WIPE_BUFFER_SIZE/2)+WIPE_BUFFER_SIZE/2;//randomize buffer size
															 //64KB-128KB

		FILE *file = _wfopen(filename.c_str(), L"r+bc");
		if(file == NULL){
			//MessageBox(0, filename.c_str(), L"", 0);
			return GetLastError();
		}
		
		setvbuf(file, NULL, _IONBF, NULL);
		
		_fseeki64(file, 0LL, SEEK_END);
		len = _ftelli64(file);
		_fseeki64(file, 0LL, SEEK_SET);

		for(int i=0;i<BUFF_SIZE;i++){
			buffer[i] = rand()%256;
		}

		for(__int64 i=0;i<len;i+=BUFF_SIZE){
			fwrite(buffer, sizeof(*buffer), (i+BUFF_SIZE < len ? BUFF_SIZE : len%BUFF_SIZE), file);
		}

		fflush(file);
		fclose(file);
		if(del){
			DeleteFile(filename.c_str());
		}

		return 0;
	}

	int wipe(wstring filename, bool del=1){
		wstring tmp;
		vector<wstring>dirs;
		int err;
		long hFile;
		struct _wfinddatai64_t c_file;

		
		if(SetCurrentDirectory(filename.c_str())){
			SetCurrentDirectory(L"..");

			dirs.push_back(filename);

			for(int i=0;i<dirs.size();i++){
				vector<wstring>filelist;
				
				tmp = dirs[i] + L"\\*.*";

				if((hFile = _wfindfirsti64(tmp.c_str(), &c_file)) == -1L){
					_findclose(hFile);
					return GetLastError();
				}else{
					
					do{
						if(/*(c_file.name[0] == L'.' && c_file.name[1] == L'.') ||*/ c_file.name[0] == L'.'/* && c_file.name[1] == L'.'*/)
							continue;
						
						if(c_file.attrib & _A_SUBDIR){
							dirs.push_back(dirs[i] + L"\\" + c_file.name);
						}else{
							/*if((err = _wipe(dirs[i] + L"\\" + c_file.name, del)) != 0)
								return err;*/
							filelist.push_back(dirs[i] + L"\\" + c_file.name);
						}

					}while(_wfindnexti64(hFile, &c_file) == 0);
				}
				_findclose(hFile);

				for(int j=0;j<filelist.size();j++){
					if((err = _wipe(filelist[j], del)) != 0)
						return err;
				}
			}

			SetCurrentDirectory(filename.c_str());
			SetCurrentDirectory(L"..");
			
			if(del){
				for(int i=dirs.size()-1;i>=0;i--){
					//MessageBox(0, dirs[i].c_str(), L"", 0);
					if(RemoveDirectory(dirs[i].c_str()) == NULL)
						return GetLastError();
				}
			}
		}else{
			_wipe(filename, del);
		}

		//return 0;
	}
}