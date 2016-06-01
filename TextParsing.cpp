#include "TText.h"
TTextMem TLink::TextMem;

void main()
{
	setlocale(LC_ALL, "Russian");
	TLink::InitMem(100);
	TText txt;
	txt.Load("text.txt");
	txt.StrChoice();
	TLink::PrintFree();
	TLink::MemClean(txt);
	TLink::PrintFree();
}