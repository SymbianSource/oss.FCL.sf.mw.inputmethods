/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   CPtiKeyMapData class implementation.
*
*/


#include <ecom/ecom.h>
#include <PtiKeyMapData.h>
#include <languages.hrh>

class TPtiLangImplBinding
	{
	public:
		TInt iUid;
		TInt iLangCode;		 
	};

const TInt KPtiDefaultNumMultitapKeys = 12;   // Conventional phone keypad.
const TInt KPtiDefaultNumQwertyKeys = 48;     // 4x12 qwerty keyboard matrix.
const TInt KPtiDefaultNum4x10QwertyKeys = 40; // 4x10 Mini qwerty keyboard
const TInt KPtiDefaultNum3x11QwertyKeys = 33; // 3x11 Mini qwerty keyboard
const TInt KPtiDefaultNumHalfQwertyKeys = 20; // 4x5 Half qwerty keyboard
const TInt KPtiLargeNumber = 100000;
const TUid KPtiKeymapDataInterfaceUid = { 0x10281896 };

void CleanupArray( TAny* aAny )
	{
	RImplInfoPtrArray* implArray = 
		reinterpret_cast< RImplInfoPtrArray*> ( aAny );
	implArray->ResetAndDestroy();
	implArray->Close();
	}

const TInt KNumItemsPerDeadKeyDataRow = 6;

//
// CPtiKeyMapData
//

// ---------------------------------------------------------------------------
// CPtiKeyMapData::KeyForBindingTableIndex
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TPtiKey CPtiKeyMapData::KeyForBindingTableIndex(TPtiKeyboardType aKeyboardType,
                                                         TInt aIndex, TPtiTextCase& aCase) const
	{
    TInt numItems = 0;         
    const TPtiKeyBinding* table = KeyBindingTable(aKeyboardType, numItems);
	
	if (table && aIndex < numItems)
	    {
	    aCase = (TPtiTextCase)table[aIndex].iCase;
	    return (TPtiKey)table[aIndex].iScanCode;
	    }
			
	return EPtiKeyNone;	
	}
	
			
// ---------------------------------------------------------------------------
// CPtiKeyMapData::~CPtiKeyMapData
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPtiKeyMapData::~CPtiKeyMapData()
	{
	}
	

// ---------------------------------------------------------------------------
// CPtiKeyMapData::LanguageCode
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiKeyMapData::LanguageCode() const
	{
	return 0;
	}


// ---------------------------------------------------------------------------
// CPtiKeyMapData::HasKeyData
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiKeyMapData::HasKeyData(TPtiKeyboardType aKeyboardType) const
    {
	TInt tmp = 0;
	return (KeyData(aKeyboardType, tmp) != NULL);	        
    }	
    
    
// ---------------------------------------------------------------------------
// CPtiKeyMapData::HasDeadKeys
// 
// ---------------------------------------------------------------------------
//   
EXPORT_C TBool CPtiKeyMapData::HasDeadKeys(TPtiKeyboardType aKeyboardType) const
    {
	TInt tmp = 0;
	return (DeadKeyDataArray(aKeyboardType, tmp) != NULL);    
    }
    
    
// ---------------------------------------------------------------------------
// CPtiKeyMapData::HasVowelSequences
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiKeyMapData::HasVowelSequences(TPtiKeyboardType aKeyboardType) const
	{
	TInt tmp = 0;
	return (VowelSequenceTable(aKeyboardType, tmp) != NULL);
	}
    
        
// ---------------------------------------------------------------------------
// CPtiKeyMapData::DataForKey
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TPtrC CPtiKeyMapData::DataForKey(TPtiKeyboardType aKeyboardType,
                                          TPtiKey aKey,
                                          TPtiTextCase aCase) const
	{		
    TInt index = CaseBasedIndexInBindingTable(aKeyboardType, aKey, aCase);       
    
    if (index != KErrNotFound)
    	{    		
    	return DataForKey(aKeyboardType, index);    	
    	}

	return TPtrC();    	
	}
    

// ---------------------------------------------------------------------------
// CPtiKeyMapData::DataForKey
// 
// ---------------------------------------------------------------------------
//	
EXPORT_C TPtrC CPtiKeyMapData::DataForKey(TPtiKeyboardType aKeyboardType,
                                          TInt aIndex) const	
	{	
	TInt dataSize = 0;
	TInt numBindItems = 0;
	
	const TPtiKeyBinding* bindPtr = KeyBindingTable(aKeyboardType, numBindItems);
	const TUint16* dataPtr = KeyData(aKeyboardType, dataSize);
		
	if (aIndex < numBindItems)
        {           
        if (bindPtr[aIndex].iIndex < dataSize)
        	{
            return TPtrC(&dataPtr[bindPtr[aIndex].iIndex], bindPtr[aIndex].iNumChars);
        	}
        }
        
	return TPtrC();    
	}

	
// ---------------------------------------------------------------------------
// CPtiKeyMapData::IsDeadKey
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiKeyMapData::IsDeadKey(TPtiKeyboardType /*aKeyboardType*/,
                                         TPtiKey /*aKey*/,
                                         TPtiTextCase /*aCase*/) const
	{
	return EFalse; // ONKO TURHA METODI?
	}
	
		
