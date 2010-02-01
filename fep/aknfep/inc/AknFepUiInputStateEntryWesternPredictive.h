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
* Description:           
*       Provides the TAknFepInputStateEntryWesternPredictive definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_ENTRY_WESTERN_PREDICTIVE_H__
#define __AKN_FEP_UI_INPUT_STATE_ENTRY_WESTERN_PREDICTIVE_H__

#include "AknFepUiInputStateBase.h"

class TAknFepInputStateEntryWesternPredictive : public TAknFepInputStateBase
    {
public:
    TAknFepInputStateEntryWesternPredictive(MAknFepUIManagerStateInterface* aOwner);
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    void HandleCommandL(TInt aCommandId);

public: // Functions from MPtiObserver interface
    void LastWordInSelectionList();
    void FirstWordInSelectionList();
    void DoPreviousMatchL();
    void DoNextMatch();
protected:
    TBool HandleStandardNumericKeysL(TInt aKey, TKeyPressLength aLength);
    TBool HandleNaviKeysL(TInt aKey, TKeyPressLength aLength);
    virtual TBool HandleStarKeyL(TKeyPressLength aLength);

private:
	void ConvertDigitToDigittype(TDigitType& digittype,TDes& aConvertedNumeral);
	TBool IsMoreCandidate();
    };

#endif //__AKN_FEP_UI_INPUT_STATE_ENTRY_WESTERN_PREDICTIVE_H__

// End of file
