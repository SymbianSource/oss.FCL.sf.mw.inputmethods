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
* Description:  peninput VKB data manager
*
*/

#include <AknDef.h>
#include <AknFepGlobalEnums.h>
#include <aknfeppeninputenums.h>
#include <peninputcommonlayoutglobalenum.h>
#include <peninputuistatemgr.h>
#include <peninputlayoutbubblectrl.h>
#include <peninputlayoutmultilineicf.h>
#include <PtiEngine.h>

#include "PeninputVkbKrDbg.h"
#include "PeninputVkbKrLayout.h"
#include "PeninputVkbKrWindow.h"
#include "PeninputVkbKrDataMgr.h"

#include "PeninputVkbKrUiStateType.h"
#include "PeninputVkbKrUiStateNone.h"
#include "PeninputVkbKrUiStateStandby.h"

/* Teleca change begin, 21.05.2009 ssal */
const TInt KPeriodicTimerIntervalSec(2500000);
/* Teleca change end, 21.05.2009 ssal */
// ---------------------------------------------------------------------------
// Constructors & Destructors
// ---------------------------------------------------------------------------
//

CPeninputVkbKrLayout* CPeninputVkbKrLayout::NewL(
        MLayoutOwner* aLayoutOwner,
        const TAny* aInitData,
        const TInt& aInputMode)
    {
    CPeninputVkbKrLayout* self = 
        new (ELeave) CPeninputVkbKrLayout(aLayoutOwner,aInputMode);
    CleanupStack::PushL(self);
    self->ConstructL(aInitData);
    CleanupStack::Pop(self);
    return self;
    }

CPeninputVkbKrLayout::CPeninputVkbKrLayout(
        MLayoutOwner* aLayoutOwner,
        const TInt& aInputMode)
    : CPeninputCommonLayoutExt(aLayoutOwner),
      iInputMode(aInputMode),
      iInEditWordQueryDlg(EFalse)
    {
    }

void CPeninputVkbKrLayout::ConstructL(const TAny* aInitData)
    {
    LOG("");
    LOG("Vkb.Layout.ConstructL==================================");
    
    iPtiEngine=CPtiEngine::NewL();
#ifdef RD_INTELLIGENT_TEXT_INPUT
    iPtiEngine->ActivateLanguageL(ELangKorean,EPtiEngineQwertyKorean);
#else
    iPtiEngine->ActivateLanguageL(ELangKorean,EPtiEngineQwerty);
#endif //RD_INTELLIGENT_TEXT_INPUT
    
    CPeninputCommonLayoutExt::ConstructL(aInitData);
    CPeninputVkbKrUiStateNone* stateNone=
        CPeninputVkbKrUiStateNone::NewL(UiStateMgr(),this,*iPtiEngine);
    UiStateMgr()->AddUiState(stateNone,EPeninputVkbKrUiStateNone);
    CPeninputVkbKrUiStateStandby* stateStandby=
        CPeninputVkbKrUiStateStandby::NewL(UiStateMgr(),this,*iPtiEngine);
    UiStateMgr()->AddUiState(stateStandby,EPeninputVkbKrUiStateStandby);
    UiStateMgr()->SetCurrentUiState(EPeninputVkbKrUiStateNone);
    
    /* Teleca change begin, 21.05.2009 ssal */
    iInfoTimer = CPeriodic::NewL(CActive::EPriorityStandard);
    /* Teleca change end, 21.05.2009 ssal */
    }

CPeninputVkbKrLayout::~CPeninputVkbKrLayout()
    {
    delete iPtiEngine;
    
    /* Teleca change begin, 21.05.2009 ssal */
    if (iInfoTimer)
    	{
    	iInfoTimer->Cancel();
    	}
   	delete iInfoTimer;
    /* Teleca change end, 21.05.2009 ssal */ 
    }

// ---------------------------------------------------------------------------
// from CFepUiLayout
// ---------------------------------------------------------------------------
//

TBool CPeninputVkbKrLayout::HandleEventL(
        TEventType aType, 
        const TAny* aData)
    {
    LOG1("Vkb.Layout.HandleEventL %d",aType);
    return CPeninputCommonLayoutExt::HandleEventL(aType,aData);
    }

