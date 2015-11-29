#include "compress.cpp"
#include "../common/units.cpp"
#include "../kgb_arch_decompress/dOverwrite.h"
#include "../common/lang.cpp"
#include "../common/icons.cpp"

#define OVERWRITE_YES 1
#define OVERWRITE_YES_ALL 0
#define OVERWRITE_NO 2
#define OVERWRITE_NO_ALL 3

int overwrite(string file, __int64 fsize, __time64_t fmod = NULL){
	char buffer[512];
	char buffer2[256];
	dOverwrite _dOverwrite;
	_dOverwrite.Create(IDD_OVERWRITE);
	HICON ico;
	//_dOverwrite.ShowWindow(SW_SHOW);
	//_dOverwrite.BeginModalState();
	
	sprintf(buffer, loadString(IDS_FILE_ALREADY_EXISTS), file.c_str());
	_dOverwrite.sOverwrite.SetWindowText(buffer);

	struct _finddatai64_t c_file;
	long hFile;
	if((hFile = _findfirsti64(file.c_str(), &c_file)) == -1L){
		//file doesn't exist?
	}else{
		strcpy(buffer2, convertUnits(c_file.size));
		sprintf(buffer, loadString(IDS_OVERWRITE_SIZE), buffer2);
		_dOverwrite.sOriginalSize.SetWindowText(buffer);

		_ctime64_s(buffer2, sizeof(buffer2), &c_file.time_write);
		sprintf(buffer, loadString(IDS_OVERWRITE_MOD), buffer2);
		_dOverwrite.sOriginalMod.SetWindowText(buffer);

		ico = getIcon(file);
		_dOverwrite.iOld.SetIcon(ico);
	}

	
	//new file
	strcpy(buffer2, convertUnits(fsize));
	sprintf(buffer, loadString(IDS_OVERWRITE_SIZE), buffer2);
	_dOverwrite.sNewSize.SetWindowText(buffer);
	if(fmod != NULL){
		_ctime64_s(buffer2, sizeof(buffer2), &fmod);
		sprintf(buffer, loadString(IDS_OVERWRITE_MOD), buffer2);
	}else{
		sprintf(buffer, loadString(IDS_OVERWRITE_MOD), "");
	}
	_dOverwrite.sNewMod.SetWindowText(buffer);
	_dOverwrite.iNew.SetIcon(ico);

	_dOverwrite.ShowWindow(SW_SHOW);
	_dOverwrite.bYes.SetButtonStyle(BS_DEFPUSHBUTTON);
	_dOverwrite.bYesAll.SetButtonStyle(_dOverwrite.bYesAll.GetButtonStyle() & ~BS_DEFPUSHBUTTON);
	return _dOverwrite.RunModalLoop();//_dOverwrite.DoModal();
}

bool mkDirs(vector<string> files){
	//MessageBox(0, "", "", 0);
	for(int i=0;i<files.size();i++){
		char *temp = (char *)files[i].c_str();
		//MessageBox(0, temp, "", 0);
		for(int j=0;j<strlen(temp);j++){
			if(temp[j] == '\\' || temp[j] == '/'){
				/*char tmp[MAX_PATH];
				for(int k=0;k<j;k++){
					tmp[k] = temp[k];
				}*/
				temp[j] = '\0';
				//MessageBox(0, temp, "", 0);
				CreateDirectory(temp, NULL);
				temp[j] = '\\';
			}
		}
	}
	return true;
}

/*security update, thanks to Joxean Koret*/
inline bool dirSecurityCheck(string filename){
	if (!((filename.find("../") != string::npos) || (filename.find("..\\") != string::npos))){
		return true;
	}else{
		char buff[360];
		sprintf(buff, "Security attack detected while decompressing '%.260s'. Would you like to continue decompressing this file?", filename.c_str());
		if(MessageBox(NULL, buff, "KGB Archiver", MB_YESNO | MB_DEFBUTTON2) == IDYES){
			return true;
		}else{
			return false;
		}
	}
}

