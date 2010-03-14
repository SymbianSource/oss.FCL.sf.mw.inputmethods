/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/


#include <PtiEngine.h>
#include <centralrepository.h>

#include <osshangulinputcontext.h>

#include "PtiKoreanQwertyCoreDebug.h"
#include "PtiKoreanQwertyCore.h"	// CPtiKoreanQwertyCore
#include "PtiKoreanQwertyCore.pan"	  	// panic codes
#include "PtiKoreanQwertyCoreCRDefs.h"

#include <e32std.h>

const static TOssCombinationItem sDefaulTOssCombinationItem[] =
    {
    {0x1169,0x1161,0x116a}, /* jungseong o      + a       = wa            */
    {0x1169,0x1162,0x116b}, /* jungseong o      + ae      = wae           */
    {0x1169,0x1175,0x116c}, /* jungseong o      + i       = oe            */
    {0x116e,0x1165,0x116f}, /* jungseong u      + eo      = weo           */
    {0x116e,0x1166,0x1170}, /* jungseong u      + e       = we            */
    {0x116e,0x1175,0x1171}, /* jungseong u      + i       = wi            */
    {0x1173,0x1175,0x1174}, /* jungseong eu     + i       = yi            */
    {0x11a8,0x11ba,0x11aa}, /* jongseong kiyeok + sios    = kiyeok-sois   */
    {0x11ab,0x11bd,0x11ac}, /* jongseong nieun  + cieuc   = nieun-cieuc   */
    {0x11ab,0x11c2,0x11ad}, /* jongseong nieun  + hieuh   = nieun-hieuh   */
    {0x11af,0x11a8,0x11b0}, /* jongseong rieul  + kiyeok  = rieul-kiyeok  */
    {0x11af,0x11b7,0x11b1}, /* jongseong rieul  + mieum   = rieul-mieum   */
    {0x11af,0x11b8,0x11b2}, /* jongseong rieul  + pieup   = rieul-pieup   */
    {0x11af,0x11ba,0x11b3}, /* jongseong rieul  + sios    = rieul-sios    */
    {0x11af,0x11c0,0x11b4}, /* jongseong rieul  + thieuth = rieul-thieuth */
    {0x11af,0x11c1,0x11b5}, /* jongseong rieul  + phieuph = rieul-phieuph */
    {0x11af,0x11c2,0x11b6}, /* jongseong rieul  + hieuh   = rieul-hieuh   */
    {0x11b8,0x11ba,0x11b9}, /* jongseong pieup  + sios    = pieup-sios    */    
    };

const TInt KSizeOfDefaulTOssCombinationItem=sizeof(sDefaulTOssCombinationItem)/sizeof(TOssCombinationItem);

const static TOssCombinationItem KCombiSsangKiyeokCho={0x1100,0x1100,0x1101}; /* choseong  kiyeok + kiyeok  = ssangkiyeok   */
const static TOssCombinationItem KCombiSsangKiyeokJong={0x11a8,0x11a8,0x11a9}; /* jongseong kiyeok + kiyeok  = ssangekiyeok  */
const static TOssCombinationItem KCombiSsangTikeut={0x1103,0x1103,0x1104}; /* choseong  tikeut + tikeut  = ssangtikeut   */
const static TOssCombinationItem KCombiSsangPieup={0x1107,0x1107,0x1108}; /* choseong  pieup  + pieup   = ssangpieup    */
const static TOssCombinationItem KCombiSsangSiosCho={0x1109,0x1109,0x110a}; /* choseong  sios   + sios    = ssangsios     */
const static TOssCombinationItem KCombiSsangSiosJong={0x11ba,0x11ba,0x11bb}; /* jongseong sios   + sios    = ssangsios     */
const static TOssCombinationItem KCombiSsangCieuc={0x110c,0x110c,0x110d}; /* choseong  cieuc  + cieuc   = ssangcieuc    */
const static TOssCombinationItem KCombiYaPlusI={0x1163,0x1175,0x1164}; /* jungseong ya-o                 = ya + o */
const static TOssCombinationItem KCombiYuPlusI={0x1167,0x1175,0x1168}; /* jungseong yu-i                 = yu + i */