TInt CPeninputVkbKrLayout::HandleCommand(TInt aCmd, TUint8* aData)
    {
    CPeninputVkbKrWindow* vkbWindow = 
        static_cast<CPeninputVkbKrWindow*>(iLayoutWindow);
    switch ( aCmd )
        {
        case ECmdPenInputRange:
            vkbWindow->PreProcessRange(*aData);
            break;
        default:
            break;
        }
    
    TInt ret(CPeninputCommonLayoutExt::HandleCommand(aCmd,aData));
    switch ( aCmd )
        {
        case ECmdPenInputPermittedRange:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputPermittedRange");
            break;
        case ECmdPenInputEditorNumericKeyMap:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputEditorNumericKeyMap");
            break;
        case ECmdPenInputCase:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputCase");
            break;
        case ECmdPenInputLanguage:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputLanguage");
            UiStateMgr()->CurrentUiState()->HandleControlEvent(EPeninputLayoutEventOpen,KNullDesC);
            break;
        case ECmdPenInputEditorState:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputEditorState");
            break;
        case ECmdPenInputSetWindowPos:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputSetWindowPos");
            break;
        case ECmdPenInputWindowOpen:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputWindowOpen");
            UiStateMgr()->SetCurrentUiState(EPeninputVkbKrUiStateStandby);
            UpdateArea(vkbWindow,vkbWindow->Rect(),ETrue);
            if( !iIsOpenVkbKr )
                {
                iKeyBoardType = iPtiEngine->KeyboardType();
                iPtiEngine->SetKeyboardType(EPtiKeyboardQwerty3x11);
                }
            iIsOpenVkbKr = ETrue;
            ret=KErrNone;
            break;
        case ECmdPenInputWindowClose:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputWindowClose");
            iIsOpenVkbKr = EFalse;
            iPtiEngine->SetKeyboardType(iKeyBoardType);
            UiStateMgr()->CurrentUiState()->HandleControlEvent(EPeninputLayoutEventClose,KNullDesC);
            UiStateMgr()->SetCurrentUiState(EPeninputVkbKrUiStateNone);
            ret=KErrNone;
            break;
        case ECmdPenInputAllowChangeCase:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputAllowChangeCase");
            break;
        case ECmdPenInputCaseMode:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputCaseMode");
            break;
        case ECmdPenInputRange:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputRange");
            break;
        case ECmdPenInputPermittedCase:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputPermittedCase");
            break;
        case ECmdPenInputPluginInputMode:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputPluginInputMode");
            break;
        case ECmdPenInputEnableSettingBtn:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputEnableSettingBtn");
            break;
        case ECmdPenInputEditorCustomNumericKeyMap:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputEditorCustomNumericKeyMap");
            break;
        case ECmdPenInputSuggestRange:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputSuggestRange");
            break;
        case ECmdPenInputIsSecretText:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputIsSecretText");
            vkbWindow->SetSwitchlistSecretFlag(*aData);
            break;
        case ECmdPenInputIsNumberGrouping:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputIsNumberGrouping");
            break;
        case ECmdPenInputDimArrowKeys:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputDimArrowKeys");
            vkbWindow->DimArrowKeys( *aData );
            break;
        case ECmdPenInputSendEditorTextAndCurPos:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputSendEditorTextAndCurPos");
            break;
        case ECmdPenInputJapaneseSetting:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputJapaneseSetting");
            break;
        case ECmdPenInputJapaneseSetSubRange:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputJapaneseSetSubRange");
            break;
        case ECmdPenInputJapaneseGetModeOfSubRange:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputJapaneseGetModeOfSubRange");
            break;
        case ECmdPenInputFingerInputMode:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputFingerInputMode");
            break;
        case ECmdPenInputFingerOpenSCT:
            LOG("Vkb.Layout.HandleCommand ECmdPenInputFingerOpenSCT");
            break;
        case ECmdPenInputSetPromptText:
            {
            if ( iInputMode == EPluginInputModeFSQ )
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

        default:
            LOG1("Vkb.Layout.HandleCommand %d",aCmd);
            break;
        }
    return ret;
    }

void CPeninputVkbKrLayout::HandleControlEvent(
        TInt aEventType, 
        CFepUiBaseCtrl* aCtrl, 
        const TDesC& aEventData)
    {
    TInt* data = (TInt*)aEventData.Ptr();

    switch (aEventType)
        {
        case EEventButtonDown:
            LOG("Vkb.Layout.HandleControlEvent EEventButtonDown");
            break;
        case EEventButtonUp:
            LOG("Vkb.Layout.HandleControlEvent EEventButtonUp");
            break;
        case EEventDraggingStart:
            LOG("Vkb.Layout.HandleControlEvent EEventDraggingStart");
            break;
        case EEventDragging:
            LOG("Vkb.Layout.HandleControlEvent EEventDragging");
            break;
        case EEventDraggingEnd:
            LOG("Vkb.Layout.HandleControlEvent EEventDraggingEnd");
            break;
        case EEventVirtualKeyUp:
            LOG("Vkb.Layout.HandleControlEvent EEventVirtualKeyUp");
            break;
        case EEventVirtualKeyDown:
            LOG("Vkb.Layout.HandleControlEvent EEventVirtualKeyDown");
            break;
        case EEventHwrStrokeStarted:
            LOG("Vkb.Layout.HandleControlEvent EEventHwrStrokeStarted");
            break;
        case EEventHwrStrokeFinished:
            LOG("Vkb.Layout.HandleControlEvent EEventHwrStrokeFinished");
            break;
        case EEventHwrCharacterTimerOut:
            LOG("Vkb.Layout.HandleControlEvent EEventHwrCharacterTimerOut");
            break;
        case EEventCandidateSelected:
            LOG("Vkb.Layout.HandleControlEvent EEventCandidateSelected");
            break;
        case EEventButtonLongPress:
            LOG("Vkb.Layout.HandleControlEvent EEventButtonLongPress");
            break;
        case EEventSizeChanged:
            LOG("Vkb.Layout.HandleControlEvent EEventSizeChanged");
            break;
        case EEventRegionUpdated:
            LOG("Vkb.Layout.HandleControlEvent EEventRegionUpdated");
            break;
        case EEventSetAppCursorSelection:
            LOG("Vkb.Layout.HandleControlEvent EEventSetAppCursorSelection");
            break;
        case EEventControlFocusGained:
            LOG("Vkb.Layout.HandleControlEvent EEventControlFocusGained");
            break;
        case EEventControlFocusLost:
            LOG("Vkb.Layout.HandleControlEvent EEventControlFocusLost");
            break;
        case EEventButtonDownCanceled:
            LOG("Vkb.Layout.HandleControlEvent EEventButtonDownCanceled");
            break;
        case EEventHwrStrokeCanceled:
            LOG("Vkb.Layout.HandleControlEvent EEventHwrStrokeCanceled");
            break;
        case EEventControlDestroyed:
            LOG("Vkb.Layout.HandleControlEvent EEventControlDestroyed");
            break;
        case EEventChoiceSelected:
            LOG("Vkb.Layout.HandleControlEvent EEventChoiceSelected");
            break;
        case EEventPointerLeftControl:
            LOG("Vkb.Layout.HandleControlEvent EEventPointerLeftControl");
            break;
        case EEventPointerEnterControl:
            LOG("Vkb.Layout.HandleControlEvent EEventPointerEnterControl");
            break;
        case EEventVirtualKeyLatched:
            LOG("Vkb.Layout.HandleControlEvent EEventVirtualKeyLatched");
            break;
        case EEventVirtualKeyUnLatched:
            LOG("Vkb.Layout.HandleControlEvent EEventVirtualKeyUnLatched");
            break;
        case EEventICFSelectionChanged:
            LOG("Vkb.Layout.HandleControlEvent EEventICFSelectionChanged");
            break;
        case EEventRawKeyDownEvent:
            LOG("Vkb.Layout.HandleControlEvent EEventRawKeyDownEvent");
            break;
        case EEventRawKeyUpEvent:
            LOG("Vkb.Layout.HandleControlEvent EEventRawKeyUpEvent");
            break;
        case EEventPointerDownOnNomatch:
            LOG("Vkb.Layout.HandleControlEvent EEventPointerDownOnNomatch");
            break;
        case EEventPointerDownOnInline:
            LOG("Vkb.Layout.HandleControlEvent EEventPointerDownOnInline");
            break;
        case EEventCapturePointer:
            LOG("Vkb.Layout.HandleControlEvent EEventCapturePointer");
            break;
        case EEventIcfPointerUpEvent:
            LOG("Vkb.Layout.HandleControlEvent EEventIcfPointerUpEvent");
            break;
        case EEventControlExtBase:
            LOG("Vkb.Layout.HandleControlEvent EEventControlExtBase");
            break;
        case EEventControlUserBase:
            LOG("Vkb.Layout.HandleControlEvent EEventControlUserBase");
            break;
        default:
            LOG1("Vkb.Layout.HandleControlEvent %d",aEventType);
            break;
        }
    if (aEventType==EPeninputLayoutEventOption)
        {
        iUiStateMgr->CurrentUiState()->HandleControlEvent(
            aEventType,
            aEventData);
        }
    CPeninputCommonLayoutExt::HandleControlEvent(
        aEventType, 
        aCtrl, 
        aEventData);
    }

TInt CPeninputVkbKrLayout::OnAppEditorTextComing(
        const TFepInputContextFieldData& aData)
    {
    LOG1("Vkb.Layout.OnAppEditorTextComing %d",aData.iCmd);
    return CPeninputCommonLayoutExt::OnAppEditorTextComing(aData);
    }

TBool CPeninputVkbKrLayout::HandleRawKeyEventL(
        const TRawEvent& aKeyEvent)
    {
    LOG1("Vkb.Layout.HandleRawKeyEventL %d",aKeyEvent.ScanCode());
    return CPeninputCommonLayoutExt::HandleRawKeyEventL(aKeyEvent);
    }


TBool CPeninputVkbKrLayout::OnPointerEventOutsideLayout(
        const TRawEvent& /*aEvent*/)
    {
    LOG("Vkb.Layout.OnPointerEventOutsideLayout");
    UiStateMgr()->CurrentUiState()->HandleControlEvent(EPeninputLayoutEventOpen,KNullDesC);
    return EFalse;
    }

TInt CPeninputVkbKrLayout::OnResourceChange(TInt aType)
    {
    LOG("Vkb.Layout.OnResourceChange");
    /*
    if (aType==KEikDynamicLayoutVariantSwitch)
        {
        UiStateMgr()->CurrentUiState()->HandleControlEvent(EPeninputLayoutEventMovePosition,KNullDesC);                
        }
    */
    return CPeninputCommonLayoutExt::OnResourceChange(aType);    
    }

// ---------------------------------------------------------------------------
// from CPeninputCommonLayout
// ---------------------------------------------------------------------------
//

void CPeninputVkbKrLayout::CreateLayoutWindowL()
    {
    iLayoutWindow = CPeninputVkbKrWindow::NewL( this, this, UiStateMgr() );
    }

void CPeninputVkbKrLayout::CreateDataMgrL( const TAny* /*aInitData*/ )
    {
    iDataMgr = CPeninputVkbKrDataMgr::NewL(this,*iPtiEngine);
    }

TInt CPeninputVkbKrLayout::LayoutType()
    {
    return iInputMode;
    }

TInt CPeninputVkbKrLayout::SizeChanged( const TAny* aData )
    {
    LOG("Vkb.Layout.SizeChanged");
    (( CPeninputVkbKrWindow*)iLayoutWindow)->UpdateLafData();
    return CPeninputCommonLayoutExt::SizeChanged( aData );
    }

void CPeninputVkbKrLayout::HandleAppInfoChange(
        const TDesC& aInfo, 
        TPeninputAppInfo aType)
    {
    LOG("Vkb.Layout.HandleAppInfoChange");
    if ( LayoutWindow() )
        {
        CFepLayoutMultiLineIcf* icf = static_cast<CFepLayoutMultiLineIcf*> 
                                    (LayoutWindow()->Control(EPeninputWindowCtrlIdMultiLineICF)) ;
        
        if ( icf && ( aType == EAppIndicatorMsg ) && (iInputMode == EPluginInputModeFSQ) )
            { 
            if ( aInfo.Length() > 0 && !iInEditWordQueryDlg)
                {
                icf->ShowBubble(aInfo,icf->MsgBubbleCtrl()->Rect());
                }
            else
                {
                icf->HideBubble();
                }
            }
        /* Teleca change begin, 19.05.2009 ssal */
        else if ( icf && ( aType == EAppByteWarningMsg ) && (iInputMode == EPluginInputModeFSQ) )
            { 
			if (!iInfoTimer->IsActive())
				{
				iInfoTimer->Start(KPeriodicTimerIntervalSec,
                          		  KPeriodicTimerIntervalSec, 
                          		  TCallBack(CPeninputVkbKrLayout::HideByteWarningBubble, this));                  		 
				}    
			icf->ShowByteWarningBubble(aInfo);                               		
    		}
		/* Teleca change end, 19.05.2009 ssal */  
		/* Teleca change begin, 09.09.2009 ssal */
		
        else if ( icf && ( aType == EAppMessageTypeChangeMsg ) && (iInputMode == EPluginInputModeFSQ) )
            { 
			if (!iInfoTimer->IsActive())
				{
				if ( aInfo.Length() > 0 && !iInEditWordQueryDlg)
                	{
                	icf->ShowBubble(aInfo,icf->MsgBubbleCtrl()->Rect());
                	}
				
				iInfoTimer->Start(KPeriodicTimerIntervalSec,
                          		  KPeriodicTimerIntervalSec, 
                          		  TCallBack(CPeninputVkbKrLayout::HideByteWarningBubble, this));     		  
				}
			icf->ShowByteWarningBubble(aInfo);	                                       
    		}
		/* Teleca change end, 09.09.2009 ssal */             
        }
    }
/* Teleca change begin, 21.05.2009 ssal */
TInt CPeninputVkbKrLayout::HideByteWarningBubble(TAny* aPointer)
    {
    CPeninputVkbKrLayout* layout = static_cast<CPeninputVkbKrLayout*>(aPointer);
    layout->HideByteWarningBubble();
    return KErrNone;
    }
    
void CPeninputVkbKrLayout::HideByteWarningBubble()
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
/* Teleca change end, 18.05.2009 ssal */
