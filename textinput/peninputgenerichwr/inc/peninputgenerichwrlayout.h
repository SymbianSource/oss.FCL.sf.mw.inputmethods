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
* Description:  generic HWR layout
*
*/


#ifndef C_PENINPUTGENERICHWRLAYOUT_H
#define C_PENINPUTGENERICHWRLAYOUT_H

// User includes
#include "peninputcommonlayout.h"

/**
 *  Pen input HWR layout class
 *  This class define the behaviors of HWR layout
 *  
 *  @lib peninputgenerichwr.lib
 *  @since S60 v3.2
 */
class CPeninputGenericHwrLayout : public CPeninputCommonLayout
    {
    
public:

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aLafEnv The LAF enviroment
     * @param aLayoutOwner The owner of layout
     * @param aInitData Init data of layout
     * @return The pointer to CPeninputGenericHwrLayout object
     */
    static CPeninputGenericHwrLayout* NewL( MLayoutOwner* aLayoutOwner, 
                                            const TAny* aInitData );

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CPeninputGenericHwrLayout();

// From base class CPeninputCommonLayout

    /**
     * From CPeninputCommonLayout
     * Handle layout command event
     *
     * @since S60 v3.2
     * @param aCmd Command Id
     * @param aData Data for command
     * @return Errors when return value smaller than zero    
     */
    TInt HandleCommand( TInt aCmd, TUint8* aData );

protected:

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @param aLafEnv The LAF enviroment
     * @param aLayoutOwner The owner of layout
     * @return None
     */
    CPeninputGenericHwrLayout( MLayoutOwner* aLayoutOwner );

// From base class CPeninputCommonLayout

    /**
     * From CPeninputCommonLayout
     * Factory method for creation of layout window. The decision which 
     * layout window(VKB or HWR) is created will be postboned in subclasses 
     * of this class
     *
     * @since S60 v3.2
     * @return None
     */
    void CreateLayoutWindowL();

    /**
     * From CPeninputCommonLayout
     * Factory method for creation resource config. It is enough at present 
     * to create and initialize resource config. Any necessary extension 
     * will be implemented in subclasses
     * 
     * @since S60 v3.2
     * @return None
     */
    void CreateDataMgrL( const TAny* aInitData );

    /**
     * From CPeninputCommonLayout
     * Get the layout type, VKB or HWR, child class must rewrite the function
     * 
     * @since S60 v3.2
     * @return The layout type
     */
     TInt LayoutType();
    
    /**
     * SemiTransparencyRequired. Tell whether this layout require transparency.
     * This is to save memory used in mask bitmap. If transparency not deeded,
     * We use black-white bitmap.
     *
     * @since S60 v3.2
     * @return ETrue if layout require transparency, otherwise EFalse.
     */        
    TBool SemiTransparencyRequired(); 
    
protected:

    /**
     * Second phase constructor
     *
     * @since S60 v3.2
     * @param aInitData Init data of layout
     * @return None
     */
     void ConstructL( const TAny* aInitData );
     
    };


/**
 *  Pen input HWR layout implementation class
 *  This class define the implementation interface of HWR layout
 * 
 *  @lib peninputgenerichwr.lib
 *  @since S60 v3.2
 */
class CPeninputGenericHwrLayoutImp : public CFepUiLayoutInterface
    {
      
public:

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aInitParams Init data for layout
     * @return The pointer to CPeninputGenericHwrLayoutImp object
     */
    static CPeninputGenericHwrLayoutImp* NewL( TAny* aInitParams );

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CPeninputGenericHwrLayoutImp();

// From base class CFepUiLayoutInterface

    /**
     * From CFepUiLayoutInterface
     * Create fep ui layout
     *
     * @since S60 v3.2
     * @param aLayoutOwner The layout owner
     * @param aData The data for creating layout
     * @return The pointer to CFepUiLayout object
     */
    CFepUiLayout* CreateFepUiLayoutL( MLayoutOwner* aLayoutOwner,
                                      const TAny* aData );

private:

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @return None
     */          
    CPeninputGenericHwrLayoutImp();

    /**
     * Second phase constructor
     *
     * @since S60 v3.2
     * @return None
     */          
    void Construct();   
    
    };

#endif // C_PENINPUTGENERICHWRLAYOUT_H
