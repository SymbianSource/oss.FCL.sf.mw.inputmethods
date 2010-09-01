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
* Description:  boxbase hwr layout
*
*/


#ifndef C_CAKNFEPHWRBXLAYOUT_H
#define C_CAKNFEPHWRBXLAYOUT_H

//  INCLUDES
#include <peninputlayout.h>
#include <peninputlayoutplugininterface.h>

class CAknFepHwrBxDataStore;
class CAknFepHwrBxWnd;
class CAknFepHwrBxStateBase;
class MLayoutOwner;
class MPenInputModeSwitchInterface;

/**
 *  peninputhwrboxcn layout interface implemtation
 *
 *  @lib peninputhwrboxcn.lib
 *  @since S60 v3.2
 */
class CAknFepHwrBxLayoutImp : public CFepUiLayoutInterface
    {
public:

    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aInitParams Init data for layout
     * @return Pointer to created CAknFepHwrBxLayoutImp object
     */
    static CAknFepHwrBxLayoutImp* NewL(TAny* aInitParams);
    
    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepHwrBxLayoutImp();

//from CFepUiLayoutInterface

    /**
     * From CFepUiLayoutInterface.
     * Handle layout command event
     *
     * @since S60 v3.2
     * @param aLayoutOwner The layout owner
     * @param aData The data for creating layout
     * @return Pointer to created CFepUiLayout object
     */
    MPenUiLayoutBase* CreateFepUiLayoutL(MLayoutOwner* aLayoutOwner,const TAny* aData);

private:

    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @return None
     */          
    CAknFepHwrBxLayoutImp();

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @return None
     */          
    void ConstructL();

private:// data
    
    };  

/**
 *  peninputhwrboxcn layout
 *
 *  @lib peninputhwrboxcn.lib
 *  @since S60 v3.2
 */