class CActiveCallback : public CActive
    {
public:
    CActiveCallback(CRepository& aRepository)
        : CActive(EPriorityNormal), 
          iRepository(aRepository)
        {
        CActiveScheduler::Add(this);
        }
    
    void Start(const TCallBack& aCallBack)
        {
        iCallBack=aCallBack;
        iRepository.NotifyRequest(EDblConsonentOnSetting,iStatus);
        SetActive();
        }
    
    void RunL()
        {
        iCallBack.CallBack();
        }
    
    void DoCancel()
        {
        iRepository.NotifyCancel(EDblConsonentOnSetting);
        }
    
    CRepository& iRepository;
    
    TCallBack iCallBack;
    
    };

/*
 ============================================================================
 Constructors & Destructiors
 ============================================================================
 */

CPtiKoreanQwertyCore* CPtiKoreanQwertyCore::NewLC(
        CPtiEngine* aOwner, 
        TDes* aTextBuffer)
    {
    CPtiKoreanQwertyCore* self = 
        new (ELeave) CPtiKoreanQwertyCore(aOwner,aTextBuffer);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CPtiKoreanQwertyCore* CPtiKoreanQwertyCore::NewL(
        CPtiEngine* aOwner, 
        TDes* aTextBuffer)
    {
    CPtiKoreanQwertyCore* self = 
        CPtiKoreanQwertyCore::NewLC(aOwner,aTextBuffer);
    CleanupStack::Pop(self);
    return self;
    }

CPtiKoreanQwertyCore::CPtiKoreanQwertyCore(
        CPtiEngine* aOwner, 
        TDes* aTextBuffer)
    : iOwner(aOwner),
      iTextBuffer(aTextBuffer),
      iKoreanLanOpen(EFalse),
      iLowerCase(ETrue)
    {
    }

void CPtiKoreanQwertyCore::ConstructL()
    {
    _LIT8(KKeyboard,"2");
    iHic=COssHangulInputContext::NewL(KKeyboard);
    iCombination=COssCombination::NewL();
    for (TInt i=0;i<KSizeOfDefaulTOssCombinationItem;i++)
        {
        iCombination->AddCombinationL(sDefaulTOssCombinationItem[i]);
        }
    iRepository=CRepository::NewL(KCRUidPtiKoreanQwertyCore);
    UpdateFromRepositoryL();
    iCRNotifier=new (ELeave) CActiveCallback(*iRepository);
    iCRNotifier->Start(TCallBack(CPtiKoreanQwertyCore::CBSettingChanged,this));
    }

CPtiKoreanQwertyCore::~CPtiKoreanQwertyCore()
    {
    
    if ( iCRNotifier )
        {
        iCRNotifier->Cancel();
        }
    
    delete iCRNotifier;
    delete iRepository;
    delete iCombination;
    delete iHic;
    
    }

/*
 ============================================================================
 From CPtiCore
 ============================================================================
 */

void CPtiKoreanQwertyCore::InitializeL(
        CPtiEngine* aOwner, 
        TDes* aTextBuffer, 
        CArrayPtrFlat<CPtiCoreLanguage>* /*aLanguageList*/, 
        TBool aUseDefautUserDictionary)
    {
    LOG1("CPtiKoreanQwertyCore::InitializeL %d",aUseDefautUserDictionary);
    iOwner=aOwner;
    iTextBuffer=aTextBuffer;
    
    CPtiCoreLanguage* lang;
    lang = static_cast<CPtiCoreLanguage*>(iOwner->GetLanguage(ELangKorean));
    if (lang)
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT
        lang->SetCore(this, EPtiEngineQwertyKorean);
#else
        lang->SetCore(this, EPtiEngineQwerty);            
#endif //RD_INTELLIGENT_TEXT_INPUT
        }
    }
    
