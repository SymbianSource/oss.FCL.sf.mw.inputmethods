/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of peninput handwriting recognition
*
*/


#include <AknFepGlobalEnums.h>  // ELowerCase
#include <aknfeppeninputenums.h>    // ERangeHiraganaKanji, ...
#include <PtiHwrRecognizer.h>
#include <PtiEngine.h>

#include "peninputhwrboxrecognizer.h"

// constant definition
const TInt KInvalidStrokeEndMarkX = -65536;
const TInt KInvalidStrokeEndMarkY = -65536;

// ---------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------
//
CPeninputHwrBoxRecognizer* CPeninputHwrBoxRecognizer::NewL(CPtiEngine* aPtiEngine)
    {
    CPeninputHwrBoxRecognizer* self = new (ELeave) CPeninputHwrBoxRecognizer();

    CleanupStack::PushL(self);
    self->ConstructL(aPtiEngine);
    CleanupStack::Pop(self);//self

    return self;
    }

// ---------------------------------------------------------
// destructor
// ---------------------------------------------------------
//
CPeninputHwrBoxRecognizer::~CPeninputHwrBoxRecognizer()
    {
    }

// ---------------------------------------------------------
// Do recoginize by engine
// ---------------------------------------------------------
//
void CPeninputHwrBoxRecognizer::DoRecognize(const RArray<TPoint>& aTraceData, RPointerArray<HBufC>& aCandidates, RArray<TInt>& aRangeStartPos)
    {
    if (iRecognizer != NULL)
        {
        aCandidates.ResetAndDestroy();

        TInt primaryCount = iRecognizer->Recognize(aTraceData, aCandidates);

        // filter recognized candidate, remove unnecessary gesture when only number is permitted

        // filter recognized candidate, set start position for all ranges
        aRangeStartPos.Reset();

        _LIT(KSeparator,"\0");
        TPtrC ptr(KSeparator);

        if (primaryCount > 0)
            {
            // the start position of primary range is 0
            aRangeStartPos.Append(0);

            // remove uncessary primary candidate
            TInt removePos = iTotalCandidateNum - (iRangeCount - 1);
            TInt removeCount = primaryCount - removePos;

            if (removePos >= 0)
                {
                for (TInt i = 0; i < removeCount; i++)
                    {
                    delete aCandidates[removePos];
                    aCandidates.Remove(removePos);
                    }
                }

            // remove range separator
            for (TInt i=0; i<aCandidates.Count(); i++)
                {
                if (aCandidates[i]->CompareC(ptr) == 0)
                    {
                    // remove separator from candidate list
                    delete aCandidates[i];
                    aCandidates.Remove(i);
                    // the start position of next range is i
                    aRangeStartPos.Append(i);
                    }
                }
            }
        }
    }


// ---------------------------------------------------------
// Set primary and auxiliary ranges for hwr engine
// ---------------------------------------------------------
//
void CPeninputHwrBoxRecognizer::SetRanges(const RArray<TInt>& aPermittedRanges)
    {
    if ((aPermittedRanges.Count() > 0) && (iRecognizer != NULL))
        {
        iPremaryRange = aPermittedRanges[0];
        iRangeCount = aPermittedRanges.Count();

        TRecognitionRange range;

        SetRecognitionRange(aPermittedRanges[0], range);
        iRecognizer->SetRange(range);

        // set auxiliary ranges for hwr engine
        for (TInt i=1; i<aPermittedRanges.Count(); i++)
            {
            SetRecognitionRange(aPermittedRanges[i], range);

            iRecognizer->AddAuxiliaryRange(range);
            }

        SetCase(iCase);
        }
    }

// ---------------------------------------------------------
// Set case
// ---------------------------------------------------------
//
void CPeninputHwrBoxRecognizer::SetCase(TInt aCase)
    {
    // set letter to lower first when LowerCase
    // set letter to upper first when UpperCase and TextCase
    if (iRecognizer != NULL)
        {
        if (ECaseLower == aCase || ECaseInverseText == aCase)
            {
            iRecognizer->SetFirstLetterOrder(ELowerFirst);
            }
        else if (ECaseUpper == aCase || ECaseText == aCase)
            {
            iRecognizer->SetFirstLetterOrder(EUpperFirst);
            }

        iCase = aCase;
        }
    }

