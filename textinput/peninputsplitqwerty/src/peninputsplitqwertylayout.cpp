/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  split qwerty layout
*
*/


// System includes
#include <aknfeppeninputenums.h>    // Command from fep or IME to plugin
#include <AknFepGlobalEnums.h>      // Define EPluginInputModeHwr
#include <peninputcmd.h>            // Use global signal
#include <peninputlayoutchoicelist.h>
#include <peninputlayoutbutton.h>
#include <peninputdragbutton.h>
#include <peninputcommonlayoutglobalenum.h>
#include <coemain.h>
#include <peninputlayoutbubblectrl.h>
#include <peninputlayoutmultilineicf.h>
#include <peninputscrollablelist.h>
#include <s32mem.h>

#include <peninputrangebar.h>
#include <peninputcmdparam.h>

// User includes
#include "peninputsplitqwerty.hrh"
#include "peninputuistatemgrinterface.h"
#include "peninputsplitqwertylayout.h"
#include "peninputsplitqwertywindow.h"
#include "peninputsplitqwertydatamgr.h"
#include "peninputsplitqwertyuistatestandby.h"
#include "peninputsplitqwertyuistatepopup.h"
#include "peninputdataconverter.h"

// ======== MEMBER FUNCTIONS ========
const TInt KNumberOfDottedChar = 16; 

const TUint16 thaiUnicodeOri[KNumberOfDottedChar] = 
    {
    0x0E31,0x0E33,0x0E34,0x0E35,0x0E36,0x0E37,0x0E38,0x0E39, 
    0x0E3A,0x0E47,0x0E48,0x0E49,0x0E4A,0x0E4B,0x0E4C,0x0E4D
    };

const TUint16 thaiUnicodeNew[KNumberOfDottedChar] = 
    {
    0xF731,0xF733,0xF734,0xF735,0xF736,0xF737,0xF738,0xF739, 
    0xF73A,0xF747,0xF748,0xF749,0xF74A,0xF74B,0xF74C,0xF74D
    }; 

