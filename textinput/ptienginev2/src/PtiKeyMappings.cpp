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
* Description:   PtiKeyMappings class implementation.
*
*/


#include "PtiKeyMappings.h"
#include <PtiKeyMapData.h>

// CONSTANTS
#ifdef _DEBUG
_LIT(KPtiNoKeyMapDataPanic, "PtiEngine: No ITU-T keymap data set.");
#endif

//
// CPtiMappings
//

// ---------------------------------------------------------------------------
// CPtiMappings::CPtiMappings
// 
// ---------------------------------------------------------------------------
// 
CPtiMappings::CPtiMappings()
	{
	}
	
	
// ---------------------------------------------------------------------------
// CPtiMappings::CPtiMappings
// 
// ---------------------------------------------------------------------------
// 	
CPtiMappings::CPtiMappings(CPtiKeyMapData* aData) : iKeyMapData(aData)
	{	
	}


// ---------------------------------------------------------------------------
// CPtiMappings::~CPtiMappings
// 
// ---------------------------------------------------------------------------
// 
CPtiMappings::~CPtiMappings()
	{
	iReplacedMappings.Close();
	}


// ---------------------------------------------------------------------------
// CPtiMappings::FindReplacedMapping
// 
// ---------------------------------------------------------------------------
// 
TInt CPtiMappings::FindReplacedMapping(TPtiKey aKey, TPtiTextCase aCase,
                                       TPtiEngineInputMode aMode) const
	{
	for (TInt i = 0; i < iReplacedMappings.Count(); i++)
		{
		if (iReplacedMappings[i].iKey == aKey &&
		    iReplacedMappings[i].iCase == aCase &&
		    iReplacedMappings[i].iMode == aMode &&
		    iReplacedMappings[i].iKeyboardType == iKeyboardType)
			{
			return i;
			}		
		}
		
	return KErrNotFound;
	}


// ---------------------------------------------------------------------------
// CPtiMappings::ReplaceKeyMapLocalL
// 
// ---------------------------------------------------------------------------
// 
void CPtiMappings::ReplaceKeyMapLocalL(TPtiKey aKey, TDesC& aMap, 
                                       TPtiTextCase aCase, TPtiEngineInputMode aMode)
	{
	TInt i = FindReplacedMapping(aKey, aCase, aMode);
	if (i != KErrNotFound)	
		{
		iReplacedMappings.Remove(i);
		}
	
	TPtiReplacedMapping newMapping;
	newMapping.iKey = aKey;
	newMapping.iCase = aCase;
	newMapping.iMode = aMode;
	newMapping.iKeyboardType = iKeyboardType;
	
	for (i = 0; i < KMaxReplacedMappingCharacters && i < aMap.Length(); i++)	
		{
		newMapping.iChrs[i] = aMap[i];
		}
	newMapping.iNumChars = i;	
		
	User::LeaveIfError(iReplacedMappings.Append(newMapping));	
	}



// ---------------------------------------------------------------------------
// CPtiMappings::DataForKeyLocal
// 
// ---------------------------------------------------------------------------
// 	
TPtrC CPtiMappings::DataForKeyLocal(TPtiKey aKey, TPtiTextCase aCase, TPtiEngineInputMode aMode) const
	{
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoKeyMapDataPanic, KErrCorrupt));
		
	if ((aCase == EPtiCaseChrLower) && (iKeyboardType != EPtiKeyboardHalfQwerty))
		{
		aCase = EPtiCaseLower;		
		}
	else if ((aCase == EPtiCaseChrUpper) && (iKeyboardType != EPtiKeyboardHalfQwerty))
		{
		aCase = EPtiCaseUpper;		
		}
				
	TInt replaced = FindReplacedMapping(aKey, aCase, aMode);
	if (replaced != KErrNotFound)
		{		
		if (iReplacedMappings[replaced].iNumChars > 0)		
			{
			return TPtrC(iReplacedMappings[replaced].iChrs,
			             iReplacedMappings[replaced].iNumChars);  
			}
		return TPtrC();	
		}
	
    return iKeyMapData->DataForKey(iKeyboardType, aKey, aCase);	
	}	


// ---------------------------------------------------------------------------
// CPtiMappings::WriteData (DEPRECATED)
// 
// ---------------------------------------------------------------------------
// 	
TInt CPtiMappings::WriteData(TInt16*)   
	{
	return 0;
	}


