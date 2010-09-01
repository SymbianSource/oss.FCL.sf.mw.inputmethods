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
* Description:  interface of data provider
*
*/


#ifndef M_MPENINPUTDATAPROVIDER_H
#define M_MPENINPUTDATAPROVIDER_H

// System includes
#include <e32base.h>

/**
 * Pen input data type
*/
enum TPeninputDataType
    {
    EPeninputDataTypeCase                 = 0x00000001,
    EPeninputDataTypeInputLanguage        = 0x00000002,
    EPeninputDataTypePermittedRange       = 0x00000004,
    EPeninputDataTypeCurrentRange         = 0x00000008,
    EPeninputDataTypeClientLayout         = 0x00000010,
    EPeninputDataTypeVkbLayout            = 0x00000020,
    EPeninputDataTypeNumericKeymap        = 0x00000040,
    EPeninputDataTypeShiftDown            = 0x00000080,
    EPeninputDataTypeCapslockDown         = 0x00000100,
    EPeninputDataTypeCurrentAccent        = 0x00000200,
    EPeninputDataTypeScreenMode           = 0x00000400,
    EPeninputDataTypeScreenSize           = 0x00000800,
    EPeninputDataTypeWindowPosition       = 0x00001000,
    EPeninputDataTypePositionSettingId    = 0x00002000,
    EPeninputDataTypeUnitWidth            = 0x00004000,
    EPeninputDataTypeUnitHeight           = 0x00008000,
    EPeninputDataTypeDataQueryDlgRect     = 0x00010000,
    EPeninputDataTypePenTailColor 		  = 0x00020000,
    EPeninputDataTypeReserve_1            = 0x00040000,
    EPeninputDataTypePermittedCase        = 0x00080000,
    EAknFepDataTypeUseDefinedResource     = 0x00100000,
    EAkninputDataTypeGuideLineTop         = 0x00200000,
    EAkninputDataTypeGuideLineBottom      = 0x00400000,
    EAkninputDataTypeSizeChanging         = 0x00800000,
    EAkninputDataTypeUnitSizeSet          = 0x01000000,
    EAkninputDataTypeLatchedSet       = 0x02000000,
    EAkninputDataTypeInputArea            = 0x04000000,
    EAkninputDataTypeAllowCaseChange      = 0x08000000
    };

/**
 *  Data provider interface
 *
 *  @lib peninputcommonlayout.lib
 *  @since S60 v3.2
 */
class MPeninputDataProvider
    {

public:

    /**
     * Request data from data provider
     *
     * @since S60 v3.2
     * @param aDataType The data request 
     * @return The data you request
     */
    virtual TAny* RequestData( TPeninputDataType aDataType ) = 0;

    /**
     * Set data according to the data type
     *
     * @since S60 v3.2
     * @param aDataType The data type need to set
     * @param aData The new data
     * @return None
     */
    virtual void SetData( TPeninputDataType aDataType, 
                                   TAny* aData ) = 0;

    /**
     * Reset some data in data manager
     *
     * @since S60 v3.2
     * @return None
     */
    virtual void Reset() = 0;

    /**
     * Save key into CenRep(AknFep)
     *
     * @since S60 v3.2
     * @param aKey The key name
     * @param aValue The new value
     * @return None
     */
    virtual void SaveKey( TInt aKey, TInt aValue ) = 0;

    };

#endif  //M_MPENINPUTDATAPROVIDER_H
