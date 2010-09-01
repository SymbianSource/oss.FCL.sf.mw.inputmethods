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












#ifndef T_AKNFEPUIINPUTSTATECANDIDATEQWERTYBASECHINESEPHRASE_H
#define T_AKNFEPUIINPUTSTATECANDIDATEQWERTYBASECHINESEPHRASE_H

#include "AknFepUiInputStateChineseBase.h"
#include <PtiDefs.h>

/**
 * key struct.
 * @since S60 v3.2.3
 */
struct TMiniQwertyKeyMap
    {
    TUint16 iValue;
    TInt iKeyCode;
    };

/**
 * Tone mark struct.
 * @since S60 v3.2.3
 */
struct TMiniQwertyToneMap
    {
    TUint16 iValue;
    TInt iInputTone;
    };

class TAknFepInputStateCandidateQwertyBaseChinesePhrase : public TAknFepInputStateChineseBase
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
    TAknFepInputStateCandidateQwertyBaseChinesePhrase(MAknFepUIManagerStateInterface* aOwner,
                                    MAknFepUICtrlContainerChinese* aUIContainer);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);

    TBool HandleHorizontalNavigation(TInt aKey);
    TBool HandleVerticalNavigation(TInt aKey);
#ifdef RD_INTELLIGENT_TEXT_INPUT
    /**
     * Check the select candidate numeric key is valid or not
     * @since S60 v3.2
     * @return ETure if the select number is valid ,others is EFalse
     */
    TBool  MapKeyToIndexSelectCandidate( TInt aKey,TInt& aIndex );
#endif
    
    inline TInt MapKeyToIndex(TInt aKey);
    void UpdateIndicator();
    void GetNewCandidatesL(const TDesC& aSource, RPointerArray<HBufC>& aList);
    };

inline TInt TAknFepInputStateCandidateQwertyBaseChinesePhrase::MapKeyToIndex(TInt aKey)
    {
    TInt index = aKey - EPtiKey0; // get zero-based value first
    return !index ? 9 : --index; // map it for one-based keys...
    }

#endif //T_AKNFEPUIINPUTSTATECANDIDATEQWERTYBASECHINESEPHRASE_H

// End of file
