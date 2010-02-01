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
* Description:  vkb layout
*
*/


#ifndef C_CAKNFEPVKBLAYOUT_H
#define C_CAKNFEPVKBLAYOUT_H

//  INCLUDES
#include <aknfepglobalenums.h>
#include <peninputlayout.h>
#include <peninputlayoutplugininterface.h>
#include "peninputvkblayoutcontext.h"
#include "peninputcmdparam.h"

/** class forward decalaration */
class CAknFepVkbDataMgr;
class CAknFepVkbUiStateMgr;
class CAknFepVkbWindow;
class CPtiEngine;

/**
 *  peninputvkbcn layout interface implemtation
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2
 */
class CAknFepVkbLayoutImp : public CFepUiLayoutInterface
    {
public:

    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aInitParams Init data for layout
     * @return Pointer to created CAknFepHwrBxLayoutImp object
     */
    static CAknFepVkbLayoutImp* NewL(TAny* aInitParams);


    static CAknFepVkbLayoutImp* NewFsqL( TAny* aInitParams );

    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */    
    virtual ~CAknFepVkbLayoutImp();

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
    CFepUiLayout* CreateFepUiLayoutL(MLayoutOwner* aLayoutOwner,const TAny* aData);

private:

    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @return None
     */          
    CAknFepVkbLayoutImp();

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @param aIsFsq Is Full screen qwert mode
     * @return None
     */          
    void ConstructL( TBool aIsFsq );

private:// data
    TBool iIsFsq; 
    };

/**
 *  peninputvkbcn layout
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2
 */
