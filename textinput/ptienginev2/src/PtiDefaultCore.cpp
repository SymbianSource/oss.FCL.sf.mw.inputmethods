/*
* Copyright (c) 2003-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Default core for predective text input engine.
*               Implements basic qwerty & multitapping input.  
*
*/


#include <e32property.h>
#include "PtiDefaultCore.h"
#include "PtiLanguage.h"
#include "PtiEngine.h"
#include "PtiKeyMappings.h"
#include <ptiengine.rsg>
#include "PtiDefaultNumberModeMappingdata.h"

#include <data_caging_path_literals.hrh> 

// CONSTANTS
const TInt KMultiTapTimeoutMS = 1000000;
//const TInt KPtiKeymappingsDefaultImplementation = 0x10281897;

_LIT(KDefaultCoreVendor, "Default core");

#ifdef _DEBUG
_LIT(KPtiEnginePanic, "CPtiDefaultCore");
#endif

// lam with alef ligatures are entered as one press but are actually two symbols
// These four arabic characters are handled as a special case.
const TUint16 KArarbicLigatureLamWithAlefWithMaddaAboveIsolatedForm = 0xFEF5;
const TUint16 KArarbicLigatureLamWithAlefWithHamzaAboveIsolatedForm = 0xFEF7;
const TUint16 KArarbicLigatureLamWithAlefWithHamzaBelowIsolatedForm = 0xFEF9;
const TUint16 KArarbicLigatureLamWithAlefIsolatedForm               = 0xFEFB;

_LIT(KArarbicLigatureLamWithAlefWithMaddaAboveIsolatedSequence, "\x0644\x622");
_LIT(KArarbicLigatureLamWithAlefWithHamzaAboveIsolatedSequence, "\x0644\x623");
_LIT(KArarbicLigatureLamWithAlefWithHamzaBelowIsolatedSequence, "\x0644\x625");
_LIT(KArarbicLigatureLamWithAlefIsolatedSequence, "\x0644\x627");


//
// CMultiTapTimer
//

// ---------------------------------------------------------------------------
// CMultiTapTimer::NewL
// 
// ---------------------------------------------------------------------------
// 
CMultiTapTimer* CMultiTapTimer::NewL(TCallBack aCallback)
	{
	CMultiTapTimer* self = new(ELeave)CMultiTapTimer(aCallback);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();	// self
	return self;
	}


// ---------------------------------------------------------------------------
// CMultiTapTimer::CMultitapTimer
// 
// ---------------------------------------------------------------------------
// 
CMultiTapTimer::CMultiTapTimer(TCallBack aCallback)
:CTimer(EPriorityStandard), iCallback(aCallback)
	{
	}

// ---------------------------------------------------------------------------
// CMultiTapTimer::ConstructL
// 
// ---------------------------------------------------------------------------
// 
void CMultiTapTimer::ConstructL()
	{
	CTimer::ConstructL();
	CActiveScheduler::Add(this);
	}

// ---------------------------------------------------------------------------
// CMultiTapTimer::After
// 
// ---------------------------------------------------------------------------
// 
void CMultiTapTimer::After(TTimeIntervalMicroSeconds32 aInterval)
	{
	if (IsActive())
		{
		Cancel();
		}
	CTimer::After(aInterval);
	}

// ---------------------------------------------------------------------------
// CMultiTapTimer::RunL
// 
// ---------------------------------------------------------------------------
// 
void CMultiTapTimer::RunL()
	{
	iCallback.CallBack();
	}


//
// CPtiDefaultCore
//

