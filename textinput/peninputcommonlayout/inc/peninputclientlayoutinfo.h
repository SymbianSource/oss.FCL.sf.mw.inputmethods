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
* Description:  client layout information
*
*/


#ifndef C_PENINPUTCLIENTLAYOUTINFO_H
#define C_PENINPUTCLIENTLAYOUTINFO_H

// System includes
#include <e32base.h>
#include <w32std.h>
#include <barsread.h>

/**
 *  Control info class
 *
 *  This class is to define control info. Application can create it from  
 *  resource file. Control info includes control id, begin row, end row,  
 *  bgin column and end column. 
 *
 *  @lib peninputcommonlayout.lib
 *  @since S60 v3.2
 */
class CPeninputControlInfo : public CBase
    {
    
public:

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader    
     * @return The pointer to CPeninputControlInfo object
     */
    IMPORT_C static CPeninputControlInfo* NewL( TResourceReader& aReader );

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */    
    IMPORT_C virtual ~CPeninputControlInfo();

    /**
     * Get control id
     *
     * @since S60 v3.2
     * @return Control id
     */    
    inline const TInt ControlID() const;

    /**
     * Get control type
     *
     * @since S60 v3.2
     * @return Control type
     */    
    inline const TInt ControlType() const;

    /**
     * Get begin row
     *
     * @since S60 v3.2
     * @return Begin row
     */    
    inline const TInt BeginRow() const;

    /**
     * Get end row
     *
     * @since S60 v3.2
     * @return End row
     */    
    inline const TInt EndRow() const;

    /**
     * Get begine column
     *
     * @since S60 v3.2
     * @return Begine column
     */    
    inline const TInt BeginColumn() const;

    /**
     * Get end column
     *
     * @since S60 v3.2
     * @return End column
     */    
    inline const TInt EndColumn() const;

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
     * Control id
     */  
    TInt iControlID;

    /**
     * Control Type
     */
    TInt iControlType;

    /**
     * Begin row
     */  
    TInt iBeginRow;

    /**
     * End row
     */  
    TInt iEndRow;

    /**
     * Begin column
     */  
    TInt iBeginColumn;

    /**
     * End column
     */  
    TInt iEndColumn;
    
    };

/**
 *  Client layout class
 *
 *  This class is to implement image layout. Application can create it from  
 *  resource file. Control info includes control id, row and column. 
 *
 *  @lib peninputcommonlayout.lib
 *  @since S60 v3.2
 */
class CPeninputClientLayoutInfo : public CBase
    {
    
public:

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader        
     * @return The pointer to CPeninputClientLayoutInfo object
     */
    IMPORT_C static CPeninputClientLayoutInfo* NewL( 
        TResourceReader& aReader );

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader        
     * @return The pointer to CPeninputClientLayoutInfo object
     */
    IMPORT_C static CPeninputClientLayoutInfo* NewLC( 
        TResourceReader& aReader );

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual ~CPeninputClientLayoutInfo();
    
    /**
     * Contruct from resource file
     *
     * @since S60 v3.2
     * @param aReader Resource reader    
     * @return None
     */    
    IMPORT_C void ConstructFromResourceL( TResourceReader& aReader );

    /**
     * Get control info
     *
     * @since S60 v3.2
     * @param aControlID control id  
     * @return The pointer to CPeninputControlInfo object
     */    
    IMPORT_C CPeninputControlInfo* FindControlInfo( const TInt aControlID );

    /**
     * Add control info
     *
     * @since S60 v3.2
     * @param aControlInfo Pointer to control info
     * @return None
     */    
    IMPORT_C void AddControlInfo( CPeninputControlInfo* aControlInfo );

    /**
     * Remove control info
     *
     * @since S60 v3.2
     * @param aControlInfo Pointer to control info
     * @return None
     */  
    IMPORT_C void RemoveControlInfo( CPeninputControlInfo* aControlInfo );
 
    /**
     * Remove control info based on control id
     *
     * @since S60 v3.2
     * @param aControlID Control id
     * @return None
     */ 
    IMPORT_C void RemoveControlInfo( const TInt aControlID );

    /**
     * Get layout id
     *
     * @since S60 v3.2
     * @return Layout id
     */ 
    inline TInt LayoutID() const;

    /**
     * Get column
     *
     * @since S60 v3.2
     * @return Column
     */ 
    inline TInt Columns() const;

    /**
     * Get row
     *
     * @since S60 v3.2
     * @return Row
     */ 
    inline TInt Rows() const;

    /**
     * Get control array
     *
     * @since S60 v3.2
     * @return Control array
     */ 
    inline RPointerArray<CPeninputControlInfo>& ControlInfoList();

protected:

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader   
     * @return None
     */ 
    void ConstructL( TResourceReader& aReader );  
                
private: // Data

    /**
     * Layout id
     */  
    TInt iLayoutID;

    /**
     * Column count
     */  
    TInt iColumns;

    /**
     * Row count
     */  
    TInt iRows;

    /**
     * Control info array
     */  
    RPointerArray<CPeninputControlInfo> iControlInfoList;
    
    };

#include "peninputclientlayoutinfo.inl"
    
#endif // C_PENINPUTCLIENTLAYOUTINFO_H
