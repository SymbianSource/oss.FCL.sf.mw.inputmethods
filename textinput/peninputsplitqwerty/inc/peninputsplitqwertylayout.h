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
 * Description:  split screen qwerty layout
 *
 */

#ifndef C_PENINPUTSPLITQWERTYLAYOUT_H
#define C_PENINPUTSPLITQWERTYLAYOUT_H

// User includes
#include <AknFepGlobalEnums.h>
#include "peninputcommonlayoutext.h"


/**
 *  Split qwerty layout implementation class
 *
 *  @lib peninputsplitqwerty.lib
 *  @since since S60 v5.0
 */
class CPeninputSplitQwertyLayoutImp : public CFepUiLayoutInterface
    {
public:

    /**
     * Symbian constructor
     *
     * @since since S60 v5.0
     * @param aInitParams Init data for layout
     * @return Pointer to created object
     */
    static CPeninputSplitQwertyLayoutImp* NewL( TAny* aInitParams );

    /**
     * C++ destructor
     *
     * @since since S60 v5.0
     * @return None
     */
    virtual ~CPeninputSplitQwertyLayoutImp();

public: // From base class CFepUiLayoutInterface

    /**
     * From CFepUiLayoutInterface
     * Create fep ui layout
     *
     * @since since S60 v5.0
     * @param aLayoutOwner The layout owner
     * @param aData The data for creating layout
     * @return Pointer to created CFepUiLayout object
     */
    CFepUiLayout* CreateFepUiLayoutL( MLayoutOwner* aLayoutOwner,
                                      const TAny* aData );

private:

    /**
     * C++ constructor
     *
     * @since since S60 v5.0
     * @return None
     */
    CPeninputSplitQwertyLayoutImp();

    /**
     * Symbian constructor
     *
     * @since since S60 v5.0
     * @return None
     */
    void ConstructL();

    };

/**
 *  class CPeninputSplitQwertyLayout
 *
 *  @lib peninputsplitqwerty.lib
 *  @since since S60 v5.0
 */
class CPeninputSplitQwertyLayout : public CPeninputCommonLayoutExt
    {
public:

    /**
     * Symbian constructor
     *
     * @since since S60 v5.0
     * @param aLayoutOwner The owner of layout
     * @param aInitData Init data of layout
     * @return The pointer to created object
     */
    static CPeninputSplitQwertyLayout* NewL( MLayoutOwner* aLayoutOwner,
                                             const TAny* aInitData );

    /**
     * C++ destructor
     *
     * @since since S60 v5.0
     * @return None
     */
    virtual ~CPeninputSplitQwertyLayout();

    /**
     * Check if  ITI is enabled.
     *
     * @return ETrue if ITI is enabled.
     */
    inline TBool IsEnableITI() const;
    
public: // From base class CPeninputCommonLayout

    /**
     * From CPeninputCommonLayout
     * Handle layout command event
     *
     * @since since S60 v5.0
     * @param aCmd Command Id
     * @param aData Data for command
     * @return Errors when return value smaller than zero
     */
    TInt HandleCommand( TInt aCmd, TUint8* aData );

    /**
     * From CPeninputCommonLayout
     * Handle control event
     *
     * @since since S60 v5.0
     * @param aEventType event type
     * @param aCtrl Controler
     * @param aEventData The event data
     * @return None    
     */
    void HandleControlEvent( TInt aEventType, CFepUiBaseCtrl* aCtrl,
                             const TDesC& aEventData );

    /**
     * From CPeninputCommonLayout
     * Handle editor text coming
     * Called by animation dll when app editor text is coming
     * If a ui layout has a editor area, it must implements 
     * this function to get the text
     *
     * @since S60 v5.0
     * @param aData Editor Text Coming data
     * @return KErrNone is operation succeeded
     */
    TInt OnAppEditorTextComing( const TFepInputContextFieldData& aData );

    /**
     * From CPeninputCommonLayout
     * Inform layout the size changing event
     *
     * @since S60 v3.2
     * @param aData The event data
     * @return KErrNone if successfully size changing
     */    
    TInt SizeChanged( const TAny* aData );

public: //from base class CFepUiLayout
    /**
     * From CFepUiLayout
     * Inform UI that application infomation changed.
     *
     * @since S60 v5.0
     * @param aInfo The information.
     * @param aType The information type
     * @return none
     */
    void HandleAppInfoChange( const TDesC& aInfo, TPeninputAppInfo aType );

    /**
     * From CFepUiLayout
     * Called by owner when the layout is going to be active
     *
     * @since S60 v5.0
     * @return none
     */
    void OnActivate();

    /**
     * From CFepUiLayout
     * Tell whether this layout require transparency.
     * This is to save memory used in mask bitmap. If transparency not deeded,
     * We use black-white bitmap.
     *
     * @since since S60 v5.0
     * @return ETrue if layout require transparency, otherwise EFalse.
     */
    TBool SemiTransparencyRequired();
    
protected:

    /**
     * C++ constructor
     *
     * @since since S60 v5.0
     * @param aLayoutOwner The owner of layout
     * @return None
     */
    CPeninputSplitQwertyLayout( MLayoutOwner* aLayoutOwner );

protected: // From base class CPeninputCommonLayout
    /**
     * Symbian constructor
     *
     * @since since S60 v5.0
     * @param aInitData Init data of layout
     * @return None
     */
    void ConstructL( const TAny *aInitData );

    /**
     * From CPeninputCommonLayout
     * Factory method for creation of layout window. The decision which 
     * layout window(VKB or HWR) is created will be postboned in subclasses 
     * of this class
     *
     * @since since S60 v5.0
     * @return None
     */
    void CreateLayoutWindowL();

    /**
     * From CPeninputCommonLayout
     * Factory method for creation resource config. It is enough at present 
     * to create and initialize resource config. Any necessary extension 
     * will be implemented in subclasses
     * 
     * @since since S60 v5.0
     * @return None
     */
    void CreateDataMgrL( const TAny* aInitData );

    /**
     * From CPeninputCommonLayout
     * Get the layout type, child class must rewrite the function
     * 
     * @since since S60 v5.0
     * @return The layout type
     */
    TInt LayoutType();

private:
    /**
     * Handle virtual key event
     * 
     * @since since S60 v5.0
     * @param aEventType event type
     * @param aCtrl the control report this event
     * @param aEventData event data
     * @return none
     */
    void HandleVirtualKeyUpL( TInt aEventType, CFepUiBaseCtrl* aCtrl,
                              const TDesC& aEventData );
    
    /**
     * Handle virtual key down event
     * 
     * @since since S60 v5.0
     * @param none
     * @return none
     */
    void HandleVirtualKeyDown();

private:

    /**
     * Layout type
     */
    TInt iLayoutType;
    
    /**
     * word query flag
     */
    TBool iInEditWordQueryDlg;

    /*
     * Indicate if ITI is enabled.
     */
    TBool iITIEnabled;
    };

/**
* Check if  ITI is enabled.
*/ 
inline TBool CPeninputSplitQwertyLayout::IsEnableITI() const
    {
    return iITIEnabled;
    }

#endif // C_PENINPUTSPLITQWERTYLAYOUT_H