// ---------------------------------------------------------------------------
// CPtiMappings::ChunkDataSize (DEPRECATED)
// 
// ---------------------------------------------------------------------------
// 	
TInt CPtiMappings::ChunkDataSize() const
	{
	return 0;		
	}


// ---------------------------------------------------------------------------
// CPtiMappings::KeyboardType
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TPtiKeyboardType CPtiMappings::KeyboardType() const
    {
    return iKeyboardType;
    }	


//
// CPtiKeyMappings
//

// ---------------------------------------------------------------------------
// CPtiKeyMappings::NewL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPtiKeyMappings* CPtiKeyMappings::NewL(TDesC& /*aData*/)
	{
	// DEPRECATED
	User::Leave(KErrNotSupported);
	return NULL;	
	}


// ---------------------------------------------------------------------------
// CPtiKeyMappings::NewL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPtiKeyMappings* CPtiKeyMappings::NewL(TInt16* /*aData*/)
	{
	// DEPRECATED
	User::Leave(KErrNotSupported);
	return NULL;
	}


// ---------------------------------------------------------------------------
// CPtiKeyMappings::NewL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPtiKeyMappings* CPtiKeyMappings::NewL(CPtiKeyMapData* aData)
	{
	CPtiKeyMappings* maps = new (ELeave) CPtiKeyMappings(aData);
	return maps; 	
	}
		

// ---------------------------------------------------------------------------
// CPtiKeyMappings::CPtiKeyMappings
// 
// ---------------------------------------------------------------------------
//
CPtiKeyMappings::CPtiKeyMappings()
	{
    iKeyboardType = EPtiKeyboard12Key;      
	}


// ---------------------------------------------------------------------------
// CPtiKeyMappings::CPtiKeyMappings
// 
// ---------------------------------------------------------------------------
//
CPtiKeyMappings::CPtiKeyMappings(CPtiKeyMapData* aData) : CPtiMappings(aData)
	{
    iKeyboardType = EPtiKeyboard12Key; 		
	}


// ---------------------------------------------------------------------------
// CPtiKeyMappings::~CPtiKeyMappings
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPtiKeyMappings::~CPtiKeyMappings()
	{
	}


// ---------------------------------------------------------------------------
// CPtiKeyMappings::StartMapping
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TUint16 CPtiKeyMappings::StartMapping(TPtiKey aKey, TPtiTextCase aCase,
                                               TPtiEngineInputMode aMode)
	{
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoKeyMapDataPanic, KErrCorrupt));
	
	DeQwertyfyCaseValue(aCase);
	
	KeyCodeToInternal(aKey);

    iCurrentMode = aMode;
	iCurrentChar = 0;
    
	TPtrC keyData = DataForKeyLocal(aKey, aCase, iCurrentMode);	
	if (keyData.Length() > 0)
	    {
	    return keyData[0];	
	    }        
	           			
	return 0;
	}  


// ---------------------------------------------------------------------------
// CPtiKeyMappings::NextChar
// 
// ---------------------------------------------------------------------------
//
TUint16 CPtiKeyMappings::NextChar(TPtiTextCase aCase)
	{
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoKeyMapDataPanic, KErrCorrupt));
	
	iCurrentChar++;
	
	DeQwertyfyCaseValue(aCase);
		
	TPtrC keyData = DataForKeyLocal(iCurrentKey, aCase, iCurrentMode);
	if (iCurrentChar >= keyData.Length())
		{
		iCurrentChar = 0;
		}
	
	if (keyData.Length())
		{
		return keyData[iCurrentChar];
		}

	return 0;	
	}


// ---------------------------------------------------------------------------
// CPtiKeyMappings::ReplaceKeyMapL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiKeyMappings::ReplaceKeyMapL(TPtiKey aKey, TDesC& aMap,
                                              TPtiTextCase aCase)                                                                                            
	{
	if (iCurrentKey == aKey)
		{		
		iCurrentKey = EPtiKeyNone;
		iCurrentChar = 0;
		}	
		
	DeQwertyfyCaseValue(aCase);		
	
	ReplaceKeyMapLocalL(aKey, aMap, aCase, EPtiEngineMultitapping);
	
	return KErrNone;
	}


