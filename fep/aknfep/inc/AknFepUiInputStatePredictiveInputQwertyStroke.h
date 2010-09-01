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












#ifndef T_AKNFEPUIINPUTSTATEPREDINPUTQWERTYSTROKE_H
#define T_AKNFEPUIINPUTSTATEPREDINPUTQWERTYSTROKE_H

#include "AknFepUiInputStateCandidateQwertyChineseBase.h"

class TAknFepInputStatePredictiveInputQwertyStroke : 
public TAknFepInputStateCandidateQwertyChineseBase
    {
public:
    TAknFepInputStatePredictiveInputQwertyStroke(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    TBool IsValidEntryKey(TInt aKey);
    /**
     * Handling Command events.
     * This Method is called by CAknFepUIManagerJapanese.
     *
     * @param aCommandId Command ID value
     */
    void HandleCommandL( TInt aCommandId );
    };

#endif T_AKNFEPUIINPUTSTATEPREDINPUTQWERTYSTROKE_H

// End of file
