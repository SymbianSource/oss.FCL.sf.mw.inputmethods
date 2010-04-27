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
* Description:  Interface of vkb ui state
*
*/


#ifndef C_CAKNFEPVKBWINDOW_H
#define C_CAKNFEPVKBWINDOW_H

// system includes
#include <peninputbasewindowext.h>

/** class forward decalaration */
class CFepUiLayout;
class CPeninputCommonChoiceList;
class CAknFepVkbClientArea;
class MAknFepVkbDataProvider;
class CAknFepVkbUiStateMgr;
class CAknFepCtrlEventButton;
class CFepLayoutMultiLineIcf;
class CPeninputLayoutInputmodelChoice;
class CPeninputVkbCtrlExt;
class CAknFepCtrlVkbLayout;
class CPeninputCnGenericVkbLafMgr;
class CAknFepCtrlRepeatButton;
class CFepLayoutScrollableList;

// class declarations
/**
 *  vkb window class
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2
 */
class CAknFepVkbWindow : public CAknFepCtrlBaseWindowExt
    {       
public:

    /**
     * Create one CAknFepVkbWindow object
     *
     * @since S60 v3.2
     * @param aUiLayout The layout plugin
     * @param aControlId This control ID
     * @param aDataProvider The interface of data provider
     * @return The CFepCtrlDropdownList object.
     */        
    static CAknFepVkbWindow* NewL(CFepUiLayout* aUiLayout, 
                                  TInt aControlId, 
                                  MAknFepVkbDataProvider* aDataProvider,
                                  CAknFepVkbUiStateMgr* aUiStateMgr);

    /**
     * Create one CAknFepVkbWindow object
     *
     * @since S60 v3.2
     * @param aUiLayout The layout plugin
     * @param aControlId This control ID
     * @param aDataProvider The interface of data provider
     * @return The CFepCtrlDropdownList object.
     */                                        
    static CAknFepVkbWindow* NewLC(CFepUiLayout* aUiLayout, 
                                   TInt aControlId, 
                                   MAknFepVkbDataProvider* aDataProvider,
                                   CAknFepVkbUiStateMgr* aUiStateMgr);

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None.
     */                                    
    virtual ~CAknFepVkbWindow();

    /**
     * Size change function
     *
     * @since S60 v3.2
     * @param aLandscapeStyle Landscape or portrait
     * @return None.
     */       
    void SizeChanged(TBool aLandscapeStyle);

    /**
     * Handle event 
     * @since Series 60 3.2
     * @param aEventType The event type
     * @param aCtrl The control who sends the event
     * @param aEventData The event data
     * @return None
     */     
    void HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData);       
    
    /**
     * Get client area control
     *
     * @since S60 v3.2
     * @return The client area control.
     */        
    CAknFepVkbClientArea* ClientArea();
    
    /**
     * Get input context field
     *
     * @since S60 v3.2
     * @return The input context field
     */     
    CFepLayoutMultiLineIcf* InputContextField();

    /**
     * Change input method layout
     *
     * @since S60 v3.2
     * @param aRange The range defined in TAknFepVkbIMLayout
     * @param aChangeVkb The flag of changing vkb or not     
     * @return None.
     */ 
    void ChangeImLayout(TInt aRange, TBool aChangeVkb = ETrue);
    
    /**
     * Loop vkb layout
     *
     * @since S60 v3.2
     * @param aType The flag showing shift down or capslock down
     * @return None.
     */ 
    void LoopVkbLayout(TVkbShiftType aType, TBool aNotify = EFalse);
    
    /**
     * Change vkb layout
     *
     * @since S60 v3.2
     * @param aVkbLayout The destination layout
     * @return None.
     */     
    void ChangeVkbLayout(TInt aVkbLayoutId);    
    
    /**
     * Get the string length in composition field
     *
     * @since S60 v3.2
     * @return The string length in composition field.
     */     
    TInt CompositionFieldStringLength();
    
    /**
     * Popup accent window
     *
     * @since S60 v3.2
     * @return None.
     */ 
    void PopupAccentWindow();
    
    
    void PopupSwitchWindow();
    
    /**
     * Reset the status accent button
     *
     * @since S60 v3.2
     * @return None.
     */ 
    void ResetAccentButton();
    
    /**
     * Set the input context field text
     *
     * @since S60 v3.2
     * @param aTotalTextLen The total text length in editor
     * @param aStartPos The position of aText in whole editor
     * @param aCursorSel Current cursor selection
     * @param aText The text to be set 
     * @return None
     */          
    void SetEditorTextL(const TFepInputContextFieldData& aData);

    /**
     * Set if the input context field text is secret
     *
     * @since S60 v3.2
     * @param aIsSecret The flag
     * @return None
     */          
    void SetTextIsSecret( TBool aIsSecret );
    
    /**
     * Dim arrow keys when the editor type is
     * find pane or secret editor
     *
     * @since S60 v3.2
     * @param aIsSecret The flag
     * @return None
     */          
    void DimArrowKeys( TBool aDimArrow );
    
    void DimEnterKey( TBool aDimmed );
    
    /**
     * Get current control who has focus
     *
     * @since S60 v3.2
     * @return None.
     */                         
    TInt FocusedControlID();
    
    /**
     * Draw window and shadow
     *
     * @since S60 v3.2
     * @return None
     */          
    void Draw();
    
    /**

    /**
     * Set layout's rectangle
     *
     * @since S60 V3.2
     * @param aRect The new control area
     * @return None
     */                                       
    void SetRect(const TRect& aRect);
    
    /**
     * Is there need to create new word
     *
     * @since S60 v3.2
     * @return ETrue if need to create new word, otherwise EFalse
     */       
    TBool NeedCreateWord();
    
    /**
     * Close Accent window
     *
     * @since S60 V3.2
     * @return None
     */                                       
    void CloseAccentWindow();
    
    static TInt BackgroundTaskL(TAny* aPtr);
        
    /**
     * Set if enable option button and language switch button
     *
     * @since S60 v3.2
     * @param aEnable The enable flag
     * @return None
     */ 
    void SetEnableSettingBtn(TBool aEnable);
    
    /**
     * Return the spell for new created words.
     *
     * @since S60 v3.2
     * @return The buffer for the gernated spell.
     */ 
    TPtrC GetCreatedWordSpell();
    
    void SetSwitchlistLanguage(TInt aLang);
    
    void SetSwitchlistSecretFlag(TBool aSecret);
    
    /**
     * Set prompt text in FSQ mode.
     *
     * @since S60 v3.2
     * @param Prompt text data
     * @return None.
     */ 
    void SetPromptTextL(TUint8* aData);
    
    void ChangeMergedButtonStatus( const TInt aIsShiftCase, 
    							   const TInt aIsCapslockCase );
    							   
    void ShowBubble(TInt aShow);
    
    void SetTextAlignmentL( TInt aAlignment );  
    void UpdateICFTextL();							   
    
    /**
     * Show tooltip of ICF
     * 
     * @since S60 v5.0
     * @param aText The text to be displayed in ICF
     * @return None
     */
    void ShowTooltipOnFSQL( const TDesC& aText );
    
    /**
     * Hide tooltip of ICF
     * 
     * @since S60 v5.0
     * @return None
     */    
    void HideTooltipOnFSQ();
    
    /**
     * Hide candidate list
     * 
     * @since S60 v5.0
     * @return None
     */        
    void HideCandidateListOnFSQ();
    
    /**
     * Show candidate list
     * 
     * @param aItemArray, the contents to be shown
     * @param aActiveIndex, the default index     
     * @return None
     */	
    void ShowCandidateListOnFSQL( TInt aAlign, const CDesCArray* aItemArray,
                            TInt aActiveIndex );    
    
    /**
     * Handle candidate list selected events
     * 
     * @param aCtrl, pointer of the candidate list on which events occur.
     * @param aEventData event data
     * @return None
     */	
    void HandleCandidateListSelectedOnFSQL( CFepUiBaseCtrl* aCtrl, 
                                       const TDesC& aEventData );        
       
