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
* Description:   PtiLanguage class implementation.
*
*/


#include "PtiKeyMappings.h"
#include "PtiLanguage.h"
#include <PtiKeyMapData.h>
#include <PtiCore.h>
#include "PtiDefaultCore.h"

#ifdef _DEBUG
_LIT(KPtiEngineLangPanic, "CPtiLanguage");
#endif

//
// TPtiKeyMappingsHolder
//

// ---------------------------------------------------------------------------
// TPtiKeyMappingsHolder::TPtiKeyMappingsHolder
// 
// ---------------------------------------------------------------------------
// 
TPtiKeyMappingsHolder::TPtiKeyMappingsHolder()
    {
    iKeyMappings = NULL;
    iQwertyKeyMappings = NULL;
    iHalfQwertyMappings = NULL;       
    }
   
    
// ---------------------------------------------------------------------------
// TPtiKeyMappingsHolder::~TPtiKeyMappingsHolder
// 
// ---------------------------------------------------------------------------
//     
TPtiKeyMappingsHolder::~TPtiKeyMappingsHolder()
    {
    Clear();
    }
    
    
// ---------------------------------------------------------------------------
// TPtiKeyMappingsHolder::´Clear()
// 
// ---------------------------------------------------------------------------
//         
void TPtiKeyMappingsHolder::Clear()
    {
    delete iKeyMappings;
    iKeyMappings = NULL;
    delete iQwertyKeyMappings;
    iQwertyKeyMappings = NULL;
    delete iHalfQwertyMappings;          
    iHalfQwertyMappings = NULL;
    }
    
    

//
// CPtiCoreLanguage
//