// ---------------------------------------------------------------------------
// CPtiKeyMappings::NextKey
// 
// ---------------------------------------------------------------------------
//	
EXPORT_C TUint16 CPtiKeyMappings::NextKey(TPtiKey aKey, TBool &aAppend,
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
// CPtiKeyMappings::KeyForCharacter
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TPtiKey CPtiKeyMappings::KeyForCharacter(TUint16 aChar)
	{
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoKeyMapDataPanic, KErrCorrupt));
	
	TInt numBindItems = 0;
	const TPtiKeyBinding* bindings = iKeyMapData->KeyBindingTable(iKeyboardType, numBindItems);
    TInt dataSize = 0;
	const TUint16* mtData = iKeyMapData->KeyData(iKeyboardType, dataSize);  
	
	TPtiTextCase textCase;
	TPtiKey key;
	for (TInt i = 0; i < numBindItems; i++)
		{
		key = iKeyMapData->KeyForBindingTableIndex(iKeyboardType, i, textCase);
		if (key != EPtiKeyNone)
			{
			TPtrC keyData = DataForKeyLocal(key, textCase, EPtiEngineMultitapping);
			if (keyData.Locate(aChar) != KErrNotFound)
				{
				return key;
				}			
			}
		}
			 
	return EPtiKeyNone;	
	}


// ---------------------------------------------------------------------------
// CPtiKeyMappings::GetDataForKey
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPtiKeyMappings::GetDataForKey(TPtiKey aKey, TDes& aResult, 
                                             TPtiTextCase aCase)
	{
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoKeyMapDataPanic, KErrCorrupt));

	DeQwertyfyCaseValue(aCase);

	aResult.Copy(DataForKeyLocal(aKey, aCase, EPtiEngineMultitapping).Left(aResult.MaxLength()));	
	}


// ---------------------------------------------------------------------------
// CPtiKeyMappings::GetAll
// 
// ---------------------------------------------------------------------------
//
TPtrC CPtiKeyMappings::GetAll(TPtiTextCase aCase)
	{
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoKeyMapDataPanic, KErrCorrupt));
	
	DeQwertyfyCaseValue(aCase);	
	
	TPtrC keyData = DataForKeyLocal(iCurrentKey, aCase, EPtiEngineMultitapping);	
	if (keyData.Length() > 1)	
		{
		// Return characters excluding the control char.
		return keyData.Mid(1);		
		}
	
	return TPtrC();	
	}
	
//
// CPtiHalfQwertyKeyMappings
// 

// ---------------------------------------------------------------------------
// CPtiHalfQwertyKeyMappings::NewL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPtiHalfQwertyKeyMappings* CPtiHalfQwertyKeyMappings::NewL(CPtiKeyMapData* aData)
    {
    CPtiHalfQwertyKeyMappings* maps = new (ELeave) CPtiHalfQwertyKeyMappings(aData);
	return maps;  
    }


// ---------------------------------------------------------------------------
// CPtiHalfQwertyKeyMappings::::CPtiHalfQwertyKeyMappings
// 
// ---------------------------------------------------------------------------
//
CPtiHalfQwertyKeyMappings::CPtiHalfQwertyKeyMappings(CPtiKeyMapData* aData) : CPtiKeyMappings(aData)
    { 
    iKeyboardType = EPtiKeyboardHalfQwerty;      
    }


// ---------------------------------------------------------------------------
// CPtiHalfQwertyKeyMappings::~CPtiHalfQwertyKeyMappings
// 
// ---------------------------------------------------------------------------
//	    
EXPORT_C CPtiHalfQwertyKeyMappings::~CPtiHalfQwertyKeyMappings()
    {    
    }

// ---------------------------------------------------------------------------
// CPtiHalfQwertyKeyMappings::StartMapping
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TUint16 CPtiHalfQwertyKeyMappings::StartMapping(TPtiKey aKey, TPtiTextCase aCase,
                                               TPtiEngineInputMode aMode)
	{
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoKeyMapDataPanic, KErrCorrupt));
	
	//DeQwertyfyCaseValue(aCase);
	
	KeyCodeToInternal(aKey);

    iCurrentMode = aMode;
	iCurrentChar = 0;
    
	TPtrC keyData = DataForKeyLocal(aKey, aCase, iCurrentMode);	
	if (keyData.Length() > 0)
	    {
	    return keyData[0];	
	    }        
	    
	           			
	return 0;
	}

