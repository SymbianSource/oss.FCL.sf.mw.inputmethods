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












#ifndef __AKN_FEP_UI_INPUT_STATE_CANDIDATE_QWERTY_CHINESE_BASE_H__
#define __AKN_FEP_UI_INPUT_STATE_CANDIDATE_QWERTY_CHINESE_BASE_H__

#include "AknFepUiInputStateChineseBase.h"
#include <PtiDefs.h>

class TAknFepInputStateCandidateQwertyChineseBase : public TAknFepInputStateChineseBase
    {
    public:
    /**
     * Handling Command events.
     * This Method is called by CAknFepUIManagerJapanese.
     *
     * @param aCommandId Command ID value
     */
    void HandleCommandL( TInt aCommandId );
protected:
    TAknFepInputStateCandidateQwertyChineseBase(MAknFepUIManagerStateInterface* aOwner,
                                    MAknFepUICtrlContainerChinese* aUIContainer);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    TBool HandleHorizontalNavigation(TInt aKey);
    TBool HandleVerticalNavigation(TInt aKey);
    inline TInt MapKeyToIndex(TInt aKey);
    void UpdateIndicator();
    };
  
inline TInt TAknFepInputStateCandidateQwertyChineseBase::MapKeyToIndex(TInt aKey)
    {
    TInt index = aKey - EPtiKey0; // get zero-based value first
    return !index ? 9 : --index; // map it for one-based keys...
    }

#endif //__AKN_FEP_UI_INPUT_STATE_CANDIDATE_QWERTY_CHINESE_BASE_H__

// End of file
