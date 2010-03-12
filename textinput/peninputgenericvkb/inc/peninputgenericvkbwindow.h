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
* Description:  peninput generic vkb window layout class
*
*/


#ifndef C_PENINPUTGENERICVKBWINDOW_H
#define C_PENINPUTGENERICVKBWINDOW_H

// System includes
#include <peninputlayoutwindowext.h>
#include <peninputuistatemgrinterface.h>
#include <AknLayoutDef.h>

// Forward declarations
class CAknFepCtrlRangeBar;
class CAknFepCtrlEventButton;
class CAknFepCtrlDragButton;
class CAknFepCtrlRepeatButton;
class CPeninputCommonChoiceList;
class CPeninputLayoutInputmodelChoice;
class CPeninputGenericVkbLafMgr;
class CPeninputSyncBitmapRotator;
class CRepository;
class CFepLayoutScrollableList;
// Constants
const TInt KMaxFileLength = 80;

const TInt KMaxRangeLabelLength = 100;

/**
 * class TAllRangeLabels
 */
class TAllRangeLabels
    {
public:
    TBuf<KMaxRangeLabelLength> iLabelNativeChar;
    TBuf<KMaxRangeLabelLength> iLabelNativeNumber;
    TBuf<KMaxRangeLabelLength> iLabelLatinChar;
    TBuf<KMaxRangeLabelLength> iLabelLatinNumber;
    TBuf<KMaxRangeLabelLength> iLabelSymbol;
    TBuf<KMaxRangeLabelLength> iLabelAccent;
    };
/**
 *  Vkb layout window class
 *  This class define methods to deal with window layout and 
 *  handle events sent to this window
 *
 *  @lib peninputgenericvkb.lib
 *  @since S60 v3.2
 */
