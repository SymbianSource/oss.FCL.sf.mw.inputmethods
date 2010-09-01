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
*       Provides the TAknFepInputStateInitialVietnameseMultitap definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_INIT_VIETNAMESE_MULTITAP_H__
#define __AKN_FEP_UI_INPUT_STATE_INIT_VIETNAMESE_MULTITAP_H__

#include "AknFepUiInputStateInitialMultitapBase.h"

class TAknFepInputStateInitialVietnameseMultitap : public TAknFepInputStateInitialMultitapBase
    {
public:
    TAknFepInputStateInitialVietnameseMultitap(MAknFepUIManagerStateInterface* aOwner); 

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    static TBool NeedsVietnameseBkDelSupport(TText aGlyphToDelete, TDes& aNewText);
    
    // Inherited from Base
    void KeyTimerExpired();
    };


#endif //__AKN_FEP_UI_INPUT_STATE_INIT_VIETNAMESE_MULTITAP_H__

// End of file
