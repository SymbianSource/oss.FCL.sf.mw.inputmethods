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
*       Provides the TAknFepInputStateInitialVietnameseMultitap methods.
*
*/












#include "AknFepUIInputStateInitialVietnameseMultitap.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface

#include "AknFepCaseManager.h"
#include "AknFepVietnameseToneManager.h"

#include <uikon.hrh>

#include <PtiEngine.h>
#include <PtiDefs.h>
#include <aknSctDialog.h>
#include <avkon.rsg>
#include <aknfep.rsg>

#include "AknFepUiManagerWestern.h"

TAknFepInputStateInitialVietnameseMultitap::
TAknFepInputStateInitialVietnameseMultitap(MAknFepUIManagerStateInterface* aOwner)
:TAknFepInputStateInitialMultitapBase(aOwner)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TPtiTextCase caseMode = ptiengine->Case();
    ptiengine->SetCase(caseMode);
    }

TBool TAknFepInputStateInitialVietnameseMultitap::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    TBool bHandled = EFalse;

    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CAknFepUIManagerWestern* uiMan = static_cast<CAknFepUIManagerWestern*>(iOwner);
    CAknFepVietnameseToneManager* toneMgr = uiMan->VietnameseToneManager();
    CPtiEngine* ptiEng = fepMan->PtiEngine();

    toneMgr->SetFepManager(fepMan);

    if (aKey == EPtiKeyStar)
        {
        if (aLength == EShortKeyPress)
            {
            if (!toneMgr->IsLooping())
                {
                // Start looping status
                if (toneMgr->StartToneMarkLooping())
                    {
                    // Save the vowel for undo back deletion
                    toneMgr->SetDeletedVowel(fepMan->PreviousChar());
                    
                    ((CAknFepManager*)fepMan)->RemovePreviousCharacterL();    // remove previous char
                    }
                }
            
            // Process the interim looping state
            if (toneMgr->IsLooping())
                {
                // Get current tone mark
                TText toneVowel = toneMgr->GetVowelWithToneMark();
                toneMgr->IncrementToneMark();

                TBuf<CAknFepManager::ESingleCharacter> aChr;
                aChr.Append(toneVowel);

                // Update inline character(s)
                fepMan->CancelInlineEdit();
                fepMan->NewCharacterL(aChr);

                // Reset the engine timer so that we can get timer expired message
                // We send the * key, and clear it in the next loop
                ptiEng->ClearCurrentWord();
                ptiEng->AppendKeyPress((TPtiKey)aKey);

                bHandled = ETrue;
                
                if (fepMan->EditSubmenuInUse())
                    {
                    // Looping tone marks, supress edit-menu.
                    fepMan->SetCcpuFlag(CAknFepManager::ECcpuStateIgnoreStarUp);
                    }
                }
            }
        else if (aLength == ELongKeyPress)
            {
            // Reset the key event so that the TAknFepInputStateInitialMultitapBase will
            // parse this event as a short key event, default reaction is to launch SCT
            aLength = EShortKeyPress;

            if (toneMgr->IsLooping())
                {
                // Remove the previous EShortKeyPress message
                fepMan->CancelInlineEdit();
                toneMgr->StopToneMarkLooping();

                // Undo the backdeletion
                TText bkChr = toneMgr->GetDeletedVowel();

                TBuf<CAknFepManager::ESingleCharacter> aChr;
                aChr.Append(bkChr);

                fepMan->NewCharacterL(aChr);
                fepMan->CommitInlineEditL();
                
                if (fepMan->EditSubmenuInUse() && fepMan->IsAbleToLaunchSCT())
                    {
                    fepMan->LaunchSpecialCharacterTableL();
                    return ETrue;
                    }                         
                }
            else if (fepMan->EditSubmenuInUse())
                {
                aLength = ELongKeyPress;
                }
            }
        }
    
    // Implementation for the Vietnamese back deletion
    if (aKey == EKeyBackspace)
        {
        // Key looping should be cancelled as pressing backspace key
        toneMgr->StopKeyLooping();
    
        TBuf<2> newText;
        // Get the last character from the current editor
        TText prevChar = fepMan->PreviousChar();
        
        // Judge if the last character needs to be converted
        if ( NeedsVietnameseBkDelSupport(prevChar, newText) )
            {
            // Delete the prev character and send the new character to editor
            ((CAknFepManager*)fepMan)->RemovePreviousCharacterL();
            fepMan->NewCharacterL(newText);
            fepMan->CommitInlineEditL();
            // Stop looping for backspace operation
            if( toneMgr->IsLooping() )
                {
                toneMgr->StopToneMarkLooping();
                }
            return ETrue;
            }
        else
            {
            if ( prevChar )
                {
                ((CAknFepManager*)fepMan)->RemovePreviousCharacterL();
                }
            else
                {
                return EFalse;
                }
                
            bHandled = EFalse;
            }
        }
    
    if( aLength == EShortKeyPress )
        {
        if( toneMgr->IsKeyLooping( aKey ) )
            {
            if( toneMgr->ToneMarkIndex() !=  KErrNotFound )
                {
                // Get current tone mark
                TText toneVowel;
                if( toneMgr->GetLoopingToneMarkVowel( toneVowel ) )
                    {
                    // remove two previous chars
                    ((CAknFepManager*)fepMan)->RemovePreviousCharacterL();
                    ((CAknFepManager*)fepMan)->RemovePreviousCharacterL();
        
                    TBuf<CAknFepManager::ESingleCharacter> aChr;
                    aChr.Append(toneVowel);
                    
                    // Update inline character(s)
                    fepMan->CancelInlineEdit();
                    fepMan->NewCharacterL(aChr);
                    
                    toneMgr->SetLoopingCombined( ETrue );
                    bHandled = ETrue;
                    }
                
                // Reset the engine timer so that we can get timer expired message
                // We send the * key, and clear it in the next loop
                ptiEng->ClearCurrentWord();
                ptiEng->AppendKeyPress((TPtiKey)EPtiKeyStar);
                }
            else
                {
                if( toneMgr->IsLoopingCombined() )
                    {
                    // remove tone mark
                    TBuf<2> newText;
                    // Get the last character from the current editor
                    TText prevChar = fepMan->PreviousChar();
                    
                    // Judge if the last character needs to be converted
                    if ( NeedsVietnameseBkDelSupport(prevChar, newText) )
                        {
                        // Delete the prev character and send the new character to editor
                        ((CAknFepManager*)fepMan)->RemovePreviousCharacterL();
                        fepMan->NewCharacterL(newText);
                        fepMan->CommitInlineEditL();
                        }
                
                    toneMgr->SetLoopingCombined( EFalse );
                    }
                }
            }
        else
            {
            // Key looping is not supported with Backspace key
            // Otherwise the process of deleting a2222 is wrong
            if ( aKey != EKeyBackspace )
                {
                toneMgr->StartKeyLooping( aKey );
                }
            }
        }
    else
        {
        // Key looping should be cancelled as long pressing key  
        // Otherwise a22 can not be inputted by press a, long press 2 and long press 2.
        toneMgr->StopKeyLooping();
        }
    
    if(!bHandled && toneMgr->IsLooping())
        {
        toneMgr->StopToneMarkLooping();
        }
    
    return bHandled ? ETrue : TAknFepInputStateInitialMultitapBase::HandleKeyL(aKey, aLength);
    }