// ---------------------------------------------------------------------------
// CPtiKeyMapData::DeadKeyDataForKey
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TPtrC CPtiKeyMapData::DeadKeyDataForKey(TPtiKeyboardType aKeyboardType,
                                                 TPtiKey aKey,
                                                 TPtiTextCase aCase) const
	{
	if (!HasDeadKeys(aKeyboardType))	
		{
		return TPtrC();	
		}
		
  	TInt index = CaseBasedIndexInBindingTable(aKeyboardType, aKey, aCase); 
  	    	
    if (index != KErrNotFound)
        {        
   	    TInt numRows = 0;	
	    const TUint16* data = DeadKeyDataArray(aKeyboardType, numRows);		
	
	    for (TInt i = 0; i < numRows; i++)
		    {
		    if (data[0] == index)
			    {
			    data++;
			    return TPtrC(data, KNumItemsPerDeadKeyDataRow - 1);
			    }
		    data += KNumItemsPerDeadKeyDataRow;
		    }
        }
	
	return TPtrC();
	}
		
		
// ---------------------------------------------------------------------------
// CPtiKeyMapData::VowelSequenceTable
// 
// ---------------------------------------------------------------------------
//
EXPORT_C const TVowelSequence* CPtiKeyMapData::VowelSequenceTable(TPtiKeyboardType /*aKeyboardType*/,
                                                                  TInt& aNumEntries) const
	{
	aNumEntries = 0;
	return NULL;		
	}				
	                  
	
// ---------------------------------------------------------------------------
// CPtiKeyMapData::NumberOfKeys
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiKeyMapData::NumberOfKeys(TPtiKeyboardType aKeyboardType) const
	{
	switch (aKeyboardType)
	    {
        case EPtiKeyboard12Key:
             return KPtiDefaultNumMultitapKeys;	   
        case EPtiKeyboardQwerty4x12:
             return KPtiDefaultNumQwertyKeys;                
        case EPtiKeyboardQwerty4x10: 
             return KPtiDefaultNum4x10QwertyKeys;	       
        case EPtiKeyboardQwerty3x11: 
             return KPtiDefaultNum3x11QwertyKeys;	                      	    	    
        case EPtiKeyboardHalfQwerty: 
             return KPtiDefaultNumHalfQwertyKeys;	               	    	                 
	    default:
	        break;
	    }
	    
    return 0;	    			
	}		
	
  
// ---------------------------------------------------------------------------
// CPtiKeyMapData::KeyData
// 
// ---------------------------------------------------------------------------
//
EXPORT_C const TUint16* CPtiKeyMapData::KeyData(TPtiKeyboardType /*aKeyboardType*/,
                                                TInt& aDataSize) const
	{
	aDataSize = 0;
	return NULL;		
	}  
  
  
// ---------------------------------------------------------------------------
// CPtiKeyMapData::KeyBindingTable
// 
// ---------------------------------------------------------------------------
//		
EXPORT_C const TPtiKeyBinding* CPtiKeyMapData::KeyBindingTable(TPtiKeyboardType /*aKeyboardType*/,
                                                               TInt& aNumItems) const
	{
	aNumItems = 0;
	return NULL;			
	}
	    	     
				
// ---------------------------------------------------------------------------
// CPtiKeyMapData::GetNumericModeKeysL 
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPtiKeyMapData::GetNumericModeKeysL(TPtiKeyboardType aKeyboardType,
                                                  RArray<TPtiNumericKeyBinding>& aResult) const
	{	
	TInt numItems = 0;
	const TPtiNumericKeyBinding* data = NumericModeKeysTable(aKeyboardType, numItems);

	aResult.Reset();

	if (data)
		{			
		for (TInt i = 0; i < numItems; i++)
			{
			User::LeaveIfError(aResult.Append(data[i]));
			}
		}
	}	 	 	
	 	 		 
	       	
// ---------------------------------------------------------------------------
// CPtiKeyMapData::IsNumberModeKey
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiKeyMapData::IsNumberModeKey(TPtiKeyboardType aKeyboardType,
                                               TPtiNumericKeyBinding& aWhichKey) const
	{
	TInt numItems = 0;
	aWhichKey.iChar = 0;
		
	const TPtiNumericKeyBinding* data = NumericModeKeysTable(aKeyboardType, numItems);
	
	if (data)
		{
		for (TInt i = 0; i < numItems; i++)
			{			 
			if (aWhichKey.iKey == data[i].iKey &&
			    aWhichKey.iCase == data[i].iCase)
				{
				aWhichKey.iChar = data[i].iChar;
				return ETrue;
				}			
			}		
		}

	return EFalse;	
	}	    
	       	
	       		       				    	    	  
// ---------------------------------------------------------------------------
// CPtiKeyMapData::NumericModeKeysTable
// 
// ---------------------------------------------------------------------------
//
EXPORT_C const TPtiNumericKeyBinding* CPtiKeyMapData::NumericModeKeysTable(TPtiKeyboardType /*aKeyboardType*/,
                                                                           TInt& aNumEntries) const
	{
	aNumEntries = 0;
	return NULL;	
	}	    
		    	  
	 
