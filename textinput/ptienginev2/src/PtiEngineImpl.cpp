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


#include <f32file.h>
#include <barsc.h>
#include <barsread.h> 

#include "PtiEngineImpl.h"
#include "PtiDefaultCore.h"
#include "PtiLanguage.h"
#include <ptiengine.rsg>
#include "PtiUserDicEntry.h"
#include "PtiEngine.h"
#include "PtiKeyMappings.h"
#include <ecom/ecom.h>
#include <PtiHwrRecognizer.h>
#include <PtiKeyMapData.h>

#include <data_caging_path_literals.hrh> 
#ifdef RD_HINDI_PHONETIC_INPUT
#include <Languages.hrh>
#endif
#include <AknUtils.h>
#include <e32property.h>
#include <AknFepInternalCRKeys.h>
#include <AvkonInternalCRKeys.h>

const TInt KDefaultLanguageGran = 5;
const TInt KDefaultLangNamesGran = 40;
const TInt KPhraseSeparator = 0x0020;
const TInt KMaxPhraseBufferSize = 255;
const TInt KMaxCandPages = 100;

const TInt KPtiChineseHash = 0xff03;
const TInt KPtiChineseStar = 0xff0a;
const TInt KPtiChinesePlus = 0xff0b;

const TInt KUserDictionaryMaxSize = 32768; 

const TInt KDirectlyAllowedScanCodesStart = 0x0021;
const TInt KDirectlyAllowedScanCodesEnd = 0x005f;

// This is needed for resource reading.
const TInt KCCMask(0x00000fff);

const TPtiKey qwertyKeys[] =
	{
	EPtiKeyQwerty1, EPtiKeyQwerty2, EPtiKeyQwerty3, EPtiKeyQwerty4, EPtiKeyQwerty5,
    EPtiKeyQwerty6, EPtiKeyQwerty7, EPtiKeyQwerty8, EPtiKeyQwerty9, EPtiKeyQwerty0,
#if defined(__WINS__)
	(TPtiKey)EStdKeyNkpPlus,  // Emulator produces different scancode for plus key than real hardware.
#else    
    EPtiKeyQwertyPlus, 
#endif    
    EPtiKeyQwertyMinus, EPtiKeyQwertyComma, EPtiKeyQwertySemicolon,EPtiKeyQwertyChr,EPtiKeyQwertyLeftShift,
    EPtiKeyQwertyFullstop, EPtiKeyQwertyHash, EPtiKeyQwertySlash, EPtiKeyQwertyApostrophe,    
    EPtiKeyQwertySpace,  EPtiKeyQwertyA, EPtiKeyQwertyB, EPtiKeyQwertyC, EPtiKeyQwertyD,
    EPtiKeyQwertyE, EPtiKeyQwertyF, EPtiKeyQwertyG, EPtiKeyQwertyH, EPtiKeyQwertyI,
    EPtiKeyQwertyJ, EPtiKeyQwertyK, EPtiKeyQwertyL, EPtiKeyQwertyM, EPtiKeyQwertyN,
    EPtiKeyQwertyO, EPtiKeyQwertyP, EPtiKeyQwertyQ, EPtiKeyQwertyR, EPtiKeyQwertyS,
    EPtiKeyQwertyT, EPtiKeyQwertyU, EPtiKeyQwertyV, EPtiKeyQwertyW, EPtiKeyQwertyX,
    EPtiKeyQwertyY, EPtiKeyQwertyZ, EPtiKeyQwertyAtCharacter, (TPtiKey)0
	};	

	
// Currently Japanese doesn't have real qwerty keymappings data file, but
// GetNumericModeKeysForQwertyL needs to work also for ELangJapanese. We'll handle
// it as a hardcoded special case for now.
const TInt KNumBindingsForNonLatinNumberLanguages = 15;
const TInt KNumExtendedBindingsForNonLatinNumberLanguages = 22;
const TPtiNumericKeyBinding NumericModeKeysForNonLatinNumberLanguages[] =
	{
	{'0', EPtiKeyQwerty0, EPtiCaseLower},
	{'1', EPtiKeyQwerty1, EPtiCaseLower},
	{'2', EPtiKeyQwerty2, EPtiCaseLower},
	{'3', EPtiKeyQwerty3, EPtiCaseLower},
	{'4', EPtiKeyQwerty4, EPtiCaseLower},
	{'5', EPtiKeyQwerty5, EPtiCaseLower},
	{'6', EPtiKeyQwerty6, EPtiCaseLower},
	{'7', EPtiKeyQwerty7, EPtiCaseLower},
	{'8', EPtiKeyQwerty8, EPtiCaseLower},
	{'9', EPtiKeyQwerty9, EPtiCaseLower},
	{'p', EPtiKeyQwertyP, EPtiCaseLower},
	{'w', EPtiKeyQwertyW, EPtiCaseLower},
	{'+', EPtiKeyQwertyPlus, EPtiCaseLower},
	{'#', EPtiKeyQwerty3, EPtiCaseUpper},
	{'*', EPtiKeyQwertyApostrophe, EPtiCaseUpper},
	{';', EPtiKeyQwertySemicolon, EPtiCaseLower},
	{'.', EPtiKeyQwertyFullstop, EPtiCaseLower},
	{',', EPtiKeyQwertyComma, EPtiCaseLower},
	{'-', EPtiKeyQwertyMinus, EPtiCaseLower},			
	{'E', EPtiKeyQwertyE, EPtiCaseUpper},
	{'?', EPtiKeyQwertySlash, EPtiCaseUpper},
	{'/', EPtiKeyQwertySlash, EPtiCaseLower}	
	};	


// Resource file name.
_LIT(KCEResourceFile, "z:PtiEngine.rsc");
_LIT(KNumericLangName, "Numeric");
_LIT(KPtiDefaultUdbPath, "c:\\Predic\\");
_LIT(KAllowedNumericChars, "0123456789pw+#*");
_LIT(KAllAllowedNumericChars, "*+pw#1234567890;.,-E?/");
_LIT(KExtendedNumericModeChars, ";.,-E?/");
#ifdef _DEBUG
_LIT(KPtiEngineImplPanic, "PtiEngineImpl");
#endif

// CONSTANTS
const TInt16 KStrokeHorizontalValue = 0x4e00; 
const TInt16 KStrokeVerticalValue = 0x4e28;
const TInt16 KStrokeDownToLeftValue = 0x4e3f;
const TInt16 KStrokeDownToRightValue = 0x4e36;
const TInt16 KStrokeBendingValue = 0x4e5b;
const TInt16 KStrokeQuestionValue = 0x003f;
const TInt16 KStrokeUnicode = 0x2461;
const TInt16 KZhuyinIndicator = 0x2462;

// Local method declarations.
LOCAL_C TInt RemapVietnameseAccentedCharacter(TUint16 aChr);
	
//
// CPtiEngineImpl implementation
//

