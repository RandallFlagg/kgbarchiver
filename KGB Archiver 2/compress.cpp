#pragma once

#include <vector>
#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <malloc.h>
#include <math.h>
#include <algorithm>
#include <time.h>
#include "home.cpp"
#include "aes/AES.h"

using namespace std;

#ifdef SFX
	#ifdef SFX_0
		#include "../paq0/paq0.cpp"
	#endif
	#ifdef SFX_1
		#include "../paq1/paq1.cpp"
	#endif
	#ifdef SFX_2
		#include "../paq2/paq2.cpp"
	#endif
	#ifdef SFX_3
		#include "../paq3/paq3.cpp"
	#endif
	#ifdef SFX_4
		#include "../paq4/paq4.cpp"
	#endif
	#ifdef SFX_5
		#include "../paq5/paq5.cpp"
	#endif
	#ifdef SFX_6
		#include "../paq6/paq6.cpp"
	#endif
	#ifdef SFX_7
		#include "../paq7/paq7.cpp"
	#endif
#endif /*KGB_SFX*/


#define ARCHIVE_VERSION 2

#define ALGO_NO_COMPRESS 0
#define ALGO_PAQ1 1
#define ALGO_PAQ2 2
#define ALGO_PAQ3 3
#define ALGO_PAQ4 4
#define ALGO_PAQ5 5
#define ALGO_PAQ6 6
#define ALGO_PAQ7 7
#define ALGO_PAQ8 8 //reserved
#define ALGO_PAQ9 9 //reserved

#define READ_BUFF_SIZE 8192
#define WRITE_BUFF_SIZE 8192 //should be >= READ_BUFF_SIZE

#define AES_BLOCK_SIZE 16 //bytes
#define AES_BLOCK_SIZE_POWER 4//power of 2

#define KGB_ERROR_BROKEN_ARCHIVE -1
#define KGB_ERROR_WRONG_PASSWORD -2
#define KGB_ERROR_SFX_NOT_FOUND -3

#define CHECKSUM_MOD 9223372036854780000 /*DO NOT CHANGE IT, NEVER*/

 void UnixTimeToFileTime(__time64_t t, LPFILETIME pft)
   {
     // Note that LONGLONG is a 64-bit value
     LONGLONG ll;

     ll = /*Int32x32To64(t, 10000000)*/t*10000000 + 116444736000000000;
     pft->dwLowDateTime = (DWORD)ll;
     pft->dwHighDateTime = ll >> 32;
   }

 struct ENCRYPTION{
	 bool enabled;
	 unsigned char pass[32];
	 unsigned __int64 checksum;

	 ENCRYPTION(){enabled=false;checksum=0;}
	 ENCRYPTION(bool e){enabled=e;checksum=0;}
	 
	 void setPassword(wchar_t *p){
		memset(pass, 0, sizeof(pass));
		wcstombs((char *)pass, p, sizeof(pass));
	 }
	 //~ENCRYPTION(){int s=0;for(int i=0;i<32;i++)s+=pass[i];printf("checksum:%d\n", s);}
 };

struct compressInfo{
	vector <_wfinddatai64_t> files;
	vector <unsigned __int64> checksums;
	wstring archiveName;
	char algorithm;
	char algoMode;
	ENCRYPTION encryption;
	bool sfx;
	int version;
	wstring destination;
	__int64 offset;
	vector <wstring> files2decompress;

	compressInfo(){algorithm = ALGO_PAQ6; algoMode = 3;sfx=0; version=ARCHIVE_VERSION;}
	compressInfo(wstring arch){ archiveName = arch; algorithm = ALGO_PAQ6; algoMode = 3;sfx=0;}
	compressInfo(wstring arch, char algo){archiveName = arch; algorithm = algo; algoMode = 3;sfx=0;}

	~compressInfo(){ files.clear(); checksums.clear(); files2decompress.clear(); }

	int insertFile(wstring filename){
		struct _wfinddatai64_t c_file;
		long hFile;

		if((hFile = _wfindfirsti64(filename.c_str(), &c_file)) == -1L){
			return GetLastError();
		}else{
			files.push_back(c_file);
		}

		return 0;
	}

	int insertDir(wstring dir){
		vector<wstring>dirs;
		struct _wfinddatai64_t c_file;
		long hFile;
		wstring tmp;

		/*if(SetCurrentDirectory(dir.c_str()) == 0)
			return GetLastError();*/

		dirs.push_back(dir);

		for(int i=0;i<dirs.size();i++){
			tmp = dirs[i] + L"\\*.*";

			if((hFile = _wfindfirsti64(tmp.c_str(), &c_file)) == -1L){
				continue;//return GetLastError();
			}else{
				
				do{
					if(/*(c_file.name[0] == L'.' && c_file.name[1] == L'.') || */c_file.name[0] == L'.'/* && c_file.name[1] != L'.'*/)
						continue;
					tmp = dirs[i] + L"\\" + c_file.name;
					wcscpy(c_file.name, tmp.c_str());

					if(c_file.attrib & _A_SUBDIR){
						dirs.push_back(c_file.name);
					}else{
						files.push_back(c_file);
					}

				}while(_wfindnexti64(hFile, &c_file) == 0);

			}

			_findclose(hFile);
		}
		return 0;
	}
};

struct _compressStatus{
	wstring filename;
	unsigned __int64 uncompressed_all;
	unsigned __int64 uncompressed_cur;
	unsigned __int64 compressed_all;
	//unsigned __int64 compressed_cur;
	unsigned __int64 done_all;
	unsigned __int64 done_cur;
	unsigned __int64 encrypted;
	time_t startTime;
	_compressStatus(){
		filename = L"";
		uncompressed_all = 1;//prevent from dividing by 0
		uncompressed_cur = 1;
		compressed_all = 0;
		//compressed_cur = 0;
		done_all = 0;
		done_cur = 0;
		encrypted = 0;}
	void init(compressInfo cmsi){
		_compressStatus();
		uncompressed_all = 0;
		for(int i=0;i<cmsi.files.size();i++){
			uncompressed_all += cmsi.files[i].size;
		}
		startTime = time(NULL);
	}
} compressStatus;

//_compressStatus compressStatus;

//#ifndef SFX
typedef void (*compressDLL)(FILE *, char *, unsigned __int64); 
typedef void (*setModeDLL)(char);
typedef void (*decompressDLL)(FILE *, char *, unsigned __int64);
//#endif

string getline(FILE* f=stdin) {
  int c;
  string result="";
  while ((c=getc(f))!=EOF && (c>=32 || c=='\t'))
    result+=char(c);
  if (c=='\r')
    (void) getc(f);
  return result;
}

#ifndef SFX