void CPtiKoreanQwertyCore::DropLanguages(CArrayPtrFlat<CPtiCoreLanguage>* /*aLanguageList*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::DropLanguages");
    }
    
TInt CPtiKoreanQwertyCore::OpenLanguageL(CPtiCoreLanguage *aLanguage)
    {
    LOG1("CPtiKoreanQwertyCore::OpenLanguageL id=%d",aLanguage->LanguageCode());
    TInt ret(KErrNotSupported);
    if (aLanguage->LanguageCode()==ELangKorean)
        {
        ret=KErrNone;
        iKoreanLanOpen=ETrue;
        }
    return ret;
    }
    
TInt CPtiKoreanQwertyCore::CloseLanguageL()
    {
    LOG1("CPtiKoreanQwertyCore::CloseLanguageL opened=%d",iKoreanLanOpen);
    __ASSERT_ALWAYS(iKoreanLanOpen,User::Panic(_L("PtiKoreanQwertyCore"),__LINE__));
    iKoreanLanOpen=EFalse;
    return KErrNone;
    }
    
TPtrC CPtiKoreanQwertyCore::GetCurrentWord() const
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::GetCurrentWord");
    TPtrC ret;
    return ret;
    }
    
TInt CPtiKoreanQwertyCore::CommitCurrentWord()
    {
    LOG("CPtiKoreanQwertyCore::CommitCurrentWord");
    iTextBuffer->Copy(iHic->GetCommitString());
    return KErrNone;
    }
    
MPtiCoreInfo* CPtiKoreanQwertyCore::GetCoreInfo()
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::GetCoreInfo");
    return NULL;
    }
            
TInt CPtiKoreanQwertyCore::SetReordering(TBool /*aStatus*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::SetReordering");
    
    return KErrNotSupported;
    }
    
void CPtiKoreanQwertyCore::AppendKeyPress(TPtiKey aKey)
    {
    TInt key=aKey;
    if (iLowerCase && ('A'<=key && key <= 'Z'))
        {
        key=key-'A'+'a';
        }
    LOG1("CPtiKoreanQwertyCore::AppendKeyPress %c",key);
    TBool preEditOngoing(iHic->GetPreeditString().Length()>0);
    if (iHic->Process(key))
        {
        if (preEditOngoing&&iTextBuffer->Length())
            {
            iTextBuffer->SetLength(iTextBuffer->Length()-1);
            }
        if (iHic->GetCommitString().Length())
            {
            LOG1("CPtiKoreanQwertyCore::AppendKeyPress len cmt %d",iHic->GetCommitString().Length());
            iTextBuffer->Append(iHic->GetCommitString());
            }
        iTextBuffer->Append(iHic->GetPreeditString());
        LOG1("CPtiKoreanQwertyCore::AppendKeyPress len pre %d",iTextBuffer->Length());
        }
    }
    
void CPtiKoreanQwertyCore::DeleteKeyPress()
    {
    LOG("CPtiKoreanQwertyCore::DeleteKeyPress");
    TInt lenPrevText(iTextBuffer->Length());
    TInt lenPrevPreEdit(iHic->GetPreeditString().Length());
    if (lenPrevPreEdit)
        {
        TInt lenToDelete=lenPrevText>=lenPrevPreEdit?lenPrevPreEdit:lenPrevText;
        iTextBuffer->SetLength(iTextBuffer->Length()-lenToDelete);
        }
    else if (lenPrevText)
        {
        iTextBuffer->SetLength(iTextBuffer->Length()-1);
        }
    iHic->BackSpace();
    if (iHic->GetPreeditString().Length())
        {
        iTextBuffer->Append(iHic->GetPreeditString());
        }
    }
    
void CPtiKoreanQwertyCore::ClearBuffer()
    {
    LOG("CPtiKoreanQwertyCore::ClearBuffer");
    iHic->Reset();
    }
    
TInt CPtiKoreanQwertyCore::SetCurrentInputSequence(TPtrC8 /*aInput*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::SetCurrentInputSequence");
    
    return KErrNotSupported;
    }
    
TInt CPtiKoreanQwertyCore::Convert(
        TPtiCharConversion /*aType*/,
        TAny* /*aInput*/,
        TInt /*aInputLength*/,
        TAny* /*aOutput*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::Convert");
    
    return KErrNotSupported;
    }

TInt CPtiKoreanQwertyCore::SetExternalKeyMapL(
        TPtiEngineInputMode /*aMode*/,
        TPtiKey /*aKey*/,
        TDesC& /*aKeyMap*/,
        TPtiTextCase /*aCase*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::SetExternalKeyMapL");
    
    return KErrNotSupported;
    }

TInt CPtiKoreanQwertyCore::GetSpelling(
        TUint16 /*aInput*/, 
        TDes& /*aOutput*/, 
        TPtiSpelling /*aType*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::GetSpelling");
    
    return KErrNotSupported;
    }
    
TInt CPtiKoreanQwertyCore::CancelTimerActivity()
    {
    LOG("CPtiKoreanQwertyCore::CancelTimerActivity");
    return KErrNone;
    }
    
TInt CPtiKoreanQwertyCore::HandleCommandL(TPtiEngineCommand aCommand, TAny* /*aParams*/)
    {
    LOG1("CPtiKoreanQwertyCore::HandleCommandL %d",aCommand);
    TInt ret(KErrNone);
    switch (aCommand)
        {
        case EPtiCommandVowelSeqFlag:
        case EPtiCommandDoubleWideEntry:
        case EPtiCommandPollQwertyDeadKeyRootFlag:
        case EPtiCommandGetAndClearLastVietnameseChar:
        case EPtiCommandClearVowelSeq:
        case EPtiCommandGetAndClearDeadKeyRootChar:
            ret=KErrNotSupported;
            break;
        case EPtiCommandUseLatinCaseUpperOnlyKeyMap:
            iLowerCase=EFalse;
            break;
        case EPtiCommandUseLatinCaseLowerOnlyKeyMap:
            iLowerCase=ETrue;
            break;
        case EPtiCommandGetAutoCompletionTailLength:
            //To do:
            break;
        case EPtiCommandGetSecondaryCandidate:
            //To do:
            break;
        case EPtiCommandSetCorrectionLevel:
            // To do
            break;
        default:
            LOG_ASSERT("CPtiKoreanQwertyCore::HandleCommandL");
            break;
        }
    
    return ret;
    }

