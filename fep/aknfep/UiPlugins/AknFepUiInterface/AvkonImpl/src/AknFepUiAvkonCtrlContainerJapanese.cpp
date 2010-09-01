/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:           
*       Provides the CAknFepUICtrlContainerJapanese methods.
*
*/












// INCLUDE FILES
#include "AknFepUIAvkonCtrlContainerJapanese.h"
#include "AknFepUIManagerJapanese.h"
#include "AknFepUICtrlCandidatePaneInterface.h" // MAknFepUiCtrlCandidatePaneInterface
#include "AknFepUIAvkonCtrlJapaneseCandidatePane.h"  // CAknFepUICtrlJapaneseCandidatePane
#include "AknFepUIAvkonCtrlJapanesePredictivePane.h" // CAknFepUICtrlJapanesePredictivePane
#include "AknFepUiInterfacePanic.h"

// CONSTANTS

/**
*  CAknFepUICtrlContainerJapanese class.
*
*/

//============================ MEMBER FUNCTIONS ==============================
MAknFepUICtrlContainerJapanese::~MAknFepUICtrlContainerJapanese()
    {
    }

// ---------------------------------------------------------------------------
// CAknFepUICtrlContainerJapanese::NewL
//
// First phase construction
// ---------------------------------------------------------------------------
//
CAknFepUICtrlContainerJapanese* CAknFepUICtrlContainerJapanese::NewL(CAknFepUIManagerJapanese* aUiMng)
    {
    CAknFepUICtrlContainerJapanese* self = new(ELeave) CAknFepUICtrlContainerJapanese();
    CleanupStack::PushL(self);
    self->ConstructL(aUiMng);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CAknFepUICtrlContainerJapanese::~CAknFepUICtrlContainerJapanese
//
// Destructor
// ---------------------------------------------------------------------------
//
CAknFepUICtrlContainerJapanese::~CAknFepUICtrlContainerJapanese()
    {
    if (iCandidatePaneArray)
        {
        iCandidatePaneArray->ResetAndDestroy();
        }
    delete iCandidatePaneArray;
    }

// ---------------------------------------------------------------------------
// CAknFepUICtrlContainerJapanese::CandidatePane
//
//
// ---------------------------------------------------------------------------
//
MAknFepUICtrlCandidatePaneInterface*
    CAknFepUICtrlContainerJapanese::CandidatePane(TAknFepUICtrlCandidatePaneId aId)
    {
    MAknFepUICtrlCandidatePaneInterface* candidatePane = NULL;

    switch(aId)
        {
        case EJapaneseCandidatePane:
            {
            candidatePane = iCandidatePaneArray->At(EJapaneseCandidatePane);
            }
        break;

        case EJapanesePredictiveCandidatePane:
            {
            candidatePane = iCandidatePaneArray->At(EJapanesePredictiveCandidatePane);
            }
        break;

        default:
            {
            AknFepUiInterfacePanic(EAknFepUiInterfacePanicNoCandidatePane);
            }
        }
    return candidatePane;
    }

// ---------------------------------------------------------------------------
// CAknFepUICtrlContainerJapanese::CAknFepUICtrlContainerJapanese
//
// Default constructor
// ---------------------------------------------------------------------------
//
CAknFepUICtrlContainerJapanese::CAknFepUICtrlContainerJapanese()
    {
    }

// ---------------------------------------------------------------------------
// CAknFepUICtrlContainerJapanese::ConstructL
//
// Second phase construction
// ---------------------------------------------------------------------------
//
void CAknFepUICtrlContainerJapanese::ConstructL(CAknFepUIManagerJapanese* aUiMng)
    {
    MAknFepUICtrlCandidatePaneInterface* candidatePane;

    iCandidatePaneArray = new(ELeave) CArrayPtrFlat<MAknFepUICtrlCandidatePaneInterface>(EJapaneseCandidatePaneMax);

    for(TInt i = 0; i < EJapaneseCandidatePaneMax; i++)
        {
        candidatePane = NULL;
        switch(i)
            {
            case EJapaneseCandidatePane:
                {
                candidatePane = CAknFepUICtrlJapaneseCandidatePane::NewL(aUiMng);
                }
                break;
            case EJapanesePredictiveCandidatePane:
                {
                candidatePane = CAknFepUICtrlJapanesePredictivePane::NewL(aUiMng);
                }
                break;
            default:
                {
                AknFepUiInterfacePanic(EAknFepUiInterfacePanicNoCandidatePane);
                }
                break;
            }

        if (candidatePane)
            {
            CleanupStack::PushL(candidatePane);
            iCandidatePaneArray->AppendL(candidatePane);
            CleanupStack::Pop(candidatePane); // candidatePane
            }
        else
            {
            iCandidatePaneArray->AppendL(candidatePane);
            }
        }
    }
// End of file
