#include <cstring>
#include <windows.h>
bool UTF8ToUnicode(const char* UTF8, wchar_t* strUnicode)
{
    DWORD dwUnicodeLen;        //转换后Unicode的长度
    TCHAR *pwText;            //保存Unicode的指针
    //  wchar_t* strUnicode;        //返回值
    //获得转换后的长度，并分配内存
    dwUnicodeLen = MultiByteToWideChar(CP_UTF8,0,UTF8,-1,NULL,0);
    pwText = new TCHAR[dwUnicodeLen];
    if (!pwText)
    {
        return false;
    }
    //转为Unicode
    MultiByteToWideChar(CP_UTF8,0,UTF8,-1,pwText,dwUnicodeLen);
    //转为CString
    wcscpy(strUnicode, pwText);
    //清除内存
    delete []pwText;
    return true;
}
