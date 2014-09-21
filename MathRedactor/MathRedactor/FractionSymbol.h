//�����: ��������� ����

#pragma once

#include "Symbol.h"
#include "LineOfSymbols.h"

//������� ������ "�����". ������� �� ���� ����� (LineOfSymbols).
class CFractionSymbol : public CSymbol {
public:

	CFractionSymbol() {}

	//�������� ����� (deep) �������. 
	virtual CSymbol* Clone() const { return new CFractionSymbol( *this ); }

	CLineOfSymbols& GetUpperLine() { return upperLine; }
	const CLineOfSymbols& GetUpperLine() const { return upperLine; }
	CLineOfSymbols& GetLowerLine( ) { return lowerLine; }
	const CLineOfSymbols& GetLowerLine( ) const { return lowerLine; }

	//�������� ���� ������� ��������� � Symbol.h
	virtual void Draw( HDC displayHandle, int posX, int posY, int realHeight, int simpleSymbolHeight ) const;
	virtual int CalculateWidth( HDC displayHandle, int simpleSymbolHeight ) const;

private:
	//������� � ������ ����� �����
	CLineOfSymbols upperLine;
	CLineOfSymbols lowerLine;
};