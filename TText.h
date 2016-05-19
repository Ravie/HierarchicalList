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
	int GoNextLink();
	int GoDownLink();
	int GoPrevLink();
	int GoFirstLink();
	int InsNextLine(char *);
	int InsNextSection(char *);
	void InsDownLine(char *);
	void InsDownSection(char *);
	void DelNextLine();
	void DelDownLine();
	char *GetLine();
	void SetLine(char *);
	void Load(char *);
	void Print();
};