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

//Добавление символа в конец. Память, переданная по указателю, принадлежит классу.
void CLineOfSymbols::Push( CSymbol* symbol )
{
	if( typeid(*symbol) == typeid(CFractionSymbol) ) {
		hasFraction = true;
	}

	arrayOfSymbolPtrs.push_back( symbol );
}

// Удаление символа из конца строки, при этом символ удаляется
void CLineOfSymbols::Pop() 
{
	if( arrayOfSymbolPtrs.empty() ) {
		return;
	}

	CSymbol* toDelete =  arrayOfSymbolPtrs.back();
	arrayOfSymbolPtrs.resize( arrayOfSymbolPtrs.size() - 1 );
	delete( toDelete );
	hasFraction = false;
	for( int i = 0; i < arrayOfSymbolPtrs.size(); ++i ) {
		if( typeid( *arrayOfSymbolPtrs[i] ) == typeid( CFractionSymbol ) ) {
			hasFraction = true;
			break;
		}
	}
}

int CLineOfSymbols::Length() const
{
	return arrayOfSymbolPtrs.size();
}

void CLineOfSymbols::Draw( HDC displayHandle, int posX, int posY, int realHeight ) const
{
	//TODO: Сделать проверку на тот случай, если высота получилась слишком маленькой
	int simpleSymbolHeight = realHeight;
	if( hasFraction ) {
		simpleSymbolHeight = (realHeight / 2) - 1;
	}

	//Устанавливаем шрифт (получаем текущий и обновляем высоту символа)
	HFONT oldFont = (HFONT)::GetCurrentObject( displayHandle, OBJ_FONT );
	assert( oldFont != 0 );

	LOGFONT fontInfo;
	::GetObject( oldFont, sizeof(LOGFONT), &fontInfo );
	fontInfo.lfHeight = simpleSymbolHeight;
	HFONT font = ::CreateFontIndirect( &fontInfo );
	assert( font != 0 );
	oldFont = (HFONT)::SelectObject( displayHandle, font );

	//Отрисовка
	for( int i = 0; i < arrayOfSymbolPtrs.size(); ++i ) {
		assert( arrayOfSymbolPtrs[i] != 0 );
		arrayOfSymbolPtrs[i]->Draw( displayHandle, posX, posY, realHeight, simpleSymbolHeight );
		posX += arrayOfSymbolPtrs[i]->CalculateWidth( displayHandle, simpleSymbolHeight );
	}

	//Возвращаем старый шрифт, удаляем созданный
	::SelectObject( displayHandle, oldFont );
	::DeleteObject( font );
}

int CLineOfSymbols::CalculateWidth( HDC displayHandle, int realHeight ) const
{
	int simpleSymbolHeight = realHeight;
	if( hasFraction ) {
		simpleSymbolHeight = (realHeight / 2) - 1;
	}
	//Устанавливаем шрифт (получаем текущий и обновляем высоту символа)
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

	//Возвращаем старый шрифт, удаляем созданный
	::SelectObject( displayHandle, oldFont );
	::DeleteObject( font );

	return result;
}