#include "TText.h"
TTextMem TLink::TextMem;

void main()
{
	TLink::InitMem(100);
	TText txt;
	txt.Load("text.txt");
	TLink::PrintFree();
	TLink::MemClean(txt);
	TLink::PrintFree();
}