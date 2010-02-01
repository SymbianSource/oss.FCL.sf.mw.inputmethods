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
* Description:  Implementation of HWR engine
*
*/


#include <AknFepGlobalEnums.h>
#include <aknfeppeninputenums.h>
#include <PtiHwrRecognizer.h>

#include "peninputhwrengine.h"
#include "peninputhwrbxdatastore.h"

// constant definition
const TInt KInvalidStrokeEndMarkX = -65536;
const TInt KInvalidStrokeEndMarkY = -65536;

//const TInt KMaxPredictiveCount = 50;
const TInt K1PagePredictiveCount = 36;
const TUint16 KGestureSpace = 0xF800;
const TUint16 KGestureEnter = 0xF801;
const TUint16 KSegment = 0x0020;
const TUint16 KGestureBackspace = 0x2408;
const TUint16 KDisplayBackspace = 0x2190;
const TInt KPremaryCandidateCount = 13;
const TInt KAuxCandidateCount = 1;

_LIT(KNumberSet,"0123456789");
_LIT(KSeparator,"\0");
_LIT(KDash,"-");
_LIT(KSolidus,"/");

// ---------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------
//
CAknFepHwrEngine* CAknFepHwrEngine::NewL(CPtiEngine* aPtiEngine, CAknFepHwrBxDataStore* aOwner)
    {
    CAknFepHwrEngine* self = new (ELeave) CAknFepHwrEngine();
    
    CleanupStack::PushL(self);
    self->ConstructL(aPtiEngine, aOwner);
    CleanupStack::Pop(self);//self

    return self;
    }

// ---------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------
//
CAknFepHwrEngine::CAknFepHwrEngine():iRecognizerInited(EFalse),
                                     iNeedSetNumberMode(EFalse),
                                     iNeedSetCase(EFalse),
                                     iNeedSetRange(EFalse)
    {
    }

// ---------------------------------------------------------
// destructor
// ---------------------------------------------------------
//
CAknFepHwrEngine::~CAknFepHwrEngine()
    {
    if(iOwnPtiEngine)
        {
        delete iPtiEngine;
        }
    delete iCustomKeymap;
    
    delete iIdle;
    
    iPermittedRanges.Reset();
    iPermittedRanges.Close();
    }

// ---------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------
//
void CAknFepHwrEngine::ConstructL(CPtiEngine* aPtiEngine, CAknFepHwrBxDataStore* aOwner)
    {
    iPtiEngine = aPtiEngine;

    if( !iPtiEngine )
        {
        iPtiEngine = CPtiEngine::NewL(ETrue);
        iOwnPtiEngine = ETrue;
        }

    iIdle = CIdle::NewL(CActive::EPriorityIdle);
    iOwner = aOwner;
    }

// ---------------------------------------------------------
// Convert stroke end mark
// ---------------------------------------------------------
//
void CAknFepHwrEngine::ConvertStrokeEndMark(RArray<TPoint>& aTraceData, TPoint aPnt1, TPoint aPnt2)
    {
    TInt count = aTraceData.Count();

    for (TInt i = 0; i < count; i++)
        {
    	if (aTraceData[i] == aPnt1)
    	    {
    		aTraceData.Remove(i);
    		aTraceData.Insert(aPnt2, i);
    	    }
        }
    }

