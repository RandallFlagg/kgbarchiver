// KGB Archiver 2 .net.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"
#include "../language.h"
//#include "file_management.cpp"

using namespace KGBArchiver2net;
using namespace Microsoft::Win32;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	
	Form1 ^fMain;

	//load settings
	String ^mainHook = gcnew String(L"HKEY_CURRENT_USER\\Software\\KGB Archiver 2");
	String ^hook;

	try{
		//reg->OpenSubKey(gcnew String(L"HKEY_CURRENT_USER\\Software\\KGB Archiver 2"));
		//MessageBox::Show(Registry::GetValue(hook, L"columnscFilename", (int)settings::columns::cFilename)->GetType()->ToString());
		hook = mainHook + L"\\columns";
		settings::columns::cFilename = (int)Registry::GetValue(hook, L"cFilename", (int)settings::columns::cFilename);
		settings::columns::cSize = (int)Registry::GetValue(hook, L"cSize", (int)settings::columns::cSize);
		settings::columns::cType = (int)Registry::GetValue(hook, L"cType", (int)settings::columns::cType);
		settings::columns::cCreated = (int)Registry::GetValue(hook, L"cCreated", (int)settings::columns::cCreated);
		settings::columns::cModified = (int)Registry::GetValue(hook, L"cModified", (int)settings::columns::cModified);
		settings::columns::cAccessed = (int)Registry::GetValue(hook, L"cAccessed", (int)settings::columns::cAccessed);
		settings::columns::cAttributes = (int)Registry::GetValue(hook, L"cAttributes", (int)settings::columns::cAttributes);

		hook = mainHook + L"\\fileManager";
		settings::fileManager::showArchiveTree = (int)Registry::GetValue(hook, L"showArchiveTree", (int)settings::fileManager::showArchiveTree);
		settings::fileManager::showHidden = (int)Registry::GetValue(hook, L"showHidden", (int)settings::fileManager::showHidden);
		settings::fileManager::showFileGroups = (int)Registry::GetValue(hook, L"showFileGroups", (int)settings::fileManager::showFileGroups);

		hook = mainHook + L"\\general";
		settings::general::checkUpdates = (int)Registry::GetValue(hook, L"checkUpdates", (int)settings::general::checkUpdates);
		settings::general::saveWindowState = (int)Registry::GetValue(hook, L"saveWindowState", (int)settings::general::saveWindowState);
		settings::general::language = (int)Registry::GetValue(hook, L"language", (int)settings::general::language);
		settings::general::recentItemsCount = (int)Registry::GetValue(hook, L"recentItemsCount", (int)settings::general::recentItemsCount);

		hook = mainHook + L"\\explorerExtension";
		settings::explorerExtension::Compress = (int)Registry::GetValue(hook, L"compress", (int)settings::explorerExtension::Compress);
		settings::explorerExtension::Decompress = (int)Registry::GetValue(hook, L"decompress", (int)settings::explorerExtension::Decompress);

		//language settings must be read first
		fMain = gcnew Form1();
		fMain->args = args;

		hook = mainHook;
		settings::skin = (int)Registry::GetValue(hook, L"skin", settings::skin);

		hook = mainHook + L"\\windowState";
		fMain->WindowState = (FormWindowState)Registry::GetValue(hook, L"", fMain->WindowState);
		
		if(fMain->WindowState != FormWindowState::Minimized){
			fMain->Left = (int)Registry::GetValue(hook, L"left", fMain->Left);
			fMain->Top = (int)Registry::GetValue(hook, L"top", fMain->Top);
			fMain->Width = (int)Registry::GetValue(hook, L"width", fMain->Width);
			fMain->Height = (int)Registry::GetValue(hook, L"height", fMain->Height);

			hook = mainHook + L"\\windowState\\addressbar";
			fMain->tAddr->Left = (int)Registry::GetValue(hook, L"left", fMain->tAddr->Left);
			fMain->tAddr->Top = (int)Registry::GetValue(hook, L"top", fMain->tAddr->Top);
			//fMain->tAddr->Dock = (DockStyle)Registry::GetValue(hook, L"docking", fMain->tAddr->Dock);
			//fMain->tAddr->Width = (int)Registry::GetValue(hook, L"width", fMain->tAddr->Width);
			//fMain->tAddr->Height = (int)Registry::GetValue(hook, L"height", fMain->tAddr->Height);

			hook = mainHook + L"\\windowState\\menubar";
			fMain->mMain->Left = (int)Registry::GetValue(hook, L"left", fMain->mMain->Left);
			fMain->mMain->Top = (int)Registry::GetValue(hook, L"top", fMain->mMain->Top);
			//fMain->mMain->Dock = (DockStyle)Registry::GetValue(hook, L"docking", fMain->mMain->Dock);
			//fMain->mMain->Width = (int)Registry::GetValue(hook, L"width", fMain->mMain->Width);
			//fMain->mMain->Height = (int)Registry::GetValue(hook, L"height", fMain->mMain->Height);

			hook = mainHook + L"\\windowState\\toolbar";
			fMain->tMain->Left = (int)Registry::GetValue(hook, L"left", fMain->tMain->Left);
			fMain->tMain->Top = (int)Registry::GetValue(hook, L"top", fMain->tMain->Top);
			//fMain->tMain->Dock = (DockStyle)Registry::GetValue(hook, L"docking", fMain->tMain->Dock);
			//fMain->tMain->Width = (int)Registry::GetValue(hook, L"width", fMain->tMain->Width);
			//fMain->tMain->Height = (int)Registry::GetValue(hook, L"height", fMain->tMain->Height);
		}
		
		
	}catch(Exception ^e){
		//MessageBox::Show(e->ToString());
		if(fMain == nullptr){
			fMain = gcnew Form1();
			fMain->args = args;
		}
		fMain->firstrun = true;
	}


	// Create the main window and run it
	Application::Run(fMain);

	//save settings
	try{
		//reg->OpenSubKey(gcnew String(L"HKEY_CURRENT_USER\\Software\\KGB Archiver 2\\"));
		
		hook = mainHook + L"\\columns";
		Registry::SetValue(hook, L"cFilename", settings::columns::cFilename, RegistryValueKind::DWord);
		Registry::SetValue(hook, L"cSize", settings::columns::cSize, RegistryValueKind::DWord);
		Registry::SetValue(hook, L"cType", settings::columns::cType, RegistryValueKind::DWord);
		Registry::SetValue(hook, L"cCreated", settings::columns::cCreated, RegistryValueKind::DWord);
		Registry::SetValue(hook, L"cModified", settings::columns::cModified, RegistryValueKind::DWord);
		Registry::SetValue(hook, L"cAccessed", settings::columns::cAccessed, RegistryValueKind::DWord);
		Registry::SetValue(hook, L"cAttributes", settings::columns::cAttributes, RegistryValueKind::DWord);

		hook = mainHook + L"\\fileManager";
		Registry::SetValue(hook, L"showArchiveTree", settings::fileManager::showArchiveTree, RegistryValueKind::DWord);
		Registry::SetValue(hook, L"showHidden", settings::fileManager::showHidden, RegistryValueKind::DWord);
		Registry::SetValue(hook, L"showFileGroups", settings::fileManager::showFileGroups, RegistryValueKind::DWord);

		hook = mainHook + L"\\general";
		Registry::SetValue(hook, L"checkUpdates", settings::general::checkUpdates, RegistryValueKind::DWord);
		Registry::SetValue(hook, L"saveWindowState", settings::general::saveWindowState, RegistryValueKind::DWord);
		Registry::SetValue(hook, L"language", settings::general::language, RegistryValueKind::DWord);
		Registry::SetValue(hook, L"recentItemsCount", settings::general::recentItemsCount, RegistryValueKind::DWord);

		hook = mainHook;
		Registry::SetValue(hook, L"skin", settings::skin, RegistryValueKind::DWord);

	}catch(Exception ^e){
		MessageBox::Show(e->ToString(), L"KGB Archiver 2");
	}

	if(fMain->restartRequied)
		Application::Restart();
	
	delete mainHook;
	delete fMain;

	return 0;
}