//stable InsertionSort
void sortFiles(vector<unsigned __int64>&key, vector<_wfinddatai64_t>&file, vector<unsigned __int64>&checksum, vector<unsigned __int64>&data){
	_wfinddatai64_t tFile;
	unsigned __int64 tKey, tData, tChecksum;
	int l=0,p,s;

	for(int i=1;i<key.size();++i){
		l = 0;
		p = i;
		while(l <= p){
			s = (l+p)/2;
			if(key[i] < key[s]){
				p = s-1;
			}else{
				l = s+1;
			}
		}
		tFile = file[i];
		tKey = key[i];
		tData = data[i];
		tChecksum = checksum[i];
		for(int j=i;j>s;--j){
			file[j] = file[j-1];
			key[j] = key[j-1];
			data[j] = data[j-1];
			checksum[j] = checksum[j-1];
		}
		file[s] = tFile;
		key[s] = tKey;
		data[s] = tData;
		checksum[s] = tChecksum;
	}

}

int MakeChecksumsAndSort(compressInfo &cmsi){
	vector <unsigned __int64> average;
	vector <unsigned __int64> sigma;

	unsigned __int64 _average = 0;
	unsigned __int64 _sigma = 0;

	unsigned __int64 __average = 0;
	unsigned __int64 __sigma = 0;

	FILE *f;

	unsigned char buffer[READ_BUFF_SIZE];
	int len;
	int cycles;

	unsigned __int64 checksum;

	for(__int64 i=0;i<cmsi.files.size();i++){
		//System::Windows::Forms::MessageBox::Show(gcnew System::String(cmsi.files[i].name));
		f = _wfopen(cmsi.files[i].name, L"rb");
		if(!f)
			return GetLastError();
		
		__average = __sigma = 0;
		checksum = 0;
		for(cycles=1;/*!feof(f)*/len = fread(buffer, sizeof(*buffer), READ_BUFF_SIZE, f);cycles++){
			
			/*if(len == 0)
				break;*/

			_average = _sigma = 0;
			for(int j=0;j<len;j++){
				_average += buffer[j];
				//checksum = (checksum + buffer[j])%CHECKSUM_MOD;
			}
			checksum = (checksum + _average)%CHECKSUM_MOD;
			_average /= len;
			for(int j=0;j<len;j++){
				_sigma += (_average-buffer[j])*(_average-buffer[j]);
			}
			_sigma = sqrt((double)_sigma/len);

			__average += _average;
			__sigma += _sigma;
		}
		average.push_back((__average/cycles)>>3);
		sigma.push_back(__sigma/cycles);

		fclose(f);

		cmsi.checksums.push_back(checksum);
		/*wchar_t xxx[32];
		wsprintf(xxx, L"%d\n", checksum);
		MessageBox(0, xxx, xxx, 0);*/
	}

	sortFiles(sigma, cmsi.files, cmsi.checksums, average);
	sortFiles(average, cmsi.files, cmsi.checksums, sigma);

	return 0;
}