// ---------------------------------------------------------------------------
// CPtiEngineImpl::NewL
// 
// ---------------------------------------------------------------------------
//
CPtiEngineImpl* CPtiEngineImpl::NewL(CPtiEngine* aFrontEnd)
	{
	CPtiEngineImpl *engine = new (ELeave) CPtiEngineImpl();
	CleanupStack::PushL( engine );
	engine->ConstructL(aFrontEnd);
	CleanupStack::Pop(); // engine
	return engine;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CPtiEngineImpl
// 
// ---------------------------------------------------------------------------
//
CPtiEngineImpl::CPtiEngineImpl()
	{	
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::~CPtiEngineImpl
// 
// ---------------------------------------------------------------------------
//
CPtiEngineImpl::~CPtiEngineImpl()
	{	
	iExtendedNumricSet.Close();
	
	DetachUserDictionaries();
	iUserDictionaries.ResetAndDestroy();	
	
	if (iCores)
		{
		iCores->ResetAndDestroy();
		delete iCores;
		}
	if (iLanguages)
		{
		iLanguages->ResetAndDestroy();
		delete iLanguages;
		}
		
	REComSession::FinalClose();		
	
	delete iCurrentHwrImpl;
	iImpIdList.Close();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::ConstructL
// 
// ---------------------------------------------------------------------------
//
void CPtiEngineImpl::ConstructL(CPtiEngine* aFrontEnd)
	{
	iFrontEnd = aFrontEnd;
	iLanguages = new( ELeave ) CArrayPtrFlat<CPtiCoreLanguage>( KDefaultLanguageGran ); 
	iCores = new (ELeave) CArrayPtrFlat<CPtiCore>( KDefaultCoresGran );
    TBuf<KPtiMaxWordSize > mapChars;
	mapChars.Zero();
	iPunctuationCharacters.Zero();
    MappingDataForKey((TPtiKey)EStdKeyFullStop,mapChars,EPtiCaseLower);
    iPunctuationCharacters.Copy(mapChars);
    MappingDataForKey((TPtiKey)EStdKeyFullStop,mapChars,EPtiCaseUpper);
    iPunctuationCharacters.Append(mapChars);
    MappingDataForKey((TPtiKey)EStdKeyFullStop,mapChars,EPtiCaseFnLower);
    iPunctuationCharacters.Append(mapChars);
    MappingDataForKey((TPtiKey)EStdKeyFullStop,mapChars,EPtiCaseFnUpper);
    iPunctuationCharacters.Append(mapChars);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::InitializeL
// 
// ---------------------------------------------------------------------------
//
void CPtiEngineImpl::InitializeL(TBool aUseDefaultUserDictionary)
	{
	CPtiCore* mtCore = CPtiDefaultCore::NewL(iFrontEnd, &iTextBuffer);
	CleanupStack::PushL(mtCore);
	iCores->AppendL( mtCore );
	CleanupStack::Pop(); // mtCore
	mtCore->InitializeL(iFrontEnd, &iTextBuffer, iLanguages, aUseDefaultUserDictionary);

	CPtiCoreLanguage* numLang = CreateNumericLanguageL(mtCore);
	CleanupStack::PushL(numLang);
	iLanguages->AppendL(numLang);
	CleanupStack::Pop(); // numLang

	LoadCoresInDefaultDirL(aUseDefaultUserDictionary);

	RefreshLanguageNamesL();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetLanguage
// 
// ---------------------------------------------------------------------------
//
MPtiLanguage* CPtiEngineImpl::GetLanguage(TInt aCode) const
	{
	const TInt count = iLanguages->Count();
	for (TInt i = 0; i < count; i++)
		{
		if (iLanguages->At(i)->LanguageCode() == aCode)
			{
			return iLanguages->At(i);
			}	
		}

	return NULL;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetAvailableLanguagesL
// 
// ---------------------------------------------------------------------------
//
void CPtiEngineImpl::GetAvailableLanguagesL(CArrayFix<TInt>* aResult)
	{
	if (aResult)
		{		
		aResult->Reset();

		const TInt count = iLanguages->Count();
#ifdef RD_HINDI_PHONETIC_INPUT
              TBool isIndicPhoneticInputPresent = EFalse;
              for (TInt i = 0; i < count; i++)
			{
			if (iLanguages->At(i)->LanguageCode() == KLangHindiPhonetic) 
				{			
				isIndicPhoneticInputPresent = ETrue;
				}
			}
#endif
		for (TInt i = 0; i < count; i++)
			{
			if ((iLanguages->At(i)->LanguageCode() != ELangNumeric)
#ifdef RD_HINDI_PHONETIC_INPUT
				&& (iLanguages->At(i)->LanguageCode() != KLangHindiPhonetic)
#endif
                )
				{
				aResult->AppendL(iLanguages->At(i)->LanguageCode());
#ifdef RD_HINDI_PHONETIC_INPUT
				if (iLanguages->At(i)->LanguageCode() == ELangHindi && isIndicPhoneticInputPresent)
					aResult->AppendL(KLangHindiPhonetic);
#endif
				}
			}
		}
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CreateUserDictionaryL
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::CreateUserDictionaryL(TDesC& aFileName, CDesCArrayFlat* aWords, TPtiEngineInputMode aMode) 
	{
	CPtiCore* core = CoreForInputMode( aMode );
	if (!SupportsUserDictionaries(core))   
		{
		return KErrNotSupported;
		}

	CPtiUserDictionary* ud = core->CreateEmptyUserDictionaryL(KUserDictionaryMaxSize);
	CleanupStack::PushL(ud);
	ud->SetFileName( aFileName );
	ud->SetCoreUID(core->GetCoreInfo()->Uid());

	TInt error = core->AttachUserDictionary( ud );
	if (error != KErrNone)
		{
		CleanupStack::PopAndDestroy(); // ud
		return error;
		}

	User::LeaveIfError(iUserDictionaries.Append(ud));
	
	CleanupStack::Pop(); // ud

	for (TInt i = 0; i < aWords->Count(); i++)
		{
		TPtiUserDictionaryEntry entry(aWords->MdcaPoint(i));
		error = core->AddUserDictionaryEntry(entry, ud);		
		if (error != KErrNone)
			{
			return error;
			}
		}
	
	return KErrNone;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::AttachUserDictionaryL
// 
// ---------------------------------------------------------------------------
//
MPtiUserDictionary* CPtiEngineImpl::AttachUserDictionaryL(TDesC& aFileName)
	{
	TInt coreUid = ReadUidFromUserDictionaryFileL(aFileName);

	CPtiUserDictionary* ud = NULL;
	CPtiCore *core = FindCoreByUid(coreUid);
	if (!SupportsUserDictionaries(core))
		{		
		return NULL;
		}

	ud = core->CreateEmptyUserDictionaryL(KUserDictionaryMaxSize);
	CleanupStack::PushL(ud);
	
	TRAPD(err, ud->OpenL(aFileName, KUserDictionaryMaxSize));
	if (err == KErrCorrupt)
		{
		// Checksum didn't match or there was some other corruption.
		// Try to delete corrupted file and leave.
		RFs fsSession;
		User::LeaveIfError( fsSession.Connect() );
		fsSession.Delete(aFileName);		
		fsSession.Close();
		User::Leave(KErrCorrupt);
		}
	User::LeaveIfError(err);
	
	TInt error = core->AttachUserDictionary( ud );
	if (error != KErrNone)
		{
		CleanupStack::PopAndDestroy(); // ud
		return NULL;
		}

	User::LeaveIfError(iUserDictionaries.Append( ud ));
	CleanupStack::Pop(); // ud

	return ud;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::AttachDefaultUserDictionaryL
// 
// ---------------------------------------------------------------------------
//
MPtiUserDictionary* CPtiEngineImpl::AttachDefaultUserDictionaryL(TUid aCoreUid, TInt aSymbolClass)
	{
	TFileName fileName;
	
	CreateUserDictName(fileName, aCoreUid, aSymbolClass);	
	
	return AttachUserDictionaryL(fileName);		
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::DetachUserDictionary
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::DetachUserDictionary(TDesC& aFileName)
	{
	for (TInt i = 0; i < iUserDictionaries.Count(); i++)
		{
		if (!iUserDictionaries[i]->FileName().Compare(aFileName))
			{
			CPtiCore* core = FindCoreByUid(iUserDictionaries[i]->CoreUID());
			if (SupportsUserDictionaries(core))
				{
				if ((core->DetachUserDictionary(iUserDictionaries[i])) ||
					(iUserDictionaries[i]->UpdateFlag()))
					{
					TRAPD(err, iUserDictionaries[i]->WriteL());
					return err;
					}			
				}
			else
				{
				return KErrCorrupt;
				}
			
			delete iUserDictionaries[i];
			iUserDictionaries.Remove(i);
			return KErrNone;
			}	
		}

	return KErrNotFound;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::DetachUserDictionary
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::DetachUserDictionary(TInt aId)
	{
	for (TInt i = 0; i < iUserDictionaries.Count(); i++)
		{
		if (iUserDictionaries[i]->Id() == aId)
			{
			CPtiCore* core = FindCoreByUid(iUserDictionaries[i]->CoreUID());
			if (SupportsUserDictionaries(core))
				{
				if ((core->DetachUserDictionary(iUserDictionaries[i])) ||
					(iUserDictionaries[i]->UpdateFlag()))
					{
					TRAPD(err, iUserDictionaries[i]->WriteL());
					return err;
					}			
				return KErrNone;
				}
			else
				{
				return KErrCorrupt;
				}
			}	
		}

	return KErrNotFound;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetCurrentWord
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::SetCurrentWord(TPtrC aWord)
	{
	if (aWord.Length() == 0)
		{
		return KErrNone;
		}

	if (IsCore())
		{
		if (aWord.Length() > iTextBuffer.MaxLength())
			{
			return KErrTooBig;
			}
		// Predictive QWERTY (XT9) changes ---->
		// The core may choose to ignore the resetting of the input sequence.
		// We should be prepared for that and make sure that the internal
		// state of the PtiEngine is consistent with the one in the core.
		
		/* Code which was replaced:
		iTextBuffer.Copy(aWord);
		ConvertToKeySequence(aWord, iInputSequence);
		MakeCaseSequence(aWord);
		return Core()->SetCurrentInputSequence(iInputSequence);
		*/
		TInt err = 0;
		
		TRAP_IGNORE(
		{
		HBufC16* oldTextBuf = iTextBuffer.AllocLC(); 
		HBufC8* oldInputSeq = iInputSequence.AllocLC();
		HBufC8* oldCaseBuf = iCaseBuffer.AllocLC();
				
		iTextBuffer.Copy(aWord);
		ConvertToKeySequence(aWord, iInputSequence);
		MakeCaseSequence(aWord);
		
		err = Core()->SetCurrentInputSequence(iInputSequence);
		
		// InputSequence is not incorrect, as there is mismatch present when autocompletion part is displayed  
		if(err == KErrLengthMismatch /*&& !iMaxEditorLengthCrossed*/)
		    {
		    TInt tailLength = 0;
		    TInt err = Core()->HandleCommandL( EPtiCommandGetAutoCompletionTailLength, &tailLength );
		    if(tailLength > 0)
		        {
		        iInputSequence.SetLength(iTextBuffer.Length() - tailLength);
		        iCaseBuffer.SetLength(iTextBuffer.Length() - tailLength);
		        TInt err = Core()->SetCurrentInputSequence(iInputSequence);
		        }
		    }
		
		// if cursor crosses max editor length or iMaxEditorLengthCrossed is 
		// True, sync the iTextBuffer with last word in editor
		if ( err == KErrAbort && !iMaxEditorLengthCrossed)
		    {
		    // the core has ingnored resetting the sequence
		    iTextBuffer.Copy( oldTextBuf->Des() );
		    iInputSequence.Copy( oldInputSeq->Des() );
		    iCaseBuffer.Copy( oldCaseBuf->Des() );
		    }
		CleanupStack::PopAndDestroy(3); //oldTextBuf,oldInputSeq,oldCaseBuf 
		
		});		
		return err;
		
		// Predictive QWERTY (XT9) changes <----
		}

	return KErrNoSuitableCore;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::ActivateLanguageL
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::ActivateLanguageL(TInt aEpocLanguageID, TPtiEngineInputMode aMode)
	{
	CloseCurrentLanguageL();

	CPtiCoreLanguage *lang = static_cast<CPtiCoreLanguage*>(GetLanguage( aEpocLanguageID ));
	if (!lang)
		{
		return KErrLanguageNotFound;
		}

	iCurrentLanguage = lang;

	TPtiEngineInputMode newMode = aMode;
	if (aMode == EPtiEngineInputModeNone)
		{
		// Find first available mode and set it.	
		for (TInt i = 1; i < EPtiEngineMaxInputModes; i++)
			{
			if (lang->HasInputMode((TPtiEngineInputMode)i))
				{
				newMode = (TPtiEngineInputMode)i;
				break;
				}
			}
		}

	iInputMode = newMode;
	
	TInt err;
	for (TInt i = 0; i < iCores->Count(); i++)
		{
		if (((*iCores)[i]) && (iCurrentLanguage->HasCore((*iCores)[i])))
			{
			err = (*iCores)[i]->OpenLanguageL(lang);
			if (err != KErrNone)
				{
				return err;
				}
			}
		}
	
	return SetInputMode( newMode );
	}

	
// ---------------------------------------------------------------------------
// CPtiEngineImpl::CloseCurrentLanguageL
// 
// ---------------------------------------------------------------------------
//
void CPtiEngineImpl::CloseCurrentLanguageL()
	{
	if (iCurrentLanguage)
		{
		for (TInt i = 0; i < iCores->Count(); i++)
			{
			if (((*iCores)[i]) && (iCurrentLanguage->HasCore((*iCores)[i])))
				{
				(*iCores)[i]->CloseLanguageL();
				}
			}

		iCurrentLanguage = NULL;
		iInputMode = EPtiEngineInputModeNone;
		}
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetInputMode
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::SetInputMode(TPtiEngineInputMode aMode)
	{
	if (!iCurrentLanguage)
		{
		if (aMode != EPtiEngineInputModeNone)
			{
			return KErrNoActiveLanguage;
			}
		return KErrNone;
		}

	if (aMode != EPtiEngineNumeric)    // Numeric is always allowed.
		{
		const TBool chinese = IsChinese();
		
		if (chinese && (aMode == EPtiEngineQwerty))
			{
			// This combination is never allowed, because Chinese
			// hase separate mode definitions for qwerty.
			return KErrNoSuitableCore;
			}
  	    else if (IsModeChineseQwerty(aMode) && chinese)
			{
			if (!iCurrentLanguage->HasInputMode(EPtiEngineQwerty))
				{
				return KErrNoSuitableCore;	
				}
			}
		else if (!iCurrentLanguage->HasInputMode(aMode))
			{
			return KErrNoSuitableCore;	
			}
		}

    iInputMode = aMode;	

	TFileName temp;
	temp.Copy(iTextBuffer);
	return SetCurrentWord( temp );
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CountToneMarks
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::GetNextWordCandidateListL(CDesCArray& aList)
	{
	if (IsCore())
		{
		return Core()->GetNextWordCandidateListL( aList );
		}
	
	if (!iCurrentLanguage)
		{
		return KErrNoActiveLanguage;
		}

	return KErrNoSuitableCore;
	}

		
// ---------------------------------------------------------------------------
// CPtiEngineImpl::AppendKeyPress
// 
// ---------------------------------------------------------------------------
//
TPtrC CPtiEngineImpl::AppendKeyPress(TPtiKey aKey)
	{
	if (IsValidKey(aKey) && IsCore())
		{
		// If input mode is chinese qwerty, then some key presses are
		// redirected to default core.	
		if (IsModeChineseQwerty(iInputMode))
			{
			TBool redirected = EFalse;
			TPtrC ret = RedirectKeyForChineseQwerty(aKey, redirected);
			if (redirected)
				{
				return ret;
				}
			}   
		if(KeyboardType() == EPtiKeyboardHalfQwerty && aKey == EStdKeyFullStop)
		    {
		    iPunctuationsEntered++;
		    }
		else if( KeyboardType() == EPtiKeyboard12Key && aKey == EPtiKey1 && iPunctuationsEntered == 0 )
		    {
		    // iPunctuationLocation marks the start of smileys in ITU-T
		    // iPunctuationsEntered is used to track the presence
		    iPunctuationsEntered = 1;
		    iPunctuationLocation = iTextBuffer.Length();
		    }
		if (iInputSequence.Length() < KPtiMaxWordSize)
			{
			TInt original = iTextBuffer.Length();
			TInt handleRet = 0;
			
			TBuf<KPtiMaxWordSize> prevWordBuf; 
			// To set the current word - by removing the extra suggested portion				    
	   		if(iInputMode == EPtiEngineWordCompletion)
	   			{
	   			prevWordBuf.Copy(iTextBuffer);
	   			if(original > iInputSequence.Length())
	   				{
	   				original = iInputSequence.Length();
	   				iTextBuffer.SetLength(original);
	   				SetCurrentWord(iTextBuffer);
	   				}
   				}
	   		
	   		TInt tailLength = 0;
            TInt err = KErrLengthMismatch;
            TRAP_IGNORE(err = Core()->HandleCommandL( EPtiCommandGetAutoCompletionTailLength, &tailLength ));
            if(tailLength > 0 && !err)
                {
                if ((iTextBuffer.Length() - tailLength) > 0)
                    iInputSequence.SetLength(iTextBuffer.Length() - tailLength);
                original = iInputSequence.Length();
                }
            TInt prevInputSeqLength =iInputSequence.Length(); 
			iInputSequence.Append(aKey);
			
			// If greek language, and uppercase and lowercase both mapped character is 
			// same, then set text case as lower case ELangGreek
			if( ( ELangGreek ==iCurrentLanguage->LanguageCode()) && (EPtiCaseUpper == Case()))
			    {
			    TBuf<KMaxName> lowerCaseCharacter;
                MappingDataForKey((TPtiKey)aKey, lowerCaseCharacter, EPtiCaseLower);  
                TBuf<KMaxName> uppercaseCaseCharacter;             
                MappingDataForKey((TPtiKey)aKey, uppercaseCaseCharacter, EPtiCaseUpper); 
                
                if( lowerCaseCharacter.Compare(uppercaseCaseCharacter) )
                    {
                    iCaseBuffer.Append((TInt8)Case());  
                    }
                else
                    {
                    iCaseBuffer.Append((TInt8)EPtiCaseLower);  
                    }
			    }
			else
			    {
			    iCaseBuffer.Append((TInt8)Case());
			    }
			
			// Forward to core object
			Core()->AppendKeyPress(aKey);
			
			if(KeyboardType() == EPtiKeyboardHalfQwerty && aKey == EStdKeyFullStop)
			    {
			    // check if the key press was valid and resulted in Candidates
			    if( iTextBuffer.Length() < iPunctuationsEntered )
			        iPunctuationsEntered --;
			    }
			
			TRAP_IGNORE((handleRet = Core()->HandleCommandL(EPtiCommandVowelSeqFlag, NULL)));
			if (handleRet <= 0)		
				{
				Capitalize(iTextBuffer);
				}
			
			TBuf<KPtiMaxWordSize> secondaryCandidate;						
			TRAP_IGNORE((Core()->HandleCommandL(EPtiCommandGetSecondaryCandidate, (TAny*)&secondaryCandidate)));
			
			TInt doubleWide = 0;
			TRAP_IGNORE((doubleWide = Core()->HandleCommandL(EPtiCommandDoubleWideEntry, NULL)));
						
			if(iInputMode == EPtiEngineWordCompletion)	
				{
				TInt inputSeqLength = iInputSequence.Length();
				if(!prevWordBuf.CompareF(iTextBuffer))
					{
					if( iTextBuffer.Length() >= inputSeqLength )
						{
						if(aKey != CharacterToKey( (TUint)iTextBuffer[inputSeqLength - 1]))
							{
							iInputSequence.SetLength(inputSeqLength - 1);
							iCaseBuffer.SetLength(iCaseBuffer.Length() - 1);
							iTextBuffer.SetLength(inputSeqLength - 1);
							}
						}
					else
						{
						iInputSequence.SetLength(inputSeqLength - 1);
						iCaseBuffer.SetLength(iCaseBuffer.Length() - 1);
						}
					}
				else
					{
					if(iTextBuffer.Length() == original)
						{
						iInputSequence.SetLength(inputSeqLength - 1);
						iCaseBuffer.SetLength(iCaseBuffer.Length() - 1);	
						}
					}
				}			
			else if ((iTextBuffer.Length() == original) && 
				(iInputMode == EPtiEngineMultitapping ||
#ifdef RD_INTELLIGENT_TEXT_INPUT
				(iInputMode == EPtiEngineInputModeZhuyinMiniQwertyFind)||
#endif
				 iInputMode == EPtiEnginePredictive ||
				 iInputMode == EPtiEngineQwerty ||
				 // make sure that the secondary candidate length has also not increased in case of qwerty
				 (iInputMode == EPtiEngineQwertyPredictive && (secondaryCandidate.Length() == original) )||  				 
				 iInputMode == EPtiEngineHalfQwerty ||
				 iInputMode == EPtiEngineHalfQwertyPredictive))
				{
				// If key press didn't increase lenght of
				// input buffer, remove key and case items from list.							
				iInputSequence.SetLength(prevInputSeqLength);
				iCaseBuffer.SetLength(prevInputSeqLength);
				}
			else if (doubleWide > 0)				
				{
				// Some (arabic) charaters in keymap data are handled as a special case by core
				// and they actually produce two unicode characters.
				iInputSequence.Append(aKey);
				iCaseBuffer.Append((TInt8)Case());					
				}				
			else if (iTextBuffer.Length() > original + 1 && iInputMode != EPtiEngineWordCompletion &&
			         iInputMode != EPtiEngineQwertyPredictive && iInputMode != EPtiEngineHalfQwertyPredictive) 
				{
				handleRet = 0;
				TRAP_IGNORE((handleRet = Core()->HandleCommandL(EPtiCommandQueryAndClearGetAllFlag, NULL)));
				if (handleRet > 0)
					{					
			 		// If several characters were added into buffer as result of
			    	// "get all"-control character, arabic special cases, etc., then make sure that
			    	// iInputSequence and iCaseBuffer stay in sync. 
			    	const TInt count = iTextBuffer.Length() - (original + 1);
			    	for (TInt j = 0; j < count; j++)
			    		{
						iInputSequence.Append(aKey);
						iCaseBuffer.Append((TInt8)Case());			    				    	
			   			}			    				    
					}
			    }			    
			    				
			if (iInputMode == EPtiEngineQwerty)
				{	
				handleRet = 0;			
				TRAP_IGNORE((handleRet = Core()->HandleCommandL(EPtiCommandPollQwertyDeadKeyRootFlag, NULL)));
				
				if (handleRet > 0)
					{
					// If there was qwerty dead key waiting, but it didn't produce any accented
					// character and dead key root char was added to buffer in addition to nornal
					// character, then sync input sequence and case buffers.
					TPtiKey ktmp = (TPtiKey)iInputSequence[iInputSequence.Length() - 1];			
					iInputSequence.Append(ktmp);
					iCaseBuffer.Append(iCaseBuffer[iCaseBuffer.Length() - 1]);
					}
				}
			}
		}
#ifdef RD_INTELLIGENT_TEXT_INPUT
     if(iInputMode == EPtiEngineInputModeZhuyinMiniQwertyFind)
        {
	    if (iZhuyinSymbols.Length()==0 ||iKeyMuliTap != aKey)
	         {
	         iKeyMuliTap = aKey;
	         iIndex = 0;
	         iZhuyinSymbols.Zero();
	         TInt8 spellcount = PhoneticSpellingCount();
	         for (TInt i=1; i<=spellcount; i++)
	             {
	             iZhuyinSymbols.Append(Core()->GetPhoneticSpelling(i));
	             }
	         }
	     else if(iZhuyinSymbols.Length()>0)
	         {
	         iIndex++;
	         if (iIndex == iZhuyinSymbols.Length())
	             {
	             iIndex=0;
	             }
	         }  
	    return (iZhuyinSymbols.Ptr()+ iIndex);
         }
#endif	
	return iTextBuffer;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::DeleteKeyPress
// 
// ---------------------------------------------------------------------------
//
TPtrC CPtiEngineImpl::DeleteKeyPress()
	{
	if (IsCore() && (iInputSequence.Length() > 0))
		{
		TInt numtones = 0;
		if ((iInputMode == EPtiEnginePinyinByPhrase) ||
			(iInputMode == EPtiEngineZhuyinByPhrase)||
			(iInputMode == EPtiEnginePinyinPhraseHalfQwerty)||
			(iInputMode == EPtiEngineZhuyinPhraseHalfQwerty))
			{
			numtones = CountToneMarks(Core()->GetPhoneticSpelling(1));
			}
		if(iPunctuationsEntered)
		    {
		    iPunctuationsEntered--;
		    }
		TText tone;
		Core()->ToneMark( tone );
		TText newtone = tone; 		
		Core()->DeleteKeyPress();		
		if (IsChinese())
			{
			if ((iInputMode == EPtiEnginePinyinByPhrase) ||
				(iInputMode == EPtiEngineZhuyinByPhrase)||
				(iInputMode == EPtiEnginePinyinPhraseHalfQwerty)||
				(iInputMode == EPtiEngineZhuyinPhraseHalfQwerty))
				{
				if (CountToneMarks(Core()->GetPhoneticSpelling(1)) != numtones)
					{
					newtone++;
					}
				}
			else
				{
				Core()->ToneMark( newtone );
				}
			}
		if (tone == newtone)
			{
			iInputSequence.SetLength(iInputSequence.Length() - 1);
			iCaseBuffer.SetLength(iCaseBuffer.Length() - 1);
			}
		Capitalize(iTextBuffer);
		}
#ifdef RD_INTELLIGENT_TEXT_INPUT
	if(iInputMode == EPtiEngineInputModeZhuyinMiniQwertyFind)
	    {
        iIndex = 0;
        iZhuyinSymbols.Zero(); 
	    } 
#endif  
	return iTextBuffer;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::RedirectKeyForChineseQwerty
// 
// ---------------------------------------------------------------------------
//
TPtrC CPtiEngineImpl::RedirectKeyForChineseQwerty(TPtiKey aKey, TBool& aRedirected)
	{
	aRedirected = EFalse;
    TPtiKeyboardType kbdType = KeyboardType();	
    
	switch (iInputMode)
		{
		case EPtiEnginePinyinQwerty:
			 if (iCase == EPtiCaseLower && aKey == EPtiKeyQwertyApostrophe)
			    {
			    break;
			    }				
		case EPtiEnginePinyinPhraseHalfQwerty:
		    if(EPtiKeyboardHalfQwerty == kbdType)
		        {
		        if ((iCase == EPtiCaseLower) && ( ( ((aKey >= EPtiKeyQwertyA)
                        && (aKey <= EPtiKeyQwertyZ))|| (aKey
                        == EPtiKeyQwertySpace) ) ))
                    {
                    return TPtrC();
                    }
		        }
		    break;
		case EPtiEnginePinyinPhraseQwerty:	
		     if(EPtiKeyboardQwerty4x10 == kbdType ||
		        EPtiKeyboardQwerty3x11 == kbdType ||
		        EPtiKeyboardCustomQwerty == kbdType)
		        {
    			 if ((iCase == EPtiCaseLower) &&
    				 (
    				 (
    				 ((aKey >= EPtiKeyQwertyA) && (aKey <= EPtiKeyQwertyZ))||
    				 (aKey == EPtiKeyQwertySpace)
    				 )
    				 ))
    				{
    				return TPtrC();
    				}		        
		        }
		     else
		        {
    			 if ((iCase == EPtiCaseLower) &&
    				 (((aKey >= EPtiKeyQwertyA) && (aKey <= EPtiKeyQwertyZ)) ||
    		  	      (aKey == EPtiKeyQwertyApostrophe)))
    				{
    				return TPtrC();
    				}
		        }		     
		     break;

		case EPtiEngineStrokeQwerty:
		case EPtiEngineStrokePhraseHalfQwerty:
		case EPtiEngineStrokePhraseQwerty:
			{
			TPtiKeyboardType keyboardType = EPtiKeyboardNone;
			TBool IsStokeKey = EFalse;
            TRAP_IGNORE(keyboardType = ActiveKeyboardTypeL());
             if(EPtiKeyboardQwerty4x10 == keyboardType ||
                     EPtiKeyboardQwerty3x11 == keyboardType )
                 {
                TBuf<KMaxName> lowerdata;
                 TInt StrokeUnicodePosition =0;
                 MappingDataForKey((TPtiKey)aKey, lowerdata, EPtiCaseLower);  
                 if(lowerdata.Length()>0)
                         {
                         for(TInt i=0;i<lowerdata.Length();i++)
                             {
                             if(lowerdata[i]==KStrokeUnicode)
                                 {
                                 StrokeUnicodePosition = i + 1;
                                 break;
                            }
                        }
                    }
                if (lowerdata.Length() > StrokeUnicodePosition && iCase
                        == EPtiCaseLower)
                    {
                    if (lowerdata[StrokeUnicodePosition]
                            == KStrokeHorizontalValue
                            || lowerdata[StrokeUnicodePosition]
                                    == KStrokeVerticalValue
                            || lowerdata[StrokeUnicodePosition]
                                    == KStrokeDownToLeftValue
                            || lowerdata[StrokeUnicodePosition]
                                    == KStrokeDownToRightValue
                            || lowerdata[StrokeUnicodePosition]
                                    == KStrokeBendingValue
                            || lowerdata[StrokeUnicodePosition]
                                    == KStrokeQuestionValue)

                        {
                        return TPtrC();
                        }
                    }
                 if (iCase == EPtiCaseLower && (aKey == EPtiKeyQwertySpace
                        || aKey == EPtiKeyQwertyChr || aKey
                        == EPtiKeyQwertyApostrophe))
                    {
                    return TPtrC();
                    }
                 if(iCase == EPtiCaseLower && aKey == EPtiKeyQwertySpace)
                     {
                     return TPtrC();
                     }
                }
             else if (EPtiKeyboardHalfQwerty == keyboardType ||
               EPtiKeyboardCustomQwerty == keyboardType)
                {
                MPtiLanguage *lang = CurrentLanguage();
                RArray < TPtiNumericKeyBinding > keybinding;
                if(lang)
                    {
                    TRAP_IGNORE(GetNumericModeKeysForQwertyL(lang->LanguageCode(), keybinding, keyboardType));
                    TInt numericKeysCount = keybinding.Count();
                    while ( numericKeysCount-- )
                        {
                        TPtiNumericKeyBinding numKeyBind = keybinding[numericKeysCount];
                        if ( (numKeyBind.iChar >= 0x31 && numKeyBind.iChar <= 0x36)&&(aKey == numKeyBind.iKey))
                            {
                               IsStokeKey = ETrue;
                               break;              
                            }
                        }
                    }
                keybinding.Reset();
                if ( ( IsStokeKey || aKey == EPtiKeyQwertySpace || aKey == EPtiKeyQwertyChr || aKey == EStdKeyLeftFunc || aKey == EStdKeyFullStop )
                    && (iCase == EPtiCaseLower))
                    {
                    if((aKey >= EPtiKeyQwertyA && aKey <= EPtiKeyQwertyZ) ||
                       (aKey >= EPtiKeyQwerty0 && aKey <= EPtiKeyQwerty9) ||
                       (aKey == EPtiKeyQwertySpace) ||
                        aKey == EPtiKeyQwertyChr || 
                        aKey == EStdKeyLeftFunc  || 
                        aKey == EStdKeyFullStop )
                        {
                        return TPtrC();
                        }
                    }
                }
            else
                {
                if ((iCase == EPtiCaseLower) &&
                 ((aKey == EPtiKeyQwertyQ) ||
                  (aKey == EPtiKeyQwertyW) ||
                  (aKey == EPtiKeyQwertyE) ||
                  (aKey == EPtiKeyQwertyA) ||
                  (aKey == EPtiKeyQwertyS) ||
                  (aKey == EPtiKeyQwertyD) ||
                  (aKey == EPtiKeyQwertyJ) ||

                  (aKey == EPtiKeyQwertyI) ||
                  (aKey == EPtiKeyQwertyO) ||
                  (aKey == EPtiKeyQwertyP) ||

                  (aKey == EPtiKeyQwertyK) ||
                  (aKey == EPtiKeyQwertyL) ||
                  (aKey == EPtiKeyQwertySpace)
                  ))
                    {
                    return TPtrC();
                    }                
                }		
			}
			 break;
		case EPtiEngineNormalCangjieQwerty:
		case EPtiEngineEasyCangjieQwerty:
		case EPtiEngineAdvCangjieQwerty:
			 if ((iCase == EPtiCaseLower) &&
				 (aKey >= EPtiKeyQwertyA) && (aKey <= EPtiKeyQwertyY))
			    {
			    return TPtrC();
			    }
			 break;
		case EPtiEngineZhuyinQwerty:
#ifdef RD_INTELLIGENT_TEXT_INPUT
		case EPtiEngineInputModeZhuyinMiniQwertyFind:
#endif
		case EPtiEngineZhuyinPhraseQwerty:
             if(  EPtiKeyboardHalfQwerty == kbdType ||
		        EPtiKeyboardCustomQwerty == kbdType)
		        {
    			 if ((iCase == EPtiCaseLower) &&
    				 (
    				 (
    				 ((aKey >= EPtiKeyQwertyA) && (aKey <= EPtiKeyQwertyZ))||
    				 (aKey == EPtiKeyQwertySpace)
    				 )
    				 ))
    				{
    				return TPtrC();
    				}		        
		        }
		     else if(EPtiKeyboardQwerty4x10 == kbdType ||
		            EPtiKeyboardQwerty3x11 == kbdType  )
		        {
		        TBuf<KMaxName> lowerdata;
                TInt i;
                MappingDataForKey((TPtiKey) aKey, lowerdata, EPtiCaseLower);
                if (lowerdata.Length() > 0)
                    {
                    for ( i = 0; i < lowerdata.Length(); i++)
                        {
                        if (lowerdata[i] == KZhuyinIndicator)
                            {
                            break;
                            }
                        }
                    if(i!=lowerdata.Length() && iCase == EPtiCaseLower )
                        {
                        return TPtrC();
                        }
    				}		        
		        }
		     else
		        {		        
    			 if ((iCase == EPtiCaseLower) &&
    				 (aKey != EPtiKeyQwertyPlus) &&
    				 (aKey != EPtiKeyQwertyMinus) &&
    				 (aKey != EPtiKeyQwertyHash) )
    				{
    				return TPtrC();	
    				}
		        }		     			
			 break;	
		default:
			return TPtrC();
		}

	CPtiCore* core = CoreForInputMode(EPtiEngineQwerty);
#ifdef RD_INTELLIGENT_TEXT_INPUT
	//if language chinese comes here the keyboard type is halfqwerty. it means that half qwerty english input needed.
	if(EPtiKeyboardHalfQwerty == kbdType)
	    {
	    core = CoreForInputMode(EPtiEngineHalfQwerty);
	    SetInputMode(EPtiEngineHalfQwerty);
	    }
	//end for half qwerty
#endif
	
	if (core)
		{
		aRedirected = ETrue;
		core->AppendKeyPress(aKey);	
		return iTextBuffer;
		}
	
	return TPtrC();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CountToneMarks
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::CountToneMarks(TPtrC aSpelling) const
	{
	TInt res = 0;

	for (TInt i = 0; i < aSpelling.Length(); i++)
		{
		if (IsToneMarkOrSeparator(aSpelling[i]))
			{
			res++;
			}
		}

	return res;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CreateLanguageNamesListLC
// 
// ---------------------------------------------------------------------------
//
CArrayFixFlat<TPtiLangName>* CPtiEngineImpl::CreateLanguageNamesListLC()
	{
	CArrayFixFlat<TPtiLangName>* langNames = new( ELeave ) CArrayFixFlat<TPtiLangName>( KDefaultLangNamesGran ); 
	CleanupStack::PushL( langNames );

	TResourceReader reader;	
	TFileName fileName;

	TParse* fp = new(ELeave) TParse(); 
	fp->Set(KCEResourceFile, &KDC_RESOURCE_FILES_DIR, NULL); 
	fileName.Copy(fp->FullName());
	delete fp;

	// Resource reading is done without coe or eikon env.
	RFs fsSession;
	User::LeaveIfError( fsSession.Connect() );
	CleanupClosePushL( fsSession );

	RResourceFile rsFile;
	rsFile.OpenL( fsSession, fileName );
	CleanupClosePushL( rsFile );

	// Read language name table.
	HBufC8* rBuffer = rsFile.AllocReadLC( R_PTI_LANGUAGE_TRANSLATIONS & KCCMask ); // Remove offset from id
	reader.SetBuffer( rBuffer );

	TPtiLangName name;
	TInt count = reader.ReadInt16();

	for (TInt i = 0; i < count; i++)
		{
		name.iName.Copy(reader.ReadTPtrC());
		name.iLanguageCode = reader.ReadInt16();	
		langNames->AppendL(name);
		}

	CleanupStack::PopAndDestroy(3);  // rsFile, fsSession, rBuffer  
	return langNames;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::FindLanguageName
// 
// ---------------------------------------------------------------------------
//
void CPtiEngineImpl::FindLanguageName(CArrayFixFlat<TPtiLangName>*aList, TDes& aResult, TInt aLangCode)
	{
	for (TInt i = 0; i < aList->Count(); i++)
		{
		if (aList->At(i).iLanguageCode == aLangCode)
			{
			aResult.Copy(aList->At(i).iName);
			return;
			}
		}
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetLocalizedLanguageName
// 
// ---------------------------------------------------------------------------
//
void CPtiEngineImpl::GetLocalizedLanguageNameL(TInt aLangCode, TDes& aResult)
	{
	CArrayFixFlat<TPtiLangName>* nameList = CreateLanguageNamesListLC();
	FindLanguageName(nameList, aResult, aLangCode);
	CleanupStack::PopAndDestroy(); // nameList
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetCandidateListL
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::GetCandidateListL(CDesCArray& aList)
	{
	if (IsCore())
		{
		return Core()->GetCandidateListL( aList );
		}

	if (!iCurrentLanguage)
		{
		return KErrNoActiveLanguage;
		}

	return KErrNoSuitableCore;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::FirstCandidate
// 
// ---------------------------------------------------------------------------
//
TPtrC CPtiEngineImpl::FirstCandidate()
	{
	iCandidateBuffer.Zero();	
	if (IsCore())
		{
		Core()->GetFirstCandidate(iCandidateBuffer);	
		Capitalize(iCandidateBuffer);		
		}

	iTextBuffer.Copy(iCandidateBuffer);   
	iTextBuffer.SetLength(iCandidateBuffer.Length());    

	return iCandidateBuffer;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::NextCandidate
// 
// ---------------------------------------------------------------------------
//	
TPtrC CPtiEngineImpl::NextCandidate()
	{
	iCandidateBuffer.Zero();	
	if (IsCore())
		{
		Core()->GetNextCandidate(iCandidateBuffer);
		Capitalize(iCandidateBuffer);		
		}

	iTextBuffer.Copy(iCandidateBuffer);   
	iTextBuffer.SetLength(iCandidateBuffer.Length());    

	return iCandidateBuffer;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::PreviousCandidate
// 
// ---------------------------------------------------------------------------
//
TPtrC CPtiEngineImpl::PreviousCandidate()
	{
	iCandidateBuffer.Zero();
	if (IsCore())
		{
		Core()->GetPreviousCandidate(iCandidateBuffer);
		Capitalize(iCandidateBuffer);		
		}

	// set to iTextBuffer
	iTextBuffer.Copy(iCandidateBuffer);   
	iTextBuffer.SetLength(iCandidateBuffer.Length());    

	return iCandidateBuffer;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CoreInfo
// 
// ---------------------------------------------------------------------------
//
MPtiCoreInfo* CPtiEngineImpl::CoreInfo(TPtiEngineInputMode aMode) const
	{
	if (IsCoreForInputMode(aMode))
		{
		return iCurrentLanguage->GetCore(aMode)->GetCoreInfo();
		}

	return NULL;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::NumberOfCandidates
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::NumberOfCandidates()
	{
	if (IsCore())
		{
		TInt numberOfCandidates = Core()->NumberOfCandidates();
		if (iInputMode == EPtiEngineWordCompletion)
			{
			Capitalize(iTextBuffer);	
			}
		
		return numberOfCandidates ;
		}

	return 0;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetReordering
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::SetReordering(TBool aStatus)
	{
	if (IsCore())
		{
		return Core()->SetReordering( aStatus );
		}

	if (!iCurrentLanguage)
		{
		return KErrNoActiveLanguage;
		}

	return KErrNoSuitableCore;
	}

			
// ---------------------------------------------------------------------------
// CPtiEngineImpl::ClearCurrentWord
// 
// ---------------------------------------------------------------------------
//
void CPtiEngineImpl::ClearCurrentWord()
	{
	ClearBuffers();
	if (IsCore())
		{
		Core()->ClearBuffer();
		}
#ifdef RD_INTELLIGENT_TEXT_INPUT
	   if(iInputMode ==  EPtiEngineInputModeZhuyinMiniQwertyFind)
	       {
	       iIndex = 0;
	       iZhuyinSymbols.Zero(); 
	       }
#endif
	}
	
// ---------------------------------------------------------------------------
// CPtiEngineImpl::RemoveTrailingPunctuations
// 
// ---------------------------------------------------------------------------
//	
void CPtiEngineImpl::RemoveTrailingPunctuations()
    {   
    // iTextBuffer contains the entire string
    // iPunctuationsEntered contains the count of punct chars.
    if(( iTextBuffer.Length()- iPunctuationsEntered ) >= 0 )
        {
        // trim off the punctuation characters
        iTextBuffer =  iTextBuffer.LeftTPtr(iTextBuffer.Length()- iPunctuationsEntered);
        }            
    iPunctuationsEntered = 0;
    }

// ---------------------------------------------------------------------------
// CPtiEngineImpl::RemoveTrailingPunctuations12Key
// 
// ---------------------------------------------------------------------------
//  
void CPtiEngineImpl::RemoveTrailingPunctuations12Key()
    {
    if(iTextBuffer.Length()>1)
        {
        // Trim off all punctuations starting at iPunctuationLocation
        iTextBuffer = iTextBuffer.LeftTPtr( iPunctuationLocation );
        }            
        iPunctuationLocation = 0;
        iPunctuationsEntered = 0;
    }

// ---------------------------------------------------------------------------
// CPtiEngineImpl::CommitCurrentWord
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::CommitCurrentWord()
	{
	if (IsCore())
		{
		TInt error = KErrNone;
		if(iPunctuationsEntered)
			{
			if( iInputMode == EPtiEngineHalfQwertyPredictive )
			    {
			    // Do not commit the trailing punctuations to Dictionary.
			    RemoveTrailingPunctuations();
			    }
			else if( iInputMode == EPtiEnginePredictive)
			    {
			    // Do not commit smileys to Dictionary
			    RemoveTrailingPunctuations12Key();
			    }
			}
	    // core should take care that single char does not get added to UDB/UWD
		error = Core()->CommitCurrentWord();	
		
		ClearBuffers();	
		return error;
		}

	if (!iCurrentLanguage)
		{
		return KErrNoActiveLanguage;
		}

	return KErrNoSuitableCore;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::AddCoreL
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::AddCoreL(const TUid aCoreUid, TBool aUseDefaultUserDictionary)
	{
	CPtiCore* core = CPtiCore::CreateCoreL(aCoreUid);
	if (!core)
		{
		return KErrNotFound;
		}
	
	CleanupStack::PushL( core );
	iCores->AppendL( core );
	CleanupStack::Pop(); // core
	core->InitializeL(iFrontEnd, &iTextBuffer, iLanguages, aUseDefaultUserDictionary);

	return KErrNone;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::RefreshLanguageNamesL
// 
// ---------------------------------------------------------------------------
//
void CPtiEngineImpl::RefreshLanguageNamesL()
	{
	if (!iLanguages)
		{
		return;
		}

	CArrayFixFlat<TPtiLangName>* langNames = CreateLanguageNamesListLC();
	
	CPtiCoreLanguage* lang;
	for (TInt i = 0; i < iLanguages->Count(); i++)
		{
		lang = iLanguages->At(i);
		if ((!lang->LocalizedName().Length()) && (lang->LanguageCode() != ELangNumeric))
			{
			FindLanguageName(langNames, lang->LocalizedNameDes(), lang->LanguageCode());		
			}
		// Make sure that there is a core for numeric input mode.
		if (!lang->GetCore(EPtiEngineNumeric))
			{
			lang->SetCore(FindCoreByUid(KDefaultCoreUid), EPtiEngineNumeric); 
			}
		}

	CleanupStack::PopAndDestroy(); // langNames
	}



// ---------------------------------------------------------------------------
// CPtiEngineImpl::CharConversion
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::CharConversion(TPtiCharConversion aType,
			                         TAny* aInput,
									 TInt aInputLength,
									 TAny* aOutput)
	{
	MPtiCoreInfo* info;
	for (TInt i = 0; i < iCores->Count(); i++)
		{
		info = iCores->At(i)->GetCoreInfo();
		if ((info) && (info->CharConversions() & aType))
			{
			return iCores->At(i)->Convert(aType, aInput, aInputLength, aOutput);
			}
		}

	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::AvailableCharConversions
// 
// ---------------------------------------------------------------------------
//
TUint32 CPtiEngineImpl::AvailableCharConversions() const
	{
	TUint32 res = 0;
	MPtiCoreInfo* info;
	
	for (TInt i = 0; i < iCores->Count(); i++)
		{
		info = iCores->At(i)->GetCoreInfo();
		if (info)
			{
			res |= info->CharConversions();
			}
		}

	return res;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::DetachUserDictionaries
// 
// ---------------------------------------------------------------------------
//
void CPtiEngineImpl::DetachUserDictionaries()
	{
	MPtiCore* core;
	
	for (TInt i = 0; i < iUserDictionaries.Count(); i++)
		{
		core = FindCoreByUid(iUserDictionaries[i]->CoreUID());
		if (core)
			{
			core->DetachUserDictionary(iUserDictionaries[i]);
			}
		}	
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::FindCoreByUid
// 
// ---------------------------------------------------------------------------
//
CPtiCore* CPtiEngineImpl::FindCoreByUid(TInt32 aUid)
	{
	for (TInt i = 0; i < iCores->Count(); i++)
		{
		MPtiCoreInfo* info = iCores->At(i)->GetCoreInfo();
		if ((info) && (info->Uid() == aUid))
			{
			return iCores->At(i);
			}			
		}

	return NULL;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetExternalKeyMapL
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::SetExternalKeyMapL(TPtiEngineInputMode aMode,
			                                 TPtiKey aKey,
										     TDesC& aKeyMap,
											 TPtiTextCase aCase)
	{
	if (iCurrentLanguage || iInputMode == EPtiEngineNumeric)
		{			
	    
		MPtiCore *core = NULL;
		if( iCurrentLanguage != NULL )
		    {
		    core = CoreForInputMode( aMode );
		    }
		if (!core)
			{
			return KErrNoSuitableCore;
			}

		return core->SetExternalKeyMapL(aMode, aKey, aKeyMap, aCase);
		}
		
	return KErrNoActiveLanguage;		
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::ConvertToKeySequence
// 
// ---------------------------------------------------------------------------
//
void CPtiEngineImpl::ConvertToKeySequence(TPtrC aIn, TDes8& aOut)
	{
	aOut.Zero();

	if (!iCurrentLanguage)
		{
		return;
		}

	const TBool vietnamese = iCurrentLanguage->LanguageCode() == ELangVietnamese;

	MPtiKeyMappings* map;
	if (iInputMode == EPtiEngineQwerty || iInputMode == EPtiEngineQwertyPredictive)
		{
		map = iCurrentLanguage->GetQwertyKeymappings();
		}
	else if( iInputMode == EPtiEngineHalfQwerty || iInputMode == EPtiEngineHalfQwertyPredictive )
		{
		map = iCurrentLanguage->GetHalfQwertyKeymappings();
		}
	else	
		{
		map = iCurrentLanguage->GetKeymappings();
		}

	if (!map)
		{
		return;
		}	

	    
	TUint16 ch;
	for (TInt i = 0; i < aIn.Length(); i++)
		{
		ch = (TUint16)map->KeyForCharacter(aIn[i]);	
		if (!ch && vietnamese)
			{
			// This may be Vietnamese tone mark or an accented character which isn't listed
			// in keymappings. 
			ch = RemapVietnameseAccentedCharacter(aIn[i]);
			}			

		if(ch) 
		    aOut.Append( ch );
		}

/* Search the KeySequence in Secondary Language as well if Dual Language
 feature is enabled */
#ifdef FF_DUAL_LANGUAGE_SUPPORT
    if(aOut.Length() == 0 && iSecondaryLanguage)
        {
        SearchKeySequenceInSecLang(aIn, aOut);                 
        }
#endif // FF_DUAL_LANGUAGE_SUPPORT
	}

#ifdef FF_DUAL_LANGUAGE_SUPPORT
void CPtiEngineImpl:: SearchKeySequenceInSecLang(TPtrC aIn, TDes8& aOut)
    {
    MPtiKeyMappings* secLangMap = NULL;
    const TBool vietnamese = iSecondaryLanguage->LanguageCode() == ELangVietnamese;
        
    if (iInputMode == EPtiEngineQwerty || iInputMode == EPtiEngineQwertyPredictive)
        {
        secLangMap = iSecondaryLanguage->GetQwertyKeymappings();
        }
    else if( iInputMode == EPtiEngineHalfQwerty || iInputMode == EPtiEngineHalfQwertyPredictive )
        {
        secLangMap = iSecondaryLanguage->GetHalfQwertyKeymappings();
        }
    else    
        {
        secLangMap = iSecondaryLanguage->GetKeymappings();
        }

    TUint16 ch;
    for (TInt i = 0; i < aIn.Length(); i++)
        {
        ch = (TUint16)secLangMap->KeyForCharacter(aIn[i]); 
        if (!ch && vietnamese)
            {
            // This may be Vietnamese tone mark or an accented character which isn't listed
            // in keymappings. 
            ch = RemapVietnameseAccentedCharacter(aIn[i]);
            }
        aOut.Append( ch );
        }
    
    }
#endif // FF_DUAL_LANGUAGE_SUPPORT  

// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetSpelling
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::GetSpelling(TUint16 aInput, TDes& aOutput, TPtiSpelling aType)
	{
	TInt ret = 0;
    for (TInt i = 0; i < iCores->Count(); i++)
        {
        MPtiCoreInfo* info = iCores->At(i)->GetCoreInfo();
        if (info && (info->Spellings() & aType ) && (info->CapsBits()
                & EPreferedSpelling ))
            {
            ret = iCores->At(i)->GetSpelling(aInput, aOutput, aType);
            if (ret == KErrNone)
                {
                return KErrNone;
                }
            }
        }

	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CancelTimerActivity
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::CancelTimerActivity()
	{
#ifdef RD_INTELLIGENT_TEXT_INPUT
	if(iInputMode ==  EPtiEngineInputModeZhuyinMiniQwertyFind)
	    {
	    iIndex =0;
	    iZhuyinSymbols.Zero();
	    }
#endif //RD_INTELLIGENT_TEXT_INPUT
	if (IsCore())
		{	
		TInt res =  Core()->CancelTimerActivity();
		if (Observer())
			{
			Observer()->KeyTimerExpired();
			}
		return res;
		}

	return KErrNotFound;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CharacterToKey
// 
// ---------------------------------------------------------------------------
//
TPtiKey CPtiEngineImpl::CharacterToKey(TUint16 aChar)
	{
	MPtiKeyMappings* map;
	if (iInputMode == EPtiEngineQwerty 
	    || iInputMode == EPtiEngineQwertyPredictive ) // Predictive QWERTY (XT9) changes
		{
		map = iCurrentLanguage->GetQwertyKeymappings();
		}
    else if (iInputMode == EPtiEngineHalfQwerty
    	|| iInputMode == EPtiEngineHalfQwertyPredictive )
        {
		map = iCurrentLanguage->GetHalfQwertyKeymappings();        
        }
	else
		{
		map = iCurrentLanguage->GetKeymappings();
		}

	if (!map)
		{
		return EPtiKeyNone;
		}	

	return map->KeyForCharacter(aChar);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::AddUserDictionaryEntry
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::AddUserDictionaryEntry(MPtiUserDictionaryEntry& aEntry)
	{
    
    if ( !iCurrentLanguage )
        {
        return KErrNoActiveLanguage;
        }
    
	CPtiUserDictionary* dict = DefaultUserDictForCore(Core());

	if (dict)
		{
		return Core()->AddUserDictionaryEntry(aEntry, dict);
		}

	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::AddUserDictionaryEntry
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::AddUserDictionaryEntry(MPtiUserDictionaryEntry& aEntry, TInt aId)
	{
	for (TInt i = 0; i < iUserDictionaries.Count(); i++)
		{
		if (iUserDictionaries[i]->Id() == aId)	
			{
			MPtiCore* core = FindCoreByUid(iUserDictionaries[i]->CoreUID());
			if (!core)
				{
				return KErrCorrupt;
				}
			return core->AddUserDictionaryEntry(aEntry, iUserDictionaries[i]);
			}
		}

	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::RemoveEntryFromUserDictionary
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::RemoveEntryFromUserDictionary(MPtiUserDictionaryEntry& aEntry)
	{
    
    if ( !iCurrentLanguage )
        {
        return KErrNoActiveLanguage;
        }
    
	CPtiUserDictionary* dict = DefaultUserDictForCore(Core());

	if (dict)
		{
		return Core()->RemoveUserDictionaryEntry(aEntry, dict);
		}

	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::RemoveEntryFromUserDictionary
// 
// ---------------------------------------------------------------------------
//	
TInt CPtiEngineImpl::RemoveEntryFromUserDictionary(MPtiUserDictionaryEntry& aEntry, TInt aId)
	{
	for (TInt i = 0; i < iUserDictionaries.Count(); i++)
		{
		if (iUserDictionaries[i]->Id() == aId)	
			{
			MPtiCore* core = FindCoreByUid(iUserDictionaries[i]->CoreUID());
			if (!core)
				{
				return KErrCorrupt;
				}
			return core->RemoveUserDictionaryEntry(aEntry, iUserDictionaries[i]);
			}
		}

	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::NumberOfEntriesInUserDictionary
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::NumberOfEntriesInUserDictionary()
	{
    
    if ( !iCurrentLanguage )
        {
        return KErrNoActiveLanguage;
        }
	CPtiUserDictionary* dict = DefaultUserDictForCore(Core());
	
	if (dict)
		{
		return Core()->NumberOfEntriesInUserDictionary(dict);
		}
	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::Capitalize
// 
// ---------------------------------------------------------------------------
//
void CPtiEngineImpl::Capitalize(TDes& aTextBuffer)
	{
	__ASSERT_DEBUG(iCurrentLanguage, User::Panic(KPtiEngineImplPanic, KErrCorrupt));
	
	// Predictive QWERTY (XT9) changes ---->
	// PtiXt9Core handles the capitalization it self, and it should not be overriden 
	// byt the PtiEngine.
	if ( iInputMode==EPtiEngineQwertyPredictive )
	    {
	    return;
	    }
	// Predictive QWERTY (XT9) changes <----
	
	if (aTextBuffer.Length())
		{
		TBuf<KPtiMaxWordSize> temp;
		TBuf<1> one;

		temp.Copy(aTextBuffer);
		aTextBuffer.Zero();
		for (TInt i = 0; i < temp.Length(); i++)
			{
			one.Copy(temp.Mid(i, 1));
			
			if (AllowCapitalize(one[0]))
				{											
				if (iCaseBuffer.Length() >= i + 1) 
					{
			   	 if ((iCaseBuffer[i] == EPtiCaseUpper) || (iCaseBuffer[i] == EPtiCaseChrUpper)
#ifdef RD_INTELLIGENT_TEXT_INPUT
|| ( iCaseBuffer[i] == EPtiCaseFnUpper )
#endif
			   	 )
						{
						one.UpperCase();
						}
					else
						{
						one.LowerCase();
						}
					}
				else
					{
				    if ((Case() == EPtiCaseUpper) || (Case() == EPtiCaseChrUpper))
						{
						one.UpperCase();
						}
					else
						{
						one.LowerCase();
						}				
					}
				}
					    					
			aTextBuffer.Append(one);
			}
		}
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CaseForMappedCharacter
// 
// ---------------------------------------------------------------------------
//
TPtiTextCase CPtiEngineImpl::CaseForMappedCharacter(TChar aCharacter)
{
    const static int numberOfCases = 6;
    TPtiTextCase casesTOCheckAgainst[numberOfCases]={EPtiCaseLower, EPtiCaseUpper, EPtiCaseChrLower, EPtiCaseChrUpper, EPtiCaseFnLower, EPtiCaseFnUpper};
    TPtiTextCase caseCalculated = EPtiCaseLower;
    TBuf<KDefaulCoreMaximumWordLength> mappedCharacters; 
    TBool isMappingFound = EFalse;
    TPtiKey key = CharacterToKey(aCharacter);

    //It loops through all the key mappings to find the character in the key mappings and
    //if the character available in the key mapping, fetches the corresponding case.
    for(int i=0;i<numberOfCases;i++)
        {
        MappingDataForKey(key,mappedCharacters,casesTOCheckAgainst[i]);
        for(int j=0; j<mappedCharacters.Length();j++)
            {
            if(mappedCharacters[j] == aCharacter)
                {
                caseCalculated = casesTOCheckAgainst[i];
                isMappingFound = ETrue;
                break;
                }
            }
        if(isMappingFound)
            {
            break;
            }
        }
    //Now if there is no key mapping found for the character, then use the default TChar
    //APIs to find the case of the character.
    if(!isMappingFound)
        {
        caseCalculated = aCharacter.IsUpper()?EPtiCaseUpper:EPtiCaseLower;
        }
    return caseCalculated;
}
// ---------------------------------------------------------------------------
// CPtiEngineImpl::MakeCaseSequence
// 
// ---------------------------------------------------------------------------
//
void CPtiEngineImpl::MakeCaseSequence(TPtrC aWord)
	{
	iCaseBuffer.Zero();
	for (TInt i = 0; i < aWord.Length(); i++)
		{
		//Find the case for the character from the mapping
		TPtiTextCase characterCase = CaseForMappedCharacter(aWord[i]);
		iCaseBuffer.Append(characterCase);
		}
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::HandleCommandL
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::HandleCommandL(TPtiEngineCommand aCommand, TAny* aParams)
	{
	switch(aCommand)
	    {
	    case EPtiCommandSetCursorCrossedMaxLength:
	        iMaxEditorLengthCrossed = ETrue;
	        break;          
	    case EPtiCommandResetCursorCrossedMaxLength:
	        iMaxEditorLengthCrossed = EFalse;
	        break;
	        
	    default:
	        break;
	    }
	
	if (IsCore())
		{
		return Core()->HandleCommandL(aCommand, aParams);
		}

	if (!iCurrentLanguage)
		{
		return KErrNoActiveLanguage;
		}

	return KErrNoSuitableCore;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CandidatePage
// 
// ---------------------------------------------------------------------------
//
TPtrC CPtiEngineImpl::CandidatePage()
	{
	if (IsCore())
		{
		return Core()->GetCandidatePage();
		}

	return TPtrC(KNullDesC);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::NextCandidatePage
// 
// ---------------------------------------------------------------------------
//
TBool CPtiEngineImpl::NextCandidatePage()
	{
	if (IsCore())
		{
		return Core()->NextCandidatePage();
		}

	return EFalse;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::PreviousCandidatePage
// 
// ---------------------------------------------------------------------------
//
TBool CPtiEngineImpl::PreviousCandidatePage()
	{
	if (IsCore())
		{
		return Core()->PreviousCandidatePage();
		}

	return EFalse;	
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::MoreCandidatePages
// 
// ---------------------------------------------------------------------------
//
TBool CPtiEngineImpl::MoreCandidatePages()
	{
	if (IsCore())
		{
		return Core()->MoreCandidatePages();
		}

	return EFalse;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetCandidatePageLength
// 
// ---------------------------------------------------------------------------
//
void CPtiEngineImpl::SetCandidatePageLength(TInt aLength)
	{
	if (IsCore())
		{
		Core()->SetCandidatePageLength(aLength);
		}
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::LoadCoresInDefaultDirL
// 
// ---------------------------------------------------------------------------
//
void CPtiEngineImpl::LoadCoresInDefaultDirL(TBool aUseDefaultUserDictionary)
	{
	TInt i;
	CArrayFix<TInt>* array = CPtiCore::ListCoresLC();
	User::LeaveIfNull(array);
	for (i = 0; i < array->Count(); i++)
		{
		AddCoreL(TUid::Uid(array->At(i)), aUseDefaultUserDictionary);
		}

	CleanupStack::PopAndDestroy(); // array
	}



// ---------------------------------------------------------------------------
// CPtiEngineImpl::CompositionData
// 
// ---------------------------------------------------------------------------
//
MPtiEngineCompositionDataInterface* CPtiEngineImpl::CompositionData()
	{
	if (IsCore())
		{
		return Core()->CompositionData();
		}

	return NULL;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::DefaultUserDictionary
// 
// ---------------------------------------------------------------------------
//
MPtiUserDictionary* CPtiEngineImpl::DefaultUserDictionary(TPtiEngineInputMode aMode)
	{	
	return DefaultUserDictForCore(CoreForInputMode( aMode ));	
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetUserDictionaryEntry
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::GetUserDictionaryEntry(TInt aIndex, MPtiUserDictionaryEntry& aResult)
	{
    
    if ( !iCurrentLanguage )
        {
        return KErrNoActiveLanguage;
        }
    
	CPtiUserDictionary* dict = DefaultUserDictForCore(Core());

	if (dict)
		{
		return Core()->GetUserDictionaryEntry(aIndex, aResult, dict);
		}

	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::DefaultUserDictForCore
// 
// ---------------------------------------------------------------------------
//
CPtiUserDictionary* CPtiEngineImpl::DefaultUserDictForCore(CPtiCore* aCore)
	{
	if (!SupportsUserDictionaries(aCore))
		{
		return NULL;
		}

	MPtiCoreInfo* info = aCore->GetCoreInfo();
	if (info)
		{
		for (TInt i = 0; i < iUserDictionaries.Count(); i++)
			{
			if ((iUserDictionaries[i]->CoreUID() == info->Uid()) &&
				(iUserDictionaries[i]->IsDefault()))
				{
				return iUserDictionaries[i];
				}
			}
		}

	return NULL;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::ReadUidFromUserDictionaryFileL
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::ReadUidFromUserDictionaryFileL(const TDesC& aFileName)
	{
	TInt res = 0;	

	RFs fsSession;
	User::LeaveIfError( fsSession.Connect() );
	CleanupClosePushL( fsSession );

	RFile file;
	User::LeaveIfError(file.Open(fsSession, aFileName, EFileRead));
	CleanupClosePushL(file);			

	TInt32  uidtmp[1];
	TPtr8 uidP((TUint8*)uidtmp, sizeof(TInt32));
	uidP.Set((TUint8*)uidtmp, sizeof(TInt32), sizeof(TInt32));
	User::LeaveIfError(file.Read(uidP, sizeof(res)));
	res = uidtmp[0];

	CleanupStack::PopAndDestroy(2); // fsSession, file

	return res;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetPhoneticSpelling
// 
// ---------------------------------------------------------------------------
//
TPtrC CPtiEngineImpl::GetPhoneticSpelling(TInt aIndex) const 
	{
	if (IsCore())
		{
#ifdef RD_INTELLIGENT_TEXT_INPUT
		if(iInputMode ==  EPtiEngineInputModeZhuyinMiniQwertyFind)
		    {
		    if(iZhuyinSymbols.Length()>0)
		        {
		        return (iZhuyinSymbols.Ptr()+ iIndex);
		        }
		    }
#endif
		return Core()->GetPhoneticSpelling(aIndex);
		}

	return TPtrC(KNullDesC);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::PhoneticSpellingCount
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::PhoneticSpellingCount() const	
	{
	if (IsCore())
		{
		return Core()->PhoneticSpellingCount();
		}

	return 0;
	}		
	

// ---------------------------------------------------------------------------
// CPtiEngineImpl::SelectPhoneticSpelling
// 
// ---------------------------------------------------------------------------
//
TBool CPtiEngineImpl::SelectPhoneticSpelling(TInt aIndex)
	{
	if (IsCore())
		{
		return Core()->SelectPhoneticSpelling(aIndex);
		}

	return EFalse;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::SelectedPhoneticSpelling
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::SelectedPhoneticSpelling() const
	{
	if (IsCore())
		{
		return Core()->SelectedPhoneticSpelling();
		}

	return -1;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::ReadingTextL
// 
// ---------------------------------------------------------------------------
//
TPtrC CPtiEngineImpl::ReadingTextL()
	{	
	CPtiCoreLanguage* lang = NULL;
	for (TInt i = 0; i < iLanguages->Count(); i++)
		{
		if (iLanguages->At(i)->LanguageCode() == ELangJapanese)
			{
			lang = iLanguages->At(i);
			break;
			}
		}
		
	if (lang)
		{
		MPtiCore* core = lang->GetCore(EPtiEngineHiraganaKanji);
		if (core)
			{
			return core->ReadingTextL();
			}
		}
	else
		{
		if (IsCore())
			{
			return Core()->ReadingTextL();
			}
		}

	return TPtrC(KNullDesC);
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::EnableToneMarks
// 
// ---------------------------------------------------------------------------
//
void CPtiEngineImpl::EnableToneMarks(TBool aValue)
	{
	TInt ignore = 0;
	if (IsCore())
		{
		if (aValue)
			{
			TRAP(ignore, Core()->HandleCommandL(EPtiCommandEnableToneMarks, NULL));
			}
		else
			{
			TRAP(ignore, Core()->HandleCommandL(EPtiCommandDisableToneMarks, NULL));
			}
		}
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::ResetToneMark
// 
// ---------------------------------------------------------------------------
//
void CPtiEngineImpl::ResetToneMark()
	{
	TInt ignore = 0;
	if (IsCore())
		{
		TRAP(ignore, Core()->HandleCommandL(EPtiCommandResetToneMark, NULL));
		}
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::ToneMark
// 
// ---------------------------------------------------------------------------
//
TBool CPtiEngineImpl::ToneMark(TText& aToneMark) const
	{
	if (IsCore())
		{
		return Core()->ToneMark(aToneMark);
		}

	return EFalse;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::IsToneMarkValidForSpelling
// 
// ---------------------------------------------------------------------------
//
TBool CPtiEngineImpl::IsToneMarkValidForSpelling() const
	{
	if (IsCore())
		{
		return Core()->IsToneMarkValidForSpelling();
		}

	return EFalse;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::IncrementToneMark
// 
// ---------------------------------------------------------------------------
//
TBool CPtiEngineImpl::IncrementToneMark(TBool aOverrideInvalid)
	{
	TInt ret = 0;
	TInt ignore = 0;

	if (IsCore())
		{
		if (aOverrideInvalid)
			{
			TRAP(ignore, ret = Core()->HandleCommandL(EPtiCommandIncrementToneMarkOverrideInvalid, NULL));
			if (ret == KErrNone)
				{
				return ETrue;
				}
			}
		else
			{
			TRAP(ignore, ret = Core()->HandleCommandL(EPtiCommandIncrementToneMark, NULL));
			if (ret == KErrNone)
				{
				return ETrue;
				}
			}
		}

	return EFalse;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetPredictiveChineseChar
// 
// ---------------------------------------------------------------------------
//
TBool CPtiEngineImpl::SetPredictiveChineseChar(const TDesC& aChar)
	{
	if (IsCore())
		{
		return Core()->SetPredictiveChineseChar(aChar);
		}

	return EFalse;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CreateNumericLanguageL
// 
// ---------------------------------------------------------------------------
//
CPtiCoreLanguage* CPtiEngineImpl::CreateNumericLanguageL(CPtiCore* aCore)
	{
	CPtiCoreLanguage* lang = CPtiCoreLanguage::NewL();

	lang->SetLanguageCode(ELangNumeric);
	lang->SetCore(aCore, EPtiEngineNumeric);
	lang->SetLocalizedName(KNumericLangName);

	return lang;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetModeNameIndexL
// 
// ---------------------------------------------------------------------------
//
void CPtiEngineImpl::GetModeNameIndexL(TPtiChineseVariant aVariant, RArray<TInt>& aResult)
	{
	TResourceReader reader;	
	TFileName fileName;

	TParse* fp = new(ELeave) TParse(); 
	fp->Set(KCEResourceFile, &KDC_RESOURCE_FILES_DIR, NULL); 
	fileName.Copy(fp->FullName());
	delete fp;

	// Resource reading is done without coe or eikon env.
	RFs fsSession;
	User::LeaveIfError( fsSession.Connect() );
	CleanupClosePushL( fsSession );

	RResourceFile rsFile;
	rsFile.OpenL( fsSession, fileName );
	CleanupClosePushL( rsFile );

	TInt resId;

	switch (aVariant)
		{
		case EPtiChineseVariantHk:
			 resId = R_PTI_TW_INDICES;
             break;
		case EPtiChineseVariantTw:
			 resId = R_PTI_HK_INDICES;
             break;
		default:
			 resId = R_PTI_PRC_INDICES;
			 break;	
		}
	// Read mode name index table.
	HBufC8* rBuffer = rsFile.AllocReadLC(resId & KCCMask); // Remove offset from id
	reader.SetBuffer( rBuffer );

	TInt value;
	for (TInt i = 0; i < 6; i++)
		{
		value = reader.ReadInt16();
		if (value != -1)
			{
			User::LeaveIfError(aResult.Append(value));
			}
		}

	CleanupStack::PopAndDestroy(3);   // fsSession, rsFile, rBuffer
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::IsValidKey
// 
// ---------------------------------------------------------------------------
//
TBool CPtiEngineImpl::IsValidKey(TPtiKey aKey) const
	{  	
    if (iFrontEnd->IsQwertyBasedMode(iInputMode) ||
	    iInputMode == EPtiEngineHalfQwerty ||
	    iInputMode == EPtiEngineHalfQwertyPredictive)
		{
        // Japanese Qwerty modes
        if (iCurrentLanguage->LanguageCode() == ELangJapanese)
            {
            if (iInputMode == EPtiEngineHiraganaKanjiQwerty ||
                iInputMode == EPtiEngineHiraganaKanjiPredictiveQwerty ||
                iInputMode == EPtiEngineKatakanaQwerty ||
                iInputMode == EPtiEngineFullWidthKatakanaQwerty ||
                iInputMode == EPtiEngineFullWidthNumericQwerty ||
                iInputMode == EPtiEngineFullWidthAlphabetQwerty ||
                iInputMode == EPtiEngineHiraganaQwerty ||
                iInputMode == EPtiEngineQwerty)
                {
                if (aKey == EPtiKeyQwertySpace)
                    {
                    return ETrue;
                    }
                }                
		    }			
		
		// Scan codes from this area are ok.
		if (aKey >= KDirectlyAllowedScanCodesStart && aKey <= KDirectlyAllowedScanCodesEnd)	
		    {
		    return ETrue;
		    }
		        
        // Also allow these		        
		if (aKey == EStdKeySquareBracketLeft ||   
		    aKey == EPtiKeyQwertyMinus ||
		    aKey == EPtiKeyQwertyComma ||
		    aKey == EPtiKeyQwertySemicolon ||
		    aKey == EPtiKeyQwertyFullstop ||
		    aKey == EPtiKeyQwertyHash ||
		    aKey == EPtiKeyQwertySlash ||
		    aKey == EPtiKeyQwertyApostrophe ||
		    aKey == EPtiKeyQwertySpace ||
		    aKey == EPtiKeyQwertyAtCharacter ||
		    aKey == EPtiKeyQwertyLeftShift
// ----> Half Qwerty
		    || aKey == EPtiKeyQwertyChr
// Half Qwerty <----
		    )
		       {
			   return ETrue;
			   }

		}
	else
		{
	    if ((aKey >= EPtiKey0 && aKey <= EPtiKey9) ||
		    (aKey == EPtiKeyStar) || (aKey == EPtiKeyHash))
		    {
		    return ETrue;	
	    	}
		}
				
	return EFalse;		    			
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetPhoneticSpellingsL
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::GetPhoneticSpellingsL(CDesCArray& aList)
	{
	aList.Reset();

	if (IsCore())
		{
		MPtiCore* core = Core();
		const TInt count = core->PhoneticSpellingCount();
		for (TInt i = 0; i < count; i++)
			{
			aList.AppendL(core->GetPhoneticSpelling(i + 1));
			}
		}	

	return aList.Count();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetChinesePhraseCandidatesL
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::GetChinesePhraseCandidatesL(CDesCArray& aList)
	{
	TBuf<KMaxPhraseBufferSize> candbuf;
	TInt start = 0, length = 0;

	aList.Reset();	
	
	if (IsCore())
		{
		if (iInputMode == EPtiEngineStroke)
			{
			TBuf<1> str;
			for (TInt j = 0; j < KMaxCandPages; j++)
				{
				candbuf.Copy(Core()->GetCandidatePage());
				for (TInt i = 0; i < candbuf.Length(); i++)
					{
					str.Zero();
					str.Append(candbuf[i]);
					aList.AppendL(str);
					}
				if (!NextCandidatePage())
					{
					break;
					}

				}
			}
		else
			{
			candbuf.Copy(Core()->GetCandidatePage());
			for (TInt i = 0; i < candbuf.Length(); i++)
				{
				if (candbuf[i] == KPhraseSeparator)
					{
					TPtrC segment(candbuf.Ptr() + start, length);
					aList.AppendL(segment);
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
				TPtrC segm(candbuf.Ptr() + start, length);
				aList.AppendL(segm);
				}

			}
		}	

	return aList.Count();
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetToneMark
// 
// ---------------------------------------------------------------------------
//
TBool CPtiEngineImpl::SetToneMark(TInt aToneMark)
	{
	if (IsCore())
		{
		return Core()->SetToneMark(aToneMark);
		}

	return EFalse;
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetAvailableLanguagesL
// 
// ---------------------------------------------------------------------------
//	
void CPtiEngineImpl::GetAvailableLanguagesL(RArray<TInt>& aResult)
	{
	aResult.Reset();
	
	const TInt count = iLanguages->Count();
	for (TInt i = 0; i < count; i++)
		{
		if (iLanguages->At(i)->LanguageCode() != ELangNumeric)
			{
			aResult.AppendL(iLanguages->At(i)->LanguageCode());
			}
		}	
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::CreateDefaultUserDictionaryFileL
// 
// ---------------------------------------------------------------------------
//	
void CPtiEngineImpl::CreateDefaultUserDictionaryFileL(TUid aCoreUid, TInt aSymbolClass)
	{
	TFileName fileName;
	
	CreateUserDictName(fileName, aCoreUid, aSymbolClass);
	
	RFs fsSession;
	User::LeaveIfError(fsSession.Connect()); // Start session
	CleanupClosePushL(fsSession);

	TEntry entry;
	TInt error;
	error = fsSession.Entry(fileName, entry);
	
	if (error == KErrPathNotFound)
		{
		error = fsSession.MkDirAll(KPtiDefaultUdbPath);
		User::LeaveIfError(error);
		error = fsSession.Entry(fileName, entry);		
		}
	
	if (error == KErrNotFound)
		{
		RFile file;
		User::LeaveIfError(file.Create(fsSession, fileName, EFileWrite));
		CleanupClosePushL(file);
		
		TInt32 uidtmp[1] = {aCoreUid.iUid};
		TPtr8 uidP((TUint8*)uidtmp, sizeof(TInt32));
		uidP.Set((TUint8*)uidtmp, sizeof(TInt32), sizeof(TInt32));
		User::LeaveIfError(file.Write(uidP, sizeof(TInt32)));
		
		TInt32 sctmp[1];
		sctmp[0] = aSymbolClass;
		TPtr8 scP((TUint8*)sctmp, sizeof(TInt32));
		scP.Set((TUint8*)sctmp, sizeof(TInt32), sizeof(TInt32));
		User::LeaveIfError(file.Write(scP, sizeof(TInt32)));
	
		CleanupStack::PopAndDestroy(); // file
		}
	else if (error != KErrNone)
		{		
		User::LeaveIfError( error );
		}

	CleanupStack::PopAndDestroy(); // fsSession		
	}

	
// ---------------------------------------------------------------------------
// CPtiEngineImpl::CreateUserDictName
// 
// ---------------------------------------------------------------------------
//				
void CPtiEngineImpl::CreateUserDictName(TDes& aResult, TUid aCoreUid, TInt aSymbolClass)
	{
	aResult.Zero();
	aResult.Append(KPtiDefaultUdbPath);
	aResult.AppendNumUC(aCoreUid.iUid, EHex);
	aResult.Append('_');
	aResult.AppendNumUC(aSymbolClass, EHex);
	aResult.Append(_L(".dat"));
	}	
	

// ---------------------------------------------------------------------------
// CPtiEngineImpl::CreateSecondaryDataFileName
// 
// ---------------------------------------------------------------------------
//	
void CPtiEngineImpl::CreateSecondaryDataFileName(TDes& aResult, TUid aCoreUid, TInt aIndex)	
	{
	aResult.Zero();
	aResult.Append(KPtiDefaultUdbPath);
	aResult.AppendNumUC(aCoreUid.iUid, EHex);
	aResult.Append('_');
	aResult.AppendNumUC(aIndex, EHex);
	aResult.Append('S');
	aResult.Append(_L(".dat"));	
	}


// ---------------------------------------------------------------------------
// CPtiEngineImpl::WriteSecondaryDataFileL
// 
// ---------------------------------------------------------------------------
//	
void CPtiEngineImpl::WriteSecondaryDataFileL(TUid aCoreUid, TInt aIndexNumber, HBufC8* aData)
	{
	TFileName fileName;
	CreateSecondaryDataFileName(fileName, aCoreUid, aIndexNumber);
	
	RFs fsSession;
	RFile file;
	User::LeaveIfError(fsSession.Connect()); // Start session
	CleanupClosePushL(fsSession);	
	
	TEntry entry;
	TInt error;
	error = fsSession.Entry(fileName, entry);
	
	// if path is not existent, create path and file.
	if (error == KErrPathNotFound) 
		{
		User::LeaveIfError(fsSession.MkDirAll(KPtiDefaultUdbPath));
		User::LeaveIfError(file.Create(fsSession, fileName, EFileWrite));
		}
    // if path is existent and file is not extent, crate file		
	else if (error == KErrNotFound)
		{
		User::LeaveIfError(file.Create(fsSession, fileName, EFileWrite));		
		}
	// if file is already existent, open it		
	else if (error == KErrNone)
		{
		User::LeaveIfError(file.Open(fsSession, fileName, EFileWrite));
		}
	else
		{
		User::Leave(error);		
		}
		
	CleanupClosePushL(file);	
	
	TPtr8 dataP((TUint8*)aData->Des().Ptr(), aData->Des().MaxSize());
	dataP.Set((TUint8*)aData->Des().Ptr(), aData->Des().Length(), aData->Des().MaxSize());	
	User::LeaveIfError(file.Write(dataP));	
	
	CleanupStack::PopAndDestroy(2); // fsSession, file
	}

// ---------------------------------------------------------------------------
// CPtiEngineImpl::ReadSecondaryDataFileL
// 
// ---------------------------------------------------------------------------
//		
HBufC8* CPtiEngineImpl::ReadSecondaryDataFileL(TUid aCoreUid, TInt aIndexNumber)
	{
	TFileName fileName;
	CreateSecondaryDataFileName(fileName, aCoreUid, aIndexNumber);
	
	RFs fsSession;
	User::LeaveIfError(fsSession.Connect()); // Start session
	CleanupClosePushL(fsSession);
	
	RFile file;
	TInt error = file.Open(fsSession, fileName, EFileRead);	
	if (error != KErrNone)
		{
		CleanupStack::PopAndDestroy();  // fsSession		
		return NULL;
		}
	
	CleanupClosePushL(file);
	
	TInt size(0);
	User::LeaveIfError(file.Size(size));  
	
	HBufC8* data = HBufC8::NewL(size);
	CleanupStack::PushL(data);
	
	TPtr8 bufP8( data->Des() );
	bufP8.SetLength(size);
	bufP8.FillZ();
	User::LeaveIfError(file.Read(bufP8, size));
		
	CleanupStack::Pop(); // data
	CleanupStack::PopAndDestroy(2);  // fsSession, file
		
	return data;
	}
	
	
// ---------------------------------------------------------------------------
// CPtiEngineImpl::MappingDataForKey
// 
// ---------------------------------------------------------------------------
// 			
void CPtiEngineImpl::MappingDataForKey(TPtiKey aKey, TDes& aResult, TPtiTextCase aCase)
	{
	aResult.Zero();
		
	if (iCurrentLanguage)	
		{
		if( EPtiKeyboardHalfQwerty != KeyboardType() )
			{
			if (aCase == EPtiCaseChrLower)
				{
				aCase = EPtiCaseLower;
				}
			else if (aCase == EPtiCaseChrUpper)
				{
				aCase = EPtiCaseUpper;
				}
			}
			
							
		switch (iInputMode)
			{
			case EPtiEngineNumeric:	
				 {
				 CPtiCoreLanguage* lang = static_cast<CPtiCoreLanguage*>(GetLanguage(ELangNumeric));
				 if (lang)	
				 	{
					CPtiKeyMappings* maps = static_cast<CPtiKeyMappings*>(lang->GetKeymappings());					
					if (maps)
						{
	 					maps->GetDataForKey(aKey, aResult, aCase);				 									
						}
				 	}				 							 
				 }
				 break;								     
			case EPtiEngineMultitapping:	
			case EPtiEnginePredictive:	
				 {
				 CPtiKeyMappings* maps = static_cast<CPtiKeyMappings*>(iCurrentLanguage->GetKeymappings());
				 
				 if( maps != NULL )
				     {
				     maps->GetDataForKey(aKey, aResult, aCase);
				     }
				 }
				 break;
		    // Predictive QWERTY (XT9) changes ---->
			case EPtiEngineQwertyPredictive: // fall through
			// Predictive QWERTY (XT9) changes <----
			case EPtiEngineStrokePhraseQwerty:
			case EPtiEnginePinyinPhraseQwerty:
			case EPtiEngineZhuyinPhraseQwerty:
			case EPtiEngineInputModeZhuyinMiniQwertyFind:
			case EPtiEngineNormalCangjieQwerty:
			case EPtiEngineEasyCangjieQwerty:
			case EPtiEngineAdvCangjieQwerty:
			case EPtiEngineQwerty:
     			 {
		     	 CPtiQwertyKeyMappings* maps = static_cast<CPtiQwertyKeyMappings*>(iCurrentLanguage->GetQwertyKeymappings());
				 if( maps != NULL )
				     {
		     	     maps->GetDataForKey(aKey, aResult, aCase);
				     }
				 }     			
			     break;							
     		case EPtiEnginePinyinPhraseHalfQwerty:
     		case EPtiEngineStrokePhraseHalfQwerty:    
     		case EPtiEngineZhuyinPhraseHalfQwerty:
			case EPtiEngineHalfQwerty:
			case EPtiEngineHalfQwertyPredictive:
     			 {
		     	 CPtiHalfQwertyKeyMappings* maps = static_cast<CPtiHalfQwertyKeyMappings*>(iCurrentLanguage->GetHalfQwertyKeymappings());
				 if( maps != NULL )
				     {
		     	     maps->GetDataForKey(aKey, aResult, aCase);
				     }
				 }     			
			     break;							
			default:
			   break;
			}		
		}
	}
								
		
// ---------------------------------------------------------------------------
// CPtiEngineImpl::MapNumericChar
// 
// ---------------------------------------------------------------------------
// 			
TBool CPtiEngineImpl::MapNumericChar(CPtiMappings* aMaps, TPtiNumericKeyBinding& aBinding,
                                     TPtiKey aKey, TPtiTextCase aCase, TBool aUseExtendedSet,
                                     TPtiEngineInputMode aMode)	
	{
	const TInt KMaxNumCharsPerCase = 50;
	TBuf<KMaxNumCharsPerCase> keys;		
				
	TPtiKey convKey = aKey;				
#if defined(__WINS__)
	if (convKey == EStdKeyNkpPlus)
		{
		convKey = EPtiKeyQwertyPlus;
		}
#endif		
 			    						
	keys.Copy(aMaps->DataForKeyLocal(convKey, aCase, aMode).Left(keys.MaxLength()));				
				
	if (keys.Length() > 0)
		{
		TUint16 firstChar = keys[0];
		switch (firstChar)			
			{
			case KPtiPinyinMarker:
			case KPtiStrokeMarker:	
			case KPtiZhuyinMarker:
			case KPtiCangjieMarker:	
			case KPtiGetAllMarker:	
				 if (keys.Length() > 1)							
				 	{
				 	firstChar = keys[1];
				 	}				 	
				 break;					 	
			default:
				break; 			
			}
			
		// Handle Chinese versions of these characters as a special case.			
		if (firstChar == KPtiChineseHash) 			
			{
			firstChar = '#';
			}
		else if (firstChar == KPtiChinesePlus)			
			{
			firstChar = '+';
			}
		else if (firstChar == KPtiChineseStar)			
			{
			firstChar = '*';
			}  
			
		TInt loc = 0;			
		if (aUseExtendedSet)
			{
			loc = KAllAllowedNumericChars().Locate(firstChar);
			}					
		else
			{
			loc = KAllowedNumericChars().Locate(firstChar);
			}
				
		if (loc != KErrNotFound)			
			{
			aBinding.iChar = firstChar;
			aBinding.iKey = aKey;
			aBinding.iCase = aCase;
			return ETrue;
			}
		}
		
	return EFalse;
	}
	
				
// ---------------------------------------------------------------------------
// CPtiEngineImpl::NumericModeKeysForQwertyL
// 
// ---------------------------------------------------------------------------
// 		
void CPtiEngineImpl::NumericModeKeysForQwertyL(TInt aLanguage,
                                               RArray<TPtiNumericKeyBinding>& aResult,
                                               TBool aUseExtendedSet,
                                               TPtiKeyboardType aKeyboardType)
	{
	aResult.Reset();		
	TPtiNumericKeyBinding bind;		
	
	TPtiKeyboardType keyboardType = aKeyboardType;
	if (keyboardType == EPtiKeyboardNone)
	    {
	    keyboardType = ActiveKeyboardTypeL();
	    
	    if (keyboardType == EPtiKeyboard12Key ||
	        keyboardType == EPtiKeyboardNone)
	        {
	        keyboardType = FindFirstAvailableQwertyKeyboardType(aLanguage);	        
	        }	    
	    }		 
	
	if (keyboardType == EPtiKeyboard12Key ||
	    keyboardType == EPtiKeyboardNone)
	    {
	    // No qwerty data available, can't go on.
	    return;
	    }
	
	if (aLanguage == ELangJapanese)	
		{		
		// Japanese doesn't have qwerty keymappings file, handle it
		// with hardcoded data.	
		TInt count = KNumBindingsForNonLatinNumberLanguages;		
		if (aUseExtendedSet)
			{
			count = KNumExtendedBindingsForNonLatinNumberLanguages;			
			}
		for (TInt i = 0; i < count; i++)
			{
			bind = NumericModeKeysForNonLatinNumberLanguages[i];
			User::LeaveIfError(aResult.Append(bind));		
			}	
		
		return;			
		}
		
    TPtiEngineInputMode mode = EPtiEngineQwerty;
    if (keyboardType == EPtiKeyboardHalfQwerty)		
        {
        mode = EPtiEngineHalfQwerty;
        }
				
	CPtiCoreLanguage *lang = static_cast<CPtiCoreLanguage*>(GetLanguage( aLanguage ));
	if (lang)
		{
		CPtiMappings* mappings = NULL;
	    CPtiQwertyKeyMappings* qmappings = NULL;
	    TPtiKeyboardType origKeyb = EPtiKeyboardNone;		
				
		if (mode == EPtiEngineHalfQwerty)
		    {
   	        CPtiHalfQwertyKeyMappings* hqmappings = 
		           static_cast<CPtiHalfQwertyKeyMappings*>(lang->GetHalfQwertyKeymappings());
            mappings = hqmappings;
            if (hqmappings)
                {
                // First insert "numeric_mode_key" definitions given in keymap data file.
			    hqmappings->GetNumericModeKeysFromDataL(aResult);                
                }		      		    		    
		    }
        else		    
            {
           	qmappings = static_cast<CPtiQwertyKeyMappings*>(lang->GetQwertyKeymappings());
            mappings = qmappings;
            if (qmappings)
                {
                origKeyb = qmappings->KeyboardType();                
                qmappings->SetKeyboardType(keyboardType);                 
                // First insert "numeric_mode_key" definitions given in keymap data file.
			    qmappings->GetNumericModeKeysFromDataL(aResult);                
                }		           
            }
			 
		if (mappings)		
			{	                         		  												
			// if not "extended" set, filter out possible extra definitions.
			if (!aUseExtendedSet)
				{
				for (TInt zz = 0; zz < aResult.Count(); zz++)
					{
					if (KExtendedNumericModeChars().Locate(aResult[zz].iChar) != KErrNotFound)
						{
						aResult.Remove(zz);
						zz--;
						}				
					}				
				}
			
			// Then use actual keymap data for remaining characters.			
			for (TInt i = 0; qwertyKeys[i] != 0; i++)
				{
				if (MapNumericChar(mappings, bind, qwertyKeys[i], EPtiCaseLower, aUseExtendedSet, mode))
					{
					User::LeaveIfError(aResult.Append(bind));										
					}
				if (MapNumericChar(mappings, bind, qwertyKeys[i], EPtiCaseUpper, aUseExtendedSet, mode))
					{
					User::LeaveIfError(aResult.Append(bind));
					}
                if (MapNumericChar(mappings, bind, qwertyKeys[i], EPtiCaseFnLower, aUseExtendedSet, mode))
					{
					User::LeaveIfError(aResult.Append(bind));							
					}
                if (MapNumericChar(mappings, bind, qwertyKeys[i], EPtiCaseFnUpper, aUseExtendedSet, mode))
					{
					User::LeaveIfError(aResult.Append(bind));
					}
				}						
				
            if (qmappings && keyboardType != origKeyb)			
                {
                qmappings->SetKeyboardType(origKeyb);
                }				
			}			      
		}		
	}	
	
	
// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetNumericModeKeysForQwertyL
// 
// ---------------------------------------------------------------------------
// 		
void CPtiEngineImpl::GetNumericModeKeysForQwertyL(TInt aLanguage,
                                                  RArray<TPtiNumericKeyBinding>& aResult)
	{			
	NumericModeKeysForQwertyL(aLanguage, aResult, EFalse);			
	}
			
	
// ---------------------------------------------------------------------------
// CPtiEngineImpl::ExtendedNumericModeKeysForQwertyL
// 
// ---------------------------------------------------------------------------
// 		
const RArray<TPtiNumericKeyBinding>& CPtiEngineImpl::ExtendedNumericModeKeysForQwertyL(TInt aLanguage)
	{
	if (iExtendedNumricSetLang != aLanguage)
		{		
		NumericModeKeysForQwertyL(aLanguage, iExtendedNumricSet, ETrue);
		iExtendedNumricSetLang = aLanguage;			
		}
	
	return iExtendedNumricSet;
	}
	
	
// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetHwrRecognizerL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
MPtiHwrRecognizer* CPtiEngineImpl::GetHwrRecognizerL(TLanguage aLanguage)
    {
    if ((iCurrentHwrImpl == NULL) ||
        (iCurrentHwrImpl->iHwrLanguagesList.Find(aLanguage) == KErrNotFound))
        {
        if (iCurrentHwrImpl != NULL)
            {
            delete iCurrentHwrImpl;
            iCurrentHwrImpl = NULL;
            }
        
        iCurrentHwrImpl = new (ELeave) CPtiHwrRecognizer;
        iCurrentHwrImpl->iHwrRecognizer = CHwrRecognizer::NewL(aLanguage, iCurrentHwrImpl->iHwrImpId);
        
        CHwrRecognizer::ListGivenAvailableLanguagesL(iCurrentHwrImpl->iHwrImpId, iCurrentHwrImpl->iHwrLanguagesList);
        }

    if (iCurrentHwrImpl != NULL)
        {
        return iCurrentHwrImpl->iHwrRecognizer;
        } 
    else
        {
        return NULL;    
        }    
    }
    
    
// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetHwrRecognizerL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
MPtiHwrRecognizer* CPtiEngineImpl::GetHwrRecognizerL(TInt aImpId)
    {
    if ((iCurrentHwrImpl == NULL) ||
        (iCurrentHwrImpl->iHwrImpId != TUid::Uid(aImpId)))
        {
        if (iCurrentHwrImpl != NULL)
            {
            delete iCurrentHwrImpl;
            iCurrentHwrImpl = NULL;
            }

        iCurrentHwrImpl = new (ELeave) CPtiHwrRecognizer;
        iCurrentHwrImpl->iHwrRecognizer = CHwrRecognizer::NewL(aImpId);
        iCurrentHwrImpl->iHwrImpId = TUid::Uid(aImpId);
        
        CHwrRecognizer::ListGivenAvailableLanguagesL(iCurrentHwrImpl->iHwrImpId, iCurrentHwrImpl->iHwrLanguagesList);
        }

    if (iCurrentHwrImpl != NULL)
        {
        return iCurrentHwrImpl->iHwrRecognizer;
        } 
    else
        {
        return NULL;    
        }    
    }
    
    
// ---------------------------------------------------------------------------
// CPtiEngineImpl::ListHwrRecognizerL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
RArray<TUid>& CPtiEngineImpl::ListHwrRecognizerL(TLanguage aLanguage)
    {
    iImpIdList.Reset();
    
    CHwrRecognizer* hwrRecognizer = new(ELeave)CHwrRecognizer;
    CleanupStack::PushL(hwrRecognizer);
    hwrRecognizer->ListImplementationsL(aLanguage, iImpIdList);
    CleanupStack::Pop(hwrRecognizer);
    delete hwrRecognizer;
    
    return iImpIdList;
    }


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetHwrAvailableLanguagesL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
void CPtiEngineImpl::GetHwrAvailableLanguagesL(RArray<TInt>& aResult)
    {
    CHwrRecognizer* hwrRecognizer = new(ELeave)CHwrRecognizer;
    CleanupStack::PushL(hwrRecognizer);
    hwrRecognizer->ListAvailableLanguagesL(aResult);
    CleanupStack::Pop(hwrRecognizer);
    delete hwrRecognizer;
    }


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetCandidatesByInputString
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C HBufC* CPtiEngineImpl::GetCandidatesByInputString(const TDesC& aInputString, 
                                                           RPointerArray<HBufC>& aList, 
                                                           const TBool aIsPredictive)    
    {
	if (IsCore())   
		{
		return Core()->GetCandidatesByInputString(aInputString, aList, aIsPredictive);
		}
    else
        {
        return NULL;
        }
    }


// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetCandidatesByInputString
// 
// ---------------------------------------------------------------------------
// 
TInt CPtiEngineImpl::SetAutoSubstitution(TBool aStatus)
    {
	if (IsCore())
		{
        TInt err = KErrNone;
        TInt ret = 0;		
		TRAP(err, ret = Core()->HandleCommandL(EPtiCommandSetAutoSubstitution, (TAny*)aStatus));
		
	    if (err != KErrNone)
            {
            return err;
            }
                                		
		return ret;
		}

	if (!iCurrentLanguage)
		{
		return KErrNoActiveLanguage;
		}

	return KErrNoSuitableCore;           
    }


// ---------------------------------------------------------------------------
// CPtiEngineImpl::AddAutoSubstitutionEntry
// 
// ---------------------------------------------------------------------------
//    
TInt CPtiEngineImpl::AddAutoSubstitutionEntry(const TDesC& aShortcut,
                                              const TDesC& aSubstitution)
    {
	if (IsCore())
		{
        TPtiAutoSubstBinding bind;
        bind.iShortcut.Copy(aShortcut.Left(bind.iShortcut.MaxLength()));		
        bind.iSubstitution.Copy(aSubstitution.Left(bind.iSubstitution.MaxLength()));		

        TInt err = KErrNone;
        TInt ret = 0;
        TRAP(err, ret = Core()->HandleCommandL(EPtiCommandAddAutoSubstEntry, &bind));
        if (err != KErrNone)
            {
            return err;
            }
                                		
		return ret;
		}

	if (!iCurrentLanguage)
		{
		return KErrNoActiveLanguage;
		}

	return KErrNoSuitableCore;   
    }

     
// ---------------------------------------------------------------------------
// CPtiEngineImpl::DeleteAutoSubstitutionEntry
// 
// ---------------------------------------------------------------------------
//      
TInt CPtiEngineImpl::DeleteAutoSubstitutionEntry(const TDesC& aShortcut)
    {
	if (IsCore())
		{
        TPtiAutoSubstBinding bind;
        bind.iShortcut.Copy(aShortcut.Left(bind.iShortcut.MaxLength()));				
		
   	    TInt err = KErrNone;
        TInt ret = 0;
		
		TRAP(err, ret = Core()->HandleCommandL(EPtiCommandDeleteAutoSubstEntry, &bind));
        if (err != KErrNone)
            {
            return err;
            }
                                		
		return ret;		
		}

	if (!iCurrentLanguage)
		{
		return KErrNoActiveLanguage;
		}

	return KErrNoSuitableCore;   
    }

     
// ---------------------------------------------------------------------------
// CPtiEngineImpl::NumberOfAutoSubstitutionEntries
// 
// ---------------------------------------------------------------------------
//            
TInt CPtiEngineImpl::NumberOfAutoSubstitutionEntries() const
    {
  	if (IsCore())
		{
        TInt err = KErrNone;
        TInt ret = 0;		
		
		TRAP(err, ret = Core()->HandleCommandL(EPtiCommandNumberOfAutoSubstEntries, NULL));
        if (err != KErrNone)
            {
            return 0;
            }
                                		
		return ret;						
		}

	if (!iCurrentLanguage)
		{
		return KErrNoActiveLanguage;
		}

	return KErrNoSuitableCore;   
    }

     
// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetAutoSubstitutionEntry
// 
// ---------------------------------------------------------------------------
//
TInt CPtiEngineImpl::GetAutoSubstitutionEntry(TInt aIndex, TDes& aShortcut,
                                              TDes& aSubstitution)
    {
	if (IsCore())
		{		
        TPtiAutoSubstBinding bind;
        bind.iIndex = aIndex;
        aShortcut.Zero();    
        aSubstitution.Zero();    
        
        TInt err = KErrNone;
        TInt ret = 0;		        
        
		TRAP(err, ret = Core()->HandleCommandL(EPtiCommandGetAutoSubstEntry, &bind));
		
        if (err != KErrNone)
            {
            return err;
            }
            
        aShortcut.Copy(bind.iShortcut.Left(aShortcut.MaxLength()));                
        aSubstitution.Copy(bind.iSubstitution.Left(aSubstitution.MaxLength()));           
                                		
		return ret;				
		}

	if (!iCurrentLanguage)
		{
		return KErrNoActiveLanguage;
		}

	return KErrNoSuitableCore;   
    }


// ---------------------------------------------------------------------------
// CPtiEngineImpl::KeyboardType
// 
// ---------------------------------------------------------------------------
//                                                                                    
TPtiKeyboardType CPtiEngineImpl::KeyboardType() const
    {
/*    CPtiMappings* maps = NULL;
    
   	if (iCurrentLanguage)
		{
		if (iFrontEnd->IsQwertyBasedMode(iInputMode))
		    {
            maps = static_cast<CPtiQwertyKeyMappings *>(iCurrentLanguage->GetQwertyKeymappings());                		   
		    }
        else if (iInputMode == EPtiEngineHalfQwerty ||
            iInputMode == EPtiEngineHalfQwertyPredictive)
            {
            maps = static_cast<CPtiQwertyKeyMappings *>(iCurrentLanguage->GetHalfQwertyKeymappings());                		                                                                   
            }
        else
            {
            maps = static_cast<CPtiQwertyKeyMappings *>(iCurrentLanguage->GetKeymappings());                		               
            }            		    		    
		}    
		
    if (maps)
        {
        return maps->KeyboardType();
        }
		    		    
    return EPtiKeyboardNone;*/
	TInt keyboardLayout = 0;
    
    RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardLayout);

    if (keyboardLayout <= 0 ||
        keyboardLayout > EPtiKeyboardMaxLayout - 1)
        {
        return EPtiKeyboardNone;
        }
        
    return (TPtiKeyboardType)keyboardLayout; 
    }
     
    
// ---------------------------------------------------------------------------
// CPtiEngine::KeyboardType
// 
// ---------------------------------------------------------------------------
//                           
TInt CPtiEngineImpl::SetKeyboardType(TPtiKeyboardType aType)
    {
	if (!iCurrentLanguage)
		{
		return KErrNoActiveLanguage;
		}    
               
    switch (aType)    
        {
        case EPtiKeyboard12Key:  
             if (iInputMode == EPtiEngineMultitapping ||             
                 iInputMode == EPtiEnginePredictive ||               
                 iInputMode == EPtiEngineWordCompletion ||   
                 iInputMode == EPtiEngineNumeric ||                    
                 iInputMode == EPtiEnginePinyin ||                           
                 iInputMode == EPtiEngineStroke ||                   
                 iInputMode == EPtiEngineZhuyin ||                    
                 iInputMode == EPtiEngineHiraganaKanji ||            
                 iInputMode == EPtiEngineHiraganaKanjiPredictive ||  
                 iInputMode == EPtiEngineKatakana ||                  
                 iInputMode == EPtiEngineFullWidthKatakana ||        
                 iInputMode == EPtiEngineFullWidthNumeric ||         
                 iInputMode == EPtiEngineFullWidthAlphabet ||        
                 iInputMode == EPtiEngineHiragana ||                  
                 iInputMode == EPtiEnginePinyinByPhrase ||                    
#ifdef RD_HINDI_PHONETIC_INPUT
                 iInputMode == EPtiEngineInputModeIndicPhoneticMultitap ||                                                                      
#endif                                
                 iInputMode == EPtiEngineZhuyinByPhrase) 
                 {
                 return KErrNone;               
                 }
             break;                    

        case EPtiKeyboardQwerty4x12:
        case EPtiKeyboardQwerty4x10: 
        case EPtiKeyboardQwerty3x11:
        case EPtiKeyboardCustomQwerty:        
             if (iFrontEnd->IsQwertyBasedMode(iInputMode))
                {
                CPtiQwertyKeyMappings *qmaps = static_cast<CPtiQwertyKeyMappings *>(iCurrentLanguage->GetQwertyKeymappings()); 
                if (qmaps)
                    {                                  
                    if (qmaps->SetKeyboardType(aType) == KErrNone)                   
                        {
                        return KErrNone;
                        }                       
                    }                
                }
             break;           
        case EPtiKeyboardHalfQwerty: 
                if (iInputMode == EPtiEngineHalfQwerty ||
                    iInputMode == EPtiEngineHalfQwertyPredictive ||
                    iInputMode == EPtiEnginePinyinPhraseHalfQwerty)
                    {
                    return KErrNone;                    
                    }
             break;                        
        default:
             break;
        }    
    
    return KErrNotSupported;
    }


// ---------------------------------------------------------------------------
// CPtiEngineImpl::KeyboardTypesSupportedByLanguage
// 
// ---------------------------------------------------------------------------
//
void CPtiEngineImpl::KeyboardTypesSupportedByLanguageL(TInt aLanguage,
                                                       RArray<TPtiKeyboardType>& aResult)
    {
    CPtiCoreLanguage* lang = NULL;
    
    if (iCurrentLanguage && iCurrentLanguage->LanguageCode() == aLanguage)
        {
        lang = iCurrentLanguage;
        }
    else
        {
        lang = static_cast<CPtiCoreLanguage*>(GetLanguage(aLanguage));        
        }
    
    if (lang)
        {       
        TInt tmpSize = 0;
        
        CPtiKeyMappings* maps = static_cast<CPtiKeyMappings*>(lang->GetKeymappings());
        if (maps &&
            maps->KeyMapData() &&
            maps->KeyMapData()->KeyData(EPtiKeyboard12Key, tmpSize))
            {
            User::LeaveIfError(aResult.Append(EPtiKeyboard12Key));
            }
        
        CPtiQwertyKeyMappings* qmaps = static_cast<CPtiQwertyKeyMappings*>(lang->GetQwertyKeymappings());
        if (qmaps)
            {
        
            CPtiKeyMapData* mapData = NULL;
            if( maps != NULL )
                {
                mapData = maps->KeyMapData();
                }
            if (mapData)
                {
                if (mapData->KeyData(EPtiKeyboardQwerty4x12, tmpSize))
                    {
                    User::LeaveIfError(aResult.Append(EPtiKeyboardQwerty4x12));
                    }
                if (mapData->KeyData(EPtiKeyboardQwerty4x10, tmpSize))
                    {
                    User::LeaveIfError(aResult.Append(EPtiKeyboardQwerty4x10));
                    }   
                if (mapData->KeyData(EPtiKeyboardQwerty3x11, tmpSize))
                    {
                    User::LeaveIfError(aResult.Append(EPtiKeyboardQwerty3x11));
                    }                                                 
                if (mapData->KeyData(EPtiKeyboardCustomQwerty, tmpSize))
                    {
                    User::LeaveIfError(aResult.Append(EPtiKeyboardCustomQwerty));
                    }                            
                }
            }
            
        CPtiHalfQwertyKeyMappings* hmaps = static_cast<CPtiHalfQwertyKeyMappings*>(lang->GetHalfQwertyKeymappings());
        if (hmaps &&
            hmaps->KeyMapData() &&
            hmaps->KeyMapData()->KeyData(EPtiKeyboardHalfQwerty, tmpSize))
            {
            User::LeaveIfError(aResult.Append(EPtiKeyboardHalfQwerty));
            }            
        }                
    }


// ---------------------------------------------------------------------------
// CPtiEngineImpl::ActiveKeyboardTypeL
// 
// ---------------------------------------------------------------------------
//
TPtiKeyboardType CPtiEngineImpl::ActiveKeyboardTypeL()
    {    
    // Qwerty Keyboard layout
    TInt keyboardLayout = 0;
    
    RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardLayout);

    if (keyboardLayout <= 0 ||
        keyboardLayout > EPtiKeyboardMaxLayout - 1)
        {
        return EPtiKeyboardNone;
        }
        
    return (TPtiKeyboardType)keyboardLayout;
    }

        
// ---------------------------------------------------------------------------
// CPtiEngineImpl::GetNumericModeKeysForQwertyL
// 
// ---------------------------------------------------------------------------
//
void CPtiEngineImpl::GetNumericModeKeysForQwertyL(TInt aLanguage,
   	                                              RArray<TPtiNumericKeyBinding>& aResult,
 	                                              TPtiKeyboardType aKeyboardType)
    {
	NumericModeKeysForQwertyL(aLanguage, aResult, EFalse, aKeyboardType);	        
    }
  
  
// ---------------------------------------------------------------------------
// CPtiEngineImpl::ExtendedNumericModeKeysForQwertyL
// 
// ---------------------------------------------------------------------------
//   
const RArray<TPtiNumericKeyBinding>& CPtiEngineImpl::ExtendedNumericModeKeysForQwertyL(TInt aLanguage,
                                                                                       TPtiKeyboardType aKeyboardType)
    {
	if (iExtendedNumricSetLang != aLanguage)
		{		
		NumericModeKeysForQwertyL(aLanguage, iExtendedNumricSet, ETrue, aKeyboardType);
		iExtendedNumricSetLang = aLanguage;			
		}
	
	return iExtendedNumricSet;        
    }        
        

// ---------------------------------------------------------------------------
// CPtiEngineImpl::FindFirstAvailableQwertyKeyboardType
// 
// ---------------------------------------------------------------------------
//          
TPtiKeyboardType CPtiEngineImpl::FindFirstAvailableQwertyKeyboardType(TInt aLang) const
    {
    CPtiCoreLanguage* lang = static_cast<CPtiCoreLanguage*>(GetLanguage(aLang));
    
    if (lang)
        {
        CPtiQwertyKeyMappings* maps = static_cast<CPtiQwertyKeyMappings*>(lang->GetQwertyKeymappings());
        if (maps)
            {
            TInt tmpSize = 0;
            CPtiKeyMapData* mapData = maps->KeyMapData();
            
            if (mapData->KeyData(EPtiKeyboardQwerty4x12, tmpSize))
                {
                return EPtiKeyboardQwerty4x12;                
                }
            if (mapData->KeyData(EPtiKeyboardQwerty4x10, tmpSize))
                {
                return EPtiKeyboardQwerty4x10;                
                }                            
            if (mapData->KeyData(EPtiKeyboardQwerty3x11, tmpSize))
                {
                return EPtiKeyboardQwerty3x11;                
                }                    
            if (mapData->KeyData(EPtiKeyboardCustomQwerty, tmpSize))
                {
                return EPtiKeyboardCustomQwerty;                
                }    
            if (mapData->KeyData(EPtiKeyboardHalfQwerty, tmpSize))
                {
                return EPtiKeyboardHalfQwerty;                
                }                                                                                   
            }        
        }
    
    return EPtiKeyboardNone;
    }                                              	
        
       
// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetNumberCandidateStatus
// 
// ---------------------------------------------------------------------------
//              
TInt CPtiEngineImpl::SetNumberCandidateStatus(TBool aStatus)
    {
    if (Core())
        {            
        TInt ret = KErrNone;
        TInt err = KErrNone;
    
        TRAP(err, ret = Core()->HandleCommandL(EPtiCommandSetNumberCandidateStatus, (TAny*)&aStatus));
    
        if (err != KErrNone)
            {
            return err;
            }
    
        return ret;    
        }
        
        
    return KErrNoSuitableCore;        
    }
        
 
// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetNumberCandidateStatus
// 
// ---------------------------------------------------------------------------
//             
TInt CPtiEngineImpl::SetMaxLengthForAutoCompletedCandidates(TInt aMaxLength)
 	{
 	if (IsCore())
 	    {
 	    TInt ret = KErrNone;
        TInt err = KErrNone;
 	    
 		TRAP(err, ret = Core()->HandleCommandL(EPtiCommandSetAutoCompletionMaxLength, (TAny*)aMaxLength));
 		
 		if (err != KErrNone)
            {
            return err;
            }
    
        return ret;    
 	    }
 	   
    return KErrNoSuitableCore;   	    
 	}
 	
 	
// ---------------------------------------------------------------------------
// CPtiEngineImpl::MarkAutoCapitalized
// 
// ---------------------------------------------------------------------------
//     	
void CPtiEngineImpl::MarkAutoCapitalized()
     {
     if (IsCore())
         {
         TRAP_IGNORE(Core()->HandleCommandL(EPtiCommandMarkAutoCapitalized, NULL));
         }
     }
             
       
// ---------------------------------------------------------------------------
// CPtiEngineImpl::AddPhrase
// 
// ---------------------------------------------------------------------------
//                  
TInt CPtiEngineImpl::AddPhrase(const TDesC& aNewPhrase)
    {
	if (IsCore())
 	    {
 	    TInt ret = KErrNone;
        TInt err = KErrNone;
 	    
 		TRAP(err, ret = Core()->HandleCommandL(EPtiCommandAddPhrase, (TAny*)(&aNewPhrase)));
 		
 		if (err != KErrNone)
            {
            return err;
            }
    
        return ret;    
 	    }
 	   
    return KErrNoSuitableCore;   	        
    }
                
                
// ---------------------------------------------------------------------------
// LOCAL METHODS
// 
// ---------------------------------------------------------------------------
//     
const TInt KVietKeyMarker = 0xffff;
const TUint16 vietnameseAccents[] =
    {
    KVietKeyMarker, EPtiKey2, // For key 2, a-based    
    0x00c1,	0x00c0, 0x1ea2, 0x00c3, 0x1ea0,
    0x00e1, 0x00e0, 0x1ea3, 0x00e3, 0x1ea1,
    0x0102, 0x1eae, 0x1eb0, 0x1eb2, 0x1eb4, 0x1eb6,
    0x0103, 0x1eaf, 0x1eb1, 0x1eb3, 0x1eb5, 0x1eb7,
    0x00c2, 0x1ea4, 0x1ea6, 0x1ea8, 0x1eaa, 0x1eac,
    0x00e2, 0x1ea5, 0x1ea7, 0x1ea9, 0x1eab, 0x1ead, 

    KVietKeyMarker, EPtiKey3, // For key 3, e-based    
    0x00c9,	0x00c8, 0x1eba, 0x1ebc, 0x1eb8,
    0x00e9, 0x00e8, 0x1ebb, 0x1ebd, 0x1eb9,
    0x00ca, 0x1ebe, 0x1ec0, 0x1ec2, 0x1ec4, 0x1ec6,
    0x00ea, 0x1ebf, 0x1ec1, 0x1ec3, 0x1ec5, 0x1ec7,

    KVietKeyMarker, EPtiKey4, // For key 4, i-based    
    0x00cd,	0x00cc, 0x1ec8, 0x0128, 0x1eca,
    0x00ed, 0x00ec, 0x1ec9, 0x0129, 0x1ecb,
    
    KVietKeyMarker, EPtiKey6, // For key 6, o-based    
    0x00d3,	0x00d2, 0x1ece, 0x00d5, 0x1ecc,
    0x00f3, 0x00f2, 0x1ecf, 0x00f5, 0x1ecd,
    0x00d4, 0x1ed0, 0x1ed2, 0x1ed4, 0x1ed6, 0x1ed8,
    0x00f4, 0x1ed1, 0x1ed3, 0x1ed5, 0x1ed7, 0x1ed9,
    0x01a0, 0x1eda, 0x1edc, 0x1ede, 0x1ee0, 0x1ee2,
    0x01a1, 0x1edb, 0x1edd, 0x1edf, 0x1ee1, 0x1ee3, 
    0
    }; 


// ---------------------------------------------------------------------------
// RemapVietnameseAccentedCharacter
// 
// ---------------------------------------------------------------------------
// 
LOCAL_C TInt RemapVietnameseAccentedCharacter(TUint16 aChr)
	{
	// Check tone marks first.	
	switch (aChr)
		{
		case 0x0301:
		     return EPtiKey2;
		case 0x0300:
		     return EPtiKey3;
		case 0x0309:
		     return EPtiKey4;		     
		case 0x0303:
			 return EPtiKey5;			
		case 0x0323:
			 return EPtiKey6;						 
		default:
			break; 	
		}	 
	
	TInt ret = 0;
	
	for (const TUint16* ptr = vietnameseAccents; *ptr != 0; ptr++)
		{
		if (*ptr == KVietKeyMarker)
			{
			ret = ptr[1];
			ptr++;
			}
		else
			{
			if (*ptr == aChr)
				{
				break;
				}
			}			
		}
		
	return ret;	 		
	}

#ifdef FF_DUAL_LANGUAGE_SUPPORT
// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetSecondaryInputL
// 
// ---------------------------------------------------------------------------
// 
TInt CPtiEngineImpl::SetSecondaryInputL(TInt aEpocLanguageID)
    {
    TInt err = KErrNone;
    
    if(iCurrentLanguage == NULL)
        {
        return KErrNoActiveLanguage;
        }
            
    if(aEpocLanguageID == 0)
        {        
        // Delete language object
        iSecondaryLanguage = NULL;
        }
    else if(!iSecondaryLanguage || iSecondaryLanguage->LanguageCode() != aEpocLanguageID)
        {
        CPtiCoreLanguage *lang = static_cast<CPtiCoreLanguage*>(GetLanguage( aEpocLanguageID ));
        if (!lang)
            {            
            // Delete language object
            iSecondaryLanguage = NULL;
            return KErrLanguageNotFound;
            }
        iSecondaryLanguage = lang;
        }

       TPtiEngineInputMode inputMode = EPtiEngineInputModeNone;   
       TPtiKeyboardType kbdType = KeyboardType();  
       switch (kbdType)
           {
           case EPtiKeyboard12Key:
               inputMode = EPtiEnginePredictive;
               break;
           case EPtiKeyboardHalfQwerty:
               inputMode = EPtiEngineHalfQwertyPredictive;
               break;
           case EPtiKeyboardQwerty4x10:
           case EPtiKeyboardQwerty3x11:
           case EPtiKeyboardQwerty4x12:
           case EPtiKeyboardCustomQwerty:
               inputMode = EPtiEngineQwertyPredictive;
               break;
           }
              
        MPtiCore* core = iCurrentLanguage->GetCore(inputMode);
        if(core)
            err = core->HandleCommandL(EPtiCommandSetSecondaryInputLanguage, static_cast<TAny*>(iSecondaryLanguage));  
        else
            err =  KErrNoSuitableCore;
                
        return err;    
    
    }
#endif //FF_DUAL_LANGUAGE_SUPPORT
// End of file