// ---------------------------------------------------------
// Do recoginize by engine
// ---------------------------------------------------------
//
void CAknFepHwrEngine::DoRecognizeL(const RArray<TPoint>& aTraceData,
                                    RPointerArray<HBufC>& aCandidates)
    {
    if (!iRecognizer)
        {
        return;
        }
        
    TPoint ctrlStrokeEndMark = iOwner->StrokeEndMarkFromControl();
    
    if (!iRecognizerInited)
        {
        DoIdleConstructL();
        }
        
    if (ctrlStrokeEndMark != iRecognizer->StrokeEndMark())
        {
        ConvertStrokeEndMark(CONST_CAST(RArray<TPoint>&, aTraceData), 
                             iOwner->StrokeEndMarkFromControl(), 
                             iRecognizer->StrokeEndMark());
        iOwner->SetStrokeEndMark(); // make control's stroke end mark info same to engine
        }
    
    aCandidates.ResetAndDestroy();
    
    TInt primaryCount;  
    TBuf<1> bufBackspace;
    bufBackspace.Append(KGestureBackspace);
    TBuf<1> bufDisplayBackspace;
    bufDisplayBackspace.Append(KDisplayBackspace);
        
    if (iOwner->IsNumberOnly())
        {     
        if ( iNumberMode == EAknEditorPlainNumberModeKeymap )
            {
            TBuf<1> bufEnter;
            bufEnter.Append( KGestureEnter );
            TBuf<1> bufSpace;
            bufSpace.Append( KGestureSpace );
            
            TBuf<13> buf;
            buf.Append( KNumberSet() );
            buf.Append( bufBackspace );
            buf.Append( bufEnter );
            buf.Append( bufSpace );
            primaryCount = iRecognizer->RecognizeWithCharSet(
                aTraceData, aCandidates, buf );
            }
        else
            {
            primaryCount = iRecognizer->Recognize(aTraceData, aCandidates);
            }
        
        for ( TInt i = aCandidates.Count() - 1; i >= 0; i-- )
            {
            if (aCandidates[i]->Compare(bufBackspace) == 0)
            	{
                *aCandidates[i] = bufDisplayBackspace;
            	}
            }
        
        if (iCustomKeymap)
            {
            for(TInt i=0; i<aCandidates.Count(); i++)
                {
                if( iCustomKeymap->Find(*aCandidates[i]) == KErrNotFound )
                    {
                    delete aCandidates[i];
    		    	aCandidates.Remove(i);
    		    	primaryCount--;
    		    	i--;
                    }
                }
            }
                	
        for (TInt i = 0; i < primaryCount-iTotalCandidateNum; i++)
            {
            delete aCandidates[iTotalCandidateNum];
            aCandidates.Remove(iTotalCandidateNum);
            }
        return;
        }

    primaryCount = iRecognizer->Recognize(aTraceData, aCandidates); 
    
    // remove uncessary primary candidate
    TInt removePos = iTotalCandidateNum - (iRangeCount - 1);        
    if (removePos >= 0)
        {
        for (TInt i = 0; i < primaryCount - removePos; i++)
            {
            delete aCandidates[removePos];
            aCandidates.Remove(removePos);
            }
        }     

    // filter recognized candidate, set start position for all ranges    
    TPtrC ptr(KSeparator);
    
    // remove uncessary aux candidate, including range separator 
    for (TInt i=0; i<aCandidates.Count(); i++)
        {
        if (aCandidates[i]->Compare(bufBackspace) == 0)
        	{
        	// convert backspace returned by engine, to make sure it display correctly.
            *aCandidates[i] = bufDisplayBackspace;
        	}
        
        else if ( (aCandidates[i]->CompareC(ptr) == 0) )
            {
            // remove separator from candidate list
            delete aCandidates[i];
            aCandidates.Remove(i);
            }
        }
    
    TPtrC dashPtr(KDash);
    TPtrC solidusPtr(KSolidus);
    
    // not allowing '-' to be the first char in chinese range 
    // not allowing '/' to be the first char in English and Number range 
    if ( ( iPremaryRange == ERangeNative ) && (aCandidates[0]->Compare(dashPtr) == 0) )
        {
        *aCandidates[0] = *aCandidates[1];
        *aCandidates[1] = dashPtr;
        }
    else if ( ( (iPremaryRange == ERangeEnglish)||(iPremaryRange == ERangeNumber) ) 
        && (aCandidates[0]->Compare(solidusPtr) == 0) )
        {
        *aCandidates[0] = *aCandidates[1];
        *aCandidates[1] = solidusPtr;
        }
    }

// ---------------------------------------------------------
// Judge if candidate exist in given position
// ---------------------------------------------------------
//
TBool CAknFepHwrEngine::CandidateExisted()
    {
	return iPtiEngine->MoreCandidatePages();
    }

// ---------------------------------------------------------
// Do predictive using trigger string
// ---------------------------------------------------------
//
void CAknFepHwrEngine::DoPredictiveL(const TDesC& aTriggerStr, 
                                     RPointerArray<HBufC>& aPredictives,
                                     TBool aNextPage)
    {
    // predictive only valid for Chinese
    if (((iLanguage == ELangPrcChinese) || 
        (iLanguage == ELangTaiwanChinese) || 
        (iLanguage == ELangHongKongChinese)) &&
        (iPtiEngine))
        {
        // activate correct pti language according to given language
        
        if (!aNextPage)
            {
        	iPtiEngine->SetCandidatePageLength(K1PagePredictiveCount);
        	aPredictives.ResetAndDestroy();
        	
        	if(!iPtiEngine->SetPredictiveChineseChar(aTriggerStr))
        	    {
        	    return;
        	    }
            }
        else
            {
            if (!iPtiEngine->NextCandidatePage())
                {
            	return;
                }
            }
            
        TPtrC ptr = iPtiEngine->CandidatePage();    
        
        if ((iPtiEngine->InputMode() == EPtiEnginePinyinByPhrase) || 
            (iPtiEngine->InputMode() == EPtiEngineStrokeByPhrase) ||
            (iPtiEngine->InputMode() == EPtiEngineZhuyinByPhrase))
        	{
        	// phrase input mode
            TInt start = 0;
            TInt length = 0;
        	
            for (TInt i = 0; i <ptr.Length(); i++)
                {
                if (ptr[i] == KSegment)
                    {
                    TPtrC segment(ptr.Ptr() + start, length);
                    TRAP_IGNORE(aPredictives.AppendL(segment.AllocL()));
                    start += (length + 1);
                    length = 0;
                    }
                else
                    {
                    length++;               
                    }
                }

            if (length)
                {
                TPtrC segm(ptr.Ptr() + start, length);
                TRAP_IGNORE(aPredictives.AppendL(segm.AllocL()));
                }
        	}
        else
            {
            TInt predictiveCandidateNum = ptr.Length();

            for (TInt i=0; i<predictiveCandidateNum; i++)
                {
                aPredictives.Append(ptr.Mid(i,1).AllocL());
                }
            }	
        }
    }