void TAknFepInputStateInitialVietnameseMultitap::KeyTimerExpired()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CAknFepUIManagerWestern* uiMan = static_cast<CAknFepUIManagerWestern*>(iOwner);
    CAknFepVietnameseToneManager* toneMgr = uiMan->VietnameseToneManager();  
    CPtiEngine* ptiEng = fepMan->PtiEngine();

    // Try commit inline editting and clean up for future input attemps
    TRAP_IGNORE(fepMan->CommitInlineEditL());

    toneMgr->StopToneMarkLooping();
    toneMgr->StopKeyLooping();
    ptiEng->ClearCurrentWord();
    }
    
TBool TAknFepInputStateInitialVietnameseMultitap::NeedsVietnameseBkDelSupport(
                                         TText aGlyphToDelete, TDes& aNewText)
    {
    TBool needBkDelSupport(EFalse);

    const TInt KMatrixColumn = sizeof(VietToneMatrix) / sizeof(TText) / KNumberOfToneMarks;

    aNewText.Zero();

    // Loop of the ToneMatrix columns starts from 1 because the first column of the ToneMatrix stores the vowels 
    for (TUint i = 1; (i < KMatrixColumn) && (!needBkDelSupport); ++i)
        {
        for (TUint j = 0; j < KNumberOfToneMarks; ++j)
            {
            if (VietToneMatrix[i][j] == aGlyphToDelete)
                {
                needBkDelSupport = ETrue;
                aNewText.Append(VietVowelList[i - 1]);
                break;
                }
            }
        }

    return needBkDelSupport;
    }

// End of file
