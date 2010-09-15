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
* Description:  drop-down list control
*
*/


#include <AknFepGlobalEnums.h>
#include <peninputdropdownlist.h>
#include <PtiEngine.h>
#include <aknfeppeninputenums.h>

#include "peninputsplititutchnuimgr.h"
#include "peninputsplititutlayout.h"
#include "peninputsplititutlayoutcontext.h"
#include "peninputsplititutdatamgr.h"
#include "peninputsplititutchnuistatestandby.h"
#include "peninputsplititutchnuistatepredict.h"
#include "peninputsplititutchnuistatestrokezhuyincomposition.h"
#include "peninputsplititutchnuistatepinyincomposition.h"
#include "peninputsplititutwesternuistatenonpredict.h"
#include "peninputsplititutwindowmanager.h"


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CChnSplitItutUiMgr::NewL
// ---------------------------------------------------------------------------
//
CChnSplitItutUiMgr* CChnSplitItutUiMgr::NewL(MSplitItutLayoutContext* aContext, 
                                   CSplitItutDataMgr* aDataMgr,
                                   CSplitItutWindowManager* aUiManager)
    {
    CChnSplitItutUiMgr* self = new (ELeave) CChnSplitItutUiMgr(aContext, aDataMgr,aUiManager);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiMgr::~CChnSplitItutUiMgr
// ---------------------------------------------------------------------------
//
CChnSplitItutUiMgr::~CChnSplitItutUiMgr()
    {
    if (iCurrentState)
        {
        iCurrentState->OnExit();
        }

    delete iStandbyState;
    delete iPinyinComp;
    delete iStoZhuComp;
    delete iPredictState;
    
    delete iNormalState;   
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiMgr::CChnSplitItutUiMgr
// ---------------------------------------------------------------------------
//
CChnSplitItutUiMgr::CChnSplitItutUiMgr(MSplitItutLayoutContext* aContext,
                             CSplitItutDataMgr* aDataMgr,
                             CSplitItutWindowManager* aUiManager)
                            :CSplitItutUiMgrBase(aContext, aDataMgr,aUiManager)
    {
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiMgr::ConstructL
// ---------------------------------------------------------------------------
//    
void CChnSplitItutUiMgr::ConstructL()
    {
    CSplitItutUiMgrBase::ConstructL();
    
    iStandbyState = CChnSplitItutUiStateStandby::NewL(this);
    iPinyinComp = CChnSplitItutUiStatePinyinComposition::NewL(this);
    iStoZhuComp = CChnSplitItutUiStateStrokeZhuyinComposition::NewL(this);
    iPredictState = CChnSplitItutUiStatePredict::NewL(this);
    
    iNormalState = CWesternSplitItutUiStateNonPredict::NewL(this);
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiMgr::HandleCommandL
// ---------------------------------------------------------------------------
//    
TInt CChnSplitItutUiMgr::HandleCommandL(TInt aCmd, TUint8* aData)
    {
    if (aCmd == ECmdPenInputFingerMatchSelection )
        {
        if (!(*(reinterpret_cast<TBool*>(aData))))
            {
            // state not on
            if (!IsChineseMode())
                {
                SetCurrentStateL(CSplitItutUiMgrBase::EStateWesternStandby);    
                }
            return KErrNone;
            }
        }
        
    TUint* data = (TUint*) aData;        
    
    switch (aCmd)
        {      
        case ECmdPenInputFingerKeyPress:
            {
            if (iCurrentState && iCurrentState->HandleKeyL(aCmd, *data))
                {
                return KErrNone;
                }
            
            return KErrNotSupported;    
            }
        case ECmdPenInputFingerLongKeyPress:
            {
            if (!IsChineseMode())
                {
                return KErrNone;   
                }
            
            if (*data >= EPtiKey0 && *data <= EPtiKey9)
                {
                HandleLongPressNumberKeyL(TPtiKey(*data));

                // long key press
                if (iCurrentState->StateType() == CSplitItutUiMgrBase::EStateComposition)
                    {
                    return KErrNone;
                    }
                }
            else if (iCurrentState && iCurrentState->HandleKeyL(aCmd, *data))    
                {
                return KErrNone;
                }

            // means not handled by ui
            return KErrNotSupported;
            }
        case ECmdPenInputDeActive:            
        	{
        	if (!iCurrentState)
        	    {
        	    return KErrNone;   
        	    }
        	    
        	TBool changeState = EFalse;
        	TUiState state;
        	 
        	if (IsChineseMode())
        	    {
        	     iCurrentState->StateType() != CSplitItutUiMgrBase::EStateStandby ?
        	        changeState = ETrue : changeState = EFalse;
        	    
        	    state = CSplitItutUiMgrBase::EStateStandby;  
        	    }
        	
            if (changeState)
            	{
            	SetCurrentStateL(state);	
            	}
            }
            break;
        case ECmdPenInputNoFreeSpace:
            {
        	if (!iCurrentState)
        	    {
        	    return KErrNone;   
        	    }
        	    
        	if (IsChineseMode() &&
        	    iCurrentState->StateType() != CSplitItutUiMgrBase::EStateStandby)
    	        {
            	SetCurrentStateL(CSplitItutUiMgrBase::EStateStandby);	
    	        }
            }
            break;
        case ECmdPenInputCase:
            {
            if (iCurrentState)// && !IsChineseMode())
                {
                iLayoutContext->DataMgr()->SetCase(*((TUint*) aData));
                return KErrNone;
                }
            }
            break;
        case ECmdPenInputFingerMatchList:
            {
            if (iCurrentState)
                {
                iCurrentState->HandleCommandL(aCmd, aData);
                }
            }
            break;
        case ECmdPenInputIsSecretText:
            {
            if (iCurrentState)
                {
                iCurrentState->HandleCommandL(aCmd, aData);
                }
            }
            break;
        default:
            break;
        }

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiMgr::HandleCtrlEventL
// ---------------------------------------------------------------------------
//    
TBool CChnSplitItutUiMgr::HandleCtrlEventL(TInt aEventType, 
                                      CFepUiBaseCtrl* aCtrl, 
                                      const TDesC& aEventData)
    {
    if (!iCurrentState)
        {
        return EFalse;
        }
        
    if (!IsChineseMode())
        {
        return iCurrentState->HandleCtrlEventL(aEventType, aCtrl, aEventData);    
        }

    if (!iCurrentState->HandleCtrlEventL(aEventType, aCtrl, aEventData))
        {
        switch (aEventType)
            {
            case EEventRawKeyUpEvent:
                {
                if (aCtrl->ControlId() == ECtrlIdBackspace)
                    {
                    // if clear key released
                    SetClrConsume(EFalse); 
                    }
                else
                    {
                    // if number key released
                    const TKeyEvent *key = reinterpret_cast<const TKeyEvent*>(aEventData.Ptr());

                    if ((key->iScanCode >= EPtiKey0) && (key->iScanCode <= EPtiKey9))
                        {
                        SetNumLongPressValid(EFalse);
                        }
                    }
                }
                break;
            default:
                break;
            }

        return EFalse;
        }
    else
        {
        // handled by state
        return ETrue;
        }
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiMgr::SetCurrentStateL
// ---------------------------------------------------------------------------
//    
void CChnSplitItutUiMgr::SetCurrentStateL(TUiState aNewStateType)
    {
    if (iCurrentState && 
        aNewStateType == iCurrentState->StateType())
    	{
        return;    		
    	}    
    
    CSplitItutUiStateBase* newState = NULL;

    switch (aNewStateType)
        {
        case CSplitItutUiMgrBase::EStateStandby:
            {
            newState = iStandbyState;
            }
            break;
        case CSplitItutUiMgrBase::EStateComposition:
            {
            newState = (DataMgr()->InputMode() == EPinyin) ? 
                iPinyinComp : iStoZhuComp;
            }
            break;
        case CSplitItutUiMgrBase::EStatePredict:
            {
            newState = iPredictState;
            }
            break;
        case CSplitItutUiMgrBase::EStateWesternStandby:
            {
            newState = iNormalState;
            }
            break;
        default:    
            return;
        }

    if (iCurrentState)
        {
        iCurrentState->OnExit();
        }

    iCurrentState = newState;
    if(DataMgr()->IsUpdate())  
    	{
    	DataMgr()->SetUpdate(EFalse);
    	
    	// Get the screen rect
    	TRect screenRect = DataMgr()->screenSize();
    	// If the screen is in portrait mode
    	if ( screenRect.Width() < screenRect.Height())
    		{
			CSplitItutUiLayout* itutLayout = 
				static_cast<CSplitItutUiLayout*>( LayoutContext()->UiLayout());
			itutLayout->SizeChanged( NULL );
    		}
    	}
    
    iCurrentState->OnEntryL();
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiMgr::ActivateUiL
// ---------------------------------------------------------------------------
//    
void CChnSplitItutUiMgr::ActivateUiL()
    {
    
    if (iCurrentState && !IsChineseMode())
        {
        iCurrentState->OnEntryL();
        return;
        }    
    
    switch (DataMgr()->InputMode())
        {
        case EPinyin:
        case EStroke:
        case EZhuyin:
            SetCurrentStateL(CSplitItutUiMgrBase::EStateStandby);
            break;
        default:
            SetCurrentStateL(CSplitItutUiMgrBase::EStateWesternStandby);
            break;    
        }
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiMgr::DeactivateUI
// ---------------------------------------------------------------------------
//    
void CChnSplitItutUiMgr::DeactivateUI()
    {
    if (iCurrentState)
        {
        iCurrentState->OnExit();
        iCurrentState = NULL;
        }

    SetNumLongPressValid(EFalse);
    }

// ---------------------------------------------------------------------------
// CChnSplitItutUiMgr::HandleLongPressNumberKeyL
// ---------------------------------------------------------------------------
//    
void CChnSplitItutUiMgr::HandleLongPressNumberKeyL(TPtiKey /*aKey*/)
    {
    if (NumLongPressValid())
        {
        // go to standby state
        SetCurrentStateL(CSplitItutUiMgrBase::EStateStandby);

        DataMgr()->PtiEngine()->ClearCurrentWord();

        SetNumLongPressValid(EFalse);
        }
    }
  
// ---------------------------------------------------------------------------
// CChnSplitItutUiMgr::IsChineseMode
// ---------------------------------------------------------------------------
//    
TBool CChnSplitItutUiMgr::IsChineseMode()
    {
    TInt immode = DataMgr()->InputMode();
    TBool chineseMode = EFalse;
    
    if (immode == EPinyin || immode == EStroke || immode == EZhuyin)
        {
        chineseMode = ETrue;
        }
    
    return chineseMode;    
    }

// End Of File
