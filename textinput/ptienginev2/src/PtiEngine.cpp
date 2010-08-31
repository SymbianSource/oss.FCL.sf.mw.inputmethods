/*
* Copyright (c) 2003-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Predective text input engine API implementation
*
*/



#include "PtiEngine.h"
#include "PtiEngineImpl.h"
#include "PtiHwrRecognizer.h"
	
//
// CPtiEngine
//

// ---------------------------------------------------------------------------
// CPtiEngineImpl::NewL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPtiEngine* CPtiEngine::NewL(TBool aUseDefaultUserDictionary)
	{
	CPtiEngine *engine = new (ELeave) CPtiEngine();
	CleanupStack::PushL( engine );
	engine->ConstructL(aUseDefaultUserDictionary);
	CleanupStack::Pop(); // engine
	return engine;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::NewL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPtiEngine* CPtiEngine::NewL(const TDesC& /*aCoreName*/, TBool /*aUseDefaultUserDictionary*/)
	{
	// Deprecated.
	User::Leave(KErrNotSupported);
	return NULL;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::NewL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPtiEngine* CPtiEngine::NewL(const TUid aCoreUid, TBool aUseDefaultUserDictionary)
	{
	CPtiEngine *engine = new (ELeave) CPtiEngine();
	CleanupStack::PushL( engine );
	engine->ConstructL(aCoreUid, aUseDefaultUserDictionary);
	CleanupStack::Pop(); // engine
	return engine;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CPtiEngine
// 
// ---------------------------------------------------------------------------
//
CPtiEngine::CPtiEngine()
	{	
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::~CPtiEngine
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPtiEngine::~CPtiEngine()
	{	
	delete iImpl;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::ConstructL
// 
// ---------------------------------------------------------------------------
//
void CPtiEngine::ConstructL(TBool aUseDefaultUserDictionary)
	{
	iImpl = CPtiEngineImpl::NewL(this);
	iImpl->InitializeL(aUseDefaultUserDictionary);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::ConstructL
// 
// ---------------------------------------------------------------------------
//
void CPtiEngine::ConstructL(const TUid /*aCoreUid*/, TBool aUseDefaultUserDictionary)
	{
	iImpl = CPtiEngineImpl::NewL(this);
	iImpl->InitializeL(aUseDefaultUserDictionary);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetLanguage
// 
// ---------------------------------------------------------------------------
//
EXPORT_C MPtiLanguage* CPtiEngine::GetLanguage(TInt aCode) const
	{
	return iImpl->GetLanguage( aCode );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetAvailableLanguagesL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPtiEngine::GetAvailableLanguagesL(CArrayFix<TInt>* aResult)
	{
	iImpl->GetAvailableLanguagesL( aResult );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CreateUserDictionaryL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::CreateUserDictionaryL(TDesC& aFileName, CDesCArrayFlat* aWords, TPtiEngineInputMode aMode) 
	{
	return iImpl->CreateUserDictionaryL(aFileName, aWords, aMode);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::AttachUserDictionaryL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C MPtiUserDictionary* CPtiEngine::AttachUserDictionaryL(TDesC& aFileName)
	{
	return iImpl->AttachUserDictionaryL (aFileName );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::DetachUserDictionary
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::DetachUserDictionary(TDesC& aFileName)
	{
	return iImpl->DetachUserDictionary( aFileName );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::DetachUserDictionary
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::DetachUserDictionary(TInt aId)
	{
	return iImpl->DetachUserDictionary( aId );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetCurrentWord
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::SetCurrentWord(TPtrC aWord)
	{
	return iImpl->SetCurrentWord( aWord );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::ActivateLanguageL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::ActivateLanguageL(TInt aEpocLanguageID, TPtiEngineInputMode aMode)
	{
	return iImpl->ActivateLanguageL(aEpocLanguageID, aMode);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CloseCurrentLanguageL
// 
// ---------------------------------------------------------------------------
//	
EXPORT_C void CPtiEngine::CloseCurrentLanguageL()
	{
	iImpl->CloseCurrentLanguageL();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetInputMode
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::SetInputMode(TPtiEngineInputMode aMode)
	{
	return iImpl->SetInputMode( aMode );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetNextWordCandidateListL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::GetNextWordCandidateListL(CDesCArray& aList)
	{
	return iImpl->GetNextWordCandidateListL( aList );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::AppendKeyPress
// 
// ---------------------------------------------------------------------------
//		
EXPORT_C TPtrC CPtiEngine::AppendKeyPress(TPtiKey aKey)
	{
	return iImpl->AppendKeyPress( aKey );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::DeleteKeyPress
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TPtrC CPtiEngine::DeleteKeyPress()
	{
	return iImpl->DeleteKeyPress();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetLocalizedLanguageName
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPtiEngine::GetLocalizedLanguageName(TInt aLangCode, TDes& aResult)
	{
	TRAP_IGNORE(iImpl->GetLocalizedLanguageNameL(aLangCode, aResult));
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetCandidateListL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::GetCandidateListL(CDesCArray& aList)
	{
	return iImpl->GetCandidateListL( aList );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::FirstCandidate
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TPtrC CPtiEngine::FirstCandidate()
	{
	return iImpl->FirstCandidate();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::NextCandidate
// 
// ---------------------------------------------------------------------------
//	
EXPORT_C TPtrC CPtiEngine::NextCandidate()
	{
	return iImpl->NextCandidate();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::PreviousCandidate
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TPtrC CPtiEngine::PreviousCandidate()
	{
	return iImpl->PreviousCandidate();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CoreInfo
// 
// ---------------------------------------------------------------------------
//
EXPORT_C MPtiCoreInfo* CPtiEngine::CoreInfo(TPtiEngineInputMode aMode) const
	{
	return iImpl->CoreInfo( aMode );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::NumberOfCandidates
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::NumberOfCandidates()
	{	
	return iImpl->NumberOfCandidates();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetReordering
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::SetReordering(TBool aStatus)
	{
	return iImpl->SetReordering( aStatus );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::ClearCurrentWord
// 
// ---------------------------------------------------------------------------
//			
EXPORT_C void CPtiEngine::ClearCurrentWord()
	{
	iImpl->ClearCurrentWord();
	}
	

// ---------------------------------------------------------------------------
// CPtiEngineImpl::CommitCurrentWord
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::CommitCurrentWord()
	{
	return iImpl->CommitCurrentWord();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::AddCoreL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::AddCoreL(const TDesC& /*aFileName*/, TBool /*aUseDefaultUserDictionary*/)
	{
	User::Leave(KErrNotSupported);
	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::AddCoreL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::AddCoreL(const TUid aCoreUid, TBool aUseDefaultUserDictionary)
	{
	return iImpl->AddCoreL(aCoreUid, aUseDefaultUserDictionary);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CharConversion
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::CharConversion(TPtiCharConversion aType,
			                         TAny* aInput,
									 TInt aInputLength,
									 TAny* aOutput)
	{
	return iImpl->CharConversion(aType, aInput, aInputLength, aOutput);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::AvailableCharConversions
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TUint32 CPtiEngine::AvailableCharConversions() const
	{
	return iImpl->AvailableCharConversions();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetExternalKeyMapL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::SetExternalKeyMapL(TPtiEngineInputMode aMode,
			                                 TPtiKey aKey,
										     TDesC& aKeyMap,
											 TPtiTextCase aCase)
	{
	return iImpl->SetExternalKeyMapL(aMode, aKey, aKeyMap, aCase);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetSpelling
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::GetSpelling(TUint16 aInput, TDes& aOutput, TPtiSpelling aType)
	{
	return iImpl->GetSpelling(aInput, aOutput, aType);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CancelTimerActivity
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::CancelTimerActivity()
	{
	return iImpl->CancelTimerActivity();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CharacterToKey
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TPtiKey CPtiEngine::CharacterToKey(TUint16 aChar)
	{
	return iImpl->CharacterToKey( aChar );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::AddUserDictionaryEntry
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::AddUserDictionaryEntry(MPtiUserDictionaryEntry& aEntry)
	{
	return iImpl->AddUserDictionaryEntry( aEntry );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::AddUserDictionaryEntry
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::AddUserDictionaryEntry(MPtiUserDictionaryEntry& aEntry, TInt aId)
	{
	return iImpl->AddUserDictionaryEntry(aEntry, aId);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::RemoveEntryFromUserDictionary
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::RemoveEntryFromUserDictionary(MPtiUserDictionaryEntry& aEntry)
	{
	return iImpl->RemoveEntryFromUserDictionary( aEntry );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::RemoveEntryFromUserDictionary
// 
// ---------------------------------------------------------------------------
//	
EXPORT_C TInt CPtiEngine::RemoveEntryFromUserDictionary(MPtiUserDictionaryEntry& aEntry, TInt aId)
	{
	return iImpl->RemoveEntryFromUserDictionary(aEntry, aId);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::NumberOfEntriesInUserDictionary
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::NumberOfEntriesInUserDictionary()
	{
	return iImpl->NumberOfEntriesInUserDictionary();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::HandleCommandL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::HandleCommandL(TPtiEngineCommand aCommand, TAny* aParams)
	{
	return iImpl->HandleCommandL(aCommand, aParams);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CandidatePage
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TPtrC CPtiEngine::CandidatePage()
	{
	return iImpl->CandidatePage();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::NextCandidatePage
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiEngine::NextCandidatePage()
	{
	return iImpl->NextCandidatePage();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::PreviousCandidatePage
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiEngine::PreviousCandidatePage()
	{
	return iImpl->PreviousCandidatePage();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::MoreCandidatePages
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiEngine::MoreCandidatePages()
	{
	return iImpl->MoreCandidatePages();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetCandidatePageLength
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPtiEngine::SetCandidatePageLength(TInt aLength)
	{
	iImpl->SetCandidatePageLength( aLength );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CompositionData
// 
// ---------------------------------------------------------------------------
//
EXPORT_C MPtiEngineCompositionDataInterface* CPtiEngine::CompositionData()
	{
	return iImpl->CompositionData();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::DefaultUserDictionary
// 
// ---------------------------------------------------------------------------
//
EXPORT_C MPtiUserDictionary* CPtiEngine::DefaultUserDictionary(TPtiEngineInputMode aMode)
	{	
	return iImpl->DefaultUserDictionary( aMode );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetUserDictionaryEntry
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::GetUserDictionaryEntry(TInt aIndex, MPtiUserDictionaryEntry& aResult)
	{
	return iImpl->GetUserDictionaryEntry(aIndex, aResult);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetPhoneticSpelling
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TPtrC CPtiEngine::GetPhoneticSpelling(TInt aIndex) const 
	{
	return iImpl->GetPhoneticSpelling( aIndex );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::PhoneticSpellingCount
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::PhoneticSpellingCount() const	
	{
	return iImpl->PhoneticSpellingCount();
	}		
	

// ---------------------------------------------------------------------------
// CPtiEngineImpl::SelectPhoneticSpelling
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiEngine::SelectPhoneticSpelling(TInt aIndex)
	{
	return iImpl->SelectPhoneticSpelling( aIndex );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::SelectedPhoneticSpelling
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::SelectedPhoneticSpelling() const
	{
	return iImpl->SelectedPhoneticSpelling();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::ReadingTextL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TPtrC CPtiEngine::ReadingTextL()
	{
	return iImpl->ReadingTextL();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::EnableToneMarks
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPtiEngine::EnableToneMarks(TBool aValue)
	{
	iImpl->EnableToneMarks( aValue );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::ResetToneMark
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPtiEngine::ResetToneMark()
	{
	iImpl->ResetToneMark();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::ToneMark
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiEngine::ToneMark(TText& aToneMark) const
	{
	return iImpl->ToneMark( aToneMark );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::IsToneMarkValidForSpelling
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiEngine::IsToneMarkValidForSpelling() const
	{
	return iImpl->IsToneMarkValidForSpelling();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::IncrementToneMark
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiEngine::IncrementToneMark(TBool aOverrideInvalid)
	{
	return iImpl->IncrementToneMark( aOverrideInvalid );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetPredictiveChineseChar
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiEngine::SetPredictiveChineseChar(const TDesC& aChar)
	{
	return iImpl->SetPredictiveChineseChar( aChar );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetModeNameIndexL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPtiEngine::GetModeNameIndexL(TPtiChineseVariant aVariant, RArray<TInt>& aResult)
	{
	iImpl->GetModeNameIndexL(aVariant, aResult);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CurrentLanguage
// 
// ---------------------------------------------------------------------------
//
EXPORT_C MPtiLanguage* CPtiEngine::CurrentLanguage() 
	{
	return iImpl->CurrentLanguage();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CurrentWord
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TPtrC CPtiEngine::CurrentWord()
	{
	return iImpl->CurrentWord();	
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::InputMode
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TPtiEngineInputMode CPtiEngine::InputMode() const
	{
	return iImpl->InputMode();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetCase
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPtiEngine::SetCase(TPtiTextCase aCase)
	{
	iImpl->SetCase( aCase );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::Case
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TPtiTextCase CPtiEngine::Case() const
	{
	return iImpl->Case();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::NumberOfLanguages
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::NumberOfLanguages() const
	{
	return iImpl->NumberOfLanguages();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::LastEnteredKey
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TPtiKey CPtiEngine::LastEnteredKey() const
	{
	return iImpl->LastEnteredKey();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::Observer
// 
// ---------------------------------------------------------------------------
//
EXPORT_C MPtiObserver* CPtiEngine::Observer()
	{
	return iImpl->Observer();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetObserver
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPtiEngine::SetObserver(MPtiObserver* aObserver)
	{
	iImpl->SetObserver( aObserver );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CurrentInputSequence
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TPtrC8 CPtiEngine::CurrentInputSequence() const
	{
	return iImpl->CurrentInputSequence();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetPhoneticSpellingsL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::GetPhoneticSpellingsL(CDesCArray& aList)
	{
	return iImpl->GetPhoneticSpellingsL(aList);
	}
	

// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetChinesePhraseCandidatesL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::GetChinesePhraseCandidatesL(CDesCArray& aList)
	{
	return iImpl->GetChinesePhraseCandidatesL(aList);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetToneMark
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiEngine::SetToneMark(TInt aToneMark)
	{
	return iImpl->SetToneMark(aToneMark);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetAvailableLanguagesL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPtiEngine::GetAvailableLanguagesL(RArray<TInt>& aResult)
	{
	return iImpl->GetAvailableLanguagesL(aResult);
	}
	

// ---------------------------------------------------------------------------
// CPtiEngineImpl::CreateDefaultUserDictionaryFileL
// 
// ---------------------------------------------------------------------------
//	
EXPORT_C void CPtiEngine::CreateDefaultUserDictionaryFileL(TUid aCoreUid, TInt aSymbolClass)
	{
	iImpl->CreateDefaultUserDictionaryFileL(aCoreUid, aSymbolClass);
	}
	

// ---------------------------------------------------------------------------
// CPtiEngineImpl::AttachDefaultUserDictionaryL
// 
// ---------------------------------------------------------------------------
//	
EXPORT_C MPtiUserDictionary* CPtiEngine::AttachDefaultUserDictionaryL(TUid aCoreUid, TInt aSymbolClass)
	{
	return iImpl->AttachDefaultUserDictionaryL(aCoreUid, aSymbolClass);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::WriteSecondaryDataFileL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPtiEngine::WriteSecondaryDataFileL(TUid aCoreUid, TInt aIndexNumber, HBufC8* aData)
	{
	iImpl->WriteSecondaryDataFileL(aCoreUid, aIndexNumber, aData);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::ReadSecondaryDataFileL
// 
// ---------------------------------------------------------------------------
//		
EXPORT_C HBufC8* CPtiEngine::ReadSecondaryDataFileL(TUid aCoreUid, TInt aIndexNumber)
	{
	return iImpl->ReadSecondaryDataFileL(aCoreUid, aIndexNumber);	
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::MappingDataForKey
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPtiEngine::MappingDataForKey(TPtiKey aKey, TDes& aResult, TPtiTextCase aCase)
	{
	iImpl->MappingDataForKey(aKey, aResult, aCase);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetNumericModeKeysForQwertyL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPtiEngine::GetNumericModeKeysForQwertyL(TInt aLanguage, RArray<TPtiNumericKeyBinding>& aResult)
	{
	iImpl->GetNumericModeKeysForQwertyL(aLanguage, aResult);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetCandidatesByInputString
// 
// ---------------------------------------------------------------------------
//	
EXPORT_C HBufC* CPtiEngine::GetCandidatesByInputString(const TDesC& aInputString, 
                                                       RPointerArray<HBufC>& aList,
                                                       const TBool aIsPredictive)
    {
    return iImpl->GetCandidatesByInputString(aInputString, aList, aIsPredictive);
    }
    
    
// ---------------------------------------------------------------------------
// CPtiEngine::GetHwrRecognizerL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C MPtiHwrRecognizer* CPtiEngine::GetHwrRecognizerL(TLanguage aLanguage)
    {
    return iImpl->GetHwrRecognizerL(aLanguage);
    }
    
    
// ---------------------------------------------------------------------------
// CPtiEngine::GetHwrRecognizerL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C MPtiHwrRecognizer* CPtiEngine::GetHwrRecognizerL(TInt aImpId)
    {
    return iImpl->GetHwrRecognizerL(aImpId);
    }
   
    
// ---------------------------------------------------------------------------
// CPtiEngine::ListHwrRecognizerL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C RArray<TUid>& CPtiEngine::ListHwrRecognizerL(TLanguage aLanguage)
    {
    return iImpl->ListHwrRecognizerL(aLanguage);
    }


// ---------------------------------------------------------------------------
// CPtiEngine::GetHwrAvailableLanguagesL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CPtiEngine::GetHwrAvailableLanguagesL(RArray<TInt>& aResult)
    {
    iImpl->GetHwrAvailableLanguagesL(aResult);
    }


// ---------------------------------------------------------------------------
// CPtiEngineImpl::ExtendedNumericModeKeysForQwertyL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C const RArray<TPtiNumericKeyBinding>& CPtiEngine::ExtendedNumericModeKeysForQwertyL(TInt aLanguage)
	{
	return iImpl->ExtendedNumericModeKeysForQwertyL(aLanguage);
	}


// ---------------------------------------------------------------------------
// CPtiEngine::SetAutoSubstitution
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::SetAutoSubstitution(TBool aStatus)
    {
    return iImpl->SetAutoSubstitution(aStatus);    
    }


// ---------------------------------------------------------------------------
// CPtiEngine::AddAutoSubstitutionEntry
// 
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt CPtiEngine::AddAutoSubstitutionEntry(const TDesC& aShortcut,
                                                   const TDesC& aSubstituition)
    {
    return iImpl->AddAutoSubstitutionEntry(aShortcut, aSubstituition);
    }

     
// ---------------------------------------------------------------------------
// CPtiEngine::DeleteAutoSubstitutionEntry
// 
// ---------------------------------------------------------------------------
//      
EXPORT_C TInt CPtiEngine::DeleteAutoSubstitutionEntry(const TDesC& aShortcut)
    {
    return iImpl->DeleteAutoSubstitutionEntry(aShortcut);
    }

     
// ---------------------------------------------------------------------------
// CPtiEngine::NumberOfAutoSubstitutionEntries
// 
// ---------------------------------------------------------------------------
//            
EXPORT_C TInt CPtiEngine::NumberOfAutoSubstitutionEntries() const
    {
    return iImpl->NumberOfAutoSubstitutionEntries();
    }

     
// ---------------------------------------------------------------------------
// CPtiEngine::GetAutoSubstitutionEntry
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::GetAutoSubstitutionEntry(TInt aIndex, TDes& aShortcut,
                                                   TDes& aSubstitution)
    {
    return iImpl->GetAutoSubstitutionEntry(aIndex, aShortcut, aSubstitution);
    }
    
    
// ---------------------------------------------------------------------------
// CPtiEngine::KeyboardType
// 
// ---------------------------------------------------------------------------
//                                                                                    
EXPORT_C TPtiKeyboardType CPtiEngine::KeyboardType() const
    {
    return iImpl->KeyboardType();
    }
     
    
// ---------------------------------------------------------------------------
// CPtiEngine::KeyboardType
// 
// ---------------------------------------------------------------------------
//                           
EXPORT_C TInt CPtiEngine::SetKeyboardType(TPtiKeyboardType aType)
    {
    return iImpl->SetKeyboardType(aType);
    }


// ---------------------------------------------------------------------------
// CPtiEngine::IsQwertyBasedMode
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiEngine::IsQwertyBasedMode(TPtiEngineInputMode aMode) const
    {
    if ((aMode == EPtiEngineQwerty) ||
        (aMode == EPtiEngineZhuyinQwerty) ||
        (aMode == EPtiEngineZhuyinPhraseQwerty) ||
        (aMode == EPtiEnginePinyinQwerty) ||
        (aMode == EPtiEnginePinyinPhraseQwerty) ||
        (aMode == EPtiEngineStrokeQwerty) ||
        (aMode == EPtiEngineStrokePhraseQwerty) ||        
        (aMode == EPtiEngineNormalCangjieQwerty) ||
        (aMode == EPtiEngineEasyCangjieQwerty) ||
        (aMode == EPtiEngineAdvCangjieQwerty) ||
        (aMode == EPtiEngineHiraganaKanjiQwerty) ||
        (aMode == EPtiEngineHiraganaKanjiPredictiveQwerty) ||
        (aMode == EPtiEngineKatakanaQwerty) ||
        (aMode == EPtiEngineFullWidthKatakanaQwerty) ||
        (aMode == EPtiEngineFullWidthNumericQwerty) ||
        (aMode == EPtiEngineFullWidthAlphabetQwerty) ||
        (aMode == EPtiEngineHiraganaQwerty) ||
        (aMode == EPtiEngineQwertyPredictive)    
#ifdef RD_HINDI_PHONETIC_INPUT
        || (aMode == EPtiEngineInputModeIndicPhoneticQwerty)
#endif
#ifdef RD_INTELLIGENT_TEXT_INPUT
        || (aMode == EPtiEnginePinyinPhraseHalfQwerty)
        || (aMode == EPtiEngineStrokePhraseHalfQwerty)
        || (aMode == EPtiEngineZhuyinPhraseHalfQwerty)
        || (aMode == EPtiEngineInputModeZhuyinMiniQwertyFind)
        || (aMode == EPtiEngineQwertyKorean)
#endif
        || (aMode == EPtiEngineNormalCangjieQwerty)
        || (aMode == EPtiEngineEasyCangjieQwerty)
        || (aMode == EPtiEngineAdvCangjieQwerty))
        {
        return ETrue;   
        }
        
    return EFalse;
    }      


// ---------------------------------------------------------------------------
// CPtiEngine::KeyboardTypesSupportedByLanguage
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPtiEngine::KeyboardTypesSupportedByLanguageL(TInt aLanguage,
                                                            RArray<TPtiKeyboardType>& aResult)
    {
    iImpl->KeyboardTypesSupportedByLanguageL(aLanguage, aResult);
    }


// ---------------------------------------------------------------------------
// CPtiEngine::GetNumericModeKeysForQwertyL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPtiEngine::GetNumericModeKeysForQwertyL(TInt aLanguage,
 	                                            RArray<TPtiNumericKeyBinding>& aResult,
 	                                            TPtiKeyboardType aKeyboardType)
    {
    iImpl->GetNumericModeKeysForQwertyL(aLanguage, aResult, aKeyboardType);
    }
  
  
// ---------------------------------------------------------------------------
// CPtiEngine::ExtendedNumericModeKeysForQwertyL
// 
// ---------------------------------------------------------------------------
//   
EXPORT_C const RArray<TPtiNumericKeyBinding>& CPtiEngine::ExtendedNumericModeKeysForQwertyL(TInt aLanguage,
                                                                                     TPtiKeyboardType aKeyboardType)
    {
    return iImpl->ExtendedNumericModeKeysForQwertyL(aLanguage, aKeyboardType);
    }
    
    
// ---------------------------------------------------------------------------
// CPtiEngine::ListAvailablePhysicalKeyboardsL
// 
// ---------------------------------------------------------------------------
//	
EXPORT_C void CPtiEngine::ListAvailablePhysicalKeyboardsL(RArray<TPtiKeyboardType>& aResult)
    {
  	// Add real implementation later.	
    User::LeaveIfError(aResult.Append(EPtiKeyboard12Key));        	    
    }    
    
        
// ---------------------------------------------------------------------------
// CPtiEngine::SetNumberCandidateStatus
// 
// ---------------------------------------------------------------------------
//	    
EXPORT_C TInt CPtiEngine::SetNumberCandidateStatus(TBool aStatus)
    {
    return iImpl->SetNumberCandidateStatus(aStatus);
    }


// ---------------------------------------------------------------------------
// CPtiEngine::SetNumberCandidateStatus
// 
// ---------------------------------------------------------------------------
//	    
EXPORT_C TBool CPtiEngine::IsValidKey(TPtiKey aKey) const
    {
    return iImpl->IsValidKey(aKey);
    }
    
    
// ---------------------------------------------------------------------------
// CPtiEngine::SetNumberCandidateStatus
// 
// ---------------------------------------------------------------------------
//	        
EXPORT_C TInt CPtiEngine::SetMaxLengthForAutoCompletedCandidates(TInt aMaxLength)
   {
   return iImpl->SetMaxLengthForAutoCompletedCandidates(aMaxLength);	
   }
      
      
// ---------------------------------------------------------------------------
// CPtiEngine::MarkAutoCapitalized()
// 
// ---------------------------------------------------------------------------
//	         
EXPORT_C void CPtiEngine::MarkAutoCapitalized()
    {
    iImpl->MarkAutoCapitalized();
    }
    

// ---------------------------------------------------------------------------
// CPtiEngine::CaseSequence()
// 
// ---------------------------------------------------------------------------
//	 
EXPORT_C TPtrC8 CPtiEngine::CaseSequence()
    {
    return iImpl->CaseSequence();
    }
    
    
// ---------------------------------------------------------------------------
// CPtiEngine::AddPhrase()
// 
// ---------------------------------------------------------------------------
//	    
EXPORT_C TInt CPtiEngine::AddPhrase(const TDesC& aNewPhrase)
    {
    return iImpl->AddPhrase(aNewPhrase);
    }
    
#ifdef FF_DUAL_LANGUAGE_SUPPORT
// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetSecondaryInputL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiEngine::SetSecondaryInputL(TInt aEpocLanguageID)
    {
    return iImpl->SetSecondaryInputL(aEpocLanguageID);
    }
#endif //FF_DUAL_LANGUAGE_SUPPORT
// End of file
