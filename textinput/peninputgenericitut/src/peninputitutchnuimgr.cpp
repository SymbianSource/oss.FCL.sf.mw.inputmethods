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

#include "peninputitutchnuimgr.h"
#include "peninputgenericitutlayout.h"
#include "peninputgenericitutlayoutcontext.h"
#include "peninputgenericitutdatamgr.h"
#include "peninputitutchnuistatestandby.h"
#include "peninputitutchnuistatepredict.h"
#include "peninputitutchnuistatestrokezhuyincomposition.h"
#include "peninputitutchnuistatepinyincomposition.h"
#include "peninputitutwesternuistatespelling.h"
#include "peninputitutwesternuistatenonpredict.h"
#include "peninputitutwesternuistatematchselection.h"
#include "peninputgenericitutwindowmanager.h"

CChnItutUiMgr* CChnItutUiMgr::NewL(MItutLayoutContext* aContext, 
                                   CGenericItutDataMgr* aDataMgr,
                                   CGenericItutWindowManager* aUiManager)
    {
    CChnItutUiMgr* self = new (ELeave) CChnItutUiMgr(aContext, aDataMgr,aUiManager);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop(self);
    return self;
    }

CChnItutUiMgr::~CChnItutUiMgr()
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
    delete iMatchSelState;
    delete iSpellState;    
    }

CChnItutUiMgr::CChnItutUiMgr(MItutLayoutContext* aContext,
                             CGenericItutDataMgr* aDataMgr,
                             CGenericItutWindowManager* aUiManager)
                            :CGenericItutUiMgrBase(aContext, aDataMgr,aUiManager)
    {
    }
    
void CChnItutUiMgr::ConstructL()
    {
    CGenericItutUiMgrBase::ConstructL();
    
    iStandbyState = CChnItutUiStateStandby::NewL(this);
    iPinyinComp = CChnItutUiStatePinyinComposition::NewL(this);
    iStoZhuComp = CChnItutUiStateStrokeZhuyinComposition::NewL(this);
    iPredictState = CChnItutUiStatePredict::NewL(this);
    
    iNormalState = CWesternItutUiStateNonPredict::NewL(this);
    iMatchSelState = CWesternItutUiStateMatchSelection::NewL(this);
    iSpellState = CWesternItutUiStateSpelling::NewL(this);    
    }

TInt CChnItutUiMgr::HandleCommandL(TInt aCmd, TUint8* aData)
    {
    if (aCmd == EItutExtCmdCustomerState || 
        aCmd == ECmdPenInputFingerMatchSelection ||
        aCmd == ECmdPenInputFingerSpelling)
        {
        if (!(*(reinterpret_cast<TBool*>(aData))))
            {
            // state not on
            if (!IsChineseMode())
                {
                SetCurrentStateL(CGenericItutUiMgrBase::EStateWesternStandby);    
                }
            return KErrNone;
            }
        }
        
    TUint* data = (TUint*) aData;        
    
    switch (aCmd)
        {
        case ECmdPenInputFingerMatchSelection:
            {
            SetCurrentStateL(CGenericItutUiMgrBase::EStateMatchSelection);
            return KErrNone;    
            }
        case ECmdPenInputFingerSpelling:
            {
            SetCurrentStateL(CGenericItutUiMgrBase::EStateSpelling);
            UiManager()->ResetIndicatorL(UiManager()->Control(ECtrlIdSpellIndicator));
            return KErrNone;
            }        
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
                if (iCurrentState->StateType() == CGenericItutUiMgrBase::EStateComposition)
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
        	     iCurrentState->StateType() != CGenericItutUiMgrBase::EStateStandby ?
        	        changeState = ETrue : changeState = EFalse;
        	    
        	    state = CGenericItutUiMgrBase::EStateStandby;  
        	    }
        	else
        	    {
        	    iCurrentState->StateType() == CGenericItutUiMgrBase::EStateMatchSelection ?
        	    changeState = ETrue : changeState = EFalse;
        	    
        	    state = CGenericItutUiMgrBase::EStateWesternStandby;
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
        	    iCurrentState->StateType() != CGenericItutUiMgrBase::EStateStandby)
    	        {
            	SetCurrentStateL(CGenericItutUiMgrBase::EStateStandby);	
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
        case EItutCmdCheckIcfEmpty:
            {
            if (iCurrentState && 
                iCurrentState->StateType() == CGenericItutUiMgrBase::EStateSpelling)
                {
                iCurrentState->HandleCommandL(aCmd, aData);
                }
                
            }
            break;
    	case EItutExtCmdSizeChanged:
   			{
   			if(iCurrentState)
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

TBool CChnItutUiMgr::HandleCtrlEventL(TInt aEventType, 
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

void CChnItutUiMgr::SetCurrentStateL(TUiState aNewStateType)
    {
    if (iCurrentState && 
        aNewStateType == iCurrentState->StateType())
    	{
        return;    		
    	}    
    
    CGenericItutUiStateBase* newState = NULL;

    switch (aNewStateType)
        {
        case CGenericItutUiMgrBase::EStateStandby:
            {
            newState = iStandbyState;
            }
            break;
        case CGenericItutUiMgrBase::EStateComposition:
            {
            newState = (DataMgr()->InputMode() == EPinyin) ? 
                iPinyinComp : iStoZhuComp;
            }
            break;
        case CGenericItutUiMgrBase::EStatePredict:
            {
            newState = iPredictState;
            }
            break;
        case CGenericItutUiMgrBase::EStateWesternStandby:
            {
            newState = iNormalState;
            }
            break;
        case CGenericItutUiMgrBase::EStateMatchSelection:
            {
            newState = iMatchSelState;
            }
            break;
        case CGenericItutUiMgrBase::EStateSpelling:
            {
            newState = iSpellState;
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
			CGenericItutUiLayout* itutLayout = 
				static_cast<CGenericItutUiLayout*>( LayoutContext()->UiLayout());
			itutLayout->SizeChanged( NULL );
    		}
    	}
    
    iCurrentState->OnEntryL();
    }

void CChnItutUiMgr::ActivateUiL()
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
            SetCurrentStateL(CGenericItutUiMgrBase::EStateStandby);
            break;
        default:
            SetCurrentStateL(CGenericItutUiMgrBase::EStateWesternStandby);
            break;    
        }
    }

void CChnItutUiMgr::DeactivateUI()
    {
    if (iCurrentState)
        {
        iCurrentState->OnExit();
        iCurrentState = NULL;
        }

    SetNumLongPressValid(EFalse);
    }

void CChnItutUiMgr::HandleLongPressNumberKeyL(TPtiKey /*aKey*/)
    {
    if (NumLongPressValid())
        {
        // go to standby state
        SetCurrentStateL(CGenericItutUiMgrBase::EStateStandby);

        DataMgr()->PtiEngine()->ClearCurrentWord();

        SetNumLongPressValid(EFalse);
        }
    }
    
TBool CChnItutUiMgr::IsChineseMode()
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