// ---------------------------------------------------------------------------
// CPtiKeyMapData::SuitableForLatinOnlyMode
// 
// ---------------------------------------------------------------------------
//	
EXPORT_C TBool CPtiKeyMapData::SuitableForLatinOnlyMode() const
	{
	return ETrue;
	}	    								


// ---------------------------------------------------------------------------
// CPtiKeyMapData::DeadKeyDataArray
// 
// ---------------------------------------------------------------------------
//
EXPORT_C const TUint16* CPtiKeyMapData::DeadKeyDataArray(TPtiKeyboardType /*aKeyboardType*/,
                                                         TInt& aNumRowsInArray) const
	{
	aNumRowsInArray = 0;
	return NULL;	
	}
	
	
// ---------------------------------------------------------------------------
// CPtiKeyMapData::HasFnKeyBindings
// 
// ---------------------------------------------------------------------------
//	
EXPORT_C TBool CPtiKeyMapData::HasFnKeyBindings(TPtiKeyboardType aKeyboardType) const
    {
    TInt numItems = 0;
    KeyBindingTable(aKeyboardType, numItems);
    
    if (numItems > 0)
        {
        if (numItems > NumberOfKeys(aKeyboardType) * 2)
            {
            // If there are data items beyond upper and lower case bindings,
            // they must be fn-key data.
            return ETrue;
            }       
        }
        
    return EFalse;
    }	
	

// ---------------------------------------------------------------------------
// CPtiKeyMapData::CaseBasedIndexInBindingTable
// 
// ---------------------------------------------------------------------------
//		
EXPORT_C TInt CPtiKeyMapData::CaseBasedIndexInBindingTable(TPtiKeyboardType aKeyboardType,
                                                           TPtiKey aKey,                                                     
	                                                       TPtiTextCase aCase) const
    {
    TInt numItems = 0;         
    const TPtiKeyBinding* table = KeyBindingTable(aKeyboardType, numItems);
    
    if (table)
        {
        for (TInt i = 0; i < numItems; i++)
            {
            if (table[i].iScanCode == aKey && table[i].iCase == aCase)
                {
                return i;
                }
            }
        }
                       
    return KPtiLargeNumber;
    }
	
			   
// ---------------------------------------------------------------------------
// Reserved ordinals.
// 
// ---------------------------------------------------------------------------
//	     
EXPORT_C void CPtiKeyMapData::Reserved_1()
	{
	}
	
EXPORT_C void CPtiKeyMapData::Reserved_2()
	{
	}
	
EXPORT_C void CPtiKeyMapData::Reserved_3()
	{
	}
	
EXPORT_C void CPtiKeyMapData::Reserved_4()
	{
	}	

//
// CPtiKeyMapDataFactory
//

// ---------------------------------------------------------------------------
// CPtiKeyMapDataFactory::CreateImplementationL
// 
// ---------------------------------------------------------------------------
//	
EXPORT_C CPtiKeyMapDataFactory* CPtiKeyMapDataFactory::CreateImplementationL(const TUid aImplUid)
    {
	TUid dtorIDKey;
	CPtiKeyMapDataFactory* iface = (CPtiKeyMapDataFactory*)REComSession::CreateImplementationL(aImplUid, dtorIDKey);		
	if (iface)
		{
		iface->SetDestructorKeyId(dtorIDKey.iUid);
		iface->SetImplementationUid(aImplUid. iUid);
		}
	return iface;	        
    }


// ---------------------------------------------------------------------------
// CPtiKeyMapDataFactory::ListImplementationsL
// 
// ---------------------------------------------------------------------------
//			
EXPORT_C void CPtiKeyMapDataFactory::ListImplementationsL(RArray<TInt>& aResult)
    {
	RImplInfoPtrArray infoArray;
	TInt i;

	aResult.Reset();
	CleanupStack::PushL( TCleanupItem( CleanupArray, &infoArray ) );
	REComSession::ListImplementationsL(KPtiKeymapDataInterfaceUid, infoArray);

	for (i = 0; i < infoArray.Count(); i++)
		{
		aResult.AppendL(infoArray[i]->ImplementationUid().iUid);
		}

	CleanupStack::PopAndDestroy(); // infoArray     
    }
        	
        	
// ---------------------------------------------------------------------------
// CPtiKeyMapDataFactory::~CPtiKeyMapDataFactory
// 
// ---------------------------------------------------------------------------
//	
EXPORT_C CPtiKeyMapDataFactory::~CPtiKeyMapDataFactory()
    {
 	REComSession::DestroyedImplementation(TUid::Uid(iDTorId));
    }


// ---------------------------------------------------------------------------
// Reserved ordinals.
// 
// ---------------------------------------------------------------------------
//	  
EXPORT_C void CPtiKeyMapDataFactory::Reserved_1()
    {    
    }
    
    
EXPORT_C void CPtiKeyMapDataFactory::Reserved_2()
    {    
    }

// End of file


