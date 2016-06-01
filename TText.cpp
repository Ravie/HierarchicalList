#include "TText.h"

TText::TText(TLink *pointer)
{
	if (pointer == NULL)
	{
		TLink *tmp = new TLink;
		pFirst = tmp;
	}
	else
		pFirst = pointer;
}

TLink* TText::ReadSection(ifstream &_file)
{
	string _str;
	TLink *pFirstInSection, *pTmp;
	pFirstInSection = new TLink;
	pTmp = pFirstInSection;
	while (!_file.eof())
	{
		getline(_file, _str);
		if (_str[0] == '}')
			break;
		if (_str[0] == '{')
			pTmp->pDown = ReadSection(_file);
		else
		{
			TLink *NewLink = new TLink(_str.c_str());
			pTmp->pNext = NewLink;
			pTmp = NewLink;
		}
	}
	if (pFirstInSection->pDown == NULL)
	{
		pTmp = pFirstInSection->pNext;
		delete pFirstInSection;
		pFirstInSection = pTmp;
	}
	return pFirstInSection;
}

void TText::PrintSection(TLink *pTmp)
{
	if (pTmp != NULL)
	{
		cout << pTmp->str << endl;
		if (pTmp->pDown != NULL)
		{
			cout << "{" << endl;
			PrintSection(pTmp->pDown);
			cout << "}" << endl;
		}
		PrintSection(pTmp->pNext);
	}
}

void TText::SaveSection(TLink *pTmp, ofstream &_file)
{
	if (pTmp != NULL)
	{
		_file << pTmp->str << endl;
		if (pTmp->pDown != NULL)
		{
			_file << '{' << endl;
			SaveSection(pTmp->pDown, _file);
			_file << '}' << endl;
		}
		SaveSection(pTmp->pNext, _file);
	}
}

void TText::Reset()
{
	path.ClearStack();
	pCurrent = pFirst;
	if (pCurrent != NULL)
	{
		if (pCurrent->pNext != NULL)
			path.PushElem(pCurrent->pNext);
		if (pCurrent->pDown != NULL)
			path.PushElem(pCurrent->pDown);
	}
}

void TText::GoNext()
{
	if (!path.IsEmpty())
	{
		pCurrent = path.Pop();
		if (pCurrent->pNext != NULL)
			path.PushElem(pCurrent->pNext);
		if (pCurrent->pDown != NULL)
			path.PushElem(pCurrent->pDown);
	}
}

bool TText::IsEnd()
{
	return path.IsEmpty();
}

void TText::GoNextLink()
{
	if (pCurrent != NULL)
		if (pCurrent->pNext != NULL)
		{
			path.PushElem(pCurrent);
			pCurrent = pCurrent->pNext;
		}
}

void TText::GoDownLink()
{
	if (pCurrent != NULL)
		if (pCurrent->pDown != NULL)
		{
			path.PushElem(pCurrent);
			pCurrent = pCurrent->pDown;
		}
}

void TText::GoPrevLink()
{
	if (pCurrent != NULL)
		if (!path.IsEmpty())
			pCurrent = path.Pop();
}

void TText::GoFirstLink()
{
	if (pCurrent != NULL)
		while (!path.IsEmpty())
			pCurrent = path.Pop();
	pCurrent = pFirst;
}

void TText::InsNextLine(string _str)
{
	if (pCurrent != NULL)
	{
		const string str = _str;
		TLink *tmp = new TLink(str.c_str());
		tmp->pNext = pCurrent->pNext;
		pCurrent->pNext = tmp;
	}
}

void TText::InsNextSection(string _str)
{
	if (pCurrent != NULL)
	{
		const string str = _str;
		TLink *tmp = new TLink(str.c_str());
		tmp->pDown = pCurrent->pNext;
		pCurrent->pNext = tmp;
	}
}

void TText::InsDownLine(string _str)
{
	if (pCurrent != NULL)
	{
		const string str = _str;
		TLink *tmp = new TLink(str.c_str());
		tmp->pNext = pCurrent->pDown;
		pCurrent->pDown = tmp;
	}
}

void TText::InsDownSection(string _str)
{
	if (pCurrent != NULL)
	{
		const string str = _str;
		TLink *tmp = new TLink(str.c_str());
		tmp->pDown = pCurrent->pDown;
		pCurrent->pDown = tmp;
	}
}

void TText::DelNextLine()
{
	if (pCurrent != NULL)
		if (pCurrent->pNext != NULL)
		{
			TLink* tmp = pCurrent->pNext;
			pCurrent->pNext = pCurrent->pNext->pNext;
			delete tmp;
		}
}

void TText::DelDownLine()
{
	if (pCurrent != NULL)
		if (pCurrent->pDown != NULL)
		{
			TLink* tmp = pCurrent->pDown;
			pCurrent->pDown = pCurrent->pDown->pNext;
			delete tmp;
		}
}

string TText::GetLine()
{
	char _str[MaxLen];
	if (pCurrent != NULL)
		strcpy(_str, pCurrent->str);
	return _str;
}

void TText::SetLine(string _str)
{
	const string s = _str;
	if (pCurrent != NULL)
		strcpy(pCurrent->str, s.c_str());
}

void TText::Load(string _str)
{
	ifstream _file(_str);
	pFirst = ReadSection(_file);
	pCurrent = pFirst;
}

void TText::Print()
{
	PrintSection(pFirst);
}

void TText::Save(string f_name)
{
	ofstream ofs(f_name);
	SaveSection(pFirst, ofs);
}

void TText::StrChoice()
{
	int k;
	do
	{
		system("cls");
		string tmp = "-> ";
		tmp += GetLine();
		SetLine((char *)tmp.c_str());
		Print();
		SetLine(pCurrent->str + 3);
		cout << endl;
		cout << "Выберите способ перехода по строкам: \n";
		cout << "1) Переход в начало \n";
		cout << "2) Переход к следующей секции \n";
		cout << "3) Переход к предыдущей секции \n";
		cout << "4) Переход к следующей строке \n";
		cout << "5) Выход \n";
		cin >> k;
		switch (k)
		{
		case 1:
			GoFirstLink();
			break;
		case 2:
			GoNextLink();
			break;
		case 3:
			GoPrevLink();
			break;
		case 4:
			GoDownLink();
			break;
		case 5:
			break;
		}
	} while (k != 5);
}