protected:

    /**
     * Construct function
     *
     * @since S60 v3.2
     * @param aUiLayout The layout plugin
     * @param aControlId This control ID
     * @param aDataProvider The interface of data provider
     * @return None.
     */ 
    CAknFepVkbWindow(CFepUiLayout* aUiLayout, 
                     TInt aControlId, 
                     MAknFepVkbDataProvider* aDataProvider,
                     CAknFepVkbUiStateMgr* aUiStateMgr);

    /**
     * 2nd construct function
     *
     * @since Series 60 3.2
     * @return None
     */ 
    void ConstructL();
        
    /**
     * Close window
     *
     * @since Series 60 3.2
     * @return None
     */ 
    TBool DoClose();
    
    /**
     * read from resource function
     *
     * @since Series 60 3.2
     * @return None
     */     
    void ConstructFromResourceL();
    
    virtual void AddCustomControlGroupL(CFepUiBaseCtrl* aCtrl);
    
    virtual TBool CreateCustomControlL( TInt16 aControlId, TInt32 aImageId );
    
private:

    /**
     * Add context field into the vkb window
     *
     * @since Series 60 3.2
     * @return None
     */
    void AddContextFieldL();
    
    /**
     * Add popup window into the vkb window
     *
     * @since Series 60 3.2
     * @return None
     */
    void AddPopupWindowL();        
    
    /**
     * Add touch input into the vkb window
     *
     * @since Series 60 3.2
     * @return None
     */
    void AddTouchInputButtonL();   
        
    /**
     * get resource info of accented chars popup window
     *
     * @since S60 v3.2
     * @param aReader Resource reader
     * @return None
     */
    void GetPopupWndInfoFromRes( TResourceReader aReader);
    
    /**
     * Construct popup window
     *
     * @since S60 v3.2
     * @return None
     */
    void DoIdleConstructL();
    
    /**
     * Set item rect, position, font, text color etc to 
     * popup window
     *
     * @since S60 v3.2
     * @return None
     */
    void SetPopupProperty();
    
    TInt GetWindowResId();
    
    void SimulateRawEvent(TInt aScanCode, TRawEvent::TType aType);
    
    void SizeChangedForTitle();
   
    /*
     * Create a candidate list control
     * 
     * @since S60 v5.0
     * @return None
     */   
    void CreateCandidateListOnFSQL();       
    
    /*
     * Retrieve text color from skin for candidiate list
     * 
     * @since S60 v5.0
     * @return None
     */   
    const TRgb CandidateListTextColorOnFSQ();
    
    /*
     * Retrieve text color from skin for autocompletion part
     * 
     * @since S60 v5.0
     * @return None
     */   
    const TRgb AutoCompletionPartColorOnFSQ();

    /*
     * Set item rect, position, font, text color etc to tooltip
     * and candidate list
     * 
     * @since S60 v5.0
     * @return None
     */   
    void SetITILayoutOnFSQ();
    