// ---------------------------------------------------------------------------
// CPtiCoreLanguage::NewL
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C CPtiCoreLanguage* CPtiCoreLanguage::NewL()
	{
	CPtiCoreLanguage* lang = new (ELeave) CPtiCoreLanguage();
	CleanupStack::PushL( lang );
	lang->ConstructL();
	CleanupStack::Pop(); // lang
	return lang;
	}


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::CPtiCoreLanguage
// 
// ---------------------------------------------------------------------------
// 
CPtiCoreLanguage::CPtiCoreLanguage()
	{
	}


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::~CPtiCoreLanguage
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C CPtiCoreLanguage::~CPtiCoreLanguage()
	{
	Clear();
	delete iHolder;
	iHolder = NULL;		
	}


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::ConstructL
// 
// ---------------------------------------------------------------------------
// 
void CPtiCoreLanguage::ConstructL()
	{
	iHolder = new (ELeave) TPtiKeyMappingsHolder();
	}


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::Clear
// 
// ---------------------------------------------------------------------------
// 
void CPtiCoreLanguage::Clear()
    {	
	if (iHolder)
	    {
        iHolder->Clear();		
	    }
	delete iKeyMapData;
	iKeyMapData = NULL;	
	iCores.Close();
	}


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::SetLanguageCode
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C void CPtiCoreLanguage::SetLanguageCode(TInt aCode)
	{
	if (aCode == iLanguageCode)
		{
		return;	
		}

	Clear();
	iLanguageCode = aCode;
	}


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::SetLocalizedName
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C void CPtiCoreLanguage::SetLocalizedName(const TDesC& aName)
	{	
	iLocalizedName.Copy(aName.Left(iLocalizedName.MaxLength()));	
	}


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::GetKeymappings
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C MPtiKeyMappings* CPtiCoreLanguage::GetKeymappings() const
	{
	__ASSERT_DEBUG(iHolder, User::Panic(KPtiEngineLangPanic, KErrCorrupt));	
	
	if (!iKeyMapData)	
		{
	    if (!iDataFactory)
		    {
		    CPtiDefaultCore* defaultCore = static_cast<CPtiDefaultCore*> (GetCoreForNumericMode());
		    if (defaultCore)
		        {
		        TRAPD(err, defaultCore->LoadFactoryForLanguageL(LanguageCode()));
		        if(err != KErrNone)
		        	{
		        	return NULL;
		        	}
		        }
            else
                {
                return NULL;
                }		        
		    }  		
		
		TRAP_IGNORE(LoadKeymappingsL());	
		}
	
	return iHolder->iKeyMappings;
	}


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::GetQwertyKeymappings
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C MPtiKeyMappings* CPtiCoreLanguage::GetQwertyKeymappings() const
	{
	__ASSERT_DEBUG(iHolder, User::Panic(KPtiEngineLangPanic, KErrCorrupt));	
	
	if (!iKeyMapData)	
		{
		if (!iDataFactory)
		    {
		    CPtiDefaultCore* defaultCore = static_cast<CPtiDefaultCore*> (GetCoreForNumericMode());
		    if (defaultCore)
		        {
		        TRAPD(err, defaultCore->LoadFactoryForLanguageL(LanguageCode()));
		        if(err != KErrNone)
		        	{
		        	return NULL;
		        	}
		        }
            else
                {
                return NULL;
                }		        
		    }  

		
		TRAP_IGNORE(LoadKeymappingsL());	
		}
	
	return iHolder->iQwertyKeyMappings;		
	}


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::ClearMultitapping
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C void CPtiCoreLanguage::ClearMultitapping()
	{
	__ASSERT_DEBUG(iHolder, User::Panic(KPtiEngineLangPanic, KErrCorrupt));	
		
	delete iHolder->iKeyMappings;
	iHolder->iKeyMappings = NULL;
	}


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::ClearQwerty
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C void CPtiCoreLanguage::ClearQwerty()
	{
	__ASSERT_DEBUG(iHolder, User::Panic(KPtiEngineLangPanic, KErrCorrupt));		
	
	delete iHolder->iQwertyKeyMappings;
	iHolder->iQwertyKeyMappings = NULL;
	}


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::LocalizedNameDes
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C TDes& CPtiCoreLanguage::LocalizedNameDes()
	{
	return iLocalizedName;
	}


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::GetCore
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C MPtiCore* CPtiCoreLanguage::GetCore(TPtiEngineInputMode aMode)
	{
	if (aMode == EPtiEngineNumeric)
		{
		return GetCoreForNumericMode();	
		}
	
	if (!iKeyMapData)	
		{
		TRAP_IGNORE(LoadKeymappingsL());	
		}		
	
	for (TInt i = 0; i < iCores.Count(); i++)
		{
		if (iCores[i].iInputMode == aMode)
			{
			return iCores[i].iCore;
			}
		}
	
	return NULL;
	}


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::SetCore
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C void CPtiCoreLanguage::SetCore(MPtiCore *aCore, TPtiEngineInputMode aMode)
	{
	SetCoreLocal(aCore, aMode);
	}


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::SetKeyMappings
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C void CPtiCoreLanguage::SetKeyMappings(MPtiKeyMappings* aMappings)
	{
	__ASSERT_DEBUG(iHolder, User::Panic(KPtiEngineLangPanic, KErrCorrupt));		
	
	delete iHolder->iKeyMappings;
	iHolder->iKeyMappings = aMappings;
	}


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::SetQwertyKeyMappings
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C void CPtiCoreLanguage::SetQwertyKeyMappings(MPtiKeyMappings* aMappings)
	{
	__ASSERT_DEBUG(iHolder, User::Panic(KPtiEngineLangPanic, KErrCorrupt));		
	
	delete iHolder->iQwertyKeyMappings;
	iHolder->iQwertyKeyMappings = aMappings;
	}


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::HasInputMode
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C TBool CPtiCoreLanguage::HasInputMode(TPtiEngineInputMode aMode) const
	{
	if (!iKeyMapData)	
		{
		TRAP_IGNORE(LoadKeymappingsL());	
		}	
	
	for (TInt i = 0; i < iCores.Count(); i++)
		{
		if (iCores[i].iInputMode == aMode)
			{	
			return ETrue;
			}
		}
	
	return EFalse;
	}


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::HasCore
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C TBool CPtiCoreLanguage::HasCore(MPtiCore* aCore) const
	{
	if (!iKeyMapData)	
		{
		TRAP_IGNORE(LoadKeymappingsL());	
		}	
	
	for (TInt i = 0; i < iCores.Count(); i++)
		{
		if (iCores[i].iCore == aCore)
			{
			return ETrue;
			}
		}

	return EFalse;
	}


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::GetAvailableInputModesL
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C void CPtiCoreLanguage::GetAvailableInputModesL(CArrayFix<TInt>* aResult)
	{
	if (!iKeyMapData)	
		{
		TRAP_IGNORE(LoadKeymappingsL());	
		}	
	
	if (aResult)
		{
		for (TInt i = 0; i < iCores.Count(); i++)
			{
			aResult->AppendL(iCores[i].iInputMode);
			}
		}
	}


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::GetAvailableInputModesL
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C void CPtiCoreLanguage::GetAvailableInputModesL(RArray<TInt>& aResult)
	{
	if (!iKeyMapData)	
		{
		TRAP_IGNORE(LoadKeymappingsL());	
		}	
	
	for (TInt i = 0; i < iCores.Count(); i++)
		{
		aResult.AppendL(iCores[i].iInputMode);
		}	
	}
	

