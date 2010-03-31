/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*
*
*/


// INCLUDE FILES

#include "AknInputLanguageInfo.h"
#include "ptiInputLanguageInfo.h"
#include "e32cmn.h"

// CONSTANTS
const TInt KInputLanguageArrayGranularity = 10;

// CLASS IMPLEMENTATION

CT9InputLanguageInfo::CT9InputLanguageInfo()
	{}


CT9InputLanguageInfo* CT9InputLanguageInfo::NewL()
	{
	CT9InputLanguageInfo* self = new(ELeave)CT9InputLanguageInfo();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();	//self
	return self;
	}

void CT9InputLanguageInfo::ConstructL()
	{
    // This implementation uses english dictionary by default
    iPtiEngineInterface = CPtiEngine::NewL(); 
    iPtiEngineInterface->ActivateLanguageL(ELangEnglish); 
	}

EXPORT_C CT9InputLanguageInfo::~CT9InputLanguageInfo()
	{
	if(iPtiEngineInterface)
		{
		TRAP_IGNORE( iPtiEngineInterface->CloseCurrentLanguageL());
		}
	delete iPtiEngineInterface;
	}


EXPORT_C void CT9InputLanguageInfo::AppendAvailableLanguagesL( CAknInputLanguageList* aList ) 
	{
	TAknInputLanguageCapabilities capabilityFilter;
	capabilityFilter.SetAllCapabilities();
	// Implemented by calling the more generic method with all filtering off:
	AppendLanguagesL( aList, NULL, capabilityFilter );
	}

EXPORT_C void CT9InputLanguageInfo::AppendLanguagesL( 
													 CAknInputLanguageList* aList, 
													 CArrayFix<TInt>* aLanguageCodeList, 
													 TAknInputLanguageCapabilities& aCapabilityFilter) 
	{
    // Provide new arrays for the call 
	CArrayFixFlat<TInt>* langCodes = new (ELeave) CArrayFixFlat<TInt>( KInputLanguageArrayGranularity );
	CleanupStack::PushL( langCodes );

    iPtiEngineInterface->GetAvailableLanguagesL( langCodes );
    
    TInt count = langCodes->Count();

    // Loop over the codes, appending them into the existing array;
	for ( TInt index = 0; index < count; ++index )
		{
        TLanguage langCode = (TLanguage)langCodes->At(index);

		// Filter by language code if array exists
		if (  !( aLanguageCodeList && !IsInLanguageCodeList(aLanguageCodeList, langCode ) ) )
			{     
            TBool chineseLanguage = (langCode == ELangTaiwanChinese || langCode == ELangHongKongChinese || langCode == ELangPrcChinese);

            // Temporary solution for Chinese variants.
            // All Chinese languages are shown in the input language list.
            // In western functionality we strip out secondary languages.
            if ( chineseLanguage || !(!aLanguageCodeList && 0) ) // missing secondary language functionality 
                {
                TAknInputLanguageCapabilities capabilities = LanguageCapabilitiesFromLanguage( langCode );
                // Test is passed if the filter has no support at all (no filtering to be done)
                // or there is a filter, but the language supports it
                if ( !aCapabilityFilter.HasAnySupport() || (capabilities.FilteredCapabilities( aCapabilityFilter )).HasAnySupport() )
                    {
                    CAknInputLanguageItem* inputItem = CAknInputLanguageItem::NewL( langCode, LanguageName(langCode), capabilities );
                    CleanupStack::PushL( inputItem );	
                    aList->AppendL( inputItem ); // Takes full ownership
                    CleanupStack::Pop(); // inputItem
                    }
                }
			}
		}

	CleanupStack::PopAndDestroy(1); // langcodes 
	}

TBool CT9InputLanguageInfo::IsInLanguageCodeList( CArrayFix<TInt>* aLanguageList, TLanguage aLanguageCode )
	{
	TBool retVal = EFalse;

	TInt index;
	TInt count = aLanguageList->Count();
	for (index = 0; index < count; ++index )
		{
        TInt language = aLanguageList->At( index );
        language &= 0x03FF;
        if (language == ELangOther)
            {
            language = ELangEnglish;
            }
        if (language == aLanguageCode)
			{
			retVal = ETrue;
			break;
			}
		}
	return retVal;
	}


EXPORT_C TAknInputLanguageCapabilities CT9InputLanguageInfo::LanguageCapabilitiesFromLanguage( TLanguage aLanguageId )
    {
    // essentially this is to map the capabilities to the non-t9 defined ones:
    
    MPtiLanguage* lang = iPtiEngineInterface->GetLanguage(aLanguageId);
    
    TAknInputLanguageCapabilities returnCapabilities;
    
    if ( lang )
        {
        returnCapabilities.AssignCapability( TAknInputLanguageCapabilities::EMultitap, ETrue );
        
        if (!lang->HasInputMode(EPtiEngineMultitapping))
            {
            returnCapabilities.AssignCapability( TAknInputLanguageCapabilities::EMultitap, EFalse );
            }
        
        if (lang->HasInputMode(EPtiEnginePredictive))
            {
            returnCapabilities.AssignCapability( TAknInputLanguageCapabilities::EPredictive, ETrue );
            }
        }
    else
        {
        returnCapabilities.AssignCapability( TAknInputLanguageCapabilities::EMultitap, EFalse );
        }
    
    return returnCapabilities;
    }

EXPORT_C TAknLanguageName CT9InputLanguageInfo::LanguageName( TLanguage aLanguageCode ) const
	{
    MPtiLanguage* lang = iPtiEngineInterface->GetLanguage(aLanguageCode);
    
    TAknLanguageName buf;
    if( lang != NULL )
        {
        buf.Copy( lang->LocalizedName() );
        }
    else
        {
        buf.Copy( KNullDesC );
        }
    return buf;
	}

EXPORT_C TLanguage CT9InputLanguageInfo::UrlLanguage() const
	{
	// English returned for default implementation
	return ELangEnglish;
	}

//
// Class CAknInputLanguageInfoFactory
//

EXPORT_C CAknInputLanguageInfo* AknInputLanguageInfoFactory::CreateInputLanguageInfoL()
	{
	CAknInputLanguageInfo* info = CT9InputLanguageInfo::NewL();
	return info;
	}

// End of File
