/*
* Copyright (c) 2005-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  header file of layout interface
*
*/


#ifndef C_CPENINPUTUILAYOUTINTERFACE_H
#define C_CPENINPUTUILAYOUTINTERFACE_H

#include <e32base.h>
#include <ecom.h>
#include <badesca.h>

class MPenUiLayoutBase;
class MLayoutOwner;

/** UID of HWR layout this interface */
const TInt KHwrInputMethodId = 0x102072FC;


/**
 * Class CFepUiLayoutInterface for layout interface
 *   
 * This is the layout interface for fep ui. Plugins need implement this interface.
 *
 * @lib feplayouteng.lib
 * @since S60 v4.0
 */
class CFepUiLayoutInterface : public CBase
    {
public:
    // Constructors and destructor
    /**
     * Two-phased constructor.
     *
     * @since S60 v4.0
     * @param aInputMethodId The input mode unique id. Default value is HWR id
     * @return The implementation interface
     */
    IMPORT_C static CFepUiLayoutInterface* NewL(TInt aImputMethodId = KHwrInputMethodId);

    /**
     * Destructor
     *
     * @since S60 v4.0
     */
    IMPORT_C virtual ~CFepUiLayoutInterface();
    
    /**
     * Create the ui layout. Pure virtual function. 
     *
     * @since S60 v4.0
     * @param aLayoutOwner The layout owner
     * @param aData The data for creating layout
     * @return The created ui layout
     */
    virtual MPenUiLayoutBase* CreateFepUiLayoutL(MLayoutOwner* aLayoutOwner,const TAny* aData) = 0;
    
    /**
     * Set the key for destruction
     *
     * @since S60 v4.0
     * @param aKey The uid key to be sete
     */
    IMPORT_C void SetDestructorKey(TUid aKey);

    /**
     * get the key offset for destruction UID
     *
     * @since S60 v4.0
     * @return The uid key offset in class
     */    
    static TInt32 DestructorUidKeyOffset();
    
protected:
    /**
     * Default constructor
     *
     * @since S60 v4.0
     */    
    IMPORT_C CFepUiLayoutInterface();
    
private:
    /**
     * Unique instance identifier key for interface destruction
     */
    TUid iDtor_ID_Key;    
};


#endif  //C_CPENINPUTUILAYOUTINTERFACE_H

