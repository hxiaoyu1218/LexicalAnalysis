#pragma once
#include<iostream>
extern int KeyFind(std::string, bool &);
extern int IDFind(std::string, bool &);
extern int DELIMITERFind(std::string, bool &);
extern int OPFind(std::string, bool &);
extern void TokenPush(void);
extern void ResetTokenArray(void);
extern void StringTokenPush(void);
extern void DelimiterTokenPush(void);
extern void OPTokenPush(void);
extern void NUMTokenPush(void);
extern void NumErrorPush(void);
extern void HexErrorPush(void);
extern void ResetList(void);
