#include "../compress.cpp"
#include "../ziparchive/ziparchive/ziparchive.h"

bool isKGBArchive(wstring file){
	//return true;
	compressInfo cmsi;
	return (readHeader(file, &cmsi) == NULL);
}

bool isZIPArchive(wstring file){
	CZipArchive zip;
	try{
		zip.Open(file.c_str(), CZipArchive::OpenMode::zipOpenReadOnly);
		zip[0]->GetFileName();
		zip.Close();
	}catch(...){
		return false;
	}
	return true;
}