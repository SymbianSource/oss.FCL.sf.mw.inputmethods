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
* Description:  range bar layout implementation
*
*/


#ifndef C_CPENINPUTRANGEBARINFO_H
#define C_CPENINPUTRANGEBARINFO_H

// System includes
#include <e32base.h>
#include <w32std.h>
#include <barsread.h>

// User includes
#include "peninputcommonlayoutglobalenum.h"

/**
 *  Peninput range bar definition
 *
 *  @lib peninputcommonlayout.lib
 *  @since S60 v3.2
 */
class CPeninputRangeInfo : public CBase
    {

public:
    /**
     * Two-phase constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader
     * @return The pointer to CPeninputRangeInfo object
     */
    IMPORT_C static CPeninputRangeInfo* NewL( TResourceReader& aReader );
    
    /**
     * Two-phase constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader
     * @return The pointer to CPeninputRangeInfo object
     */
    IMPORT_C static CPeninputRangeInfo* NewLC( TResourceReader& aReader );

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual ~CPeninputRangeInfo();  

    /**
     * Get range id
     *
     * @since S60 v3.2
     * @return Range id
     */
    inline TInt RangeId();

    
    /**
     * Get response style
     *
     * @since S60 v3.2
     * @return Response style
     */
    inline TPeninputRangeResponseStyle ResponseStyle();

    /**
     * Get client layout id
     *
     * @since S60 v3.2
     * @return Client layout id
     */
    inline TInt ClientLayoutId();

    /**
     * Get vkb layout id
     *
     * @since S60 v3.2
     * @return Vkb layout id
     */
    inline TInt VkbLayoutId();


protected:

    /**
     * Second phase constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader
     * @return None
     */
    void ConstructL( TResourceReader& aReader );

    /**
     * Construct from resource 
     *
     * @since S60 v3.2
     * @param aReader Resource reader
     * @return None
     */
    void ConstructFromResourceL( TResourceReader& aReader );

private: // data

    /**
     * Range id
     */
    TInt iRangeId;
    
    /**
     * Response style
     */
    TPeninputRangeResponseStyle iResponseStyle;

    /**
     * Client layout id
     */
    TInt iClientLayoutId;

    /**
     * Vkb layout id
     */
    TInt iVkbLayoutId;

    };

/**
 *  Peninput range bar definition
 *
 *  @lib peninputcommonlayout.lib
 *  @since S60 v3.2
 */
class CPeninputRangeBarInfo : public CBase
    {

public:

    /**
     * Two-phase constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader
     * @return The pointer to CPeninputRangeBarInfo object
     */
    IMPORT_C static CPeninputRangeBarInfo* NewL( TResourceReader& aReader );
    
    /**
     * Two-phase constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader    
     * @return The pointer to CPeninputRangeBarInfo object
     */
    IMPORT_C static CPeninputRangeBarInfo* NewLC( TResourceReader& aReader );

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual ~CPeninputRangeBarInfo();  

    /**
     * Get range style
     *
     * @since S60 v3.2
     * @return Range style
     */
    inline TInt RangeStyle();
    
    /**
     * Get range info by range id
     *
     * @since S60 v3.2
     * @param aRangeId Range id  
     * @return The pointer to CPeninputRangeInfo object
     */
    IMPORT_C CPeninputRangeInfo* FindRange( TInt aRangeId );

    /**
     * Get reference of pointer array of ranges
     *
     * @since S60 v3.2
     * @return The reference of pointer array of ranges
     */
    inline RPointerArray<CPeninputRangeInfo>& Ranges();

protected:

    /**
     * Second phase constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader
     * @return None
     */
    void ConstructL( TResourceReader& aReader );

    /**
     * Construct from resource 
     *
     * @since S60 v3.2
     * @param aReader Resource reader
     * @return None
     */
    void ConstructFromResourceL( TResourceReader& aReader );

private: // data

    /**
     * Range bar style
     */
    TInt iRangeStyle;

    /**
     * Array of ranges on rangebar
     */
    RPointerArray<CPeninputRangeInfo> iRanges;

    };

#include "peninputrangebarinfo.inl" 

#endif // C_CPENINPUTRANGEBARINFO_H
