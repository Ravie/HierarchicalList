#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>

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

	TLink(char *, TLink *, TLink *);
	~TLink() {};
	void *operator new(size_t);
	void operator delete(void *);
	static void InitMem(size_t);
	static void MemClean(TText &);
	static void PrintFree();
};