DWORD WINAPI createArchive(LPVOID lpParam){

	compressInfo *_cmsi = (compressInfo*)lpParam;
	compressInfo cmsi = *_cmsi;

	wchar_t textBuffer[256];

	//MessageBox(0, cmsi.archiveName.c_str(), L"", 0);
	compressDLL compressFunc;
	setModeDLL setModeFunc;
	FILE *archive;
	char rbuffer[READ_BUFF_SIZE];
	char wbuffer[WRITE_BUFF_SIZE];
	__int64 size;
	unsigned __int64 headerOffset;
	unsigned __int64 archiveSize;

	if(MakeChecksumsAndSort(cmsi) != 0)
		return GetLastError();
	//System::Windows::Forms::MessageBox::Show("");

	wsprintf(textBuffer, L"%wspaq%d.dll", getHome().c_str(), cmsi.algorithm);
	HMODULE DLL = LoadLibrary(textBuffer);
	if(DLL == NULL)
		return GetLastError();

	compressFunc = (compressDLL)GetProcAddress(DLL, "kgbcompress");
	if(compressFunc == NULL)
		return GetLastError();

	setModeFunc = (setModeDLL)GetProcAddress(DLL, "setMode");
	if(setModeFunc == NULL)
		return GetLastError();

	setModeFunc(cmsi.algoMode);

	archive = _wfopen(cmsi.archiveName.c_str(), L"w+b");
	if(archive == NULL)
		return GetLastError();
	
	//SFX
	if(cmsi.sfx){
		/*FILE *sfx = _wfopen((getHome()+L"sfx_kgb.sfx").c_str(), L"rb");
		if(sfx == NULL){
			fclose(archive);
			FreeLibrary(DLL);
			return KGB_ERROR_SFX_NOT_FOUND;
		}
		while(!feof(sfx)){
			size = fread(rbuffer, sizeof(*rbuffer), READ_BUFF_SIZE, sfx);
			fwrite(rbuffer, sizeof(*rbuffer), size, archive);
		}
		fclose(sfx);
		if((size=_ftelli64(archive)%512) != 0){//write some garbage ;)
			size = 512-size;
			fwrite(rbuffer, sizeof(*rbuffer), size, archive);
		}


		fwrite("KGBSFXST", 1, 8, archive);*/
		
		wsprintf(textBuffer, L"%sstub%d.exe", getHome().c_str(), cmsi.algorithm);

		FILE *sfx = _wfopen(textBuffer, L"rb");
		if(sfx == NULL){
			fclose(archive);
			FreeLibrary(DLL);
			return KGB_ERROR_SFX_NOT_FOUND;
		}
		while(!feof(sfx)){
			size = fread(rbuffer, sizeof(*rbuffer), READ_BUFF_SIZE, sfx);
			fwrite(rbuffer, sizeof(*rbuffer), size, archive);
		}
		fclose(sfx);
		//fwrite("KGBSFXEN", 1, 8, archive);

		if((size=_ftelli64(archive)%512) != 0){//write some garbage ;)
			size = 512-size;
			fwrite(rbuffer, sizeof(*rbuffer), size, archive);
		}

		/*fwrite("KGBSFXDL", 1, 8, archive);
		
		wsprintf(textBuffer, L"%wssfx\\paq%d.dll", getHome().c_str(), cmsi.algorithm);
		sfx = _wfopen(textBuffer, L"rb");
		if(sfx == NULL){
			fclose(archive);
			FreeLibrary(DLL);
			return KGB_ERROR_SFX_NOT_FOUND;
		}
		while(!feof(sfx)){
			size = fread(rbuffer, sizeof(*rbuffer), READ_BUFF_SIZE, sfx);
			fwrite(rbuffer, sizeof(*rbuffer), size, archive);
		}
		fclose(sfx);
		fwrite("KGBSFXEN", 1, 8, archive);

		if((size=_ftelli64(archive)%512) != 0){//write some garbage ;)
			size = 512-size;
			fwrite(rbuffer, sizeof(*rbuffer), size, archive);
		}*/
	}

	compressStatus.init(cmsi);

	setvbuf(archive, wbuffer, _IOFBF, WRITE_BUFF_SIZE);

	//write header of archive
	fwprintf(archive, L"KGB%d", ARCHIVE_VERSION);
	fwrite(&cmsi.encryption.enabled, sizeof(cmsi.encryption.enabled), 1, archive);
	fwrite(&cmsi.algorithm, sizeof(cmsi.algorithm), 1, archive);
	fwrite(&cmsi.algoMode, sizeof(cmsi.algoMode), 1, archive);
	size_t tmpSize;
	unsigned long count = cmsi.files.size();
	fwrite(&count, sizeof(count), 1, archive);
	for(unsigned __int64 i=0;i<cmsi.files.size();i++){
		tmpSize = sizeof(cmsi.files[i]);
		fwrite(&tmpSize, sizeof(tmpSize), 1, archive);
		fwrite(&cmsi.files[i], tmpSize, 1, archive);
		fwrite(&cmsi.checksums[i], sizeof(cmsi.checksums[i]), 1, archive);
	}

	if(cmsi.encryption.enabled)
		fwrite(&cmsi.encryption.checksum, sizeof(cmsi.encryption.checksum), 1, archive);

	//printf(" --> %d <-- \n", ftell(archive));
	headerOffset = _ftelli64(archive);

	for(unsigned __int64 i=0;i<cmsi.files.size();i++){
		compressStatus.uncompressed_cur = cmsi.files[i].size;
		compressStatus.done_cur = 0;
		compressStatus.filename = cmsi.files[i].name;
		FILE *fin = _wfopen(cmsi.files[i].name, L"rb");
		if(fin == NULL)
			continue;

		while(!feof(fin)){
			size = fread(rbuffer, sizeof(*rbuffer), READ_BUFF_SIZE, fin);

			compressFunc(archive, rbuffer, size);
			compressStatus.done_cur += size;
			compressStatus.done_all += size;
			compressStatus.compressed_all = _ftelli64(archive);
		}
		//compressFunc(archive, "\0", 1);

		fclose(fin);
	}

	if(FreeLibrary(DLL) == 0)
		return GetLastError();

	if(cmsi.encryption.enabled){
		fflush(archive);
		archiveSize = _ftelli64(archive);
		setvbuf(archive, NULL, _IONBF, NULL);

		compressStatus.filename = L"\1";

		AES aes;
		aes.SetParameters(256); //convert to bits
		aes.StartEncryption(cmsi.encryption.pass);

		_fseeki64(archive, headerOffset, SEEK_SET);

		while(_ftelli64(archive) < archiveSize){
			size = fread(rbuffer, sizeof(*rbuffer), READ_BUFF_SIZE, archive);

			aes.Encrypt((unsigned char*)rbuffer, (unsigned char*)wbuffer, (size%AES_BLOCK_SIZE==0 ? (size>>AES_BLOCK_SIZE_POWER) : (size>>AES_BLOCK_SIZE_POWER)+1), AES::BlockMode::ECB);
			//printf("size: %lld blocksize: %d blocks: %lld\n", size, AES_BLOCK_SIZE, (size%AES_BLOCK_SIZE==0 ? size/AES_BLOCK_SIZE : size/AES_BLOCK_SIZE+1));
			_fseeki64(archive, -size, SEEK_CUR);
			fwrite(wbuffer, sizeof(*wbuffer), (size%AES_BLOCK_SIZE==0 ? size/AES_BLOCK_SIZE : size/AES_BLOCK_SIZE+1)*AES_BLOCK_SIZE, archive);

			//computing checksum
			for(unsigned __int64 j=0;j<(size%AES_BLOCK_SIZE==0 ? (size>>AES_BLOCK_SIZE_POWER) : (size>>AES_BLOCK_SIZE_POWER)+1)*AES_BLOCK_SIZE;j++)
				cmsi.encryption.checksum = (cmsi.encryption.checksum+(unsigned char)rbuffer[j])%CHECKSUM_MOD;

			compressStatus.encrypted = _ftelli64(archive);
		}
		_fseeki64(archive, headerOffset-sizeof(cmsi.encryption.checksum), SEEK_SET);
		fwrite(&cmsi.encryption.checksum, sizeof(cmsi.encryption.checksum), 1, archive);
		//printf("checksum: %llu\n", cmsi.encryption.checksum);
	}

	fclose(archive);

	return 0;
}

#endif //SFX

void readHeader(FILE *archive, compressInfo *cmsi, int version=ARCHIVE_VERSION){
	cmsi->version = version;
	if(version == ARCHIVE_VERSION){
		unsigned long count;
		size_t tmpSize;
		_wfinddatai64_t *fh;
		unsigned __int64 checksum;

		fread(&cmsi->encryption.enabled, sizeof(cmsi->encryption.enabled), 1, archive);
		fread(&cmsi->algorithm, sizeof(cmsi->algorithm), 1, archive);
		fread(&cmsi->algoMode, sizeof(cmsi->algoMode), 1, archive);

		fread(&count, sizeof(count), 1, archive);

		cmsi->files.clear();
		cmsi->checksums.clear();
		for(unsigned long i=0;i<count;i++){
			fread(&tmpSize, sizeof(tmpSize), 1, archive);
			fh = (_wfinddatai64_t*)malloc(tmpSize);
			/*wchar_t xx[30];
			_swprintf(xx, L"%d", tmpSize);
			MessageBox(0, xx, L"", 0);*/
			if(fh == NULL){
				SetLastError(ERROR_OUTOFMEMORY);
				return;
			}
			fread(fh, tmpSize, 1, archive);
			cmsi->files.push_back(*fh);
			free(fh);
			fread(&checksum, sizeof(checksum), 1, archive);
			cmsi->checksums.push_back(checksum);
		}

		if(cmsi->encryption.enabled)
			fread(&cmsi->encryption.checksum, sizeof(cmsi->encryption.checksum), 1, archive);//cmsi->encryption.checksum = 0;

		cmsi->offset = _ftelli64(archive);
	}else if(version == 1){
		cmsi->algorithm = ALGO_PAQ6;

		_wfinddatai64_t fh;

		_fseeki64(archive, 0, SEEK_SET);

		/*from kgb archiver 1.2*/
		string s=getline(archive);
        if (s.substr(0, string("KGB_arch").size()) != "KGB_arch") {
           //printf("Archiwum %s nie jest w formacie " PROGNAME "\n", argv[1]);
           return;
        }
    
        // Get option -m where m is a digit
        if (s.size()>2 && s[s.size()-2]=='-') {
           int c=s[s.size()-1];
           if (c>='0' && c<='9')
               cmsi->algoMode=c-'0';
        }
        
        // Read "size filename" in "%d\t%s\r\n" format
		cmsi->files.clear();       
		while (true) {
              string s=getline(archive);
			  __int64 tmpSize;
			  sscanf(s.c_str(), "%lld", &tmpSize);
              if (s.size()>1) {
				  fh.size = tmpSize;//filesize.push_back(/*atol(s.c_str())*/tmpSize);
                 
				  string::iterator tab=find(s.begin(), s.end(), '\t');
				  if (tab!=s.end()){
					 mbstowcs(fh.name, string(tab+1, s.end()).c_str(), sizeof(fh.name)/sizeof(*fh.name));//filename.push_back(string(tab+1, s.end()));
                 /*else
                    filename.push_back("");*/
					cmsi->files.push_back(fh);
				  }
              }
              else
                  break;
        }
        
        // Test end of header for "\f\0"
        {
           int c1=0, c2=0;
           if ((c1=getc(archive))!='\f' || (c2=getc(archive))!=0) {
              //printf("%s: Zly format naglowka pliku %d %d\n", argv[1],
              //c1, c2)
              return;
           }
        }
		cmsi->offset = _ftelli64(archive);
	}
}

