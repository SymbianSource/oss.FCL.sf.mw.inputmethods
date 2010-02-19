/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
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


#ifndef C_PENINPUTFINGERHWRARWND_H
#define C_PENINPUTFINGERHWRARWND_H

// INCLUDES
#include <peninputlayoutctrlgroup.h>

// FORWARD DECLARATIONS
class CFepLayoutMultiLineIcf;
class CAknFepCtrlEventButton;
class CAknFepCtrlRepeatButton;
class CFepCtrlDropdownList;
class CPeninputLayoutInputmodelChoice;
class CTransparentHwrWndExt;
class CPeninputFingerHwrArLafManager;
class CPeninputArabicFingerHwrSymbolTable;
class CPeninputArabicFingerHwrIndicator;

/**
 *  class CPeninputFingerHwrArWnd.
 *
 *  Main window of finger hwr arabic. 
 *
 *  @lib peninputfingerhwrar.lib
 *  @since S60 v5.0
 */
class CPeninputFingerHwrArWnd : public CControlGroup
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
     * @return Pointer to created CPeninputFingerHwrArWnd object
     */
    static CPeninputFingerHwrArWnd* NewL( CFepUiLayout* aFepUiLayout,
            TInt aControlId, TBool aLandscapeStyle );

    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * 
     * @param aUiLayout The layout
     * @param aControlId The control id
     * @param aLandscapeStyle Current screen is landscape or not
     * @return Pointer to created CPeninputFingerHwrArWnd object
     */
    static CPeninputFingerHwrArWnd* NewLC( CFepUiLayout* aFepUiLayout,
            TInt aControlId, TBool aLandscapeStyle );

    /**
     * standard c++ destructor.
     *
     * @since S60 v5.0
     * @return None
     */
    virtual ~CPeninputFingerHwrArWnd();

    
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
     * @return None
     */
    void SizeChangedL();
	
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
     * set guide line on or off. 
     * 
     * @since S60 v5.0
     * @return none
     */
    void SetGuideLineOn( const TBool aGuideLineOn);
    
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

public: // show symbol table page
    
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
    
public: //child control access
    
    /**
     * retrieve the ICF control. 
     * 
     * @since S60 v5.0
     * @return pointer to ICF
     */
    CFepLayoutMultiLineIcf* Icf();

    
    /**
     * retrieve the candicate list control. 
     * 
     * @since S60 v5.0
     * @return pointer to candicate list 
     */    
    CFepCtrlDropdownList* CandidateList();
    
    /**
     * retrieve the number candicate list control. 
     * 
     * @since S60 v5.0
     * @return pointer to candicate list 
     */    
    CFepCtrlDropdownList* NumCandidateList();

public: //ui
    
    /**
     * update layout
     * safe version of UpdateLayoutL()
     * 
     * @since S60 v5.0
     * @param aIsStandby ETrue if is standby state
     * @return none
     */
    void UpdateLayout( const TBool aIsStandby, 
                       const TBool aReloadCandicate=ETrue );
    
    /**
     * update layout
     * 
     * @since S60 v5.0
     * @param aIsStandby ETrue if is standby state
     * @return none
     */    
    void UpdateLayoutL( const TBool aIsStandby, 
                        const TBool aReloadCandicate=ETrue);

    /**
     * dim arrow buttons
     * 
     * @since S60 v5.0
     * @param aDimArrow ETrue to dim arrow buttons, EFalse to set arrow buttons as not dimmed. 
     * @return none
     */
    void DimArrowKeys( TBool aDimArrow );
    
