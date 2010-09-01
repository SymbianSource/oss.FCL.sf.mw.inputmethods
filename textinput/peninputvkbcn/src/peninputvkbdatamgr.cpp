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
* Description:  Implementation of the vkb data manager
*
*/


// system includes
#include <aknfeppeninputenums.h>
#include <PtiEngine.h>
#include <centralrepository.h>
#include <AknFepInternalCRKeys.h>
#include <settingsinternalcrkeys.h>
#include <peninputvkbcn.rsg>
#include <AknLayoutDef.h>
#include <AknUtils.h>
#include <aknlayoutscalable_avkon.cdl.h>

// user includes
#include "peninputvkb.hrh"
#include "peninputvkbdatamgr.h"
#include "peninputvkbdataprovider.h"
#include "peninputvkbevent.h"
#include "peninputvkblayoutcontext.h"
#include "peninputvkblayout.h"
#include "peninputvkbcnlafdatamgr.h"
const TUint16 KSegment = 0x0020;
const TInt K1PageCandCount = 36;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::CAknFepVkbDataMgr
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//      
CAknFepVkbDataMgr::CAknFepVkbDataMgr(MAknFepVkbLayoutContext* aContext)
    :iInputLanguage(ELangPrcChinese),
     iImLayout(EAknFepVkbImLatin),
     iContext(aContext),
     iWindowPosition(0),
     iSizeChanging(EFalse),
     iUnitShadowSizeRead(EFalse)
    {
    iDefaultImLayout4HK = EAknFepVkbImCnStroke;
    iDefaultImLayout4PRC = EAknFepVkbImCnPinyin;
    iDefaultImLayout4TW = EAknFepVkbImCnZhuyin;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::ConstructL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::ConstructL(CPtiEngine* aPtiEngine) 
    {
    /*
    TResourceReader reader;
    //Read imlayout from resource
    CCoeEnv::Static()->CreateResourceReaderLC(reader, R_AKN_FEP_VKB_LANG_STRING_LIST);
    
    const TInt count = reader.ReadInt16();
    
    for (TInt i=0; i < count; i++)
        {
        SLanguageItem* item = new SLanguageItem();
        item->iLangId = reader.ReadInt16();
        item->iDisplayText = reader.ReadTPtrC();
        iLangStringList.Append(item);
        }
    CleanupStack::PopAndDestroy(); // reader
    */
    TInt ret = 0;
    
    TRAP(ret, iFepRepository = CRepository::NewL(KCRUidAknFep));
    TRAP(ret, iGSRepository = CRepository::NewL(KCRUidPersonalizationSettings));
    
    if (ret == KErrNone)
        {
        iGSRepositoryWatcher = CAknFepRepositoryWatcher::NewL(
            KCRUidPersonalizationSettings,
            TCallBack(HandleGSRepositoryCallBack, this),
            iGSRepository);
        }
    
    // Read peninput language from CenRep
    if (iGSRepository)
        {
        iGSRepository->Get(KSettingsPenInputLang, iInputLanguage);
        }
    
    // Read default imlayout of current peninput language from CenRep
    DefaultModeChangeNotification();
    // Create engine
    iPtiEngine = aPtiEngine;
    if( !iPtiEngine )
        {
        iPtiEngine = CPtiEngine::NewL(ETrue);
        iOwnPtiEngine = ETrue;
        }
    } 
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::NewL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
CAknFepVkbDataMgr* CAknFepVkbDataMgr::NewL(MAknFepVkbLayoutContext* aContext, 
                                           CPtiEngine* aPtiEngine)
    {
    CAknFepVkbDataMgr* self = NewLC(aContext, aPtiEngine);
    CleanupStack::Pop();
    return self;
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::NewLC
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
CAknFepVkbDataMgr* CAknFepVkbDataMgr::NewLC(MAknFepVkbLayoutContext* aContext, 
                                            CPtiEngine* aPtiEngine)
    {
    CAknFepVkbDataMgr* self = new(ELeave) CAknFepVkbDataMgr(aContext);
    CleanupStack::PushL(self);
    self->ConstructL(aPtiEngine); 
    return self;        
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::~CAknFepVkbDataMgr
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
CAknFepVkbDataMgr::~CAknFepVkbDataMgr()
    {
    //iLangStringList.ResetAndDestroy();
    //iLangStringList.Close();
    
    iCandidates.ResetAndDestroy();
    iCandidates.Close();
    
    // Cen Rep  
    delete iFepRepositoryWatcher;    
    delete iGSRepositoryWatcher;
    delete iFepRepository;    
    delete iGSRepository;
    delete iUserDefinedResource;
    // Delete engine        
    if( iOwnPtiEngine )
        {
        delete iPtiEngine;
        } 
    
    iUnits.Close();    
    }
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::GetCandidatesL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TInt CAknFepVkbDataMgr::GetCandidatesL(const TDesC& aInputString, 
                                       const TBool aIsPredictive,
                                       TBool aForNextPage)
    {
    if (aForNextPage)
        {
        if ( iCandidatesCount > KMaxPredictCandCnt )
            {
            return iCandidatesCount;
            }
            
        if ((iPtiEngine) && (iPtiEngine->NextCandidatePage()))
    	    {
    	    TPtrC ptr = iPtiEngine->CandidatePage();

            TInt start = 0;
            TInt length = 0;

            for (TInt i = 0; i <ptr.Length(); i++)
                {
                if (ptr[i] == KSegment)
                    {
                    TPtrC segment(ptr.Ptr() + start, length);
                    TRAP_IGNORE(iCandidates.AppendL(segment.AllocL()));
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
                TRAP_IGNORE(iCandidates.AppendL(segm.AllocL()));
                }
               
            if ( iCandidates.Count() >= KMaxPredictCandCnt )
                {
                for (TInt i = KMaxPredictCandCnt; i < iCandidates.Count(); i++)
                    {
                    delete iCandidates[KMaxPredictCandCnt];
                    iCandidates.Remove(KMaxPredictCandCnt);
                    }
                }
        
            iContext->SendEventToVkbControl(EVkbEventGetNextPageCandidate,(
                                            CFepUiBaseCtrl*)&iCandidates);
            
            iCandidatesCount = iCandidates.Count();

            return iCandidatesCount;
    	    }
    	
    	return KErrNotSupported;    
        }

    if (aIsPredictive)
        {
        if( iPredictiveBuf.Length() + aInputString.Length() > KMaxLengthPredictiveString )
            {
            iPredictiveBuf.Zero();
            }
            
        if ( ( aInputString.Length() == 1) 
             && (aInputString[0] == EKeyBackspace)  )
            {
            if ( iPredictiveBuf.Length() >= 1 )
                {
                iPredictiveBuf.Delete(iPredictiveBuf.Length()-1,1);
                }
            }
        else
            {
            iPredictiveBuf.Append(aInputString);            
            }           
        }
    else
        {
        iPredictiveBuf.Zero();  
        }
    
    const HBufC* formatString = NULL;
    
    if (aIsPredictive && iPredictiveBuf.Length() == 0)
        {
        iCandidates.ResetAndDestroy();
        }
    else
        {
        iPtiEngine->SetCandidatePageLength(K1PageCandCount);

        if(aIsPredictive)
            {
            DoPredictive(iPredictiveBuf, iCandidates);
            if(iCandidates.Count() == 0)
                {
                iPredictiveBuf.Delete(0, iPredictiveBuf.Length() - 1);
                DoPredictive(iPredictiveBuf, iCandidates);
                }
            }
        else
            {
            formatString = iPtiEngine->GetCandidatesByInputString(aInputString, 
                                                              iCandidates, 
                                                              aIsPredictive);
            }
        }    
    
    if (!formatString && !aIsPredictive)
        {
        return 0;
        }
        
    if (!aIsPredictive)
        {
        iContext->SendEventToVkbControl(EVkbEventCompFieldAnalysisResponse,NULL,*formatString);
        }      
              
    iContext->SendEventToVkbControl(EVkbEventCandidatesChanged,(CFepUiBaseCtrl*)&iCandidates);
    
    delete formatString;
    
    iCandidatesCount = iCandidates.Count();
    
    return iCandidatesCount;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::DoPredictive
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::DoPredictive(const TDesC& aTriggerStr, 
                                     RPointerArray<HBufC>& aPredictives)
    {
	aPredictives.ResetAndDestroy();
	
	if(!iPtiEngine->SetPredictiveChineseChar(aTriggerStr))
	    {
	    return;
	    }
    TPtrC ptr = iPtiEngine->CandidatePage();
    
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

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::ClearTriggerString
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::ClearTriggerString()
    {
    iPredictiveBuf.Zero();
    }

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::Reset
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::Reset()
    {
    // iCase = 0;

    // iEditorNumericKeymap = 0;
    
    // iPermittedRange = 0;

    // iImLayout = ERangeChinese;

    iCandidatesCount = 0;
    
    iShiftDown = EFalse;
    
    iCapslockDown = EFalse;
    
    iCandidates.ResetAndDestroy();
    
    }
        
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::Candidates
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
RPointerArray<HBufC> CAknFepVkbDataMgr::Candidates()
    {
    return iCandidates;
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::Case
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TInt CAknFepVkbDataMgr::Case()
    {
    return iCase;
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::InputLanguage
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TInt CAknFepVkbDataMgr::InputLanguage()
    {
    return iInputLanguage;
    } 
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::PermittedRange
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TInt CAknFepVkbDataMgr::PermittedRange()
    {
    return iPermittedRange;
    } 
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::CurrentRange
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TInt CAknFepVkbDataMgr::CurrentRange()
    {
    return iCurrentRange;
    }     
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::CurrentAccent
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TInt CAknFepVkbDataMgr::CurrentAccent()
    {
    return iCurrentAccent;
    }     
        
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::EditorNumericKeymap
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TInt CAknFepVkbDataMgr::EditorNumericKeymap()
    {
    return iEditorNumericKeymap;
    } 
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::IMLayout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TInt CAknFepVkbDataMgr::IMLayout()
    {
    return iImLayout;
    } 

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::ShiftDown
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TBool CAknFepVkbDataMgr::ShiftDown()
    {
    return iShiftDown;
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::CapslockDown
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TBool CAknFepVkbDataMgr::CapslockDown()
    {
    return iCapslockDown;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::LengthWhenButtondownOnBack
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TInt CAknFepVkbDataMgr::LengthWhenButtondownOnBack()
    {
    return iLengthWhenButtondownOnBack;
    }
            
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::CandidatesCount
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TInt CAknFepVkbDataMgr::CandidatesCount()
    {
    return iCandidatesCount;
    } 

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::WindowPosition
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TInt CAknFepVkbDataMgr::WindowPosition()
    {
    return iWindowPosition;
    } 

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::SetWindowPosition
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::SetWindowPosition(TInt aPosition)
    {
    iWindowPosition = aPosition;
    
    //Store it into CenRep
    WriteWindowPostion(iWindowPosition);
    } 
            
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::SetCase
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::SetCase(TInt aCase)
    {
    iCase = aCase;
    } 
    
void CAknFepVkbDataMgr::SetPermittedCase(TInt aPermittedCase)
    {
    iPermittedCase =  aPermittedCase;
    }
    
TInt CAknFepVkbDataMgr::PermittedCase()
    {
    return iPermittedCase;
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::NextCandidateExist
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TBool CAknFepVkbDataMgr::NextCandidateExist()
    {
	if (iPtiEngine)
	    {
		return iPtiEngine->MoreCandidatePages();
	    }
	
	return EFalse;    
    }

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::LanguageIsChinese
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TBool CAknFepVkbDataMgr::LanguageIsChinese(TInt aLang)
    {
	if ((aLang == ELangPrcChinese) || 
	    (aLang == ELangHongKongChinese) ||
	    (aLang == ELangTaiwanChinese))
	    {
		return ETrue;
	    }
	
	return EFalse;    
    }

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::SetEngineLanguage
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::SetEngineLanguage(TInt aLang)
    {
    if (iPtiEngine) 
        {
        switch (aLang)
            {
            case ELangHongKongChinese:
                TRAP_IGNORE(iPtiEngine->ActivateLanguageL(aLang, EPtiEngineStrokeVkb));
                break;
            case ELangTaiwanChinese:
                TRAP_IGNORE(iPtiEngine->ActivateLanguageL(aLang, EPtiEngineZhuyinVkb));
                break;
            case ELangPrcChinese:
                TRAP_IGNORE(iPtiEngine->ActivateLanguageL(aLang, EPtiEnginePinyinVkb));
                break;
            default:
                break;
            }
        }
    }

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::SetInputLanguage
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TInt CAknFepVkbDataMgr::SetInputLanguage(TInt aInputLanguage)
    {
    /*
    if( aInputLanguage != -1 && aInputLanguage != iInputLanguage )
        {            
        iInputLanguage = aInputLanguage;
        
        if( iPtiEngine )
            {
            switch (iInputLanguage)
                {
                case ELangHongKongChinese:
                    TRAP_IGNORE(iPtiEngine->ActivateLanguageL(iInputLanguage, EPtiEngineStrokeVkb));           
                    break;
                case ELangTaiwanChinese:
                    TRAP_IGNORE(iPtiEngine->ActivateLanguageL(iInputLanguage, EPtiEngineZhuyinVkb));                    
                    break;
                case ELangPrcChinese:
                    TRAP_IGNORE(iPtiEngine->ActivateLanguageL(iInputLanguage, EPtiEnginePinyinVkb));                    
                    break;   
                default:
                    break;                
                }
            }
        }
    */
    if (!LanguageIsChinese(aInputLanguage) && (aInputLanguage != ELangEnglish))
        {
        return ELangNone;
        }
    
    if (aInputLanguage == ELangEnglish)
        {
    	TLanguage displayLang = User::Language();

    	// set real language to a Chinese
        if (!LanguageIsChinese(iInputLanguage))
            {
            // original language is not Chinese
            if (LanguageIsChinese(displayLang))
                {
            	iInputLanguage = displayLang;
                }
            else
                {
                iInputLanguage = ELangPrcChinese;	
                }
                
            SetEngineLanguage(iInputLanguage);
            }
        
        return aInputLanguage;
        }
    else
        {
        // aInputLanguage is Chinese
        if (iInputLanguage != aInputLanguage)
            {
        	iInputLanguage = aInputLanguage;
            
            SetEngineLanguage(iInputLanguage);
            }
        
        return iInputLanguage;
        }
    /*
    const SLanguageItem* item = GetItemByLangId(iInputLanguage);
    if ( item )
        {
        return item->iDisplayText;
        }
    return KNullDesC;
    */
    } 
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::SetPermittedRange
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::SetPermittedRange(TInt aRange)
    {
    if ( aRange & ERangeNative )
        {
        aRange = aRange | ERangeAccent;
        }
        
    if ( iPermittedRange != aRange )
        {
        iPermittedRange = aRange;
        TPtr ptr((TUint16* ) &iPermittedRange,sizeof(TInt));
        iContext->SendEventToVkbControl(EVkbEventSetPermittedRange,NULL,ptr);    
        }    
    } 
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::SetCurrentAccent
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::SetCurrentAccent(TInt aAccent)
    {
    iCurrentAccent = aAccent;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::SetCandidatesCount
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::SetCandidatesCount(TInt aCandidatesCount)
    {
    iCandidatesCount = aCandidatesCount;
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::SetCurrentRange
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::SetCurrentRange(TInt aRange)
    {
    iCurrentRange = aRange;
    if ( aRange == ERangeNative )
        {
        //According to current input language, set IMLayout
        switch (iInputLanguage)
            {
            case ELangHongKongChinese:
                iImLayout = iDefaultImLayout4HK;  
                break;
            case ELangTaiwanChinese:
                iImLayout = iDefaultImLayout4TW;  
                break;
            case ELangPrcChinese:
                iImLayout = iDefaultImLayout4PRC;
                break;   
            default:
                iImLayout = EAknFepVkbImLatin;
                break;
            }
        
        switch(iImLayout)
            {
            case EAknFepVkbImCnPinyin:
                iPtiEngine->SetInputMode(EPtiEnginePinyinVkb);
                break;
            case EAknFepVkbImCnZhuyin:
                iPtiEngine->SetInputMode(EPtiEngineZhuyinVkb);
                break;
            case EAknFepVkbImCnStroke:
                iPtiEngine->SetInputMode(EPtiEngineStrokeVkb);
                break;
            case EAknFepVkbImCnCangjie:
                {
                // here , Get general setting:).
                TInt subCangJieMode = ECangJieNormal;
                iFepRepository->Get( KAknFepCangJieMode, subCangJieMode );
                switch( subCangJieMode )
                    {
                    case ECangJieEasy:
                        iPtiEngine->SetInputMode( EPtiEngineEasyCangjieVkb );
                        break;
                    case ECangJieAdvanced:
                        iPtiEngine->SetInputMode( EPtiEngineAdvCangjieVkb );
                        break;
                    case ECangJieNormal:
                        iPtiEngine->SetInputMode( EPtiEngineNormalCangjieVkb );
                        break;
                    default:
                        iPtiEngine->SetInputMode( EPtiEngineNormalCangjieVkb );
                        break;
                    }
                }
                break;
            default:
                break;
            }
        }
    else if( aRange == ERangeNumber )
        {
        if (iInputLanguage == ELangTaiwanChinese && iDefaultImLayout4TW != EAknFepVkbImCnStroke)
        	{
        	iImLayout = EAknFepVkbImNumberForZhuyin;
        	}
        else
        	{
        	iImLayout = EAknFepVkbImNumber;
        	}
        }
    else
        {
        if (iInputLanguage == ELangTaiwanChinese && iDefaultImLayout4TW != EAknFepVkbImCnStroke)
        	{
        	iImLayout = EAknFepVkbImLatinForZhuyin;
        	}
        else
        	{
        	iImLayout = EAknFepVkbImLatin;
        	}
        }
    } 

void CAknFepVkbDataMgr::SetDataQueryRect(const TRect& aRect)
    {
    iDataQueryRect = aRect;    
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::SetEditorNumericKeymap
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::SetEditorNumericKeymap(TInt aEditorNumericKeymap)
    {
    iEditorNumericKeymap = aEditorNumericKeymap;
    } 
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::SetImLayout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::SetImLayout(TInt aImLayout)
    {
    iImLayout = aImLayout;
    ResetUnitSize();
    }

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::ChangeShiftDown
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TInt CAknFepVkbDataMgr::ChangeShiftDown()
    {
    iShiftDown = ! iShiftDown;
    return CalculateCase();
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::ChangeCapslockDown
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TInt CAknFepVkbDataMgr::ChangeCapslockDown()
    {
    iShiftDown = 0;
    iCapslockDown = ! iCapslockDown;
    return CalculateCase();
    }

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::CalculateCase
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
TInt CAknFepVkbDataMgr::CalculateCase()
    {
    if (iShiftDown && iCapslockDown)
        return ECaseInverseText;
    else if (!iShiftDown && iCapslockDown)
        return ECaseUpper;
    else if (iShiftDown && !iCapslockDown)
        return ECaseText;
    else if (!iShiftDown && !iCapslockDown)
        return ECaseLower;
    else 
        return ECaseLower;
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::SetShiftDown
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::SetShiftDown(TBool aFlag)
    {
    iShiftDown = aFlag;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::SetCapslockDown
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::SetCapslockDown(TBool aFlag)
    {
    iCapslockDown = aFlag;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::SetLengthWhenButtondownOnBack
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::SetLengthWhenButtondownOnBack(TInt aLength)
    {
    iLengthWhenButtondownOnBack = aLength;
    }
            
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::SetData
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::SetData(TAknFepDataType aDataType,TInt aData)
    {
    switch (aDataType)
        {
        case EAknFepDataTypeCase:
            SetCase(aData);
            break;
        case EAknFepDataTypeInputLanguage:
            SetInputLanguage(aData);
            break;
        case EAknFepDataTypePermittedRange:
            SetPermittedRange(aData);
            break;
        case EAknFepDataTypeCurrentRange:
            SetCurrentRange(aData);
            break;
        case EAknFepDataTypeIMLayout:
            SetImLayout(aData);
            break; 
        case EAknFepDataTypeNumericKeymap:
            SetEditorNumericKeymap((TAknEditorNumericKeymap)aData);
            break;
        case EAknFepDataTypeShfitDown:
            SetShiftDown(aData);
            break;
        case EAknFepDataTypeCapslockDown:
            SetCapslockDown(aData);
            break;            
        case EAknFepDataTypeCurrentAccent:
            SetCurrentAccent(aData);
            break;
        case EAknFepDataTypeCandidateCount:
            SetCandidatesCount(aData);
            break;
        case EAknFepDataTypeLengthWhenButtondownOnBack:
            SetLengthWhenButtondownOnBack(aData);  
            break;  
        case EAknFepDataTypeWindowPosition:
            SetWindowPosition(aData);    
            break;
        case EAknFepDataTypeUnitWidth:
            iUnitWidth = aData;
            break;
        case EAknFepDataTypeUnitHeight:        
            iUnitHeight = aData;
            break;
        case EAknFepDataTypeUseDefinedResource:
            {
            delete iUserDefinedResource;
            TInt* len = (TInt*)(aData - 4);
            TPtrC16 keymapRes((const TUint16*)aData, *len/2);  
            iUserDefinedResource = keymapRes.Alloc();
            }
            break;
        case EAknFepDataTypeNeedBRJustify:
            iNeedBrJustify = aData;
        default:
            break;
        }
    } 
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::RequestData
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TInt CAknFepVkbDataMgr::RequestData(TAknFepDataType aDataType)  
    {
    switch (aDataType)
        {
        case EAknFepDataTypeCase:
            return iCase;
        case EAknFepDataTypeInputLanguage:
            return iInputLanguage;
        case EAknFepDataTypePermittedRange:
            return iPermittedRange;
        case EAknFepDataTypeCurrentRange:
            return iCurrentRange;
        case EAknFepDataTypeIMLayout:
            return iImLayout;
        case EAknFepDataTypeNumericKeymap:
            return iEditorNumericKeymap;
        case EAknFepDataTypeShfitDown:
            return iShiftDown;
        case EAknFepDataTypeCapslockDown:
            return iCapslockDown;
        case EAknFepDataTypeCurrentAccent:
            return iCurrentAccent;
        case EAknFepDataTypeCandidateCount:
            return iCandidatesCount;
        case EAknFepDataTypeLengthWhenButtondownOnBack:
            return iLengthWhenButtondownOnBack;
        case EAknFepDataTypeWindowPosition:
            return iWindowPosition;        
        case EAknFepDataTypeUnitWidth:
            return iUnitWidth;
        case EAknFepDataTypeUnitHeight:
            return iUnitHeight;
        case EAknFepDataTypeScreenSize:
            return iScreenMode.iPixelSize.iWidth | ( iScreenMode.iPixelSize.iHeight << 16);
        case EAknFepDataTypeDataQueryRect:
            return (TInt)&iDataQueryRect;
        case EAknFepDataTypeUseDefinedResource:
            return (TInt) iUserDefinedResource;
        case EAknFepDataTypeShadowTlSize:
            return (TInt)(&iShadowTl);
        case EAknFepDataTypeShadowBrSize:
            return (TInt)(&iShadowBr);
        case EAknFepDataTypeMainPaneRect:
            return (TInt)(&iMainPaneRect);
        case EAknFepDataTypeIcfColumnNums:
            {
            TInt columNum = 7;
            if( iDefaultImLayout4TW != EAknFepVkbImCnStroke && 
            	iInputLanguage == ELangTaiwanChinese /* ||
                ( iCurrentRange == ERangeNumber && 
                  static_cast<CAknFepVkbLayout*>(iContext)->PenInputType() == EPluginInputModeFSQ )*/ )
            	{
            	columNum = 8;	
            	}
            return columNum;
            }
        case EAknFepDataTypeTitleBarHeight:
            {
            if( EAknFepVkbImCnZhuyin == iImLayout )
                {
                return iUnitHeight;
                }
            else
                {
                return iUnitHeight*2;
                }
            }
        case EAknFepDataTypeDefaultIMLayout:
            return iDefaultImLayout4TW;
        case EAknFepDataTypeNeedBRJustify:
            return iNeedBrJustify;
        default:
            break; 
        }
    return 0;
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::IsShiftPermitted
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TBool CAknFepVkbDataMgr::IsShiftPermitted()
    {
    if ( ( iImLayout == EAknFepVkbImLatin ) || ( iImLayout == EAknFepVkbImNumber )
        || ( iImLayout == EAknFepVkbImLatinForZhuyin ) 
        || ( iImLayout == EAknFepVkbImNumberForZhuyin ) )
        {
        return (iPermittedCase & ECaseText) ||
               (iPermittedCase & ECaseInverseText);
        }
    return EFalse;
    }
    
TBool CAknFepVkbDataMgr::IsCapsLockPermitted()
    {
    if ( ( iImLayout == EAknFepVkbImLatin ) || ( iImLayout == EAknFepVkbImNumber ) )
        {
        return iPermittedCase & ECaseUpper;
        }
    return EFalse;
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::CAknFepVkbDataMgr::GetItemByLangId
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
/*
const CAknFepVkbDataMgr::SLanguageItem* CAknFepVkbDataMgr::GetItemByLangId(TInt aLangId)
    {
    for (TInt i=0;i<iLangStringList.Count();i++)
        {
        SLanguageItem* item = iLangStringList[i];
        if (item->iLangId == aLangId)
            {
            return item;
            }
        }
    
    return GetItemByLangId(0);
    }
*/    

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::HandleFepRepositoryCallBack
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TInt CAknFepVkbDataMgr::HandleFepRepositoryCallBack(TAny* aPtr)
    {
    CAknFepVkbDataMgr *self = STATIC_CAST(CAknFepVkbDataMgr*, aPtr);
    
    CAknFepRepositoryWatcher* watcher = self->iFepRepositoryWatcher;
    
    if (watcher)
        {
        if ( (watcher->ChangedKey()) == self->iPositionSettingId )
            {
            return self->PositionChangeNotification();
            }
        }
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::HandleGSRepositoryCallBack
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TInt CAknFepVkbDataMgr::HandleGSRepositoryCallBack(TAny* aPtr)
    {
    CAknFepVkbDataMgr *self = STATIC_CAST(CAknFepVkbDataMgr*, aPtr);
    
    CAknFepRepositoryWatcher* watcher = self->iGSRepositoryWatcher;
    
    if (watcher )
        {
        if  ( (watcher->ChangedKey() == KSettingsDefaultVkbPrc)
              || (watcher->ChangedKey() == KSettingsDefaultVkbTw) 
                 || (watcher->ChangedKey() == KSettingsDefaultVkbHk) )
            {
            return self->DefaultModeChangeNotification();
            }
        else if (watcher->ChangedKey() == KSettingsPenInputLang)
            {
            return self->PeninputlanguageChangeNotification();
            }
        }
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::PositionChangeNotification
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TInt CAknFepVkbDataMgr::PositionChangeNotification()
    {
    if (iFepRepository)
        {
        TInt newValue;

        iFepRepository->Get(iPositionSettingId, newValue);
        
        // if position have been changed, then notify window to change
        if ( iWindowPosition != newValue )
            {
            iWindowPosition = newValue;

            iContext->SendEventToVkbControl(EVkbEventPositionChanged,NULL);
            }
        }
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::DefaultModeChangeNotification
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TInt CAknFepVkbDataMgr::DefaultModeChangeNotification()
    {
    //Read default mode again the peninput language
    iGSRepository->Get(KSettingsDefaultVkbHk, iDefaultImLayout4HK);
    
    switch(iDefaultImLayout4HK)
        {
        case EAknFepVkbImCnStroke :
        case EAknFepVkbImCnCangjie:
        case EAknFepVkbImLatin:
            break;
        default:
            iDefaultImLayout4HK = EAknFepVkbImCnStroke;
            break;        
        }
        
    iGSRepository->Get(KSettingsDefaultVkbTw, iDefaultImLayout4TW);
    switch(iDefaultImLayout4TW)
        {
        case EAknFepVkbImCnZhuyin :
        case EAknFepVkbImCnStroke :
        case EAknFepVkbImLatin    :
            break;
        default:
            iDefaultImLayout4TW = EAknFepVkbImCnZhuyin;
            break;        
        }

    iGSRepository->Get(KSettingsDefaultVkbPrc, iDefaultImLayout4PRC);
    switch(iDefaultImLayout4PRC)
        {
        case EAknFepVkbImCnPinyin: 
        case EAknFepVkbImCnStroke :
        case EAknFepVkbImLatin    :
            break;
        default:
            iDefaultImLayout4PRC = EAknFepVkbImCnPinyin;
            break;        
        }
    
    iNeedBrJustify = ETrue;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::PeninputlanguageChangeNotification
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TInt CAknFepVkbDataMgr::PeninputlanguageChangeNotification()
    {
    //Read peninput language again 

    TInt value = 0;
    if (iGSRepository)
        {
        iGSRepository->Get(KSettingsPenInputLang, value);
        }
        
    //Call layout to change language
    //const TDesC& showText = SetInputLanguage(value);    
    TInt switchDisplayLang = SetInputLanguage(value);
        
    TBuf<10> showText;    
    AknPenInputUtils::GetISOLanguageCode(TLanguage(switchDisplayLang), showText);
    
    iContext->SendEventToVkbControl(EVkbEventLanguageChanged,NULL,showText);     
        
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::WriteWindowPostion
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::WriteWindowPostion(TInt aPosition)
    {
    if (iFepRepository)
        {
        iFepRepository->Set(iPositionSettingId, aPosition);
        }
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::WriteLastUsedRange
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::WriteLastUsedRange(TInt aLastUsedRange)
    {
    if (iFepRepository)
        {
        iFepRepository->Set(KAknFepLastUsedRange,aLastUsedRange);
        }
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::SetScreenMode
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::SetScreenMode(const TPixelsTwipsAndRotation& aScreenMode)
    {
    switch (aScreenMode.iRotation)
        {
        case CFbsBitGc::EGraphicsOrientationNormal:
            {
            iPositionSettingId = KAknFepHwrWindowPosition0;
            break;
            }
        case CFbsBitGc::EGraphicsOrientationRotated90:
            {
            iPositionSettingId = KAknFepHwrWindowPosition1;
            break;
            }
        case CFbsBitGc::EGraphicsOrientationRotated180:
            {
            iPositionSettingId = KAknFepHwrWindowPosition2;
            break;
            }
        case CFbsBitGc::EGraphicsOrientationRotated270:
            {
            iPositionSettingId = KAknFepHwrWindowPosition3;
            break;
            }
        default:
            //User::Panic(KCategory, KErrArgument);
            break;
        }
    
    iScreenMode = aScreenMode;
              
    if (iFepRepository)
        {
        iFepRepository->Get(iPositionSettingId, iWindowPosition);        
        }
    }         

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::ReadUnitShadowSize
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::ReadUnitShadowSize()
    {
    if (SizeChanging() || (!iUnitShadowSizeRead))
    	{
        iUnits.Reset();
    
        TAknWindowLineLayout appWnd; 
        TAknWindowLineLayout mainPane;
        // PSZ means pinyin(cangjie, latin), stroke, zhuyin respectively
        TAknWindowLineLayout wndLayoutP, wndLayoutS, wndLayoutZ;
        //TAknWindowLineLayout unitGridP, unitGridS, unitGridZ;
        TAknLayoutRect mainPaneRect;
        TAknLayoutRect wndRectP, wndRectS, wndRectZ;
        //TAknLayoutRect unitRectP, unitRectS, unitRectZ;
            
        appWnd = AknLayoutScalable_Avkon::application_window( 0 ).LayoutLine();
    
        if (appWnd.iW > appWnd.iH)
            {
    	    // landscape
            mainPane = AknLayoutScalable_Avkon::main_pane(4).LayoutLine();
            }
        else
            {
    	    // portrait
    	    mainPane = AknLayoutScalable_Avkon::main_pane(1).LayoutLine();
            }
    
        mainPaneRect.LayoutRect(appWnd.Rect(), mainPane);
    
        iMainPaneRect = mainPaneRect.Rect();
    
        wndLayoutP = AknLayoutScalable_Avkon::popup_fep_vkb_window(2).LayoutLine();
        wndRectP.LayoutRect(mainPaneRect.Rect(), wndLayoutP);
        wndLayoutS = AknLayoutScalable_Avkon::popup_fep_vkb_window(1).LayoutLine();
        wndRectS.LayoutRect(mainPaneRect.Rect(), wndLayoutS);
        wndLayoutZ = AknLayoutScalable_Avkon::popup_fep_vkb_window(3).LayoutLine();
        wndRectZ.LayoutRect(mainPaneRect.Rect(), wndLayoutZ);
    
        TAknWindowLineLayout shadowPane,shadowTl,shadowBr;
        TAknLayoutRect shadowRect,tlRect,brRect;
    
        shadowPane = AknLayoutScalable_Avkon::bg_popup_fep_shadow_pane(1).LayoutLine();
        shadowRect.LayoutRect(wndRectP.Rect(), shadowPane);
    
        shadowTl = AknLayoutScalable_Avkon::bg_popup_fep_shadow_pane_g1().LayoutLine();
        tlRect.LayoutRect(shadowRect.Rect(), shadowTl);
    
        shadowBr = AknLayoutScalable_Avkon::bg_popup_fep_shadow_pane_g8().LayoutLine();
        brRect.LayoutRect(shadowRect.Rect(), shadowBr);
    
        iShadowTl = tlRect.Rect().Size();
        iShadowBr = brRect.Rect().Size();

/*        unitGridP = AknLayoutScalable_Avkon::aid_size_cell_vkb(2).LayoutLine();
        unitRectP.LayoutRect(wndRectP.Rect(), unitGridP);
        unitGridS = AknLayoutScalable_Avkon::aid_size_cell_vkb(3).LayoutLine();
        unitRectS.LayoutRect(wndRectP.Rect(), unitGridS);
        unitGridZ = AknLayoutScalable_Avkon::aid_size_cell_vkb(4).LayoutLine();
        unitRectZ.LayoutRect(wndRectP.Rect(), unitGridZ);
    
        iUnits.Append(unitRectP.Rect().Size());
        iUnits.Append(unitRectS.Rect().Size());
        iUnits.Append(unitRectZ.Rect().Size());
*/
		//TSize  unitGridP, unitGridS, unitGridZ, unitGridL, unitGridN;
		TSize unit10x4, unit11x4;
		if( static_cast<CAknFepVkbLayout*>(iContext)->PenInputType() == EPluginInputModeFSQ )
			{
			unit10x4 = CPeninputCnGenericVkbLafMgr::GetCellSize( EVkbCangjieAndPinying_FSQ );
			unit11x4 = CPeninputCnGenericVkbLafMgr::GetCellSize( EVkbZhuyin_FSQ );
			}
		else if( static_cast<CAknFepVkbLayout*>(iContext)->PenInputType() == EPluginInputModeVkb )
			{
			unit10x4 = CPeninputCnGenericVkbLafMgr::GetCellSize( EVkbCangjieAndPinying );
			unit11x4 = CPeninputCnGenericVkbLafMgr::GetCellSize( EVkbZhuyin );
			}
		
        iUnits.Append( unit10x4 );
        iUnits.Append( unit11x4 );
		     		     
        iUnitShadowSizeRead = ETrue;
    	}
    
    ResetUnitSize();
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::ResetUnitSize
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::ResetUnitSize()
    {
    TInt index;
    switch (iImLayout)
        {
        
	    case EAknFepVkbImCnPinyin:
	    case EAknFepVkbImCnStroke:
	    case EAknFepVkbImCnCangjie:
	    case EAknFepVkbImLatin:
	    case EAknFepVkbImNumber:
	    	{
	    	index = 0;
	    	}
	    	break;
	    	
	    case EAknFepVkbImCnZhuyin:
	    case EAknFepVkbImLatinForZhuyin:
	    case EAknFepVkbImNumberForZhuyin:
        	{
        	if (iDefaultImLayout4TW == EAknFepVkbImCnStroke)
        	    {
        	    index = 0;
        	    }
        	else
        	    {
        	    index = 1;
        	    }
        	}
        	break;
       	default:
       	    {
            index = 0;
       	    }
            break;       	
        }

    if (iUnits.Count() >= (index+1))
        {
        TInt tempBtnWidth = iUnits[index].iWidth;
        TInt tempBtnHeight = iUnits[index].iHeight;
      
        iUnitWidth = tempBtnWidth;
        iUnitHeight = tempBtnHeight;            
        }
    else
        {
        iUnitWidth = 0;
        iUnitHeight = 0;
        }    
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::SetSizeChanging
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
void CAknFepVkbDataMgr::SetSizeChanging(TBool aFlag)
	{
	iSizeChanging = aFlag;
	}

// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::SizeChanging
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TBool CAknFepVkbDataMgr::SizeChanging()
	{
	return iSizeChanging;
	}
	
// ---------------------------------------------------------------------------
// CAknFepVkbDataMgr::UnitShadowSizeSet
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
TBool CAknFepVkbDataMgr::UnitShadowSizeSet()
	{
	return iUnitShadowSizeRead;
	}

// End Of File