int readHeader(wstring filename, compressInfo *cmsi){
	wchar_t textBuffer[256];
	char	*charBuffer;
	
	//compressInfo cmsi;
	FILE *archive = _wfopen(filename.c_str(), L"rb");
	if(archive == NULL)
		return GetLastError();

	cmsi->archiveName = filename;

	fread(textBuffer, sizeof(*textBuffer), 4, archive);
	charBuffer = (char*)textBuffer;

	if(textBuffer[0] == L'K' && textBuffer[1] == L'G' && textBuffer[2] == L'B' && textBuffer[3] == (L'0'+ARCHIVE_VERSION)){
		readHeader(archive, cmsi, 2);
		fclose(archive);
		if(GetLastError() == ERROR_OUTOFMEMORY)
			return GetLastError();
		return NULL;
	}else if(charBuffer[0] == 'K' && charBuffer[1] == 'G' && charBuffer[2] == 'B' && charBuffer[3] == '_'){
		readHeader(archive, cmsi, 1);
		fclose(archive);
		return NULL;
	}else if(charBuffer[0] == 0x4D && charBuffer[1] == 0x5A){ //sfx archive
		_fseeki64(archive, 0, SEEK_SET);
		for(unsigned __int64 i=1;fread(textBuffer, sizeof(*textBuffer), sizeof(textBuffer)/sizeof(*textBuffer), archive) && i <= 500 /*look for header only in first 200KB*/;i++){
			
			if(textBuffer[0] == L'K' && textBuffer[1] == L'G' && textBuffer[2] == L'B' && textBuffer[3] == (L'0'+ARCHIVE_VERSION)){
				cmsi->sfx = true;
				
				_fseeki64(archive, -504/*-sizeof(*textBuffer)*252*/, SEEK_CUR);
				
				readHeader(archive, cmsi, 2);
				fclose(archive);
				if(GetLastError() == ERROR_OUTOFMEMORY)
					return GetLastError();
				return NULL;
			}
		}
		return KGB_ERROR_BROKEN_ARCHIVE;
	}/*else
		SetLastError(KGB_ERROR_BROKEN_ARCHIVE);//wprintf(L"Broken archive!");*/

	fclose(archive);
	return KGB_ERROR_BROKEN_ARCHIVE;
}

int makeDirs(compressInfo &cmsi){
	wchar_t tmp[1024];
	int err;
	for(unsigned __int64 i=0;i<cmsi.files.size();i++){
		//tmp = cmsi.files[i].name;
		if(wcslen(cmsi.files[i].name) < 2)
			continue;
		for(int j=(cmsi.files[i].name[1] == L'\\'?3:0);j<wcslen(cmsi.files[i].name);j++){
			if(cmsi.files[i].name[j] == L'\\' || cmsi.files[i].name[j] == L'/'){
				wcscpy_s(tmp, sizeof(tmp)/sizeof(*tmp), cmsi.files[i].name);
				tmp[j] = L'\0';
				//MessageBox(0, tmp, L"", 0);
				err = NULL;
				if(CreateDirectory(tmp, NULL) == NULL && (err=GetLastError()) != ERROR_ALREADY_EXISTS)
					return err;
			}
		}
	}
	return 0;
}
//#define _kgb2compressor

#ifdef _kgb2compressor

#include "overwrite.h"
#include "KGB Archiver 2 .net/units.cpp"

#define OVERWRITE_ALL 3
#define OVERWRITE_YES 2
#define OVERWRITE_NO 1
#define OVERWRITE_NEVER 0

int overwriteMode;

using namespace kgb2compressor;

bool overwriteFile(_wfinddatai64_t *file){
	if(overwriteMode == OVERWRITE_ALL || overwriteMode == OVERWRITE_NEVER)
		return overwriteMode;
	
	Windows::Forms::DialogResult tmp;
	overwrite ^fOverwrite = gcnew overwrite();
	IO::FileInfo ^fi = gcnew IO::FileInfo(gcnew String(file->name));
	SHFILEINFO shfi;
	memset(&shfi,0,sizeof(shfi));
	SHGetFileInfo(file->name, FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(shfi), SHGFI_ICON|SHGFI_USEFILEATTRIBUTES|/*SHGFI_SHELLICONSIZE|*/SHGFI_LARGEICON/*|SHGFI_TYPENAME*/);
	

	fOverwrite->gExisting->Text += IO::Path::GetFileName(gcnew System::String(file->name));
	fOverwrite->lExistSize->Text = gcnew String(convertUnits(fi->Length).c_str());
	fOverwrite->lExistModified->Text = fi->LastWriteTime.ToShortDateString() + L" " + fi->LastWriteTime.ToShortTimeString();
	if(shfi.hIcon != NULL)
		fOverwrite->pExisting->Image = System::Drawing::Icon::FromHandle(IntPtr(shfi.hIcon))->ToBitmap();

	fOverwrite->lNewSize->Text = gcnew String(convertUnits(file->size).c_str());
	fOverwrite->lNewModified->Text = System::DateTime(1970, 1, 1).AddSeconds(file->time_write).ToShortDateString() + L" " + System::DateTime(1970, 1, 1).AddSeconds(file->time_write).ToShortTimeString();
	fOverwrite->pNew->Image = fOverwrite->pExisting->Image;

	tmp = fOverwrite->ShowDialog();

	delete fOverwrite;
		
	switch(tmp){
		case System::Windows::Forms::DialogResult::OK: overwriteMode = OVERWRITE_ALL;
		case System::Windows::Forms::DialogResult::Yes: return true; break;
		case System::Windows::Forms::DialogResult::Cancel: overwriteMode = OVERWRITE_NEVER;
		case System::Windows::Forms::DialogResult::No: return false; break;
	}

	return true;
}

