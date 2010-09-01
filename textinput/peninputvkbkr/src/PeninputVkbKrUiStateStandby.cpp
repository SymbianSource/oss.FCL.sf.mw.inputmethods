/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  peninput VKB data manager
*
*/

#include <aknfeppeninputenums.h>
#include <peninputlayoutbasecontrol.h>
#include <peninputcommonlayoutglobalenum.h>
#include <peninputdataconverter.h>
#include <peninputlayoutcontext.h>
#include <peninputlayoutvkb.h>
#include <PtiEngine.h>

#include "PeninputVkbKrDbg.h"
#include "PeninputVkbKr.hrh"
#include "PeninputVkbKrUiStateStandby.h"

const TInt KSingleCharacter = 1;

// ---------------------------------------------------------------------------
// Constructors & Destructors
// ---------------------------------------------------------------------------
//

CPeninputVkbKrUiStateStandby* CPeninputVkbKrUiStateStandby::NewLC(
        MPeninputUiStateMgr* aUiStateMgr, 
        MPeninputLayoutContext* aContext,
        CPtiEngine& aPtiEngine)
    {
    CPeninputVkbKrUiStateStandby* self =
        new (ELeave) CPeninputVkbKrUiStateStandby(aUiStateMgr,aContext,aPtiEngine);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CPeninputVkbKrUiStateStandby* CPeninputVkbKrUiStateStandby::NewL(
        MPeninputUiStateMgr* aUiStateMgr, 
        MPeninputLayoutContext* aContext,
        CPtiEngine& aPtiEngine)
    {
    CPeninputVkbKrUiStateStandby* self = 
        CPeninputVkbKrUiStateStandby::NewLC(aUiStateMgr,aContext,aPtiEngine);
    CleanupStack::Pop(); // self;
    return self;
    }

CPeninputVkbKrUiStateStandby::CPeninputVkbKrUiStateStandby(
        MPeninputUiStateMgr* aUiStateMgr, 
        MPeninputLayoutContext* aContext,
        CPtiEngine& aPtiEngine)
    : CPeninputVkbKrUiStateBase(aUiStateMgr,aContext,aPtiEngine),
      iTextCommited(EFalse)
    {
    // No implementation required
    }

void CPeninputVkbKrUiStateStandby::ConstructL()
    {
    }

CPeninputVkbKrUiStateStandby::~CPeninputVkbKrUiStateStandby()
    {
    }

// ---------------------------------------------------------------------------
// from CPeninputVkbKrUiStateBase
// ---------------------------------------------------------------------------
//

TBool CPeninputVkbKrUiStateStandby::HandleKeyEventL(
        const TRawEvent& aData)
    {
    LOG1("Vkb.UiState.Standby.HandleKeyEventL %d",aData.ScanCode());
    TBool ret(EFalse);
    return ret;
    }

TBool CPeninputVkbKrUiStateStandby::HandleControlEvent(
        TInt aEventType,
        const TDesC& aEventData)
    {
    TBool ret(EFalse);
    TBool clearAutomata(ETrue);
    switch ( aEventType )
        {
        case EEventButtonDown:
        case EEventButtonUp:
        case EEventVirtualKeyDown:
            clearAutomata=EFalse;
            break;
        case EEventVirtualKeyUp:
            ret=HandleVkbEvent( aEventType, aEventData );
            clearAutomata=EFalse;
            break;
        case EPeninputLayoutEventBack:
            ret=HandleBackEvent( aEventType, aEventData );
            clearAutomata=EFalse;
            break;            
        case EPeninputLayoutEventLeftArrow:
        case EPeninputLayoutEventRightArrow:
            SendKey( aEventData );
            break;
        case EPeninputLayoutEventTab:
            ret=HandleTabEvent( aEventType, aEventData ); 
            break;
        case EPeninputLayoutEventEnter:
        case EPeninputLayoutEventSpace:
            ret=HandleEnterSpaceEvent( aEventType, aEventData );
            break;
        case EPeninputLayoutEventClose:
        default:
            break;
        }
    LOG2(
        "Vkb.UiState.Standby.HandleControlEvent %d - %d",
        aEventType,
        ret);
    
    if (clearAutomata)
        {
        LOG("Vkb.UiState.Standby.HandleControlEvent CLEAR AUTOMATA !!");
        iPtiEngine.ClearCurrentWord();
        }
    
    return ret;
    }

void CPeninputVkbKrUiStateStandby::KeyTimerExpired()
    {
    //TPtrC eventData(iPtiEngine.CurrentWord());
    //SendKey( eventData );
    iTextCommited=ETrue;
    }

void CPeninputVkbKrUiStateStandby::LastWordInSelectionList()
    {
    
    }

void CPeninputVkbKrUiStateStandby::FirstWordInSelectionList()
    {
    
    }

// ---------------------------------------------------------------------------
// Internal use
// ---------------------------------------------------------------------------
//

TBool CPeninputVkbKrUiStateStandby::HandleVkbEvent( 
        TInt /*aEventType*/,
        const TDesC& aEventData )
    {
    TBool ret(EFalse);
    if (IsKoreanRange())
        {
        ret=HandleKoreanKey(aEventData);
        }
    
    if (IsKoreanShiftLayout())
        {
        iContext->SendEventToWindow(EPeninputLayoutEventShift,NULL);
        }

    if (!ret)
        {
        iPtiEngine.ClearCurrentWord();
        ret=SendKey(aEventData);
        }
    
    return ret;
    }
    
// --------------------------------------------------------------------------
// CPeninputUiStateVkbStandby::HandleBackEvent
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TBool CPeninputVkbKrUiStateStandby::HandleBackEvent( 
        TInt /*aEventType*/, 
        const TDesC& aEventData )
    {
    TBool ret(EFalse);
    if (IsKoreanRange())
        {
        ret=HandleKoreanKey(aEventData);
        }

    if (!ret)
        {
        iPtiEngine.ClearCurrentWord();
        ret=SendKey(aEventData);
        }
    return ret;
    }
    
TBool CPeninputVkbKrUiStateStandby::HandleEnterSpaceEvent( 
        TInt /*aEventType*/, 
        const TDesC& aEventData )
    {
    return SendKey( aEventData );
    }
    
TBool CPeninputVkbKrUiStateStandby::HandleTabEvent( 
        TInt /*aEventType*/,  
        const TDesC& aEventData )
    {
    SendKey( aEventData );
    return EFalse;
    }

TBool CPeninputVkbKrUiStateStandby::IsKoreanRange()
    {
    TInt range = CPeninputDataConverter::AnyToInt(
        iContext->RequestData(EPeninputDataTypeCurrentRange));
    return range==ERangeNative;
    }

TBool CPeninputVkbKrUiStateStandby::IsKoreanShiftLayout()
    {
    TInt vkbLayout = CPeninputDataConverter::AnyToInt(
        iContext->RequestData(EPeninputDataTypeVkbLayout));
    return vkbLayout==EPeninputVkbLayoutNativeCharShift;        
    }

TInt CPeninputVkbKrUiStateStandby::GetVkbLayout()
    {
    TInt layout = CPeninputDataConverter::AnyToInt(
        iContext->RequestData(EPeninputDataTypeVkbLayout));
    return layout;        
    }

TBool CPeninputVkbKrUiStateStandby::HandleKoreanKey(const TDesC& aEventData)
    {
    TBool ret(EFalse);
    
    TUint16 keyCodeOrig;
    TUint16 keyCodePti;
    if (aEventData.Length()>1)
        {
        TVirtualKeyEventData* data = (TVirtualKeyEventData*)aEventData.Ptr();
        
        __ASSERT_ALWAYS(data->iKeyData.Length()==1,User::Panic(_L("Multiple Key"),__LINE__));
        
        if (!(0x3130<=data->iKey->KeyCode()&&data->iKey->KeyCode()<=0x318F))
            {
            return EFalse;
            }
        keyCodeOrig=data->iKey->ScanCode();
        }
    else
        {
        keyCodeOrig=*(aEventData.Ptr());
        }
    
    TBool latin(EFalse);
    keyCodePti=keyCodeOrig;
    if ('a'<=keyCodeOrig&&keyCodeOrig<='z')
        {
        TRAP_IGNORE(iPtiEngine.HandleCommandL(
            EPtiCommandUseLatinCaseLowerOnlyKeyMap,
            NULL));
        keyCodePti=keyCodeOrig-'a'+'A';
        latin=ETrue;
        }
    else if ('A'<=keyCodeOrig&&keyCodeOrig<='Z')
        {
        TRAP_IGNORE(iPtiEngine.HandleCommandL(
            EPtiCommandUseLatinCaseUpperOnlyKeyMap,
            NULL));
        latin=ETrue;
        }
    
    TInt previousLen = iPtiEngine.CurrentWord().Length();
    
    switch (keyCodePti)
        {
        case '\b': // back space
            if (previousLen)
                {
                iPtiEngine.DeleteKeyPress();
                TPtrC currentWord(iPtiEngine.CurrentWord());
                SendReplaceText(previousLen,currentWord);
                ret=ETrue;
                }
            break;
        
        default:
            {
            if (latin)
                {
             	             	
                TPtrC text = iPtiEngine.AppendKeyPress((TPtiKey)(keyCodePti));
                
                if(previousLen)
                    {
                    SendReplaceText(previousLen,iPtiEngine.CurrentWord());
                    ret=ETrue;
                    
                   	if(iPtiEngine.CurrentWord().Length() >= 2)
             			{
             			if (CommitOrNot(text.Right(KSingleCharacter)))
             				{
             				iPtiEngine.DeleteKeyPress();
             				iPtiEngine.CommitCurrentWord();
             				iPtiEngine.AppendKeyPress((TPtiKey)(keyCodePti));
             			//	iContext->Sendkey(ESignalKeyEvent,iPtiEngine.CurrentWord());
                    		ret=ETrue;
             				}
             			}
                    }
                else
                    {
                    iContext->Sendkey(ESignalKeyEvent,iPtiEngine.CurrentWord());
                    ret=ETrue;
                    }
                }
            else
                {
                iPtiEngine.ClearCurrentWord();
                }
            }
            break;
        }
    
    return ret;
    }


void CPeninputVkbKrUiStateStandby::SendReplaceText(
        const TInt& aLenToReplace,
        const TDesC& aTextToReplace)
    {
    TInt lenTextToReplace(aTextToReplace.Length());
    LOG2(
        "Vkb.UiState.Standby.SendReplaceText %d - %d",
        aLenToReplace,
        lenTextToReplace);
    
    if (aLenToReplace)
        {
        if (aLenToReplace<=lenTextToReplace)
            {
            HBufC* p = HBufC::New(lenTextToReplace+2);
            if (p)
                {
                TPtr eventData(p->Des());
                eventData.Zero();
                eventData.Append((TUint16*)&aLenToReplace, 2);
                eventData.Append(aTextToReplace);
                iContext->Sendkey(ESignalReplaceText,eventData);
                delete p;
                }
            }
        else
            {
            TBuf<1> backSpace;
            backSpace.Append(TChar(EKeyBackspace));
            iContext->Sendkey(ESignalKeyEvent,backSpace);
            iPtiEngine.ClearCurrentWord();
            }
        }
    else
        {
        iContext->Sendkey(ESignalKeyEvent,aTextToReplace);
        }
    }
TBool CPeninputVkbKrUiStateStandby::CommitOrNot(TPtrC aData)
	{
	TBool commit = EFalse;
	TBuf<KSingleCharacter> mappings;
	
	TPtiKey key = iPtiEngine.CharacterToKey(*aData.Ptr());
	
	iPtiEngine.MappingDataForKey(key, mappings, iPtiEngine.Case());
	TInt len = mappings.Length();
	
	if(aData.Compare(mappings) == 0)
		{
		commit = ETrue;
		}
		
	return commit;
	}
