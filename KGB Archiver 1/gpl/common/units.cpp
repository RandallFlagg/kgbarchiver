#include <string.h>
#include <stdio.h>

using namespace std;

#ifndef units_cpp
#define units_cpp

static char *convertUnits(__int64 value){
	string u[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "JT"};//9
	long double tmp = (long double)value;
	int i;
	char out[32];
	for(i=0;tmp >= 1024.0 && i < 9;i++){
		tmp /= 1024.0;
	}
	sprintf(out, "%.1f%s", tmp, u[i].c_str());
	return out;
}

#endif