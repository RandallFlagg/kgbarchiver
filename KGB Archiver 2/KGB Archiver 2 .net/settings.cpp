//#include "stdafx.h"

namespace KGBArchiver2net {

	using namespace System;

    static ref class settings{
	public:
		static ref class columns{
		public:
			static bool cFilename = true;
			static bool cSize = true;
			static bool cType = true;
			static bool cCreated = false;
			static bool cModified = true;
			static bool cAccessed = false;
			static bool cAttributes = true;
			//columns(){cFilename=cSize=cType=cCreated=cModified=cAccessed=cAttributes=true;}
		};
		static ref class general{
		public:
			static int language = 0x0009;
			static bool checkUpdates = true;
			static bool saveWindowState = true;
			static int recentItemsCount = 10;
			//general(){language=0;checkUpdates=true;saveWindowState=true;}
		};
		static ref class fileManager{
		public:
			static bool showHidden = true;
			static bool showArchiveTree = true;
			static bool showFileGroups = true;
			//fileManager(){showHidden=showArchiveTree=true;}
		};
		static ref class explorerExtension{
		public:
			static bool Compress = true;
			static bool Decompress = true;
		};
		static int skin = 0;
	};
	
};