/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            ?Description
*
*/












#ifndef T_AKNFEPUIINPUTSTATEPREDINPUTQWERTYSTROKEPHRASE_H
#define T_AKNFEPUIINPUTSTATEPREDINPUTQWERTYSTROKEPHRASE_H

#include "AknFepUiInputStateCandidateQwertyBaseChinesePhrase.h"

class TAknFepInputStatePredictiveInputQwertyStrokePhrase : 
public TAknFepInputStateCandidateQwertyBaseChinesePhrase
    {
public:
    TAknFepInputStatePredictiveInputQwertyStrokePhrase(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    /**
     * Handling Command events.
     * This Method is called by CAknFepUIManagerJapanese.
     *
     * @param aCommandId Command ID value
     */
    void HandleCommandL( TInt aCommandId );
private:
    TBool IsValidEntryKey(TInt aKey);
    };

#endif T_AKNFEPUIINPUTSTATEPREDINPUTQWERTYSTROKEPHRASE_H

// End of file