class CAknFepVkbLayout : public CFepUiLayout, 
                         public MAknFepVkbLayoutContext
    {
    
public:

    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aInitData Init data for layout
     * @return Pointer to created CAknFepHwrBxLayout object
     */
    static CAknFepVkbLayout* NewL(MLayoutOwner* aLayoutOwner, const TAny* aInitData, TBool aIsFsq);

    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepVkbLayout();

    /**
     * Get data manager.
     *
     * @since S60 v3.2
     * @return  The data manager
     */
    CAknFepVkbDataMgr* DataMgr() const;

    /**
     * Get vkb window.
     *
     * @since S60 v3.2
     * @return The vkb window
     */
    CAknFepVkbWindow* Window() const;
    
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
     * From CFepUiLayout 
     * Get the layout type, vkb or hwr. It's now similar to LayoutType which defines in 
     * another interface.
     * 
     * @since S60 v3.2
     * @return The layout type
     */
    TInt PenInputType();
    
    void SetPositionFromOutside(const TPoint& aNewPos);
    
    void HandleAppInfoChange(const TDesC& aInfo, 
                             TPeninputAppInfo aType);

//from MAknFepVkbLayoutContext


    /**
     * Send event to vkb window
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aCtrl  The sender
     * @param aEventData The event data.
     * @return None                
     */
    void SendEventToVkbControl(TInt aEventType, CFepUiBaseCtrl* aCtrl, 
                               const TDesC& aEventData = KNullDesC);

    /**
     * Request data from data provider
     *
     * @since S60 v3.2
     * @param aDataType The data request 
     * @return The data you request
     */
    TInt RequestData(TAknFepDataType aDataType);

    /**
     * Get resource utils
     *
     * @since S60 v3.2
     * @param aEventType Event type
     * @param aData Event data
     * @return The resource utils
     */
    void SetData(TAknFepDataType aDataType,TInt aData);

    /**
     * Sendkey to fep
     *
     * @since S60 v3.2
     * @param aEventType Event type
     * @param aEventData Event data
     * @return None.
     */
    void Sendkey(TInt aEventType,const TDesC& aEventData = KNullDesC);

    /**
     * Force edit gain focus
     *
     * @since S60 v3.2
     * @param aWhichEdit The control id which will get focus
     * @return None.
     */
    void ForceEditGainFocus(TInt aWhichEdit);

    /**
     * Reset all status in layout
     *
     * @since S60 v3.2
     * @return None.
     */    
    void Reset();

    /**
     * Get the length in composition field
     *
     * @since S60 v3.2
     * @return The string length in composition field
     */    
    TInt CompositionFieldStringLength();

    /**
     * Get candidates
     *
     * @since S60 v3.2
     * @param aInputString the input string sent to engine
     * @param aIsPredictive The flag showing predictive or not
     * @param aForNextPage Is this for next page candidate
     * @return The canidates count
     */
    TInt GetCandidatesL(const TDesC& aInputString, 
                        const TBool aIsPredictive,
                        TBool aForNextPage = EFalse);

    /**
     * Clear trigger string
     *
     * @since S60 v3.2
     * @return None
     */
    void ClearTriggerString();

    /**
     * Set current range
     *
     * @since S60 v3.2
     * @param aRange The range need to set
     * @param aNeedSave The flag need to save or not
     * @return None.
     */    
    void SetCurrentRange(TInt aRange, TBool aNeedSave = EFalse, TBool aNotify = EFalse);
    
    /**
     * SemiTransparencyRequired. Tell whether this layout require transparency.
     * This is to save memory used in mask bitmap. If transparency not deeded,
     * We use black-white bitmap.
     *
     * @since S60 v3.2
     * @return ETrue if layout require transparency, otherwise EFalse.
     */
    TBool SemiTransparencyRequired();
private:

    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @return None
     */ 
    CAknFepVkbLayout(MLayoutOwner* aLayoutOwner);

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @param aInitData Init data of layout
     * @return None
     */   
    void ConstructL(const TAny* aInitData, TBool aIsFsq);

    /**
     * Change input language
     *
     * @since S60 v3.2
     * @param aLangID The language id you want to change to
     * @return None
     */   
    void ChangeInputLanguage(TInt aLangID);

    /**
     * do when case change
     *
     * @since S60 v3.2
     * @return None.
     */
	void DoCaseChange(TInt aCase);

    /**
     * Handle focus gained event
     *
     * @since S60 v3.2
     * @param aCtrl  The sender
     * @param aEventData The event data.  
     * @return None
     */       
    void HandleFocusGained(CFepUiBaseCtrl* aCtrl, const TDesC& aEventData);

    /**
     * From CFepUiLayout.
     * Test whether the position is valid. 
     * This is used to test the target position when draging controls to some place
     *
     * @since S60 v4.0
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

    void HandleMergedBtnClicked( TInt aCase );
    
    /**
     * Handle show tooltip command
     *
     * @since S60 v5.0
     * @param aData pointer is sent from FEP, point to a TPtrC
     * @return None
     */    
    void HandleShowTooltipOnFSQCmdL( TUint8* aData );
    
    /**
     * Handle show candidate list command
     *
     * @since S60 v5.0
     * @param aData pointer is sent from FEP, point to a TPtrC
     * @return None
     */    
    void HandleShowCandidateListOnFSQCmdL( TUint8* aData );

    /**
     * Read text stored in a block of memory into HBufC
     *
     * @since S60 v5.0
     * @param aData aStartPtr the starting address of a block of memory with a 
     *        string in it
     * @param aLength The length of the string
     * @return pointer of a HBufC storing the string in it
     */ 
    HBufC* ReadTextInfoHBufCL( const TUint16* aStartPtr, TInt aLength );    
private: //data

    TInt iLayoutType;  
    /**
     * The data manager
     * Own
     */
    CAknFepVkbDataMgr* iDataMgr;
    
    /**
     * The ui state manager
     * Own
     */    
    CAknFepVkbUiStateMgr* iUiStateMgr;

    /**
     * The vkb window
     * Own
     */    
    CAknFepVkbWindow* iVkbWindow;

    /**
     * The flag showing need to deal focus gained or not
     * Own
     */    
    TBool iNeedDealFocusGained;

    /**
     * The vkb resource id
     */    
    TInt iResId;
    
    /**
     * The ptiengine
     * Not own
     */    
    CPtiEngine* iPtiEngine;
    
    /*
    * Indicate if ITI is enabled.
    */    
    TBool iITIEnabled;	
    };

#endif // C_CAKNFEPVKBLAYOUT_H

//End Of File
