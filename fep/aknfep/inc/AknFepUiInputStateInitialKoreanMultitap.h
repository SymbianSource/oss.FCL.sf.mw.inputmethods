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
*       Provides the TAknFepInputStateInitialKoreanMultitap definition.
*
*/

#ifndef __AKN_FEP_UI_INPUT_STATE_INIT_KOREAN_MULTITAP_H__
#define __AKN_FEP_UI_INPUT_STATE_INIT_KOREAN_MULTITAP_H__

#include "AknFepUiInputStateInitialMultitapBase.h"

class TAknFepInputStateInitialKoreanMultitap : public TAknFepInputStateInitialMultitapBase
    {
public:
    TAknFepInputStateInitialKoreanMultitap(MAknFepUIManagerStateInterface* aOwner); 

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
	
	/**
	 * Decides whether text input should be commited or not.
	 * Function makes comparison between latest key input and korean keymappping data.
	 * if data match, then text is to be commited. 
     * @param aData TPtrC
     * @param aKey TInt.
     * @return TBool
	 */
	TBool CommitOrNot(TPtrC aData, TInt aKey);

    };

#endif //__AKN_FEP_UI_INPUT_STATE_INIT_KOREAN_MULTITAP_H__

// End of file