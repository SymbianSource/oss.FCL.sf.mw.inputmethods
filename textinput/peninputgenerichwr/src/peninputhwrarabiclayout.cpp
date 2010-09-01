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
* Description:  generic HWR layout
*
*/


// System includes
#include <aknfeppeninputenums.h>    // Command from fep or IME to plugin
#include <AknFepGlobalEnums.h>      // Define EPluginInputModeHwr
#include <peninputcmd.h>            // Use global signal
#include <peninputpluginutils.h>    // Use resource utils and laf env
#include <peninputlayoutchoicelist.h>
#include <peninputlayoutbutton.h>
#include <peninputdragbutton.h>
#include <peninputcommonlayoutglobalenum.h>
#include <peninputdataconverter.h>

// User includes
#include "peninputhwrarabic.hrh"
//#include "peninputhwrevent.h"
#include "peninputuistatemgrinterface.h"
#include "peninputhwrarabiclayout.h"
#include "peninputhwrarabicwindow.h"
#include "peninputhwrarabicdatamgr.h"
#include "peninputhwrarabicstate.h"
#include "peninputhwrarabicstatestandby.h"
#include "peninputhwrarabicstatebeginwriting.h"
#include "peninputhwrarabicstateendwriting.h"
#include "peninputhwrboxarabicstatecandidateselecting.h"
#include "peninputuistatemgr.h"
#include "peninputlayouthwrwnd.h"

