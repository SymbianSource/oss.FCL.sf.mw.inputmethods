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
* Description:  peninput hwr data manager
*
*/


// System includes
#include <peninputhwrwindowjp.rsg>
#include <centralrepository.h>
#include <settingsinternalcrkeys.h>
#include <peninputlayoutcontext.h> // MPeninputLayoutContext
#include <aknfeppeninputenums.h>    // ERangeHiraganaKanji, ...
#include <PtiEngine.h>

// User includes
#include "peninputhwrevent.h"  // EPeninputLayoutHwrEventCandidates, ....
#include "peninputhwrboxdatamgr.h"
#include "peninputhwrboxrecognizer.h"

// Constants
const TInt KPremaryCandidateCount = 36;
const TInt KCandidateCount = 10;
enum
    {
    EHwrWritingSpeedVeryFast    = 300 * 1000,
    EHwrWritingSpeedFast        = 400 * 1000,
    EHwrWritingSpeedNormal      = 550 * 1000,
    EHwrWritingSpeedSlow        = 700 * 1000,
    EHwrWritingSpeedVerySlow    = 1000 * 1000
    };


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::NewL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputHwrBoxDataMgr* CPeninputHwrBoxDataMgr::NewL(MPeninputLayoutContext* aContext)
    {
    CPeninputHwrBoxDataMgr* self = NewLC(aContext);
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::NewLC
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputHwrBoxDataMgr* CPeninputHwrBoxDataMgr::NewLC(MPeninputLayoutContext* aContext)
    {
    CPeninputHwrBoxDataMgr* self =
        new ( ELeave ) CPeninputHwrBoxDataMgr( aContext );
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::~CPeninputHwrBoxDataMgr
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputHwrBoxDataMgr::~CPeninputHwrBoxDataMgr()
    {
    iCandidates.ResetAndDestroy();
    iCandidates.Close();

    iRangeStartPos.Close();

    iPermittedRanges.Close();

    delete iHwrBoxRecognizer;
    delete iPtiEngine;
    delete iCharacterRangeArray;
    }

// ---------------------------------------------------------
// Do recoginize by engine
// ---------------------------------------------------------
//
void CPeninputHwrBoxDataMgr::DoRecognize(const RArray<TPoint>& aTraceData)
    {
    if (iHwrBoxRecognizer)
        {
        iHwrBoxRecognizer->DoRecognize(aTraceData, iCandidates, iRangeStartPos);
        }

    SetCandidates();
    }

// ---------------------------------------------------------
// Get the candidate count
// ---------------------------------------------------------
//
TInt CPeninputHwrBoxDataMgr::CandidateCount() const
    {
    return iCandidates.Count();
    }

// ---------------------------------------------------------
// Set permitted ranges
// ---------------------------------------------------------
//
void CPeninputHwrBoxDataMgr::SetPermittedRanges(TInt aPermittedRanges)
    {
    iPermittedRanges.Reset();

    if (aPermittedRanges & ERangeNative)
        {
        iPermittedRanges.Append(ERangeNative);
        }
    if ((aPermittedRanges & ERangeEnglish) && !FullHalf())
        {
        iPermittedRanges.Append(ERangeEnglish);
        }

    if ((aPermittedRanges & ERangeNumber) && !FullHalf())
        {
        iPermittedRanges.Append(ERangeNumber);
        }

    if (aPermittedRanges & ERangeSymbol)
        {
        iPermittedRanges.Append(ERangeSymbol);
        }

    if (aPermittedRanges & ERangeHiraganaKanji)
        {
        iPermittedRanges.Append(ERangeHiraganaKanji);
        }

    if ((aPermittedRanges & ERangeKatakana) && !FullHalf())
        {
        iPermittedRanges.Append(ERangeKatakana);
        }

    if ((aPermittedRanges & ERangeFullWidthKatakana) && FullHalf())
        {
        iPermittedRanges.Append(ERangeFullWidthKatakana);
        }

    if ((aPermittedRanges & ERangeFullWidthEnglish) && FullHalf())
        {
        iPermittedRanges.Append(ERangeFullWidthEnglish);
        }

    if ((aPermittedRanges & ERangeFullWidthNumeric) && FullHalf())
        {
        iPermittedRanges.Append(ERangeFullWidthNumeric);
        }

    if (aPermittedRanges & ERangeHiraganaOnly)
        {
        iPermittedRanges.Append(ERangeHiraganaOnly);
        }

    // set primary and auxiliary ranges for hwr engine
    if (iHwrBoxRecognizer)
        {
        iHwrBoxRecognizer->SetRanges(iPermittedRanges);
        }
    }

// ---------------------------------------------------------
// Get the permitted ranges count
// ---------------------------------------------------------
//
TInt CPeninputHwrBoxDataMgr::PermittedRangesCount() const
    {
    return iPermittedRanges.Count();
    }

// ---------------------------------------------------------
// Set premary range
// ---------------------------------------------------------
//
void CPeninputHwrBoxDataMgr::SetPremaryRange(TInt aPremaryRange)
    {
    const TInt oldIndex = iPermittedRanges.Find(aPremaryRange);

    if (oldIndex > 0)
        {
        iPermittedRanges.Sort();
        const TInt index = iPermittedRanges.Find(aPremaryRange);

        iPermittedRanges.Remove(index);
        iPermittedRanges.Insert(aPremaryRange, 0);
        }

    iContext->SendEventToWindow(EPeninputLayoutEventSetCharacterRangeText,NULL,KNullDesC);

    // set primary and auxiliary ranges for hwr engine
    if (iHwrBoxRecognizer)
        {
        iHwrBoxRecognizer->SetRanges(iPermittedRanges);
        }
    }

// ---------------------------------------------------------
// Set the candidate list
// ---------------------------------------------------------
//
void CPeninputHwrBoxDataMgr::SetCandidates()
    {
    TPtrC ptr;
    ptr.Set(reinterpret_cast<TText*>(&iCandidates), sizeof(iCandidates)/sizeof(TUint16));
    iContext->SendEventToWindow(EPeninputLayoutHwrEventCandidates, NULL, ptr);
    }

// ---------------------------------------------------------
// Get WritingSpeed
// ---------------------------------------------------------
//
TInt CPeninputHwrBoxDataMgr::WritingSpeed() const
    {
    return iWritingSpeed;
    }

// ---------------------------------------------------------
// Get PenSize
// ---------------------------------------------------------
//
TSize CPeninputHwrBoxDataMgr::PenSize() const
    {
    return iPenSize;
    }

// ---------------------------------------------------------
// Get PenColor
// ---------------------------------------------------------
//
TRgb CPeninputHwrBoxDataMgr::PenColor() const
    {
    return iPenColor;
    }

// ---------------------------------------------------------
// Get the candidate
// ---------------------------------------------------------
//
void CPeninputHwrBoxDataMgr::GetCandidate(TInt aIndex, TDes& aCharCode) const
    {
    aCharCode.Zero();
    if (aIndex >=0 && aIndex < iCandidates.Count())
        {
        aCharCode = *iCandidates[aIndex];
        }
    }

// ---------------------------------------------------------
// Clear the candidate list
// ---------------------------------------------------------
//
void CPeninputHwrBoxDataMgr::ClearCandidates()
    {
    iCandidates.ResetAndDestroy();

    TPtrC ptr;
    ptr.Set(reinterpret_cast<TText*>(&iCandidates), sizeof(iCandidates)/sizeof(TUint16));
    iContext->SendEventToWindow(EPeninputLayoutHwrEventCandidates, NULL, ptr);
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::InitMoreJpL
// instesd of "void InitMore();"
// (other items were commented in a header)
// ---------------------------------------------------------------------------
void CPeninputHwrBoxDataMgr::InitMoreJpL()
    {
    // Read pen data
    if ( iGSRepository )
        {
        TInt newValue;
        newValue = 0;
        iGSRepository->Get( KSettingsPenColor, newValue );
        iPenColor = newValue;

        newValue = 1;
        iGSRepository->Get( KSettingsPenWidth, newValue );
        iPenSize = TSize( newValue, newValue );

        newValue = 2;
        iGSRepository->Get( KSettingsWritingSpeed, newValue );
        iWritingSpeed = TransferWritingSpeed( newValue );
        }

    iPtiEngine = CPtiEngine::NewL( ETrue );
    iPtiEngine->ActivateLanguageL(ELangJapanese, EPtiEngineHiraganaKanjiPredictiveHwr);

    iHwrBoxRecognizer = CPeninputHwrBoxRecognizer::NewL(iPtiEngine);
    iCharacterRangeArray = new(ELeave) CDesCArrayFlat(1);
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::HandleGSRepositoryChange
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxDataMgr::HandleGSRepositoryChange( TInt aChangedKey )
    {
    TInt newValue = 0;

    switch ( aChangedKey )
        {
        case KSettingsPenColor:
            {
            iGSRepository->Get( KSettingsPenColor, newValue );
            iPenColor = newValue;
            TPtr ptr((TUint16*)&iPenColor,sizeof(TRgb)/sizeof(TUint16));
            iContext->SendEventToWindow(EPeninputLayoutHwrEventPenColorChange, NULL, ptr);
            }
            break;
        case KSettingsPenWidth:
            {
            iGSRepository->Get( KSettingsPenWidth, newValue );
            iPenSize = TSize( newValue, newValue );
            TPtr ptr((TUint16*)&iPenSize,sizeof(TSize)/sizeof(TUint16));
            iContext->SendEventToWindow(EPeninputLayoutHwrEventPenSizeChange, NULL, ptr);
            }
            break;
        case KSettingsWritingSpeed:
            {
            iGSRepository->Get( KSettingsWritingSpeed, newValue );
            iWritingSpeed = TransferWritingSpeed( newValue );
            TPtr ptr((TUint16*)&iWritingSpeed,sizeof(TInt)/sizeof(TUint16));
            iContext->SendEventToWindow(EPeninputLayoutHwrEventWritingSpeedChange, NULL, ptr);
            }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::PtiEngine
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPtiEngine* CPeninputHwrBoxDataMgr::PtiEngine()
    {
    return iPtiEngine;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::Predictive
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool CPeninputHwrBoxDataMgr::Predictive() const
    {
    return iPredictive;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::FullHalf
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CPeninputHwrBoxDataMgr::FullHalf() const
    {
    return iFullHalf;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::Conversion
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool CPeninputHwrBoxDataMgr::Conversion() const
    {
    return iConversion;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::Conversion
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CDesCArrayFlat* CPeninputHwrBoxDataMgr::CharacterRangeArray()
    {
    return iCharacterRangeArray;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::CharacterRangeId
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CPeninputHwrBoxDataMgr::CharacterRangeId() const
    {
    TInt rangeId = EPeninutCharacterRangeIdAll;
    if (0 <= iCurrentCharacterRangeIndex && iCurrentCharacterRangeIndex < KCharacterRangePopupMaxRow)
        {
        rangeId = iCharacterRangeIdArray.At(iCurrentCharacterRangeIndex);
        }
    return rangeId;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::CurrentCharacterRangeIndex
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CPeninputHwrBoxDataMgr::CurrentCharacterRangeIndex() const
    {
    return iCurrentCharacterRangeIndex;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::SetEngineInputMode
// (other items were commented in a header).
// ---------------------------------------------------------------------------
void CPeninputHwrBoxDataMgr::SetEngineInputMode(TInt aRange)
    {
    if (iPermittedRange & aRange)
        {
        TInt clearDirection = iClear;   // 1:delete 0:backspace
        TPtiEngineInputMode mode = EPtiEngineInputModeNone;
        if (ERangeHiraganaKanji == aRange && Conversion())
            {
            mode = (Predictive())? EPtiEngineHiraganaKanjiPredictiveHwr
                                 : EPtiEngineHiraganaKanjiHwr;
            }
        iPtiEngine->SetInputMode(mode);
        TRAP_IGNORE(iPtiEngine->HandleCommandL(EPtiCommandSetClearFunction, &clearDirection));
        }
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::SetPredictive
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxDataMgr::SetPredictive(TBool aPredictive)
    {
    iPredictive = aPredictive;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::SetFullHalf
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxDataMgr::SetFullHalf(TInt aFullHalf)
    {
    iFullHalf = aFullHalf;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::SetClear
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxDataMgr::SetClear(TInt aClear)
    {
    iClear = aClear;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::SetConversion
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxDataMgr::SetConversion(TBool aConversion)
    {
    iConversion = aConversion;
   }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::SetCurrentCharacterRangeIndex
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxDataMgr::SetCurrentCharacterRangeIndex(TInt aIndex)
    {
    iCurrentCharacterRangeIndex = aIndex;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::Reset
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxDataMgr::Reset()
    {
    iPermittedRange = 0;
    iPermittedCase = 0;
    iCurrentRange = 0;
    iCase = 0;
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::SetInputAreaSize
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBoxDataMgr::SetInputAreaSize( const TSize& aSize )
    {
    if ( iHwrBoxRecognizer )
        {
        iHwrBoxRecognizer->SetInputAreaSize( aSize );
        }
    }

// ---------------------------------------------------------
// Return the clear direction
// ---------------------------------------------------------
//
TInt CPeninputHwrBoxDataMgr::ClearDirection() const
    {
    return iClear;   // 1:delete 0:backspace
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::CPeninputHwrBoxDataMgr
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputHwrBoxDataMgr::CPeninputHwrBoxDataMgr(
    MPeninputLayoutContext* aContext )
    : CPeninputDataMgr( aContext ),
     iPredictive(ETrue),
     iConversion(ETrue)
    {
    iInputLanguage = ELangJapanese;
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::ConstructL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBoxDataMgr::ConstructL()
    {
    CPeninputDataMgr::ConstructL();
    InitMoreJpL();
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::TransferWritingSpeed
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CPeninputHwrBoxDataMgr::TransferWritingSpeed( TInt aWritingSpeed ) const
    {
    TInt speed;

    switch ( aWritingSpeed )
        {
        case 0:
            {
            speed = EHwrWritingSpeedVeryFast;
            }
            break;
        case 1:
            {
            speed = EHwrWritingSpeedFast;
            }
            break;
        case 2:
            {
            speed = EHwrWritingSpeedNormal;
            }
            break;
        case 3:
            {
            speed = EHwrWritingSpeedSlow;
            }
            break;
        case 4:
            {
            speed = EHwrWritingSpeedVerySlow;
            }
            break;
        default:
            {
            speed = EHwrWritingSpeedNormal;
            }
            break;
        }

    return speed;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxDataMgr::NotifyEngine
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxDataMgr::NotifyEngine( TPeninputDataType aType,
                                               TAny* aData )
    {
    // Call the function in base class, which will store data to CenRep
    CPeninputDataMgr::NotifyEngine( aType, aData );

    TInt* data = ( TInt* ) aData;

    // Notify engine, when language, range, case changed
    switch ( aType )
        {
        case EPeninputDataTypePermittedRange:
            {
            SetPermittedRanges(*data);
            TRAP_IGNORE(SetCharacterRangePopupL(*data));
            }
            break;
        case EPeninputDataTypeCase:
            {
            iHwrBoxRecognizer->SetCase( *data );
            }
            break;
        case EPeninputDataTypeInputLanguage:
            {
            // select suitable hwr recognizer according to language
            if (iHwrBoxRecognizer)
                {
                TRAP_IGNORE(iHwrBoxRecognizer->SetLanguageL(*data));
                iHwrBoxRecognizer->SetPrimaryCandidateNum(KPremaryCandidateCount);
                iHwrBoxRecognizer->SetCandidateNum(KCandidateCount);

                TPoint strokeEndMark = iHwrBoxRecognizer->StrokeEndMark();
                TPtr ptr((TUint16*)&strokeEndMark,sizeof(TPoint)/sizeof(TUint16));
                iContext->SendEventToWindow(EPeninputLayoutHwrEventStrokeEndMark, NULL, ptr);
                }
            }
            break;
        case EPeninputDataTypeCurrentRange:
            {
            SetPremaryRange( *data );
            }
            break;
        case EPeninputDataTypeNumericKeymap:
            {
            iHwrBoxRecognizer->SetNumberMode((TAknEditorNumericKeymap)*data);
            iHwrBoxRecognizer->SetRanges(iPermittedRanges);
            }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------
// Set Character range popup
// ---------------------------------------------------------
//
void CPeninputHwrBoxDataMgr::SetCharacterRangePopupL(TInt aPermittedRanges)
    {
    // Read resource
    TResourceReader reader;
    TInt index = 0;
    iCharacterRangeArray->Reset();
    iCharacterRangeIdArray.Reset();
    iCurrentCharacterRangeIndex = 0;
    if ((aPermittedRanges & ERangeHiraganaOnly) == aPermittedRanges)
        {
        CCoeEnv::Static()->CreateResourceReaderLC( reader, R_PENINPUT_HWR_CHARACTER_HIRAGANA );
        TPtrC p2 = reader.ReadTPtrC();
        iCharacterRangeArray->AppendL(p2);
        CleanupStack::PopAndDestroy( 1 );
        iCharacterRangeIdArray.At(0) = EPeninutCharacterRangeIdKanaKanji;  // not used
        return;
        }
    if ((aPermittedRanges & ERangeHiraganaKanji)
        && (aPermittedRanges & (ERangeHiraganaKanji | ERangeKatakana | ERangeFullWidthKatakana)) == aPermittedRanges)
        {
        CCoeEnv::Static()->CreateResourceReaderLC( reader, R_PENINPUT_HWR_CHARACTER_KANAKANJI );
        TPtrC p2 = reader.ReadTPtrC();
        iCharacterRangeArray->AppendL(p2);
        CleanupStack::PopAndDestroy( 1 );
        iCharacterRangeIdArray.At(0) = EPeninutCharacterRangeIdKanaKanji;
        return;
        }
    if ((aPermittedRanges & (ERangeEnglish | ERangeFullWidthEnglish)) == aPermittedRanges)
        {
        CCoeEnv::Static()->CreateResourceReaderLC( reader, R_PENINPUT_HWR_CHARACTER_LATIN );
        TPtrC p3 = reader.ReadTPtrC();
        iCharacterRangeArray->AppendL(p3);
        CleanupStack::PopAndDestroy( 1 );
        iCharacterRangeIdArray.At(0) = EPeninutCharacterRangeIdLatin;
        return;
        }
    if ((aPermittedRanges & (ERangeNumber | ERangeFullWidthNumeric)) == aPermittedRanges)
        {
        CCoeEnv::Static()->CreateResourceReaderLC( reader, R_PENINPUT_HWR_CHARACTER_NUMBER );
        TPtrC p4 = reader.ReadTPtrC();
        iCharacterRangeArray->AppendL(p4);
        CleanupStack::PopAndDestroy( 1 );
        iCharacterRangeIdArray.At(0) = EPeninutCharacterRangeIdNumber;
        return;
        }
    if ((aPermittedRanges & (ERangeKatakana | ERangeFullWidthKatakana)) == aPermittedRanges)
        {
        CCoeEnv::Static()->CreateResourceReaderLC( reader, R_PENINPUT_HWR_CHARACTER_KATAKANA );
        TPtrC p4 = reader.ReadTPtrC();
        iCharacterRangeArray->AppendL(p4);
        CleanupStack::PopAndDestroy( 1 );
        iCharacterRangeIdArray.At(0) = EPeninutCharacterRangeIdKanaKanji;  // not used
        return;
        }
    CCoeEnv::Static()->CreateResourceReaderLC( reader, R_PENINPUT_HWR_CHARACTER_ALL );
    TPtrC p1 = reader.ReadTPtrC();
    iCharacterRangeArray->AppendL(p1);
    CleanupStack::PopAndDestroy( 1 );
    iCharacterRangeIdArray.At(index++) = EPeninutCharacterRangeIdAll;
    if (aPermittedRanges & (ERangeHiraganaKanji | ERangeKatakana | ERangeFullWidthKatakana))
        {
        CCoeEnv::Static()->CreateResourceReaderLC( reader, R_PENINPUT_HWR_CHARACTER_KANAKANJI );
        TPtrC p2 = reader.ReadTPtrC();
        iCharacterRangeArray->AppendL(p2);
        CleanupStack::PopAndDestroy( 1 );
        iCharacterRangeIdArray.At(index++) = EPeninutCharacterRangeIdKanaKanji;
        }
    if (aPermittedRanges & (ERangeEnglish | ERangeFullWidthEnglish))
        {
        CCoeEnv::Static()->CreateResourceReaderLC( reader, R_PENINPUT_HWR_CHARACTER_LATIN );
        TPtrC p3 = reader.ReadTPtrC();
        iCharacterRangeArray->AppendL(p3);
        CleanupStack::PopAndDestroy( 1 );
        iCharacterRangeIdArray.At(index++) = EPeninutCharacterRangeIdLatin;
        }
    if (aPermittedRanges & (ERangeNumber | ERangeFullWidthNumeric))
        {
        CCoeEnv::Static()->CreateResourceReaderLC( reader, R_PENINPUT_HWR_CHARACTER_NUMBER );
        TPtrC p4 = reader.ReadTPtrC();
        iCharacterRangeArray->AppendL(p4);
        CleanupStack::PopAndDestroy( 1 );
        iCharacterRangeIdArray.At(index++) = EPeninutCharacterRangeIdNumber;
        }
    }

