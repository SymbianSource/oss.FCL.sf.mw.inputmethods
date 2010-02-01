/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:               Language database class declaration.
*
*/














#ifndef _PTI_KEYBOARD_DATABASE_H
#define _PTI_KEYBOARD_DATABASE_H

// INCLUDES
#include <badesca.h> 
#include "PtiDefs.h"

const TInt KKdbOpaqueDataLen = 8;
		
//
// TPtiLangDatabaseMappingOpaque
//
class TPtiKeyboardDatabaseMappingOpaque
	{
	public:
		TInt iUid;	
		TBuf8<KKdbOpaqueDataLen> iOpaque;
	};	
	
/**
* An interface class for wrapping keyboard data. This is used for separating
* keyboard data from language data for some predictive input implementations.
*/	
class MPtiKeyboardDatabase
	{
	public:
								
		/**
		* Returns pointer to language data. The meaning of the data in array depends
		* on the core object implementation.
		*
		* @since S60 V5.0
		* @param aIndex Index of language implementation.		
		* @param aNativeParams Core object related extra parameters.
		* @return Pointer to language data array.
		*/
		virtual TInt8* KdbData(TInt aNativeKdbId, TAny* aNativeParams) const = 0;
		
		/**
		* Returns pointer to language data. The meaning of the data in array depends
		* on the core object implementation.
		*
		* @since S60 V5.0			
		* @return Language code for kdb data implemented in this keyboard database.
		*/
        virtual TInt LanguageCode() const = 0;			
        
   		/**
		* Returns pointer to scan code binding table. The array contains Symbian OS 
		* scan code calues and the order of array items specifies bindings
		* between S60 scan codes and kdb key entries.
		*
		* @since S60 V5.0
		* @param aKeyboardType Keyboard type for binding table.			
		* @param aNumEntries   Number of binding array entries will be stored here.
		* @return Pointer to scan code binding table.
		*/
        virtual TUint8* ScanBindingTable(TPtiKeyboardType aKeyboardType,
                                         TInt& aNumEntries) const = 0;	
                                         
	    /**
		* Returns engine specific native language id code.
		*
		* @since S60 V5.0			
		* @return Engine vendor specified language id.
		*/
        virtual TInt NativeId() const = 0;		                                         		

        // Reserved vtable entries.		
		virtual void Reserved_1() = 0;
		virtual void Reserved_2() = 0;						
	};


/**
* Default implementation for MPtiLanguageDatabase interface.
*/
class CPtiKeyboardDatabase : public CBase, public MPtiKeyboardDatabase
	{
	public: 				
		// Empty implementations for MPtiKeyboardDatabase			
		IMPORT_C TInt8* KdbData(TInt aNativeKdbId, TAny* aNativeParams) const;
		IMPORT_C TInt LanguageCode() const;
        IMPORT_C TInt NativeId() const;			
		
		IMPORT_C void Reserved_1();
		IMPORT_C void Reserved_2();
																	
	private:
		TInt iDtorKeyId;
		TInt iImplementationUid;
		TInt Reserved;
	};


/**
* Keybaord database factory class.
*/
class CPtiKeyboardDatabaseFactory : public CBase
    {
    public:     
    	/**
		* Fills given array with available keyboard data implemenations and their
		* opaque data fields.
		*
		* @since S60 V5.0
		* @param aInterfaceUid An interface uid for kdb plugins.
		* @param aResult Resulting array.
		*/
		IMPORT_C static TInt CreateMappingTableWithOpaqueL(TInt aInterfaceUid,
                                                           RArray<TPtiKeyboardDatabaseMappingOpaque>& aResult);					
		              
    	/**
		* Creates a keyboard database factory instance for given implementation uid.
		*
		* @since S60 V5.0
		* @param aImplUid An implemenation uid for keyboard database factory to be created.
		* @return Pointer to key map data factory interface.
		*         NULL if not found.
		*/			
		IMPORT_C static CPtiKeyboardDatabaseFactory* CreateImplementationL(const TUid aImplUid);
		
		/**
		* Fills given list with implementation uids of all found key map data
		* factory implementations.
		*
		* @since S60 V5.0
		* @param aResult An array to be filled with uids.
		*/			
		IMPORT_C static void ListImplementationsL(TInt aCoreUid, RArray<TInt>& aResult);
        	
        /**
        * Returns keymap data object for given language.
        *
        * @since S60 V5.0
        * @param aLanguageCode Languace code for requested data.
        * @return Pointer to keymap data obejct.
        *         NULL if no data available for given language.
        */        	
        virtual MPtiKeyboardDatabase* KeyMapDataForLanguageL(TInt aLanguageCode) = 0;
        
        /**
        * Lists all languages supported by this data factory.
        * 
        * @since S60 V5.0
        * @param aResult List instance for storing results.
        */
        virtual void ListLanguagesL(RArray<TInt>& aResult) = 0;
        
        /**
        * Resturns a bit vector indicating which keyboard types are supported
        * by given language.
        *
        * @since S60 V5.0
        * @param aLanguage Requested language
        * @return A combination of flags indicating which keyboard types are supported.
        */        
        virtual TUint32 SupportedKeyboardTypes(TInt aLanguage) = 0;
        
    	IMPORT_C ~CPtiKeyboardDatabaseFactory();

        IMPORT_C virtual void Reserved_1();	    
	    IMPORT_C virtual void Reserved_2();
	    
    public:	    
	    inline TInt ImplementationUid() const;	 
	    
	private:	  		
		inline void SetDestructorKeyId(TInt aUid);
		inline void SetImplementationUid(TInt aUid);
    
    private:
    	TInt iDTorId;
    	TInt iImplUid;			        
        TInt iReserved;	
    };


// ---------------------------------------------------------------------------
// CPtiKeybaordDatabaseFactory::SetDestructorKeyId
// 
// ---------------------------------------------------------------------------
// 	
inline void CPtiKeyboardDatabaseFactory::SetDestructorKeyId(TInt aDtorKeyId)
	{
	iDTorId = aDtorKeyId;
	}

// ---------------------------------------------------------------------------
// CPtiKeybaordDatabaseFactory::SetImplementationUid
// 
// ---------------------------------------------------------------------------
// 	
inline void CPtiKeyboardDatabaseFactory::SetImplementationUid(TInt aImplUid)
    {
    iImplUid = aImplUid;
    }
    
// ---------------------------------------------------------------------------
// CPtiKeybaordDatabaseFactory::ImplementationUid
// 
// ---------------------------------------------------------------------------
// 	
inline TInt CPtiKeyboardDatabaseFactory::ImplementationUid() const
    {
    return iImplUid;
    }    

#endif // _PTI_KEYBOARD_DATABASE_H

// End of file