// constant definition
//const TInt KMaxPredictiveLen = 128;
const TInt KMaxCandidateLen = 128;
//const TUint16 KBackRep = 0x2408;
// convert displayable backspace
const TUint16 KBackRep = 0x2190;
const TUint16 KEnterRep = 0xF801;
const TUint16 KSpaceRep = 0xF800;
//const TUint16 KEnterCode = 0x2029;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPeninputHwrBxAbLayoutImp::NewL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//    
CPeninputHwrBxAbLayoutImp* CPeninputHwrBxAbLayoutImp::NewL( 
    TAny* /*aInitParams*/ )
    {
    CPeninputHwrBxAbLayoutImp* self = 
        new ( ELeave ) CPeninputHwrBxAbLayoutImp();
    CleanupStack::PushL( self );
    self->Construct(); 
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBxAbLayoutImp::~CPeninputHwrBxAbLayoutImp
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
CPeninputHwrBxAbLayoutImp::~CPeninputHwrBxAbLayoutImp()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBxAbLayoutImp::CPeninputHwrBxAbLayoutImp
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
CPeninputHwrBxAbLayoutImp::CPeninputHwrBxAbLayoutImp()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBxAbLayoutImp::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
void CPeninputHwrBxAbLayoutImp::Construct()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBxAbLayoutImp::CreateFepUiLayoutL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
CFepUiLayout* CPeninputHwrBxAbLayoutImp::CreateFepUiLayoutL(
    MLayoutOwner* aLayoutOwner, const TAny* aData )
    {
    CPeninputHwrBxAbLayout* uiLayout = 
        CPeninputHwrBxAbLayout::NewL( aLayoutOwner, aData );

    return uiLayout;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBxAbLayout::NewL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
CPeninputHwrBxAbLayout* CPeninputHwrBxAbLayout::NewL( 
    MLayoutOwner* aLayoutOwner, 
    const TAny* aInitData )
    {
    CPeninputHwrBxAbLayout* self = 
        new ( ELeave ) CPeninputHwrBxAbLayout( aLayoutOwner );
    CleanupStack::PushL( self );
    self->ConstructL( aInitData );
    CleanupStack::Pop( self );

    return self;  
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBxAbLayout::CPeninputHwrBxAbLayout
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
CPeninputHwrBxAbLayout::CPeninputHwrBxAbLayout(
    MLayoutOwner* aLayoutOwner )
    : CPeninputCommonLayout( aLayoutOwner )
    {
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBxAbLayout::~CPeninputHwrBxAbLayout
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
CPeninputHwrBxAbLayout::~CPeninputHwrBxAbLayout()
    {    
    }        

// ---------------------------------------------------------------------------
// CPeninputHwrBxAbLayout::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
void CPeninputHwrBxAbLayout::ConstructL( const TAny *aInitData )
    {
    CPeninputCommonLayout::ConstructL( aInitData ); 

    
    CPeninputHwrBxAbStateBase* initState = CPeninputHwrBxAbStateBase::NewL( UiStateMgr(), this );
    CPeninputHwrBxAbStateStandby* standbyState = CPeninputHwrBxAbStateStandby::NewL( UiStateMgr(), this );
    CPeninputHwrBxAbStateBeginWriting* beginWritingState = CPeninputHwrBxAbStateBeginWriting::NewL( UiStateMgr(), this );
    CPeninputHwrBxAbStateEndWriting* endWritingState = CPeninputHwrBxAbStateEndWriting::NewL( UiStateMgr(), this );
    CPeninputHwrBxAbStateCandidateSelecting* candSelectingState = CPeninputHwrBxAbStateCandidateSelecting::NewL( UiStateMgr(), this );

    UiStateMgr()->AddUiState( initState, EPeninputHwrBxAbStateInit );
    UiStateMgr()->AddUiState( standbyState, EPeninputHwrBxAbStateStandby );
    UiStateMgr()->AddUiState( beginWritingState, EPeninputHwrBxAbStateBeginWriting );
    UiStateMgr()->AddUiState( endWritingState, EPeninputHwrBxAbStateEndWriting );
    UiStateMgr()->AddUiState( candSelectingState, EPeninputHwrBxAbStateCandidateSelecting );
    UiStateMgr()->SetCurrentUiState( initState );
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBxAbLayout::HandleCommand
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
TInt CPeninputHwrBxAbLayout::HandleCommand( TInt aCmd, TUint8* aData )
    {
    CPeninputCommonLayout::HandleCommand( aCmd, aData );
    
    if ( aCmd == ECmdPenInputWindowOpen )
        {
        TInt curRange = CPeninputDataConverter::AnyToInt( RequestData( EPeninputDataTypeCurrentRange ) );
        UiStateMgr()->SetCurrentUiState( EPeninputHwrBxAbStateStandby );
        if (curRange == ERangeNative)
            {
            static_cast<CPeninputHwrBxAbWnd *>(LayoutWindow())->ResetAndShowDropdownList();
            }
        else
            {
            static_cast<CPeninputHwrBxAbWnd *>(LayoutWindow())->ClearAndCloseDropdownList();
            }
        }
    else if ( aCmd == ECmdPenInputWindowClose )
        {
        UiStateMgr()->SetCurrentUiState( EPeninputHwrBxAbStateInit );
        }
    else if( aCmd == ECmdPenInputLanguage )
        {
        ((CPeninputHwrBxAbWnd*)iLayoutWindow)->OnLanguageChange();
        }
    else if(aCmd == ECmdPenInputRange)
        {
        UiStateMgr()->SetCurrentUiState( EPeninputHwrBxAbStateStandby );        
        iLayoutWindow->SizeChanged(ETrue);
        }
    return KErrNone;    
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBxAbLayout::HandleControlEvent
// Handle event from control
// ---------------------------------------------------------
//
void CPeninputHwrBxAbLayout::HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, 
                                            const TDesC& aEventData)
    {
    MPeninputUiStateMgr * UIStateMgr = UiStateMgr();
    if (UIStateMgr)
        {    
        CPeninputHwrBxAbStateBase* current =
                        static_cast<CPeninputHwrBxAbStateBase*>(UIStateMgr->CurrentUiState());
        if (current)
            {
            TBuf<KMaxCandidateLen> charCode;
            TBool handled = EFalse;
            TRAPD(err, current->HandleControlEventPenL(aEventType, aEventData, *this, 
                                                        charCode, handled));
            if (KErrNone != err)
                { // close peninput
                CPeninputCommonLayout::HandleControlEvent(
                        EPeninputLayoutEventClose, NULL, KNullDesC);
                handled = ETrue;
                }
            if (handled)
                {
                return;
                }
            }
        }
    CPeninputCommonLayout::HandleControlEvent(aEventType, aCtrl, aEventData);	

    return;
    }
// ---------------------------------------------------------------------------
// CPeninputHwrBxAbLayout::CreateLayoutWindowL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
void CPeninputHwrBxAbLayout::CreateLayoutWindowL()
    {
    iLayoutWindow = CPeninputHwrBxAbWnd::NewL( this, this );
    }
 
// ---------------------------------------------------------------------------
// CPeninputHwrBxAbLayout::CreateDataMgrL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//      
void CPeninputHwrBxAbLayout::CreateDataMgrL( const TAny* aInitData )
    {
    TPeninputLayoutData* layoutData = (TPeninputLayoutData*)aInitData;

    iDataMgr = CPeninputHwrBxAbDataMgr::NewL( this, (*layoutData).iPtiEngine );
    }    

// ---------------------------------------------------------------------------
// CPeninputHwrBxAbLayout::LayoutType
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
TInt CPeninputHwrBxAbLayout::LayoutType()
    {
    return EPluginInputModeHwr;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBxAbLayout::SemiTransparencyRequired
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
TBool CPeninputHwrBxAbLayout::SemiTransparencyRequired()
    {
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBxAbLayout::LayoutWindow
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputLayoutWindow* CPeninputHwrBxAbLayout::LayoutWindow() const
    {
    return iLayoutWindow;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBxAbLayout::TranslateCharCode
// Translate the char code.
// ---------------------------------------------------------
//
void CPeninputHwrBxAbLayout::TranslateCharCode( TDes& aCharCode )
    {
    TPtrC16 ptr;
    ptr.Set(&KBackRep, sizeof(KBackRep)/2);
    
    if (aCharCode.CompareC(ptr) == 0)
        {
        aCharCode.Zero();
        aCharCode.Append((TChar)EKeyBackspace);
        }
    else 
        {
        ptr.Set(&KSpaceRep, sizeof(KSpaceRep)/2);
        if (aCharCode.CompareC(ptr) == 0)
            {
            aCharCode.Zero();
            aCharCode.Append((TChar)EKeySpace);
            }
        else
            {
            ptr.Set(&KEnterRep, sizeof(KEnterRep)/2);
            if (aCharCode.CompareC(ptr) == 0)
                {
                aCharCode.Zero();
                aCharCode.Append((TChar)EKeyEnter);
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBxAbLayout::ReplaceL
// Delete the previous charcode and submit the new one
// ---------------------------------------------------------
//
void CPeninputHwrBxAbLayout::ReplaceL(const TDesC& aOldCharCode, const TDesC& aNewCharCode)
    {
    // user tries to correct the previous selection
    // Delete the previous character and append the new one
    _LIT(KSpaceChar, " ");
    TBuf<1> buf;
    buf.Zero();
    buf.Append(KSpaceChar);
    HBufC* p = NULL;
    HBufC* oldCharCode = aOldCharCode.AllocLC();
    HBufC* newCharCode = aNewCharCode.AllocLC();
            
    if (static_cast<CPeninputHwrBxAbWnd *>(LayoutWindow())->IsAddingSpace(newCharCode))
        {
        p = HBufC::New(aNewCharCode.Length() + 2 + buf.Length());
        }
    else
        {
        p = HBufC::New(aNewCharCode.Length() + 2);
        }
    if( p )
        {
        TInt len = aOldCharCode.Length();
        if (static_cast<CPeninputHwrBxAbWnd *>(LayoutWindow())->IsAddingSpace(oldCharCode))
            {
            len += buf.Length();
            }
        p->Des().Append((TUint16*)&len, 2);
        p->Des().Append(aNewCharCode);
        if (static_cast<CPeninputHwrBxAbWnd *>(LayoutWindow())->IsAddingSpace(newCharCode))
            {
            p->Des().Append(buf);
            }
        SignalOwner( ESignalReplaceText, *p );
        delete p;
        }
    CleanupStack::PopAndDestroy(newCharCode);
    CleanupStack::PopAndDestroy(oldCharCode);        
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBxAbLayout::SubmitL
// Submit the char code.
// ---------------------------------------------------------
//
void CPeninputHwrBxAbLayout::SubmitL(const TDesC& aCharCode)
    {
    HBufC* buf = HBufC::NewLC(aCharCode.Length());
    TPtr charcode(buf->Des());
    charcode = aCharCode;
    TranslateCharCode( charcode );
    SignalOwner(ESignalKeyEvent, charcode);
    
    CleanupStack::PopAndDestroy(buf);
    }

//End Of File
