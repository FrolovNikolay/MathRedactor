//�����: ��������� ����

#pragma once

#include <Windows.h>

//����������� ����� ������� (�������� � ��������). ����� �������� ���� � Device Context
class CSymbol {
public:
	virtual ~CSymbol() {}

	//�������� ����� (deep) �������
	virtual CSymbol* Clone() const = 0;

	//���������� ������ � displayHandle, ������� � ������� (startX, startY) - ����� ������� ����. ������������
	//������� FONT � PEN. realHeight - �������� ������ ������ (������� ������� �������). simpleSymbolHeight - ������
	//�������� ������� (�����, �����).
	virtual void Draw( HDC displayHandle, int posX, int posY, int realHeight, int simpleSymbolHeight ) const = 0;
	//���������� ������ �������, ��������������� ������ �������� �������
	virtual int CalculateWidth( HDC displayHandle, int simpleSymbolHeight ) const = 0;
};