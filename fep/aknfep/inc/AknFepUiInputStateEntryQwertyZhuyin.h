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
*       Provides the TAknFepInputStateEntryQwertyZhuyin definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_INPUT_QWERTY_ZHUYIN_H__
#define __AKN_FEP_UI_INPUT_STATE_INPUT_QWERTY_ZHUYIN_H__

#include "AknFepUiInputStateEntryQwertyBase.h"

class TAknFepInputStateEntryQwertyZhuyin : public TAknFepInputStateEntryQwertyBase
    {
    public:
        TAknFepInputStateEntryQwertyZhuyin(MAknFepUIManagerStateInterface* aOwner,
                                     MAknFepUICtrlContainerChinese* aUIContainer);

        TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
	 /**
     * Handling Command events.
     * This Method is called by CAknFepUIManagerJapanese.
     *
     * @param aCommandId Command ID value
     */
    void HandleCommandL( TInt aCommandId );
    public: // Functions from MPtiObserver interface
        void KeyTimerExpired();    
    protected:
        void RefreshUI();
        void UpdateIndicator();
        TBool HandleVerticalNavigation(TInt aKey);            
    };

#endif //__AKN_FEP_UI_INPUT_STATE_INPUT_QWERTY_ZHUYIN_H__

// End of file
