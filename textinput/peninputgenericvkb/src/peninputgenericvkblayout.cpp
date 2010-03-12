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
* Description:  generic VKB layout
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

// User includes
#include "peninputgenericvkb.hrh"
#include "peninputuistatemgrinterface.h"
#include "peninputgenericvkblayout.h"
#include "peninputgenericvkbwindow.h"
#include "peninputgenericvkbdatamgr.h"
#include "peninputgenericvkbuistatestandby.h"
#include "peninputgenericvkbuistatepopup.h"
#include "peninputdataconverter.h"

const TInt KPeriodicTimerIntervalSec(2500000);
// ======== MEMBER FUNCTIONS ========
const TInt KNumberOfDottedChar = 16; 
const TUint16 thaiUnicodeOri[KNumberOfDottedChar] = {0x0E31,0x0E33,0x0E34,0x0E35,0x0E36,0x0E37,0x0E38,0x0E39, 
                              0x0E3A,0x0E47,0x0E48,0x0E49,0x0E4A,0x0E4B,0x0E4C,0x0E4D}; 
const TUint16 thaiUnicodeNew[KNumberOfDottedChar] = {0xF731,0xF733,0xF734,0xF735,0xF736,0xF737,0xF738,0xF739, 
                              0xF73A,0xF747,0xF748,0xF749,0xF74A,0xF74B,0xF74C,0xF74D}; 


_LIT(KDisplaySpace, "\x0020");
_LIT(KEmptyString, "");

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLayoutImp::NewL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputGenericVkbLayoutImp* CPeninputGenericVkbLayoutImp::NewL( 
    TAny* /*aInitParams*/ )
    {
    CPeninputGenericVkbLayoutImp* self = new ( ELeave ) 
        CPeninputGenericVkbLayoutImp();
    CleanupStack::PushL( self );
    self->Construct( EFalse ); 
    CleanupStack::Pop( self );

    return self;
    }