class CAknFepHwrBxLayout : public CFepUiLayout
    {
public:

    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aLafEnv The LAF enviroment
     * @param aInitData Init data for layout
     * @return Pointer to created CAknFepHwrBxLayout object
     */
    static CAknFepHwrBxLayout* NewL(MLayoutOwner* aLayoutOwner, const TAny* aInitData);

    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepHwrBxLayout();
       
    /**
     * Get the data store
     *
     * @since S60 v3.2
     * @return The data store
     */          
    inline CAknFepHwrBxDataStore* DataStore() const;
    
    /**
     * Get the Ui window
     *
     * @since S60 v3.2
     * @return The window
     */          
    inline CAknFepHwrBxWnd* Window() const;
    
    /**
     * Set the current state 
     *
     * @since S60 v3.2
     * @param aState New state
     * @return None
     */          
    void SetCurrentStateL(CAknFepHwrBxStateBase* aState);
    
    /**
     * Send command to open option menu 
     *
     * @since S60 v3.2
     * @return None
     */          
    void OpenOptionMenu();
    
    /**
     * Send command to open language switch menu 
     *
     * @since S60 v3.2
     * @return None
     */          
    void OpenLangSwitchMenu();

    /**
     * Translate if char code is enter or back or space
     *
     * @since S60 v3.2
     * @param aCharCode The char code
     * @return the char code after translation
     */
    void TranslateCharCode(TDes& aCharCode);
    
    /**
     * Submit the candidate.
     *
     * @since S60 v3.2
     * @param aCharCode The char code
     * @return None
     */
    void SubmitL(const TDesC& aCharCode);

    /**
     * Submit the candidate.
     *
     * @since S60 v3.2
     * @param aOldCharCode The old char code
     * @param aNewCharCode The new char code
     * @return None
     */
    void ReplaceL(const TDesC& aOldCharCode, const TDesC& aNewCharCode);

    /**
     * Delete the candidate.
     *
     * @since S60 v3.2
     * @param aChar The delete key code
     * @return None
     */
    void Delete(TInt aChar = EKeyBackspace);

    /**
     * Set Layout position
     *
     * @since S60 v3.2
     * @return None
     */
    void SetLayoutPositionL();

//from CFepUiLayout

    /**
     * From CFepUiLayout.
     * Handle layout command event
     *
     * @since S60 v3.2
     * @param aCmd Command Id.
     * @param aData Data for command.
     * @return Errors when return value small than 0.        
     */
    TInt HandleCommand(TInt aCmd, TUint8* aData);

    /**
     * From CFepUiLayout.
     * Inform layout the size changing event
     *
     * @since S60 v3.2
     * @param pData The event data.
     * @return KErrNone if successfully size changing.        
     */
    TInt SizeChanged(const TAny* pData);

    /**
     * From CFepUiLayout.
     * Handle the event from control
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aCtrl  The sender
     * @param aEventData The event data.
     * @return None                
     */
    void HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData);

    /**
     * From CFepUiLayout.
     * Test whether the position is valid. 
     * This is used to test the target position when draging controls to some place
     *
     * @since S60 v3.2
     * @param aRect The rect to be checked.
     * @param aDragbar The dragbar being dragged
     * @param aVInfo The valid information in vertial dirtection. ETrue is vertical
     *        moving is valid.It's only used when aRect is not valid but moving in 
     *        vertical direction is valid.
     * @param aHInfo The valid information in horizontal dirtection. ETrue is 
     *        horizontal moving is valid.It's only used when aRect is not 
     *        valid but moving in vertical direction is valid.
     * @param aFlag ETrue if whole layout is moving, otherwise, only components moving
     * @return ETue if aRect is a valid position. Otherwise, EFalse. 
     */                                             
    TBool IsValidDestination(const TRect& aRect,
                            CDragBar* aDragBar,
                            TBool& aVInfo,TBool& aHInfo,
                            TBool aFlag);
    
    /**
     * From CFepUiLayout.
     * Handle editor text coming
     * Called by animation dll when app editor text is coming.
     * If a ui layout has a editor area, it must implements this function to get the text
     *
     * @since S60 v3.2
     * @param aTotalTextLen The total editor text len
     * @param aTextStartPos The position of the selection in whole editor text
     * @param aCursolSel The cursor selection in the editor
     * @param aText The text sent by fep.
     * @return KErrNone is operation succeeded.
     */
    TInt OnAppEditorTextComing( const TFepInputContextFieldData& aData);

    /**
     * From CFepUiLayout.
     * Handle raw event key
     *
     * @since S60 v3.2
     * @param aData the key raw event data
     * @return ETrue if the key was handled, otherwise EFalse
     */
    TBool HandleRawKeyEventL(const TRawEvent& aKeyEvent);

    /**
     * From CFepUiLayout.
     * Handle pointer down/up event while this event is outside of the layout
     * Do nothing in default implementation
     *
     * @since S60 v3.2
     * @param aEvent The raw event
     * @return EFalse if the event needs forward on. Otherwise ETrue.
     */                
    TBool OnPointerEventOutsideLayout(const TRawEvent& aEvent);

    /**
     * From CFepUiLayout 
     * Get the layout type, vkb or hwr. It's now similar to LayoutType which defines in 
     * another interface.
     * 
     * @since S60 v3.2
     * @return The layout type
     */
    TInt PenInputType();
    
    void SetPositionFromOutside(const TPoint& aNewPos);
    
private:

    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @param aLafEnv The LAF enviroment
     * @return None
     */          
    CAknFepHwrBxLayout(MLayoutOwner* aLayoutOwner);

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @param aInitData Init data of layout
     * @return None
     */          
    void ConstructL(const TAny* aInitData);
    
    /**
     * Read layout rect from LAF and set layout self position
     *
     * @since S60 v3.2
     * @param aLandscapeStyle Current screen is landscape or not
     * @return None
     */          
    void ResetLayoutRectL();

    /**
     * Go to standby state
     *
     * @since S60 v5.0
     * @return None
     */          
    void GoToStandbyStateL( TInt aOpenFlag );
private://data

    /**
     * The data store (Own)
     */
    CAknFepHwrBxDataStore* iDataStore;
    
    /**
     * The window (Own)
     */
    CAknFepHwrBxWnd* iBxWindow;
    
    /**
     * The current state (Own)
     */
    CAknFepHwrBxStateBase* iCurrentState;
    
    /**
     * Is LandscapeStyle
     */
    //TBool iLandscapeStyle;
    
    /**
     * The hwr resource id
     */    
    TInt iResId;
    
    MPenInputModeSwitchInterface* iInputModeSwitch;

    };

#include "peninputhwrbxlayout.inl"

#endif // C_CAKNFEPHWRBXLAYOUT_H

// End Of File