DWORD WINAPI decompress(LPVOID lParam/*char *archive_name*//*, vector<string> filenames*/){
    sTime = time(NULL);
	_filename = DECOMPRESS_PREPARING;

	setLang();
	
	char overwrite_mode = OVERWRITE_YES;
	//unsigned long uncompressed_bytes=0, compressed_bytes=0;  // Input, output sizes
    
    vector<string> filename;
    vector<__int64> filesize;

	char filebuff[32768];
    
    _size_all = 0;
    _done_all = 0;
    
    FILE* archive=fopen(archive_name, "rb");
    //MessageBox(0,archive_name,"",0);
    if(archive){
		_fseeki64(archive, 0LL, SEEK_END);
		_compressed /*= compressed_bytes*/=_ftelli64(archive);
		_fseeki64(archive, 0LL, SEEK_SET);
		
		if(tolower(archive_name[strlen(archive_name)-1]) == 'e' && tolower(archive_name[strlen(archive_name)-2]) == 'x' && tolower(archive_name[strlen(archive_name)-3]) == 'e'){
			char buff[2048];
			for(int i=0;!feof(archive);i++){
				fread(buff, sizeof(*buff), sizeof(buff)/sizeof(*buff), archive);
				if(cmp(PROGNAME, buff)){
					_fseeki64(archive, i*2048, SEEK_SET);
					break;
				}else if(cmp("ENC", buff)){
					//tu nic nie robimy, to sie zrobi gdzies wczesniej
				}
			}
			//_fseeki64(archive, SFX_OFFSET, SEEK_SET);
			//_compressed -= SFX_OFFSET;
			sfx_arch = true;
		}

		//MessageBox(0, "", "", 0);

        string s=getline(archive);
        if (s.substr(0, string(PROGNAME).size()) != PROGNAME) {
           //printf("Archiwum %s nie jest w formacie " PROGNAME "\n", argv[1]);
           return false;
        }
    
        // Get option -m where m is a digit
        if (s.size()>2 && s[s.size()-2]=='-') {
           int c=s[s.size()-1];
           if (c>='0' && c<='9')
               MEM=c-'0';
        }
        
        // Read "size filename" in "%d\t%s\r\n" format
        while (true) {
              string s=getline(archive);
			  __int64 tmpSize;
			  sscanf(s.c_str(), "%lld", &tmpSize);
              if (s.size()>1) {
                 filesize.push_back(/*atol(s.c_str())*/tmpSize);
                 
                 _size_all += tmpSize;//atol(s.c_str());
                 
                 string::iterator tab=find(s.begin(), s.end(), '\t');
                 if (tab!=s.end())
                    filename.push_back(string(tab+1, s.end()));
                 else
                    filename.push_back("");
              }
              else
                  break;
        }
        
        // Test end of header for "\f\0"
        {
           int c1=0, c2=0;
           if ((c1=getc(archive))!='\f' || (c2=getc(archive))!=0) {
              //printf("%s: Zly format naglowka pliku %d %d\n", argv[1],
              //c1, c2);
              return false;
           }
        }

		_filename = DECOMPRESS_CREATING_DIRS;
		if(files4decompress.size() != 0)
			mkDirs(files4decompress);
		else
			mkDirs(filename);
        
         // Extract files from archive data
         Transformer e(DECOMPRESS, archive);
         
         for (int i=0; i<int(filename.size()); ++i) {
             
             _filename = filename[i];
             
             const __int64 size=filesize[i];
             //uncompressed_bytes+=size;
             _size = size;
             
             FILE *f=NULL;//=fopen(filename[i].c_str(), "wb");
 
			 if(files4decompress.size() != 0){
				//MessageBox(0, "", "", 0);
				for(int j=0;j<files4decompress.size();j++){
					if(filename[i] == files4decompress[j]){
						if(_access(filename[i].c_str(), 4) == 0){
							/*char errMSG[512];
							sprintf(errMSG, loadString(IDS_FILE_ALREADY_EXISTS), filename[i].c_str());
							if(MessageBox(0, errMSG, "KGB Archiver", MB_YESNO) == IDNO){
								break;
							}*/
							if(overwrite_mode != OVERWRITE_YES_ALL && overwrite_mode != OVERWRITE_NO_ALL)
								overwrite_mode = overwrite(filename[i], filesize[i]);
							if(overwrite_mode == OVERWRITE_NO || overwrite_mode == OVERWRITE_NO_ALL)
								break;
						}
						/*security update, thanks to Joxean Koret*/
						if(!dirSecurityCheck(filename[i])){
							_filename = DECOMPRESS_SKIPPING;
						}else
							f = fopen(filename[i].c_str(), "wb");
						break;
					}
				}
				if(f == NULL)
					_filename = DECOMPRESS_SKIPPING;
			 }else{
				 //bool overwrite = true;
				 if(overwrite_mode == OVERWRITE_NO)
					 overwrite_mode = OVERWRITE_YES;
				 if(_access(filename[i].c_str(), 4) == 0){
					/*char errMSG[512];
					sprintf(errMSG, loadString(IDS_FILE_ALREADY_EXISTS), filename[i].c_str());
					if(MessageBox(0, errMSG, "KGB Archiver", MB_YESNO) == IDNO){
						_filename = DECOMPRESS_SKIPPING;
						overwrite = false;
					}*/
					 if(overwrite_mode != OVERWRITE_YES_ALL && overwrite_mode != OVERWRITE_NO_ALL)
						overwrite_mode = overwrite(filename[i], filesize[i]);
					 if(overwrite_mode == OVERWRITE_NO || overwrite_mode == OVERWRITE_NO_ALL)
						_filename = DECOMPRESS_SKIPPING;
				 }
				 if((overwrite_mode == OVERWRITE_YES || overwrite_mode == OVERWRITE_YES_ALL) || _access(filename[i].c_str(), 4) != NULL){
					/*security update, thanks to Joxean Koret*/
					 if(!dirSecurityCheck(filename[i])){
						_filename = DECOMPRESS_SKIPPING;
					}else
						f = fopen(filename[i].c_str(), "wb");
				 }
			 }

			 if (f){
				 setvbuf(f, filebuff, _IOFBF, sizeof(filebuff));
             }
             for (unsigned long j=0; j<size; ++j) {
                 int c=e.decode();
                 if (f)
                    putc(c, f);
                 _done = j;
             }
             if (f) {
                 //printf("wypakowany\n");
                 fclose(f);
             }
             
			 _done_all += _size;
         }
         
         fclose(archive);
         
    }else
       return false;
       
    return true; 
}

