#ifndef TTEXT_H
#define TTEXT_H	// препроцессорная обёртка
#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include "Stack.h"
#include "TLink.h"

using namespace std;

class TLink;

class TText
{
private:
	TStack<TLink *> path;
	TLink *pFirst, *pCurrent;
	TLink* ReadSection(ifstream &);
	void PrintSection(TLink *);
	void SaveSection(TLink *, ofstream &);
public:
	TText(TLink *pointer = NULL);
	~TText() {};
	void Reset();
	void GoNext();
	bool IsEnd();
	void GoNextLink();
	void GoDownLink();
	void GoPrevLink();
	void GoFirstLink();
	void InsNextLine(string);
	void InsNextSection(string);
	void InsDownLine(string);
	void InsDownSection(string);
	void DelNextLine();
	void DelDownLine();
	string GetLine();
	void SetLine(string);
	void Load(string);
	void Print();
	void Save(string);
	void StrChoice();
};

#endif TTEXT_H