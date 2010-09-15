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


#include "peninputitutwesternuimgr.h"
#include "peninputgenericitutdata.h"
#include "peninputgenericitutdatamgr.h"
#include "peninputitutwesternuistatenonpredict.h"
#include "peninputitutwesternuistatecusotmization.h"
#include "peninputitutwesternuistatematchselection.h"
#include "peninputgenericitutlayoutcontext.h"
#include "peninputgenericitutcustomization.h"
#include "peninputitutcustomizationresourcemgr.h"
#include "peninputitutwesternuistatespelling.h"
#include "peninputgenericitutlayout.h"
#include "peninputgenericitutwindowmanager.h"

CWesternItutUiMgr* CWesternItutUiMgr::NewL(MItutLayoutContext* aContext, 
                                           CGenericItutDataMgr* aDataMgr,
                                           CGenericItutWindowManager* aUiManager)
    {
    CWesternItutUiMgr* self = new (ELeave) CWesternItutUiMgr(aContext, aDataMgr,aUiManager);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop(self);
    return self;
    }

CWesternItutUiMgr::~CWesternItutUiMgr()
    {
    if (iCurrentState)
        {
        iCurrentState->OnExit();
        }
        
    delete iNormalState;
    delete iCustomerState;
    delete iMatchSelState;
    delete iSpellState;
    }

CWesternItutUiMgr::CWesternItutUiMgr(MItutLayoutContext* aContext,
                                     CGenericItutDataMgr* aDataMgr,
                                     CGenericItutWindowManager* aUiManager)
                                    :CGenericItutUiMgrBase(aContext, aDataMgr,aUiManager)
    {
    iDataMgr = aDataMgr;
    }

void CWesternItutUiMgr::ConstructL()
    {
    CGenericItutUiMgrBase::ConstructL();

    iNormalState = CWesternItutUiStateNonPredict::NewL(this);
    iCustomerState = CWesternItutUiStateCustomization::NewL(this);
    iMatchSelState = CWesternItutUiStateMatchSelection::NewL(this);
    iSpellState = CWesternItutUiStateSpelling::NewL(this);
    }

TBool CWesternItutUiMgr::HandleCtrlEventL(TInt aEventType, 
                                          CFepUiBaseCtrl* aCtrl, 
                                          const TDesC& aEventData)
    {
    if (!iCurrentState)
        {
        return EFalse;
        }

    return iCurrentState->HandleCtrlEventL(aEventType, aCtrl, aEventData);
    }

void CWesternItutUiMgr::SetCurrentStateL(TUiState aNewState)
    {
    if (iCurrentState && aNewState == iCurrentState->StateType())
    	{
        return;    		
    	}
    
    CGenericItutUiStateBase* newstate = NULL;

    switch (aNewState)
        {
        case CGenericItutUiMgrBase::EStateWesternStandby:
            {
            newstate = iNormalState;
            }
            break;
        case CGenericItutUiMgrBase::EStateCustomization:
            {
            newstate = iCustomerState;
            }
            break;
        case CGenericItutUiMgrBase::EStateMatchSelection:
            {
            newstate = iMatchSelState;
            }
            break;
        case CGenericItutUiMgrBase::EStateSpelling:
            {
            newstate = iSpellState;
            }
            break;
        default:
            return;    
        }

    if (iCurrentState)
        {
        iCurrentState->OnExit();
        }

    iCurrentState = newstate;    
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

void CWesternItutUiMgr::ActivateUiL()
    {
    if (iCurrentState)
        {
        iCurrentState->OnEntryL();
        }
    else
        {
        SetCurrentStateL(CGenericItutUiMgrBase::EStateWesternStandby);
        }
    }

void CWesternItutUiMgr::DeactivateUI()
    {
    if (iCurrentState)
        {
        iCurrentState->OnExit();
        //iCurrentState = NULL;
        }
    }

TInt CWesternItutUiMgr::HandleCommandL(TInt aCmd, TUint8* aData)
    {
    if (aCmd == EItutExtCmdCustomerState || 
        aCmd == ECmdPenInputFingerMatchSelection ||
        aCmd == ECmdPenInputFingerSpelling)
        {
        if (!(*(reinterpret_cast<TBool*>(aData))))
            {
            // state not on
            SetCurrentStateL(CGenericItutUiMgrBase::EStateWesternStandby);

            return KErrNone;
            }
        }

    switch (aCmd)
        {
        case EItutExtCmdCustomerState:
            {
            SetCurrentStateL(CGenericItutUiMgrBase::EStateCustomization);
            return KErrNone;
            }
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
        case ECmdPenInputDeActive:            
        	{
            if (iCurrentState && 
                iCurrentState->StateType() == CGenericItutUiMgrBase::EStateMatchSelection)
            	{
            	SetCurrentStateL(CGenericItutUiMgrBase::EStateWesternStandby);	
            	}
            }
            break;
        case ECmdPenInputCase:
            {
            if (iCurrentState)
                {
                iLayoutContext->DataMgr()->SetCase(*((TUint*) aData));
                return KErrNone;
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
        default:
            break;
        }

    if (iCurrentState)
        {
        return iCurrentState->HandleCommandL(aCmd, aData);
        }

    return KErrNotSupported;
    }

// End Of File
