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
* Description:   CPtiQwertyKeymappings class implementation.
*
*/


#include "PtiKeyMappings.h"
#include <PtiKeyMapData.h>

// CONSTANTS

#ifdef _DEBUG
_LIT(KPtiNoQwertyKeyMapDataPanic, "PtiEngine: No qwerty keymap data set!");
_LIT(KPtiNoExtensionPanic, "PtiEngine: No extension class created!");
#endif

const TUint16 KEmptyDeadKeySlotMarker = 0xf000;
const TInt KPtiUWithAHorn = 0x01b0;
const TInt KPtiQwertyLowCaseLetterBase = 97;

const TInt KNumVietnameseToneMarks = 5;
const TInt KNumVietnameseToneMarkDataEntries = 24;

class TVietnameseToneMarkData
	{
	public:
		TUint16 iBaseChar;
		TUint16 iToneData[KNumVietnameseToneMarks]; 
	};


const TVietnameseToneMarkData vietnameseToneMarkData[KNumVietnameseToneMarkDataEntries] =
	{
	// for A-based
	{0x0041, {0x00c1, 0x00c0, 0x1ea2, 0x00c3, 0x1ea0}},		
	{0x0061, {0x00e1, 0x00e0, 0x1ea3, 0x00e3, 0x1ea1}},		
	{0x0102, {0x1eae, 0x1eb0, 0x1eb2, 0x1eb4, 0x1eb6}},		
	{0x0103, {0x1eaf, 0x1eb1, 0x1eb3, 0x1eb5, 0x1eb7}},		
	{0x00c2, {0x1ea4, 0x1ea6, 0x1ea8, 0x1eaa, 0x1eac}},		
	{0x00e2, {0x1ea5, 0x1ea7, 0x1ea9, 0x1eab, 0x1ead}},	
	// for E-based
	{0x0045, {0x00c9, 0x00c8, 0x1eba, 0x1ebc, 0x1eb8}},
	{0x0065, {0x00e9, 0x00e8, 0x1ebb, 0x1ebd, 0x1eb9}},		
	{0x00ca, {0x1ebe, 0x1ec0, 0x1ec2, 0x1ec4, 0x1ec6}},		
	{0x00ea, {0x1ebf, 0x1ec1, 0x1ec3, 0x1ec5, 0x1ec7}},		
	// for I-based	
	{0x0049, {0x00cd, 0x00cc, 0x1ec8, 0x0128, 0x1eca}},		
	{0x0069, {0x00ed, 0x00ec, 0x1ec9, 0x0129, 0x1ecb}},		
	// for O-based
	{0x004f, {0x00d3, 0x00d2, 0x1ece, 0x00d5, 0x1ecc}},		
	{0x006f, {0x00f3, 0x00f2, 0x1ecf, 0x00f5, 0x1ecd}},		
	{0x00d4, {0x1ed0, 0x1ed2, 0x1ed4, 0x1ed6, 0x1ed8}},			
	{0x00f4, {0x1ed1, 0x1ed3, 0x1ed5, 0x1ed7, 0x1ed9}},		
	{0x01a0, {0x1eda, 0x1edc, 0x1ede, 0x1ee0, 0x1ee2}},		
	{0x01a1, {0x1edb, 0x1edd, 0x1edf, 0x1ee1, 0x1ee3}},		
	// for U-based
	{0x0055, {0x00da, 0x00d9, 0x1ee6, 0x0168, 0x1ee4}},		
	{0x0075, {0x00fa, 0x00f9, 0x1ee7, 0x0169, 0x1ee5}},		
	{0x01af, {0x1ee8, 0x1eea, 0x1eec, 0x1eee, 0x1ef0}},			
	{0x01b0, {0x1ee9, 0x1eeb, 0x1eed, 0x1eef, 0x1ef1}},	
	// for Y based
	{0x0059, {0x00dd, 0x1ef2, 0x1ef6, 0x1ef8, 0x1ef4}},		
	{0x0079, {0x00fd, 0x1ef3, 0x1ef7, 0x1ef9, 0x1ef5}}			
	};


const TPtiKey vietnameseToneMarkKeys[KNumVietnameseToneMarks] = 
    {
    EPtiKeyQwertyS, EPtiKeyQwertyF, EPtiKeyQwertyR, EPtiKeyQwertyX, EPtiKeyQwertyJ	
    };
    
    
