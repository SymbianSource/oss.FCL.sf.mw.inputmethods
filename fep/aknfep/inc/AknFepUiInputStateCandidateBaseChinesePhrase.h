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












#ifndef T_AKNFEPUIINPUTSTATECANDIDATEBASECHINESEPHRASE_H_
#define T_AKNFEPUIINPUTSTATECANDIDATEBASECHINESEPHRASE_H_

#include "AknFepUiInputStateCandidateBase.h"
#include <PtiDefs.h>

class TAknFepInputStateCandidateBaseChinesePhrase : public TAknFepInputStateCandidateBase
    {
protected:
    TAknFepInputStateCandidateBaseChinesePhrase(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    
    void HandleCommandL(TInt aCommandId);

    /**
     * Handle CBA1 or select event.
     * 
     * @since S60 v3.2.3
     * @return None.
     */
    void HandleCommitL();
    
    TBool HandleHorizontalNavigation(TInt aKey);
    TBool HandleVerticalNavigation(TInt aKey);
    TBool HandleVerticalNavigation(TInt aKey, TKeyPressLength aLength);
    void UpdateIndicator();
    //the follow 6 functions for creation
    void GetValidkeystrokeL();
    TBool CheckSpellingOfPtiEngine();
    void GetKeystrokeCode( TInt& aKey, const TDesC& aKeystroke );
    void ClearPtiEngineKeystroke();
    void DeleteTonMarkFromInEffectKeytrokeArray();
    void CheckTonemarkL();
    TBool IsValid(const TDesC& aDesC, TInt aCount);
    void GetNewCandidatesL(const TDesC& aSource, RPointerArray<HBufC>& aList);
    };

#endif //T_AKNFEPUIINPUTSTATECANDIDATEBASECHINESEPHRASE_H_

// End of file
