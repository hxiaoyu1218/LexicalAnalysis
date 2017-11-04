#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <stdio.h>
#include <cstring>
#include <algorithm>
#include <qstring.h>
#include <fstream>
#include "func.h"
#define BUFFER_SIZE 128
#define TOKEN_SIZE 256
using namespace std;
string Path;//输入文件路径
int lineCount = 1;//行数
int charCount = -1;//字符数
char Buffer[BUFFER_SIZE * 2];//配对缓冲区
char token[TOKEN_SIZE];//记号缓冲区
int tokenIndex = 0;//记号缓冲区指针
FILE* input;//输入文件流指针
vector<pair<string, int>> LIST;//记号表
vector<string> NUM;
vector<string> KEY{ "auto", "double", "int", "struct", "break", "else", "long",
                    "switch","case", "enum", "register", "typedef", "char",
                    "extern", "return", "union","const", "float", "short",
                    "unsigned", "continue", "for", "signed", "void","default",
                    "goto", "sizeof", "volatile", "do", "if", "while", "static" };
vector<string> ID;
vector<string> OP{ "+","-","*","/","%","(",")","<",">","=","++","--","+=","-=","*=",
                   "/=","<=",">=","==" ,"!=","|=","&=","%=","^=","!","~","^","&","||","|" ,"<<",">>"};
vector<string> DELIMITER{ ",",";","//","{","}","[","]","\"", "'" };
vector<string> STRING;
vector<string> ERROR;

extern bool UTF8ToUnicode(const char* UTF8, wchar_t* strUnicode);

