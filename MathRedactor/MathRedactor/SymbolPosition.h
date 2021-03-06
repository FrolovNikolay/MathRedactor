﻿// Автор: Фролов Николай.

// Описание: Структура, отвечающая за хранение позиции во внутренней структуре редактора.

#pragma once

#include "LineOfSymbols.h"
#include <memory>

struct CSymbolPosition {
public:
	// Позиция в строке.
	int Index;
	// Строка, в которой находимся.
	const CLineOfSymbols* CurrentLine;

	CSymbolPosition( int _Index, const CLineOfSymbols* _CurrentLine, const CSymbolPosition* _parentPosition = 0 ) 
		: Index( _Index )
		, CurrentLine( _CurrentLine )
		, parentPosition( _parentPosition )
	{ }

	CSymbolPosition( int _Index, const CSymbolPosition& src )
		: Index( _Index )
		, CurrentLine( src.CurrentLine )
		, parentPosition( src.parentPosition )
	{ }

	CSymbolPosition( const CSymbolPosition& src )
		: Index( src.Index )
		, CurrentLine( src.CurrentLine )
		, parentPosition( src.parentPosition )
	{ }

	CSymbolPosition& operator = ( const CSymbolPosition& src )
	{
		if( this == &src ) {
			return *this;
		}
		CSymbolPosition tmp( src );

		std::swap( Index, tmp.Index );
		std::swap( CurrentLine, tmp.CurrentLine );
		std::swap( parentPosition, tmp.parentPosition );
		return *this;
	}

	~CSymbolPosition()
	{ }

	const CSymbolPosition* GetParent() const { return parentPosition.get(); };
private:
	// 0, если находимся в базовой строке.
	std::shared_ptr<const CSymbolPosition> parentPosition;
};