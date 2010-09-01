/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Provides the TAknFepInputStateInitialChineseGeneric methods.
*
*/












// INCLUDE FILES
#include <PtiEngine.h>
#include <e32keys.h>
#include <PtiDefs.h>

#include "aknfepuifingeritutinputstategeneric.h"
#include "AknFepCaseManager.h"
#include "AknFepUIManagerStateInterface.h"
#include "AknFepManager.h"

//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------------------------
//
TAknFepUIFingerItutInputStateChineseGeneric::TAknFepUIFingerItutInputStateChineseGeneric(
    MAknFepUIManagerStateInterface* aOwner/*,
    MAknFepUICtrlContainerChinese* aUIContainer*/)
    :TAknFepInputStateBase(aOwner),//TAknFepInputStateInitialChineseMultitapBase(aOwner, aUIContainer),
    iPreObserver(NULL)
    {
    }

// ---------------------------------------------------------------------------
// Handle key event
// ---------------------------------------------------------------------------
//
TBool TAknFepUIFingerItutInputStateChineseGeneric::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TBool ret = ETrue;
    
    if (aLength == ELongKeyPress &&
       (aKey >= EPtiKey0 && aKey <= EPtiKey9))
        {
        TChar ch(aKey);
        TBuf<1> buf;
        buf.Append(ch);
	    fepMan->NewCharacterL(buf);
	    fepMan->CommitInlineEditL();
	    ptiengine->ClearCurrentWord();
        return ret;
        }
    
    if(iData && (iData != aKey))
        {
        fepMan->CommitInlineEditL();
        ptiengine->ClearCurrentWord();
        }

    //7,8,9 are not multitap in EStroke, so commit
    if ((fepMan->InputMode() == EStroke) &&
         (iData == EPtiKey7 || iData == EPtiKey8 || iData == EPtiKey9 ))
    	{
        fepMan->CommitInlineEditL();
        ptiengine->ClearCurrentWord();
    	}

    iData = aKey;

    switch(aKey)
        {
        case EPtiKey0:
        case EPtiKey1:
            {
            const TPtiTextCase oldCase = ptiengine->Case();
            if (fepMan->InputMode() == EStroke)
                {
                ptiengine->SetCase(EPtiCaseUpper);
                }
            SetPreObserver(ptiengine->Observer());
            ptiengine->SetObserver(this);
            TPtrC retText = ptiengine->AppendKeyPress((TPtiKey)aKey);
            fepMan->NewCharacterL(retText);
            ptiengine->SetCase(oldCase);
            }
        break;
        case EPtiKey7:
        case EPtiKey8:
        case EPtiKey9:
           {
            if (fepMan->InputMode() == EStroke)
                {
                const TPtiTextCase oldCase = ptiengine->Case();
                ptiengine->SetCase(EPtiCaseUpper);
                
                SetPreObserver(ptiengine->Observer());
                ptiengine->SetObserver(this);
                
                TPtrC retText = ptiengine->AppendKeyPress((TPtiKey)aKey);
                fepMan->NewCharacterL(retText);
                ptiengine->SetCase(oldCase);
                }
            }
        break;
        case EPtiKeyStar:
            {
            if (aLength == EShortKeyPress)
                {
                if (fepMan->EditSubmenuInUse())
                    {
                    fepMan->ClearCcpuFlag(CAknFepManager::ECcpuStateIgnoreStarUp);
                    }
               else if (fepMan->IsAbleToLaunchSCT())
                    {
                    fepMan->LaunchSpecialCharacterTableL();
                    }
                }
            else
                {
                if (fepMan->EditSubmenuInUse())
                    {
                    fepMan->LaunchSpecialCharacterTableL();
                    }
                }
            }
        break;
        case EStdKeyNull:
            {
            ret = EFalse; 
            }
        break;
        case EKeyOK:
        case EKeyBackspace:
        default:
            {
            ret = EFalse; 
            }
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// Set Previous ptiengine observer
// ---------------------------------------------------------------------------
//
void TAknFepUIFingerItutInputStateChineseGeneric::SetPreObserver(MPtiObserver* aPreObserver)
    {
    if (aPreObserver != this)
        {
    	  iPreObserver = aPreObserver; 
    	  }
    }
    
// ---------------------------------------------------------------------------
// Get Previous ptiengine observer
// ---------------------------------------------------------------------------
//
MPtiObserver* TAknFepUIFingerItutInputStateChineseGeneric::PreObserver()
    {
    return iPreObserver;
    }
    
// ---------------------------------------------------------------------------
// Key timer expired called by ptiengine 
// ---------------------------------------------------------------------------
//
void TAknFepUIFingerItutInputStateChineseGeneric::KeyTimerExpired()
    {
    if (iData)
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        TRAP_IGNORE(fepMan->CommitInlineEditL());

        CPtiEngine* ptiengine = iOwner->PtiEngine();
        ptiengine->ClearCurrentWord();

        CAknFepCaseManager* caseMan = iOwner->CaseMan(); 
        caseMan->UpdateCase(ENullNaviEvent); 

        iData = 0;
        
        ptiengine->SetObserver(PreObserver());
        }
    }

// End of file
