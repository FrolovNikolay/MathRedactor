#include "LineOfSymbols.h"
#include "FractionSymbol.h"
#include "SimpleSymbol.h"
#include <assert.h>
#include <typeinfo>

CLineOfSymbols::CLineOfSymbols( const CLineOfSymbols& src )
{
	hasFraction = src.hasFraction;

	for( int i = 0; i < src.arrayOfSymbolPtrs.size(); ++i ) {
		CSymbol* symbol = src.arrayOfSymbolPtrs[i]->Clone();
		assert( symbol != 0 );
		arrayOfSymbolPtrs.push_back( symbol );
	}
}

CLineOfSymbols::~CLineOfSymbols()
{
	for( int i = 0; i < arrayOfSymbolPtrs.size(); ++i ) {
		delete arrayOfSymbolPtrs[i];
	}
}

//���������� ������� � �����. ������, ���������� �� ���������, ����������� ������.
void CLineOfSymbols::Push( CSymbol* symbol )
{
	if( typeid(*symbol) == typeid(CFractionSymbol) ) {
		hasFraction = true;
	}

	arrayOfSymbolPtrs.push_back( symbol );
}

void CLineOfSymbols::Draw( HDC displayHandle, int posX, int posY, int realHeight ) const
{
	//TODO: ������� �������� �� ��� ������, ���� ������ ���������� ������� ���������
	int simpleSymbolHeight = realHeight;
	if( hasFraction ) {
		simpleSymbolHeight = (realHeight / 2) - 1;
	}

	//������������� ����� (�������� ������� � ��������� ������ �������)
	HFONT oldFont = (HFONT)::GetCurrentObject( displayHandle, OBJ_FONT );
	assert( oldFont != 0 );

	LOGFONT fontInfo;
	::GetObject( oldFont, sizeof(LOGFONT), &fontInfo );
	fontInfo.lfHeight = simpleSymbolHeight;
	HFONT font = ::CreateFontIndirect( &fontInfo );
	assert( font != 0 );
	oldFont = (HFONT)::SelectObject( displayHandle, font );

	//���������
	for( int i = 0; i < arrayOfSymbolPtrs.size(); ++i ) {
		assert( arrayOfSymbolPtrs[i] != 0 );
		arrayOfSymbolPtrs[i]->Draw( displayHandle, posX, posY, realHeight, simpleSymbolHeight );
		posX += arrayOfSymbolPtrs[i]->CalculateWidth( displayHandle, simpleSymbolHeight );
	}

	//���������� ������ �����, ������� ���������
	::SelectObject( displayHandle, oldFont );
	::DeleteObject( font );
}

int CLineOfSymbols::CalculateWidth( HDC displayHandle, int realHeight ) const
{
	int simpleSymbolHeight = realHeight;
	if( hasFraction ) {
		simpleSymbolHeight = (realHeight / 2) - 1;
	}
	//������������� ����� (�������� ������� � ��������� ������ �������)
	HFONT oldFont = (HFONT)::GetCurrentObject( displayHandle, OBJ_FONT );
	assert( oldFont != 0 );

	LOGFONT fontInfo;
	::GetObject( oldFont, sizeof(LOGFONT), &fontInfo );
	fontInfo.lfHeight = simpleSymbolHeight;
	HFONT font = ::CreateFontIndirect( &fontInfo );
	assert( font != 0 );
	oldFont = (HFONT)::SelectObject( displayHandle, font );

	int result = 0;
	for( int i = 0; i < arrayOfSymbolPtrs.size( ); ++i ) {
		assert( arrayOfSymbolPtrs[i] != 0 );
		result += arrayOfSymbolPtrs[i]->CalculateWidth( displayHandle, simpleSymbolHeight );
	}

	//���������� ������ �����, ������� ���������
	::SelectObject( displayHandle, oldFont );
	::DeleteObject( font );

	return result;
}