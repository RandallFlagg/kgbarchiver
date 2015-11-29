#include "compress.cpp"

void sortFiles(vector<string> &files){
	string common[6][36] = {{"txt", "htm", "html", "xml", "css", "php", "php3", "php4", "php5", "asp", "vcproj", "sln", "dev", "cpp", ".c", "h", "pl" /*perl*/, "rc", "pas", "iss" /*inno setup*/, ".vb", ".js", "vbs", "bat", "asm", "rtf", "reg", "key", "csv", "cue", "ini", "inf", "doc", "xls", "suo", ""},
							{"exe",	"com", "dll", ".o", "obj", "lib", ".a", ""},
						    {"bmp", "dib", "gif", "ico", "png", "wmf", "fla", "pps", "cpt", "pspimage", "psp", "psd", "jpg", "jpeg", "jpe", "jp2", "j2c", "j2k", "jpc", "jpx", "tga", "tiff", "swf", ""},
							{"mpg", "mpeg", "mpe", "asf", "wmv", "avi", "mov", ""},
							{"wav", "wma", "mp2", "mp3", "mp4", "ogg", ""},
							{"zoo", "zip", ".gz", "bz2", ".7z", "rar", "ace", "uha", ""}};
	unsigned long idx = 0;
	string temp;

	for(unsigned long i=0;i<6;i++){
		for(unsigned long j=0;common[i][j].length()!=0;j++){
			for(unsigned long k=idx;k<files.size();k++){
				if(files[k].length() < common[i][j].length())
					continue;
				//MessageBox(0, txt2lower(common[i][j]), "", 0);
				if(cmp((char *)txt2lower(common[i][j]).c_str(), (char *)txt2lower(files[k].substr(files[k].length()-common[i][j].length())).c_str())){
					temp = files[idx];
					files[idx] = files[k];
					files[k] = temp;
					idx++;
				}
			}
		}
	}

	/*for(int i=0;i<files.size();i++){
		MessageBox(0, files[i].c_str(), "", 0);
	}*/

}

