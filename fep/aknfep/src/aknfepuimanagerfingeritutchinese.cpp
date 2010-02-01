/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Chinese finger ITUT ui manager implementation file.
*
*/












// INCLUDE FILES
#include <e32keys.h>
#include <AknFepPluginManager.h>
#include <PtiEngine.h> 
#include <PtiDefs.h>

#include "AknFepUiManagerBase.h"
#include "aknfepuimanagerfingeritutchinese.h"
#include "AknFepManagerUIInterface.h"
#include "AknFepGlobalEnums.h"
#include "aknfepuifingeritutinputstategeneric.h"
#include "AknFepCaseManager.h"
#include "AknFepPanic.h"

//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CAknFepUIManagerFingerItutChinese* CAknFepUIManagerFingerItutChinese::NewL(
    MAknFepManagerUIInterface* aFepMan, CAknFepCaseManager* aCaseMan, 
    TLanguage aLanguage, CAknFepPluginManager* aPluginMager)
    {   
    CAknFepUIManagerFingerItutChinese* self = new(ELeave)
        CAknFepUIManagerFingerItutChinese(aFepMan, aCaseMan, aPluginMager);
        
    CleanupStack::PushL(self);
    self->ConstructL(aLanguage);
    CleanupStack::Pop();

    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CAknFepUIManagerFingerItutChinese::~CAknFepUIManagerFingerItutChinese()
    {
    }

// ---------------------------------------------------------------------------
// Handle key event
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerFingerItutChinese::HandleKeyL( TInt aKey, TKeyPressLength aLength,
		                                            TEventCode /*aEventCode*/ )
    {
    TUIState currentState = iFepUiState.State();
    if (aLength == EShortKeyPress)
        {
        iInitialFepUIState = currentState;
        }

    if(!iPluginMager->CurrentPluginInputFepUI()->HandleKeyL(
         aKey == EStdKeyNull ? EPtiKeyStar : aKey , aLength))
        {
        return iStatePtr->HandleKeyL(aKey, aLength); // Failed to handle it
        }
        
    return ETrue; // It is ok.
    }

// ---------------------------------------------------------------------------
// Close current Finger ITUT ui
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerFingerItutChinese::CloseUI()
    {
    }

// ---------------------------------------------------------------------------
// Input mode is supported by this ui
// ---------------------------------------------------------------------------
//
TInt CAknFepUIManagerFingerItutChinese::SupportLanguage(TInt /*aMode*/) const
    {
    return iLanguage;
    }

// ---------------------------------------------------------------------------
// Change state machine
// ---------------------------------------------------------------------------
//
TUIState CAknFepUIManagerFingerItutChinese::ChangeState(TUIState /* aState*/)
    {
    // ask pti engine if phrase input is supported
    return iStatePtr->State();
    }

// ---------------------------------------------------------------------------
// Get current fep ui state machine
// ---------------------------------------------------------------------------
//
TAknFepInputStateBase* CAknFepUIManagerFingerItutChinese::FepUIState()
    {
    return &iFepUiState;
    }

// ---------------------------------------------------------------------------
// Set current input language
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerFingerItutChinese::SetInputLanguageL(TLanguage aLanguage)
    {
    iLanguage = aLanguage;
    }

// ---------------------------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------------------------
//
CAknFepUIManagerFingerItutChinese::CAknFepUIManagerFingerItutChinese(
    MAknFepManagerUIInterface* aFepMan, 
    CAknFepCaseManager* aCaseMan, 
    CAknFepPluginManager* aPluginMager)
    : CAknFepUIManagerBase(aFepMan, aCaseMan),
    iFepUiState(NULL),
    iPrevMode(ENumber),
    iPluginMager(aPluginMager)
    {
    }

// ---------------------------------------------------------------------------
// Symbian second-phase constructor
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerFingerItutChinese::ConstructL(TLanguage aLanguage)
    {
    CAknFepUIManagerBase::ConstructL(aLanguage);
    SetMode(iFepMan->InputMode(), ETrue, EFalse);
    iFepUiState = TAknFepUIFingerItutInputStateChineseGeneric(this/*, NULL*/);
    }

// ---------------------------------------------------------------------------
// Change ITUT state machine
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerFingerItutChinese::ChangeStatetItut(TUIState /* aState*/)
    {
    }

// End of file
