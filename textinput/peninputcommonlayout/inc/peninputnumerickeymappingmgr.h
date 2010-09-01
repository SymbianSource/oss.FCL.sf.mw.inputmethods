/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  vkb number keymapping manager header file
*
*/


#ifndef C_CPENINPUTNUMERICKEYMAPPINGMGR_H
#define C_CPENINPUTNUMERICKEYMAPPINGMGR_H

// System includes
#include <e32base.h>
#include <barsread.h>

/**
 *  vkb number keymapping manager class
 *
 *  This class is to manage number keymapping. When this class contructs, keymapping 
 *  manager loads all mapping number key string from resource file. User can get 
 *  mapping key string according to keymapping id and delete all internal value when 
 *  destructing or calling reset.
 *
 *  @lib peninputcommonlayout.lib
 *  @since S60 v3.2
 */
class CPeninputNumericKepMappingMgr : public CBase
    {
    
public:

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader      
     * @return The pointer to CPeninputNumericKepMappingMgr object
     */
    IMPORT_C static CPeninputNumericKepMappingMgr* NewL( 
        TResourceReader aReader );

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader       
     * @return The pointer to CPeninputNumericKepMappingMgr object
     */    
    IMPORT_C static CPeninputNumericKepMappingMgr* NewLC( 
        TResourceReader aReader );

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual ~CPeninputNumericKepMappingMgr();
 
    /**
     * Remove all keymapping ids and strings in array 
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void Reset();

    /**
     * Get count of items in keymapping id array 
     *
     * @since S60 v3.2
     * @return The count of items in keymapping id array 
     */  
    IMPORT_C TInt Count() const;

    /**
     * Get keymapping string by keymapping id
     *
     * @since S60 v3.2
     * @param aKeyMappingId Keymapping id
     * @return The pointer to HBufC data containing keymapping string 
     */    
    IMPORT_C HBufC* KeyMappingStringL( const TInt aKeyMappingId ) const;
    
    /**
     * Get Arabic keymapping string by keymapping id
     *
     * @since S60 v3.2
     * @param aKeyMappingId Keymapping id
     * @return The pointer to HBufC data containing Arabic keymapping string 
     */    
    IMPORT_C HBufC* KeyMappingStringL( const TInt aKeyMappingID, 
                                       const TInt aLang ) const;
         
protected:

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader         
     * @return None
     */ 
    void ConstructL( TResourceReader aReader );
    
private: // Data

    /**
     * Keymapping id array
     */  
    RArray<TInt> iKeyMappingIdList;
    
    /**
     * Keymapping string array
     */          
    RPointerArray<HBufC> iKeyMappingStringList;
    
    };

#endif // C_CPENINPUTNUMERICKEYMAPPINGMGR_H
