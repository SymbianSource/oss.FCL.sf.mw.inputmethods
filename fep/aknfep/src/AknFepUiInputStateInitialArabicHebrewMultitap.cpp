/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateInitialArabicHebrewMultitap methods.
*
*/












#include "AknFepUiInputStateInitialArabicHebrewMultitap.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface

#include "AknFepCaseManager.h"
#include <uikon.hrh>

#include <PtiEngine.h>

TAknFepInputStateInitialArabicHebrewMultitap::
TAknFepInputStateInitialArabicHebrewMultitap(MAknFepUIManagerStateInterface* aOwner)
:TAknFepInputStateInitialLatinMultitap(aOwner)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();

    // This can be removed now because the PTI engine is in use?
    TPtiTextCase caseMode = ptiengine->Case();

    ptiengine->ClearCurrentWord();
    ptiengine->SetInputMode(EPtiEngineMultitapping);

    ptiengine->SetCase(caseMode);
    }

TBool TAknFepInputStateInitialArabicHebrewMultitap::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TBool ret = ETrue;

    if(iData && (iData != aKey))
        {
        fepMan->CommitInlineEditL();
        ptiengine->ClearCurrentWord();
        CAknFepCaseManager* caseMan = iOwner->CaseMan(); 
        caseMan->UpdateCase(ENullNaviEvent); 
        }

    iData = aKey;
    if(aLength == EShortKeyPress)
        {
        if (aKey == EPtiKeyStar)
            {
  			if (!fepMan->EditSubmenuInUse() && fepMan->IsAbleToLaunchSCT())
        		{
        		fepMan->LaunchSpecialCharacterTableL();
        		}                        
            }
        else
            {
            TPtrC text = ptiengine->AppendKeyPress((TPtiKey)aKey);
            if (text.Length())
                {
                fepMan->NewCharacterL(text);
                }
            }
        }
    else // long keypress
        {
	     if (aKey == EPtiKeyStar && fepMan->EditSubmenuInUse())
            {
       		fepMan->LaunchSpecialCharacterTableL();                
            }
		else
			{					                    
        	TChar ch(aKey);

        	TBuf<1> buf;
        	buf.Append(ch);
        	fepMan->NewCharacterL(buf);
        	fepMan->CommitInlineEditL();
        	ptiengine->ClearCurrentWord();
			}
        }

    return ret;
    }


// End of file
