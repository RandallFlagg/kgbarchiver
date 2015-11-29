static wchar_t *loadString(int id){
	CString cs;
	cs.LoadString(id);
	return (wchar_t *)cs.GetString();
}