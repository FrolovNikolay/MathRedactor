﻿#include "LineOfSymbols.h"
#include "SimpleSymbol.h"
#include <assert.h>
#include <typeinfo>

CLineOfSymbols::CLineOfSymbols( int _simpleSymbolHeight ) :
	height( _simpleSymbolHeight ),
	baselineOffset( 0 ),
	simpleSymbolHeight( _simpleSymbolHeight )
{}



CLineOfSymbols::CLineOfSymbols( const CLineOfSymbols& src ) :
	height( src.height ),
	baselineOffset( src.baselineOffset ),
	simpleSymbolHeight( src.simpleSymbolHeight )
{

	for( int i = 0; i < src.Length(); ++i ) {
		assert( src[i] != 0 );
		Push( src[i]->Clone( ) );
	}
}

CLineOfSymbols::~CLineOfSymbols( )
{
	for( int i = 0; i < arrayOfSymbolPtrs.size( ); ++i ) {
		delete arrayOfSymbolPtrs[i];
	}
}

//Добавление символа в конец. Память, переданная по указателю, принадлежит классу.
void CLineOfSymbols::Push( CSymbol* symbol )
{
	arrayOfSymbolPtrs.push_back( symbol );
	height = max( height, symbol->GetHeight( simpleSymbolHeight ) );
	baselineOffset = max( baselineOffset, symbol->GetBaselineOffset( simpleSymbolHeight ) );
}

void CLineOfSymbols::Draw( HDC displayHandle, int posX, int posY ) const
{

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
	for( int i = 0; i < arrayOfSymbolPtrs.size( ); ++i ) {
		assert( arrayOfSymbolPtrs[i] != 0 );
		arrayOfSymbolPtrs[i]->Draw( displayHandle, posX, posY + baselineOffset, simpleSymbolHeight );
		posX += arrayOfSymbolPtrs[i]->CalculateWidth( displayHandle );
	}

	//Возвращаем старый шрифт, удаляем созданный
	::SelectObject( displayHandle, oldFont );
	::DeleteObject( font );
}

int CLineOfSymbols::CalculateWidth( HDC displayHandle ) const
{
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
		result += arrayOfSymbolPtrs[i]->CalculateWidth( displayHandle );
	}

	//Возвращаем старый шрифт, удаляем созданный
	::SelectObject( displayHandle, oldFont );
	::DeleteObject( font );

	return result;
}

void CLineOfSymbols::recalculate()
{
	height = simpleSymbolHeight;
	baselineOffset = 0;
	for( int i = 0; i < Length(); ++i ) {
		height = max( height, arrayOfSymbolPtrs[i]->GetHeight( simpleSymbolHeight ) );
		baselineOffset = max( baselineOffset, arrayOfSymbolPtrs[i]->GetBaselineOffset( simpleSymbolHeight ) );
	}
}