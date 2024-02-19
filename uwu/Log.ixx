#include <wtypes.h>
#include <atltime.h>
#include <ctime>;
#include <iostream>;
#include <cstdarg>;
#include <string>;
#include <cstring>;

export module Log;


//export void Message(std::string message) {
//	std::cout << message;
//}


//Using CString::FormatV(), you can write functions like the following:

export void WriteLogEntry(LPCTSTR pstrFormat, ...)
{
	CTime timeWrite;
	timeWrite = CTime::GetCurrentTime();

	// write the time out
	CString str = timeWrite.Format(_T("%d %b %y %H:%M:%S - "));
	//refFile.Write(str, str.GetLength());
	std::wcout << str.GetString();

	// format and write the data we were given
	va_list args;
	va_start(args, pstrFormat);
	str.FormatV(pstrFormat, args);
	va_end(args);

	//refFile.Write(str, str.GetLength());
	std::wcout << str.GetString() << std::endl;

	str.ReleaseBuffer();
	return;
}