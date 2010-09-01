/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/
#ifndef PENINPUTHWRARABICLAYOUT_
#define PENINPUTHWRARABICLAYOUT_
// User includes
#include "peninputcommonlayout.h"

class CPeninputHwrBxAbWnd;
/**
 *  Pen input HWR layout class
 *  This class define the behaviors of HWR layout
 *  
 *  @lib peninputgenerichwr.lib
 *  @since S60 v5.0
 */
class CPeninputHwrBxAbLayout : public CPeninputCommonLayout
    {
    
public:

    /**
     * Two-phased constructor
     *
     * @since S60 v5.0
     * @param aLafEnv The LAF enviroment
     * @param aLayoutOwner The owner of layout
     * @param aInitData Init data of layout
     * @return The pointer to CPeninputHwrBxAbLayout object
     */
    static CPeninputHwrBxAbLayout* NewL( MLayoutOwner* aLayoutOwner, 
                                            const TAny* aInitData );

    /**
     * Destructor
     *
     * @since S60 v5.0
     * @return None
     */
    virtual ~CPeninputHwrBxAbLayout();

// From base class CPeninputCommonLayout

    /**
     * From CPeninputCommonLayout
     * Handle layout command event
     *
     * @since S60 v5.0
     * @param aCmd Command Id
     * @param aData Data for command
     * @return Errors when return value smaller than zero    
     */
    TInt HandleCommand( TInt aCmd, TUint8* aData );
    /**
     * From CFepUiLayout
     * Handle the event from control
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aCtrl The sender
     * @param aEventData The event data
     * @return None
     */    
    void HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, 
                                            const TDesC& aEventData);  
    /**
     * Get layout window
     *
     * @since S60 v3.2
     * @return The pointer to CPeninputLayoutWindow object
     */
    CPeninputLayoutWindow* LayoutWindow() const;

    void TranslateCharCode( TDes& aCharCode );	

    void ReplaceL(const TDesC& aOldCharCode, const TDesC& aNewCharCode);	
    void SubmitL(const TDesC& aCharCode);	
    void ToPenStandBy(TBool aNotStandBy = EFalse);	
	
protected:

    /**
     * Constructor
     *
     * @since S60 v5.0
     * @param aLayoutOwner The owner of layout
     * @return None
     */
    CPeninputHwrBxAbLayout( MLayoutOwner* aLayoutOwner );

// From base class CPeninputCommonLayout

    /**
     * From CPeninputCommonLayout
     * Factory method for creation of layout window. The decision which 
     * layout window(VKB or HWR) is created will be postboned in subclasses 
     * of this class
     *
     * @since S60 v5.0
     * @return None
     */
    void CreateLayoutWindowL();

    /**
     * From CPeninputCommonLayout
     * Factory method for creation resource config. It is enough at present 
     * to create and initialize resource config. Any necessary extension 
     * will be implemented in subclasses
     * 
     * @since S60 v5.0
     * @return None
     */
    void CreateDataMgrL( const TAny* aInitData );

    /**
     * From CPeninputCommonLayout
     * Get the layout type, VKB or HWR, child class must rewrite the function
     * 
     * @since S60 v5.0
     * @return The layout type
     */
     TInt LayoutType();
    
    /**
     * SemiTransparencyRequired. Tell whether this layout require transparency.
     * This is to save memory used in mask bitmap. If transparency not deeded,
     * We use black-white bitmap.
     *
     * @since S60 v5.0
     * @return ETrue if layout require transparency, otherwise EFalse.
     */        
    TBool SemiTransparencyRequired(); 
    
protected:

    /**
     * Second phase constructor
     *
     * @since S60 v5.0
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
 *  @since S60 v5.0
 */
class CPeninputHwrBxAbLayoutImp : public CFepUiLayoutInterface
    {
      
public:

    /**
     * Two-phased constructor
     *
     * @since S60 v5.0
     * @param aInitParams Init data for layout
     * @return The pointer to CPeninputHwrBxAbLayoutImp object
     */
    static CPeninputHwrBxAbLayoutImp* NewL( TAny* aInitParams );

    /**
     * Destructor
     *
     * @since S60 v5.0
     * @return None
     */
    virtual ~CPeninputHwrBxAbLayoutImp();

// From base class CFepUiLayoutInterface

    /**
     * From CFepUiLayoutInterface
     * Create fep ui layout
     *
     * @since S60 v5.0
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
     * @since S60 v5.0
     * @return None
     */          
    CPeninputHwrBxAbLayoutImp();

    /**
     * Second phase constructor
     *
     * @since S60 v5.0
     * @return None
     */          
    void Construct();   
    
    };

#endif /*PENINPUTHWRARABICLAYOUT_*/
