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
* Description:  controls management utility
*
*/


#ifndef C_CPENINPUTCTRLPOOL_H
#define C_CPENINPUTCTRLPOOL_H

// System includes
#include <e32base.h>
#include <w32std.h>

// Forward declaration
class CFepUiBaseCtrl;

/**
 *  Peninput control pool class
 *
 *  This class is control pool. Application can use this class to 
 *  manage their controls. Control creates out side and add itself to this 
 *  pool. Application can get and remove one control based on control
 *  id. 
 *
 *  @lib peninputcommonlayout.lib
 *  @since S60 v3.2
 */
class CPeninputCtrlPool : public CBase
    {
    
public:

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @return Pointer to created CPeninputCtrlPool object
     */
    IMPORT_C static CPeninputCtrlPool* NewL();

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @return Pointer to created CPeninputCtrlPool object
     */    
    IMPORT_C static CPeninputCtrlPool* NewLC();

    /**
     * Destructor, it need to destroy all controls
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual ~CPeninputCtrlPool();
 
    /**
     * Add one control to the pooling, the control should have 
     * one control id
     *
     * @since S60 v3.2
     * @param aControl Pointer to control
     * @return None
     */
    IMPORT_C void AddControl( CFepUiBaseCtrl* aControl ); 
               
    /**
     * Remove one control from the pooling 
     *
     * @since S60 v3.2
     * @param aControlID Control id
     * @return None
     */    
    IMPORT_C void RemoveControl( const TInt aControlID ); 

    /**
     * Get one control from the pooling according to the control id
     *
     * @since S60 v3.2
     * @param aControlID Control id
     * @return Pointer to control 
     */   
    IMPORT_C CFepUiBaseCtrl* Control( const TInt aControlID ) const;
 
    /**
     * Remove all controls and delete them
     *
     * @since S60 v3.2
     * @return None
     */    
    IMPORT_C void Reset();

    /**
     * Get current control count in this pool
     *
     * @since S60 v3.2
     * @return The count of controls
     */  
    IMPORT_C TInt ControlCount() const;

    /**
     * Get the control from the pool according to the index
     *
     * @since S60 v3.2
     * @param aIndex Index in array
     * @return Pointer to control 
     */    
    IMPORT_C CFepUiBaseCtrl* ControlByIndex( const TInt aIndex ) const;

protected:

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @return None
     */ 
    IMPORT_C void Construct();  
    
private: // Data

    /**
     * Control pointer array
     */  
    RPointerArray<CFepUiBaseCtrl> iControlList;

    };

#endif // C_CPENINPUTCTRLPOOL_H
