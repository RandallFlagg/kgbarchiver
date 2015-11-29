#include <string.h>
#include <stdio.h>

using namespace std;

wstring convertUnits(__int64 value){
	wstring u[] = {L"B", L"KB", L"MB", L"GB", L"TB", L"PB", L"EB", L"ZB", L"JT"};//9
	long double tmp = (long double)value;
	int i;
	wchar_t out[32];
	for(i=0;tmp >= 1024.0f && i < 9;i++){
		tmp /= 1024.0f;
	}
	swprintf(out, L"%.1f%ws", tmp, u[i].c_str());
	return (wstring)out;
}

wstring convertAttributes(unsigned int attr){
	wstring out;

	out += (attr & _A_SUBDIR ? L"d" : L"-");
	out += (attr & _A_RDONLY ? L"r-" : L"rw");
	out += (attr & _A_ARCH ? L"a" : L"-");
	out += (attr & _A_HIDDEN ? L"h" : L"-");
	out += (attr & _A_SYSTEM ? L"s" : L"-");

	return out;
}

wstring convertTime(time_t t){
	wchar_t out[16];
	int h, m, s;
	h = t/3600;
	m = (t%3600)/60;
	s = t%60;
	swprintf(out, L"%d%d:%d%d:%d%d", h/10, h%10, m/10, m%10, s/10, s%10);
	return (wstring)out;
}