// ---------------------------------------------------------------------------
// CPtiCoreLanguage::RemoveCore
// 
// ---------------------------------------------------------------------------
// 	
EXPORT_C void CPtiCoreLanguage::RemoveCore(MPtiCore* aCore)
	{
	for (TInt i = 0; i < iCores.Count(); i++)	
		{
		if (iCores[i].iCore == aCore)	
			{
			iCores.Remove(i);
			i--;			
			}		
		}	
	}
	
	
// ---------------------------------------------------------------------------
// CPtiCoreLanguage::GetCoreForNumericMode
// 
// ---------------------------------------------------------------------------
// 	
CPtiCore* CPtiCoreLanguage::GetCoreForNumericMode() const
	{
	for (TInt i = 0; i < iCores.Count(); i++)	
		{
		if (iCores[i].iInputMode == EPtiEngineNumeric)	
			{
			return static_cast<CPtiCore*>(iCores[i].iCore);						
			}		
		}		
	
	return NULL;
	}
	
	
// ---------------------------------------------------------------------------
// CPtiCoreLanguage::LoadKeymappingsL
// 
// ---------------------------------------------------------------------------
// 	
void CPtiCoreLanguage::LoadKeymappingsL() const
	{	
	__ASSERT_DEBUG(iHolder, User::Panic(KPtiEngineLangPanic, KErrCorrupt));		
	
    iHolder->Clear();
    
	delete iKeyMapData;
	iKeyMapData = NULL; 
	
	if (iDataFactory)
	    {	    		
	    CPtiCore* defaultCore = GetCoreForNumericMode();	
	    iKeyMapData = static_cast<CPtiKeyMapData*>(iDataFactory->KeyMapDataForLanguageL(iLanguageCode));
	    if (iKeyMapData)
		    {
		    if (iKeyMapData->HasKeyData(EPtiKeyboard12Key))	
			    {				
			    iHolder->iKeyMappings = CPtiKeyMappings::NewL(iKeyMapData);
			    SetCoreLocal(defaultCore, EPtiEngineMultitapping);					
			    }
		    if (iKeyMapData->HasKeyData(EPtiKeyboardQwerty4x12) ||
		        iKeyMapData->HasKeyData(EPtiKeyboardQwerty4x10) ||
		        iKeyMapData->HasKeyData(EPtiKeyboardQwerty3x11))							
			    {					
			    SetCoreLocal(defaultCore, EPtiEngineQwerty);													
			    iHolder->iQwertyKeyMappings = CPtiQwertyKeyMappings::NewL(iKeyMapData);					
			    }
		    if (iKeyMapData->HasKeyData(EPtiKeyboardHalfQwerty))							
			    {					
			    SetCoreLocal(defaultCore, EPtiEngineHalfQwerty);																
		        iHolder->iHalfQwertyMappings = CPtiHalfQwertyKeyMappings::NewL(iKeyMapData);									    				  
			    }																			
		    }		
	    }
						
	}
	
	
