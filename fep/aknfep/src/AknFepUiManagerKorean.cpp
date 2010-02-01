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
* Description:            Provides the CAknFepUIManagerKorean Methods.
*
*/

// INCLUDE FILES
#include <e32svr.h> // RDebug stuff...
#include <e32keys.h>

#include "AknFepGlobalEnums.h"
#include "AknFepManagerUIInterface.h"
#include "AknFepManagerInterface.h"
#include "AknFepUIManagerStateInterface.h"
#include "AknFepUiManagerBase.h"
#include "AknFepUiManagerKorean.h"

// Western input
#include "AknFepUiInputStateInitialLatinMultitap.h"
#include "AknFepUiInputStateInitialNumber.h"

//Korean input
#include "AknFepUiInputStateInitialKoreanMultitap.h"
#include "AknFepUiInputStateQwertyKorean.h"
#include "AknFepPanic.h"

#include <AknIndicatorContainer.h>  // CAknIndicatorContainer::TIndicatorContext
#include <PtiEngine.h> 
#include <PtiDefs.h>

// CONSTANTS

/**
 *  CAknFepUIManagerKorean class.
 * 
 */

//============================ MEMBER FUNCTIONS ==============================


// ---------------------------------------------------------------------------
// CAknFepUIManagerKorean::NewL
// 
// 
// ---------------------------------------------------------------------------
//
CAknFepUIManagerKorean* CAknFepUIManagerKorean::NewL(MAknFepManagerUIInterface* aFepMan, CAknFepCaseManager* aCaseMan, TLanguage aLanguage)
    {   
    CAknFepUIManagerKorean* self = new(ELeave)CAknFepUIManagerKorean(aFepMan, aCaseMan);
    CleanupStack::PushL(self);
    self->ConstructL(aLanguage);
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerKorean::~CAknFepUIManagerKorean
// 
// 
// ---------------------------------------------------------------------------
//
CAknFepUIManagerKorean::~CAknFepUIManagerKorean()
    {
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerKorean::HandleKeyL
// 
// 
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerKorean::HandleKeyL( TInt aKey, TKeyPressLength aLength,
		                                  TEventCode /*aEventCode*/ )
    {
    TUIState currentState = iFepUiState.State();
    if (aLength == EShortKeyPress)
        {
        iInitialFepUIState = currentState;
        }

    if(!iStatePtr->HandleKeyL(aKey, aLength))
        {       
        if(currentState != iStatePtr->State()) //Check to see if we've changed state.
            {
			return iStatePtr->HandleKeyL(aKey, aLength);                                     
            }
        return EFalse; // we've failed to handle it..       
        }
    return ETrue; //we're ok.
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerKorean::CloseUI
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerKorean::CloseUI()
    {
    ChangeState(EInitial);
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerKorean::SupportLanguage
// 
// 
// ---------------------------------------------------------------------------
//
TInt CAknFepUIManagerKorean::SupportLanguage(TInt aMode) const
    {
    TInt lang = iLanguage;
    switch (aMode)
        {
        case ELatin:
            {
            lang = ELangEnglish;
            }
            break;
        case EHangul:
        case ENumeric:
        default:
            break;
        }
    return lang;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerKorean::ActivateUI
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerKorean::ActivateUI()
    {
    ChangeState(EEntry);
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerKorean::ChangeState
// 
// 
// ---------------------------------------------------------------------------
// MAknFepUIManagerStateInterface
TUIState CAknFepUIManagerKorean::ChangeState(TUIState aState)
    {
    switch(aState)
        {
        case EInitial:
            {
            switch(iMode)
                {
                case ELatin:
                    {
                    iFepUiState = TAknFepInputStateInitialLatinMultitap(this);
                    break;               
                    }
                case EHangul:
                	{
                    iFepUiState = TAknFepInputStateInitialKoreanMultitap(this);	         
                	break;
                	}
                case ENumber:
                case ENativeNumber:
                    {
                    //Map directly to the resources...
                    iFepUiState = TAknFepInputStateInitialNumber(this);
                    break;
                    }
                default:
                    AknFepPanic(EAknFepPanicBadInputState);
                    break;
                }
            break;
            }
        case EEntry:
            {
            switch(iMode)
                {
                default:
                    AknFepPanic(EAknFepPanicBadInputState);
                    break;
                }
            break;  
            }   	
        case ECandidate:
        	{
        	switch(iMode)
        		{
        		default:
                    AknFepPanic(EAknFepPanicBadInputState);
                    break;
        		}
        	break;
        	}
        	
        case EPredictiveCandidate:
        	{
        	switch(iMode)
        		{
        		default:
                    AknFepPanic(EAknFepPanicBadInputState);
                    break;
        		}
            break;
        	}
        	
        case EQwerty:
            {
            switch(iMode)
                {
                case ELatin:
                    {
                    iFepUiState = TAknFepInputStateQwertyKorean(this);
                    break;               
                    }
                default:
			AknFepPanic(EAknFepPanicBadInputState);	
           	break;
                }
               break;
            }
            
        case EPredictiveInput:
            AknFepPanic(EAknFepPanicBadInputState);
            break;
            
        default:
            AknFepPanic(EAknFepPanicBadInputState);
         	break;   
        }
    
    iPtiEngine->SetObserver(&iFepUiState);
#ifdef _DEBUG
    RDebug::Print(_L("UI Manager: New State: %d"), iFepUiState.State());
#endif
    return iStatePtr->State();
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerKorean::CAknFepUIManagerKorean
// 
// 
// ---------------------------------------------------------------------------
//
CAknFepUIManagerKorean::CAknFepUIManagerKorean(MAknFepManagerUIInterface* aFepMan, CAknFepCaseManager* aCaseMan)
    : CAknFepUIManagerBase(aFepMan, aCaseMan),
      iFepUiState(NULL)

    {
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerKorean::ConstructL
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerKorean::ConstructL(TLanguage aLanguage)
    {   
    CAknFepUIManagerBase::ConstructL(aLanguage);
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerKorean::FepUIState
// 
// 
// ---------------------------------------------------------------------------
//
TAknFepInputStateBase* CAknFepUIManagerKorean::FepUIState()
    {
    return &iFepUiState;
    }
    
// End of file
