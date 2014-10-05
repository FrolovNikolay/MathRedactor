// Автор: Федюнин Валерий

#include "FractionSymbol.h"
#include "SimpleSymbol.h"
#include "SigmaSymbol.h"
#include "CommandHandler.h"
#include "RibbonIDs.h"

CCommandHandler::CCommandHandler( CEditWindow* _editWindow) {
	editWindow = _editWindow;
	referenceCount = 1;
}

STDMETHODIMP_( ULONG ) CCommandHandler::AddRef()
{
	return ::InterlockedIncrement( &referenceCount );
}

STDMETHODIMP_( ULONG ) CCommandHandler::Release()
{
	return ::InterlockedDecrement( &referenceCount );
}

STDMETHODIMP CCommandHandler::QueryInterface( REFIID type, void** res )
{
	if( type == __uuidof( IUnknown ) ) {
		*res = static_cast< IUnknown* >( this );
	} else if( type == __uuidof( IUICommandHandler ) ) {
		*res = static_cast< IUICommandHandler* >( this );
	} else {
		*res = 0;
		return E_NOINTERFACE;
	}

	AddRef();
	return S_OK;
}

STDMETHODIMP CCommandHandler::UpdateProperty( UINT nCmdId, REFPROPERTYKEY key, const PROPVARIANT* propvarCurrentValue,
	PROPVARIANT* propvarNewValue )
{
	return E_NOTIMPL;
}

STDMETHODIMP CCommandHandler::Execute( UINT nCmdId, UI_EXECUTIONVERB verb, const PROPERTYKEY* key,
	const PROPVARIANT* propvarValue, IUISimplePropertySet* pCommandExecutionProperties )
{
	switch( nCmdId ) {
	case ID_CMD_SIGMA:
	{
		CSigmaSymbol* sigma = new CSigmaSymbol( editWindow->GetSimpleSymbolHeight() );
		sigma->GetUpperLine().Push( new CSimpleSymbol( L'9' ), 0 );
		sigma->GetLowerLine().Push( new CSimpleSymbol( L'1' ), 0 );
		editWindow->AddSymbol( sigma );
		break;
	}
	case ID_CMD_FRACTION:
		CFractionSymbol* fraction = new CFractionSymbol( editWindow->GetSimpleSymbolHeight() );
		fraction->GetUpperLine().Push( new CSimpleSymbol( L'1' ), 0 );
		fraction->GetLowerLine().Push( new CSimpleSymbol( L'2' ), 0 );
		editWindow->AddSymbol( fraction );
		break;
	}

	return S_OK;
}