// ---------------------------------------------------------------------------
// CPtiCoreLanguage::SetCoreLocal
// 
// ---------------------------------------------------------------------------
// 	
void CPtiCoreLanguage::SetCoreLocal(MPtiCore *aCore, TPtiEngineInputMode aMode) const
	{
	if (aMode > EPtiEngineInputModeNone && aMode <= EPtiEngineMaxInputModes)
		{
		for (TInt i = 0; i < iCores.Count(); i++)
			{
			if (iCores[i].iInputMode == aMode)
				{
				iCores[i].iCore = aCore;
				return;
				}				
			}

		TPtiCoreBinding binding;
		binding.iCore = aCore;
		binding.iInputMode = aMode;
		// If this fails, then it just fails.
		iCores.Append(binding);
		}
	}
	
	
// ---------------------------------------------------------------------------
// CPtiCoreLanguage::GetHalfQwertyKeymappings
// 
// ---------------------------------------------------------------------------
// 		
EXPORT_C MPtiKeyMappings* CPtiCoreLanguage::GetHalfQwertyKeymappings() const
    {
	__ASSERT_DEBUG(iHolder, User::Panic(KPtiEngineLangPanic, KErrCorrupt));	
        
	if (!iKeyMapData)	
		{
	    if (!iDataFactory)
		    {
		    CPtiDefaultCore* defaultCore = static_cast<CPtiDefaultCore*> (GetCoreForNumericMode());
		    if (defaultCore)
		        {
		        TRAPD(err, defaultCore->LoadFactoryForLanguageL(LanguageCode()));
		        if(err != KErrNone)
		        	{
		        	return NULL;
		        	}
		        }
            else
                {
                return NULL;
                }		        
		    }  		
		TRAP_IGNORE(LoadKeymappingsL());	
		}
	
	return iHolder->iHalfQwertyMappings;	    
    }				


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::SetHalfQwertyKeyMappings
// 
// ---------------------------------------------------------------------------
// 			
EXPORT_C void CPtiCoreLanguage::SetHalfQwertyKeyMappings(MPtiKeyMappings* aMappings)
    {
	__ASSERT_DEBUG(iHolder, User::Panic(KPtiEngineLangPanic, KErrCorrupt));	    
    
	delete iHolder->iHalfQwertyMappings;
	iHolder->iHalfQwertyMappings = aMappings;    
    }


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::SetHalfQwertyKeyMappings
// 
// ---------------------------------------------------------------------------
// 	
EXPORT_C void CPtiCoreLanguage::ClearHalfQwerty()
    {
	__ASSERT_DEBUG(iHolder, User::Panic(KPtiEngineLangPanic, KErrCorrupt));		
	
	delete iHolder->iHalfQwertyMappings;
	iHolder->iHalfQwertyMappings = NULL;    
    }


// ---------------------------------------------------------------------------
// CPtiCoreLanguage::SetKeyMapDataFactory
// 
// ---------------------------------------------------------------------------
//	
void CPtiCoreLanguage::SetKeyMapDataFactory(CPtiKeyMapDataFactory* aFactory)
    {
    if (iDataFactory != aFactory)
        {
        iHolder->Clear();       
	    delete iKeyMapData;
    	iKeyMapData = NULL;     		
        iDataFactory = aFactory;            
        }
    }
    
    
// ---------------------------------------------------------------------------
// CPtiCoreLanguage::DataFactory
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPtiKeyMapDataFactory* CPtiCoreLanguage::DataFactory() const
    {
    return iDataFactory;    
    }	
    
    
// ---------------------------------------------------------------------------
// CPtiCoreLanguage::DataFactory
// 
// ---------------------------------------------------------------------------
//    
EXPORT_C CPtiKeyMapData* CPtiCoreLanguage::RawKeyMapData() const
    {
    return iKeyMapData;
    }

// End of file