// ---------------------------------------------------------
// Set primary and auxiliary ranges for hwr engine
// ---------------------------------------------------------
//
void CAknFepHwrEngine::SetRanges(const RArray<TInt>& aPermittedRanges)
    {
    if (!iRecognizerInited)
        {
    	iNeedSetRange = ETrue;
    	iPermittedRanges.Reset();
    	
    	for (TInt i = 0; i < aPermittedRanges.Count(); i++)
    	    {
            iPermittedRanges.Append(aPermittedRanges[i]);
    	    }
    	
    	return;    
        }

    if ((aPermittedRanges.Count() > 0) && (iRecognizer))
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
void CAknFepHwrEngine::SetCase(TInt aCase)
    {
    if (!iRecognizerInited)
        {
    	iNeedSetCase = ETrue;
    	iCase = aCase;
    	return;
        }
    
    // set letter to lower first when LowerCase
    // set letter to upper first when UpperCase and TextCase
    if (iRecognizer)
        {
        if (aCase == ELowerCase)
            {
            iRecognizer->SetFirstLetterOrder(ELowerFirst);
            }
        else
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
void CAknFepHwrEngine::SetNumberMode(const TAknEditorNumericKeymap& aNumberMode)
    {
    iNumberMode = aNumberMode;
    
    if (!iRecognizerInited)
        {
        iNeedSetNumberMode = ETrue;
    	
    	return;
        }

    if (iRecognizer)
        {
        iRecognizer->SetNumberMode(aNumberMode);
        if( aNumberMode !=  EKeymapFromResource)
            {
            ResetCustomKeyMap();
            }
        }
    }

// ---------------------------------------------------------
// Get stroke end mark from hwr engine
// ---------------------------------------------------------
//
TPoint CAknFepHwrEngine::StrokeEndMark() const
    {
    if (iRecognizer)
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
TInt CAknFepHwrEngine::SetPrimaryCandidateNum(TInt aNum)
    {
    if (iRecognizer)
        {
        return iRecognizer->SetCandidateNum(aNum);
        }
    else
        {
        return KErrGeneral;
        }
    }

// ---------------------------------------------------------
// Set aux candidate num that should be shown
// ---------------------------------------------------------
//
TInt CAknFepHwrEngine::SetAuxCandidateNum(TInt aNum)
    {
    if (iRecognizer)
        {
        return iRecognizer->SetAuxCandidateNum(aNum);
        }
    else
        {
        return KErrGeneral;
        }
    }


// ---------------------------------------------------------
// Get aux candidate num that should be shown
// ---------------------------------------------------------
//
TInt CAknFepHwrEngine::GetAuxCandidateNum()
    {
    if (iRecognizer)
        {
        return iRecognizer->GetAuxCandidateNum();
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
TInt CAknFepHwrEngine::SetCandidateNum(TInt aNum)
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
void CAknFepHwrEngine::SetLanguageL(TInt aLanguage)
    {
    if ( (iLanguage == aLanguage) ||
         (aLanguage != ELangPrcChinese && 
          aLanguage != ELangHongKongChinese && 
          aLanguage != ELangTaiwanChinese) )
        {
        return;
        }
        
    iLanguage = aLanguage;
  switch (iLanguage)
    {
    case ELangPrcChinese:
        {
        if (iPtiEngine->ActivateLanguageL(iLanguage, EPtiEnginePinyinByPhrase) != KErrNone)
        	{
        	iPtiEngine->ActivateLanguageL(iLanguage, EPtiEnginePinyin);
        	}
        }
        break;
    case ELangHongKongChinese:
        if (iPtiEngine->ActivateLanguageL(iLanguage, EPtiEngineStrokeByPhrase) != KErrNone)
        	{
        	iPtiEngine->ActivateLanguageL(ELangHongKongChinese, EPtiEngineStroke);
        	}
        break;
    case ELangTaiwanChinese:
        if (iPtiEngine->ActivateLanguageL(iLanguage, EPtiEngineZhuyinByPhrase) != KErrNone)
        	{
        	iPtiEngine->ActivateLanguageL(ELangTaiwanChinese, EPtiEngineZhuyin);
        	}
        break;
    default:
        return;
    }

    iRecognizerInited = EFalse;
    
    iIdle ->Start(TCallBack(BackgroundTaskL,this));
    }
    
// ---------------------------------------------------------------------------
// CAknFepHwrEngine::DoIdleConstructL
// Do background construct.
// ---------------------------------------------------------------------------
//
void CAknFepHwrEngine::DoIdleConstructL()
    {
    if (iRecognizerInited)
        {
        return;
        }

	iRecognizer = iPtiEngine->GetHwrRecognizerL(TLanguage(iLanguage));
	iRecognizerInited = ETrue;
	iOwner->SetStrokeEndMark();
	SetPrimaryCandidateNum(KPremaryCandidateCount);
	SetAuxCandidateNum(KAuxCandidateCount);
	
    if (iNeedSetRange)
        {
        SetRanges(iPermittedRanges);
        iNeedSetRange = EFalse;
        iPermittedRanges.Reset();
        }
    
    if (iNeedSetCase)
        {
    	SetCase(iCase);
    	iNeedSetCase = EFalse;
        }
    
    if (iNeedSetNumberMode)
        {
    	SetNumberMode(TAknEditorNumericKeymap(iNumberMode));
    	iNeedSetNumberMode = EFalse;
        }
    }

// ---------------------------------------------------------------------------
// CAknFepHwrEngine::BackgroundConstructL
// Do background construct.
// ---------------------------------------------------------------------------
//
TInt CAknFepHwrEngine::BackgroundTaskL(TAny* aPtr)
    {
    CAknFepHwrEngine* self = static_cast<CAknFepHwrEngine*>(aPtr);
    self->DoIdleConstructL();
    return EFalse;
    }
    
// ---------------------------------------------------------
// Set recognition range for hwr engine
// ---------------------------------------------------------
//
void CAknFepHwrEngine::SetRecognitionRange(const TInt aRange, TRecognitionRange& aRecognitionRange)
    {
    aRecognitionRange.iLanguage = TLanguage(iLanguage);

    switch (aRange)
        {
        case ERangeNative:

            if (iLanguage == ELangPrcChinese)
                {
                aRecognitionRange.iSubRange = EPtiRangePRCChinese;
                }
            else
                {
                if (iLanguage == ELangHongKongChinese)
                    {
                    aRecognitionRange.iSubRange = EPtiRangeHKChinese;
                    }
                else
                    {
                    aRecognitionRange.iSubRange = EPtiRangeTWChinese;
                    }
                }

            break;
        case ERangeEnglish:
            aRecognitionRange.iLanguage = ELangEnglish;
            aRecognitionRange.iSubRange = EPtiRangeLatin;

            break;
        case ERangeNumber:
            aRecognitionRange.iSubRange = EPtiRangeNumber;

            break;
        case ERangeSymbol:
            // The symbol mode is associated with iLanguage instead of iPremaryRange
            aRecognitionRange.iLanguage = TLanguage(iLanguage);
            aRecognitionRange.iSubRange = EPtiRangeSymbol;

            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------
// Set recognition range for hwr engine
// ---------------------------------------------------------
//
void CAknFepHwrEngine::SetCustomKeymapL(const TDesC& aKeyMap)
    {
    delete iCustomKeymap;
    iCustomKeymap = NULL;
    iCustomKeymap = HBufC::NewL(aKeyMap.Length() + KNumberSet().Length());
    iCustomKeymap->Des().Copy(KNumberSet());
    iCustomKeymap->Des().Append(aKeyMap);
    }

// ---------------------------------------------------------
// Set recognition range for hwr engine
// ---------------------------------------------------------
//
void CAknFepHwrEngine::ResetCustomKeyMap()
    {
    delete iCustomKeymap;
    iCustomKeymap = NULL;
    }
TInt CAknFepHwrEngine::SetInputAreaSize(TSize& aSize)
    {
    if(iRecognizer)
        {
        return iRecognizer->SetInputAreaSize(aSize);        
        }
    else
        {
        return KErrNotFound;
        }        
    }
TInt CAknFepHwrEngine::SetScreenSize(TSize& aSize)    
    {
      if(iRecognizer)
        {
        return iRecognizer->SetScreenSize(aSize);        
        }
    else
        {
        return KErrNotFound;
        }  
    }
    
//End Of File