#endif //_kgb2compressor

DWORD WINAPI extractArchive(LPVOID lpParam){

	/*needs at least archive name and password*/
	compressInfo *_cmsi = (compressInfo*)lpParam;
	compressInfo cmsi = *_cmsi;
	
	wchar_t textBuffer[256];
	char	*charBuffer;

	char rbuffer[READ_BUFF_SIZE];
	char wbuffer[WRITE_BUFF_SIZE];
	HMODULE DLL;
	decompressDLL decompressFunc;
	setModeDLL setModeFunc;
	__int64 tmpBuffSize, tmpBuffSize2;
	unsigned __int64 checksum;
	
//MessageBox(0, L"1", L"", 0);

	readHeader(cmsi.archiveName, &cmsi);
	compressStatus.init(cmsi);

	FILE *archive = _wfopen(cmsi.archiveName.c_str(), L"rb");
	if(archive == NULL){
		return GetLastError();
	}


	_fseeki64(archive, cmsi.offset, SEEK_SET);

	/*fread(textBuffer, sizeof(*textBuffer), 4, archive);
	charBuffer = (char*)textBuffer;*/

	int mkdirerr;
	if((mkdirerr=makeDirs(cmsi)) != NULL)
		return mkdirerr;

	if(cmsi.version == 1/*charBuffer[0] == 'K' && charBuffer[1] == 'G' && charBuffer[2] == 'B' && charBuffer[3] == '_'*/){
		//old archive format

		/*readHeader(archive, &cmsi, 1);
		compressStatus.init(cmsi);*/

		//for(int i=0;i<cmsi.files.size();i++)
		//	printf("\n ->> %s: %lld\n", cmsi.files[i].name, cmsi.files[i].size);

		wsprintf(textBuffer, L"%wspaq6.dll", getHome().c_str());
		DLL = LoadLibrary(textBuffer);

		if(DLL == NULL){
			fclose(archive);
			return GetLastError();
		}


		decompressFunc = (decompressDLL)GetProcAddress(DLL, "kgbdecompress");
		if(decompressFunc == NULL){
			fclose(archive);
			return GetLastError();
		}

		setModeFunc = (setModeDLL)GetProcAddress(DLL, "setMode");
		if(setModeFunc == NULL){
			fclose(archive);
			return GetLastError();
		}
		
		setModeFunc(cmsi.algoMode);

		

		//printf("%d\n", ftell(archive));
		//_fseeki64(archive, -1, SEEK_CUR);

		for(unsigned __int64 i=0;i<cmsi.files.size();i++){
			compressStatus.done_cur = 0;
			compressStatus.uncompressed_cur = cmsi.files[i].size;
			compressStatus.filename = cmsi.files[i].name;

#ifdef _kgb2compressor
			bool ow = true;
			
			if(_waccess((const wchar_t*)cmsi.files[i].name, 00) == NULL){
				ow = overwriteFile(&cmsi.files[i]);
			}
			if(ow){
				FILE *fout = _wfopen(cmsi.files[i].name, L"wb");
				if(!fout)
					compressStatus.filename = L"\3";
				else
					setvbuf(fout, wbuffer, _IOFBF, WRITE_BUFF_SIZE);		

				for(tmpBuffSize = 0;tmpBuffSize < cmsi.files[i].size;tmpBuffSize += READ_BUFF_SIZE){
					tmpBuffSize2=(cmsi.files[i].size-tmpBuffSize > READ_BUFF_SIZE ? READ_BUFF_SIZE : cmsi.files[i].size-tmpBuffSize);
					//printf("%lld %d %lld %lld : %lld ", i, tmpBuffSize < cmsi.files[i].size, tmpBuffSize, cmsi.files[i].size,  tmpBuffSize2);
					decompressFunc(archive, rbuffer, tmpBuffSize2);
					if(fout)
						fwrite(rbuffer, sizeof(*rbuffer), tmpBuffSize2, fout);
					compressStatus.done_cur += tmpBuffSize2;
					compressStatus.done_all += tmpBuffSize2;
				}

				fclose(fout);
			}else{
				compressStatus.filename = L"Skipping...";
				for(tmpBuffSize = 0;tmpBuffSize < cmsi.files[i].size;tmpBuffSize += READ_BUFF_SIZE){
					tmpBuffSize2=(cmsi.files[i].size-tmpBuffSize > READ_BUFF_SIZE ? READ_BUFF_SIZE : cmsi.files[i].size-tmpBuffSize);
					//printf("%lld %d %lld %lld : %lld ", i, tmpBuffSize < cmsi.files[i].size, tmpBuffSize, cmsi.files[i].size,  tmpBuffSize2);
					decompressFunc(archive, rbuffer, tmpBuffSize2);
					//fwrite(rbuffer, sizeof(*rbuffer), tmpBuffSize2, fout);
					compressStatus.done_cur += tmpBuffSize2;
					compressStatus.done_all += tmpBuffSize2;
				}
			}
#else
			FILE *fout = _wfopen(cmsi.files[i].name, L"wb");
			if(!fout)
				compressStatus.filename = L"\3";
			setvbuf(fout, wbuffer, _IOFBF, WRITE_BUFF_SIZE);		

			for(tmpBuffSize = 0;tmpBuffSize < cmsi.files[i].size;tmpBuffSize += READ_BUFF_SIZE){
				tmpBuffSize2=(cmsi.files[i].size-tmpBuffSize > READ_BUFF_SIZE ? READ_BUFF_SIZE : cmsi.files[i].size-tmpBuffSize);
				//printf("%lld %d %lld %lld : %lld ", i, tmpBuffSize < cmsi.files[i].size, tmpBuffSize, cmsi.files[i].size,  tmpBuffSize2);

				decompressFunc(archive, rbuffer, tmpBuffSize2);

				fwrite(rbuffer, sizeof(*rbuffer), tmpBuffSize2, fout);
				compressStatus.done_cur += tmpBuffSize2;
				compressStatus.done_all += tmpBuffSize2;
			}

			fclose(fout);
#endif //_kgb2compressor
		}
		

		if(FreeLibrary(DLL) == 0)
			return GetLastError();

	}else if(cmsi.version == 2/*textBuffer[0] == L'K' && textBuffer[1] == L'G' && textBuffer[2] == L'B' && textBuffer[3] == (L'0'+ARCHIVE_VERSION)*/){
		//new format
		size_t tmpSize;
		HANDLE hTimestamp;
		FILETIME accTime;
		FILETIME modTime;
		FILETIME creTime;

		//readHeader(archive, &cmsi);

		/*for(int i=0;i<cmsi.files.size();i++){
			wprintf(L"  -> %s\n", cmsi.files[i].name);
		}*/

		//compressStatus.init(cmsi);
#ifndef SFX
		wsprintf(textBuffer, L"%wspaq%d.dll", getHome().c_str(), cmsi.algorithm);
#else
		wsprintf(textBuffer, L"%wsdecompressor.dll", getHome().c_str());
#endif

#ifndef SFX
		DLL = LoadLibrary(textBuffer);
		if(DLL == NULL){
			fclose(archive);
			//wprintf(L"->> %s error: %d <<-\n", textBuffer, GetLastError());
			return GetLastError();
		}

		decompressFunc = (decompressDLL)GetProcAddress(DLL, "kgbdecompress");
		if(decompressFunc == NULL){
			fclose(archive);
			return GetLastError();
		}

		setModeFunc = (setModeDLL)GetProcAddress(DLL, "setMode");
		if(setModeFunc == NULL){
			fclose(archive);
			return GetLastError();
		}
#else
		decompressFunc = kgbdecompress;
		setModeFunc = setMode;
#endif

		setModeFunc(cmsi.algoMode);

		{
			__int64 _curPos = _ftelli64(archive);
			_fseeki64(archive, 0LL, SEEK_END);
			compressStatus.compressed_all = (unsigned __int64)_ftelli64(archive);
			_fseeki64(archive, _curPos, SEEK_SET);
		}

		//decryption
		if(cmsi.encryption.enabled){
			compressStatus.filename = L"\2";
			//setvbuf(archive, NULL, _IONBF, NULL);

			//fread(&cmsi.encryption.checksum, sizeof(cmsi.encryption.checksum), 1, archive);

			unsigned __int64 checksum = 0;
			AES aes;
			aes.SetParameters(256);
			aes.StartDecryption(cmsi.encryption.pass);

			GetTempPath(sizeof(textBuffer)/sizeof(*textBuffer), textBuffer);
			GetTempFileName(textBuffer, L"", NULL, textBuffer);
			FILE *tmpFile = _wfopen(textBuffer, L"w+bTD"); //T - tempfile, D - delete after close
			if(tmpFile == NULL)
				return GetLastError();

			while(!feof(archive)){
				tmpSize = fread(rbuffer, sizeof(*rbuffer), READ_BUFF_SIZE, archive);
				
				aes.Decrypt((unsigned char*)rbuffer, (unsigned char*)wbuffer, (tmpSize>>AES_BLOCK_SIZE_POWER), AES::BlockMode::ECB);//tmpSize always is dividable by AES_BLOCK_SIZE

				fwrite(wbuffer, sizeof(*wbuffer), tmpSize, tmpFile);
				
				//computing checksum
				for(unsigned __int64 j=0;j<tmpSize;j++)
					checksum = (checksum+(unsigned char)wbuffer[j])%CHECKSUM_MOD;

				compressStatus.encrypted = _ftelli64(archive);
			}

			//fclose(tmpFile);
			fclose(archive);
			archive = tmpFile;
			//printf("checksum: %llu %llu\n", checksum, cmsi.encryption.checksum);
			if(checksum != cmsi.encryption.checksum){
				fclose(archive);
				//printf("wrong password!\n");
				return KGB_ERROR_WRONG_PASSWORD;
			}
			_fseeki64(archive, 0LL, SEEK_SET);
			
		}

		//MessageBox(0, L"", L"", 0);

		for(unsigned __int64 i=0;i<cmsi.files.size();i++){
			compressStatus.done_cur = 0;
			compressStatus.uncompressed_cur = cmsi.files[i].size;
			compressStatus.filename = cmsi.files[i].name;
			
			checksum = 0;
			bool isOpen = true;
			bool onList = true;

			if(cmsi.files2decompress.size() != 0){
				onList = false;
				for(unsigned __int64 j=0;j<cmsi.files2decompress.size();++j){
					if(cmsi.files2decompress[j] == (wstring)cmsi.files[i].name){
						onList = true;
						break;
					}
				}
			}

#ifdef _kgb2compressor
			bool ow = true;

			if(onList && _waccess((const wchar_t*)cmsi.files[i].name, 00) == NULL){
				ow = overwriteFile(&cmsi.files[i]);
			}
			if(onList && ow){
				FILE *fout = _wfopen(cmsi.files[i].name, L"wb");
				if(!fout){
					compressStatus.filename = L"\3";
					isOpen = false;
				}
				else
					setvbuf(fout, wbuffer, _IOFBF, WRITE_BUFF_SIZE);		
				
				for(tmpBuffSize = 0;tmpBuffSize < cmsi.files[i].size;tmpBuffSize += READ_BUFF_SIZE){
					tmpBuffSize2=(cmsi.files[i].size-tmpBuffSize > READ_BUFF_SIZE ? READ_BUFF_SIZE : cmsi.files[i].size-tmpBuffSize);
					
					decompressFunc(archive, rbuffer, tmpBuffSize2);
					
					for(int j=0;j<tmpBuffSize2;j++)
						checksum = (checksum + (unsigned char)rbuffer[j])%CHECKSUM_MOD;
					if(isOpen)
						fwrite(rbuffer, sizeof(*rbuffer), tmpBuffSize2, fout);

					compressStatus.done_cur += tmpBuffSize2;
					compressStatus.done_all += tmpBuffSize2;
				}
				

				if(isOpen){
					fclose(fout);
			
					SetFileAttributes(cmsi.files[i].name, cmsi.files[i].attrib);
					hTimestamp = CreateFile(cmsi.files[i].name, FILE_WRITE_ATTRIBUTES, NULL, NULL, OPEN_EXISTING, cmsi.files[i].attrib, NULL);
					UnixTimeToFileTime(cmsi.files[i].time_access, &accTime);
					UnixTimeToFileTime(cmsi.files[i].time_create, &creTime);
					UnixTimeToFileTime(cmsi.files[i].time_write, &modTime);
					SetFileTime(hTimestamp, &creTime, &accTime, &modTime);
					CloseHandle(hTimestamp);
				}
			}else{
				compressStatus.filename = L"Skipping...";
				for(tmpBuffSize = 0;tmpBuffSize < cmsi.files[i].size;tmpBuffSize += READ_BUFF_SIZE){
					tmpBuffSize2=(cmsi.files[i].size-tmpBuffSize > READ_BUFF_SIZE ? READ_BUFF_SIZE : cmsi.files[i].size-tmpBuffSize);
					//printf("%lld %d %lld %lld : %lld ", i, tmpBuffSize < cmsi.files[i].size, tmpBuffSize, cmsi.files[i].size,  tmpBuffSize2);
					decompressFunc(archive, rbuffer, tmpBuffSize2);
					for(int j=0;j<tmpBuffSize2;j++)
						checksum = (checksum + (unsigned char)rbuffer[j])%CHECKSUM_MOD;
					//fwrite(rbuffer, sizeof(*rbuffer), tmpBuffSize2, fout);
					compressStatus.done_cur += tmpBuffSize2;
					compressStatus.done_all += tmpBuffSize2;
				}
			}
#else
			FILE *fout;
			if(onList)
				fout = _wfopen(cmsi.files[i].name, L"wb");
			
			if(!fout){
				compressStatus.filename = L"\3";
				isOpen = false;
			}
			setvbuf(fout, wbuffer, _IOFBF, WRITE_BUFF_SIZE);		
/*#ifdef SFX
			MessageBox(0, compressStatus.filename.c_str(), L"", 0);
#endif*/
			for(tmpBuffSize = 0;tmpBuffSize < cmsi.files[i].size;tmpBuffSize += READ_BUFF_SIZE){
				tmpBuffSize2=(cmsi.files[i].size-tmpBuffSize > READ_BUFF_SIZE ? READ_BUFF_SIZE : cmsi.files[i].size-tmpBuffSize);
				//printf("%lld %d %lld %lld : %lld ", i, tmpBuffSize < cmsi.files[i].size, tmpBuffSize, cmsi.files[i].size,  tmpBuffSize2);
				decompressFunc(archive, rbuffer, tmpBuffSize2);
				for(int j=0;j<tmpBuffSize2;j++)
					checksum = (checksum + (unsigned char)rbuffer[j])%CHECKSUM_MOD;
				if(isOpen)
					fwrite(rbuffer, sizeof(*rbuffer), tmpBuffSize2, fout);
				compressStatus.done_cur += tmpBuffSize2;
				compressStatus.done_all += tmpBuffSize2;
			}

			if(isOpen){
				fclose(fout);
				
				SetFileAttributes(cmsi.files[i].name, cmsi.files[i].attrib);
				hTimestamp = CreateFile(cmsi.files[i].name, FILE_WRITE_ATTRIBUTES, NULL, NULL, OPEN_EXISTING, cmsi.files[i].attrib, NULL);
				UnixTimeToFileTime(cmsi.files[i].time_access, &accTime);
				UnixTimeToFileTime(cmsi.files[i].time_create, &creTime);
				UnixTimeToFileTime(cmsi.files[i].time_write, &modTime);
				SetFileTime(hTimestamp, &creTime, &accTime, &modTime);
				CloseHandle(hTimestamp);
			}
#endif //_kgb2compressor
			//MessageBox::Show(String::Format("{0}", checksum));
			if(checksum != cmsi.checksums[i]){
				FreeLibrary(DLL);
				fclose(archive);
				DeleteFile(cmsi.files[i].name);
				return KGB_ERROR_BROKEN_ARCHIVE;
			}
		}
#ifndef SFX
		if(FreeLibrary(DLL) == 0)
			return GetLastError();
#endif		

	}else{
		fclose(archive);
		return KGB_ERROR_BROKEN_ARCHIVE;
	}
	
	fclose(archive);
	return 0;
}
#ifndef SFX
DWORD WINAPI testArchive(LPVOID lpParam){

	/*needs at least archive name and password*/
	compressInfo *_cmsi = (compressInfo*)lpParam;
	compressInfo cmsi = *_cmsi;
	
	wchar_t textBuffer[256];
	char	*charBuffer;

	char rbuffer[READ_BUFF_SIZE];
	char wbuffer[WRITE_BUFF_SIZE];
	HMODULE DLL;
	decompressDLL decompressFunc;
	setModeDLL setModeFunc;
	__int64 tmpBuffSize, tmpBuffSize2;
	unsigned __int64 checksum;
	
//MessageBox(0, L"1", L"", 0);

	readHeader(cmsi.archiveName, &cmsi);
	compressStatus.init(cmsi);

	FILE *archive = _wfopen(cmsi.archiveName.c_str(), L"rb");
	if(archive == NULL)
		return GetLastError();
	//MessageBox::Show(String::Format("{0}", cmsi.offset));
	_fseeki64(archive, cmsi.offset, SEEK_SET);

	/*fread(textBuffer, sizeof(*textBuffer), 4, archive);
	charBuffer = (char*)textBuffer;*/

	if(cmsi.version == 1/*charBuffer[0] == 'K' && charBuffer[1] == 'G' && charBuffer[2] == 'B' && charBuffer[3] == '_'*/){
		//old archive format

		/*readHeader(archive, &cmsi, 1);
		compressStatus.init(cmsi);*/

		//for(int i=0;i<cmsi.files.size();i++)
		//	printf("\n ->> %s: %lld\n", cmsi.files[i].name, cmsi.files[i].size);

		wsprintf(textBuffer, L"%wspaq6.dll", getHome().c_str());
		DLL = LoadLibrary(textBuffer);

		if(DLL == NULL){
			fclose(archive);
			return GetLastError();
		}

		decompressFunc = (decompressDLL)GetProcAddress(DLL, "kgbdecompress");
		if(decompressFunc == NULL){
			fclose(archive);
			return GetLastError();
		}

		setModeFunc = (setModeDLL)GetProcAddress(DLL, "setMode");
		if(setModeFunc == NULL){
			fclose(archive);
			return GetLastError();
		}
		
		setModeFunc(cmsi.algoMode);

		//printf("%d\n", ftell(archive));
		//_fseeki64(archive, -1, SEEK_CUR);

		for(unsigned __int64 i=0;i<cmsi.files.size();i++){
			compressStatus.done_cur = 0;
			compressStatus.uncompressed_cur = cmsi.files[i].size;
			compressStatus.filename = cmsi.files[i].name;

			FILE *fout = _wfopen(cmsi.files[i].name, L"wb");
			setvbuf(fout, wbuffer, _IOFBF, WRITE_BUFF_SIZE);		

			for(tmpBuffSize = 0;tmpBuffSize < cmsi.files[i].size;tmpBuffSize += READ_BUFF_SIZE){
				tmpBuffSize2=(cmsi.files[i].size-tmpBuffSize > READ_BUFF_SIZE ? READ_BUFF_SIZE : cmsi.files[i].size-tmpBuffSize);
				//printf("%lld %d %lld %lld : %lld ", i, tmpBuffSize < cmsi.files[i].size, tmpBuffSize, cmsi.files[i].size,  tmpBuffSize2);
				decompressFunc(archive, rbuffer, tmpBuffSize2);
				fwrite(rbuffer, sizeof(*rbuffer), tmpBuffSize2, fout);
				compressStatus.done_cur += tmpBuffSize2;
				compressStatus.done_all += tmpBuffSize2;
			}

			fclose(fout);
		}
		

		if(FreeLibrary(DLL) == 0)
			return GetLastError();

	}else if(cmsi.version == 2/*textBuffer[0] == L'K' && textBuffer[1] == L'G' && textBuffer[2] == L'B' && textBuffer[3] == (L'0'+ARCHIVE_VERSION)*/){
		//new format

		size_t tmpSize;
		HANDLE hTimestamp;
		FILETIME accTime;
		FILETIME modTime;
		FILETIME creTime;

		//readHeader(archive, &cmsi);

		/*for(int i=0;i<cmsi.files.size();i++){
			wprintf(L"  -> %s\n", cmsi.files[i].name);
		}*/

		//compressStatus.init(cmsi);

		wsprintf(textBuffer, L"%wspaq%d.dll", getHome().c_str(), cmsi.algorithm);
		DLL = LoadLibrary(textBuffer);
		if(DLL == NULL){
			fclose(archive);
			//wprintf(L"->> %s error: %d <<-\n", textBuffer, GetLastError());
			return GetLastError();
		}

		decompressFunc = (decompressDLL)GetProcAddress(DLL, "kgbdecompress");
		if(decompressFunc == NULL){
			fclose(archive);
			return GetLastError();
		}


		setModeFunc = (setModeDLL)GetProcAddress(DLL, "setMode");
		if(setModeFunc == NULL){
			fclose(archive);
			return GetLastError();
		}
	
		setModeFunc(cmsi.algoMode);

		{
			__int64 _curPos = _ftelli64(archive);
			_fseeki64(archive, 0LL, SEEK_END);
			compressStatus.compressed_all = (unsigned __int64)_ftelli64(archive);
			_fseeki64(archive, _curPos, SEEK_SET);
		}

		//decryption
		if(cmsi.encryption.enabled){
			compressStatus.filename = L"\2";
			//setvbuf(archive, NULL, _IONBF, NULL);

			//fread(&cmsi.encryption.checksum, sizeof(cmsi.encryption.checksum), 1, archive);

			unsigned __int64 checksum = 0;
			AES aes;
			aes.SetParameters(256);
			aes.StartDecryption(cmsi.encryption.pass);

			GetTempPath(sizeof(textBuffer)/sizeof(*textBuffer), textBuffer);
			GetTempFileName(textBuffer, L"", NULL, textBuffer);
			FILE *tmpFile = _wfopen(textBuffer, L"w+bTD"); //T - tempfile, D - delete after close
			if(tmpFile == NULL)
				return GetLastError();

			while(!feof(archive)){
				tmpSize = fread(rbuffer, sizeof(*rbuffer), READ_BUFF_SIZE, archive);
				
				aes.Decrypt((unsigned char*)rbuffer, (unsigned char*)wbuffer, (tmpSize>>AES_BLOCK_SIZE_POWER), AES::BlockMode::ECB);//tmpSize always is dividable by AES_BLOCK_SIZE

				fwrite(wbuffer, sizeof(*wbuffer), tmpSize, tmpFile);
				
				//computing checksum
				for(unsigned __int64 j=0;j<tmpSize;j++)
					checksum = (checksum+(unsigned char)wbuffer[j])%CHECKSUM_MOD;

				compressStatus.encrypted = _ftelli64(archive);
			}

			//fclose(tmpFile);
			fclose(archive);
			archive = tmpFile;
			//printf("checksum: %llu %llu\n", checksum, cmsi.encryption.checksum);
			if(checksum != cmsi.encryption.checksum){
				fclose(archive);
				//printf("wrong password!\n");
				return KGB_ERROR_WRONG_PASSWORD;
			}
			_fseeki64(archive, 0LL, SEEK_SET);
			
		}

		//MessageBox(0, L"", L"", 0);

		/*int mkdirerr;
		if((mkdirerr=makeDirs(cmsi)) != NULL)
			return mkdirerr;*/

		for(unsigned __int64 i=0;i<cmsi.files.size();i++){
			compressStatus.done_cur = 0;
			compressStatus.uncompressed_cur = cmsi.files[i].size;
			compressStatus.filename = cmsi.files[i].name;

			checksum = 0;

			/*FILE *fout = _wfopen(cmsi.files[i].name, L"wb");
			if(!fout)
				compressStatus.filename = (wstring)L"Cannot create " + cmsi.files[i].name + L", skipping...";
			setvbuf(fout, wbuffer, _IOFBF, WRITE_BUFF_SIZE);		
*/
			for(tmpBuffSize = 0;tmpBuffSize < cmsi.files[i].size;tmpBuffSize += READ_BUFF_SIZE){
				tmpBuffSize2=(cmsi.files[i].size-tmpBuffSize > READ_BUFF_SIZE ? READ_BUFF_SIZE : cmsi.files[i].size-tmpBuffSize);
				//printf("%lld %d %lld %lld : %lld ", i, tmpBuffSize < cmsi.files[i].size, tmpBuffSize, cmsi.files[i].size,  tmpBuffSize2);
				decompressFunc(archive, rbuffer, tmpBuffSize2);
				for(int j=0;j<tmpBuffSize2;j++)
					checksum = (checksum + (unsigned char)rbuffer[j])%CHECKSUM_MOD;
				//fwrite(rbuffer, sizeof(*rbuffer), tmpBuffSize2, fout);
				compressStatus.done_cur += tmpBuffSize2;
				compressStatus.done_all += tmpBuffSize2;
			}

			/*fclose(fout);
			
			SetFileAttributes(cmsi.files[i].name, cmsi.files[i].attrib);
			hTimestamp = CreateFile(cmsi.files[i].name, FILE_WRITE_ATTRIBUTES, NULL, NULL, OPEN_EXISTING, cmsi.files[i].attrib, NULL);
			UnixTimeToFileTime(cmsi.files[i].time_access, &accTime);
			UnixTimeToFileTime(cmsi.files[i].time_create, &creTime);
			UnixTimeToFileTime(cmsi.files[i].time_write, &modTime);
			SetFileTime(hTimestamp, &creTime, &accTime, &modTime);
			CloseHandle(hTimestamp);*/

			//System::Windows::Forms::MessageBox::Show(System::String::Format("{0}", checksum));

			if(checksum != cmsi.checksums[i]){
				FreeLibrary(DLL);
				fclose(archive);
				return KGB_ERROR_BROKEN_ARCHIVE;
			}
		}

		if(FreeLibrary(DLL) == 0)
			return GetLastError();
		

	}else{
		fclose(archive);
		return KGB_ERROR_BROKEN_ARCHIVE;
	}
	
	fclose(archive);
	return 0;
}

#endif //SFX