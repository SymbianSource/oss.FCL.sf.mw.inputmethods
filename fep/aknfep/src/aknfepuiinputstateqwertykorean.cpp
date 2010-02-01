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
*       CAknFepInputStateQwertyKorean implementation
*
*/

#include "AknFepUiInputStateQwertyKorean.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
//#include "AknFepDbgKorean.h"

#include <PtiEngine.h>
#include <PtiDefs.h>
#include <PtiKeyMappings.h>
#include <PtiDefs.h>        

TAknFepInputStateQwertyKorean::TAknFepInputStateQwertyKorean(
        MAknFepUIManagerStateInterface* aOwner)
    : TAknFepInputStateQwerty(aOwner)
    {
    CPtiEngine& ptiEngine(*(aOwner->PtiEngine()));
    ptiEngine.SetInputMode(EPtiEngineQwertyKorean);
    ptiEngine.ClearCurrentWord();
    }

TAknFepInputStateQwertyKorean::~TAknFepInputStateQwertyKorean()
    {
    }

TBool TAknFepInputStateQwertyKorean::HandleKeyL(TInt aKey, TKeyPressLength /*aLength*/)
    {
//    LOG2("KO.IS.QWERTY.HandleKeyL %d,%d",aKey,aLength);
    CPtiEngine& ptiEngine(*(iOwner->PtiEngine()));
    MAknFepManagerUIInterface& fepMan(*(iOwner->FepMan()));
    
    TPtrC text(ptiEngine.AppendKeyPress((TPtiKey)aKey));
    TBuf<1> chr;            
    for (TInt jj = 0; jj < text.Length(); jj++)
        {
        chr.Zero();
        chr.Append(text[jj]);
        fepMan.NewCharacterL(chr);
        }
    return ETrue;
    }

void TAknFepInputStateQwertyKorean::KeyTimerExpired()
    {
//    LOG1("KO.IS.QWERTY.KeyTimerExpired %d",iData);
    CPtiEngine& ptiEngine(*(iOwner->PtiEngine()));
    MAknFepManagerUIInterface& fepMan(*(iOwner->FepMan()));
    
    TPtrC text(ptiEngine.CurrentWord());
    TBuf<1> chr;            
    for (TInt jj = 0; jj < text.Length(); jj++)
        {
        chr.Zero();
        chr.Append(text[jj]);
        TRAP_IGNORE( fepMan.NewCharacterL(chr) );
        }
    
    //TRAPD(err, fepMan.CommitInlineEditL())
    TRAP_IGNORE( fepMan.CommitInlineEditL() )
    }

//fepMan.StartInlineEditL(text);
//iFepMan.StartInlineEditL()
//virtual void SetCursorSelectionL(const TCursorSelection& aCurSel, TBool aSyncCursor) = 0;
