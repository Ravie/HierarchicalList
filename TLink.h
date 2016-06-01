#ifndef TLINK_H
#define TLINK_H	// препроцессорная обёртка

#include <iostream>
#include <string>
#include "TText.h"

using namespace std;

class TText;
class TLink;
struct TTextMem { TLink *pFirst, *pLast, *pFree; };

const int MaxLen = 81;

class TLink
{
public:
	char str[MaxLen];
	TLink *pNext, *pDown;
	static TTextMem TextMem;

	TLink(const char *_str = NULL, TLink *_pNext = NULL, TLink *_pDown = NULL);
	~TLink() {};
	void *operator new(size_t);
	void operator delete(void *);
	static void InitMem(size_t);
	static void MemClean(TText &);
	static void PrintFree();
};

#endif TLINK_H