// ---------------------------------------------------------------------------
// CPtiHalfQwertyKeyMappings::NextKey
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C TUint16 CPtiHalfQwertyKeyMappings::NextKey(TPtiKey aKey, TBool &aAppend,
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
// TUint16 CPtiHalfQwertyKeyMappings::NextChar(TPtiTextCase aCase)
// 
// ---------------------------------------------------------------------------
//        
TUint16 CPtiHalfQwertyKeyMappings::NextChar(TPtiTextCase aCase)
	{
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoKeyMapDataPanic, KErrCorrupt));
	
	iCurrentChar++;
	// we want to retain chr cases in half qwerty
	//DeQwertyfyCaseValue(aCase);
		
	TPtrC keyData = DataForKeyLocal(iCurrentKey, aCase, iCurrentMode);
	if (iCurrentChar >= keyData.Length())
		{
		iCurrentChar = 0;
		}
	
	if (keyData.Length())
		{
		return keyData[iCurrentChar];
		}

	return 0;	
	}
// ---------------------------------------------------------------------------
// CPtiHalfQwertyKeyMappings::ReplaceKeyMapL
// 
// ---------------------------------------------------------------------------
//        
EXPORT_C TInt CPtiHalfQwertyKeyMappings::ReplaceKeyMapL(TPtiKey aKey, TDesC& aMap, TPtiTextCase aCase)
    {
	if (iCurrentKey == aKey)
		{		
		iCurrentKey = EPtiKeyNone;
		iCurrentChar = 0;
		}	
		
//	DeQwertyfyCaseValue(aCase);		
	
	ReplaceKeyMapLocalL(aKey, aMap, aCase, EPtiEngineHalfQwerty);
	
	return KErrNone;        
    }
     
 
// ---------------------------------------------------------------------------
// CPtiHalfQwertyKeyMappings::KeyForCharacter
// 
// ---------------------------------------------------------------------------
//   
EXPORT_C TPtiKey CPtiHalfQwertyKeyMappings::KeyForCharacter(TUint16 aChar)
    {
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoKeyMapDataPanic, KErrCorrupt));
	
	TInt numBindItems = 0;
	const TPtiKeyBinding* bindings = iKeyMapData->KeyBindingTable(iKeyboardType, numBindItems);
    TInt dataSize = 0;
	const TUint16* mtData = iKeyMapData->KeyData(iKeyboardType, dataSize);  
	
	TPtiTextCase textCase;
	TPtiKey key;
	for (TInt i = 0; i < numBindItems; i++)
		{
		key = iKeyMapData->KeyForBindingTableIndex(iKeyboardType, i, textCase);
		if (key != EPtiKeyNone)
			{
			TPtrC keyData = DataForKeyLocal(key, textCase, EPtiEngineHalfQwerty);
			if (keyData.Locate(aChar) != KErrNotFound)
				{
				return key;
				}			
			}
		}
			 
	return EPtiKeyNone;	    
    }
    

// ---------------------------------------------------------------------------
// CPtiHalfQwertyKeyMappings::GetAll
// 
// ---------------------------------------------------------------------------
//    
EXPORT_C TPtrC CPtiHalfQwertyKeyMappings::GetAll(TPtiTextCase aCase)
    {
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoKeyMapDataPanic, KErrCorrupt));
	
//	DeQwertyfyCaseValue(aCase);	
	
	TPtrC keyData = DataForKeyLocal(iCurrentKey, aCase, EPtiEngineHalfQwerty);	
	if (keyData.Length() > 1)	
		{
		// Return characters excluding the control char.
		return keyData.Mid(1);		
		}
	
	return TPtrC();	    
    }	
    
    
// ---------------------------------------------------------------------------
// CPtiHalfQwertyKeyMappings::GetNumericModeKeysFromDataL
// 
// ---------------------------------------------------------------------------
// 
void CPtiHalfQwertyKeyMappings::GetNumericModeKeysFromDataL(RArray<TPtiNumericKeyBinding>& aResult)
	{
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoKeyMapDataPanic, KErrCorrupt));
	
	TInt numEntries = 0;
	const TPtiNumericKeyBinding* dataEntries = iKeyMapData->NumericModeKeysTable(iKeyboardType, 
	                                                                             numEntries);
	
	for (TInt i = 0; i < numEntries; i++)
		{
		User::LeaveIfError(aResult.Append(dataEntries[i]));
		}
	}    
	
	
// ---------------------------------------------------------------------------
// CPtiQwertyKeyMappings::GetDataForKey
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C void CPtiHalfQwertyKeyMappings::GetDataForKey(TPtiKey aKey, TDes& aResult,
                                                       TPtiTextCase aCase)
	{
	__ASSERT_DEBUG(iKeyMapData, User::Panic(KPtiNoKeyMapDataPanic, KErrCorrupt));

	aResult.Copy(DataForKeyLocal(aKey, aCase, EPtiEngineHalfQwerty).Left(aResult.MaxLength()));
	}
		

// End of file


