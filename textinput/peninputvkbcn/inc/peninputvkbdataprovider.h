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
* Description:  Interface of vkb ui state
*
*/


#ifndef M_AKNFEPVKBDATAPROVIDER_H
#define M_AKNFEPVKBDATAPROVIDER_H

enum TAknFepDataType
    {
    EAknFepDataTypeCase                 = 0x0001,
    EAknFepDataTypeInputLanguage        = 0x0002,
    EAknFepDataTypePermittedRange       = 0x0004,
    EAknFepDataTypeCurrentRange         = 0x0008,
    EAknFepDataTypeIMLayout             = 0x0010,
    EAknFepDataTypeNumericKeymap        = 0x0020,                       
    EAknFepDataTypeShfitDown            = 0x0040,
    EAknFepDataTypeCapslockDown         = 0x0080,
    EAknFepDataTypeCurrentAccent        = 0x0100,    
    EAknFepDataTypeCandidateCount       = 0x0200,    
    EAknFepDataTypeLengthWhenButtondownOnBack = 0x0400,
    EAknFepDataTypeWindowPosition       = 0x0800,
    EAknFepDataTypeUnitWidth        = 0x1000,
    EAknFepDataTypeUnitHeight        = 0x2000,
    EAknFepDataTypeScreenSize        = 0x4000,
    //EAknFepDataTypePoistionJustifyStyle = 0x8000,
    EAknFepDataTypeDataQueryRect        = 0x10000,
    EAknFepDataTypeUseDefinedResource   = 0x20000,
    EAknFepDataTypeShadowTlSize = 0x40000,
    EAknFepDataTypeShadowBrSize = 0x80000,
    EAknFepDataTypeMainPaneRect = 0x100000,
    EAknFepDataTypeTitleBarHeight = 0x200000,
    EAknFepDataTypeIcfColumnNums  = 0x400000,
    EAknFepDataTypeDefaultIMLayout = 0x800000,
    EAknFepDataTypeNeedBRJustify = 0x8000
    };

// class declarations
/**
 *  Data manager class
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2
 */
class MAknFepVkbDataProvider
    {
public:
    /**
     * Request data from data provider
     *
     * @since S60 v3.2
     * @param aDataType The data request 
     * @return The data you request
     */
    virtual TInt RequestData(TAknFepDataType aDataType) = 0;
    
    /**
     * Set data according to the data type
     *
     * @since S60 v3.2
     * @param aDataType The data type need to set
     * @param aData The new data
     * @return candidates list.
     */    
    virtual void SetData(TAknFepDataType aDataType,TInt aData) = 0;
    
    /**
     * Read unit and shadow size
     *
     * @since S60 v3.2
     * @return None
     */    
    virtual void ReadUnitShadowSize() = 0;
    
    /**
     * Whether size is changing
     *
     * @since S60 v3.2
     * @return ETrue if size is changing, otherwise EFalse
     */    
    virtual TBool SizeChanging() = 0;
    
    /**
     * Get info of whether unit and shadow size have been read
     *
     * @since S60 v3.2
     * @return Flag to indicate whether unit and shadow size
     * have been read
     */      
    virtual TBool UnitShadowSizeSet() = 0;
    };

#endif  //M_AKNFEPVKBDATAPROVIDER_H

//End Of File