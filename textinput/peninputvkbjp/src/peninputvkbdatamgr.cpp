/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the vkb data manager
*
*/


// system includes
#include <peninputlayoutcontext.h>
#include <aknfeppeninputenums.h>    // ERangeHiraganaKanji, ...

#include <PtiEngine.h>
#include <PtiCompositionDataIF.h>
#include <peninputcommonlayoutglobalenum.h>

// user includes
#include "peninputvkbdatamgr.h"

// constant definition
const TInt LengthSymbolFuncyional = 3;

const TUint KSymbolsFuncyional[] = {0x309b,  // dakuten(Full-width)
                                    0x309c,  // handakuten(Full-width)
                                    0x5c0f}; // little
const TUint16 KEWComma = 0x002C;
const TUint16 KEWFullStop = 0x002E;
const TUint16 KEWFullWidthComma = 0xFF0C;
const TUint16 KEWFullWidthFullStop = 0xFF0E;
const TUint16 KEWHalfWidthIdeographicComma = 0xFF64;  // touten
const TUint16 KEWHalfWidthIdeographicFullStop = 0xFF61;  // kuten
const TUint16 KEWIdeographicComma = 0x3001;  // touten
const TUint16 KEWIdeographicFullStop = 0x3002;  // kuten
const TUint KHalfDakuten = 0xff9e;
const TUint KHalfHanDakuten = 0xff9f;
const TUint KLittle = 0x5c0f;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPeninputVkbDataMgr::NewL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputVkbDataMgr* CPeninputVkbDataMgr::NewL(MPeninputLayoutContext* aContext)
    {
    CPeninputVkbDataMgr* self = NewLC(aContext);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CPeninputVkbDataMgr::NewLC
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputVkbDataMgr* CPeninputVkbDataMgr::NewLC(MPeninputLayoutContext* aContext)
    {
    CPeninputVkbDataMgr* self = new(ELeave) CPeninputVkbDataMgr(aContext);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CPeninputVkbDataMgr::~CPeninputVkbDataMgr
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputVkbDataMgr::~CPeninputVkbDataMgr()
    {
    delete iPtiEngine;
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbDataMgr::InitMoreJpL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// this method maybe Leave. this method is called by ConstructL()
///void CPeninputVkbDataMgr::InitMore()
void CPeninputVkbDataMgr::InitMoreJpL()
    {
    // Create engine
    iPtiEngine = CPtiEngine::NewL(ETrue);
    iPtiEngine->ActivateLanguageL(ELangJapanese, EPtiEngineHiraganaKanjiPredictiveVkb);
    }

// ---------------------------------------------------------------------------
// CPeninputVkbDataMgr::PtiEngine
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPtiEngine* CPeninputVkbDataMgr::PtiEngine()
    {
    return iPtiEngine;
    }

// ---------------------------------------------------------------------------
// CPeninputVkbDataMgr::Predictive
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool CPeninputVkbDataMgr::Predictive() const
    {
    return iPredictive;
    }

// ---------------------------------------------------------------------------
// CPeninputVkbDataMgr::FullHalf
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CPeninputVkbDataMgr::FullHalf() const
    {
    return iFullHalf;
    }

// ---------------------------------------------------------------------------
// CPeninputVkbDataMgr::JapaneseFlags
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CPeninputVkbDataMgr::JapaneseFlags() const
    {
    return iJapaneseFlags;
    }

// ---------------------------------------------------------------------------
// CPeninputVkbDataMgr::SetPermittedRange
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputVkbDataMgr::SetPermittedRange(TInt aRange)
    {
    iPermittedRange = aRange;
    TPtr ptr((TUint16* ) &iPermittedRange,sizeof(iPermittedRange)/sizeof(TUint16));
    iContext->SendEventToWindow(EPeninputLayoutEventSetPermittedRange,NULL,ptr);
    }


// ---------------------------------------------------------------------------
// CPeninputVkbDataMgr::SetEngineInputMode
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputVkbDataMgr::SetEngineInputMode(TInt aRange)
    {
    if (iPermittedRange & aRange)
        {
        TInt clearDirection = iClear;   // 1:delete 0:backspace
        TPtiEngineInputMode mode = EPtiEngineInputModeNone;
        switch (aRange)
            {
            case ERangeHiraganaKanji:
                mode = (Predictive())? EPtiEngineHiraganaKanjiPredictiveVkb
                                     : EPtiEngineHiraganaKanjiVkb;
                break;
            case ERangeKatakana:
                mode = EPtiEngineKatakanaVkb;
                break;
            case ERangeFullWidthKatakana:
                mode = EPtiEngineFullWidthKatakanaVkb;
                break;
            case ERangeHiraganaOnly:
                mode = EPtiEngineHiraganaVkb;
                break;
            default:
                break;
            }
        iPtiEngine->SetInputMode(mode);
        TRAP_IGNORE(iPtiEngine->HandleCommandL(EPtiCommandSetClearFunction, &clearDirection));
        }
    }

// ---------------------------------------------------------------------------
// CPeninputVkbDataMgr::SetPredictive
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputVkbDataMgr::SetPredictive(TBool aPredictive)
    {
    iPredictive = aPredictive;
    }

// ---------------------------------------------------------------------------
// CPeninputVkbDataMgr::SetFullHalf
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputVkbDataMgr::SetFullHalf(TInt aFullHalf)
    {
    iFullHalf = aFullHalf;
    }

// ---------------------------------------------------------------------------
// CPeninputVkbDataMgr::SetClear
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputVkbDataMgr::SetClear(TInt aClear)
    {
    iClear = aClear;
    }

// ---------------------------------------------------------------------------
// CPeninputVkbDataMgr::SetJapaneseFlags
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputVkbDataMgr::SetJapaneseFlags(TInt aJapaneseFlags)
    {
    iJapaneseFlags = aJapaneseFlags;
    }

// -----------------------------------------------------------------------------
// CPeninputVkbDataMgr::HandleFunctionalVkbEventL
// (other items were commented in a header).
// return aStatus == False --->UiStateMgr()->CurrentUiState()->HandleControlEvent() by UiLayout
// -----------------------------------------------------------------------------
//
void CPeninputVkbDataMgr::HandleFunctionalVkbEventL(const TDesC& aEventData, TUint aPreChar
                                                    , TBool& aStatus, TInt& aSendText)
    {
    aStatus = EFalse;
    TUint unicode =  *((TUint16*) aEventData.Ptr());  // always aEventData length == 1
    for (TInt ii=0; ii < LengthSymbolFuncyional; ii++)
        {
        if ( unicode == KSymbolsFuncyional[ii] )
            {
            iPtiEngine->ClearCurrentWord();
            iPtiEngine->HandleCommandL(EPtiCommandAppendCharacter, &aPreChar);
            iPtiEngine->HandleCommandL(EPtiCommandAppendCharacter, &unicode);
            const MPtiEngineCompositionDataInterface* data = iPtiEngine->CompositionData();
            TPtrC convertingString = data->ConvertingString();
            if (convertingString.Length() == 1 && aPreChar != convertingString[0])
                {
                //replace
                //
                TInt replacedLen = 1;
                HBufC* p = HBufC::NewL(convertingString.Length() + 2);
                p->Des().Append((TUint16*)&replacedLen, 2);
                p->Des().Append(convertingString);
                iContext->Sendkey( ESignalReplaceTextJp, *p );
                delete p;
                aSendText = 1;
                aStatus = ETrue;
                }
            else if (2 == ii)   // ignore little
                {
                aStatus = ETrue;
                }
            iPtiEngine->ClearCurrentWord();
            break;
            }
        }
    }
// -----------------------------------------------------------------------------
// CPeninputVkbDataMgr::HandleFunctionalVkbEventL
// (other items were commented in a header).
// return aStatus == False --->UiStateMgr()->CurrentUiState()->HandleControlEvent() by UiLayout
// -----------------------------------------------------------------------------
//
void CPeninputVkbDataMgr::HandleFunctionalVkbEventWithPrePreCharL(const TDesC& aEventData, TUint aPreChar
                                                                  , TUint aPrePreChar, TBool& aStatus, TInt& aSendText)
    {
    aStatus = EFalse;
    TUint unicode =  *((TUint16*) aEventData.Ptr());  // always aEventData length == 1
    if (KHalfDakuten == unicode || KHalfHanDakuten == unicode || KLittle == unicode)
        {
        TInt len = 2;
        iPtiEngine->ClearCurrentWord();
        if (aPrePreChar != KNoChar)
            {
            iPtiEngine->HandleCommandL(EPtiCommandAppendCharacter, &aPrePreChar);
            ++ len;
            }
        iPtiEngine->HandleCommandL(EPtiCommandAppendCharacter, &aPreChar);
        iPtiEngine->HandleCommandL(EPtiCommandAppendCharacter, &unicode);
        const MPtiEngineCompositionDataInterface* data = iPtiEngine->CompositionData();
        TPtrC convertingString = data->ConvertingString();
        TInt backSpace = EKeyDelete;
        TPtrC ptr(reinterpret_cast<TText*>(&backSpace), 1);
        if (convertingString.Length() != len )
            {
            if (convertingString.Length() == len-1 && len > 1 && aPreChar != convertingString[len-2])
                { // pa + "-->ba
                TInt replacedLen = 1;
                HBufC* p = HBufC::NewL(convertingString.Right(1).Length() + 2);
                p->Des().Append((TUint16*)&replacedLen, 2);
                p->Des().Append(convertingString.Right(1));
                iContext->Sendkey( ESignalReplaceTextJp, *p );
                delete p;
                aSendText = 1;
                }
            else if (convertingString.Length() == len-2 && len > 2)
                {
                if (aPrePreChar != convertingString[0])   // zu + little ---> little tsu
                    {
                    TInt replacedLen = 1;
                    HBufC* p = HBufC::NewL(convertingString.Length() + 2);
                    p->Des().Append((TUint16*)&replacedLen, 2);
                    p->Des().Append(convertingString);
                    iContext->Sendkey(ESignalKeyEvent, ptr);  // delete "
                    iContext->Sendkey( ESignalReplaceTextJp, *p );  // tsu --->little tsu
                    delete p;
                    }
                else  // ga + " ---> ka
                    {
                    iContext->Sendkey(ESignalKeyEvent, ptr);
                    }
                aSendText = 1;
                }
            }
        else if (aPreChar != convertingString[len-2])   // little tsu + " ---> zu
            {
            TInt replacedLen = 1;
            HBufC* p = HBufC::NewL(convertingString.Mid(len-2,1).Length() + 2);
            p->Des().Append((TUint16*)&replacedLen, 2);
            p->Des().Append(convertingString.Mid(len-2,1));
            iContext->Sendkey( ESignalReplaceTextJp, *p );
            delete p;
            iContext->Sendkey(ESignalKeyEvent, convertingString.Right(1));
            aSendText = 1;
            }
        else if (KLittle != unicode)   // ignore little
            {
            iContext->Sendkey(ESignalKeyEvent, convertingString.Right(1));
            aSendText = 1;
            }
        aStatus = ETrue;
        iPtiEngine->ClearCurrentWord();
        }
    }

// -----------------------------------------------------------------------------
// CPeninputVkbDataMgr::ConvertCommaFullStop
// (other items were commented in a header).
// return aStatus == False : not changed
// -----------------------------------------------------------------------------
//
TBool CPeninputVkbDataMgr::ConvertCommaFullStop(TUint& aNewChar) const
    {
    TUint unicode =  aNewChar;
    switch(unicode)
        {
        case KEWIdeographicComma:   // touten
            if (iJapaneseFlags & EPenInputJapaneseSettingQwertyComma)  // Japanese
                {
                aNewChar = (iFullHalf)? KEWIdeographicComma : KEWHalfWidthIdeographicComma;
                }
            else
                {   // English
                aNewChar = (iFullHalf)? KEWFullWidthComma : KEWComma;
                }
            break;
        case KEWIdeographicFullStop:  // kuten
            if (iJapaneseFlags & EPenInputJapaneseSettingQwertyPeriod)  // Japanese
                {
                aNewChar = (iFullHalf)? KEWIdeographicFullStop : KEWHalfWidthIdeographicFullStop;
                }
            else
                {   // English
                aNewChar = (iFullHalf)? KEWFullWidthFullStop : KEWFullStop;
                }
            break;
        case KEWHalfWidthIdeographicComma:   // touten(halfwidth)
            if (!(iJapaneseFlags & EPenInputJapaneseSettingQwertyComma))  // English
                {
                aNewChar = KEWComma;
                }
            break;
        case KEWHalfWidthIdeographicFullStop:   // kuten(halfwidth)
            if (!(iJapaneseFlags & EPenInputJapaneseSettingQwertyPeriod))  // English
                {
                aNewChar = KEWFullStop;
                }
            break;
        default:
            break;
        }
    return (aNewChar == unicode)? EFalse : ETrue;
    }

// ---------------------------------------------------------
// Return the clear direction
// ---------------------------------------------------------
//
TInt CPeninputVkbDataMgr::ClearDirection() const
    {
    return iClear;   // 1:delete 0:backspace
    }

// ---------------------------------------------------------------------------
// CPeninputVkbDataMgr::CPeninputVkbDataMgr
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputVkbDataMgr::CPeninputVkbDataMgr(MPeninputLayoutContext* aContext)
    : CPeninputDataMgr::CPeninputDataMgr(aContext),
     iPredictive(ETrue)
    {
    iInputLanguage = ELangJapanese;
    }

// --------------------------------------------------------------------------
// CPeninputVkbDataMgr::ConstructL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputVkbDataMgr::ConstructL()
    {
    CPeninputDataMgr::ConstructL();
    InitMoreJpL();
    }

// End Of File
