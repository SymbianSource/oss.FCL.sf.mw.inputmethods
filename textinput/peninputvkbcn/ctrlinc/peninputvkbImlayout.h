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
* Description:  control info and image layout header file
*
*/


#ifndef C_AKNFEPVKBIMLAYOUT_H
#define C_AKNFEPVKBIMLAYOUT_H

// INCLUDES
#include <e32base.h>
#include <w32std.h>
#include <barsread.h>

/**
 *  vkb control info class
 *
 *  This class is to define control info. Application can create it from resource 
 *  file. Control info includes control id, begin row, end row, bgin column and 
 *  end column. 
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2
 */
class CAknFepControlInfo : public CBase
    {
    
public:

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader    
     * @return Pointer to created CAknFepControlInfo object
     */
    static CAknFepControlInfo* NewL(TResourceReader& aReader);

    /**
     * Contruct from resource file
     *
     * @since S60 v3.2
     * @param aReader Resource reader    
     * @return None
     */    
    void ConstructFromResourceL(TResourceReader& aReader);

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */    
    virtual ~CAknFepControlInfo();

    /**
     * Get control id
     *
     * @since S60 v3.2
     * @return Control id
     */    
    inline const TInt ControlID() const;

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
    
private: //Data

    /**
     * Control id
     */  
    TInt iControlID;

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
 *  vkb image layout class
 *
 *  This class is to implement image layout. Application can create it from resource 
 *  file. Control info includes control id, row and column. 
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2
 */
class CAknFepVkbImLayout: public CBase
    {
    
public:

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader        
     * @return Pointer to created CAknFepVkbImLayout object
     */
    static CAknFepVkbImLayout* NewL(TResourceReader& aReader);

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader        
     * @return Pointer to created CAknFepVkbImLayout object
     */
    static CAknFepVkbImLayout* NewLC(TResourceReader& aReader);

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepVkbImLayout();
    
    /**
     * Contruct from resource file
     *
     * @since S60 v3.2
     * @param aReader Resource reader    
     * @return None
     */    
    void ConstructFromResourceL(TResourceReader& aReader);

    /**
     * Contruct language is available
     *
     * @since S60 v3.2
     * @param aLangID language id  
     * @return ETrue for available language; otherwise EFalse
     */    
    TBool IsAvailableForLang(const TInt aLangID);

    /**
     * Get control info
     *
     * @since S60 v3.2
     * @param aLangID language id  
     * @return Pointer to CAknFepControlInfo
     */    
    CAknFepControlInfo* FindControlInfo(const TInt aControlID);

    /**
     * Add control info
     *
     * @since S60 v3.2
     * @param aControlInfo Pointer to control info
     * @return None
     */    
    void AddControlInfo(CAknFepControlInfo* aControlInfo);

    /**
     * Remove control info
     *
     * @since S60 v3.2
     * @param aControlInfo Pointer to control info
     * @return None
     */  
    void RmoveControlInfo(CAknFepControlInfo* aControlInfo);
 
    /**
     * Remove control info based on control id
     *
     * @since S60 v3.2
     * @param aControlID Control id
     * @return None
     */ 
    void RmoveControlInfo(const TInt aControlID);

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
    inline RPointerArray<CAknFepControlInfo>& ControlInfoList();

protected:

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @param aReader Resource reader   
     * @return None
     */ 
    void ConstructL(TResourceReader& aReader);  
                
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
     * Permitted langauge (Own)
     */  
    HBufC* iPermittedLanguage;

    /**
     * Control info array
     */  
    RPointerArray<CAknFepControlInfo> iControlInfoList;
    
    };

#include "peninputvkbimlayout.inl"
    
#endif // C_AKNFEPVKBIMLAYOUT_H

// End Of File
