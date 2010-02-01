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
* Description:  header of main window of finger hwr.
*
*/

#ifndef C_PENINPUTFINGERHWRWND_H
#define C_PENINPUTFINGERHWRWND_H

// INCLUDES
#include <peninputlayoutctrlgroup.h>

// FORWARD DECLARATIONS
class CFepLayoutMultiLineIcf;
class CAknFepCtrlButton;
class CAknFepCtrlEventButton;
class CAknFepCtrlRepeatButton;
class CAknFepCtrlMultiImageButton;
class CFepCtrlDropdownList;
class TResourceReader;
class CPeninputLayoutInputmodelChoice;
class CTransparentHwrWnd;
class CVirtualKeyboard;
class CVirtualKey;
class CPeninputFingerHwrLAFManager;
class CMultiPageVirtualKeyboard;
class CPeninputFingerHwrRangeSelector;

enum TPagePosition
    {
    EPagePosPageNo,
    EPagePosNextPage,
    EPagePosPrevPage,
    EPagePosLastPage,
    EPagePosFirstPage,
    };

/**
 *  class CPeninputFingerHwrWnd.
 *
 *  Main window of finger hwr. 
 *
 *  @lib peninputfingerhwr.lib
 *  @since S60 v5.0
 */
class CPeninputFingerHwrWnd : public CControlGroup
    {

public:

    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * 
     * @param aUiLayout The layout
     * @param aControlId The control id
     * @param aLandscapeStyle Current screen is landscape or not
     * @return Pointer to created CPeninputFingerHwrWnd object
     */
    static CPeninputFingerHwrWnd* NewL( CFepUiLayout* aFepUiLayout,
            TInt aControlId, TBool aLandscapeStyle );

    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * 
     * @param aUiLayout The layout
     * @param aControlId The control id
     * @param aLandscapeStyle Current screen is landscape or not
     * @return Pointer to created CPeninputFingerHwrWnd object
     */
    static CPeninputFingerHwrWnd* NewLC( CFepUiLayout* aFepUiLayout,
            TInt aControlId, TBool aLandscapeStyle );

    /**
     * standard c++ destructor.
     *
     * @since S60 v5.0
     * @return None
     */
    virtual ~CPeninputFingerHwrWnd();

    
public: //from base class CFepUiBaseCtrl
    /**
     * From CFepUiBaseCtrl.
     * Construct from resource. Handle resource change.
     *
     * @since S60 v4.0
     */
    void ConstructFromResourceL();
    
    /**
     * From CFepUiBaseCtrl
     * Draw control
     *
     * @since S60 V4.0
     */      
    void Draw();
    
public: 

    /**
     * Resize according to LAF.
     *
     * @since S60 v5.0
     * @param aLandscapeStyle Current screen is landscape or not
     * @return None
     */
    void SizeChangedL( TBool aLandscapeStyle );

public: // target editor attributes
    
    /**
     * accept editor's text alignment.  
     * 
     * @since S60 v5.0
     * @param aAlignment text alignment id. It can be one of the following: 
     *     EAknEditorAlignNone
     *     EAknEditorAlignCenter
     *     EAknEditorAlignLeft
     *     EAknEditorAlignRight
     *     EAknEditorAlignBidi
     * @param aLangId language id
     * @return none
     */ 
    void SetTextAlignmentL( TInt aAlignment, TInt aLangId );
    
    /**
     * accept editor's text.
     * 
     * @since S60 v5.0
     * @param aData editor's text.
     * @return none
     */
    void SetEditorTextL( const TFepInputContextFieldData& aData );
    
    /**
     * accept editor's promopt txt
     * 
     * @since S60 v5.0
     * @param aPromoptData editor's promopt text
     * @return none
     */    
    void SetPromptTextL( TUint8* aPromoptData );
    
    /**
     * Dim option button
     * 
     * @since S60 v5.0
     * @param aEnable Enable or disable option button
     * @return none
     */    
    void SetEnableSettingBtn(const TBool aEnable);
    
    /**
     * accept editor's bubble text.
     * 
     * @since S60 v5.0
     * @param aPromoptData editor's bubble text.
     * @return none
     */       
    void SetBubbleTextL( const TDesC& aInfo );    

    /**
     * accept editor's char ranges restriction.
     * 
     * @since S60 v5.0
     * @param aPermittedRanges permitted char range flags. It can be combination of the following: 
     *     ERangeNative
     *     ERangeEnglish
     *     ERangeNumber.
     *     See TAknFepPenInputRange.
     * @return none
     */
    void SetPermittedRanges( const TInt aPermittedRanges );
    
    /**
     * accept editor's number mapping restriction.
     * 
     * @since S60 v5.0
     * @param aNumMapping a string contains numbers and related char. max length is 16.
     * @return none
     */
    void SetNumericMapping( const TDesC& aNumMapping );
    
    /**
     * accept editor's input language
     * 
     * @since S60 v5.0
     * @param aLanguageId language id.
     * @return none
     */
    void SetInputLanguage( const TInt aLanguageId );
    
public: //hwr 
    
    /**
     * get strokes from hwr writing box
     * 
     * @since S60 v5.0
     * @return a TPoint array of strokes
     */
    const RArray<TPoint>& StrokeList();

    /**
     * set end mark of hwr
     * 
     * @since S60 v5.0
     * @param aEndMark a TPoint object as end mark
     * @return none
     */
    void SetStrokeEndMark( const TPoint& aEndMark );
    
    /**
     * set pen color of hwr writing 
     * 
     * @since S60 v5.0
     * @return none
     */
    void SetBoxPenColor( const TInt aPenColor );
    
    /**
     * set pen size of hwr writing 
     * 
     * @since S60 v5.0
     * @return none
     */
    void SetBoxPenSize( const TSize aPenSize );
    
    /**
     * set writing speed hwr writing 
     * 
     * @since S60 v5.0
     * @return none
     */    
    void SetBoxWritingSpeed( const TTimeIntervalMicroSeconds32& aCharDelay );
    
    /**
     * retrieve rect of hwr writingbox.
     * 
     * @since S60 v5.0
     * @return rect of hwr writingbox.
     */
    TRect WritingBoxRect();
    
    /**
     * Cancel wrting
     *
     * @since S60 v5.0
     * @return None
     */
    void CancelWriting();    

public: //ime switching list   
    /**
     * Popup ime switch list
     *
     * @since S60 v5.0
     * @return None
     */
    void PopupImeSwitcher();
    
public: //range list
    /**
     * show range selection list
     * 
     * @since S60 v5.0
     * @param aDirect direction of popup. It can be one of the following:
     *     EPopDirAuto 
     *     EPopDirLeft 
     *     EPopDirRight 
     *     EPopDirTop 
     *     EPopDirDown
     *     default value is EPopDirAuto. See TPeninputPopListDirection.
     * @return none
     */
    void ShowRangeDropList( TInt aDirect );
    
    /**
     * cancel range selection and hide selection list.
     * 
     * @since S60 v5.0
     * @return none
     */
    void CancelRangeDropList();    

    /**
     * test whether the range selection list is visible 
     * 
     * @since S60 v5.0
     * @return ETrue if range selection list is visible
     */
    TBool RangeDropListVisible();
    
public: //sct
    
    /**
     * Change SCT page.
     * 
     * @since S60 v5.0
     * @param aPageNo page no.
     * @param aPos Specifies the interpretation of the aPageNo parameter:
            EPagePosPageNo - go to page aPageNo.
            EPagePosNextPage -  go to next page.
            EPagePosPrevPage -  go to previous page.
            EPagePosLastPage -  go to last page.
            EPagePosFirstPage  - go to first page.
     * @return none
     */
    void ShowSctPage( TInt aPageNo, TInt aPos );
    
    /**
     * sync sct paging buttons.
     * 
     * @since S60 v5.0
     * @return none
     */
    void UpdateSctPageButtonsUi();
    
public: //child control access
    
    /**
     * retrieve the ICF control. 
     * 
     * @since S60 v5.0
     * @return pointer to ICF
     */
    CFepLayoutMultiLineIcf* Icf();

    /**
     * retrieve the predictive List control. 
     * 
     * @since S60 v5.0
     * @return pointer to predictive List
     */    
    CFepCtrlDropdownList* PredictiveList();
    
    /**
     * retrieve the candicate list control. 
     * 
     * @since S60 v5.0
     * @return pointer to candicate list 
     */    
    CFepCtrlDropdownList* CandidateList();
    
public: //ui
    
    /**
     * cancel all popped list
     * 
     * @since S60 v5.0
     * @return none
     */
    void CancelAllPopList();
    
    /**
     * update layout
     * safe version of UpdateLayoutL()
     * 
     * @since S60 v5.0
     * @param aIsStandby ETrue if is standby state
     * @return none
     */
    void UpdateLayout( const TBool aIsStandby, 
            const TBool aReloadCandicate=ETrue, 
            const TBool aReloadPredictive=ETrue );
    
    /**
     * update layout
     * 
     * @since S60 v5.0
     * @param aIsStandby ETrue if is standby state
     * @return none
     */    
    void UpdateLayoutL( const TBool aIsStandby, 
            const TBool aReloadCandicate=ETrue, 
            const TBool aReloadPredictive=ETrue );
    
#ifndef RD_INTELLIGENT_TEXT_INPUT
    /**
     * hide or show eng-symbol candidate lists.
     * 
     * @since S60 v5.0
     * @param aIsHide ETrue to hide lists, EFalse to show lists.
     * @return none
     */
    void HideTwoRowEngSymbolList( const TBool aIsHide );
#endif

    /**
     * dim arrow buttons
     * 
     * @since S60 v5.0
     * @param aDimArrow ETrue to dim arrow buttons, EFalse to set arrow buttons as not dimmed. 
     * @return none
     */
    void DimArrowKeys( TBool aDimArrow );
    
public: //range
    
    /**
     * change layout to specified char range.
     * 
     * @since S60 v5.0
     * @param aCharRange current range id. It can be one of the following:
     *      EFingerHwrNativeRange
     *      EFingerHwrEnglishRange
     *      EFingerHwrNumberRange
     *      EFingerHwrSymbolRange
     * @return none
     */
    void TryToChangeCharRange( TInt aCharRange );
    
    /**
     * retrieve char range of layout, including sct.
     * 
     * @since S60 v5.0
     * @return char range id.
     */
    TInt CurrentCharRange();
    
    /**
     * restore the char range saved before sct.
     * 
     * @since S60 v5.0
     * @return none
     */
    TInt RestoreNonSctLayout();
    
    /**
     * Show preview bubble
     * 
     * @since S60 v5.0
     * @param aShow Show/Hide flag
     * @return none
     */
    void ShowBubble(TInt aShow);

protected:

    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @param aFepUiLayout A Ui Layout environment (CFepUiLayout)
     * @param aControlId Control id
     * @return None
     */
    CPeninputFingerHwrWnd( CFepUiLayout* aFepUiLayout, TInt aControlId );

private:

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v5.0
     * @param aLandscapeStyle Current screen is landscape or not
     * @return None
     */
    void ConstructL( TBool aLandscapeStyle );

    /**
     * create icf.
     * 
     * @since S60 v5.0
     * @return None
     */
    void CreateContextFieldL();

    /**
     * create candidate lists.
     * 
     * @since S60 v5.0
     * @return None
     */
    void CreateCandidateListL();

    /**
     * create predictive lists.
     * 
     * @since S60 v5.0
     * @return None
     */    
    void CreatePredictListL();
    
    /**
     * create writing box.
     * 
     * @since S60 v5.0
     * @return None
     */    
    void CreateWritingBoxL();
    
    /**
     * create all function buttons. 
     * 
     * @since S60 v5.0
     * @return None
     */    
    void CreateButtonsL(); 
    
    /**
     * create virtual number pad.
     * 
     * @since S60 v5.0
     * @return None
     */    
    void CreateVirtualNumpadL();    
    
    /**
     * create virtual sct pad.
     * 
     * @since S60 v5.0
     * @return None
     */    
    void CreateVirtualSctpadL();
    
    /**
     * create space and backspace virtual pad.
     * 
     * @since S60 v5.0
     * @return None
     */    
    void CreateFixVirtualSctpadL();
    
    /**
     * EventButton creation helper.
     * 
     * @since S60 v5.0
     * @param aCtrlId button control id.
     * @param aResId resource id.
     * @param aEvent event id;
     * @param aUnicode a unicode value to be sent
     * @return pointer to created CAknFepCtrlEventButton obj.
     */    
    CAknFepCtrlEventButton* CreateEventBtnL( TInt aCtrlId, TInt32 aResId, 
            TInt aEvent = 0xFFFF, TInt aUnicode=0 );
    
    /**
     * RepeatButton creation helper.
     * 
     * @since S60 v5.0
     * @param aCtrlId button control id.
     * @param aResId resource id.
     * @param aEvent event id;
     * @param aUnicode a unicode value to be sent
     * @return pointer to created CAknFepCtrlRepeatButton obj. 
     */
    CAknFepCtrlRepeatButton* CreateRepBtnL( const TInt aCtrlId, TInt32 aResId,  
            const TInt aEvent, const TInt aUnicode );
    
    /**
     * load virtual number pad images. 
     * 
     * @since S60 v5.0
     * @param aResId resource id.
     * @param aKeySize virtual key rect.
     * @return None
     */    
    void LoadVkbKeyImageL( CVirtualKeyboard* aVkb, const TInt aResId, const TSize& aKeySize );
    
    
    /**
     * create virtual key.
     * 
     * @since S60 v5.0
     * @param aReader resource reader.
     * @param aKeyRect rect of virtual key.
     * @return pointer of created CVirtualKey obj.
     */    
    CVirtualKey* CreateVkbKeyL( TResourceReader& aReader, const TRect aKeyRect );
    
    /**
     * update virtual key info. 
     * 
     * @since S60 v5.0
     * @param aVirtualKey an exist virtul key.
     * @param aReader resource reader.
     * @param aKeyRect rect of virtual key.
     * @return None
     */    
    void UpdateVkbKeyL( CVirtualKey* aVirtualKey, TResourceReader& aReader, 
            const TRect aKeyRect );

    /**
     * load virtual number keys.
     * 
     * @since S60 v5.0
     * @param aResId resource id.
     * @param aCellRects rects of virtual keys.
     * @return None
     */    
    void LoadVirtualNumpadKeysL( const TInt aResId, const RArray<TRect>& aCellRects );
    
    /**
     * load virtual sct keys.
     * 
     * @since S60 v5.0
     * @param aResId resource id.
     * @param aCellRects rects of virtual keys.
     * @return None
     */      
    void LoadVirtualSctpadKeysL( const TInt aResId, const RArray<TRect>& aCellRects );
        
    /**
     * load space and backspace virtual keys.
     * 
     * @since S60 v5.0
     * @param aResId resource id.
     * @param aCellRects rects of virtual keys.
     * @return None
     */      
    void LoadFixVirtualSctpadKeysL( const TInt aResId, const RArray<TRect>& aCellRects );
    
    /**
     * relayout full ui, reset all controls position. 
     * 
     * @since S60 v5.0
     * @return None
     */    
    void ResetLayoutL();

    /**
     * layout for chinese range standby.
     * 
     * @since S60 v5.0
     * @return None
     */    
    void LayoutStandbyChinese();
    
    /**
     * layout for english range standby. 
     * 
     * @since S60 v5.0
     * @return None
     */    
    void LayoutStandbyEnglish();
    
    /**
     * layout for number range standby.
     * 
     * @since S60 v5.0
     * @return None
     */    
    void LayoutStandbyNumeric();
    
    /**
     * layout for symbol range standby.
     * 
     * @since S60 v5.0
     * @return None
     */    
    void LayoutStandbySymbol();

    /**
     * EventButton layout helper. Move button to specified rect.
     * 
     * @since S60 v5.0
     * @param aButton the button to move
     * @param aRect new rect of button
     * @param aXPadding horz padding of button icon.
     * @param aYPadding vert padding of button icon.
     * @param aReloadImages Specifies whether the images is to be reloaded.
     * @return None
     */    
    void MoveIconButton( CAknFepCtrlEventButton* aButton, const TRect& aRect, 
            TInt aXPadding=0, TInt aYPadding=0, TBool aReloadImages=ETrue );

    /**
     * EventButton layout helper. Move button to specified rect.
     * 
     * @since S60 v5.0
     * @param aButton the button to move
     * @param aRect new rect of button
     * @param aXPadding horz padding of button icon.
     * @param aYPadding vert padding of button icon.
     * @param aReloadImages Specifies whether the images is to be reloaded.
     * @return None
     */    
    void MoveSwitchPageButton( CAknFepCtrlEventButton* aButton, const TRect& aRect, 
            TInt aXPadding=0, TInt aYPadding=0, TBool aReloadImages=ETrue );


    /**
     * read control's shadow info.
     * 
     * @since S60 v5.0
     * @param aResId resource id.
     * @return None
     */    
    void ReadShadowInfoL( const TInt aResId );
    
    /**
     * read control's background info.
     *
     * @since S60 v5.0
     * @param aResId resource id.
     * @return None
     */
    void ReadBackgroundInfoL( const TInt aResId );
    
    /**
     * read icf appearance info.
     * 
     * @since S60 v5.0
     * @param aResId resource id.
     * @return None
     */    
    void ReadICFInfoL( const TInt aResId );
    
    /**
     * read writingbox appearance info.
     * 
     * @since S60 v5.0
     * @param aResId resource id.
     * @return None
     */    
    void ReadWritingBoxInfoL( TInt aResId );

    /**
     * update feedback state of all virtual keys 
     * 
     * @since S60 v5.0
     * @param aType char range id
     * @return None
     */
    void UpdateAllVirtualKeysFeedback( TInt aType );

    /**
     * Reload guide line
     * 
     * @since S60 v5.0
     * @return None
     */
    void ReloadWritingGuideLinesL();
     
    /**
     * Set highlight cell in candidate list
     * 
     * @since S60 v5.0
     * @return None
     */
    void SetCandHighlightCell();
    
private: // datas

    /**
     * Input context field control.
     * Not own.
     */
    CFepLayoutMultiLineIcf* iContextField;


    /**
     * 1st fixed eng-symbol list control.
     * Not own.
     */    
    CFepCtrlDropdownList* iFixEngSymbolList1;
    
    /**
     * 2nd fixed eng-symbol list control.
     * Not own.
     */    
    CFepCtrlDropdownList* iFixEngSymbolList2;

    /**
     * Candidate list control.
     * Not own.
     */    
    CFepCtrlDropdownList* iCandidateList;
    
    /**
     * Predictive list control.
     * Not own.
     */
    CFepCtrlDropdownList* iPredictList;
    
    /**
     * Writing box control.
     * Not own.
     */
    CTransparentHwrWnd* iWritingBox;

    /**
     * Close button control.
     * Not own.
     */    
    CAknFepCtrlEventButton* iCloseBtn;

    /**
     * Option button control.
     * Not own.
     */     
    CAknFepCtrlEventButton* iOptionBtn;
    
    /**
     * Ime switch button control.
     * Not own.
     */     
    CAknFepCtrlEventButton* iImeSwitchBtn;

    /**
     * Backspace button control.
     * Not own.
     */     
    CAknFepCtrlRepeatButton* iBackspaceBtn;

    /**
     * Left arrow button control.
     * Not own.
     */     
    CAknFepCtrlRepeatButton* iArrowLeftBtn;
    
    /**
     * Right arrow button control.
     * Not own.
     */     
    CAknFepCtrlRepeatButton* iArrowRightBtn;
    
    /**
     * Up arrow button control.
     * Not own.
     */     
    CAknFepCtrlRepeatButton* iArrowUpBtn;
    
    /**
     * Down arrow button control.
     * Not own.
     */     
    CAknFepCtrlRepeatButton* iArrowDownBtn;

    /**
     * Sct selection button control.
     * Not own.
     */     
    CAknFepCtrlEventButton* iRangeSpecialBtn;

    /**
     * Sct page 0 selction button control.
     * Not own.
     */     
    CAknFepCtrlEventButton* iSctPage0Btn;
    
    /**
     * Sct page 1 selction button control.
     * Not own.
     */     
    CAknFepCtrlEventButton* iSctPage1Btn;
    
    /**
     * Sct page 2 selction button control.
     * Not own.
     */     
    CAknFepCtrlEventButton* iSctPage2Btn;

    
    /**
     * Virtual number pad control.
     * Not own.
     */     
    CVirtualKeyboard* iVirtualNumpad;
    
    /**
     * Virtual sct pad control.
     * Not own.
     */     
    CMultiPageVirtualKeyboard* iVirtualSctpad;

    CVirtualKeyboard* iVirtualSctpadFix;
    /**
     * Range selection list control.
     * Not own.
     */     
    CPeninputFingerHwrRangeSelector* iRangeSelector;

    /**
     * Ime selection list control.
     * Not own.
     */     
    CPeninputLayoutInputmodelChoice* iInputModeSwitch;
    
    /**
     * L&F manager.
     * Own.
     */
    CPeninputFingerHwrLAFManager* iLafManager;    

    /**
     * First time construct flag
     */
    TBool iFirstTimeConstruct;


    /**
     * rect of range selection control.
     */
    TRect iRectOfRangeBtn;
    
    /**
     * Current char range id, including SCT.
     */
    TInt iCurCharRange;
    
    /**
     * Current char range id, without SCT.
     */
    TInt iCurCharRangeNoSct;
    
    /**
     * bit mask of permitted char ranges.
     */
    TInt iPermittedRanges;
    

    /**
     * flag.
     */
    TBool iInEditWordQueryDlg;
    
    TRgb iWritingBoxBkColor;
    TRgb iWritingBoxFrameColor;
    };

#endif // C_PENINPUTFINGERHWRWND_H
// End Of File