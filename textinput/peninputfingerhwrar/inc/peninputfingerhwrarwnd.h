/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
class CPeninputFingerHwrArLafManager;
class CPeninputArabicFingerHwrSymbolTable;
class CPeninputArabicFingerHwrIndicator;
class CPeninputArabicFingerHwrWritingWnd;
class CPeninputArabicFingerHwrNumSymbolTable;

/**
 *  class CPeninputFingerHwrArWnd.
 *
 *  Main window of finger hwr arabic. 
 *
 *  @lib peninputfingerhwrar.lib
 *  @since Symbian TB9.2
 */
class CPeninputFingerHwrArWnd : public CControlGroup
    {   
public:

    /**
     * Symbian constructor.
     *
     * @since Symbian TB9.2
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
     * @since Symbian TB9.2
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
     * @since Symbian TB9.2
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
     * @since Symbian TB9.2
     * @return None
     */
    void SizeChangedL();
	
public: // target editor attributes
    /**
     * accept editor's text alignment.  
     * 
     * @since Symbian TB9.2
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
     * @since Symbian TB9.2
     * @param aData editor's text.
     * @return none
     */
    void SetEditorTextL( const TFepInputContextFieldData& aData );
    
    /**
     * accept editor's promopt txt
     * 
     * @since Symbian TB9.2
     * @param aPromoptData editor's promopt text
     * @return none
     */    
    void SetPromptTextL( TUint8* aPromoptData );
    
    /**
     * Dim option button
     * 
     * @since Symbian TB9.2
     * @param aEnable Enable or disable option button
     * @return none
     */    
    void SetEnableSettingBtn(const TBool aEnable);
    
    /**
     * accept editor's bubble text.
     * 
     * @since Symbian TB9.2
     * @param aPromoptData editor's bubble text.
     * @return none
     */       
    void SetBubbleTextL( const TDesC& aInfo );    

public: //hwr 
    
    /**
     * get strokes from hwr writing box
     * 
     * @since Symbian TB9.2
     * @return a TPoint array of strokes
     */
    const RArray<TPoint>& StrokeList();

    /**
     * set end mark of hwr
     * 
     * @since Symbian TB9.2
     * @param aEndMark a TPoint object as end mark
     * @return none
     */
    void SetStrokeEndMark( const TPoint& aEndMark );
    
    /**
     * set guide line on or off. 
     * 
     * @since Symbian TB9.2
     * @return none
     */
    void SetGuideLineOn( const TBool aGuideLineOn);
    
    /**
     * set pen color of hwr writing 
     * 
     * @since Symbian TB9.2
     * @return none
     */
    void SetBoxPenColor( const TInt aPenColor );
    
    /**
     * set pen size of hwr writing 
     * 
     * @since Symbian TB9.2
     * @return none
     */
    void SetBoxPenSize( const TSize aPenSize );
    
    /**
     * set writing speed hwr writing 
     * 
     * @since Symbian TB9.2
     * @return none
     */    
    void SetBoxWritingSpeed( TTimeIntervalMicroSeconds32& aCharDelay );
    
    /**
     * retrieve rect of hwr writingbox.
     * 
     * @since Symbian TB9.2
     * @return rect of hwr writingbox.
     */
    TRect WritingBoxRect();
    
    /**
     * Cancel wrting
     *
     * @since Symbian TB9.2
     * @return None
     */
    void CancelWriting();    

public: // show symbol table page
    
    /**
     * Change symbol table page.
     * 
     * @since Symbian TB9.2
     * @param aPageNo page no.
     * @param aPos Specifies the interpretation of the aPageNo parameter:
            EPagePosPageNo - go to page aPageNo.
            EPagePosNextPage -  go to next page.
            EPagePosPrevPage -  go to previous page.
            EPagePosLastPage -  go to last page.
            EPagePosFirstPage  - go to first page.
     * @return none
     */
    void ShowSymbolPage( TInt aPageNo, TInt aPos );
    /**
     * accept editor's number mapping restriction.
     * 
     * @since S60 v5.0
     * @param aNumMapping a string contains numbers and related char. max length is 16.
     * @return none
     */
    void SetNumericMapping( const TDesC& aNumMapping );


public: //child control access
    
    /**
     * retrieve the ICF control. 
     * 
     * @since Symbian TB9.2
     * @return pointer to ICF
     */
    CFepLayoutMultiLineIcf* Icf();

    
    /**
     * retrieve the candicate list control. 
     * 
     * @since Symbian TB9.2
     * @return pointer to candicate list 
     */    
    CFepCtrlDropdownList* CandidateList();
    
//    /**
//     * retrieve the number candicate list control. 
//     * 
//     * @since Symbian TB9.2
//     * @return pointer to candicate list 
//     */    
//    CFepCtrlDropdownList* NumCandidateList();
	
    /**
     * dim arrow buttons
     * 
     * @since Symbian TB9.2
     * @param aDimArrow ETrue to dim arrow buttons, EFalse to set arrow buttons as not dimmed. 
     * @return none
     */
    void DimArrowKeys( TBool aDimArrow );
    
public: // symbol table & candidate list    
    /**
     * Show preview bubble
     * 
     * @since Symbian TB9.2
     * @param aShow Show/Hide flag
     * @return none
     */
    void ShowBubble(TInt aShow);
	
	/**
     * Open the symble table
     * 
     * @since Symbian TB9.2
     * @return none
     */
    void OpenSymbolTable();
    
	/**
     * Close the symble table
     * 
     * @since Symbian TB9.2
     * @return none
     */
	void CloseSymbolTable();
	
	/**
     * Open the candidate list
     * 
     * @since Symbian TB9.2
     * @return none
     */
	void OpenCandidateList();
	
	/**
     * Close the candidate list
     * 
     * @since Symbian TB9.2
     * @return none
     */
	void CloseCandidateList();
	
	/**
     * Check if the symbol table is alreay opened up
     * 
     * @since Symbian TB9.2
     * @return ETrue if the symbol table was already opened up, EFalse otherwise
     */
    TBool IsSymbolTableShowingUp();
	

    /**
     * Check if the candidate list was opened up
     * 
     * @since Symbian TB9.2
     * @return ETrue if the candidate list was already opened up, EFalse otherwise
     */
	TBool IsCandidateShowup();

    /**
     * Hide the indicator of handwriting
     * @since Symbian TB9.2
     * @return none	 
     */
    void HideIndicator();
	
	/**
	 * Get the char before the cursor
	 *
	 * @param aCharPos The the char position
	 * @param aCharBeforeCursor The char to be retrieved
	 * @since Symbian TB9.2
     * @return TBool ETrue if char have been retrieved, EFalse otherwise
	 */
	TBool GetCharBeforeCursor(TInt aCharPos, TUint16& aCharBeforeCursor); 
	
	/**
	 * Handle window open
	 */
	void HandleWindowOpenCommandL();
    /*
     * set arabic number mode.
     * @since s60 v5.2
     * @param aIsNativeNumMode a flag to indicate whether it is native number mode.
     * @return none.
     */
    void SetNativeNumMode(const TBool aIsNativeNumMode);
    
    /*
     * set icf language.
     * @since s60 v5.2
     * @param aLang language id.
     * @return none.
     */
    void SetIcfLanguage( TInt aLang );    
protected:

    /**
     * C++ constructor
     *
     * @since Symbian TB9.2
     * @param aFepUiLayout A Ui Layout environment (CFepUiLayout)
     * @param aControlId Control id
     * @return None
     */
    CPeninputFingerHwrArWnd( CFepUiLayout* aFepUiLayout, TInt aControlId );

private:

    /**
     * Symbian second-phase constructor
     *
     * @since Symbian TB9.2
     * @param aLandscapeStyle Current screen is landscape or not
     * @return None
     */
    void ConstructL( TBool aLandscapeStyle );

    /**
     * create icf.
     * 
     * @since Symbian TB9.2
     * @return None
     */
    void CreateContextFieldL();

    /**
     * create candidate lists.
     * 
     * @since Symbian TB9.2
     * @return None
     */
    void CreateCandidateListL();
//    /**
//     * create number candidate lists.
//     * 
//     * @since Symbian TB9.2
//     * @return None
//     */
//    void CreateNumCandidateListL();
    
    /**
     * create writing box.
     * 
     * @since Symbian TB9.2
     * @return None
     */    
    void CreateWritingBoxL();
    
    /**
     * create all function buttons. 
     * 
     * @since Symbian TB9.2
     * @return None
     */    
    void CreateButtonsL();
    
    /**
     * create virtual sct pad.
     * 
     * @since Symbian TB9.2
     * @return None
     */    
    void CreateSymbolTableL();
    
    /**
     * create virtual number modesct pad.
     * 
     * @since Symbian TB9.2
     * @return None
     */    
    void CreateNumSymbolTableL();
    
    /**
     * EventButton creation helper.
     * 
     * @since Symbian TB9.2
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
     * @since Symbian TB9.2
     * @param aCtrlId button control id.
     * @param aResId resource id.
     * @param aEvent event id;
     * @param aUnicode a unicode value to be sent
     * @return pointer to created CAknFepCtrlRepeatButton obj. 
     */
    CAknFepCtrlRepeatButton* CreateRepBtnL( const TInt aCtrlId, TInt32 aResId,  
            const TInt aEvent, const TInt aUnicode );
        
    /**
     * load virtual sct keys.
     * 
     * @since Symbian TB9.2
     * @param aResId resource id.
     * @param aCellRects rects of virtual keys.
     * @return None
     */      
    void LoadSymbolVirtualKeysL( const TInt aResId, const RArray<TRect>& aCellRects );

    /**
     * load virtual number mode sct keys.
     * 
     * @since Symbian TB9.2
     * @param aResId resource id.
     * @param aCellRects rects of virtual keys.
     * @return None
     */      
    void LoadNumSymbolVirtualKeysL( const TInt aResId, 
            const RArray<TRect>& aCellRects, TBool aReload = ETrue );
    
    /**
     * relayout full ui, reset all controls position. 
     * 
     * @since Symbian TB9.2
     * @return None
     */    
    void ResetLayoutL();

    /**
     * switch to arabic standby view
     * 
     * @since Symbian TB9.2
     * @return None
     */    
    void SwitchToStandbyView();
    
    /**
     * switch to symbol table view
     * 
     * @since Symbian TB9.2
     * @return None
     */    
    void SwitchToSymbolTableView();

    /**
     * EventButton layout helper. Move button to specified rect.
     * 
     * @since Symbian TB9.2
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
     * @since Symbian TB9.2
     * @param aResId resource id.
     * @return None
     */    
    void ReadShadowInfoL( const TInt aResId );
    
    /**
     * read control's background info.
     *
     * @since Symbian TB9.2
     * @param aResId resource id.
     * @return None
     */
    void ReadBackgroundInfoL( const TInt aResId );
    
    /**
     * read icf appearance info.
     * 
     * @since Symbian TB9.2
     * @param aResId resource id.
     * @return None
     */    
    void ReadICFInfoL( const TInt aResId );
    
    /**
     * read writingbox appearance info.
     * 
     * @since Symbian TB9.2
     * @param aResId resource id.
     * @return None
     */    
    void ReadWritingBoxInfoL( TInt aResId );
    
	/**
	 * Calcaulate the postion of guideline
	 * @since Symbian TB9.2
	 * @return None
	 */
    void CalculateGuideLinePos();
	
	/**
	 * Draw the guideline
	 * @since Symbian TB9.2
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
     * Writing box control.
     * Not own.
     */
    CPeninputArabicFingerHwrWritingWnd* iWritingBox;

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
     * enter button control.
     * Not own.
     */     
    CAknFepCtrlEventButton* iEnterBtn;
    /**
     * space button control.
     * Not own.
     */     
    CAknFepCtrlEventButton* iSpaceBtn;
    
    /**
     * Symbol table control.
     * Not own.
     */     
    CPeninputArabicFingerHwrSymbolTable* iSymbolTable;

    /**
     * Number mode Symbol table control.
     * Not own.
     */     
    CPeninputArabicFingerHwrNumSymbolTable* iNumSymbolTable;
    
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
