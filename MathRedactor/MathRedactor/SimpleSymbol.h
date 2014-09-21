//�����: ��������� ����

#pragma once

#include "Symbol.h"

//������� ������ (�����, ����� ��� ������� ��������). ��� ������������ ��-���������.
class CSimpleSymbol : public CSymbol {
public:
	CSimpleSymbol( wchar_t _symbol ) { symbol[0] = _symbol; }

	//�������� ����� (deep) �������
	virtual CSymbol* Clone() const { return new CSimpleSymbol( *this ); }

	wchar_t GetSymbol() const { return symbol[0]; }
	void SetSymbol( wchar_t _symbol ) { symbol[0] = _symbol;  }

	//�������� ���� ������� ��������� � Symbol.h
	virtual void Draw( HDC displayHandle, int posX, int posY, int realHeight, int simpleSymbolHeight ) const;
	virtual int CalculateWidth( HDC displayHandle, int simpleSymbolHeight ) const;

private:
	//��� ������� (���������� � TextOut).
	wchar_t symbol[1];
};