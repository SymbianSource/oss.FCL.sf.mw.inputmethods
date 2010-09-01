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
* Description:  vkb control pool header file.
*
*/


#ifndef C_AKNFEPVKBCTRLPOOL_H
#define C_AKNFEPVKBCTRLPOOL_H

// INCLUDES
#include <e32base.h>
#include <w32std.h>

// FORWARD DECLARATION
class CFepUiBaseCtrl;

/**
 *  vkb control pool class
 *
 *  This class is control pool. Application can use this class to 
 *  manage their controls. Control creates out side and add itself to this 
 *  pool. Application can get and remove one control based on control
 *  id. 
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2
 */
class CAknFepVkbCtrlPool: public CBase
    {
    
public:

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @return Pointer to created CAknFepVkbCtrlPool object
     */
    static CAknFepVkbCtrlPool* NewL();

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @return Pointer to created CAknFepVkbCtrlPool object
     */    
    static CAknFepVkbCtrlPool* NewLC();

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepVkbCtrlPool();        
 
    /**
     * Add one control to the pooling, the control should have 
     * one control id
     *
     * @since S60 v3.2
     * @param aControl Pointer to control
     * @return None
     */
    void AddControl(CFepUiBaseCtrl* aControl); 
               
    /**
     * Remove one control from the pooling 
     *
     * @since S60 v3.2
     * @param aControlID Control id
     * @return None
     */    
    void RemoveControl(const TInt aControlID); 

    /**
     * Get one control from the pooling according to the control id
     *
     * @since S60 v3.2
     * @param aControlID Control id
     * @return Pointer to control 
     */   
    CFepUiBaseCtrl* Control(const TInt aControlID) const;    
 
    /**
     * Remove all controls and delete them
     *
     * @since S60 v3.2
     * @return None
     */    
    void Reset();

    /**
     * Get current control count in this pool
     *
     * @since S60 v3.2
     * @return None
     */  
    TInt ControlCount() const;

    /**
     * Get one control from the pooling according to the index
     *
     * @since S60 v3.2
     * @param aIndex Index in array
     * @return Pointer to control 
     */    
    CFepUiBaseCtrl* ControlByIndex(const TInt aIndex) const;
         
protected:

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @return None
     */ 
    void ConstructL();  
    
private: // Data

    /**
     * Control pointer array
     */  
    RPointerArray<CFepUiBaseCtrl> iControlList;     
    
    };

#endif // C_AKNFEPVKBCTRLPOOL_H

// End Of File