// ---------------------------------------------------------------------------
// CPtiDefaultCore::NewL
// 
// ---------------------------------------------------------------------------
// 
CPtiDefaultCore* CPtiDefaultCore::NewL(CPtiEngine* aOwner, TDes* aTextBuffer)
	{
	CPtiDefaultCore *core = new (ELeave) CPtiDefaultCore(aOwner, aTextBuffer);
	CleanupStack::PushL( core );
	core->ConstructL();
	CleanupStack::Pop(); // core
	return core;
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::CPtiDefaultCore
// .
// ---------------------------------------------------------------------------
// 
CPtiDefaultCore::CPtiDefaultCore()
	{
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::CPtiDefaultCore
// 
// ---------------------------------------------------------------------------
// 
CPtiDefaultCore::CPtiDefaultCore(CPtiEngine* aOwner, TDes* aTextBuffer) : iOwner(aOwner), iTextBuffer(aTextBuffer)
	{
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::~CPtiDefaultCore
// 
// ---------------------------------------------------------------------------
// 
CPtiDefaultCore::~CPtiDefaultCore()
	{
	if (iTimer)
		{
		iTimer->Cancel();
		}
		 
    ShutDownKeymapFactory();
    					
	delete iTimer;	
	delete iNumericMappings;
	delete iNumericData;
	    
    iFactoryBindings.Close();    	
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::ConstructL
// 
// ---------------------------------------------------------------------------
// 
void CPtiDefaultCore::ConstructL()
	{
	FillCoreInfo();
	TCallBack timerExpire(TimerExpire, this);
	iTimer = CMultiTapTimer::NewL(timerExpire);
	iNumericData = CPtiNumberModeDataImpl::NewL();
	iNumericMappings = CPtiKeyMappings::NewL(iNumericData);  
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::InitializeL
// 
// ---------------------------------------------------------------------------
// 
void CPtiDefaultCore::InitializeL(CPtiEngine* aOwner, TDes* aTextBuffer, CArrayPtrFlat<CPtiCoreLanguage>* aLanguageList, TBool /*aUseDefautUserDictionary*/)
	{
	iOwner = aOwner;
	iTextBuffer = aTextBuffer;
    iLanguageList = aLanguageList;	
	
    TInt count = 0;
    TInt err = RProperty::Get(KPtiEnginePSUid, EKeyMapPropertyCount, count);
    if (err == KErrNone && count > 0)
        {
        // PS key found, keymap implementation list has been created and can be read
        TBuf16<256> dataBuffer;
        err = RProperty::Get(KPtiEnginePSUid, EKeyMapPropertyData, dataBuffer);
                                             
        if( err == KErrNone )
            {
            TInt langId;
            TInt factoryUid;
            for(TInt i = 0; i < count; i++ )
                {
                langId = dataBuffer[3*i];
                factoryUid = (dataBuffer[3*i+1] << 16) + dataBuffer[3*i+2];
            
                CPtiCoreLanguage* language = CPtiCoreLanguage::NewL();
                language->SetLanguageCode(langId);
		        language->SetCore(this, EPtiEngineNumeric);	

	            CleanupStack::PushL( language );     
	       	    aLanguageList->AppendL( language );
	            CleanupStack::Pop(); // language
             
                TPtiDataFactoryBinding newBind(langId, factoryUid);
                iFactoryBindings.AppendL(newBind);
                }
            return;
            }
        }

    // PS key not defined or error, we need to fetch the data from dll:s
	RArray<TInt> dataImpl;
	CleanupClosePushL(dataImpl);
	CPtiKeyMapDataFactory::ListImplementationsL(dataImpl);
   
	for (TInt i = 0; i < dataImpl.Count(); i++)
	    {
        ShutDownKeymapFactory();
	    iKeyMapDataFactory = CPtiKeyMapDataFactory::CreateImplementationL(TUid::Uid(dataImpl[i]));	  	    	  	   
	    ListDataFactoryLanguagesL(iKeyMapDataFactory, dataImpl[i], aLanguageList);	     
	    }
		
	if (!iFactoryBindings.Count())
	    {
	    // Cannot function without keymapping data.
	    User::Leave(KErrCorrupt);
	    } 
                     
    // Write language->dll mapping data to P/S-key so we don't need to load every dll later
    TBuf16<256> dataBuffer;
    for(TInt i = 0; i < iFactoryBindings.Count(); i++)
       {
       dataBuffer.Append(iFactoryBindings[i].iLanguage); // Language id should fit in 16 bits
       dataBuffer.Append(iFactoryBindings[i].iFactoryImplUid >> 16); // 32-bit Uid is appended in two parts
       dataBuffer.Append(iFactoryBindings[i].iFactoryImplUid);
       }
    
    // If these fail, then they just fail.                  
    RProperty::Set(KPtiEnginePSUid, EKeyMapPropertyCount, iFactoryBindings.Count());  
    RProperty::Set(KPtiEnginePSUid, EKeyMapPropertyData, dataBuffer);        
                  
	CleanupStack::PopAndDestroy(); // dataImpl    
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::OpenLanguageL
// 
// ---------------------------------------------------------------------------
// 
TInt CPtiDefaultCore::OpenLanguageL(CPtiCoreLanguage* aLanguage)
	{		
	TRAPD(err, LoadFactoryForLanguageL(aLanguage->LanguageCode()));
	
	ClearFlag(EPtiFlagVowelSeq);	

	return err;
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::CloseLanguageL
// 
// ---------------------------------------------------------------------------
// 
TInt CPtiDefaultCore::CloseLanguageL()
	{
	if (iOwner->CurrentLanguage())
	    {
	    CPtiCoreLanguage* curLang = static_cast<CPtiCoreLanguage*>(iOwner->CurrentLanguage());
	    curLang->SetKeyMapDataFactory(NULL);  
	    }			
	
	ClearFlag(EPtiFlagVowelSeq);	
	return KErrNone;
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::GetCurrentWord
// 
// ---------------------------------------------------------------------------
// 
TPtrC CPtiDefaultCore::GetCurrentWord() const
	{
	__ASSERT_DEBUG(iTextBuffer, User::Panic(KPtiEnginePanic, KErrCorrupt));
		
	return TPtrC(iTextBuffer->Ptr());
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::CommitCurrentWord
// 
// ---------------------------------------------------------------------------
// 
TInt CPtiDefaultCore::CommitCurrentWord()
	{
	ClearBuffer();
	return KErrNone;
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::NumberOfCandidates
// 
// ---------------------------------------------------------------------------
// 
TInt CPtiDefaultCore::NumberOfCandidates()
	{
	__ASSERT_DEBUG(iTextBuffer, User::Panic(KPtiEnginePanic, KErrCorrupt));
		
	if (iTextBuffer->Length() == 0)
		{
		return 0;	
		}

	return 1;
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::FillCoreInfo
// 
// ---------------------------------------------------------------------------
// 
void CPtiDefaultCore::FillCoreInfo()
	{
	iCoreInfo.SetCapsBits(0);
	iCoreInfo.SetMaxWordLength(KDefaulCoreMaximumWordLength);
	iCoreInfo.SetVendorString( KDefaultCoreVendor );
	iCoreInfo.SetMaxNumberOfCandidates(1);
	iCoreInfo.SetUid(KDefaultCoreUid);
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::SetReordering
// 
// ---------------------------------------------------------------------------
// 
TInt CPtiDefaultCore::SetReordering(TBool aStatus)
	{
	if (aStatus)
		{
		return KErrNotSupported;
		}

	return KErrNone;
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::AppendKeyPress
// 
// ---------------------------------------------------------------------------
// 
void CPtiDefaultCore::AppendKeyPress(TPtiKey aKey)
	{
	__ASSERT_DEBUG(iOwner, User::Panic(KPtiEnginePanic, KErrCorrupt));

	CPtiCoreLanguage* lang = static_cast<CPtiCoreLanguage*>(iOwner->CurrentLanguage());
	const TPtiEngineInputMode inputMode = iOwner->InputMode();
	const TBool qwerty = iOwner->IsQwertyBasedMode(inputMode);
	TBool append = ETrue;
	TUint16 overridingDeadKey = 0;
	
	ClearFlag(EPtiGetAllFlag);		
	
	__ASSERT_DEBUG(lang, User::Panic(KPtiEnginePanic, KErrCorrupt));

	MPtiKeyMappings* maps = NULL;

	if (inputMode == EPtiEngineNumeric)
		{
		maps = iNumericMappings;
		}
    else if (inputMode == EPtiEngineHalfQwerty)		
        {
        if (lang->HasInputMode(EPtiEngineHalfQwerty))
            {
	        maps = lang->GetHalfQwertyKeymappings();            
            }        
        }		
	else if (qwerty)
		{
		if (lang->HasInputMode(EPtiEngineQwerty))
			{
			maps = lang->GetQwertyKeymappings();
			}
		}
	else
		{
		if (lang->HasInputMode(EPtiEngineMultitapping))
			{
			maps = lang->GetKeymappings();
			}
		}

	if (!maps)
		{
		iTimer->Cancel();
		return;
		}

	if (iTimer->IsActive())
		{
		TUint16 chr = maps->NextKey(aKey, append, iOwner->Case());
		if (chr && (!append) && (iTextBuffer->Length() > 0))
			{
			iTextBuffer->SetLength(iTextBuffer->Length() - 1);	
			}
		if (iTextBuffer->Length() < iTextBuffer->MaxLength())
			{
			if (chr)
				{
				iTextBuffer->Append(chr);
				}
			}
		}
	else
		{
		if (iTextBuffer->Length() < iTextBuffer->MaxLength())
			{
			if (qwerty)
				{
				CPtiQwertyKeyMappings* qmap = static_cast<CPtiQwertyKeyMappings*>(maps);		
				if (lang->LanguageCode() == ELangVietnamese)
					{
					qmap->SetFlag(CPtiQwertyKeyMappings::ETrackVietnameseToneMarksAndVowels);
					}
				else
					{
					qmap->ResetFlag(CPtiQwertyKeyMappings::ETrackVietnameseToneMarksAndVowels);
					}					
										
				if (qmap->DeadKey() && (aKey == qmap->CurrentKey()))
					{
					overridingDeadKey = qmap->DeadKeyRootChar();				
					} 
				}
			
			TUint16 chr = maps->StartMapping(aKey, iOwner->Case(), inputMode);
			
			if (chr == KPtiGetAllMarker) 
				{
				if (lang->LanguageCode() == ELangHindi)
					{									
					TPtrC ptr = maps->GetAll(iOwner->Case());				
					iTextBuffer->Append(ptr.Left(iTextBuffer->MaxLength() - iTextBuffer->Length()));
					SetFlag(EPtiGetAllFlag);		
					}
				}			
			else if (chr)
				{
                switch (chr)
                	{
          			case KArarbicLigatureLamWithAlefWithMaddaAboveIsolatedForm:
                 	     iTextBuffer->Append(KArarbicLigatureLamWithAlefWithMaddaAboveIsolatedSequence);
                 	     SetFlag(EPtiDoubleWideEntry);
                 	     break;
            		case KArarbicLigatureLamWithAlefWithHamzaAboveIsolatedForm:
                 	     iTextBuffer->Append(KArarbicLigatureLamWithAlefWithHamzaAboveIsolatedSequence);
                 	     SetFlag(EPtiDoubleWideEntry);                 
                 	     break;
            		case KArarbicLigatureLamWithAlefWithHamzaBelowIsolatedForm:
                 	     iTextBuffer->Append(KArarbicLigatureLamWithAlefWithHamzaBelowIsolatedSequence);
                         SetFlag(EPtiDoubleWideEntry);                 
                         break; 
                    case KArarbicLigatureLamWithAlefIsolatedForm:
                         iTextBuffer->Append(KArarbicLigatureLamWithAlefIsolatedSequence);      
                         SetFlag(EPtiDoubleWideEntry);                 
                         break;
                    default:       
                         iTextBuffer->Append(chr);    
                     }                
				}
				
			if (qwerty)
				{
				// There was dead key press waiting, but last key press didn't
				// contain dead key data, so both keys will be inserted.	
				CPtiQwertyKeyMappings* qmap = static_cast<CPtiQwertyKeyMappings*>(maps);
				
				if (overridingDeadKey != 0) 
					{				
					qmap->ClearDeadKey();
					}				
				else if (qmap->DeadKeyRootFlag())
					{
					if (iTextBuffer->Length() < iTextBuffer->MaxLength())
						{	
						SetFlag(EPtiDeadKeyRootFlag); // This will indicate PtiEngine that extra character 
						                             // was added to buffer.
						iTextBuffer->Append(maps->StartMapping(aKey, iOwner->Case()));	
						}
					}
				else if ((qmap->VowelSequenceFlag()) &&
					     (lang->LanguageCode() == ELangVietnamese))
					{
					// This was Vietnamese vowel sequence, remove first vowel
					// component from buffer.
					TInt numRemove = (iTextBuffer->Length() > 1 ? 2 : 1);
					iTextBuffer->SetLength(iTextBuffer->Length() - numRemove);
					iTextBuffer->Append(qmap->VowelSequenceResult());						
					SetFlag(EPtiFlagVowelSeq);					
					}
				}				
			}
		}
		
	const TPtiTextCase textCase = iOwner->Case();		
// In Querty mode . default core always get called.
// When Fn key is pressed its not in multitap its  not in multitapmode
// multitap timer should be canceled.

	if (qwerty &&			
		((textCase == EPtiCaseLower) || (textCase == EPtiCaseUpper) ||(textCase == EPtiCaseFnLower) || (textCase == EPtiCaseFnUpper)||
		 ((inputMode == EPtiEngineZhuyinQwerty || inputMode ==EPtiEnginePinyinQwerty ||
		   inputMode == EPtiEngineStrokeQwerty ||inputMode == EPtiEngineZhuyinPhraseQwerty ||
		   inputMode == EPtiEngineStrokePhraseQwerty ||inputMode == EPtiEnginePinyinPhraseQwerty ) && 
		   (textCase == EPtiCaseChrUpper ||
		 textCase == EPtiCaseChrLower)))) 
		{
		iTimer->Cancel();
		}
	else 
		{
		iTimer->After(KMultiTapTimeoutMS); 
		}
	}



// ---------------------------------------------------------------------------
// CPtiDefaultCore::DeleteKeyPress
// 
// ---------------------------------------------------------------------------
// 
void CPtiDefaultCore::DeleteKeyPress()
	{
	__ASSERT_DEBUG(iOwner, User::Panic(KPtiEnginePanic, KErrCorrupt));

	ClearFlag(EPtiGetAllFlag | EPtiFlagVowelSeq);		

	if (iTimer->IsActive())
		{
		iTimer->Cancel();
		}

	if (iTextBuffer->Length() > 0)
		{
		// Delete last character.
		iTextBuffer->SetLength(iTextBuffer->Length() - 1);	
		}

	ClearVowelSequence();
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::ClearBuffer
// 
// ---------------------------------------------------------------------------
// 
void CPtiDefaultCore::ClearBuffer()
	{
	__ASSERT_DEBUG(iTimer, User::Panic(KPtiEnginePanic, KErrCorrupt));
	
	iTimer->Cancel();	
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::ClearVowelSequence
// 
// ---------------------------------------------------------------------------
// 
void CPtiDefaultCore::ClearVowelSequence()
	{
	__ASSERT_DEBUG(iOwner, User::Panic(KPtiEnginePanic, KErrCorrupt));
		
	if (iOwner->IsQwertyBasedMode(iOwner->InputMode()))
		{
		CPtiCoreLanguage* lang = static_cast<CPtiCoreLanguage*>(iOwner->CurrentLanguage());
		__ASSERT_DEBUG(lang, User::Panic(KPtiEnginePanic, KErrCorrupt));
		CPtiQwertyKeyMappings* qMap = static_cast<CPtiQwertyKeyMappings*>(lang->GetQwertyKeymappings());
		if (qMap)
			{
			qMap->ClearVowelSequence();
			qMap->ClearLastChar();
			}
		}
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::SetCurrentInputSequence
// 
// ---------------------------------------------------------------------------
// 
TInt CPtiDefaultCore::SetCurrentInputSequence(TPtrC8 /*aInput*/)
	{
	__ASSERT_DEBUG(iTimer, User::Panic(KPtiEnginePanic, KErrCorrupt));
		
	if (iTimer->IsActive())
		{
		iTimer->Cancel();
		}	

	ClearVowelSequence();

	return KErrNone;
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::TimerExpire
// 
// ---------------------------------------------------------------------------
//
TInt CPtiDefaultCore::TimerExpire(TAny* aPtr)
	{
	return reinterpret_cast<CPtiDefaultCore*>(aPtr)->DoTimerExpire();
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::DoTimerExpire
// 
// ---------------------------------------------------------------------------
//
TInt CPtiDefaultCore::DoTimerExpire()
	{
	__ASSERT_DEBUG(iOwner, User::Panic(KPtiEnginePanic, KErrCorrupt));

	if (iOwner->Observer())
		{
		iOwner->Observer()->KeyTimerExpired();
		}
	return KErrNone;
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::DropLanguages
// 
// ---------------------------------------------------------------------------
//
void CPtiDefaultCore::DropLanguages(CArrayPtrFlat<CPtiCoreLanguage>* aLanguageList)
	{
	for (TInt i = 0; i < aLanguageList->Count(); i++)
		{
		if (aLanguageList->At(i)->HasInputMode(EPtiEngineMultitapping))
			{
			aLanguageList->At(i)->ClearMultitapping();			
			if (aLanguageList->At(i)->GetCore(EPtiEngineMultitapping) == this)
			    {
			    aLanguageList->At(i)->SetCore(NULL, EPtiEngineMultitapping);			    
			    }
			}
        if (aLanguageList->At(i)->HasInputMode(EPtiEngineQwerty))			
            {
	        aLanguageList->At(i)->ClearQwerty(); 
		    if (aLanguageList->At(i)->GetCore(EPtiEngineQwerty) == this)
			    {
			    aLanguageList->At(i)->SetCore(NULL, EPtiEngineQwerty);			    
			    }	                   
            }
        if (aLanguageList->At(i)->HasInputMode(EPtiEngineHalfQwerty))			
            {
            aLanguageList->At(i)->ClearHalfQwerty();             
		    if (aLanguageList->At(i)->GetCore(EPtiEngineHalfQwerty) == this)
			    {
			    aLanguageList->At(i)->SetCore(NULL, EPtiEngineHalfQwerty);			    
			    }	                      
            }            
		}
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::GetCoreInfo
// 
// ---------------------------------------------------------------------------
//
MPtiCoreInfo* CPtiDefaultCore::GetCoreInfo()
	{
	return &iCoreInfo;   
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::Convert
// 
// ---------------------------------------------------------------------------
//
TInt CPtiDefaultCore::Convert(TPtiCharConversion /*aType*/,
							  TAny* /*aInput*/,
							  TInt /*aInputLength*/,
							  TAny* /*aOutput*/)
	{
	return KErrNotSupported;	
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::SetExternalKeyMapL
// 
// ---------------------------------------------------------------------------
//
TInt CPtiDefaultCore::SetExternalKeyMapL(TPtiEngineInputMode aMode,
		                                 TPtiKey aKey,
									     TDesC& aKeyMap,
										 TPtiTextCase aCase)
	{
	if (aMode == EPtiEngineNumeric)
		{
		__ASSERT_DEBUG(iNumericMappings, User::Panic(KPtiEnginePanic, KErrCorrupt));

		// Always available. Case doesn't apply for numeric mode,
		// so both cases are always updated.
		TInt ret = iNumericMappings->ReplaceKeyMapL(aKey, aKeyMap, EPtiCaseLower);
		if (ret != KErrNone)
			{
			return ret;
			} 
		return iNumericMappings->ReplaceKeyMapL(aKey, aKeyMap, EPtiCaseUpper);
		}
	else
		{		
		__ASSERT_DEBUG(iOwner, User::Panic(KPtiEnginePanic, KErrCorrupt));
				
		CPtiCoreLanguage* lang = static_cast<CPtiCoreLanguage*>(iOwner->CurrentLanguage());
		if (!lang)
			{
			return KErrCorrupt;
			}
		
		MPtiKeyMappings* maps = NULL;
	    if (aMode == EPtiEngineMultitapping)
			{
			maps = lang->GetKeymappings();
			}
		else if (aMode == EPtiEngineQwerty)
			{
			maps = lang->GetQwertyKeymappings();		
			}		
        else if (aMode == EPtiEngineHalfQwerty)
            {
           	maps = lang->GetHalfQwertyKeymappings();	
            }
        						
		if (maps)
			{
			return maps->ReplaceKeyMapL(aKey, aKeyMap, aCase);
			}					
		}		
		

	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::GetSpelling
// 
// ---------------------------------------------------------------------------
//
TInt CPtiDefaultCore::GetSpelling(TUint16 /*aInput*/, TDes& /*aOutput*/, 
                                  TPtiSpelling /*aType*/)
	{
	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::CancelTimerActivity
// 
// ---------------------------------------------------------------------------
//
TInt CPtiDefaultCore::CancelTimerActivity()
	{
	__ASSERT_DEBUG(iTimer, User::Panic(KPtiEnginePanic, KErrCorrupt));
		
	iTimer->Cancel();
	return KErrNone;	
	}


// ---------------------------------------------------------------------------
// CPtiDefaultCore::HandleCommandL
// 
// ---------------------------------------------------------------------------
//
TInt CPtiDefaultCore::HandleCommandL(TPtiEngineCommand aCommand, TAny* aParams)
	{
	__ASSERT_DEBUG(iOwner, User::Panic(KPtiEnginePanic, KErrCorrupt));
		
	CPtiCoreLanguage* lang = static_cast<CPtiCoreLanguage*>(iOwner->CurrentLanguage());	
	TBool qwerty = EFalse;
	if (lang)
		{
		qwerty = lang->HasInputMode(EPtiEngineQwerty);
		}
	
	switch (aCommand)
		{
		case EPtiCommandPollQwertyDeadKeyRootFlag:
			{
			if (IsFlagSet(EPtiDeadKeyRootFlag))
				{
				ClearFlag(EPtiDeadKeyRootFlag);	
				return ETrue;
				}

			return EFalse;
			}
		case EPtiCommandGetAndClearDeadKeyRootChar:	
			{
			TUint16 ret = 0;
		
			if (lang && qwerty)
				{
				CPtiQwertyKeyMappings* maps = static_cast<CPtiQwertyKeyMappings*>(lang->GetQwertyKeymappings());	
				if (maps->DeadKey())
					{
					maps->ClearDeadKey();	
					ret = maps->DeadKeyRootChar();						
					}								
				}
				
			return ret;
			}
		case EPtiCommandDeadKeyWaiting:	
			{
			TUint16 ret = 0;
		
			if (lang && qwerty)
				{
				CPtiQwertyKeyMappings* maps = static_cast<CPtiQwertyKeyMappings*>(lang->GetQwertyKeymappings());	
				if (maps->DeadKey())	
					{
					ret = 1;						
					}								
				}
				
			return ret;
			}		
	
		case EPtiCommandQueryAndClearGetAllFlag:
			{
			TBool ret = IsFlagSet(EPtiGetAllFlag);
			ClearFlag(EPtiGetAllFlag);				
			return ret;	
			}
			
		case EPtiCommandGetAndClearVowelSequence:
			{
			if (IsFlagSet(EPtiFlagVowelSeq))
				{
				if (lang && qwerty)
					{
					CPtiQwertyKeyMappings* maps = static_cast<CPtiQwertyKeyMappings*>(lang->GetQwertyKeymappings());	
					ClearFlag(EPtiFlagVowelSeq);						
					return maps->VowelSequenceResult();						
					}								
				}
													
			return 0;
			}							
		case EPtiCommandVowelSeqFlag:
             return IsFlagSet(EPtiFlagVowelSeq);			
		case EPtiCommandClearVowelSeq:
			 {					
			 ClearFlag(EPtiFlagVowelSeq);
		     if (lang && qwerty)
			 	{
				CPtiQwertyKeyMappings* maps = static_cast<CPtiQwertyKeyMappings*>(lang->GetQwertyKeymappings());							
				if (maps)
					{
					maps->ClearVowelSequence();	
					}
				}					 
			 }
		     return 0; 		     
		case EPtiCommandGetAndClearLastVietnameseChar:
			 {					
			 if (lang && qwerty)
			 	{
				CPtiQwertyKeyMappings* maps = static_cast<CPtiQwertyKeyMappings*>(lang->GetQwertyKeymappings());							
				if (maps)
					{
					TInt ret = maps->GetLastChar();
					maps->ClearLastChar();	
					return ret;
					}
				}	 					
			 }
		     return 0;
		case EPtiCommandSetVietnameseLastChar:
	 		 {					
		     if (lang && qwerty)
			 	{
				CPtiQwertyKeyMappings* maps = static_cast<CPtiQwertyKeyMappings*>(lang->GetQwertyKeymappings());							
				if (maps)
					{
					maps->SetLastChar((TInt)aParams);	
					}
				}	 					
			 }					 
		     return 0;  
		case EPtiCommandQueryReplacePrevious:
			 {										 			 	 
			if (lang && qwerty)
			 	{
				CPtiQwertyKeyMappings* maps = static_cast<CPtiQwertyKeyMappings*>(lang->GetQwertyKeymappings());							
				if (maps)
					{
					TChar rep = maps->ReplacedCharacter();
					if (maps->VowelCase() == EPtiCaseUpper ||
					    maps->VowelCase() == EPtiCaseChrUpper)
						{
						rep.UpperCase();
						}
					else
						{
						rep.LowerCase();
						}						
					
					return rep;	
					}
				}	 					
			}				     
		     return 0;     	     		     		     		     
		case EPtiCommandSetLastKeyForVietnamese:
			 {	 		 			 	 
		     if (lang && qwerty)
			 	{
				CPtiQwertyKeyMappings* maps = static_cast<CPtiQwertyKeyMappings*>(lang->GetQwertyKeymappings());							
				if (maps)
					{					
					TPtiKey key = (TPtiKey)((TInt)aParams);
					maps->SetLastKey(key);	
					}
				}	 					
		 	 }
		 	 return 0;
		 	 
		case EPtiCommandResetVietnameseVowelSeqAndToneMark: 		  		     
		     ClearFlag(EPtiFlagVowelSeq);
			 if (lang && qwerty)
			 	{
				CPtiQwertyKeyMappings* maps = static_cast<CPtiQwertyKeyMappings*>(lang->GetQwertyKeymappings());							
				if (maps)
					{					
					maps->ResetVietnameseVowelSequenceAndToneMarks();	
					}
				}
			return 0;
			
		case EPtiCommandGetVietnameseLastKey:
		 	 if ((lang) && (lang->HasInputMode(EPtiEngineQwerty)))
			 	{
				CPtiQwertyKeyMappings* maps = static_cast<CPtiQwertyKeyMappings*>(lang->GetQwertyKeymappings());							
				if (maps)
					{					
					return (TInt)maps->LastKey();	
					}
				}				 			
		    return 0;
		case EPtiCommandDoubleWideEntry:
		    {
		    TInt ret = IsFlagSet(EPtiDoubleWideEntry);	
			ClearFlag(EPtiDoubleWideEntry);		    		
            return ret;
		    }		    			 											
		default:
			break;
		}		
	
	return KErrNotSupported;
	}
	
	
// ---------------------------------------------------------------------------
// CPtiDefaultCore::ListDataFactoryLanguagesL
// 
// ---------------------------------------------------------------------------
//	
void CPtiDefaultCore::ListDataFactoryLanguagesL(CPtiKeyMapDataFactory* aDataFactory,
                                                TInt aImplUid,
                                                CArrayPtrFlat<CPtiCoreLanguage>* aLanguageList)
    {
	__ASSERT_DEBUG(aDataFactory, User::Panic(KPtiEnginePanic, KErrCorrupt));
            
    RArray<TInt> langs;
    aDataFactory->ListLanguagesL(langs);    
    CleanupClosePushL(langs);            
                    
    for (TInt i = 0; i < langs.Count(); i++)
        {
        CPtiCoreLanguage* language = NULL;        
        
        for (TInt j = 0; j < iFactoryBindings.Count(); j++)    
            {
            // Remove possible earlier bindings for this language (ie. 
            // newly installed dlls will replace old languages).
            if (iFactoryBindings[j].iLanguage == langs[i])
                {
                iFactoryBindings.Remove(j);
                break;
                }          
            }
            
        if (!language)            
            {
            language = CPtiCoreLanguage::NewL();
	        CleanupStack::PushL( language );            
	       	aLanguageList->AppendL( language ); 
	        CleanupStack::Pop(); // language   	        
            }
            
		language->SetLanguageCode(langs[i]); 		
		language->SetCore(this, EPtiEngineNumeric);	
		language->SetKeyMapDataFactory(aDataFactory);
		language->GetKeymappings();  // Force core list to be updated.
		language->SetKeyMapDataFactory(NULL);		
            
        TPtiDataFactoryBinding newBind(langs[i], aImplUid);
        iFactoryBindings.AppendL(newBind);                       
        }
    
    CleanupStack::PopAndDestroy(); // langs    
    }
		 	
			
// ---------------------------------------------------------------------------
// CPtiDefaultCore::LoadFactoryForLanguageL
// 
// ---------------------------------------------------------------------------
//		
void CPtiDefaultCore::LoadFactoryForLanguageL(TInt aLanguage)
    {
	__ASSERT_DEBUG(iOwner, User::Panic(KPtiEnginePanic, KErrCorrupt));
        
    TInt currentUid = 0;    
    if (iKeyMapDataFactory)    
        {
        currentUid = iKeyMapDataFactory->ImplementationUid();
        }
        
    CPtiCoreLanguage* coreLang = static_cast<CPtiCoreLanguage*>(iOwner->GetLanguage(aLanguage));
    if (!coreLang)        
        {
        User::Leave(KErrCorrupt);
        }    
            
    coreLang->SetKeyMapDataFactory(NULL);            
    for (TInt i = 0; i < iFactoryBindings.Count(); i++)    
        {
        if (iFactoryBindings[i].iLanguage == aLanguage)
            {
            if (iFactoryBindings[i].iFactoryImplUid == currentUid)
                {
                coreLang->SetKeyMapDataFactory(iKeyMapDataFactory);               
                return;
                }
            
            // Factory is about to change. Clear references to old one.
            ShutDownKeymapFactory();
            
    	    iKeyMapDataFactory = CPtiKeyMapDataFactory::CreateImplementationL(TUid::Uid(iFactoryBindings[i].iFactoryImplUid));	  	    	  	   
            coreLang->SetKeyMapDataFactory(iKeyMapDataFactory);       	    
            
            return;            
            }
        }
        
    User::Leave(KErrNotFound);
    }
    

// ---------------------------------------------------------------------------
// CPtiDefaultCore::ShutDownKeymapFactory
// 
// ---------------------------------------------------------------------------
//	    
void CPtiDefaultCore::ShutDownKeymapFactory()
    {
    if (iLanguageList)
        {    
        CPtiCoreLanguage* lang = NULL;    	   
        for (TInt i = 0; i < iLanguageList->Count(); i++)
            {
    	    lang = static_cast<CPtiCoreLanguage*>(iLanguageList->At(i));
    	    if (lang && lang->DataFactory() == iKeyMapDataFactory)
    	        {
    		    lang->SetKeyMapDataFactory(NULL);		
    	        }
            }

        delete iKeyMapDataFactory;
        iKeyMapDataFactory = NULL;
        }   
    }
	
// End of file