__int64 isSFXencrypted(char *arch){
	FILE* archive=fopen(arch, "rb");
    
    if(archive){

		if(tolower(arch[strlen(arch)-1]) == 'e' && tolower(arch[strlen(arch)-2]) == 'x' && tolower(arch[strlen(arch)-3]) == 'e'){
			char buff[2048];
			for(__int64 i=0;!feof(archive);i++){
				fread(buff, sizeof(*buff), sizeof(buff)/sizeof(*buff), archive);
				if(cmp(PROGNAME, buff)){
					_fseeki64(archive, i*2048, SEEK_SET);
					break;
				}else if(cmp("ENC", buff)){
					//tu nic nie robimy, to sie zrobi gdzies wczesniej
					fclose(archive);
					return i*2048+sizeof("ENC");
				}
			}
			//_compressed -= SFX_OFFSET;
			fclose(archive);
			return false;//sfx_arch = true;
		}
	fclose(archive);
	return false;
	}
	return false;
}

bool checkArchiveFormat(char *arch){
	FILE* archive=fopen(arch, "rb");
    
    if(archive){

		if(tolower(arch[strlen(arch)-1]) == 'e' && tolower(arch[strlen(arch)-2]) == 'x' && tolower(arch[strlen(arch)-3]) == 'e'){
			char buff[2048];
			for(int i=0;!feof(archive);i++){
				fread(buff, sizeof(*buff), sizeof(buff)/sizeof(*buff), archive);
				if(cmp(PROGNAME, buff)){
					_fseeki64(archive, i*2048, SEEK_SET);
					break;
				}else if(cmp("ENC", buff)){
					//tu nic nie robimy, to sie zrobi gdzies wczesniej
				}
			}
			//_compressed -= SFX_OFFSET;
			sfx_arch = true;
		}

        string s=getline(archive);
        if (s.substr(0, string(PROGNAME).size()) != PROGNAME) {
           //printf("Archiwum %s nie jest w formacie " PROGNAME "\n", argv[1]);
           return false;
        }
    
        // Get option -m where m is a digit
        if (s.size()>2 && s[s.size()-2]=='-') {
           int c=s[s.size()-1];
		   if (c>='0' && c<='9'){
			   //MEM=c-'0';
		   }else
			   return false;
        }
        
        // Read "size filename" in "%d\t%s\r\n" format
        while (true) {
              string s=getline(archive);
              if (s.size()>1) {
                 //filesize.push_back(atol(s.c_str()));
                 
                 //_size_all += atol(s.c_str());
                 
                 string::iterator tab=find(s.begin(), s.end(), '\t');
				 if (tab!=s.end()){
                    //filename.push_back(string(tab+1, s.end()));
				 }else{
                    //filename.push_back("");
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
              //c1, c2);
              return false;
           }
        }

	fclose(archive);

	}else
		return false;

	return true;
}

bool getFilesFromArchive(char *arch, vector<string> &files){
	FILE* archive=fopen(arch, "rb");

	if(!archive){
		return false;
	}

	if(tolower(arch[strlen(arch)-1]) == 'e' && tolower(arch[strlen(arch)-2]) == 'x' && tolower(arch[strlen(arch)-3]) == 'e'){
		char buff[2048];
		for(int i=0;!feof(archive);i++){
			fread(buff, sizeof(*buff), sizeof(buff)/sizeof(*buff), archive);
			if(cmp(PROGNAME, buff)){
				_fseeki64(archive, i*2048, SEEK_SET);
				break;
			}else if(cmp("ENC", buff)){
				//tu nic nie robimy, to sie zrobi gdzies wczesniej
			}
		}
		//_compressed -= SFX_OFFSET;
		sfx_arch = true;
	}

	string s=getline(archive);
    if (s.substr(0, string(PROGNAME).size()) != PROGNAME) {
        //printf("Archiwum %s nie jest w formacie " PROGNAME "\n", argv[1]);
        return false;
    }

	// Read "size filename" in "%d\t%s\r\n" format
    while (true) {
            string s=getline(archive);
            if (s.size()>1) {
                //filesize.push_back(atol(s.c_str()));
                
                //_size_all += atol(s.c_str());
                
                string::iterator tab=find(s.begin(), s.end(), '\t');
                if (tab!=s.end())
					files.push_back(string(tab+1, s.end()));
				else{
					//files.push_back("");
				}
            }
            else
                break;
    }

	fclose(archive);

	return true;
}

bool getFilesFromArchive(char *arch, vector<string> &files, vector<__int64> &filesizes){
	FILE* archive=fopen(arch, "rb");

	if(!archive){
		return false;
	}

	if(tolower(arch[strlen(arch)-1]) == 'e' && tolower(arch[strlen(arch)-2]) == 'x' && tolower(arch[strlen(arch)-3]) == 'e'){
		char buff[2048];
		for(int i=0;!feof(archive);i++){
			fread(buff, sizeof(*buff), sizeof(buff)/sizeof(*buff), archive);
			if(cmp(PROGNAME, buff)){
				_fseeki64(archive, i*2048, SEEK_SET);
				break;
			}
		}
		//_compressed -= SFX_OFFSET;
		sfx_arch = true;
	}

	string s=getline(archive);
    if (s.substr(0, string(PROGNAME).size()) != PROGNAME) {
        //printf("Archiwum %s nie jest w formacie " PROGNAME "\n", argv[1]);
        return false;
    }

	// Read "size filename" in "%d\t%s\r\n" format
    while (true) {
            string s=getline(archive);
            __int64 tmpSize;
			sscanf(s.c_str(), "%lld", &tmpSize);
			if (s.size()>1) {
                filesizes.push_back(/*atol(s.c_str())*/tmpSize);
                
                //_size_all += atol(s.c_str());
                
                string::iterator tab=find(s.begin(), s.end(), '\t');
                if (tab!=s.end())
					files.push_back(string(tab+1, s.end()));
				else{
					//files.push_back("");
				}
            }
            else
                break;
    }

	fclose(archive);

	return true;
}

int getArchOption(char *arch){
	FILE* archive=fopen(archive_name, "rb");
    //MessageBox(0,archive_name,"",0);
    if(archive){
		_fseeki64(archive, 0LL, SEEK_END);
		_compressed /*= compressed_bytes*/=_ftelli64(archive);
		_fseeki64(archive, 0LL, SEEK_SET);
		
		if(tolower(archive_name[strlen(archive_name)-1]) == 'e' && tolower(archive_name[strlen(archive_name)-2]) == 'x' && tolower(archive_name[strlen(archive_name)-3]) == 'e'){
			char buff[2048];
			for(int i=0;!feof(archive);i++){
				fread(buff, sizeof(*buff), sizeof(buff)/sizeof(*buff), archive);
				if(cmp(PROGNAME, buff)){
					_fseeki64(archive, i*2048, SEEK_SET);
					break;
				}else if(cmp("ENC", buff)){
					//tu nic nie robimy, to sie zrobi gdzies wczesniej
				}
			}
			//_fseeki64(archive, SFX_OFFSET, SEEK_SET);
			//_compressed -= SFX_OFFSET;
			sfx_arch = true;
		}

		//MessageBox(0, "", "", 0);

        string s=getline(archive);
        if (s.substr(0, string(PROGNAME).size()) != PROGNAME) {
           //printf("Archiwum %s nie jest w formacie " PROGNAME "\n", argv[1]);
           return false;
        }
    
        // Get option -m where m is a digit
        if (s.size()>2 && s[s.size()-2]=='-') {
           int c=s[s.size()-1];
           if (c>='0' && c<='9')
               return c-'0';
        }
	}
	return false;
}