// TInt number in command, used to show candidate list
const TInt KStartIntNumber = 3;
const TInt KEndIntNumber = 4;
// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CPeninputSplitQwertyLayoutImp* CPeninputSplitQwertyLayoutImp::NewL( TAny* /*aInitParams*/ )
    {
    CPeninputSplitQwertyLayoutImp* 
                    self = new ( ELeave ) CPeninputSplitQwertyLayoutImp();

    CleanupStack::PushL( self );
    self->ConstructL(); 
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor
// ---------------------------------------------------------------------------
//
CPeninputSplitQwertyLayoutImp::~CPeninputSplitQwertyLayoutImp()
    {
    }

// ---------------------------------------------------------------------------
// Create fep ui layout
// ---------------------------------------------------------------------------
//
CFepUiLayout* CPeninputSplitQwertyLayoutImp::CreateFepUiLayoutL( 
                                                   MLayoutOwner* aLayoutOwner, 
                                                   const TAny* aData )
    {
    CPeninputSplitQwertyLayout* 
        uiLayout = CPeninputSplitQwertyLayout::NewL( aLayoutOwner, aData );
    
    return uiLayout;
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CPeninputSplitQwertyLayoutImp::CPeninputSplitQwertyLayoutImp()
    {
    }

// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyLayoutImp::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CPeninputSplitQwertyLayout* CPeninputSplitQwertyLayout::NewL( 
    MLayoutOwner* aLayoutOwner, const TAny* aInitData )
    {
    CPeninputSplitQwertyLayout* self = 
                    new (ELeave) CPeninputSplitQwertyLayout( aLayoutOwner );

    CleanupStack::PushL( self );
    self->ConstructL( aInitData );
    CleanupStack::Pop( self );

    return self;  
    }

// ---------------------------------------------------------------------------
// C++ destructor
// ---------------------------------------------------------------------------
//
CPeninputSplitQwertyLayout::~CPeninputSplitQwertyLayout()
    {
    }

// ---------------------------------------------------------------------------
// Handle layout command event
// ---------------------------------------------------------------------------
//
TInt CPeninputSplitQwertyLayout::HandleCommand( TInt aCmd, TUint8* aData )
    {
    CPeninputCommonLayoutExt::HandleCommand( aCmd, aData );
    CPeninputSplitQwertyWindow* 
        vkbWindow = static_cast<CPeninputSplitQwertyWindow*>(iLayoutWindow);
                    
    switch ( aCmd )
        {
        case ECmdPenInputWindowOpen:
            {
            UiStateMgr()->SetCurrentUiState( EPeninputVkbUiStateStandby );
            }
            break;
        case ECmdPenInputWindowClose:
            {
            vkbWindow->UnLatchDeadKey();
            UiStateMgr()->SetCurrentUiState( EPeninputVkbUiStateNone );
            }
            break;
        case ECmdPenInputLanguage:
            {
            vkbWindow->SetSwitchlistLanguage( *aData );
            vkbWindow->SetDeadKey();
            TRAP_IGNORE(
                    vkbWindow->ConstructAccentListL( *aData );
                    vkbWindow->ConstructRangeLabelListL( *aData )
                    );
            }
            break;
        case ECmdPenInputRange:
            {
            TRAP_IGNORE( vkbWindow->UpdateRangeCtrlsL() );
            }
            break;
        case ECmdPenInputIsSecretText:
            {
            vkbWindow->SetSwitchlistSecretFlag( *aData );
            }
            break;
        case ECmdPenInputDimArrowKeys:
            {
            TBool IsDimArrowKeys = *aData;
            vkbWindow->DimArrowKeys( IsDimArrowKeys );
            }
            break;
        case ECmdPeninputVietSyncToneMarker:
        	{
        	TBool bDimToneMarker = *aData;
        	vkbWindow->EnableToneMarker(bDimToneMarker);
        	}
        	break;
        case ECmdPenInputSetPromptText:
            break;
        case ECmdPenInputCharacterPreview:
            {
            vkbWindow->ShowBubble( *aData );    
            }
            break;
        case ECmdPenInputInEditWordQueryDlg:
            {
            iInEditWordQueryDlg = *aData;
            }
            break;
        case ECmdPenInputPopupCandidateList:
            {
            TRAP_IGNORE( HandleShowCandidateListCmdL( aData ) );
            }
            break;
        case ECmdPenInputHideCandidateList:
            {
            SignalOwner( ESignalHideServerCandidate );
            }
            break;
        case ECmdPeninputITIStatus:
            {
            // Set the flag to indicate if FSQ with ITI feature is opened
            iITIEnabled = CPeninputDataConverter::AnyToInt( aData );
            }
            break;
        case ECmdPeninputSelectServerCandidate:
            {
            TRAP_IGNORE( HandleSelectServerCandItemL( aData ) );
            }
            break;
        default:
            {
            }
            break;
        }

    return KErrNone;    
    }

// ---------------------------------------------------------------------------
// Handle control event
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyLayout::HandleControlEvent( TInt aEventType, 
                                                     CFepUiBaseCtrl* aCtrl, 
                                                     const TDesC& aEventData )
    {
    switch ( aEventType )
        {
        case EEventVirtualKeyUp:
            {
            TRAP_IGNORE( HandleVirtualKeyUpL( aEventType, aCtrl, aEventData ) );
            }
            break;
        default:
            {
            CPeninputCommonLayoutExt::HandleControlEvent( aEventType, 
                                                          aCtrl, aEventData );
            }
            break;       
        }
    }

// ---------------------------------------------------------------------------
// Handle editor text coming
// ---------------------------------------------------------------------------
//
TInt CPeninputSplitQwertyLayout::OnAppEditorTextComing( 
                                      const TFepInputContextFieldData& aData )
    {
    CPeninputSplitQwertyWindow* 
        win = static_cast<CPeninputSplitQwertyWindow*>(iLayoutWindow);
        
    //Suppress ICF related actions
    if ( win )
        {
        return KErrNone;
        }
    
    return CPeninputCommonLayoutExt::OnAppEditorTextComing( aData );
    }

// ---------------------------------------------------------------------------
// Inform layout the size changing event
// ---------------------------------------------------------------------------
//
TInt CPeninputSplitQwertyLayout::SizeChanged( const TAny* aData )
    {
    return CPeninputCommonLayoutExt::SizeChanged( aData );
    }

// ---------------------------------------------------------------------------
// Inform UI that application infomation changed
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyLayout::HandleAppInfoChange( const TDesC& /*aInfo*/, 
                                                      TPeninputAppInfo /*aType*/ )
    {
    //Suppress ICF related actions
    }

// ---------------------------------------------------------------------------
// Called by owner when the layout is going to be active
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyLayout::OnActivate()
    {
    CPeninputCommonLayoutExt::OnActivate();
    
    CPeninputSplitQwertyWindow*
        win = static_cast<CPeninputSplitQwertyWindow*>( iLayoutWindow );
        
    if( win )
        {
        win->UpdateLayoutPosAndSize();
        }
    }

// ---------------------------------------------------------------------------
// Tell whether this layout require transparency.
// ---------------------------------------------------------------------------
//
TBool CPeninputSplitQwertyLayout::SemiTransparencyRequired()
    {
    return ETrue;
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CPeninputSplitQwertyLayout::CPeninputSplitQwertyLayout( MLayoutOwner* aLayoutOwner )
    : CPeninputCommonLayoutExt( aLayoutOwner )
    , iLayoutType( EPluginInputModeFSQ )
    {
    iInEditWordQueryDlg = EFalse;
    }

// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyLayout::ConstructL( const TAny* aInitData )
    {
    // must set layout type first
    iLayoutType = EPluginInputModeFSQ;
    
    CPeninputCommonLayoutExt::ConstructL( aInitData );
    
    CPeninputUiStateBase* state = NULL;
    CPeninputUiStateBase* initialState = NULL;
    
    //init state
    state = CPeninputUiStateBase::NewL( UiStateMgr(), this );
    UiStateMgr()->AddUiState( state, EPeninputVkbUiStateNone );
    initialState = state;
    
    //standby state
    state= CPeninputSplitQwertyUiStateStandby::NewL( UiStateMgr(), this );
    UiStateMgr()->AddUiState( state, EPeninputVkbUiStateStandby );
    
    //popup state
    state = CPeninputSplitQwertyUiStatePopup::NewL( UiStateMgr(), this );
    UiStateMgr()->AddUiState( state, EPeninputVkbUiStatePopup );
    
    UiStateMgr()->SetCurrentUiState( initialState );
    }

// ---------------------------------------------------------------------------
// Factory method for creation of layout window
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyLayout::CreateLayoutWindowL()
    {
    iLayoutWindow = CPeninputSplitQwertyWindow::NewL( this, this, UiStateMgr() );
    }

// ---------------------------------------------------------------------------
// Factory method for creation resource config
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyLayout::CreateDataMgrL( const TAny* /*aInitData*/ )
    {
    iDataMgr = CPeninputSplitQwertyDataMgr::NewL( this );
    }    

// ---------------------------------------------------------------------------
// Get the layout type
// ---------------------------------------------------------------------------
//
TInt CPeninputSplitQwertyLayout::LayoutType()
    {
    return iLayoutType;
    }

// ---------------------------------------------------------------------------
// Handle virtual key event
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyLayout::HandleVirtualKeyUpL( TInt aEventType, 
                                                      CFepUiBaseCtrl* aCtrl, 
                                                      const TDesC& aEventData )
    {
    CPeninputSplitQwertyWindow* 
        win = static_cast<CPeninputSplitQwertyWindow*>( iLayoutWindow );
    
    TBool flag = win->HandleVirtualKeyUpEvent( aEventType, aCtrl, aEventData );
    if( !flag )
        {
        HBufC* number = HBufC::NewL( 1 );
        TPtrC eventDataPtr = aEventData;
        TInt len = aEventData.Length();
        if ( len > 1 )
            {
            TVirtualKeyEventData* data = (TVirtualKeyEventData*) aEventData.Ptr();
            TUint16 inputCode = data->iKeyEvent.iCode;
            TUint16 outputCode = inputCode;
            TBool isDottedChar = EFalse;
            for ( TInt i = 0; i < KNumberOfDottedChar; i++ )
                {
                if ( thaiUnicodeNew[i] == inputCode )
                    {
                    outputCode = thaiUnicodeOri[i];
                    isDottedChar = ETrue;
                    break;
                    }
                }
            if ( isDottedChar )
                {
                number->Des().Append( outputCode );
                data->iKeyData.Set( *number );
                eventDataPtr.Set( reinterpret_cast<TUint16*>(data),
                                  sizeof(*data)/sizeof(TUint16) );
                }
            }
        
        CPeninputCommonLayoutExt::HandleControlEvent( aEventType, aCtrl, 
                                                      eventDataPtr );
        delete number;
        }
    }


// ---------------------------------------------------------------------------
// Handle show candidate list command.
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyLayout::HandleShowCandidateListCmdL( TUint8* aData )
    {
    // Read candidate data from a block of memory staring from aData
    // The format is activeIndex | count of candiates | 
    // length 1 | text 1 | length 2 | text 2 |...
    TPtr8 buf8( aData, sizeof( TInt32 ) * KStartIntNumber, 
                sizeof( TInt32 ) * KStartIntNumber );
    RDesReadStream readStream;
    readStream.Open( buf8 );
    CleanupClosePushL( readStream );
    // Get activeIndex
    TInt activeIndex = readStream.ReadInt32L();
    // Get coutn of candidates
    TInt count = readStream.ReadInt32L();    
    TInt langCode = readStream.ReadInt32L();
    TBidiText::TDirectionality dir = 
                    TBidiText::ScriptDirectionality( ( TLanguage )langCode );
    CGraphicsContext::TTextAlign align = ( dir == TBidiText::ELeftToRight ) ?
                                           CGraphicsContext::ELeft :
                                           CGraphicsContext::ERight;
    CleanupStack::PopAndDestroy( &readStream );
    
    CDesCArray* itemArray = NULL;
    if ( count > 0 )
        {        
        TUint8* curPointer = aData + sizeof( TInt ) * KStartIntNumber;
        itemArray = new ( ELeave ) CDesCArrayFlat( count );
        CleanupStack::PushL( itemArray );
        for ( TInt i = 0; i < count; i++ )
            {
            // Get length
            buf8.Set( curPointer, sizeof( TInt32 ), sizeof( TInt32 ) );
            readStream.Open( buf8 );
            CleanupClosePushL( readStream );
            TInt32 textSize = 0;
            textSize = readStream.ReadInt32L();            
            CleanupStack::PopAndDestroy( &readStream );
            if ( textSize > 0 )
                {
                // Get text
                curPointer += sizeof( TInt32 );
                HBufC* itemText = ReadTextInfoHBufCL
                                      ( (TUint16*)curPointer, 
                                        ( textSize + 1 )/ 2 );
                if ( itemText )
                    {
                    CleanupStack::PushL( itemText );
                    itemArray->AppendL( *itemText );                    
                    CleanupStack::PopAndDestroy( itemText ); 
                    }     
                curPointer += textSize;
                }
            }
        
        buf8.Set( curPointer, sizeof( TInt32 ) * KEndIntNumber, 
                  sizeof( TInt32 ) * KEndIntNumber );
        readStream.Open( buf8 );
        CleanupClosePushL( readStream );
        TRect rect;
        rect.iTl.iX = readStream.ReadInt32L();
        rect.iTl.iY = readStream.ReadInt32L();
        rect.iBr.iX = readStream.ReadInt32L();
        rect.iBr.iY = readStream.ReadInt32L();
        CleanupStack::PopAndDestroy( &readStream );
        
        TPeninputCandidateData cmd;
        cmd.iAlign = ( TInt ) align;
        cmd.iInitRect = rect;
        cmd.iSpellEnabled = EFalse;
        cmd.iTextWidthEnabled = EFalse;
        cmd.iItemArray = itemArray;
        cmd.iActiveIndex = activeIndex;
        
    	TPtrC buf( reinterpret_cast<TUint16*>( &cmd ), sizeof( cmd ) );
        SignalOwner( ESignalShowServerCandidate, buf );
        
        CleanupStack::PopAndDestroy( itemArray );
        }
    }


// ---------------------------------------------------------------------------
// Read text stored in a block of memory into HBufC.
// ---------------------------------------------------------------------------
//
HBufC* CPeninputSplitQwertyLayout::ReadTextInfoHBufCL( TUint16* aStartPtr, 
                                                      TInt aLength )
    {
    HBufC* itemText = NULL;
    if ( aLength > 0 )
        {
        itemText = HBufC::NewLC( aLength );
        TPtr itemTextPtr = itemText->Des();
        itemTextPtr.Copy( aStartPtr, aLength ); 
        CleanupStack::Pop( itemText ); 
        }    
    return itemText;
    }

// ---------------------------------------------------------------------------
// Handle select candidate list item command.
// ---------------------------------------------------------------------------
//
void CPeninputSplitQwertyLayout::HandleSelectServerCandItemL( TUint8* aData )
	{
	TPtr8* ptr = reinterpret_cast< TPtr8* > ( aData );
    RDesReadStream readStream;
    readStream.Open( *ptr );
    CleanupClosePushL( readStream );
    TInt command = readStream.ReadInt32L();
    TInt focusItem = readStream.ReadInt32L();
    CleanupStack::PopAndDestroy(&readStream);
    
    if ( command == ECandItemCmdItemSelected )
    	{
        TPtrC buf( reinterpret_cast< TUint16* >( &focusItem ),
        		   sizeof( TInt ) );
        SignalOwner ( ESignalSelectCandidate, buf );       			
		}	
	}

// End Of File