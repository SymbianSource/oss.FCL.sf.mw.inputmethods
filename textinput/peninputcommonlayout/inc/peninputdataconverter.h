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
* Description:  data converter utility
*
*/


#ifndef C_CPENINPUTDATACONVERTER_H
#define C_CPENINPUTDATACONVERTER_H

// System includes
#include <e32cmn.h>
#include <e32des16.h>
#include <e32base.h>
#include <e32def.h>

/**
 *  Peninput data converter utility
 *
 *  @lib peninputcommonlayout.lib
 *  @since S60 v3.2
 */
class CPeninputDataConverter : public CBase
    {
    
public:

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
     IMPORT_C virtual ~CPeninputDataConverter();
     
     /**
      * From integer to descriptor
      *
      * @since S60 v3.2
      * @param aNum Integer number to be converted
      * @param aBuf Buffer to store converted number
      * @return None
      */
     IMPORT_C static void IntToDesc( TUint aNum, 
                                     TDes16& aBuf );

     /**
      * From combination to descriptor
      *
      * @since S60 v3.2
      * @param aBool Bool value to be converted
      * @param aNum Number to append to a Bool value
      * @param aBuf Buffer to store converted number
      * @return None
      */
     IMPORT_C static void CombinationToDesc( TBool aBool, 
                                             TUint aNum, 
                                             TDes16& aBuf );

     /**
      * From TAny* to TInt
      *
      * @since S60 v3.2
      * @param aOrginal Orginal data
      * @return Converted descriptor
      */
     IMPORT_C static TInt AnyToInt( TAny* aOrginal );

     /**
      * Get shift and caps status by case
      *
      * @since S60 v3.2
      * @param aCase The case
      * @param aShiftStatus The shift status
      * @param aCapsStatus The capslock status
      * @return None
      */
     IMPORT_C static void ShiftCapslockByCase( TInt aCase, 
                                               TInt& aShiftStatus, 
                                               TInt& aCapsStatus );

     /**
      * Get fep case by case id internal
      *
      * @since S60 v3.2
      * @param aCaseId The internal case
      * @return The fep case
      */
     IMPORT_C static TInt FepCaseByCaseId( TInt aCaseId );
     
protected:

    /**
     * 1st phase constructor
     *
     * @since S60 v3.2
     * @return None
     */
     IMPORT_C CPeninputDataConverter();

    };

#endif // C_CPENINPUTDATACONVERTER_H