public: // symbol table & candidate list    
    /**
     * retrieve char range of layout, including sct.
     * 
     * @since S60 v5.0
     * @return char range id.
     */
    TInt CurrentCharRange();
    
    /**
     * Show preview bubble
     * 
     * @since S60 v5.0
     * @param aShow Show/Hide flag
     * @return none
     */
    void ShowBubble(TInt aShow);
	
	/**
     * Open the symble table
     * 
     * @since S60 v5.0
     * @return none
     */
    void OpenSymbolTable();
    
	/**
     * Close the symble table
     * 
     * @since S60 v5.0
     * @return none
     */
	void CloseSymbolTable();
	
	/**
     * Check if the symbol table is alreay opened up
     * 
     * @since S60 v5.0
     * @return none
     */
    TBool IsSymbolTableShowingUp();
	
	/**
     * Check if the candidate list was opened up
     * 
     * @since S60 v5.0
     * @return none
     */
	TBool IsCandidateShowup();

    /**
     * Hide the indicator of handwriting
     * @since S60 v5.0
     * @return none	 
     */
    void HideIndicator();
	
	/**
	 * Get the char before the cursor
	 *
	 * @param aCharPos The the char position
	 * @param aCharBeforeCursor The char to be retrieved
	 * @since S60 v5.0
     * @return TBool ETrue if char have been retrieved, EFalse otherwise
	 */
	TBool GetCharBeforeCursor(TInt aCharPos, TUint16& aCharBeforeCursor); 
	
protected:

    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @param aFepUiLayout A Ui Layout environment (CFepUiLayout)
     * @param aControlId Control id
     * @return None
     */
    CPeninputFingerHwrArWnd( CFepUiLayout* aFepUiLayout, TInt aControlId );

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
     * create number candidate lists.
     * 
     * @since S60 v5.0
     * @return None
     */
    void CreateNumCandidateListL();
    
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
     * create virtual sct pad.
     * 
     * @since S60 v5.0
     * @return None
     */    
    void CreateSymbolTableL();
    
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
    void LoadVkbKeyImageL( const TInt aResId, const TSize& aKeySize );
    
    
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
    void SwitchToStandbyView();
    
    /**
     * layout for symbol range standby.
     * 
     * @since S60 v5.0
     * @return None
     */    
    void SwitchToSymbolTableView();

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
	 * Calcaulate the postion of guideline
	 * @since S60 v5.0
	 * @return None
	 */
    void CalculateGuideLinePos();
	
	/**
	 * Draw the guideline
	 * @since S60 v5.0
	 * @return None
	 */
    void DrawGuideLine();
    
private: // datas

    /**
     * Input context field control.
     * Not own.
     */
    CFepLayoutMultiLineIcf* iContextField;


    /**
     * Candidate list control.
     * Not own.
     */    
    CFepCtrlDropdownList* iCandidateList;
    
    
    /**
     * number Candidate list control.
     * Not own.
     */    
    CFepCtrlDropdownList* iNumCandidateList;
    /**
     * Writing box control.
     * Not own.
     */
    CTransparentHwrWndExt* iWritingBox;

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
    CAknFepCtrlEventButton* iSymbolTableBtn;
    
    /**
     * Symbol table control.
     * Not own.
     */     
    CPeninputArabicFingerHwrSymbolTable* iSymbolTable;
	
    /**
     * Ime selection list control.
     * Not own.
     */     
    CPeninputLayoutInputmodelChoice* iInputModeSwitch;
    
    /**
     * L&F manager.
     * Own.
     */
    CPeninputFingerHwrArLafManager* iLafManager;    

    /**
     * First time construct flag
     */
    TBool iFirstTimeConstruct;
    
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
	 * Right Bottom point of guideline
	 */
	TPoint iGuideLineBottomTl;
	
	/**
	 * Right Bottom point of guideline
	 */
    TPoint iGuideLineBottomBr;
    
	/**
	 * Check if the symbol table has been opened up
	 */
	TBool iIsSymbolTableShowingUp;

    /**
     * Check if the guideline is switched on
     */	 
    TBool iGuideLineOn;

	/**
	 * Handwring indicator
	 */
	CPeninputArabicFingerHwrIndicator* iHandwritingIndicator; 
    };

#endif // C_PENINPUTFINGERHWRARWND_H
// End Of File
