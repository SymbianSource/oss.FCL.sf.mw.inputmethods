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
* Description:  shift and capslock information
*
*/


#ifndef C_CPENINPUTSHIFTCAPSINFO_H
#define C_CPENINPUTSHIFTCAPSINFO_H
   
// System includes
#include <e32base.h>
#include <w32std.h>
#include <barsread.h>

/**
 *  Peninput shift and capslock info definition
 *
 *  @lib peninputcommonlayout.lib
 *  @since S60 v3.2
 */
class CPeninputShiftCapsInfo : public CBase
    {

public:

    /**
     * shift&caps single info
     */
    struct TPeninputShiftCapsSingle
        {
        TInt iId;
        TInt iVkbLayoutId;
        TInt iCaseUsed;
        };

    /**
     * Two-phase constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader    
     * @return The pointer to CPeninputShiftCapsInfo object
     */
    IMPORT_C static CPeninputShiftCapsInfo* NewL( TResourceReader& aReader );
    
    /**
     * Two-phase constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader    
     * @return The pointer to CPeninputShiftCapsInfo object
     */
    IMPORT_C static CPeninputShiftCapsInfo* NewLC( TResourceReader& aReader );

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual ~CPeninputShiftCapsInfo();  

    /**
     * Get shift caps single info by id
     *
     * @since S60 v3.2
     * @param aId Shift caps id 
     * @return The pointer to TPeninputShiftCapsSingle object
     */
    IMPORT_C TPeninputShiftCapsSingle* FindShiftCapsSingleById( TInt aId );

    /**
     * Get shift caps single info by case
     *
     * @since S60 v3.2
     * @param aCase Range id 
     * @return The pointer to TPeninputShiftCapsSingle object
     */
    IMPORT_C TPeninputShiftCapsSingle* FindShiftCapsSingleByCase( TInt aCase );
    
    /**
     * Get range id
     *
     * @since S60 v3.2
     * @return Range id
     */
    inline TInt RangeId();
    
    /**
     * Get array of shift caps single info
     *
     * @since S60 v3.2
     * @return The reference of pointer array of shift 
     *         caps single info
     */
    inline RPointerArray<TPeninputShiftCapsSingle>& ShiftCapsSingleList();

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

private: // Data

    /**
     * Range id
     */
    TInt iRangeId;

    /**
     * Array of shift caps single info
     */
    RPointerArray<TPeninputShiftCapsSingle> iShiftCapsSingleList;

    };

#include "peninputshiftcapsinfo.inl" 

#endif // C_CPENINPUTSHIFTCAPSINFO_H
