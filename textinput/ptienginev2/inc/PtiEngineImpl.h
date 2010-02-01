/*
* Copyright (c) 2003-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Predective text input engine implementation class header
*
*/



#ifndef _PTI_ENGINE_IMPL_H
#define _PTI_ENGINE_IMPL_H

// INCLUDES
#include <e32base.h>
#include <badesca.h> 
#include "PtiDefs.h"
#include "PtiObserver.h"

// FORWARD DECLARATIONS
class CPtiCore;
class MPtiLanguage;
class CPtiCoreLanguage;
class CPtiUserDictionary;
class MPtiUserDictionary;
class MPtiEngineCompositionDataInterface;
class MPtiUserDictionaryEntry;
class CPtiEngine;
class CPtiMappings;
class MPtiHwrRecognizer;
class CHwrRecognizer;
class CPtiHwrRecognizer;
class MPtiKeyMappings;
const TInt KPtiMaxKeymappingsLength = 32;
/**
* CPtiEngineImpl class.
* Implementation class for main PtiEngine API. See file PtiEngine.h for actual documentation.
*/
NONSHARABLE_CLASS(CPtiEngineImpl) : public CBase
	{
	public:
	    /**
		* Two phase constructor.
		*
		* @param  aUseDefaultUserDictionary
		* @return 
		*/
		static CPtiEngineImpl* NewL(CPtiEngine* aFrontEnd);		

		/**
		* Destructor.
		*/
		~CPtiEngineImpl();

        // The PtiEngine API.
        // See the description of corresponding method in PtiEngine.h
		TInt ActivateLanguageL(TInt aEpocLanguageID, TPtiEngineInputMode aMode);
		void CloseCurrentLanguageL();
		IMPORT_C MPtiCoreInfo* CoreInfo(TPtiEngineInputMode aMode) const;
		inline MPtiLanguage* CurrentLanguage();
		MPtiLanguage* GetLanguage(TInt aCode) const;
		TInt NumberOfCandidates();
		TPtrC AppendKeyPress(TPtiKey aKey);
		TPtrC DeleteKeyPress();
		TInt AddCoreL(const TUid aCoreUid, TBool aUseDefaultUserDictionary);
		TInt GetCandidateListL(CDesCArray& aList);
		TInt GetNextWordCandidateListL(CDesCArray& aList);
		TPtrC FirstCandidate();		
		TPtrC NextCandidate();
		TPtrC PreviousCandidate();
		TInt SetInputMode(TPtiEngineInputMode aMode);
		inline TPtiEngineInputMode InputMode() const;
		TInt SetReordering(TBool aStatus);
		TInt SetCurrentWord(TPtrC aWord);
		inline TPtrC CurrentWord();
		void ClearCurrentWord();
		inline void SetCase(TPtiTextCase aCase);
		inline TPtiTextCase Case() const;	
		void GetAvailableLanguagesL(CArrayFix<TInt>* aResult);
		void GetAvailableLanguagesL(RArray<TInt>& aResult);
		inline TInt NumberOfLanguages() const;
		TInt CreateUserDictionaryL(TDesC& aFileName, CDesCArrayFlat* aWords,
		                           TPtiEngineInputMode aMode); 
		MPtiUserDictionary* AttachUserDictionaryL(TDesC& aFileName);	
		MPtiUserDictionary* AttachDefaultUserDictionaryL(TUid aCoreUid, TInt aSymbolClass);		
		TInt DetachUserDictionary(TDesC& aFileName);
		TInt DetachUserDictionary(TInt aId);
		void GetLocalizedLanguageNameL(TInt aLangCode, TDes& aResult);
		TInt CommitCurrentWord();
		TInt CharConversion(TPtiCharConversion aType,
		               TAny* aInput,
		  			   TInt aInputLength,
					   TAny* aOutput);
		TUint32 AvailableCharConversions() const;
		TInt SetExternalKeyMapL(TPtiEngineInputMode aMode,
		                        TPtiKey aKey,
						  	    TDesC& aKeyMap,
								TPtiTextCase aCase);
		inline TPtiKey LastEnteredKey() const;
		inline TPtrC8 CurrentInputSequence() const;
		TInt GetSpelling(TUint16 aInput, TDes& aOutput, TPtiSpelling aType);
		TInt CancelTimerActivity();
		TPtiKey CharacterToKey(TUint16 aChar);
		TInt AddUserDictionaryEntry(MPtiUserDictionaryEntry& aEntry);
		TInt AddUserDictionaryEntry(MPtiUserDictionaryEntry& aEntry, TInt aId);
		TInt RemoveEntryFromUserDictionary(MPtiUserDictionaryEntry& aEntry);
		TInt RemoveEntryFromUserDictionary(MPtiUserDictionaryEntry& aEntry, TInt aId);
		TInt NumberOfEntriesInUserDictionary();
		TInt GetUserDictionaryEntry(TInt aIndex, MPtiUserDictionaryEntry& aResult);
		MPtiUserDictionary* DefaultUserDictionary(TPtiEngineInputMode aMode);
		inline void SetObserver(MPtiObserver* aObserver);
		inline MPtiObserver* Observer();
		TInt HandleCommandL(TPtiEngineCommand aCommand, TAny* aParams);
		TPtrC CandidatePage();
		TBool NextCandidatePage();
		TBool PreviousCandidatePage();
		TBool MoreCandidatePages();
		void SetCandidatePageLength(TInt aLength);
	    TPtrC GetPhoneticSpelling(TInt aIndex) const; 
		TInt PhoneticSpellingCount() const;	
	    TBool SelectPhoneticSpelling(TInt aIndex);
	    TInt SelectedPhoneticSpelling() const; 
		void EnableToneMarks(TBool aValue);
		void ResetToneMark();
		TBool ToneMark(TText& aToneMark) const;
		TBool IsToneMarkValidForSpelling() const;
		TBool IncrementToneMark(TBool aOverrideInvalid);
		TBool SetPredictiveChineseChar(const TDesC& aChar);
		MPtiEngineCompositionDataInterface* CompositionData();
		TPtrC ReadingTextL();
		void GetModeNameIndexL(TPtiChineseVariant aVariant, RArray<TInt>& aResult);
		TInt GetPhoneticSpellingsL(CDesCArray& aList);
		TInt GetChinesePhraseCandidatesL(CDesCArray& aList);
		TBool SetToneMark(TInt aToneMark);	
		void CreateDefaultUserDictionaryFileL(TUid aCoreUid, TInt aSymbolClass);					
		void WriteSecondaryDataFileL(TUid aCoreUid, TInt aIndexNumber, HBufC8* aData);		
		HBufC8* ReadSecondaryDataFileL(TUid aCoreUid, TInt aIndexNumber);			
		void MappingDataForKey(TPtiKey aKey, TDes& aResult, TPtiTextCase aCase);		
		void GetNumericModeKeysForQwertyL(TInt aLanguage, RArray<TPtiNumericKeyBinding>& aResult);                        								
    IMPORT_C HBufC* GetCandidatesByInputString(const TDesC& aInputString, 
                                                RPointerArray<HBufC>& aList, 
                                                const TBool aIsPredictive);		
		void InitializeL(TBool aUseDefaultUserDictionary);
        MPtiHwrRecognizer* GetHwrRecognizerL(TLanguage aLanguage);
        MPtiHwrRecognizer* GetHwrRecognizerL(TInt aImpId);		   
        RArray<TUid>& ListHwrRecognizerL(TLanguage aLanguage);
        void GetHwrAvailableLanguagesL(RArray<TInt>& aResult);

 	    const RArray<TPtiNumericKeyBinding>& ExtendedNumericModeKeysForQwertyL(TInt aLanguage);
        TInt SetAutoSubstitution(TBool aStatus); 
        TInt AddAutoSubstitutionEntry(const TDesC& aShortcut, const TDesC& aSubstituition);     
        TInt DeleteAutoSubstitutionEntry(const TDesC& aShortcut);     
        TInt NumberOfAutoSubstitutionEntries() const;     
        TInt GetAutoSubstitutionEntry(TInt aIndex, TDes& aShortcut,
                                      TDes& aSubstitution);                                           
        TPtiKeyboardType KeyboardType() const;     
        TInt SetKeyboardType(TPtiKeyboardType aType);   
        void KeyboardTypesSupportedByLanguageL(TInt aLanguage,
                                               RArray<TPtiKeyboardType>& aResult);                       
        void GetNumericModeKeysForQwertyL(TInt aLanguage,
 	                                      RArray<TPtiNumericKeyBinding>& aResult,
 	                                   TPtiKeyboardType aKeyboardType);                                                                             
        const RArray<TPtiNumericKeyBinding>& ExtendedNumericModeKeysForQwertyL(TInt aLanguage,                                                                            TPtiKeyboardType aKeyboardType);
        TInt SetNumberCandidateStatus(TBool aStatus);                                                                               
  	    TBool IsValidKey(TPtiKey aKey) const;
  	    TInt SetMaxLengthForAutoCompletedCandidates(TInt aMaxLenght); 
        void MarkAutoCapitalized(); 
        inline TPtrC8 CaseSequence();   
        TInt AddPhrase(const TDesC& aNewPhrase);                	                                                                                                
#ifdef FF_DUAL_LANGUAGE_SUPPORT
        TInt SetSecondaryInputL(TInt aEpocLanguageID);
        MPtiKeyMappings* GetCombinedKeyMappings(TPtiKeyboardType aKeyboardType);
#endif //FF_DUAL_LANGUAGE_SUPPORT
                                        
	private:
		enum
			{
			KPtiMaxWordSize = 255
			};
						
	private:
		CPtiEngineImpl();
		void ConstructL(CPtiEngine* aFrontEnd);
		CArrayFixFlat<TPtiLangName>* CreateLanguageNamesListLC();
		void FindLanguageName(CArrayFixFlat<TPtiLangName>*aList, TDes& aResult, TInt aLangCode);
		void RefreshLanguageNamesL();
		inline TBool IsCoreForInputMode(TPtiEngineInputMode aMode) const;
		inline TBool IsCore() const;
		inline CPtiCore* Core() const;
		inline CPtiCore* CoreForInputMode(TPtiEngineInputMode aMode) const;		
		void DetachUserDictionaries();
		CPtiCore* FindCoreByUid(TInt32 aUid);
		void ConvertToKeySequence(TPtrC aIn, TDes8& aOut);
		inline void ClearBuffers();
		void Capitalize(TDes& aTextBuffer);
		void MakeCaseSequence(TPtrC aWord);
		void LoadCoresInDefaultDirL(TBool aUseDefaultUserDictionary);
		CPtiUserDictionary* DefaultUserDictForCore(CPtiCore* aCore);
		TInt ReadUidFromUserDictionaryFileL(const TDesC& aFileName);
		CPtiCoreLanguage* CreateNumericLanguageL(CPtiCore* aCore);
		inline TBool IsChinese() const;
		TInt CountToneMarks(TPtrC aSpelling) const;
		inline TBool SupportsUserDictionaries(CPtiCore* aCore);
		inline TBool IsToneMarkOrSeparator(TInt aChar) const;
		inline TBool IsModeChineseQwerty(TPtiEngineInputMode aMode) const;
		TPtrC RedirectKeyForChineseQwerty(TPtiKey aKey, TBool& aRedirected);
		void CreateUserDictName(TDes& aResult, TUid aCoreUid, TInt aSymbolClass);	
		void CreateSecondaryDataFileName(TDes& aResult, TUid aCoreUid, TInt aIndex);				
        TBool MapNumericChar(CPtiMappings* aMaps, TPtiNumericKeyBinding& aBinding,
                             TPtiKey aKey, TPtiTextCase aCase, TBool aUseExtendedSet,
                             TPtiEngineInputMode aMode);	
   		inline TBool AllowCapitalize(TUint16 aChar) const;
		void NumericModeKeysForQwertyL(TInt aLanguage,
                                       RArray<TPtiNumericKeyBinding>& aResult,
                                       TBool aUseExtendedSet,
                                       TPtiKeyboardType aKeyboardtype = EPtiKeyboardNone);	
        TPtiKeyboardType ActiveKeyboardTypeL();
        TPtiKeyboardType FindFirstAvailableQwertyKeyboardType(TInt aLang) const;                                              	
        TPtiTextCase CaseForMappedCharacter(TChar aCharacter);
        void RemoveTrailingPunctuations();
        void RemoveTrailingPunctuations12Key();
#ifdef FF_DUAL_LANGUAGE_SUPPORT
        void  SearchKeySequenceInSecLang(TPtrC aIn, TDes8& aOut);
#endif // FF_DUAL_LANGUAGE_SUPPORT        
	private:	
	    /*
	    * Pointer to PtiEngine front end.
	    */
		CPtiEngine*              iFrontEnd;
		
		/*
		* Primary text buffer.
		*/
		TBuf<KPtiMaxWordSize>    iTextBuffer;
		
		TBuf<KPtiMaxKeymappingsLength>    iPunctuationCharacters;
		
		/*
		* Input sequence buffer. Holds scan codes for
		* each key press for current word.
		*/
		TBuf8<KPtiMaxWordSize>   iInputSequence;
		
		/*
		* Case buffer. Holds the shift status for each key press.
		*/
		TBuf8<KPtiMaxWordSize>   iCaseBuffer;
		
		/*
		* Current input mode.
		*/
		TPtiEngineInputMode      iInputMode;	
		
		/*
		* Pointer to currently active language.
		*/	
		CPtiCoreLanguage*        iCurrentLanguage;
		
		/*
		* Current text case.
		*/
		TPtiTextCase             iCase;
		
		/*
		* The language list. 
		*/
		CArrayPtrFlat<CPtiCoreLanguage>* iLanguages;
		
		/*
		* Candidate buffer.
		*/
		TBuf<KPtiMaxWordSize>    iCandidateBuffer;
		
		/*
		* Core object array.
		*/ 
		CArrayPtrFlat<CPtiCore>* iCores;

        /*
        * User dictionary array.
        */
		RPointerArray<CPtiUserDictionary> iUserDictionaries;		
		
		/*
		* Pointer to registered PtiEngine observer.
		*/
		MPtiObserver*            iObserver;
			
		/* 
		* Language for current extended numeric mode set.
		*/	
		TInt                     iExtendedNumricSetLang;
		
		/*
		* Extended numeric mode characters. 
		*/
		RArray<TPtiNumericKeyBinding> iExtendedNumricSet;

        /**
        * The array used to store hwr implementation uid list
        */      
        RArray<TUid> iImpIdList; 
		
        /**
        * The pointer points current active hwr implementation
        */
        CPtiHwrRecognizer* iCurrentHwrImpl;                
#ifdef FF_DUAL_LANGUAGE_SUPPORT
        /*
        * Pointer to currently active language.
        */  
        CPtiCoreLanguage*        iSecondaryLanguage;
        
        /**
        * The pointer used to store current combined keymapping class
        */
        MPtiKeyMappings* iCombinedKeyMappings;
#endif //FF_DUAL_LANGUAGE_SUPPORT
#ifdef RD_INTELLIGENT_TEXT_INPUT
        /*
        * Pointer to currently active language.
        */        
        TBuf16<4>     iZhuyinSymbols;
        /*
        * current key inputted.
        */ 
        TPtiKey       iKeyMuliTap;
        /*
        * index in zhuyin symbol array for mini qwerty zhuyin multi-tap .
        */ 
        TInt8         iIndex;
#endif //RD_INTELLIGENT_TEXT_INPUT

        /*
         * When cursor position in editor crossed max length in editor then set this flag
        */
        TBool iMaxEditorLengthCrossed;
        /*
         * Keeps count of the punctuation keys entered simultaneously
         */
        TInt iPunctuationsEntered;
        
        /*
         * Keeps track of the location of start of punct symbol
         */
        TInt iPunctuationLocation;
	};

#include "PtiEngine.inl"

#endif  _PTI_ENGINE_H

// End of file