DWORD WINAPI compress(LPVOID lpParam){	
   sTime = time(NULL);
   _filename = COMPRESS_PREPARING;	

   vector<__int64> filesize;
   
   /*unsigned long compressed_bytes=0;
   unsigned long uncompressed_bytes=0;*/

   char filebuff[32768];
   char readbuff[32768];
   __int64 rbuffSize=0;
   __int64 SFX_SIZE = 0;
   
   _size_all = 0;
   if(passwd[0] != '\0')
	   _size_all = 1;
   _done_all = 0;
   _compressed = 0;

   _filename = COMPRESS_SORTING;
   sortFiles(filename);
   
   _filename = COMPRESS_GETTING_FILE_SIZES;

   // Get file sizes
    for (int i=0; i<int(filename.size()); ++i) {
      FILE* f=fopen(filename[i].c_str(), "rb");
      if (!f) {
        //printf("Nie znaleziono pliku, pomijanie: %s\n",
        //filename[i].c_str());
        filesize.push_back(-1);
      }
      else {
        _fseeki64(f, 0LL, SEEK_END);
        filesize.push_back(_ftelli64(f));
        _size_all += _ftelli64(f);
        fclose(f);
      }
    }

    /*if (filesize.empty() || *max_element(filesize.begin(), filesize.end())<0){
      //printf("Brak plikow do kompresji, archiwum nie zostalo utworzone.\n");
		//_filename = COMPRESS_NOTHING2COMPRESS;
		MessageBox(0, loadString(IDS_NOTHING2COMPRESS), "KGB Archiver", 0);
		return false;
    } */
	if(_size_all == 0){
		//MessageBox(0, loadString(IDS_NOTHING2COMPRESS), "KGB Archiver", 0);
		//_filename = COMPRESS_NOTHING2COMPRESS;
		return false;
	}
   
    // Write header
    FILE *archive=fopen(archive_name, "wb");
    if (!archive) {
      //printf("Nie mozna utworzyc archiwum: %s\n", argv[1]);
      return false;
    }

	/*if(*/setvbuf(archive, filebuff, _IOFBF, sizeof(filebuff));// != NULL){
		/*return false;
	}*/

	if(sfx_arch){ //do³¹czamy modu³ sfx
		_filename = COMPRESS_ADDING_SFX_MOD;
		//odczytujemy œcie¿kê do modu³u
		HANDLE hProc = GetCurrentProcess();
		HMODULE hMod;
        DWORD cbNeeded;
		char processName[MAX_PATH];

        EnumProcessModules(hProc, &hMod, sizeof(hMod), &cbNeeded);
        GetModuleFileNameEx(hProc, hMod, processName, MAX_PATH);
        
		for(int i=strlen(processName)-1;i>=0;i--){
			if(processName[i] == '\\' || processName[i] == '/'){
				processName[i+1] = '\0';
				sprintf(processName, "%skgb_arch_sfx.mod", processName);
				break;
			}
		}

		FILE *sfx = fopen(processName, "rb");

		if(sfx == NULL){
			MessageBox(0, "An internal error has occured, please reinstall KGB Archiver!", "KGB Archiver", 0);
			_fcloseall();
			DeleteFile(archive_name);
			return false;
		}

		char buffer[2048];
		while(!feof(sfx)){
			fread(buffer, sizeof(*buffer), sizeof(buffer)/sizeof(*buffer), sfx);
			fwrite(buffer, sizeof(*buffer), sizeof(buffer)/sizeof(*buffer), archive);
			SFX_SIZE+=2048;
		}

		//fread(buffer, sizeof(char), sizeof(buffer), sfx);
		//fclose(sfx);
		//fwrite(buffer, sizeof(char), sizeof(buffer), archive);//zapiszemy aplikacje + troche œmieci z bufora;)
	}

    fprintf(archive, PROGNAME " -%d\r\n", MEM);
    for (int i=0; i<int(filename.size()); ++i) {
      if (filesize[i]>=0)
        fprintf(archive, "%lld\t%s\r\n", filesize[i], filename[i].c_str());
    }
    putc(032, archive);  // MSDOS EOF
    putc('\f', archive);
    putc(0, archive);
    
    
	// Write data
    Transformer e(COMPRESS, archive);
    __int64 file_start=_ftelli64(archive);
    for (int i=0; i<int(filename.size()); ++i) {
      const __int64 size=filesize[i];
      if (size>=0) {
        _filename = filename[i];
        _size = size;
        _done = 0;
        
        /*uncompressed_bytes+=size;*/
        //printf("%-23s %10ldKB -> ", filename[i].c_str(), size/1024);
        FILE* f=fopen(filename[i].c_str(), "rb");
        /*int c;
        for (__int64 j=0; j<size; ++j) {
          if (f)
            c=getc(f);
          else
            c=0;
          e.encode(c);
          _done=j;
        }*/
		if(f){
			for(__int64 j=0;j<size;j+=sizeof(readbuff)){
				rbuffSize = fread(readbuff, sizeof(*readbuff), sizeof(readbuff), f);
				for(__int64 k=0;k<rbuffSize;k++){
					e.encode(readbuff[k]);
				}
				_done = j;
				_compressed = _ftelli64(archive);
			}
		}else
			e.encode(0);
        if (f)
          fclose(f);
        //printf("%ldKB\n", (ftell(archive)-file_start)/1024);
        _done_all += _size;
        _compressed = file_start = _ftelli64(archive);
        //_compressed = file_start;
      }
    }
    e.flush();
    /*compressed_bytes=ftell(archive);*/
    fclose(archive);

	if(passwd[0] == '\0')
		goto end;

	//szyfrowanie

	//gen_tabs();
	_filename = COMPRESS_ENCODING;
	archive = fopen(archive_name, "rb");
	char archive_name2[MAX_PATH];
	strcpy(archive_name2, archive_name);
	archive_name2[strlen(archive_name2)-1] = 'e';//zmiana rozszerzenia
	FILE *fout = fopen(archive_name2, "wb");

	if(archive == NULL || fout == NULL)
		return false;

	aes_ctx ctx[1];
	ctx->n_rnd = 0; // ensure all flags are initially set to zero
    ctx->n_blk = 0;
	aes_enc_key((unsigned char*)passwd, 32, ctx);
	if(sfx_arch){
		char buff[2048];
		for(int i=0;i<SFX_SIZE;i+=2048){
			fread(buff, sizeof(*buff), sizeof(buff)/sizeof(*buff), archive);
			fwrite(buff, sizeof(*buff), sizeof(buff)/sizeof(*buff), fout);
		}
		//fread(buff, sizeof(char), sizeof(PROGNAME), archive);
		fwrite("ENC", sizeof(char), sizeof("ENC"), fout);
	}
	if(encfile(archive, fout, ctx, archive_name, archive_name2) != OK)
		return false;

	fclose(archive);
	fclose(fout);

	DeleteFile(archive_name);

end:
	if(sfx_arch){
		if(passwd[0] != '\0'){
			archive_name[strlen(archive_name)-3] = '\0';
			sprintf(archive_name, "%sexe", archive_name);
			MoveFile(archive_name2, archive_name);
		}else{
			char archive_name2[MAX_PATH];
			strcpy(archive_name2, archive_name);
			archive_name[strlen(archive_name)-3] = '\0';
			sprintf(archive_name, "%sexe", archive_name);
			MoveFile(archive_name2, archive_name);
		}
	}

	if(passwd[0] != '\0')
		_done_all++;
    
    return true;
}