/*
 ============================================================================
 From CPtiCore Predictive latin API
 ============================================================================
 */

TInt CPtiKoreanQwertyCore::NumberOfCandidates()
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::NumberOfCandidates");
    
    return KErrNotSupported;
    }
    
TInt CPtiKoreanQwertyCore::GetCandidateListL(CDesCArray& /*aList*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::GetCandidateListL");
    
    return KErrNotSupported;
    }
    
TInt CPtiKoreanQwertyCore::GetNextWordCandidateListL(CDesCArray& /*aList*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::GetNextWordCandidateListL");
    
    return KErrNotSupported;
    }
    
TInt CPtiKoreanQwertyCore::GetFirstCandidate(TDes& /*aResult*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::GetFirstCandidate");
    
    return KErrNotSupported;
    }
    
TInt CPtiKoreanQwertyCore::GetNextCandidate(TDes& /*aResult*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::GetNextCandidate");
    
    return KErrNotSupported;
    }
    
TInt CPtiKoreanQwertyCore::GetPreviousCandidate(TDes& /*aResult*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::GetPreviousCandidate");
    
    return KErrNotSupported;
    }

/*
 ============================================================================
 From CPtiCore User dictionary API
 ============================================================================
 */

TInt CPtiKoreanQwertyCore::AddUserDictionaryEntry(
        MPtiUserDictionaryEntry& /*aEntry*/, 
        CPtiUserDictionary* /*aDict*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::AddUserDictionaryEntry");
    
    return KErrNotSupported;
    }
    
TInt CPtiKoreanQwertyCore::RemoveUserDictionaryEntry(
        MPtiUserDictionaryEntry& /*aEntry*/, 
        CPtiUserDictionary* /*aDict*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::RemoveUserDictionaryEntry");
    
    return KErrNotSupported;
    }
    
TInt CPtiKoreanQwertyCore::NumberOfEntriesInUserDictionary(
        CPtiUserDictionary* /*aDict*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::NumberOfEntriesInUserDictionary");
    
    return KErrNotSupported;
    }
    
TInt CPtiKoreanQwertyCore::GetUserDictionaryEntry(
        TInt /*aIndex*/, 
        MPtiUserDictionaryEntry& /*aResult*/, 
        CPtiUserDictionary* /*aDict*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::GetUserDictionaryEntry");
    
    return KErrNotSupported;
    }
    
CPtiUserDictionary* CPtiKoreanQwertyCore::CreateEmptyUserDictionaryL(TInt /*aMaxSize*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::CreateEmptyUserDictionaryL");
    
    return NULL;
    }
    
TInt CPtiKoreanQwertyCore::AttachUserDictionary(CPtiUserDictionary* /*aDict*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::AttachUserDictionary");
    
    return KErrNotSupported;
    }
    
TBool CPtiKoreanQwertyCore::DetachUserDictionary(CPtiUserDictionary* /*aDict*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::DetachUserDictionary");
    
    return EFalse;
    }

/*
 ============================================================================
 From CPtiCore Chinese API
 ============================================================================
 */

TPtrC CPtiKoreanQwertyCore::GetCandidatePage()
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::GetCandidatePage");
    
    return KNullDesC();
    }
    
TBool CPtiKoreanQwertyCore::NextCandidatePage()
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::NextCandidatePage");
    
    return EFalse;
    }
    
TBool CPtiKoreanQwertyCore::PreviousCandidatePage()
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::PreviousCandidatePage");
    
    return EFalse;
    }
    
TBool CPtiKoreanQwertyCore::MoreCandidatePages()
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::MoreCandidatePages");
    
    return EFalse;
    }
    
