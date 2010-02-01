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
*       This is a private header
*
*       Implementation of the CAknInputLanguageInfo-conformant object. This actually has
*       all the functionality.
*       This file also contains the implementation of the factory object itself that returns
*       an instance of CT9InputLanguageInfo.
*
*/


#if !defined(__ET9LANGUAGEINFO_H__)
#define __ET9LANGUAGEINFO_H__

#include <e32base.h>
#include <AknInputLanguageInfo.h>
#include <PtiEngine.h>

/**
* This is a specialization of the Avkon input language class. It may be returned by 
* a factory method to provide available language information to a client
*
* It owns a CT9Interface object that provides the real services.
*
* This class is not meant for client code derivation. Indeed it is not visible to the client 
* directly.
*/
NONSHARABLE_CLASS(CT9InputLanguageInfo) : public CAknInputLanguageInfo
	{
    public:
	    /** 
	    * Non-trivial C++ descructor. 
	    */
	    IMPORT_C ~CT9InputLanguageInfo();

	    /**
	    * NewL method for this class. This can be used internal to the factory method 
	    */
	    static CT9InputLanguageInfo* NewL();

	    /**
	    * Provide a language list corresponding to the input list of SymbianOS language codes.
	    * The order of the languages in the output list is that order implemented in the 
	    * interface object, and the order of the input list of language codes is ignored.
	    *
	    * @param aInputLanguageList Append to this list; 
	    * @param aLanguageCodeList	Append only languages whose code is in this list. If Null
        *                           is passed, then no language code filtering is done
	    * @param aCapabilityFilter	Append only languages with capabilities set in this filter.
	    *
	    */ 
	    EXPORT_C virtual void AppendLanguagesL( 
		    CAknInputLanguageList* aInputLanguageList, 
		    CArrayFix<TInt>* aLanguageCodeList,
		    TAknInputLanguageCapabilities& aCapabilityFilter );

	    /**
	    * From CAknInputLanguagesInfo.
	    *
	    * Append to a list of language, language code pairs
	    */

	    IMPORT_C void AppendAvailableLanguagesL( CAknInputLanguageList* aInputLanguageArray );

	    /**
	    * From CAknInputLanguageInfo
	    *
	    * @return	the capabilies of the keyed language
	    */
	    IMPORT_C virtual TAknInputLanguageCapabilities LanguageCapabilitiesFromLanguage(TLanguage aLanguageId);

	    /**
	    * Get the language name that corresponds to the passed-in Symbian OS language code.
	    * This name should be localized, or not, depending upon the policy decided for the product
	    * 
	    * @param	aLanguageCode	- language code for the language whose name is required
	    * @return	TPtrC			- holds the name of the language.  Buffer for the name is within 
	    *							the interface object, not the client. The buffer will be valid only 
	    *							while the instance of this class exists.
	    */
	    IMPORT_C virtual TAknLanguageName LanguageName( TLanguage aLanguageCode ) const;

	    IMPORT_C TLanguage UrlLanguage() const;

    private:
	    /**
	    * Private constructor
	    */
	    CT9InputLanguageInfo();

	    /**
	    * Private 2nd stage constructor
	    */
	    void ConstructL();

	    /**
	    *
	    * Reserved virtuals
	    *
	    */
	    void Reserved_1();
	    void Reserved_2();

	    static TBool IsInLanguageCodeList( CArrayFix<TInt>* aList , TLanguage languageToFind );

    private:
	    CPtiEngine* iPtiEngineInterface;  // Owned by this class
	};

#endif

// End of File
