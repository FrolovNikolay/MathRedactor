//�����: ��������� ����

#pragma once

#include "Symbol.h"
#include "LineOfSymbols.h" 

//������� ������ "�����". ������� �� ���� ����� (������� � ������)
class CSigmaSymbol : public CSymbol {
public:

	CSigmaSymbol( int simpleSymbolHeight );

	//�������� ����� (deep) �������. 
	virtual CSymbol* Clone( ) const { return new CSigmaSymbol( *this ); }

	virtual int GetHeight( int simpleSymbolHeight ) const;
	virtual int GetBaselineOffset( int simpleSymbolHeight ) const;

	CLineOfSymbols& GetUpperLine() { return upperLine; }
	const CLineOfSymbols& GetUpperLine() const { return upperLine; }
	CLineOfSymbols& GetLowerLine() { return lowerLine; }
	const CLineOfSymbols& GetLowerLine() const { return lowerLine; }

	//�������� ���� ������� ��������� � Symbol.h
	virtual void Draw( HDC displayHandle, int posX, int posY, int simpleSymbolHeight ) const;
	virtual int CalculateWidth( HDC displayHandle ) const;

private:
	//������ ��� ������ � ��� ������
	CLineOfSymbols upperLine;
	CLineOfSymbols lowerLine;

	//������ ����� ��� TextOut
	static const wchar_t symbol[];

	//���������� ������ ������ � ������� ������
	static int calculateSublineHeight( int simpleSymbolHeight ) 
		{ return static_cast<int>( simpleSymbolHeight * 0.7 ); }
	//������ ������� �����
	static int getSigmaHeight( int simpleSymbolHeight ) 
		{ return static_cast<int>( simpleSymbolHeight * 1.5 ); }
	//����� ������� � ������ ����� ������������ �����
	static int getSublinesOffset() { return 1; }
};