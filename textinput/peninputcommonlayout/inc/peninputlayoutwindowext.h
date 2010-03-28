/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  common layout window class
*
*/


#ifndef C_CPENINPUTLAYOUTWINDOWEXT_H
#define C_CPENINPUTLAYOUTWINDOWEXT_H

// System includes
#include <peninputbasewindowext.h>

// User includes
#include "peninputctrlpool.h"
#include "peninputlayoutconfig.h"
#include "peninputcommonlayoutglobalenum.h"

// Forward declarations
class CFepUiLayout;
class CFepInputContextField;
class CAknFepCtrlEventButton;
class MPeninputLayoutContext;
class CPeninputLayoutConfig;
class CPeninputNumericKepMappingMgr;

/**
 *  Common pen input layout window class.
 *  This class define the common behaviors of VKB layout window and 
 *  HWR layout window.
 *
 *  @lib peninputcommonlayout.lib
 *  @since S60 v3.2
 */
class CPeninputLayoutWindowExt : public CAknFepCtrlBaseWindowExt
    {

public:

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual ~CPeninputLayoutWindowExt();

    /**
     * Get layout config
     *
     * @since S60 v3.2
     * @return The pointer to CPeninputLayoutConfig object
     */
    IMPORT_C CPeninputLayoutConfig* ConfigInfo() const;

    /**
     * Change input language
     *
     * @since S60 v3.2
     * @param aLangID The new language id
     * @return None
     */
    IMPORT_C void ChangeInputLanguageL( TInt aLangID );

    /**
     * Change range
     *
     * @since S60 v3.2
     * @param aRange The destination range
     * @return None
     */     
    IMPORT_C void ChangeRange( TInt aRange, TInt aVkbLayoutId = 0, TBool aNotify = ETrue );
   
    /**
     * Set the input context field text
     *
     * @since S60 v3.2
     * @param aData The data of type TFepInputContextFieldData to be set
     * @return None
     */          
    IMPORT_C void SetEditorTextL( const TFepInputContextFieldData& aData );
    
    /**
     * Set if the input context field text is secret
     *
     * @since S60 v3.2
     * @param aData The flag
     * @return None
     */          
    IMPORT_C void SetTextIsSecret( TBool aData );

    void SetNumberGrouping(TBool aData);

    /**
     * Method for size changing of the window
     *
     * @since S60 v3.2
     * @param aLandscapeStyle The flag that specify the window style is 
     *  landscape or portrait
     * @return None
     */
    IMPORT_C virtual void SizeChanged( TBool aLanscapeStyle );
    
    /**
     * Handle control event 
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aCtrl The control who sends the event
     * @param aEventData The event data
     * @return None
     */     
    IMPORT_C virtual void HandleControlEvent( TInt aEventType, 
                                              CFepUiBaseCtrl* aCtrl, 
                                              const TDesC& aEventData );  

    /**
     * Change client area layout
     *
     * @since S60 v3.2
     * @param aClientLayoutId The id of client area layout
     * @return None
     */
    IMPORT_C void ChangeClientLayout( TInt aClientLayoutId );
    
    /**
     * Change vkb area layout
     *
     * @since S60 v3.2
     * @param aVkbLayoutId The id of vkb area layout
     * @return None
     */     
    IMPORT_C void ChangeVkbLayout( TInt aVkbLayoutId );  
    
    /**
     * Get id of window config resource
     *
     * @since S60 v3.2
     * @return The id of window config resource
     */
    virtual TInt GetWindowConfigResId() = 0;

    /**
     * Get id of window resource
     *
     * @since S60 v3.2
     * @return The id of window resource
     */ 
    virtual TInt GetWindowResId() = 0;

    /**
     * Get file name of window config resource
     *
     * @since S60 v3.2
     * @param aLangID The id of language
     * @return The file name of window config resource
     */ 
    virtual const TDesC& 
        GetWindowConfigResFileName( TInt aLangID ) = 0;

    /**
     * Get file name of window resource
     *
     * @since S60 v3.2
     * @return The file name of window resource
     */ 
    virtual const TDesC& GetWindowResFileName() = 0;
    
    /**
     * Change unit size
     *
     * @since S60 v3.2
     * @return The rect of whole window
     */
	virtual const TRect ChangeUnitSize() = 0;
  
    /**
     * Change size of client area
     *
     * @since S60 v3.2
     * @param aLeftTopPoint The left top point of client area
     * @return None
     */
    virtual void ChangeClientSize() = 0; 
    
    /**
     * Construct all controls specified in resource(implementation will be 
     * postponed in subclass)
     *
     * @since S60 v3.2
     * @return None
     */
    virtual void CreateAllControlsL() = 0;
    
    /**
     * Set control fonts. Fonts info comes from resource
     *
     * @since S60 v3.2
     * @return None
     */
    virtual void SetControlsFont() = 0;
  
    /**
     * Pop up choice list
     *
     * @since S60 v3.2
     * @return None
     */
    virtual void PopupChoiceList() = 0;
  
    /**
     * Re-organize all controls in the assigned client area layout
     *
     * @since S60 v3.2
     * @param aClientLayoutId The id of client area layout
     * @return None
     */
    virtual void ReorganizeControls( TInt aClientLayoutId, TBool aNeedReset ) = 0;

    /**
     * Do when case changed
     *
     * @since S60 v3.2
     * @param aNewCase The new case
     * @return None
     */
    virtual void DoCaseChange( TInt aNewCase ) = 0;
    IMPORT_C virtual void OnInputLanguageChange();
          
    /**
     * Handler for range button clicked
     *
     * @since S60 v3.2
     * @param aRange The range id
     * @return None
     */
    IMPORT_C virtual void HandleRangeEvent( TInt aRange );

    /**
     * Handler for range loop event
     *
     * @since S60 v3.2
     * @param aLoop The loop id
     * @return None
     */
    IMPORT_C virtual void HandleRangeLoopEvent( TInt aLoop );
    
	/**
     * Handler for shift and capslock merged button clicked
     *
     * @since S60 v3.2
     * @param aMode The mode of the mutimode button
     * @return None
     */
    IMPORT_C virtual void HandleMergedBtnClicked( TInt aCase );
    
    /**
     * Handler for shift button clicked
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual void HandleShiftBtnClicked();

    /**
     * Handler for capslock button clicked
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual void HandleCapslockBtnClicked();
    
    /**
     * Handler for shif and capslock button clicked
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual void HandleShiftAndCapslockBtnClicked();

    /**
     * Draw window and shadow
     *
     * @since S60 v3.2
     * @return None
     */          
    IMPORT_C virtual void Draw();
    
    /**
     * Set if enable switch to hwr
     *
     * @since S60 v3.2
     * @param aEnable The enable flag
     * @return None
     */ 
    IMPORT_C void SetEnableSwitchToHwr(TBool aEnable);
    
    /**
     * Set if enable option button and language switch button
     *
     * @since S60 v3.2
     * @param aEnable The enable flag
     * @return None
     */ 
    IMPORT_C void SetEnableSettingBtn(TBool aEnable);

    /**
     * config unit size, reason may due to client layout id changed,
     * or screen resolution changed
     *
     * @since S60 v3.2
     * @param aClientLayoutId New client layout id
     * @return ETrue means unit size changed, otherwise EFalse
     */
    IMPORT_C virtual TBool ReconfigUnitSize(TInt aClientLayoutId);
    
    /**
     * Get numeric keymapping resource id
     *
     * @since S60 v3.2
     * @return The id of numeric keymapping
     */ 
    virtual TInt GetNumKeymappingResId() = 0;
    
    IMPORT_C void SetTextAlignmentL( TInt aAlignment );
       
protected:

    /**
     * constructor
     *
     * @since S60 v3.2
     * @param aUiLayout The pointer to CFepUiLayout object
     * @param aLayoutContext The pointer to MPeninputLayoutContext
     * @return None
     */
    IMPORT_C CPeninputLayoutWindowExt( CFepUiLayout* aUiLayout, 
                                    MPeninputLayoutContext* aLayoutContext );

    /**
     * Second-phase constructor
     *
     * @since S60 v3.2
     * @return None
     */ 
    IMPORT_C void ConstructL();
    /**
     * From CAknFepCtrlBaseWindow
     * Close window
     *
     * @since S60 v3.2
     * @return The TBool indicating the result of close operation
     */ 
    IMPORT_C TBool DoClose();
    
    /**
     * Construct from resource file
     *
     * @since S60 v3.2
     * @param aResReader The resource reader
     * @param aResUtils The resource utils
     * @return None
     */     
    //IMPORT_C virtual void ConstructFromResourceL( 
                                     //TResourceReader& aResReader );

    /** 
     * ConstructFromResourceL load settings from resource.
     * Before calling this function, the resource id should be set
     *
     * @since S60 v3.2
     * @return None
     */
    //IMPORT_C void ConstructFromResourceL();
    
     /**
     * Add not owned control into the window
     *
     * @since S60 v3.2
     * @param aControl The control to be added
     * @return None
     */  
    IMPORT_C void AddNotOwnedControl( CFepUiBaseCtrl* aControl );

    /**
     * Change button status
     *
     * @since S60 v3.2
     * @param aIsDown The button is down or not
     * @param aControlId The destination button id
     * @return None
     */  
    
    IMPORT_C void ChangeButtonStatus(const TInt aIsDown, const TInt aControlId);
	
	/**
     * Change Merged shift and capslock button status
     *
     * @since S60 v3.2
     * @param aIsShiftCase when shift case
     * @param aIsCapslockCase when capslock case
     * @return None
     */  
    
	IMPORT_C void ChangeMergedButtonStatus(	const TInt aIsShiftCase, 
	    								 	const TInt aIsCapslockCase );    								 
			
    /**
     * Change layout position
     *
     * @since S60 v3.2
     * @return The new rectangle of layout
     */      
    IMPORT_C void ChangeLayoutPosition( TInt aJustifyStyle );
    
    /**
     * Set the window rect
     *
     * @since S60 v3.2
     * @param aRect The new rect of the window
     * @return None
     */                                            
    IMPORT_C void SetWindowRect(const TRect& aRect);

    /**
     * Read icf bitmap from resource
     *
     * @since S60 v3.2
     * @param aResReader The reource reader
     * @return None
     */
    //IMPORT_C void ReadIcfInfo( TResourceReader aResReader );
    
    /**
     * Set text font for language switch button.
     *
     * @since S60 v3.2
     * @param aSwitchBtn Pointer to language switch button
     * @return None
     */
    IMPORT_C void SetSwitchBtnFont(CAknFepCtrlEventButton& aSwitchBtn);
    
    /**
     * Set text color for language switch button.
     *
     * @since S60 v3.2
     * @param aSwitchBtn Pointer to language switch button
     * @return None
     */
    IMPORT_C void SetSwitchBtnTextColor(CAknFepCtrlEventButton& aSwitchBtn);
    
    
    /**
     * Change size for base window, such as move button,drag button etc.
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C  virtual void SizeChangedForBaseWindow( TInt aTotalColumns ); 
    
    IMPORT_C virtual void SetLafLayOut(TInt aRows, TInt aColumns);   
    
    IMPORT_C virtual void SetVkbLayoutSize();
    
    virtual TBool CreateCustomControlL( TInt16 aControlId, TInt32 aImageId );
    
    virtual void AddCustomControlGroupL(CFepUiBaseCtrl* aCtrl);

private:

    /**
     * Read laf of shadow
     *
     * @since S60 v3.2
     * @param aRect the parent window
     * @return None
     */
    void ReadLafForShadow();
    
    /**
     * Check whether or not the resource file exists
     *
     * @since S60 v3.2
     * @param aFileName The name of resource file
     * @return The TBool indicating existance of resource file
     */
    TBool CheckResourceExist( const TDesC& aFileName );
    
    /**
     * Get case decided by shift&caps status
     *
     * @since S60 v3.2
     * @return The case value
     */
    TInt CaseByShiftCapslockStatus();
    
    /**
     * Dim keys
     *
     * @since S60 v3.2
     * @return None
     */
    void DimKeys();
    void DimKeysL();    
    
    /**
     * Change range bar
     *
     * @since S60 v3.2
     * @return None
     */
    void ChangeRangeBarL();
    
    /**
     * Change window position
     *
     * @since S60 v3.2
     * @return None.
     */    
    void TopLeftJustify();

    /**
     * Change window position
     *
     * @since S60 v3.2
     * @return None.
     */    
    void BottomRightJustify();

    /**
     * Change window position
     *
     * @since S60 v3.2
     * @return None.
     */    
    void DataQueryJustify();
        
    /**
     * Get key mapping string from resource or user defined mappings
     *
     * @since S60 v3.2
     * @param aKeyMapping The key mapping enum
     * @return None.
     */    
    HBufC* GetKeyMappingStringL( TInt aKeyMapping, const TInt aLang );
    
    void ResetLastColRow();
protected: // data

    /**
     * The layout config
     * Own
     */    
    CPeninputLayoutConfig* iConfigInfo;

    /**
     * The layout context
     * Not own
     */
    MPeninputLayoutContext* iLayoutContext;        

    /**
     * The numeric kep mapping manager
     * Own
     */        
    CPeninputNumericKepMappingMgr* iKeyMappingMgr;
        
    /**
     * Vkb layout
     * Own
     */ 
    CAknFepCtrlVkbLayout* iVkbLayout;

    /**
     * Language is changed
     */
    TBool iLangOrSizeChanged;

    /**
     * The total column for the window
     */
    TInt iLastUsedTotalColumns;

    TInt iLastUsedTotalRows;

    /**
     * The resource id of window
     */
    TInt iWinResId;

    /**
     * The resource id of config
     */
    TInt iConfigResId;
    
    /**
     * Width of top left shadow rect
     */
    TInt iShadowTlWidth;

    /**
     * Height of top left shadow rect
     */
    TInt iShadowTlHeight;

    /**
     * Width of bottom right shadow rect
     */
    TInt iShadowBrWidth;

    /**
     * Height of bottom right shadow rect
     */
    TInt iShadowBrHeight;
    
    /**
     * First time construct flag
     */
    TBool iFirstTimeConstruct;        
    
    /**
     * Switch button font id
     */
    TInt32 iSwitchFont;
    
    /**
     * Switch button shadow font id
     */
    TInt32 iSwitchShadowFont;
    
    /**
     * Indicate whether switch(shadow) font has been set
     */
    TBool iSwitchFontSet;
    
    /**
     * Indicate whether need size change when change unit size
     */
    TBool iUnitSizeChange;
    
    /**
     * Tone mark set for Viet
     */
    HBufC* iToneSet;
    
    /**
     * Vowel char flag for Viet
     */
    TBool iVowelChar;
    };
#endif // C_CPeninputLayoutWindowExt_H
