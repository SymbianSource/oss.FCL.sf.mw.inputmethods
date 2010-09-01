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
* Description:  CknFepHwrBxLayout
*
*/

#include <bautils.h>
#include <peninputcmd.h>
#include <peninputlayoutbutton.h>
#include <peninputdropdownlist.h>
#include <peninputhwrbxcn.rsg>
#include <aknfeppeninputenums.h>
#include <AknFepGlobalEnums.h>
#include <peninputlayoutbutton.h>
#include <AknUtils.h>
#include <peninputdragbutton.h>
#include <coemain.h>
#include <peninputlayoutchoicelist.h>

#include "peninputhwrbxlayout.h"
#include "peninputhwrctrlid.h"
#include "peninputhwrbxwnd.h"
#include "peninputhwrbxstate.h"
#include "peninputhwrevent.h"
#include "peninputhwrbxdatastore.h"
#include "aknlayoutscalable_avkon.cdl.h"

_LIT(KResourceFile, "z:\\resource\\peninputhwrbxcn.rsc");

const TInt KUpperCaseIndex = 0;
const TInt KLowerCaseIndex = 1;
const TInt KTextCaseIndex = 2;
const TInt KRepInitBrX = 0;
const TInt KRepInitBrY = 0;


// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CAknFepHwrBxLayoutImp* CAknFepHwrBxLayoutImp::NewL( TAny* /*aInitParams*/ )
    {
    CAknFepHwrBxLayoutImp* self = new(ELeave) CAknFepHwrBxLayoutImp();
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop(self);

    return self;
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CAknFepHwrBxLayoutImp::~CAknFepHwrBxLayoutImp()
    {
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CAknFepHwrBxLayoutImp::CAknFepHwrBxLayoutImp()
    {
    // See ConstructL() for initialisation completion.
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
void CAknFepHwrBxLayoutImp::ConstructL()
    {
    }

// ---------------------------------------------------------
// Implementation of CFepUiLayoutInterface
// ---------------------------------------------------------
//
MPenUiLayoutBase* CAknFepHwrBxLayoutImp::CreateFepUiLayoutL(MLayoutOwner* aLayoutOwner,
                                                            const TAny* aData)
    {
    CAknFepHwrBxLayout* uiLayout = CAknFepHwrBxLayout::NewL( aLayoutOwner, aData );

    return uiLayout;
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CAknFepHwrBxLayout* CAknFepHwrBxLayout::NewL(MLayoutOwner* aLayoutOwner, const TAny* aInitData)
    {
    CAknFepHwrBxLayout* self = new (ELeave) CAknFepHwrBxLayout(aLayoutOwner);

    CleanupStack::PushL(self);
    self->ConstructL(aInitData);
    CleanupStack::Pop(self);

    return self;  
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CAknFepHwrBxLayout::CAknFepHwrBxLayout(MLayoutOwner* aLayoutOwner)
: CFepUiLayout(aLayoutOwner)
    {
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
void CAknFepHwrBxLayout::ConstructL(const TAny* aInitData)
    {
    BaseConstructL();

    //ignore aInitData, for later use
    CCoeEnv* coeEnv = CCoeEnv::Static();
    TPixelsTwipsAndRotation ptSize;     
    coeEnv->ScreenDevice()->GetDefaultScreenSizeAndRotation(ptSize);

    SetScreenSize(ptSize.iPixelSize);
    
    //iLafEnv->CreateLafEnvL(*ptSize);

    iResId = coeEnv->AddResourceFileL(KResourceFile);

    //create date store must before create UI because of property subscriber
    TPeninputLayoutData* layoutData = (TPeninputLayoutData*)aInitData;
    
    iDataStore = CAknFepHwrBxDataStore::NewL((*layoutData).iPtiEngine);

    TBool boxWndLandscape;
    
    //set proper screen style for box window
    if (ptSize.iPixelSize.iWidth < ptSize.iPixelSize.iHeight)
    	{
    	// protrait screen
        boxWndLandscape = EFalse;
    	}
    else
        {
    	// landscape screen
        boxWndLandscape = ETrue;
        }
    
    //create window UI
    iBxWindow = CAknFepHwrBxWnd::NewL(this, EHwrCtrlIdHwrWindow, boxWndLandscape);
    iBxWindow->SetResourceId(R_AKN_FEP_HWR_WINDOW);
    iBxWindow->ConstructFromResourceL();
    
    iDataStore->SetScreenMode(ptSize);
    iDataStore->LoadAndPublishDefaultL();

    //set propery size
    //iDataStore->SetSizeChanging(ETrue);
    iBxWindow->SizeChanged(boxWndLandscape);
    //iDataStore->SetSizeChanging(EFalse);
    
    AddControlL(iBxWindow);
    EnableLayoutMoving(static_cast<CDragBar*>(iBxWindow->MoveButton()));
    
    //set current state
    CAknFepHwrBxStateBase * state = CAknFepHwrBxStateInit::NewLC(this);
    SetCurrentStateL(state);
    CleanupStack::Pop(state);
    
    ResetLayoutRectL();
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CAknFepHwrBxLayout::~CAknFepHwrBxLayout()
    {
    delete iDataStore;
    CCoeEnv::Static()->DeleteResourceFile(iResId);
    delete iCurrentState;
    }
          
// ---------------------------------------------------------
// Set current state
// ---------------------------------------------------------
//
void CAknFepHwrBxLayout::SetCurrentStateL(CAknFepHwrBxStateBase* aState)
    {
    if (iCurrentState)
        {
        iCurrentState->OnExit();
        
        delete iCurrentState;
        iCurrentState = NULL;
        }
        
    if (aState)        
        {
        aState->OnEntry();
        }
        
    iCurrentState = aState;
    }
    
// ---------------------------------------------------------
// Open option menu
// ---------------------------------------------------------
//
void CAknFepHwrBxLayout::OpenOptionMenu()
    {
    SignalOwner(ESignalLaunchOptionMenu); 
    }
    
// ---------------------------------------------------------
// Open lang switch menu
// ---------------------------------------------------------
//
void CAknFepHwrBxLayout::OpenLangSwitchMenu()
    {
    SignalOwner(ESignalLaunchLanguageMenu); 
    }

// ---------------------------------------------------------
// Handle command from both Fep & internal
// ---------------------------------------------------------
//
TInt CAknFepHwrBxLayout::HandleCommand(TInt aCmd, TUint8* aData)
    {
    TInt ret = KErrNone;
    
    switch ( aCmd )
        {
        case ECmdPenInputEditorNumericKeyMap:
            iDataStore->SetNumberMode(*(TAknEditorNumericKeymap*)aData);
            break;
        case ECmdPenInputPermittedRange:
                {
                const TInt* ranges = (TInt*)aData;
                iDataStore->SetPermittedRanges(*ranges);
                }
            break;
        case ECmdPenInputPermittedCase:
                {
                const TInt* ranges = (TInt*)aData;
                iDataStore->SetPermittedCases(*ranges);
                }
            break;
        case ECmdPenInputLanguage:
                {
                const TInt* language = (TInt*)aData;
                TRAP_IGNORE(iDataStore->SetLanguageL(*language));
                }
            break;
        case ECmdPenInputRange:
                {
                TInt* range = (TInt*)aData;
                iDataStore->SetPremaryRange(*range);
                
                TBuf<4> buf;
                buf.Append(reinterpret_cast<TText*>(range), 4);
                SignalOwner(ESignalRange, buf);   
                }
            break;
        case ECmdPenInputCase:
                {
                TInt cs = *((TInt*)aData);
                switch(cs)
                    {
                    case ECaseUpper:
                        cs = KUpperCaseIndex;
                        break;
                    case ECaseLower:
                        cs = KLowerCaseIndex;
                        break;
                    case ECaseText:
                        cs = KTextCaseIndex;
                        break;
                    default:
                        cs = KUpperCaseIndex;
                        break;
                    }
                iDataStore->SetCase(cs);                
                }
            break;
        case ECmdPenInputSetWindowPos:
                {
                const TPoint* tl = (TPoint*)aData;
                const TRect rect = iBxWindow->Rect();
                iBxWindow->SetRect(TRect(*tl,TSize(rect.Width(),rect.Height())));                
                }
            break;
        case ECmdPenInputEditorCustomNumericKeyMap:
                {
                TInt* len = (TInt*)(aData - 4);
                TPtrC16 keymapRes((const TUint16*)aData, *len/2);
                TRAP_IGNORE(iDataStore->SetNumberModeL(keymapRes));
                }
            break;
        case ECmdPenInputEnableSettingBtn:
            {
            iBxWindow->SetEnableSettingBtn(*aData);
            }
            break;  
        case ECmdPenInputWindowOpen:
            {
            //inform engine about input area size
            TRect hwrRect = iBxWindow->ClientPane()->Rect();
            TSize boxSize(hwrRect.Width()/10*9/2, hwrRect.Height());         
            //jump to standby state
            TRAP_IGNORE( GoToStandbyStateL( *aData ) );    
            iDataStore->SetInputAreaSize(boxSize);                
            TPixelsTwipsAndRotation size; 
            CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation(size);
            TSize wndSize(size.iPixelSize.iWidth,size.iPixelSize.iHeight);
            iDataStore->SetScreenSize(wndSize);
            
            }
            break;
        case ECmdPenInputSetTextAlignment:
        	{
        	TRAP_IGNORE(iBxWindow->SetTextAlignmentL( *aData ));
        	}
        	break;            
        default:
            {
            ret = CFepUiLayout::HandleCommand( aCmd, aData );
            }
            break;
        }
        
    TRAP_IGNORE(iCurrentState->HandleCommandL(aCmd,aData));
        
    return ret;
    }

// ---------------------------------------------------------
// Handle size changed event
// ---------------------------------------------------------
//
TInt CAknFepHwrBxLayout::SizeChanged(const TAny* /*pData*/)
    {
    TBool landscapeStyle;    
    TPixelsTwipsAndRotation ptSize;     
    
    CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation(ptSize);
    
    if (ptSize.iPixelSize.iWidth < ptSize.iPixelSize.iHeight)
    	{
    	// protrait screen
    	landscapeStyle = EFalse;
    	}
    else
        {
    	// landscape screen
    	landscapeStyle = ETrue;
        }
        
    SetShadowRect(TRect());
    iBxWindow->SetShadowRect(TRect());
    
    iDataStore->SetSizeChanging(ETrue);
    iBxWindow->SizeChanged(landscapeStyle);
    iDataStore->SetSizeChanging(EFalse);
    
    iDataStore->SetScreenMode(ptSize);
    SetScreenSize(ptSize.iPixelSize);

    //config layout self
    TRAP_IGNORE(ResetLayoutRectL());
    return KErrNone;
    }

// ---------------------------------------------------------
// Handle event from control
// ---------------------------------------------------------
//
void CAknFepHwrBxLayout::HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, 
                                            const TDesC& aEventData)
    {
    TBool handled = EFalse;
    
    //translate the button up event into events usable for FSM
    if (aEventType == EEventButtonUp)
        {
        switch (aCtrl->ControlId())
            {
            case EHwrCtrlIdSpace:
                aEventType = EHwrEventKeySpace;
                break;    
            case EHwrCtrlIdVkb:
                aEventType = EHwrEventVkb;
                break;
            case EHwrCtrlIdTouchInputOption:
                aEventType = EHwrEventTouchInputOption; 
                break;
            default:
                break;
            }
        }

    if (aEventType == EEventDraggingEnd && iDataStore)
        {
        iDataStore->SaveWindowPos(Position() + Rect().Size());
        }

    //filter un-related events
    if ((aEventType != EEventSetAppCursorSelection) &&
        (aEventType >= EHwrEventLast || aEventType < EHwrEventWindowClose))
        {
        if (aEventType != EEventChoiceSelected)
            {
            return;
            }
        }
        
    if (iCurrentState != NULL && 
        aEventType != EHwrEventVkb && 
        aEventType != EHwrEventSetRange)
        {
        TRAP_IGNORE( (handled = iCurrentState->HandleEventL(aEventType, aEventData)) );
        }
    
    if (!handled)
        {
        switch (aEventType)
            {
            case EHwrEventInputLangSwitch:
                OpenLangSwitchMenu();
                break;
            case EHwrEventTouchInputOption:
                OpenOptionMenu();
                break;
            case EHwrEventVkb:
                {
                iBxWindow->CancelWriting();
                iBxWindow->PopUpPenInputModeSwitcher();
                }
                break;
            case EHwrEventSetRange:
                {
                CAknFepHwrBxStateBase* state = NULL;
                TRAP_IGNORE( state = CAknFepHwrBxStateStandby::NewL( this ));
                TRAP_IGNORE( SetCurrentStateL(state));
        
                TInt range = *(TInt*)aEventData.Ptr();
                
                if ( range == ERangeSymbol )
            		{
            		SignalOwner(ESignalLaunchSCT);
            		}
                else            		
                    {
                    if (iDataStore)
                        {
                        iDataStore->SetPremaryRange(range);
                        iDataStore->SaveRange(range);
                        }
                    TBuf<4> buf;
                    buf.Append(reinterpret_cast<TText*>(&range), 4);
                    SignalOwner(ESignalRange, buf);   
                    TRAP_IGNORE(SetLayoutPositionL());
                    }
                }
                break;
            case EHwrEventSetCase:
                    {
                    const TInt* cs = (TInt*)aEventData.Ptr();
                    if (iDataStore)
                        {
                        iDataStore->SetCase(*cs);
                        }
                    TInt curCase = ECaseUpper;
                    switch(*cs)
                        {
                        case KUpperCaseIndex:
                            curCase = ECaseUpper;
                            break;
                        case KLowerCaseIndex:
                            curCase = ECaseLower;
                            break;
                        case KTextCaseIndex:
                            curCase = ECaseText;
                            break;
                        default:
                            break;
                        }
                        
                    TBuf<4> buf;
                    buf.Append(reinterpret_cast<TText*>(&curCase), 4);
                    SignalOwner(ESignalCaseMode, buf);                    
                    }
                break;
           	case EHwrEventGetNextCandidatePage:
           	    {
                iDataStore->GetNextPredictivePage();
           	    }
           	    break;
           	case EHwrEventCandidateExisted:
           	    {
                (iBxWindow->DropdownList())->SetFlagCandidateExist(
                                                  iDataStore->PredictiveCandidateExisted());
           	    }
           	    break;
            case EHwrEventKeyBack:
                Delete();
                break;
            case EHwrEventKeySpace:
                {
                if ( !( iDataStore->IsNumberOnly() ) )
                    {
                    TBuf<1> buf;
                    buf.Append( EKeySpace );
                    SignalOwner(ESignalKeyEvent, buf);
                    }
                }
                break;
            case EHwrEventWindowClose:
                SignalOwner(ESignalLayoutClosed);
                break;
            case EEventChoiceSelected:
                {
                CFepLayoutChoiceList::SEvent* event = 
                                                  (CFepLayoutChoiceList::SEvent*)aEventData.Ptr();
                if( event->iIndex != -1 )
                    {
                    TBool switchByMode = ETrue;
                    TInt  mode = event->iCommand;

                    TBuf<8> buf;
                    buf.Append(reinterpret_cast<TText*>(&switchByMode), 
                               sizeof(TBool)/sizeof(TText));
                    buf.Append(reinterpret_cast<TText*>(&mode), sizeof(TInt)/sizeof(TText));
                    SignalOwner(ESignalLayoutUIChanged,buf);
                    }
                }
                break;
            default:
                break;
            }
        }
    }
// ---------------------------------------------------------
// Handle key event 
// ---------------------------------------------------------
//
TBool CAknFepHwrBxLayout::HandleRawKeyEventL(const TRawEvent& aKeyEvent)
    {
    TBool handled = EFalse;
    
    if (iCurrentState)
        {
        handled = iCurrentState->HandleKeyEventL(aKeyEvent);
        }
        
    return handled;
    }

// ---------------------------------------------------------
// Validate move button destination 
// ---------------------------------------------------------
//
TBool CAknFepHwrBxLayout::IsValidDestination(const TRect& aRect,
                                            CDragBar* aDragBar,
                                            TBool& aVInfo,TBool& aHInfo,
                                            TBool aFlag)
    {
    TRect rect = aRect;
    
    if (aDragBar)
        {
        rect.SetSize(aDragBar->Rect().Size());
        }
    
    if( aFlag ) 
        {
        aHInfo = aVInfo = EFalse;  
        if(rect.iTl.iX >= 0 && rect.iBr.iX < ScreenSize().iWidth)
            {
            aHInfo = ETrue;
            }
        
        if(rect.iTl.iY >= 0 && rect.iBr.iY < ScreenSize().iHeight)
            {
            aVInfo = ETrue;
            }
            
        return aHInfo && aVInfo;
        }
    else
        {
        TBool tlInvalid = !Rect().Contains(rect.iTl);
        TBool brInvalid = !Rect().Contains(rect.iBr);
        
        
        if (tlInvalid && !brInvalid)
            {
            if (rect.iTl.iX >= Rect().iTl.iX && rect.iTl.iX <= Rect().iBr.iX)
                {
                aHInfo = ETrue;
                }
            else if (rect.iTl.iY >= Rect().iTl.iY && rect.iTl.iY <= Rect().iBr.iY)
                {
                aVInfo = ETrue;
                }
            }
        else if (!tlInvalid && brInvalid)
            {
            if (rect.iBr.iX >= Rect().iTl.iX && rect.iBr.iX <= Rect().iBr.iX)
                {
                aHInfo = ETrue;
                }
            else if (rect.iBr.iY >= Rect().iTl.iY && rect.iBr.iY <= Rect().iBr.iY)

                {
                aVInfo = ETrue;
                }
            }
            
        return !(tlInvalid || brInvalid);
        }
    }

// ---------------------------------------------------------
// Handle edit text coming
// ---------------------------------------------------------
//
TInt CAknFepHwrBxLayout::OnAppEditorTextComing(const TFepInputContextFieldData& aData)
    {
    if (iBxWindow)
        {
        TRAPD(err, iBxWindow->SetEditorTextL(aData));
        return err;
        }
        
    return KErrNone;
    }
    
// ---------------------------------------------------------
// Translate the char code.
// ---------------------------------------------------------
//
void CAknFepHwrBxLayout::TranslateCharCode( TDes& aCharCode )
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

// ---------------------------------------------------------
// Submit the char code.
// ---------------------------------------------------------
//
void CAknFepHwrBxLayout::SubmitL(const TDesC& aCharCode)
    {
    HBufC* buf = HBufC::NewLC(aCharCode.Length());
    TPtr charcode(buf->Des());
    charcode = aCharCode;
    TranslateCharCode( charcode );
    SignalOwner(ESignalKeyEvent, charcode);
    
    CleanupStack::PopAndDestroy(buf);
    }

// ---------------------------------------------------------
// Delete the previous charcode and submit the new one
// ---------------------------------------------------------
//
void CAknFepHwrBxLayout::ReplaceL(const TDesC& aOldCharCode, const TDesC& aNewCharCode)
    {
    // user tries to correct the previous selection
    // Delete the previous character and append the new one
    HBufC* p = HBufC::New(aNewCharCode.Length() + 2);
    if( p )
        {
        TInt len = aOldCharCode.Length();
        p->Des().Append((TUint16*)&len, 2);
        p->Des().Append(aNewCharCode);
        SignalOwner( ESignalReplaceText, *p );
        delete p;
        }
    }

// ---------------------------------------------------------
// Delete the previous charcode 
// ---------------------------------------------------------
//
void CAknFepHwrBxLayout::Delete(TInt aChar)
    {
        //delete the last char
    const TInt len = sizeof(aChar);
    TBuf<len> aBufBck;
    aBufBck.Append((TChar)aChar);

    SignalOwner(ESignalKeyEvent, aBufBck);
    }

// ---------------------------------------------------------
// Reset layout rect 
// ---------------------------------------------------------
//
void CAknFepHwrBxLayout::SetLayoutPositionL()
    {
    ResetLayoutRectL();
    }

// ---------------------------------------------------------
// Reset layout rect by LAF data
// ---------------------------------------------------------
//
void CAknFepHwrBxLayout::ResetLayoutRectL()
    {
    TRect rtTmp(iBxWindow->Rect());

    //get shadow size
    rtTmp.Resize(iBxWindow->ShadowTlSize() + iBxWindow->ShadowBrSize());
    
    TPoint br = iDataStore->GetWindowPos();
    if (br.iX == KRepInitBrX && br.iY ==KRepInitBrY)
        {
        SetRect(rtTmp);
        }
    else
        {
        TRect layouRect = TRect(TPoint(0, 0), ScreenSize());
        TSize temp = rtTmp.Size();
        if (br.iX > layouRect.iBr.iX)
            {
            br.iX = layouRect.iBr.iX;
            }

        if (br.iY > layouRect.iBr.iY)
            {
            br.iY = layouRect.iBr.iY;
            }

        TPoint tl = br - temp;

        if (tl.iX > layouRect.iBr.iX)
            {
            tl.iX = layouRect.iBr.iX;
            }
        else if (tl.iX < layouRect.iTl.iX)
            {
            tl.iX = layouRect.iTl.iX;
            }

        if (tl.iY > layouRect.iBr.iY)
            {
            tl.iY = layouRect.iBr.iY;
            }
        else if (tl.iY < layouRect.iTl.iY)
            {
            tl.iY = layouRect.iTl.iY;
            }
            
        SetRect( TRect(tl, temp) );
        }
    }


// ---------------------------------------------------------------------------
// CAknFepHwrBxLayout::OnPointerEventOutsideLayout
// Call back when the pointer down/up event happends outside of the layout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//       
TBool CAknFepHwrBxLayout::OnPointerEventOutsideLayout(const TRawEvent& aEvent)
    {
    if (aEvent.Type() == TRawEvent::EButton1Down)                
        {
        HandleControlEvent(EHwrEventPointerOutsideWnd, NULL, KNullDesC);
        }
    //forward the event
    return EFalse;    
    }
// ---------------------------------------------------------------------------
// CAknFepHwrBxLayout::PenInputUiType
// return current input UI type
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CAknFepHwrBxLayout::PenInputType()
    {
    return EPluginInputModeHwr;
    }

void CAknFepHwrBxLayout::SetPositionFromOutside(const TPoint& aNewPos)
    {
    SetLayoutPos(aNewPos);
    iDataStore->SaveWindowPos(aNewPos);
    }
            
// ---------------------------------------------------------------------------
// CAknFepHwrBxLayout::GoToStandbyStateL
// return current input UI type
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepHwrBxLayout::GoToStandbyStateL( TInt aOpenFlag )
    {
    CAknFepHwrBxStateBase* state = NULL;
    if ( aOpenFlag == EPenInputOpenManually)
        {
        iBxWindow->SetPropertyL(MAknFepHwrPropertySubscriber::EAknFepHwrPropertySizeChanged, 
                                KNullDesC);
        }
    state = CAknFepHwrBxStateStandby::NewLC( this );
    SetCurrentStateL(state);
    CleanupStack::Pop(state);//state                
    }
// End Of File
