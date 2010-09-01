/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateEntryQwertyStroke definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_ENTRY_QWERTY_STROKE_H__
#define __AKN_FEP_UI_INPUT_STATE_ENTRY_QWERTY_STROKE_H__

#include "AknFepUiInputStateEntryQwertyBase.h"
#include "AknFepUIManagerStateInterface.h"


class TAknFepInputStateEntryQwertyStroke : public TAknFepInputStateEntryQwertyBase
    {
public:
    TAknFepInputStateEntryQwertyStroke(MAknFepUIManagerStateInterface* aOwner,
                               MAknFepUICtrlContainerChinese* aUIContainer);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    /**
     * Handling Command events.
     * This Method is called by CAknFepUIManagerJapanese.
     *
     * @param aCommandId Command ID value
     */
    void HandleCommandL( TInt aCommandId );
protected:
    void RefreshUI();
private:
    TBool HandleHorizontalNavigation(TInt aKey);
    TBool HandleVerticalNavigation(TInt aKey);
    void  UpdateIndicator();
    };

#endif //__AKN_FEP_UI_INPUT_STATE_ENTRY_QWERTY_STROKE_H__

// End of file