//
// CPtiQwertyKeymappingsExtension
//

// ---------------------------------------------------------------------------
// CPtiQwertyKeymappingsExtension::NewL
// 
// ---------------------------------------------------------------------------
// 
CPtiQwertyKeymappingsExtension* CPtiQwertyKeymappingsExtension::NewL()
	{
	return new (ELeave)	CPtiQwertyKeymappingsExtension();
	}
	

// ---------------------------------------------------------------------------
// CPtiQwertyKeymappingsExtension::CPtiQwertyKeymappingsExtension
// 
// ---------------------------------------------------------------------------
// 	
CPtiQwertyKeymappingsExtension::CPtiQwertyKeymappingsExtension()
	{	
	}
	

// ---------------------------------------------------------------------------
// CPtiQwertyKeymappingsExtension::CPtiQwertyKeymappingsExtension
// 
// ---------------------------------------------------------------------------
// 	
CPtiQwertyKeymappingsExtension::~CPtiQwertyKeymappingsExtension()
	{	
	}

//
// CPtiQwertyKeymappingsExtension
//

// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::NewL
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C CPtiQwertyKeyMappings* CPtiQwertyKeyMappings::NewL(TDesC& /*aData*/)
	{
	// DEPRECATED
	User::Leave(KErrNotSupported);
	return NULL;
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::NewL
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C CPtiQwertyKeyMappings* CPtiQwertyKeyMappings::NewL(TInt16* /*aData*/)
	{
	// DEPRECATED
	User::Leave(KErrNotSupported);
	return NULL;
	}
	

// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::NewL
// 
// ---------------------------------------------------------------------------
// 	
EXPORT_C CPtiQwertyKeyMappings* CPtiQwertyKeyMappings::NewL(CPtiKeyMapData* aData)
	{
	CPtiQwertyKeyMappings* maps = new (ELeave) CPtiQwertyKeyMappings(aData);
	CleanupStack::PushL(maps);
	maps->ConstructL();
	CleanupStack::Pop(); // maps
	return maps;	
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::CPtiQwertyKeyMappings
// 
// ---------------------------------------------------------------------------
// 
CPtiQwertyKeyMappings::CPtiQwertyKeyMappings()
	{
    iKeyboardType = EPtiKeyboardQwerty4x12;	
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::CPtiQwertyKeyMappings
// 
// ---------------------------------------------------------------------------
// 
CPtiQwertyKeyMappings::CPtiQwertyKeyMappings(CPtiKeyMapData* aData) : CPtiMappings(aData)
	{
	if(aData->HasKeyData(EPtiKeyboardQwerty4x12))
	    {
	    iKeyboardType = EPtiKeyboardQwerty4x12;
	    }
	else if(aData->HasKeyData(EPtiKeyboardQwerty4x10))
        {
        iKeyboardType = EPtiKeyboardQwerty4x10;
        }
	else if(aData->HasKeyData(EPtiKeyboardQwerty3x11))
	    {
	    iKeyboardType = EPtiKeyboardQwerty3x11;
	    }
	else
	    { 
	    iKeyboardType = EPtiKeyboardCustomQwerty;
	    }
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::~CPtiQwertyKeyMappings
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C CPtiQwertyKeyMappings::~CPtiQwertyKeyMappings()
	{
	iReservedArray.Close();
	delete iExtension;
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::ConstructL
// 
// ---------------------------------------------------------------------------
// 
void CPtiQwertyKeyMappings::ConstructL()
	{
	iExtension = CPtiQwertyKeymappingsExtension::NewL(); 	
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::StartMapping
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C TUint16 CPtiQwertyKeyMappings::StartMapping(TPtiKey aKey, TPtiTextCase aCase, 
                                                     TPtiEngineInputMode aMode)
	{
	__ASSERT_DEBUG(iExtension, User::Panic(KPtiNoExtensionPanic, KErrCorrupt));	
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoQwertyKeyMapDataPanic, KErrCorrupt));
		
	KeyCodeToInternal(aKey);
	
	ResetFlag(EDeadKeyRootFlag | EVowelSeqFlag);	

	if (iDeadKey)
		{
		TUint16 deadChar = GetDeadKeyChar(iCurrentKey, aCase);
		iDeadKey = 0;
		if (deadChar)
			{
			return deadChar;	
			}
		SetFlag(EDeadKeyRootFlag);	
		return iDeadKeyRootChar;	
		}

	iCurrentChar = 0;
	iExtension->iReplaced = 0;

	TPtrC keyData = DataForKeyLocal(aKey, aCase, EPtiEngineQwerty);	

	TInt chrIndexOffset= 0 ;
			
	if (aCase == EPtiCaseChrLower)
		{			
		if (IsChnSpecialCharKey(iCurrentKey))
   			{
       		if (aMode == EPtiEnginePinyinQwerty || aMode == EPtiEnginePinyinPhraseQwerty)
   				{
   				chrIndexOffset = 2;
   				}
            else if (aMode == EPtiEngineZhuyinQwerty || aMode == EPtiEngineZhuyinPhraseQwerty)
        		{
        		chrIndexOffset = 2;
        		}  		
			else if (aMode == EPtiEngineStrokeQwerty || aMode == EPtiEngineStrokePhraseQwerty)
    			{
    			chrIndexOffset = 6;
    			}   			   			    		
   			}
   		else if(aKey >= EPtiKeyQwerty0 && aKey <= EPtiKeyQwerty9)
   		    {
       		if (aMode == EPtiEnginePinyinQwerty || aMode == EPtiEnginePinyinPhraseQwerty)
   				{
   				chrIndexOffset = 2;
   				}
            else if (aMode == EPtiEngineZhuyinQwerty || aMode == EPtiEngineZhuyinPhraseQwerty)
        		{
        		chrIndexOffset = 2;
        		}  		
   		    }
		}
    	    	    		
	if (keyData.Length() <= chrIndexOffset)
		{
		return 0;
		}
	
	if (iFlags & ETrackVietnameseToneMarksAndVowels)
		{
		TUint toneMarked = VietnameseToneMark(iExtension->iLastChar, aKey);
		if (toneMarked)
			{
			/* In case of FnLower Case, Vowel Sequence shouldnt be constructed, instead we should clear the Vowel Sequence  */
			if (aCase != EPtiCaseFnLower)
          {
          iExtension->iLastToneKey = aKey;               
          iVowelSeqResult = toneMarked;       
          SetFlag(EVowelSeqFlag);
          iLastKey = EPtiKeyNone;         
          return toneMarked;
          }
			else
			    {
			    ClearVowelSequence();
                ClearLastChar();
			    }
			}
		}	
	
	if (iExtension->iLastToneKey == aKey)
		{
		iExtension->iReplaced = iExtension->iLastChar;			
		TChar lastCase = iExtension->iLastChar;
		iExtension->iVowelCase = EPtiCaseLower;		
		if (lastCase.IsUpper())		
			{
			iExtension->iVowelCase = EPtiCaseUpper;
			}
		iVowelSeqResult = 0;							
		}				
	else if (iVowelSeqResult)
		{
		iExtension->iReplaced = RepeatingVowelSequence(aKey);
		if (!iExtension->iReplaced)
			{
			TPtiKey first = VowelSeqRootKey(iVowelSeqResult);
			iVowelSeqResult = VowelSequence(first, aKey, iExtension->iVowelCase);			
			}
		else
			{
			iVowelSeqResult = 0;
			}			
		}
		
	iExtension->iLastTone = 0;	
	iExtension->iLastToneKey = EPtiKeyNone;	
	
	TUint16 ret = GetCharForMode(aMode, keyData.Mid(chrIndexOffset),
								(aMode == EPtiEngineQwertyPredictive ||
	                             aMode == EPtiEngineQwerty || aMode == EPtiEngineInputModeNone)
								 && (aCase == EPtiCaseChrLower || aCase == EPtiCaseChrUpper));
								
	if ((aKey == EPtiKey0) && 
	    (aCase == EPtiCaseChrLower) &&
        (aMode == EPtiEngineZhuyinQwerty  || aMode ==  EPtiEngineZhuyinPhraseQwerty ))
	    {
	    ret = 0;
	    }
																
	if (ChineseChrModeConversionNeeded(aCase, aMode) &&
	    aKey >= EPtiKeyQwertyA && aKey <= EPtiKeyQwertyZ)
		{
        ret = KPtiQwertyLowCaseLetterBase + aKey - EPtiKeyQwertyA;		
		}
								         
	if (ret == KPtiGetAllMarker)
		{
		return ret;
		}  

	if (IsDeadKeyCode(ret))
		{
		iDeadKey = ret;
		if (keyData.Length() > 1)
			{
			iDeadKeyRootChar = keyData[1];
			}
		
		return 0;
		}

	TPtiTextCase vowelCase = iExtension->iVowelCase;			
	//  Do not use vowel sequence if user is in Numeric mode (fnLower)
    if (aCase != EPtiCaseFnLower && iVowelSeqResult == 0 && (iFlags & ETrackVietnameseToneMarksAndVowels))  
        {
		vowelCase = (aCase == EPtiCaseUpper || 
		            (iFlags & EPrevKeyInUpperCase) ? EPtiCaseUpper : EPtiCaseLower);		
		iVowelSeqResult = VowelSequence(iLastKey, aKey, vowelCase);		
		}
	
	if (iVowelSeqResult)
		{	
		iExtension->iVowelCase = vowelCase;
		iExtension->iLastChar = iVowelSeqResult;
		SetFlag(EVowelSeqFlag);
		iLastKey = EPtiKeyNone;
		}
	else
		{
		// This is for vietnamese, iReplaced can be non-zero only for vietnamese.
		if (iExtension->iReplaced != 0 && aKey == EPtiKeyQwertyW)
			{
			if (ret == KPtiUWithAHorn)
				{
				ret = 'w';
				}
			else
				{
				ret = 'W';
				}				
			iLastKey = EPtiKeyNone;
			iExtension->iLastChar = 0;
			}			
		else
			{
			/* In case of FnLower Case, Vowel Sequence shouldnt be constructed, instead we should clear the Vowel Sequence  */
			if(aCase != EPtiCaseFnLower)
			    {
                iLastKey = aKey;
                iExtension->iLastChar = ret;	
			    }
			else
			    {
			    ClearVowelSequence();
			    ClearLastChar();
			    }
			}			
		}

	if (aCase == EPtiCaseUpper || aCase == EPtiCaseChrUpper)
		{
		SetFlag(EPrevKeyInUpperCase);
		}
	else
		{
		ResetFlag(EPrevKeyInUpperCase);
		}

	return ret;		
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::GetDeadKeyChar
// 
// ---------------------------------------------------------------------------
// 
TUint16 CPtiQwertyKeyMappings::GetDeadKeyChar(TPtiKey aKey, TPtiTextCase aCase)
	{
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoQwertyKeyMapDataPanic, KErrCorrupt));
		
	TUint deadIndex = DeadKeyIndex(iDeadKey);
	TPtrC deadKeyData = iKeyMapData->DeadKeyDataForKey(iKeyboardType, aKey, aCase);
	if (deadIndex > 0 && deadIndex <= deadKeyData.Length())
		{
		if (deadKeyData[deadIndex - 1] != KEmptyDeadKeySlotMarker)
			{
			return deadKeyData[deadIndex - 1];   
			}
		}
	
	return 0;	
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::NextChar
// 
// ---------------------------------------------------------------------------
// 
TUint16 CPtiQwertyKeyMappings::NextChar(TPtiTextCase aCase)
	{
	__ASSERT_DEBUG(iExtension, User::Panic(KPtiNoExtensionPanic, KErrCorrupt));		
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoQwertyKeyMapDataPanic, KErrCorrupt));
		
	TPtrC keyData = DataForKeyLocal(iCurrentKey, aCase, EPtiEngineQwerty);	
		
	iCurrentChar++;
	if (iCurrentChar >= keyData.Length())
		{
		iCurrentChar = 0;
		if (aCase == EPtiCaseChrLower || aCase == EPtiCaseChrUpper)
			{
			iCurrentChar++;			
			if (iCurrentChar >= keyData.Length())
				{
				iCurrentChar = 0;
				return 0;
				} 
			} 
		}	
	
	if (keyData.Length() == 0)
		{
		return 0;
		}
		
	if (iFlags & ETrackVietnameseToneMarksAndVowels)
		{
		iExtension->iLastChar = keyData[iCurrentChar];
		}		
		
	return keyData[iCurrentChar];						
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::ReplaceKeyMapL
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C TInt CPtiQwertyKeyMappings::ReplaceKeyMapL(TPtiKey aKey, TDesC& aMap,
                                                    TPtiTextCase aCase)
	{
	__ASSERT_DEBUG(iExtension, User::Panic(KPtiNoExtensionPanic, KErrCorrupt));	
		
	if (iCurrentKey == aKey)
		{		
		iCurrentKey = EPtiKeyNone;
		iCurrentChar = 0;
		iDeadKey = 0;
		iVowelSeqResult = 0;
		iExtension->iLastToneKey = EPtiKeyNone;
		}				
		
	if (aCase == EPtiCaseChrLower)		
		{
		aCase = EPtiCaseLower;
		}
	else if (aCase == EPtiCaseChrUpper)		
		{
		aCase = EPtiCaseUpper;
		}
		
	ReplaceKeyMapLocalL(aKey, aMap, aCase, EPtiEngineQwerty);
					
	return KErrNone;			
	}

	
// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::GetCharForMode
// 
// ---------------------------------------------------------------------------
// 
TUint16 CPtiQwertyKeyMappings::GetCharForMode(TPtiEngineInputMode aMode, TPtrC aChars, TBool aSkipFirst)
	{
	for (TInt i = 0; i < aChars.Length(); i++)
		{
		if (!IsModeControlChar(aChars[i]))
			{
			if (i == 0 && aSkipFirst && aChars.Length() > 1)
				{
				iCurrentChar++;				
				continue;	
				}
			return aChars[i];
			}
		if (DoesModeCharMatchToInputMode(aChars[i], aMode))
			{
			if (i + 1 >= aChars.Length())
				{
				return 0;
				}
			return aChars[i + 1];		
			}
		i++;   // Wrong mode, skip char
		}

	return 0;
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::NextKey
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C TUint16 CPtiQwertyKeyMappings::NextKey(TPtiKey aKey, TBool &aAppend,
                                                TPtiTextCase aCase)
	{
	if (CurrentKey() == aKey)
		{
		aAppend = EFalse;
		return NextChar(aCase);	
		}

	return StartMapping(aKey, aCase);
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::KeyForCharacter
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C TPtiKey CPtiQwertyKeyMappings::KeyForCharacter(TUint16 aChar)
	{
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoQwertyKeyMapDataPanic, KErrCorrupt));
	
	TInt numBindItems = 0;
	const TPtiKeyBinding* bindings = iKeyMapData->KeyBindingTable(iKeyboardType, numBindItems);
	
	TPtiTextCase textCase;
	TPtiKey key;
	for (TInt i = 0; i < numBindItems; i++)
		{
		key = iKeyMapData->KeyForBindingTableIndex(iKeyboardType, i, textCase);
		if (key != EPtiKeyNone)
			{
			TPtrC keyData = DataForKeyLocal(key, textCase, EPtiEngineQwerty);
			if (keyData.Locate(aChar) != KErrNotFound)
				{
				return key;
				}			
			}
		}
			 
	return EPtiKeyNone;		
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::KeyCodeToInternal
// 
// ---------------------------------------------------------------------------
// 
void CPtiQwertyKeyMappings::KeyCodeToInternal(TPtiKey aKey)
	{
	iCurrentKey = aKey;
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::GetDataForKey
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C void CPtiQwertyKeyMappings::GetDataForKey(TPtiKey aKey, TDes& aResult,
                                                   TPtiTextCase aCase)
	{
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoQwertyKeyMapDataPanic, KErrCorrupt));

	aResult.Copy(DataForKeyLocal(aKey, aCase, EPtiEngineQwerty).Left(aResult.MaxLength()));
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::DoesModeCharMatchToInputMode
// 
// ---------------------------------------------------------------------------
// 
TBool CPtiQwertyKeyMappings::DoesModeCharMatchToInputMode(TUint16 aChar, 
                                               TPtiEngineInputMode aMode) const
	{
	switch (aMode)
		{
		case EPtiEnginePinyin:
		case EPtiEnginePinyinQwerty:
		case EPtiEnginePinyinPhraseQwerty:
			 if (aChar == KPtiPinyinMarker)
				{
				return ETrue;
				}
			 break;			
		case EPtiEngineStroke:
		case EPtiEngineStrokeQwerty:
		case EPtiEngineStrokePhraseQwerty:		
			 if (aChar == KPtiStrokeMarker)
				{
				return ETrue;
				}
			 break;
		case EPtiEngineZhuyin:
		case EPtiEngineZhuyinQwerty:
		case EPtiEngineZhuyinPhraseQwerty:
		case EPtiEngineInputModeZhuyinMiniQwertyFind:
			 if (aChar == KPtiZhuyinMarker)
				{
				return ETrue;
				}
			 break;
		case EPtiEngineNormalCangjieQwerty:
		case EPtiEngineEasyCangjieQwerty:
		case EPtiEngineAdvCangjieQwerty: 
			 if (aChar == KPtiCangjieMarker)
				{
				return ETrue;
				}
			 break;
		default:
			break;
		}

	return EFalse;
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::VowelSequence
// 
// ---------------------------------------------------------------------------
// 
TUint16 CPtiQwertyKeyMappings::VowelSequence(TPtiKey aKey1, TPtiKey aKey2, 
                                             TPtiTextCase aCase) const
	{
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoQwertyKeyMapDataPanic, KErrCorrupt));

    TPtrC keyData1 = DataForKeyLocal((TPtiKey)aKey1, aCase, EPtiEngineQwerty);
    TPtrC keyData2 = DataForKeyLocal((TPtiKey)aKey2, aCase, EPtiEngineQwerty);
		
	// This method tracks Vietnamese vowel sequences.
	TInt numVowelSeqs = 0;
	const TVowelSequence* vowelSeqs = iKeyMapData->VowelSequenceTable(iKeyboardType, numVowelSeqs);

	TInt shift = (aCase == EPtiCaseLower ? 0 : 1);

	// This loop is to support keylayouts which 
	// can have numeric scancodes
	if( keyData1.Length() > 0 && keyData2.Length() > 0 )
	    {
	    // convert to caps to compare with vowel data
	    TChar data1 = keyData1[0];
	    data1 = data1.GetUpperCase();
	    
	    TChar data2 = keyData2[0];
	    data2 = data2.GetUpperCase();
	    
	    for (TInt i = 0; i < numVowelSeqs; i++)
    		{
    		if (((TPtiKey)(TUint)data1 == (TUint)vowelSeqs[i].iFirst) &&
    			((TPtiKey)(TUint)data2 == (TUint)vowelSeqs[i].iSecond))
    			{
    			return vowelSeqs[i].iChars[shift];
    			}
    		}
	    }

	return 0;
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::RepeatingVowelSequence
// 
// ---------------------------------------------------------------------------
// 
TUint16 CPtiQwertyKeyMappings::RepeatingVowelSequence(TPtiKey aKey)
	{
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoQwertyKeyMapDataPanic, KErrCorrupt));
	
	TPtrC keyData = DataForKeyLocal((TPtiKey)aKey, EPtiCaseUpper, EPtiEngineQwerty);
    if( keyData.Length() > 0 )
        {
        TChar data = keyData[0];
        aKey = (TPtiKey)(TUint)data;
        }
    
	TInt numVowelSeqs = 0;
	const TVowelSequence* vowelSeqs = iKeyMapData->VowelSequenceTable(iKeyboardType, numVowelSeqs);	
	
	for (TInt i = 0; i < numVowelSeqs; i++)
		{
		if (vowelSeqs[i].iSecond == aKey)
			{
			if (iVowelSeqResult == vowelSeqs[i].iChars[0] ||
			    iVowelSeqResult == vowelSeqs[i].iChars[1])
				{
				return vowelSeqs[i].iFirst;				
				}			
			}			
		}
		
	return 0;	
	}	


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::VowelSeqRootKey
// 
// ---------------------------------------------------------------------------
// 
TPtiKey CPtiQwertyKeyMappings::VowelSeqRootKey(TUint16 aChar)
	{
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoQwertyKeyMapDataPanic, KErrCorrupt));
		
	TInt numVowelSeqs = 0;
	const TVowelSequence* vowelSeqs = iKeyMapData->VowelSequenceTable(iKeyboardType, numVowelSeqs);		
	
	for (TInt i = 0; i < numVowelSeqs; i++)
		{
		if (vowelSeqs[i].iChars[0] == aChar ||
			vowelSeqs[i].iChars[1] == aChar)
			{
			return vowelSeqs[i].iFirst;
			}	
		}
		
	return EPtiKeyNone;		
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::VietnameseToneMark
// 
// ---------------------------------------------------------------------------
// 
TUint16 CPtiQwertyKeyMappings::VietnameseToneMark(TUint16 aPrevChar, TPtiKey aKey)
	{
	__ASSERT_DEBUG(iExtension, User::Panic(KPtiNoExtensionPanic, KErrCorrupt));	

	TPtrC keyData = DataForKeyLocal((TPtiKey)aKey, EPtiCaseUpper, EPtiEngineQwerty);

    if( keyData.Length() > 0 )
        {
        TChar data = keyData[0];
        aKey = (TPtiKey)(TUint)data;
        }
	
	TInt toneIndex = 0;
	
	for (; toneIndex < KNumVietnameseToneMarks; toneIndex++)
		{
		if (aKey == vietnameseToneMarkKeys[toneIndex])
			{
			if (aKey == (TPtiKey)iExtension->iLastTone)
				{
				return 0;				
				}
			break;
			}
		}
	
	if (toneIndex >= KNumVietnameseToneMarks)
		{
		return 0;
		}	
	
	for (TInt i = 0; i < KNumVietnameseToneMarkDataEntries; i++)
		{
		if (vietnameseToneMarkData[i].iBaseChar == aPrevChar)
			{
			iExtension->iLastTone = (TInt16)aKey;
			return vietnameseToneMarkData[i].iToneData[toneIndex];
			}		
		}
	
	return 0;
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::GetAll
// 
// ---------------------------------------------------------------------------
// 
TPtrC CPtiQwertyKeyMappings::GetAll(TPtiTextCase aCase)
	{
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoQwertyKeyMapDataPanic, KErrCorrupt));
	
    TPtrC keyData = DataForKeyLocal((TPtiKey)iCurrentKey, aCase, EPtiEngineQwerty);
	if (keyData.Length() >= 1)
		{
		// return all characters excluding the marker character.
		return TPtrC(keyData.Mid(1));		
		}
			
	return TPtrC();
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::GetNumericModeKeysFromDataL
// 
// ---------------------------------------------------------------------------
// 
void CPtiQwertyKeyMappings::GetNumericModeKeysFromDataL(RArray<TPtiNumericKeyBinding>& aResult)
	{
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoQwertyKeyMapDataPanic, KErrCorrupt));
	
	TInt numEntries = 0;
	const TPtiNumericKeyBinding* dataEntries = iKeyMapData->NumericModeKeysTable(iKeyboardType, 
	                                                                             numEntries);
	
	for (TInt i = 0; i < numEntries; i++)
		{
		User::LeaveIfError(aResult.Append(dataEntries[i]));
		}
	}
			

// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::WriteData (DEPRECATED)
// 
// ---------------------------------------------------------------------------
// 	
TInt CPtiQwertyKeyMappings::WriteData(TInt16*)   
	{
	return 0;
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::ChunkDataSize (DEPRECATED)
// 
// ---------------------------------------------------------------------------
// 	
TInt CPtiQwertyKeyMappings::ChunkDataSize() const
	{
	return 0;		
	}


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::SetKeyboardType
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C TInt CPtiQwertyKeyMappings::SetKeyboardType(TPtiKeyboardType aKeyboardType)
    {
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoQwertyKeyMapDataPanic, KErrCorrupt));
        
    TInt sizeTmp = 0;
    if (!iKeyMapData->KeyData(aKeyboardType, sizeTmp))
        {
        return KErrNotSupported;
        }
    
    iKeyboardType = aKeyboardType;    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::HasDataBlockForKeyboardType
// 
// ---------------------------------------------------------------------------
//    
EXPORT_C TBool CPtiQwertyKeyMappings::HasDataBlockForKeyboardType(TPtiKeyboardType aKeyboardType) const
    {
    TInt sizeTmp = 0;
    if (iKeyMapData->KeyData(aKeyboardType, sizeTmp))
        {
        return ETrue;
        }    
    
    return EFalse;
    }	

// End of file