// ---------------------------------------------------------
// Set number mode for hwr engine
// ---------------------------------------------------------
//
void CPeninputHwrBoxRecognizer::SetNumberMode(const TAknEditorNumericKeymap& aNumberMode)
    {
    if (iRecognizer != NULL)
        {
        iRecognizer->SetNumberMode(aNumberMode);
        }
    }

// ---------------------------------------------------------
// Get stroke end mark from hwr engine
// ---------------------------------------------------------
//
TPoint CPeninputHwrBoxRecognizer::StrokeEndMark() const
    {
    if (iRecognizer != NULL)
        {
        return iRecognizer->StrokeEndMark();
        }
    else
        {
        return TPoint(KInvalidStrokeEndMarkX, KInvalidStrokeEndMarkY);
        }
    }

// ---------------------------------------------------------
// Set primary candidate num for hwr engine
// ---------------------------------------------------------
//
TInt CPeninputHwrBoxRecognizer::SetPrimaryCandidateNum(TInt aNum)
    {
    if (iRecognizer != NULL)
        {
        return iRecognizer->SetCandidateNum(aNum);
        }
    else
        {
        return KErrGeneral;
        }
    }

// ---------------------------------------------------------
// Set total candidate num that should be shown
// ---------------------------------------------------------
//
TInt CPeninputHwrBoxRecognizer::SetCandidateNum(TInt aNum)
    {
    if (aNum > 0)
        {
        iTotalCandidateNum = aNum;
        return KErrNone;
        }
    else
        {
        return KErrGeneral;
        }
    }

// ---------------------------------------------------------
// Set language
// ---------------------------------------------------------
//
void CPeninputHwrBoxRecognizer::SetLanguageL(TInt aLanguage)
    {

    iLanguage = aLanguage;

    // select suitable hwr recognizer according to language
    iRecognizer = iPtiEngine->GetHwrRecognizerL(TLanguage(iLanguage));
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxRecognizer::SetInputAreaSize
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBoxRecognizer::SetInputAreaSize( TSize aSize )
    {
    if ( iRecognizer )
        {
        iRecognizer->SetInputAreaSize( aSize );
        }
    }

// ---------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------
//
CPeninputHwrBoxRecognizer::CPeninputHwrBoxRecognizer()
    {
    }

// ---------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------
//
void CPeninputHwrBoxRecognizer::ConstructL(CPtiEngine* aPtiEngine)
    {
    if (!aPtiEngine)
        {
        User::Leave(KErrArgument);
        }
    iPtiEngine = aPtiEngine;
    }

// ---------------------------------------------------------
// Set recognition range for hwr engine
// ---------------------------------------------------------
//
void CPeninputHwrBoxRecognizer::SetRecognitionRange(const TInt aRange, TRecognitionRange& aRecognitionRange) const
    {
    aRecognitionRange.iLanguage = TLanguage(iLanguage);

    switch (aRange)
        {
        case ERangeNative:
        case ERangeHiraganaKanji:
            aRecognitionRange.iSubRange = EPtiRangeHiraganaKanji;
            break;
        case ERangeKatakana:
            aRecognitionRange.iSubRange = EPtiRangeKatakana;
            break;
        case ERangeFullWidthKatakana:
            aRecognitionRange.iSubRange = EPtiRangeFullWidthKatakana;
            break;
        case ERangeFullWidthEnglish:
            aRecognitionRange.iSubRange = EPtiRangeFullWidthEnglish;
            break;
        case ERangeFullWidthNumeric:
            aRecognitionRange.iSubRange = EPtiRangeFullWidthNumeric;
            break;
        case ERangeHiraganaOnly:
            aRecognitionRange.iSubRange = EPtiRangeHiraganaOnly;
            break;
        case ERangeEnglish:
            aRecognitionRange.iSubRange = EPtiRangeLatin;
            break;
        case ERangeNumber:
            aRecognitionRange.iSubRange = EPtiRangeNumber;
            break;
        case ERangeSymbol:
            if (iPremaryRange == ERangeEnglish)
                {
                aRecognitionRange.iLanguage = ELangEnglish;
                }
            else
                {
                aRecognitionRange.iLanguage = TLanguage(iLanguage);
                }
            aRecognitionRange.iSubRange = EPtiRangeSymbol;
            break;
        default:
            break;
        }
    }

//End Of File
