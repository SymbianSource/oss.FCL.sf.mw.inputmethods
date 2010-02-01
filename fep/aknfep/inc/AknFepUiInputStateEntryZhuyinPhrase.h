/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateEntryZhuyinPhrase definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_INPUT_ZHUYIN_PHRASE_H__
#define __AKN_FEP_UI_INPUT_STATE_INPUT_ZHUYIN_PHRASE_H__

//#include "AknFepUIInputStateEntryZhuyinStrokeBasePhrase.h"
#include "aknfepuiinputstatezhuyinphrasecreationbase.h"

class TAknFepInputStateEntryZhuyinPhrase : public TAknFepInputStateZhuyinPhraseCreationBase //TAknFepInputStateEntryZhuyinStrokeBasePhrase
    {
public:
    TAknFepInputStateEntryZhuyinPhrase(
                                    MAknFepUIManagerStateInterface* aOwner,
                                    MAknFepUICtrlContainerChinese* aUIContainer);
	TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
	void HandleCommandL(TInt aCommandId);

public: // Functions from MPtiObserver interface
    void KeyTimerExpired();
    void GetNewCandidatesL(const TDesC& aSource, RPointerArray<HBufC>& aList);
    void AddPhraseToDB( TDes& aPhrase );
    /**
     * Handle CBA1,ok and select candidate phrase key press.
     *
     * @since S60 v5.0
     * @return None.
     */
    void HandleCommitL();
    };

#endif //__AKN_FEP_UI_INPUT_STATE_INPUT_ZHUYIN_PHRASE_H__

// End of file
