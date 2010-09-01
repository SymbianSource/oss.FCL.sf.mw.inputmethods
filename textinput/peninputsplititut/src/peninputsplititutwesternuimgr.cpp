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


#include "peninputsplititutwesternuimgr.h"
#include "peninputsplititutdata.h"
#include "peninputsplititutdatamgr.h"
#include "peninputsplititutwesternuistatenonpredict.h"
#include "peninputsplititutwesternuistatespelling.h"
#include "peninputsplititutlayoutcontext.h"
#include "peninputsplititutlayout.h"
#include "peninputsplititutwindowmanager.h"


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CWesternSplitItutUiMgr::NewL
// ---------------------------------------------------------------------------
//
CWesternSplitItutUiMgr* CWesternSplitItutUiMgr::NewL(MSplitItutLayoutContext* aContext, 
                                           CSplitItutDataMgr* aDataMgr,
                                           CSplitItutWindowManager* aUiManager)
    {
    CWesternSplitItutUiMgr* self = new (ELeave) CWesternSplitItutUiMgr(aContext, aDataMgr,aUiManager);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CWesternSplitItutUiMgr::NewL
// ---------------------------------------------------------------------------
//
CWesternSplitItutUiMgr::~CWesternSplitItutUiMgr()
    {
    if (iCurrentState)
        {
        iCurrentState->OnExit();
        }
        
    delete iNormalState;
    delete iSpellState;
    }

// ---------------------------------------------------------------------------
// CWesternSplitItutUiMgr::CWesternSplitItutUiMgr
// ---------------------------------------------------------------------------
//
CWesternSplitItutUiMgr::CWesternSplitItutUiMgr(MSplitItutLayoutContext* aContext,
                                     CSplitItutDataMgr* aDataMgr,
                                     CSplitItutWindowManager* aUiManager)
                                    :CSplitItutUiMgrBase(aContext, aDataMgr,aUiManager)
    {
    iDataMgr = aDataMgr;
    }

// ---------------------------------------------------------------------------
// CWesternSplitItutUiMgr::ConstructL
// ---------------------------------------------------------------------------
//
void CWesternSplitItutUiMgr::ConstructL()
    {
    CSplitItutUiMgrBase::ConstructL();

    iNormalState = CWesternSplitItutUiStateNonPredict::NewL(this);
    iSpellState = CWesternSplitItutUiStateSpelling::NewL(this);
    }

// ---------------------------------------------------------------------------
// CWesternSplitItutUiMgr::HandleCtrlEventL
// ---------------------------------------------------------------------------
//
TBool CWesternSplitItutUiMgr::HandleCtrlEventL(TInt aEventType, 
                                          CFepUiBaseCtrl* aCtrl, 
                                          const TDesC& aEventData)
    {
    if (!iCurrentState)
        {
        return EFalse;
        }

    return iCurrentState->HandleCtrlEventL(aEventType, aCtrl, aEventData);
    }

// ---------------------------------------------------------------------------
// CWesternSplitItutUiMgr::SetCurrentStateL
// ---------------------------------------------------------------------------
//
void CWesternSplitItutUiMgr::SetCurrentStateL(TUiState aNewState)
    {
    if (iCurrentState && aNewState == iCurrentState->StateType())
    	{
        return;    		
    	}
    
    CSplitItutUiStateBase* newstate = NULL;

    switch (aNewState)
        {
        case CSplitItutUiMgrBase::EStateWesternStandby:
            {
            newstate = iNormalState;
            }
            break;
        case CSplitItutUiMgrBase::EStateSpelling:
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
    if ( DataMgr()->IsUpdate() )  
        {
        DataMgr()->SetUpdate( EFalse );
        CSplitItutUiLayout* itutLayout = 
            static_cast<CSplitItutUiLayout*>( LayoutContext()->UiLayout() );
        itutLayout->SizeChanged( NULL );

        iCurrentState->OnEntryL();
        
        //redraw layout immediately to minimize flicker
        itutLayout->DisableLayoutDrawing( EFalse );
        itutLayout->Draw();
        }
    else
        {
        iCurrentState->OnEntryL();
        }
    }

// ---------------------------------------------------------------------------
// CWesternSplitItutUiMgr::ActivateUiL
// ---------------------------------------------------------------------------
//
void CWesternSplitItutUiMgr::ActivateUiL()
    {
    if (iCurrentState)
        {
        iCurrentState->OnEntryL();
        }
    else
        {
        SetCurrentStateL(CSplitItutUiMgrBase::EStateWesternStandby);
        }
    }

// ---------------------------------------------------------------------------
// CWesternSplitItutUiMgr::DeactivateUI
// ---------------------------------------------------------------------------
//
void CWesternSplitItutUiMgr::DeactivateUI()
    {
    if (iCurrentState)
        {
        iCurrentState->OnExit();
        }
    }

// ---------------------------------------------------------------------------
// CWesternSplitItutUiMgr::HandleCommandL
// ---------------------------------------------------------------------------
//
TInt CWesternSplitItutUiMgr::HandleCommandL(TInt aCmd, TUint8* aData)
    {
    if ( aCmd == ECmdPenInputFingerMatchSelection ||
         aCmd == ECmdPenInputFingerSpelling )
        {
        if (!(*(reinterpret_cast<TBool*>(aData))))
            {
            // state not on
            SetCurrentStateL(CSplitItutUiMgrBase::EStateWesternStandby);

            return KErrNone;
            }
        }

    switch (aCmd)
        {
        case ECmdPenInputCase:
            {
            if (iCurrentState)
                {
                iLayoutContext->DataMgr()->SetCase(*((TUint*) aData));
                return KErrNone;
                }
            }
        case ECmdPenInputFingerSpelling:
            {
            SetCurrentStateL(CSplitItutUiMgrBase::EStateSpelling);
            return KErrNone;
            }    
            // Modify warning
            //break;          
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
