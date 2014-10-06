//�����: ��������� ����

#pragma once

#include "Symbol.h"
#include "LineOfSymbols.h" 

//������� ������ "������". ������� �� ���� ����� (������� � ����������� ���������)
class CRootSymbol : public CSymbol {
public:

	CRootSymbol( int simpleSymbolHeight );

	//�������� ����� (deep) �������. 
	virtual CSymbol* Clone( ) const { return new CRootSymbol( *this ); }

	virtual int GetHeight( int simpleSymbolHeight ) const;
	virtual int GetBaselineOffset( int simpleSymbolHeight ) const;

	CLineOfSymbols& GetExponentLine( ) { return exponentLine; }
	const CLineOfSymbols& GetExponentLine( ) const { return exponentLine; }
	CLineOfSymbols& GetRadicandLine( ) { return radicandLine; }
	const CLineOfSymbols& GetRadicandLine( ) const { return radicandLine; }

	//�������� ���� ������� ��������� � Symbol.h
	virtual void Draw( HDC displayHandle, int posX, int posY, int simpleSymbolHeight ) const;
	virtual int CalculateWidth( HDC displayHandle ) const;

private:
	//������ ������� � ����������� ���������
	CLineOfSymbols exponentLine;
	CLineOfSymbols radicandLine;

	//������ ����� ��� TextOut
	static const wchar_t symbol[];

	//���������� ������ ������ �������
	static int getExponentHeight( int simpleSymbolHeight )
		{ return static_cast<int>( simpleSymbolHeight * 0.5 ); }
	//������ ���������� ������� �����
	static int getRootHeight( int simpleSymbolHeight )
	{
		return static_cast<int>( simpleSymbolHeight * 1.6 );
	}
	//����� �� ����������� ���������� �������
	static int getExponentOffset( int rootWidth )
	{
		return -static_cast<int>( rootWidth * 0.4 );
	}
	static int getExponentVerticalOffset( int rootHeight )
	{
		return static_cast<int>( rootHeight * 0.4 );
	}
};