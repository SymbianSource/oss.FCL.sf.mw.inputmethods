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
* Description:            Provides the TAknFepInputStateEntryIndicPredictive Method.
*
*/












#include "AknFepUiInputStateEntryIndicPredictive.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepPanic.h"
#include "AknFepManagerUIInterface.h"
#include <AknFep.rsg>
#include <PtiEngine.h>
#include <PtiDefs.h>

// -----------------------------------------------------------------------------
// TAknFepInputStateEntryIndicPredictive::TAknFepInputStateEntryIndicPredictive

// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------

TAknFepInputStateEntryIndicPredictive::
TAknFepInputStateEntryIndicPredictive( MAknFepUIManagerStateInterface* aOwner, 
                                       TLanguage /*aLanguage*/ )
    :TAknFepInputStateEntryWesternPredictive( aOwner )
    {
  	}

void TAknFepInputStateEntryIndicPredictive::LastWordInSelectionList()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();

    if (iMatchState != EWordMatchAfterLast)
        {
        TRAP_IGNORE(fepMan->UpdateCbaL(R_AKNFEP_SOFTKEYS_SPELL_PREVIOUS));
        iMatchState = EWordMatchLast;
        }
    }

// End of File