public:
    CPeninputCnGenericVkbLafMgr* iLafMgr;

private: // data

    /**
     * The client area control
     * Own
     */
    CAknFepVkbClientArea* iClientArea;

    /**
     * The input context field
     * Own
     */    
    CFepLayoutMultiLineIcf* iInputContextField;


    /**
     * The popup window
     * Own
     */    
    CPeninputCommonChoiceList* iPopupWnd;
    
    /**
     * The data provider
     * Not own
     */     
    MAknFepVkbDataProvider* iDataProvider;
public:    
    /**
     * The UI state manager
     * Not own
     */ 
    CAknFepVkbUiStateMgr* iUiStateMgr;
private:    
    /**
     * The background bitmap of popup window
     * Not own
     */      
    CFbsBitmap* iPopupWndBgImg;
    
    /**
     * The background mask bitmap of popup window
     * Not own
     */      
    CFbsBitmap* iPopupWndBgMaskImg;  
    

    /**
     * The focus bitmap of popup window
     * Not own
     */  
    CFbsBitmap* iPopupWndFocusImg;
    
    /**
     * The focus mask bitmap of popup window
     * Not own
     */      
    CFbsBitmap* iPopupWndFocusMaskImg;
    
    /**
     * The rect of whole vkb window
     */
    TRect iWndRect;
    
    /**
     * Accent popuo window open state
     */
    TBool iAccentPopup;
    
    /**
     * First time construct flag
     */
    TBool iFirstTimeConstruct;
    
    CIdle* iIdle;
    TBool iPopupInited;
    
    /**
     * Flag to indicate whether popup window's property
     * have been set
     */
    TBool iPopupSet;
    
    /**
     * Popup window item size
     */
    TSize iPopupItemSize;
    
    CPeninputLayoutInputmodelChoice* iInputModeSwitch;
    CAknFepCtrlRepeatButton* iArrowLeftButton;
    
    CAknFepCtrlRepeatButton* iArrowRightButton;
    
    CAknFepCtrlDragButton* iMoveButton;
    
    /**
     * Vkb layout
     * Own
     */ 
    CAknFepCtrlVkbLayout* iVkbLayout; 
    TAknWindowLineLayout iBubbleSize;
    TAknTextLineLayout iBubbleTextLayout; 

    CFepLayoutScrollableList* iCandidateList;
    
    };  

#endif // C_CAKNFEPVKBWINDOW_H

// End Of File