CPeninputGenericVkbLayoutImp* CPeninputGenericVkbLayoutImp::NewFsqL( 
    TAny* /*aInitParams*/ )
    {
    CPeninputGenericVkbLayoutImp* self = new ( ELeave ) 
        CPeninputGenericVkbLayoutImp();
    CleanupStack::PushL( self );
    self->Construct( ETrue ); 
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLayoutImp::~CPeninputGenericVkbLayoutImp
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputGenericVkbLayoutImp::~CPeninputGenericVkbLayoutImp()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLayoutImp::CPeninputGenericVkbLayoutImp
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputGenericVkbLayoutImp::CPeninputGenericVkbLayoutImp()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLayoutImp::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbLayoutImp::Construct( TBool aIsFsq )
    {
    iIsFsq = aIsFsq;
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLayoutImp::CreateFepUiLayoutL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CFepUiLayout* CPeninputGenericVkbLayoutImp::CreateFepUiLayoutL( 
    MLayoutOwner* aLayoutOwner, const TAny* aData )
    {
    CPeninputGenericVkbLayout* uiLayout = 
        CPeninputGenericVkbLayout::NewL( aLayoutOwner, aData, iIsFsq );

    return uiLayout;
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLayout::NewL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputGenericVkbLayout* CPeninputGenericVkbLayout::NewL( 
    MLayoutOwner* aLayoutOwner, const TAny* aInitData, TBool aIsFsq )
    {
    CPeninputGenericVkbLayout* self = 
        new (ELeave) CPeninputGenericVkbLayout( aLayoutOwner );
    CleanupStack::PushL( self );
    self->ConstructL( aInitData, aIsFsq );
    CleanupStack::Pop( self );

    return self;  
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLayout::CPeninputGenericVkbLayout
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputGenericVkbLayout::CPeninputGenericVkbLayout( MLayoutOwner* aLayoutOwner )
    : CPeninputCommonLayoutExt( aLayoutOwner ), iLayoutType( EPluginInputModeVkb )
    {
    iInEditWordQueryDlg = EFalse;
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLayout::~CPeninputGenericVkbLayout
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputGenericVkbLayout::~CPeninputGenericVkbLayout()
    {    
    if (iInfoTimer)
        {
        iInfoTimer->Cancel();
        }
       delete iInfoTimer;
    }        

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLayout::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbLayout::ConstructL( const TAny* aInitData, TBool aIsFsq )
    {
    // must set layout type first
    iLayoutType = aIsFsq ? EPluginInputModeFSQ : EPluginInputModeVkb;
    
    CPeninputCommonLayoutExt::ConstructL( aInitData );
    
    CPeninputUiStateBase* initialState = 
        CPeninputUiStateBase::NewL( UiStateMgr(), this );  
    CPeninputUiStateVkbStandby* standbyState = 
        CPeninputUiStateVkbStandby::NewL( UiStateMgr(), this );   
	CPeninputUiStateVkbPopup* popupState = 
        CPeninputUiStateVkbPopup::NewL( UiStateMgr(), this );
        
    UiStateMgr()->AddUiState( initialState, EPeninputVkbUiStateNone );
    UiStateMgr()->AddUiState( standbyState, EPeninputVkbUiStateStandby );
    UiStateMgr()->AddUiState( popupState, EPeninputVkbUiStatePopup );
    UiStateMgr()->SetCurrentUiState( initialState );
    iInfoTimer = CPeriodic::NewL(CActive::EPriorityStandard);
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLayout::HandleCommand
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CPeninputGenericVkbLayout::HandleCommand( TInt aCmd, TUint8* aData )
    {
    CPeninputCommonLayoutExt::HandleCommand( aCmd, aData );
    CPeninputGenericVkbWindow* vkbWindow = 
    				static_cast<CPeninputGenericVkbWindow*>(iLayoutWindow);
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
            
            // For Addition of ITI features on FSQ
            // Close tooltip and candidate list
            vkbWindow->HideTooltip();
            vkbWindow->HideCandidateList();
            }
            break;
        case ECmdPenInputLanguage:
            {
            vkbWindow->SetSwitchlistLanguage(*aData);
            vkbWindow->SetDeadKey();
            if ( iLayoutType == EPluginInputModeFSQ )
                {
                TRAP_IGNORE(vkbWindow->ConstructFSQAccentListL( *aData ));
                TRAP_IGNORE(vkbWindow->ConstructRangeLabelListL( *aData ));
                }
            else
                {
                TRAP_IGNORE(vkbWindow->ConstructAccentListL(*aData));
                }
            }
            break;
        case ECmdPenInputRange:
            {
            if ( iLayoutType == EPluginInputModeFSQ )
                {
                TRAP_IGNORE( vkbWindow->UpdateRangeCtrlsL());
                }
            }
            break;
        case ECmdPenInputIsSecretText:
            {
            vkbWindow->SetSwitchlistSecretFlag(*aData);
            }
            break;
        case ECmdPenInputDimArrowKeys:
            {
            TBool IsDimArrowKeys = *aData;
            vkbWindow->DimArrowKeys( IsDimArrowKeys );
            }
            break;
        case ECmdPenInputSetPromptText:
            {
            if ( iLayoutType == EPluginInputModeFSQ )
            TRAP_IGNORE(vkbWindow->SetPromptTextL(aData));
            }
            break;
        case ECmdPenInputCharacterPreview:
            {
            vkbWindow->ShowBubble(*aData);    
            }
			break;
        case ECmdPenInputInEditWordQueryDlg:
            {
            iInEditWordQueryDlg = *aData;
            }
            break;
        case ECmdPenInputPopupTooltip:
            {    
            TRAP_IGNORE( HandleShowTooltipCmdL( aData ) );
            }
            break;
        case ECmdPenInputHideTooltip:
            {
            vkbWindow->HideTooltip();
            }
            break;
        case ECmdPenInputPopupCandidateList:
            {
            TRAP_IGNORE( HandleShowCandidateListCmdL( aData ) );
            }
            break;
        case ECmdPenInputHideCandidateList:
            {
            vkbWindow->HideCandidateList();
            }
            break;
        case ECmdPeninputITIStatus:
            {
            // Set the flag to indicate if FSQ with ITI feature is opened
            iITIEnabled = CPeninputDataConverter::AnyToInt( aData );
            }            
            break;
        case ECmdPenInputFingerMatchIndicator:
            {
            if ( iLayoutType == EPluginInputModeFSQ )
                {
				TRAP_IGNORE( vkbWindow->UpdateIndiBubbleL( aData ));
                }
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
// CPeninputGenericVkbLayout::HandleControlEvent
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbLayout::HandleControlEvent(
    TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData )
    {
    switch ( aEventType )
        {
        case EEventVirtualKeyUp:
            {
            TRAP_IGNORE(HandleVirtualKeyUpL(aEventType, aCtrl, aEventData));            
            }
            break;
        case EEventTooltipPointerUp:
        	{
        	// Signal to FEP to accept tooltip data 
        	SignalOwner( ESignalAcceptTooltip );        	
        	}
        	break;
        case EEventPointerDownOnInline:
            {
            // add by jiawenjuan 
            // show candidate list when tap on active words
            SignalOwner(ESignalShowCandidate );            
            }
            break;
        case EEventInsertSpace:
            {
            SignalOwner(ESignalKeyEvent, KDisplaySpace);            
            }
        	break;
        default:
            {
            CPeninputCommonLayoutExt::HandleControlEvent(aEventType, aCtrl, aEventData);
            }
            break;       
        }
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLayout::CreateLayoutWindowL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbLayout::CreateLayoutWindowL()
    {
    iLayoutWindow = CPeninputGenericVkbWindow::NewL( this, this, UiStateMgr() );
    }
    
// ---------------------------------------------------------------------------
// CPeninputGenericVkbLayout::CreateDataMgrL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbLayout::CreateDataMgrL( const TAny* /*aInitData*/ )
    {
    iDataMgr = CPeninputGenericVkbDataMgr::NewL( this );
    }    

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLayout::LayoutType
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//    
TInt CPeninputGenericVkbLayout::LayoutType()
    {
    return iLayoutType;
    }
TInt CPeninputGenericVkbLayout::SizeChanged( const TAny* aData )
    {
    (( CPeninputGenericVkbWindow*)iLayoutWindow)->UpdateLafData();
    return CPeninputCommonLayoutExt::SizeChanged( aData );
    }
void CPeninputGenericVkbLayout::HandleAppInfoChange(const TDesC& aInfo, 
                                                    TPeninputAppInfo aType)
    {
    if ( LayoutWindow() )
        {
        CFepLayoutMultiLineIcf* icf = static_cast<CFepLayoutMultiLineIcf*> 
                                    (LayoutWindow()->Control(EPeninputWindowCtrlIdMultiLineICF)) ;
        
        if ( icf && ( aType == EAppIndicatorMsg ) && (iLayoutType == EPluginInputModeFSQ) )
            { 
            if ( aInfo.Length() > 0 && !iInEditWordQueryDlg)
                {
				icf->HideBubble();
                CPeninputGenericVkbWindow* vkbWindow = 
                    static_cast<CPeninputGenericVkbWindow*>(iLayoutWindow);
                vkbWindow->SetIndiWithTextFlag( ETrue );
                vkbWindow->IndiBubbleWithText();
                
                if ( vkbWindow->IndicatorData().iIndicatorImgID != 0 && 
                     vkbWindow->IndicatorData().iIndicatorMaskID != 0 && 
                     vkbWindow->IndicatorData().iIndicatorTextImgID != 0 &&
                     vkbWindow->IndicatorData().iIndicatorTextMaskID != 0 )
                    {
					TRAP_IGNORE( vkbWindow->SetIndiBubbleImageL( 
                            vkbWindow->IndicatorData().iIndicatorImgID,
                            vkbWindow->IndicatorData().iIndicatorMaskID,
                            vkbWindow->IndicatorData().iIndicatorTextImgID,
                            vkbWindow->IndicatorData().iIndicatorTextMaskID ));
                    }
                icf->ShowBubble(aInfo,icf->MsgBubbleCtrl()->Rect());
                }
            else
                {
                icf->HideBubble();
                CPeninputGenericVkbWindow* vkbWindow = 
                    static_cast<CPeninputGenericVkbWindow*>(iLayoutWindow);
                vkbWindow->SetIndiWithTextFlag( EFalse );
                vkbWindow->IndiBubbleWithoutText();
                
                if ( vkbWindow->IndicatorData().iIndicatorImgID != 0 && 
                     vkbWindow->IndicatorData().iIndicatorMaskID != 0 && 
                     vkbWindow->IndicatorData().iIndicatorTextImgID != 0 &&
                     vkbWindow->IndicatorData().iIndicatorTextMaskID != 0 )
                    {
					TRAP_IGNORE( vkbWindow->SetIndiBubbleImageL( 
                            vkbWindow->IndicatorData().iIndicatorImgID,
                            vkbWindow->IndicatorData().iIndicatorMaskID,
                            vkbWindow->IndicatorData().iIndicatorTextImgID,
                            vkbWindow->IndicatorData().iIndicatorTextMaskID));
                    }
                
                icf->ShowBubble(KEmptyString, icf->MsgBubbleCtrl()->Rect());
                }
            }            
        else 
            {
            if ( icf && ( aType == EAppByteWarningMsg ) && (iLayoutType == EPluginInputModeFSQ) )
                { 
                if (!iInfoTimer->IsActive())
                    {
                    iInfoTimer->Start(KPeriodicTimerIntervalSec,
                                    KPeriodicTimerIntervalSec, 
                                    TCallBack(CPeninputGenericVkbLayout::HideByteWarningBubble, this));                           
                    }    
                    icf->ShowByteWarningBubble(aInfo);                                       
                }
            else 
                {
                if ( icf && ( aType == EAppMessageTypeChangeMsg ) && (iLayoutType == EPluginInputModeFSQ) )
                    { 
                    if (!iInfoTimer->IsActive())
                        {
                        if ( aInfo.Length() > 0 && !iInEditWordQueryDlg)
                            {
                            icf->ShowBubble(aInfo,icf->MsgBubbleCtrl()->Rect());
                            }
                        iInfoTimer->Start(KPeriodicTimerIntervalSec,
                                          KPeriodicTimerIntervalSec, 
                                          TCallBack(CPeninputGenericVkbLayout::HideByteWarningBubble, this));               
                        }
                        icf->ShowByteWarningBubble(aInfo);                                           
                    }
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLayout::SemiTransparencyRequired
// .
// ---------------------------------------------------------------------------
//
TBool CPeninputGenericVkbLayout::SemiTransparencyRequired()
    {
    return ETrue;
    }

void CPeninputGenericVkbLayout::HandleVirtualKeyUpL(TInt aEventType, 
                                                   CFepUiBaseCtrl* aCtrl, 
                                                   const TDesC& aEventData)
    {
    TBool flag;
    flag = static_cast<CPeninputGenericVkbWindow*>
           (iLayoutWindow)->HandleVirtualKeyUpEvent( aEventType, aCtrl, aEventData );
    if(!flag)
        {
        HBufC* number = HBufC::NewL( 1 );
        TPtrC eventDataPtr = aEventData;
        TInt len = aEventData.Length();
        if ( len > 1)
            {
            TVirtualKeyEventData* data = (TVirtualKeyEventData* ) aEventData.Ptr();
            TUint16 inputCode = data->iKeyEvent.iCode;
            TUint16 outputCode = inputCode;
            TBool isDottedChar = EFalse;
            for (TInt i = 0; i < KNumberOfDottedChar; i++)
                {
                if (thaiUnicodeNew[i] == inputCode)
                    {
                    outputCode = thaiUnicodeOri[i];
                    isDottedChar = ETrue;
                    break;
                    }
                }
            if (isDottedChar)
                {
                number->Des().Append( outputCode );                    
                data->iKeyData.Set(*number);
                eventDataPtr.Set(reinterpret_cast<TUint16*>(data),sizeof(*data)/sizeof(TUint16));
                }
            }           
        CPeninputCommonLayoutExt::HandleControlEvent(aEventType, aCtrl, eventDataPtr);
        delete number;
        }
    }

// ---------------------------------------------------------------------------
// Handle show tooltip command.
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbLayout::HandleShowTooltipCmdL( TUint8* aData )
    {
    CPeninputGenericVkbWindow* vkbWindow = 
    				static_cast<CPeninputGenericVkbWindow*>(iLayoutWindow);
    if ( !vkbWindow )
        {
        return;
        }
    
    TPtr8 buf8( aData, sizeof(TInt32), sizeof(TInt32) );
    RDesReadStream readStream;
    readStream.Open( buf8 );
    CleanupClosePushL( readStream );
    TInt dataSize = readStream.ReadInt32L();
    CleanupStack::PopAndDestroy( &readStream );
    if ( dataSize > 0 )
        {
        TUint16* dataAddress = (TUint16*)( aData + sizeof(TInt32) );
        HBufC* tooltipText = ReadTextInfoHBufCL( dataAddress, 
                                                 ( dataSize + 1 )/ 2 );
        if ( tooltipText )
            {
            CleanupStack::PushL( tooltipText );
            vkbWindow->ShowTooltipL( *tooltipText );
            CleanupStack::PopAndDestroy( tooltipText );
            }        
        }     
    }

// ---------------------------------------------------------------------------
// Handle show candidate list command.
// ---------------------------------------------------------------------------
//
void CPeninputGenericVkbLayout::HandleShowCandidateListCmdL( TUint8* aData )
    {
    CPeninputGenericVkbWindow* vkbWindow 
             = static_cast<CPeninputGenericVkbWindow*>(iLayoutWindow);
    if ( !vkbWindow || !IsEnableITIOnFSQ() )
        {
        return;
        }
        
    // Read candidate data from a block of memory staring from aData
    // The format is activeIndex | count of candiates | 
    // length 1 | text 1 | length 2 | text 2 |...
    TPtr8 buf8( aData, sizeof( TInt32 )*2, sizeof( TInt32 )*2 );
    RDesReadStream readStream;
    readStream.Open( buf8 );
    CleanupClosePushL( readStream );
    // Get activeIndex
    TInt activeIndex = readStream.ReadInt32L();
    // Get coutn of candidates
    TInt count = readStream.ReadInt32L();    
    CleanupStack::PopAndDestroy( &readStream );
    
    CDesCArray* itemArray = NULL;
    if ( count > 0 )
        {        
        TUint8* curPointer = aData + sizeof(TInt) * 2;
        itemArray = new (ELeave) CDesCArrayFlat( count );
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
        vkbWindow->ShowCandidateListL( itemArray, activeIndex );
        CleanupStack::PopAndDestroy( itemArray );
        }
    else
        {
        // Open empty candidate list.
        vkbWindow->ShowCandidateListL( NULL, activeIndex );        
        }    
    }


// ---------------------------------------------------------------------------
// Handle show candidate list command.
// ---------------------------------------------------------------------------
//
HBufC* CPeninputGenericVkbLayout::ReadTextInfoHBufCL( TUint16* aStartPtr, 
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

TInt CPeninputGenericVkbLayout::HideByteWarningBubble(TAny* aPointer)
    {
    CPeninputGenericVkbLayout* layout = static_cast<CPeninputGenericVkbLayout*>(aPointer);
    layout->HideByteWarningBubble();
    return KErrNone;
    }
void CPeninputGenericVkbLayout::HideByteWarningBubble()
    {
    if (!iInfoTimer)
        {
        return;
        }
    CFepLayoutMultiLineIcf* icf = static_cast<CFepLayoutMultiLineIcf*> 
                                 (LayoutWindow()->Control(EPeninputWindowCtrlIdMultiLineICF));
    if (iInfoTimer->IsActive())
        {
        iInfoTimer->Cancel();
        icf->HideInfoBubble();
        }
    }    
// End Of File
