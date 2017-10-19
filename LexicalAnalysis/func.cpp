#include<iostream>
#include<string>
#include<cctype>
#include<vector>
#include<algorithm>
using namespace std;
extern vector<string> KEY;
extern vector<string> ID;
extern vector<string> DELIMITER;
extern vector<string> OP;
extern vector<string> STRING;
extern vector<string> NUM;
extern vector<pair<string, int>> LIST;
extern vector<string> ERROR;
extern char token[128];
extern int tokenIndex;
extern int lineCount;
int KeyFind(std::string str, bool &isFind)
{
    int pos;
    vector<string>::iterator KEYiter = find(KEY.begin(), KEY.end(), str);
    pos = distance(KEY.begin(), KEYiter);
    if (KEY.end() != KEYiter)
    {
        isFind = true;
    }
    else
    {
        isFind = false;
    }
    return pos;
}
int IDFind(std::string str, bool &isFind)
{
    int pos;
    vector<string>::iterator IDiter = find(ID.begin(), ID.end(), str);
    pos = distance(ID.begin(), IDiter);
    if (ID.end() != IDiter)
    {
        isFind = true;
    }
    else
    {
        isFind = false;
    }
    return pos;
}
int DELIMITERFind(std::string str, bool &isFind)
{
    int pos;
    vector<string>::iterator DELIMITERiter = find(DELIMITER.begin(), DELIMITER.end(), str);
    pos = distance(DELIMITER.begin(), DELIMITERiter);
    if (DELIMITER.end() != DELIMITERiter)
    {
        isFind = true;
    }
    else
    {
        isFind = false;
    }
    return pos;
}
int OPFind(std::string str, bool &isFind)
{
    int pos;
    vector<string>::iterator OPiter = find(OP.begin(), OP.end(), str);
    pos = distance(OP.begin(), OPiter);
    if (OP.end() != OPiter)
    {
        isFind = true;
    }
    else
    {
        isFind = false;
    }
    return pos;
}
void ResetTokenArray(void)
{
    memset(token, 0, sizeof(token));
    tokenIndex = 0;
}
void TokenPush(void)
{
    bool find;
    token[tokenIndex] = '\0';
    string t = token;
    int KeyPos = KeyFind(t, find);
    if (find)
    {
        LIST.push_back(make_pair("key", KeyPos));
        printf("<%s, key>\n", token);
        ResetTokenArray();
    }
    else
    {
        int IDPos = IDFind(t, find);
        if (find)//id
        {
            LIST.push_back(make_pair("ID", IDPos));
        }
        else//id
        {
            ID.push_back(t);
            LIST.push_back(make_pair("ID", ID.size() - 1));
        }
        printf("<%s, id>\n", token);
        ResetTokenArray();
    }
}
void StringTokenPush(void)
{
    token[tokenIndex] = '\0';
    string t = token;
    STRING.push_back(t);
    LIST.push_back(make_pair("string", STRING.size()-1));
    printf("<%s, string>\n", t.c_str());
    ResetTokenArray();
}
void DelimiterTokenPush(void)
{
    bool find;
    token[tokenIndex] = '\0';
    string t = token;
    int pos = DELIMITERFind(t, find);
    LIST.push_back(make_pair("delimiter", pos));
    printf("<%s, delimiter>\n", t.c_str());
    ResetTokenArray();
}
void OPTokenPush(void)
{
    bool find;
    token[tokenIndex] = '\0';
    string t = token;
    int pos = OPFind(t, find);
    LIST.push_back(make_pair("op", pos));
    printf("<%s, op>\n", t.c_str());
    ResetTokenArray();
}
void NUMTokenPush(void)
{
    token[tokenIndex] = '\0';
    string t = token;
    NUM.push_back(t);
    LIST.push_back(make_pair("num", NUM.size()-1));
    printf("<%s, num>\n", t.c_str());
    ResetTokenArray();
}
void NumErrorPush(void)
{
    token[tokenIndex] = '\0';
    string t = token;
    char e[140];
    sprintf(e,"LINE %d ERROR: Token<%s> is not a unsigned number!\n",lineCount, t.c_str());
    printf("LINE %d ERROR: Token<%s> is not a unsigned number!\n",lineCount, t.c_str());
    string er=e;
    ERROR.push_back(er);
    ResetTokenArray();
}
