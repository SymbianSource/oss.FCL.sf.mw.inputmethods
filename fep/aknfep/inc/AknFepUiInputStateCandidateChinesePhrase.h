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












#ifndef T_AKNFEPUIINPUTSTATECANDIDATECHINESEPHRASE_H_
#define T_AKNFEPUIINPUTSTATECANDIDATECHINESEPHRASE_H_

#include "AknFepUiInputStateCandidateBaseChinesePhrase.h"

class TAknFepInputStateCandidateChinesePhrase : public TAknFepInputStateCandidateBaseChinesePhrase
    {
public:
    TAknFepInputStateCandidateChinesePhrase(MAknFepUIManagerStateInterface* aOwner,
                               MAknFepUICtrlContainerChinese* aUIContainer);

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);

    //the follow 3 functions for phrase creation
      TBool UpdateEEPContent();
      
      void AddPhraseToDB( TDes& aPhrase );
    };

#endif //T_AKNFEPUIINPUTSTATECANDIDATECHINESEPHRASE_H_

// End of file