void CPtiKoreanQwertyCore::SetCandidatePageLength(TInt /*aLength*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::SetCandidatePageLength");
    
    }
    
TPtrC CPtiKoreanQwertyCore::GetPhoneticSpelling(TInt /*aIndex*/) const
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::GetPhoneticSpelling");
    return KNullDesC();
    }

TInt CPtiKoreanQwertyCore::PhoneticSpellingCount() const
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::PhoneticSpellingCount");
    
    return KErrNotSupported;
    }

TBool CPtiKoreanQwertyCore::SelectPhoneticSpelling(TInt /*aIndex*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::SelectPhoneticSpelling");
    
    return EFalse;
    }
    
TInt CPtiKoreanQwertyCore::SelectedPhoneticSpelling() const
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::SelectedPhoneticSpelling");
    
    return KErrNotSupported;
    }

TBool CPtiKoreanQwertyCore::ToneMark(TText& /*aToneMark*/) const
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::ToneMark");
    
    return EFalse;
    }
    
TBool CPtiKoreanQwertyCore::IsToneMarkValidForSpelling()
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::IsToneMarkValidForSpelling");
    
    return EFalse;
    }
    
TBool CPtiKoreanQwertyCore::SetPredictiveChineseChar(const TDesC& /*aChar*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::SetPredictiveChineseChar");
    
    return EFalse;
    }
    
TBool CPtiKoreanQwertyCore::SetToneMark(TInt /*aToneMark*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::SetToneMark");
    
    return EFalse;
    }


/*
 ============================================================================
 From CPtiCore Japanese API
 ============================================================================
 */

TPtrC CPtiKoreanQwertyCore::ReadingTextL()
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::ReadingTextL");
    
    return KNullDesC();
    }
    
MPtiEngineCompositionDataInterface* CPtiKoreanQwertyCore::CompositionData()
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::CompositionData");
    
    return NULL;
    }

/*
 ============================================================================
 From CPtiCore Chinese API
 ============================================================================
 */

HBufC* CPtiKoreanQwertyCore::GetCandidatesByInputString(
        const TDesC& /*aInputString*/, 
        RPointerArray<HBufC>& /*aList*/,
        const TBool /*aIsPredictive*/)
    {
    LOG_ASSERT("CPtiKoreanQwertyCore::GetCandidatesByInputString");
    
    return NULL;
    }

void CPtiKoreanQwertyCore::UpdateFromRepositoryL()
    {
    UpdateDblConsonentL(EDblConsonentSsangKiyeok,KCombiSsangKiyeokCho);
    UpdateDblConsonentL(EDblConsonentSsangKiyeok,KCombiSsangKiyeokJong);
    UpdateDblConsonentL(EDblConsonentSsangTikeut,KCombiSsangTikeut);
    UpdateDblConsonentL(EDblConsonentSsangPieup,KCombiSsangPieup);
    UpdateDblConsonentL(EDblConsonentSsangSios,KCombiSsangSiosCho);
    UpdateDblConsonentL(EDblConsonentSsangSios,KCombiSsangSiosJong);
    UpdateDblConsonentL(EDblConsonentSsangCieuc,KCombiSsangCieuc);
    UpdateDblConsonentL(EDblConsonentYaPlusI,KCombiYaPlusI);
    UpdateDblConsonentL(EDblConsonentYuPlusI,KCombiYuPlusI);
    iHic->SetCombinationL(*iCombination);
    }

void CPtiKoreanQwertyCore::UpdateDblConsonentL(
        const TInt& aCenRepId,
        const TOssCombinationItem& aCombination)
    {
    TInt value;
    if (KErrNone==iRepository->Get(aCenRepId,value))
        {
        if (value)
            {
            iCombination->AddCombinationL(aCombination);
            }
        else
            {
            iCombination->RemoveCombination(aCombination);
            }
        }    
    }

TInt CPtiKoreanQwertyCore::CBSettingChanged(void* aSelf)
    {
    CPtiKoreanQwertyCore* self=(CPtiKoreanQwertyCore*)aSelf;
    TInt settingOnGoing(EFalse);
    TInt ret(KErrNone);
    if (KErrNone==self->iRepository->Get(EDblConsonentOnSetting,settingOnGoing))
        {
        if (!settingOnGoing)
            {
            TRAP(ret,self->UpdateFromRepositoryL());
            }
        }
    if (ret==KErrNone)
        {
        self->iCRNotifier->Start(
            TCallBack(CPtiKoreanQwertyCore::CBSettingChanged,self));        
        }
    return ret;
    }
