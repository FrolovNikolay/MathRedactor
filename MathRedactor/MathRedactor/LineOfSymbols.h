//�����: ��������� ����

#pragma once

#include <Windows.h>
#include <vector>
#include "Symbol.h"

//����� ������������ ����� ������ ��������. ����� ������������ ���� � hdc.
class CLineOfSymbols {
public:
	CLineOfSymbols() { hasFraction = false; }
	CLineOfSymbols( const CLineOfSymbols& src );
	~CLineOfSymbols();

	//���������� ������ � Device Context, ������� � ������� (startX, startY) - ����� ������� ���� ������.
	//realHeight - �������� ������ ������ (��� �� ������� �� ������� ����� ��������)
	void Draw( HDC displayHandle, int posX, int posY, int realHeight ) const;
	//���������� ������ ������, ��������������� �������� ������ (�������� ������ != ������ �������� �������)
	int CalculateWidth( HDC displayHandle, int realHeight ) const;

	//�������� ������ � ����� ������ (������, ���������� �� ���������, ����� ����������� � ����������� ������ CLineOfSymbols)
	void Push( CSymbol* symbol );

	//TODO: ���������� ��� ������
	const CSymbol* operator[] ( int index ) const;
	CSymbol* operator[] ( int index );

private:
	std::vector<CSymbol*> arrayOfSymbolPtrs;
	//���� ������� ����� (�� ����� ������� simpleSymbolHeight)
	bool hasFraction;

};