class CPeninputGenericVkbWindow : public CPeninputLayoutWindowExt
    {
private:
    enum TIndicatorAlign
        {
        EIndiAlignLeft,
        EIndiAlignCenter,
        EIndiAlignRight 
        };
public:

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aUiLayout The plugin ui layout
     * @param aLayoutContext The vkb layout context
     * @return The pointer to CPeninputGenericVkbWindow object
     */
    static CPeninputGenericVkbWindow* NewL( CFepUiLayout* aUiLayout, 
        MPeninputLayoutContext* aLayoutContext, MPeninputUiStateMgr* aUiStateMgr );
    
    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CPeninputGenericVkbWindow();
    
    /**
     * Do when case changed
     *
     * @since S60 v3.2
     * @param aNewCase The new case
     * @return None
     */
    void DoCaseChange( TInt aNewCase );

// From base class CPeninputLayoutWindow

    /**
     * From CPeninputLayoutWindow
     * Handle control event 
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aCtrl The control who sends the event
     * @param aEventData The event data
     * @return None
     */     
    void HandleControlEvent( TInt aEventType,
                             CFepUiBaseCtrl* aCtrl, 
                             const TDesC& aEventData );

    /**
     * From CPeninputLayoutWindow
     * Get id of window config resource
     *
     * @since S60 v3.2
     * @return The id of window config resource
     */
    TInt GetWindowConfigResId();

    /**
     * From CPeninputLayoutWindow
     * Get id of window resource
     *
     * @since S60 v3.2
     * @return The id of window resource
     */ 
    TInt GetWindowResId();
    
    /**
     * From CPeninputLayoutWindowExt
     * Get resource id of numeric keymapping
     *
     * @since S60 v3.2
     * @return The id of window resource
     */ 
    TInt GetNumKeymappingResId();

    /**
     * From CPeninputLayoutWindow
     * Get file name of window config resource
     *
     * @since S60 v3.2
     * @param aLangID The id of language
     * @return The file name of window config resource
     */ 
    const TDesC& GetWindowConfigResFileName( TInt aLangID );

    /**
     * From CPeninputLayoutWindow
     * Get file name of window resource
     *
     * @since S60 v3.2
     * @return The file name of window resource
     */ 
    const TDesC& GetWindowResFileName();

    /**
     * From CPeninputLayoutWindow
     * Re-organize all controls in the assigned client area layout
     *
     * @since S60 v3.2
     * @param aClientLayoutId The id of client area layout
     * @return None
     */
    void ReorganizeControls( TInt aClientLayoutId, TBool aNeedReset );
  
    /**
     * From CPeninputLayoutWindow
     * Change unit size
     *
     * @since S60 v3.2
     * @return The window rect
     */
    const TRect ChangeUnitSize();
  
    /**
     * From CPeninputLayoutWindow
     * Change size of client area
     *
     * @since S60 v3.2
     * @return None
     */
    void ChangeClientSize();  
    
    /**
     * From CPeninputLayoutWindow
     * Construct all controls specified in resource
     *
     * @since S60 v3.2
     * @return None
     */
    void CreateAllControlsL();
    
    /**
     * From CPeninputLayoutWindow
     * Set control fonts. Fonts info comes from resource
     *
     * @since S60 v3.2
     * @return None
     */
    void SetControlsFont();
  
    /**
     * From CPeninputLayoutWindow
     * Pop up choice list
     *
     * @since S60 v3.2
     * @return None
     */
    void PopupChoiceList(); 
        
    /** 
     * ConstructFromResourceL load settings from resource.
     * Before calling this function, the resource id should be set
     *
     * @since S60 v3.2
     * @return None
     */
    void ConstructFromResourceL();

    static TInt BackgroundTaskL(TAny* aPtr);

    /**
     * config unit size, reason may due to client layout id changed,
     * Set current language
     *
     * @since S60 v3.2
     * @param aLang Current language
     * @return None
     */
    void SetSwitchlistLanguage(TInt aLang);

    /**
     * Current editor is secret
     *
     * @since S60 v3.2
     * @param aSecret Editor is secret
     * @return None
     */
    void SetSwitchlistSecretFlag(TBool aSecret);  
    
    /**
     * Dim the arrow keys on FSQ when the editor 
     * is findpane or secret editor
     *
     * @since S60 v3.2
     * @param aSecret Editor is secret
     * @return None
     */
    void DimArrowKeys( TBool aDimArrow );


    /**
     * Pop up switcher windows
     *
     * @since S60 v3.2
     * @param aSecret Editor is secret
     * @return None
     */    
    void PopupSwitchWindowL();

    /**
     * Handle accented char event
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data
     * @param aDeadKey The DeadKey char buffer
     * @param anewCharBuf The new Accented buffer
     * @return None
     */    
    TBool HandleAccentCharEvent(TInt aEventType, 
                                const TDesC& aEventData, 
                                const TDesC& aDeadKey, 
                                HBufC* anewCharBuf );

    /**
     * Handle virtual key latched event
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aCtrl The controler
     * @param aEventData Event data
     * @return None
     */    
    void HandleVirtualKeyLatchedEvent(TInt aEventType, 
                                      CFepUiBaseCtrl* aCtrl, 
                                      const TDesC& aEventData );


    /**
     * handle virtual key up event
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aCtrl The controler
     * @param aEventData Event data
     * @return None
     */                                  
    TBool HandleVirtualKeyUpEvent(TInt aEventType, 
                                 CFepUiBaseCtrl* aCtrl, 
                                 const TDesC& aEventData );
  
    virtual void SetLafLayOut(TInt aRows, TInt aColumns);
     /*
     *
     * @since S60 v3.2
     * @return None
     */                                  
    void SetDeadKey();
    void ChangeInputModeL();

    /**
     * Unlatch a DeadKey by the key buffer
     *
     * @since S60 v3.2
     * @param abuf The key buffer which will be unlatched
     * @return None
     */
    void UnLatchDeadKey( const TDesC& abuf = KNullDesC );
  
    void SetPromptTextL(TUint8* aData);
    
    void UpdateLafData();
    
	void ConstructAccentListL(TInt aLang);
	
	void ShowBubble(TInt aShow);
	
    /**
     * Show tooltip of ICF
     * 
     * @param aText The text to be displayed in ICF
     * @return None
     */
	void ShowTooltipL( const TDesC& aText );
	
    /**
     * Hide tooltip of ICF
     * 
     * @return None
     */	
	void HideTooltip();
	
    /**
     * Hide candidate list
     * 
     * @return None
     */		
	void HideCandidateList();
	
    /**
     * Show candidate list
     * 
     * @param aItemArray, the contents to be shown
     * @param aActiveIndex, the default index     
     * @return None
     */	
    void ShowCandidateListL( const CDesCArray* aItemArray,
                            TInt aActiveIndex );    
    
    /**
     * Handle candidate list selected events
     * 
     * @param aCtrl, pointer of the candidate list on which events occur.
     * @param, aEventData event data
     * @return None
     */	
    void HandleCandidateListSelectedL( CFepUiBaseCtrl* aCtrl, const TDesC& aEventData );        
    
    /*
     * Load range labels for range list
     *
     * @since S60 v5.0
     * @param aLangId current language id
     * @return None
     */
    void ConstructRangeLabelListL( TInt aLangId );
    
    /**
     * Update state of range button
     * 
     * @since S60 v5.0
     * @return None
     */
    void UpdateRangeCtrlsL();
    
    void ConstructFSQAccentListL( TInt aLangId );
    
    void SetIndiBubbleImageL( const TInt aImgID1,
                              const TInt aMaskID1,
                              const TInt aImgID2,
                              const TInt aMaskID2 );
    
    void IndiBubbleWithText();
    
    void IndiBubbleWithoutText();
    
    void SetIndiWithTextFlag( TBool aFlag );
    
    void UpdateIndiBubbleL( TUint8* aData );
    
    inline TFepIndicatorInfo IndicatorData();
    
    inline void SetIndicatorData( const TFepIndicatorInfo& aIndicatorData );
protected:

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @param aUiLayout The plugin ui layout
     * @param aLayoutContext The vkb layout context
     * @return None
     */
    CPeninputGenericVkbWindow( CFepUiLayout* aUiLayout, 
                               MPeninputLayoutContext* aLayoutContext,
                               MPeninputUiStateMgr* aUiStateMgr );
    void ConstructL();
    
    virtual TBool CreateCustomControlL( TInt16 aControlId, TInt32 aImageId );  
    virtual void  AddCustomControlGroupL(CFepUiBaseCtrl* aCtrl);  

private:
    
    /**
     * Show range list
     *
     * @since S60 v5.0
     * @return None
     */
    void PopupRangeListL();
    
    /**
      * Prepare range list items
      * 
      * @since S60 v5.0
      * @param aRangeItems items in range list
      * @return None
      */
    void PrepareRangeListItems( RArray<CFepLayoutChoiceList::SItem>& 
            aRangeItems );
    
    /**
     * Utils to get context value 
     * 
     * @since S60 v5.0
     * @param aDataIndex index of context value
     * @return conext value
     */
    TInt IntContext( TPeninputDataType aDataIndex );
     
    void CalIndicatorRect(const TRect& aBoundRect,
                          TRect& aRealRect1,
                          TRect& aRealRect2,
                          TIndicatorAlign aAlign);
    
    /**
     * Handle range list event
     *
     * @since S60 v5.0
     * @return None
     */
    void HandleRangeListSelectedL( CFepUiBaseCtrl* aCtrl, 
            const TDesC& aData );
    
    /**
     * Get resource ids of range button icon
     * 
     * @since S60 v5.0
     * @param aLangId current language id
     * @param aAlphaRange variable to receive icon resource id for alpha range
     * @param aOtherRange variable to receive icon resource id for other ranges
     * @return None
     */
    void GetRangeIconResource( TInt aLangId, TInt& aAlphaRange, 
            TInt& aOtherRange );
    
    /**
     * Add range bar into client area
     *
     * @since S60 v3.2
     * @return None
     */
    void AddRangeBarL();

    /**
     * Add button to client area
     *
     * @since S60 v3.2
     * @param aControlId The control id
     * @param aEventId The control's event id
     * @param aResId The control's resource id
     * @param aUnicode The control's unicode
     * @param aIsRepeat The TBool indicating whether the 
     *        control is repeated or not
     * @return The pointer to CAknFepCtrlEventButton object
     */    
    CAknFepCtrlEventButton* AddButtonL( const TInt aControlId,
                                        const TInt aEventId,
                                        const TInt aResId,
                                        const TInt aUnicode = 0,
                                        const TBool aIsRepeat = EFalse );

    /**
     * Add popup window to display accented char list
     *
     * @since S60 v3.2
     * @return None
     */
    void AddPopupWindowL();

    /**
     * Reset accent button
     *
     * @since S60 v3.2
     * @return None
     */
    void ResetAccentButton();

    /**
     * Handle size changed for accented chars popup window
     *
     * @since S60 v3.2
     * @return None
     */
    void PopupWindowSizeChange();
    
    /**
     * get resource info of accented chars popup window
     *
     * @since S60 v3.2
     * @return None
     */
    void GetPopupWndInfoFromResL( TResourceReader reader, const TRect& aRect );  
    void DoIdleConstructL();
    
    void SetVkbLayoutSize();
    
    void SimulateRawEvent(TInt aScanCode, TRawEvent::TType aType);
    
    void SetRangeBarSizeL(TInt x, TInt y);
    
    TBool HandleDeadKeyL(TInt aEventType, const TDesC& aEventData );
    
    void ReorgnizeICFAndButtons();
    
    void UpdateICFTextL();
    
    inline TBool IsRtoLLanguage( TInt aLanguage );
    inline TBool IsValidLanguage(TInt aLanguage); 
    
    void HandleButtonResOnLangDirChange( TInt aControlId );
    void HandleButtonResOnLangDirChangeL( TInt aControlId );
    
    /*
    * Create a candidate list control
    */
    void CreateCandidateListL();       
    
    /*
    * Retrieve text color from skin for candidiate list
    */
    TRgb CandidateListTextColor();
    
    /*
    * Retrieve text color from skin for autocompletion part
    */
    TRgb AutoCompletionPartColor();
        
private: // Data

    /**
     * The vkb extenting control for vkb area
     * Not own
     */
    CPeninputVkbCtrlExt* iVkbCtrl;
    
    /**
     * The range bar
     * Not own
     */
    CAknFepCtrlRangeBar* iRangeBar;
    
    /**
     * The language switch button
     * Not own
     */
    CAknFepCtrlEventButton* iLangSwitchBtn;
    
    /**
     * The resource file name
     */
    TBuf<KMaxFileLength> iResourceFilename;
    
    /**
     * The popup window
     * Own
     */    
    CPeninputCommonChoiceList* iPopupWnd;
    
    /**
     * The background bitmap of popup window
     * Not own
     */      
    CFbsBitmap* iPopupWndBgImg;  

    /**
     * The focus bitmap of popup window
     * Not own
     */  
    CFbsBitmap* iPopupWndFocusImg;
    
    /**
     * The UI state manager
     * Not own
     */  
    MPeninputUiStateMgr* iUiStateMgr;
    
    /**
     * Application window rect
     */  
    TRect iAppWndRect;
    
    /**
     * VKB window rect
     */  
    TRect iVkbWndRect;
    
    CIdle *iIdle;                     
    
    /**
     * Whether popup window has been initied
     */  
    TBool iPopupInited;
    
    /**
     * Popup window item rect
     */  
    TRect iPopupItemRect;

    /**
     * Choice text layout
     */  
    TAknTextLineLayout iChoiceTextLayout;
    
    /**
     * Whether popup window text property has been set
     */  
    TBool iPopupSet;
    
    /**
     * Array of unit size
     * Own
     */  
    RArray<TSize> iUnits;
    /**
     * Touch input switcher
     * Own
     */  
    CAknFepCtrlRepeatButton* iArrowLeftButton;
    
    CAknFepCtrlRepeatButton* iArrowRightButton;
    
    CAknFepCtrlDragButton* iMoveButton;
    
    CPeninputGenericVkbLafMgr* iLafMgr;
    
//    CPeninputLayoutInputmodelChoice* iModeSwitchChoice;

    /**
     * The new DeadKey buffer
     */  
    TBuf<16> iNewDeadKeyBuf;

    /**
     * The old DeadKey buffer
     */  
    TBuf<16> iOldDeadKeyBuf;
    
    
    TAknWindowLineLayout iBubbleSize;
    TAknTextLineLayout iBubbleTextLayout;    
    TBool iFirstConstruct;   
    
    /**
     * Writing language
     */  
    TInt iLanguage;
    
    /**
     * Bitmap rotator
     */  
	CPeninputSyncBitmapRotator* iBmpRotator;
    /**
     * Accent command list
     */
	RArray<CFepLayoutChoiceList::SItem> iAccentCmdList;
	
    TSize iIndicatorSize;
    
    TSize iIndicatorTextSize;
    
    TBool iIndiWithText;
    
    TFepIndicatorInfo iIndicatorData;
    
    /**
     * Range labels for range list
     */
    TAllRangeLabels iRangeLabels;

private:	
	CFepLayoutScrollableList* iCandidateList;
	
    };

inline TBool CPeninputGenericVkbWindow::IsRtoLLanguage( TInt aLanguage )
	{
	return ( aLanguage == ELangArabic ||
			 aLanguage == ELangHebrew ||
			 aLanguage == ELangFarsi  ||
			 aLanguage == ELangUrdu );	
	}    

inline TBool CPeninputGenericVkbWindow::IsValidLanguage(TInt aLanguage)
	{
	return (aLanguage != ELangTest && aLanguage != ELangNone);	
	}
inline void CPeninputGenericVkbWindow::SetIndicatorData( 
        const TFepIndicatorInfo& aIndicatorData )
    {
    iIndicatorData.iIndicatorImgID = aIndicatorData.iIndicatorImgID;
    iIndicatorData.iIndicatorMaskID = aIndicatorData.iIndicatorMaskID;
    iIndicatorData.iIndicatorTextImgID = aIndicatorData.iIndicatorTextImgID;
    iIndicatorData.iIndicatorTextMaskID = aIndicatorData.iIndicatorTextMaskID;
    }
    
inline TFepIndicatorInfo CPeninputGenericVkbWindow::IndicatorData()
    {
    return iIndicatorData;
    }
#endif // C_PENINPUTGENERICVKBWINDOW_H