void Init(void)//open file
{
    wchar_t strUnicode[260];
    UTF8ToUnicode(Path.c_str(), strUnicode);
    input = _wfopen(strUnicode, L"r");
    FILE *E=input;
}
void Output(void)//write file
{
    string t=Path;
    string dir=t.substr(0,1+t.find_last_of('/'));//获取目录
    string fname=t.substr(1+t.find_last_of('/'),t.length());//获取文件名
    string outPath=dir+fname.substr(0,fname.find_last_of('.'))+".lex";//合成输出路径

    FILE *tempOut;
    wchar_t strUnicode[260];
    UTF8ToUnicode(outPath.c_str(), strUnicode);
    tempOut = _wfopen(strUnicode, L"w");

    fprintf(tempOut,"%d\n",LIST.size());
    for(int i=0;i<LIST.size();i++)
    {
        fprintf(tempOut,"%s\n%d\n",LIST[i].first.c_str(),LIST[i].second);
    }
    fprintf(tempOut,"%d\n",NUM.size());
    for(int i=0;i<NUM.size();i++)
    {
        fprintf(tempOut,"%s\n",NUM[i].c_str());
    }
    fprintf(tempOut,"%d\n",ID.size());
    for(int i=0;i<ID.size();i++)
    {
        fprintf(tempOut,"%s\n",ID[i].c_str());
    }
    fprintf(tempOut,"%d\n",STRING.size());
    for(int i=0;i<STRING.size();i++)
    {
       fprintf(tempOut,"%s\n",STRING[i].c_str());
    }
    fclose(tempOut);
}
int ScanLbuffer(void)//填充左缓冲区
{
    memset(Buffer, 0, sizeof(Buffer) / 2);
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        if (feof(input))
        {
            charCount+=i;
            return i;
        }
        fscanf(input, "%c", &Buffer[i]);
    }
    charCount+=BUFFER_SIZE;
    return BUFFER_SIZE;
}
int ScanRbuffer(void)//填充右缓冲区
{
    memset(Buffer + BUFFER_SIZE, 0, sizeof(Buffer) / 2);
    for (int i = BUFFER_SIZE; i < BUFFER_SIZE * 2; i++)
    {
        if (feof(input))
        {
            charCount+=i-BUFFER_SIZE;
            return i;
        }
        fscanf(input, "%c", &Buffer[i]);
    }
    charCount+=BUFFER_SIZE;
    return BUFFER_SIZE * 2;
}
void Analysis(void)//词法分析DFA
{
    Init();
    int state = 0;
    int i = 0;
    int countL = ScanLbuffer();
    int countR = ScanRbuffer();
    int count = countL;
    bool find;
    bool isMin = false;
    while (i <= count)
    {
        switch (state)
        {
            case 0://start
            {
                if (Buffer[i] == ' ' || Buffer[i] == '\t' || Buffer[i] == '\n')
                {
                    charCount--;
                    if (Buffer[i] == '\n')lineCount++;
                }
                else
                {
                    if (isalpha(Buffer[i])||Buffer[i]=='_')//id
                    {
                        state = 1;
                        token[tokenIndex++] = Buffer[i];
                    }
                    else if (isdigit(Buffer[i]))//unsigned num
                    {
                        token[tokenIndex++] = Buffer[i];
                        if(Buffer[i] == '0')
                        {
                            state = 14;
                        }
                        else
                            state = 2;
                    }
                    else//op & delimiter
                    {
                        state = 3;
                        token[tokenIndex++] = Buffer[i];
                        isMin = true;
                    }
                }
                break;
            }
            case 1://id
            {
                if (isalnum(Buffer[i]))
                {
                    token[tokenIndex++] = Buffer[i];
                }
                else
                {
                    TokenPush();
                    state = 0;
                    isMin = true;
                }

                break;
            }
            case 2://num
            {
                if (isdigit(Buffer[i]))
                    token[tokenIndex++] = Buffer[i];
                else if (Buffer[i] == 'E')
                {
                    token[tokenIndex++] = Buffer[i];
                    state = 6;
                }
                else if (Buffer[i] == '.')
                {
                    token[tokenIndex++] = Buffer[i];
                    state = 7;
                }

                else//end
                {
                    state = 0;
                    isMin = true;
                    NUMTokenPush();
                }
                break;
            }
            case 3:
            {
                switch (Buffer[i])
                {
                    case '+':
                    case '-':
                    case '*':
                    case '/':
                    case '<':
                    case '>':
                    case '%':
                    case '|':
                    case '&':
                    case '^':
                    case '!':
                    case '=':
                    {
                        state = 4;
                        break;
                    }
                    case '(':
                    case ')':
                    case '~':
                    {
                        state = 0;
                        token[tokenIndex] = '\0';
                        string t = token;
                        int pos = OPFind(t.substr(0, 1), find);
                        LIST.push_back(make_pair("op", pos));
                        printf("<%s, op>\n", t.substr(0, 1).c_str());
                        ResetTokenArray();
                        break;
                    }
                    case ',':
                    case ';':
                    case '{':
                    case '}':
                    case '[':
                    case ']':
                    {
                        state = 0;
                        DelimiterTokenPush();
                        break;
                    }
                    case '"':
                    {
                        state = 11;
                        ResetTokenArray();
                        break;
                    }
                    case '\'':
                    {
                        state = 12;
                        ResetTokenArray();
                        break;
                    }
                    default:
                    {
                        //error deal
                        if(token[0]=='\0')break;
                        else
                        {
                            state = 0;
                            token[tokenIndex] = '\0';
                            string t = token;
                            printf("LINE %d ERROR: Token<%s> non-existent!\n",lineCount, t.c_str());
                            char temp[100];
                            sprintf(temp,"LINE %d ERROR: Token<%s> non-existent!\n", lineCount, t.c_str());
                            string er=temp;
                            ERROR.push_back(er);
                            ResetTokenArray();
                        }
                        break;
                    }
                }
                break;
            }
            case 4://operator single
            {
                if (isalnum(Buffer[i]))
                {
                    state = 0;
                    isMin = true;
                    OPTokenPush();
                    break;
                }

                if (Buffer[i] == '=')//second operator char += -= *= /= %= == <= >= != ^= &= |=
                {
                    state = 0;
                    token[tokenIndex++] = Buffer[i];
                    OPTokenPush();
                }
                else if (token[0] == '+' || token[0] == '-' || token[0] == '|' || token[0] == '&'
                         || token[0] == '<'|| token[0] == '>')// ++ -- || && << >>
                {
                    if (token[0] == Buffer[i])
                    {
                        state = 0;
                        token[tokenIndex++] = Buffer[i];
                        OPTokenPush();
                    }
                    else
                    {
                        state = 0;
                        OPTokenPush();
                        isMin=true;
                    }
                }
                else if (token[0] == '/'&&token[0] == Buffer[i])// comment op "//"
                {
                    state = 5;
                    token[tokenIndex++] = Buffer[i];
                    DelimiterTokenPush();
                }
                else //+, +>
                {
                    state = 0;
                    isMin = true;
                    OPTokenPush();
                }
                break;
            }
            case 5://comment string
            {
                if (Buffer[i] != '\n')
                {
                    token[tokenIndex++] = Buffer[i];
                }
                else
                {
                    lineCount++;
                    StringTokenPush();
                    state = 0;
                }
                break;
            }
            case 6://'E'
            {
                if (Buffer[i] == '+' || Buffer[i] == '-')
                {
                    token[tokenIndex++] = Buffer[i];
                    state = 8;
                }
                else if (isdigit(Buffer[i]))
                {
                    token[tokenIndex++] = Buffer[i];
                    state = 9;
                }
                else//error
                {
                    isMin = true;
                    state = 0;
                    NumErrorPush();
                }
                break;
            }
            case 7://'.'
            {
                if (isdigit(Buffer[i]))
                {
                    state = 10;
                    token[tokenIndex++] = Buffer[i];
                }
                else//error
                {
                    state = 0;
                    isMin = true;
                    token[tokenIndex++] = Buffer[i];
                    NumErrorPush();
                }
                break;
            }
            case 8:
            {
                if (isdigit(Buffer[i]))
                {
                    token[tokenIndex++] = Buffer[i];
                    state = 9;
                }
                else//error
                {
                    state = 0;
                    isMin = true;
                    //token[tokenIndex++] = Buffer[i];
                    NumErrorPush();
                }
                break;
            }
            case 9:
            {
                if (isdigit(Buffer[i]))
                {
                    token[tokenIndex++] = Buffer[i];
                }
                else//num end
                {
                    state = 0;
                    isMin = true;
                    NUMTokenPush();
                }
                break;
            }
            case 10:
            {
                if (isdigit(Buffer[i]))
                {
                    token[tokenIndex++] = Buffer[i];
                }
                else if (Buffer[i] == 'E')
                {
                    token[tokenIndex++] = Buffer[i];
                    state = 6;
                }
                else//num end
                {
                    state = 0;
                    isMin = true;
                    NUMTokenPush();
                }
                break;
            }
            case 11:// ""string
            {
                if (Buffer[i] == '\n')
                {

                    //error
                    state = 0;
                    token[tokenIndex] = '\0';
                    string t = token;
                    char e[140];
                    sprintf(e,"LINE %d ERROR: Token<%s> string illegal!\n", lineCount, t.c_str());
                    string er=e;
                    ERROR.push_back(er);
                    ResetTokenArray();
                    lineCount++;
                }
                else if (Buffer[i] == '"')
                {
                    state = 0;

                    token[tokenIndex] = '\0';
                    string t = token;       //save string

                    ResetTokenArray();
                    token[tokenIndex++] = '"';
                    DelimiterTokenPush();

                    STRING.push_back(t);
                    LIST.push_back(make_pair("string", STRING.size()-1));
                    printf("<%s, string>\n", t.c_str());
                    ResetTokenArray();

                    token[tokenIndex++] = '"';
                    DelimiterTokenPush();
                }
                else
                {
                    token[tokenIndex++] = Buffer[i];
                }
                break;
            }
            case 12://''char
            {
                if (Buffer[i] == '\n')
                {

                    state = 0;
                    token[tokenIndex] = '\0';
                    string t = token;
                    printf("LINE %d ERROR: Token<'> dismiss!\n", lineCount);
                    char temp[100];
                    sprintf(temp,"LINE %d ERROR: Token<'> dismiss!\n", lineCount);
                    string er=temp;
                    ERROR.push_back(er);
                    ResetTokenArray();
                    lineCount++;
                    break;
                }
                token[tokenIndex++] = Buffer[i];
                state = 13;
                break;
            }
            case 13:
            {
                if (Buffer[i] == '\'')
                {
                    state = 0;

                    token[tokenIndex] = '\0';
                    string t = token;

                    ResetTokenArray();
                    token[tokenIndex++] = '\'';
                    DelimiterTokenPush();

                    STRING.push_back(t);
                    LIST.push_back(make_pair("string", STRING.size()-1));
                    printf("<%s, char>\n", t.c_str());
                    ResetTokenArray();

                    token[tokenIndex++] = '\'';
                    DelimiterTokenPush();
                }
                else
                {
                    state = 0;
                    isMin = true;
                    token[tokenIndex] = '\0';
                    string t = token;
                    printf("LINE %d ERROR: Token<%s> char illegal!\n", lineCount, t.c_str());
                    char temp[100];
                    sprintf(temp,"LINE %d ERROR: Token<%s> char illegal!\n", lineCount, t.c_str());
                    string er=temp;
                    ERROR.push_back(er);
                    ResetTokenArray();
                }
                break;
            }
            case 14://hex num
            {
                if(Buffer[i] == 'x'||Buffer[i] == 'X')
                {
                    token[tokenIndex++] = Buffer[i];
                    state = 15;
                }
                else
                {
                    isMin = true;
                    state = 2;
                }
                break;
            }
            case 15:
            {
                token[tokenIndex++] = Buffer[i];
                if(isdigit(Buffer[i]) || (Buffer[i] >= 'a' && Buffer[i] <= 'f')
                        || (Buffer[i] >= 'A' && Buffer[i] <= 'F'))
                {        
                    state = 16;
                }
                else
                {
                    state = 0;
                   // isMin = true;
                    HexErrorPush();
                }
                break;
            }
            case 16:
            {
                if(isdigit(Buffer[i]) || (Buffer[i] >= 'a' && Buffer[i] <= 'f')
                        || (Buffer[i] >= 'A' && Buffer[i] <= 'F'))
                {
                    token[tokenIndex++] = Buffer[i];
                }
                else
                {
                    state = 0;
                    isMin = true;
                    NUMTokenPush();
                }
                break;
            }
            default:
                break;
        }
        i++;
        //charCount++;
        if (!isMin)
        {
            if (i == countL)
            {
                countL = ScanLbuffer();
                count = countR;
            }
            else if (i == countR)
            {
                countR = ScanRbuffer();
                count = countL;
                i = 0;
            }
        }
        else
        {
            i--;
            //charCount--;
            isMin = false;
        }

    }

    if (state == 5 && tokenIndex > 0)
    {
        StringTokenPush();
    }
    Output();
    fclose(input);
}
