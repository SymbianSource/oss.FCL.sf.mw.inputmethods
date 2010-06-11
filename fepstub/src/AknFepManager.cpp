/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CAknFepManager methods.
*
*/


#include "AknFepManager.h"

#include <bautils.h>
#include <coeaui.h>                 //CCoeAppUi

CAknFepManager::CAknFepManager(CCoeEnv& aConeEnvironment)
    :CCoeFep(aConeEnvironment)
    {
    }

void CAknFepManager::ConstructL(const CCoeFepParameters& /*aFepParameters*/)
    {
    }
CAknFepManager::~CAknFepManager()
    {
    }
void CAknFepManager::HandleAknEdwinStateEventL(CAknEdwinState* /*aAknEdwinState*/, 
                                               EAknEdwinStateEvent /*aEventType*/)
{
}
void CAknFepManager::HandleChangeInFocus()
    {
    }
void CAknFepManager::HandleGainingForeground()
    {
    }
void CAknFepManager::HandleLosingForeground()
    {
    }
void CAknFepManager::HandleDestructionOfFocusedItem()
    {
    }   

TInt CAknFepManager::NumberOfAttributes() const
    {
    return 0;
    }

TUid CAknFepManager::AttributeAtIndex(TInt /*aIndex*/) const
    {
    return KNullUid;
    }

void CAknFepManager::WriteAttributeDataToStreamL(TUid /*aAttributeUid*/, RWriteStream& /*aStream*/) const
    {
    }

void CAknFepManager::ReadAttributeDataFromStreamL(TUid /*aAttributeUid*/, RReadStream& /*aStream*/)
    {
    }


void CAknFepManager::CancelTransaction()
    {

    }

void CAknFepManager::IsOnHasChangedState()
    {
    }

void CAknFepManager::OfferKeyEventL(TEventResponse& /*aEventResponse*/, 
                                    const TKeyEvent& /*aKeyEvent*/, 
                                    TEventCode /*aEventCode*/)
    {
    }

void CAknFepManager::OfferPointerEventL(TEventResponse& /*aEventResponse*/, 
                                        const TPointerEvent& /*aPointerEvent*/, 
                                        const CCoeControl* /*aWindowOwningControl*/)
    {
    }

void CAknFepManager::OfferPointerBufferReadyEventL(TEventResponse& /*aEventResponse*/, 
                                                   const CCoeControl* /*aWindowOwningControl*/)
    {
    }

    
// End of file
