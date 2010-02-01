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
* Description:  vkb number keymapping manager header file.
*
*/


#ifndef C_AKNFEPVKBNUMERICKEYMAPPINGMGR_H
#define C_AKNFEPVKBNUMERICKEYMAPPINGMGR_H

// INCLUDES
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
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2
 */
class CAknFepVkbNumericKepMappingMgr: public CBase
    {
    
public:

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader      
     * @return Pointer to keymapping manager
     */
    static CAknFepVkbNumericKepMappingMgr* NewL(TResourceReader aReader);

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader       
     * @return Pointer to keymapping manager
     */    
    static CAknFepVkbNumericKepMappingMgr* NewLC(TResourceReader aReader);

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepVkbNumericKepMappingMgr();          
 
    /**
     * Remove all keymapping id and string in array 
     *
     * @since S60 v3.2
     * @return None
     */
    void Reset();

    /**
     * Get Keymapping id array item count
     *
     * @since S60 v3.2
     * @return Key mapping array item count
     */  
    TInt Count() const;

    /**
     * Get number keymapping string
     *
     * @since S60 v3.2
     * @param aKeyMappingId Keymapping id
     * @return Pointer to keymapping string 
     */    
    HBufC* KeyMappingStringL(const TInt aKeyMappingId) const;
         
protected:

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader         
     * @return None
     */ 
    void ConstructL(TResourceReader aReader);      
    
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

#endif // C_AKNFEPVKBNUMERICKEYMAPPINGMGR_H

// End Of File
