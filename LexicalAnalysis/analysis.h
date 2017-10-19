#ifndef ANALYSIS_H
#define ANALYSIS_H
#include<cctype>
#include<vector>
#include<qstring.h>
using namespace std;
extern vector<pair<string, int>> LIST;
extern vector<string> KEY;
extern vector<string> ID;
extern vector<string> DELIMITER;
extern vector<string> OP;
extern vector<string> STRING;
extern vector<string> NUM;
extern vector<string> ERROR;
extern void Analysis(void);
extern int lineCount;
extern int charCount;
extern QString Path;
#endif // ANALYSIS_H
