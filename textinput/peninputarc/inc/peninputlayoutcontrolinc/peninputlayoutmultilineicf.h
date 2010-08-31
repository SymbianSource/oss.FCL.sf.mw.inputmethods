/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  header file of base class for Edwin-like editing area
*
*/

#ifndef C_PENINPUTLAYOUTMULTILINEICF_H
#define C_PENINPUTLAYOUTMULTILINEICF_H

#include "peninputlayoutctrlgroup.h"
#include <coecntrl.h>
#include <AknsItemID.h>
#include <gdi.h>

class CRichText;
class CParaFormatLayer;
class CCharFormatLayer;
class CTextLayout;
class CTextView;
class CCoeEnv;
class TCharFormat;
class TCharFormatMask;
class CInsertionPoint;
class CFepLayoutMIcfCustomDraw;
class CFepLayoutMIcfCustomWrap;
class CBubbleCtrl;
class CFepUiCursor;

class CAknEdwinFormExtendedInterfaceProvider;
class CAknInlineTextSource;

class CFepLayoutMultiLineIcfEditor;
class CTooltipBubbleCtrl;
class CPeninputSmileyManager;


class CFepLayoutMultiLineIcf : public CControlGroup, public MEventObserver
    {
public:
    enum TArrowBtnEvent
        {
        EArrowLeft,
        EArrowRight,
        EArrowUp,
        EArrowDown
        };

    enum TIcfState
        {
        EIcfNormal,
        EIcfInline,
        EIcfAutoComplete
        };
public:  

    IMPORT_C static CFepLayoutMultiLineIcf* NewL(TRect aRect,
                                                 CFepUiLayout* aUiLayout,
                                                 TInt aControlId,
                                                 TInt aFontHeight,
                                                 TInt aMaxFontHeight,
                                                 const CFont* aFont);

    // EAttColor of charformat
    IMPORT_C virtual void SetTextColorL(const TRgb aColor);

    IMPORT_C virtual void SetAutoCompleteTextColor(const TRgb aColor);

    // EAttFontHighlightColor of charformat
    IMPORT_C virtual void SetTextSelColorL(const TRgb aColor);

    // Tests if the field has focus. 
    // return ETrue if it has focus, otherwise EFalse
    IMPORT_C virtual TBool IsFocused();

    // Sets the text to be displayed
    // cursorSel Current cursor selection
    // aText The text to be set in fep ui
    IMPORT_C virtual void SetTextL(const TFepInputContextFieldData& aData);
            
    // Sets dim
    // aDimFlag Dim flag, ETrue for dim
    IMPORT_C virtual void SetDimmed(TBool aDimFlag);

    IMPORT_C virtual void SetFontL(TInt aFontHeight, TInt aMaxFontHeight, const CFont* aFont);

    // Set editor focus
    IMPORT_C virtual void SetFocus(TBool aFlag = ETrue);    

    // Set cursor visibility
    // aFlag ETrue for visisble, EFalse for invisible
    IMPORT_C virtual void SetCursorVisible( TBool aCursorVisibleFlag = ETrue );    

    // Get cursor visibility
    // return ETrue for visisble, EFalse for invisible
    IMPORT_C virtual TBool CursorVisible() const;

    IMPORT_C void SetRect(const TRect& aRect);

    IMPORT_C virtual void SizeChangedL(const TRect& aRect,
                               TInt aFontHeight,
                               TInt aMaxFontHeight,
                               const CFont* aFont);

    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
    
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerMoveEventL(const TPoint& aPoint);

    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);
    
    IMPORT_C virtual void CancelPointerDownL();
    
    IMPORT_C virtual void HandlePointerLeave(const TPoint& aPoint);
    
    IMPORT_C virtual void HandleArrowBtnEventL(TArrowBtnEvent aEventType);
    
    IMPORT_C virtual void UpdateValidRegion(CFepUiBaseCtrl* aCtrl,TBool aRemoveFlag);
    
    IMPORT_C TBool Contains(const TPoint& aPt);

    IMPORT_C virtual void SetOverLapState(TBool aSupportOverlap, TRect aRect = TRect(0, 0, 0, 0));
    
    IMPORT_C virtual void AddOverLapState(TRect aRect);

    IMPORT_C virtual TInt GetEditorFieldMaxLen();
    
    IMPORT_C virtual void SetTextAlignmentL(TInt aAlignment, TInt aLanguage);

    // Move conntrol, aOffset The offset to be moved
    IMPORT_C void Move(const TPoint& aOffset);

    IMPORT_C virtual void SetPromptTextL(const TDesC& aPromptText, TBool aCleanContent = ETrue );
    
    IMPORT_C virtual const HBufC* PromptText();


    IMPORT_C virtual void InlineTextPos(TPoint& aPosBelow, TPoint& aPosRight);

    IMPORT_C virtual void EnableTextSelection(TBool aEnable);
    
    IMPORT_C virtual void SetTextIsSecret(TBool aIsSecret);

    IMPORT_C virtual TBool TextIsSecret() const;
    
    // ICF using frame as bg image
    IMPORT_C virtual void SetBgImgSkinId(TAknsItemID aSkinId);

    TAknsItemID BgImgSkinId() const;

    // set whether there is line separator between prompt text and normal text
    IMPORT_C  virtual void SetLineSeparatorAfterPrompt(TBool aLineSeparator);
    
    IMPORT_C  virtual void SetLanguageId( TInt aLanguageId );
    
    IMPORT_C virtual TBool NomatchState();
    
    IMPORT_C virtual TBool IsEmpty();

    IMPORT_C virtual TBool InlineStateOn();
    
    IMPORT_C virtual void SetTextMargin
        (TInt aLeftMargin, TInt aRightMargin, TInt aTopMargin, TInt aBottomMargin);
    
    IMPORT_C virtual void SetLineSpace( TInt aLineSpace );
    
    IMPORT_C virtual void ExtractText(TDes &aBuf, TInt aPos, TInt aLength);

    IMPORT_C virtual CBubbleCtrl* MsgBubbleCtrl();
    IMPORT_C virtual CBubbleCtrl* InfoBubbleCtrl();
    IMPORT_C virtual void ShowBubble(const TDesC& aText, const TRect& aRect);
    IMPORT_C virtual void ShowByteWarningBubble(const TDesC& aInfo);
    IMPORT_C virtual void HideBubble();
    IMPORT_C virtual void HideInfoBubble(); 
    IMPORT_C virtual void SetMsgBubbleCtrlSize(const TSize& aSize);
    IMPORT_C virtual void SetInfoBubbleCtrlSize(const TSize& aSize);
    
    IMPORT_C virtual void SetBackgroundBitmapL(CFbsBitmap* aBmp);
    
    void DrawIcfBackground( CFbsBitGc& aGc, const TRect& aDrawnRect, 
                            TBool aBmpStretchFlag = ETrue );
    IMPORT_C virtual void UpdateSecretTextL();
    
    IMPORT_C virtual void ReDrawRect(const TRect& aRect);
    
    IMPORT_C void SetNumberGrouping(TBool aIsNumberGrouping);

    /**
     * Return the pointer to Toolitp control
     * 
     * @return the pointer to Toolitp control
     */
    IMPORT_C CTooltipBubbleCtrl* TooltipBubbleCtrl();

    /**
     * Show tooltip of ICF
     * 
     * @param aText The text to be displayed in ICF
     * @return None
     */
    IMPORT_C void ShowTooltipL( const TDesC& aText );
    

    /**
     * Hide tooltip of ICF
     * 
     * @param aText The text to be displayed in ICF
     * @param aPos The position of tooltip
     * @return None
     */
    IMPORT_C void HideTooltip();
    
protected:  // Methods

    /**
     * Handle control event 
     *
     * @since S60 v4.0
     * @param aEventType The event type
     * @param aCtrl The control who sends the event
     * @param aEventData The event data
     */            
    IMPORT_C virtual void HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl,
                                                const TDesC& aEventData);

    IMPORT_C virtual void PrepareForFocusLoss();

    IMPORT_C virtual TBool PrepareForFocusGain();    

    IMPORT_C CFepLayoutMultiLineIcf(CFepUiLayout* aUiLayout,
                           TInt aControlId);

    IMPORT_C void BaseConstructL(TRect aRect,
                                 TInt aFontHeight, 
                                 TInt aMaxFontHeight, 
                                 const CFont* aFont);

private:    // Methods
	/**
	 * Find what is the control of aPoint. 
	 *	 
	 * @param aPoint The position to be checked
	 */
	CFepUiBaseCtrl* ControlForPoints( const TPoint& aPoint );
	
private:  // Data
    CFepLayoutMultiLineIcfEditor* iIcfEditor;
    CBubbleCtrl* iMsgBubble;        
    CTooltipBubbleCtrl* iTooltip;
    CBubbleCtrl* iInfoBubble; 
    };


NONSHARABLE_CLASS(CFepLayoutMultiLineIcfEditor) : public CFepUiBaseCtrl
    {
public:

    IMPORT_C static CFepLayoutMultiLineIcfEditor* NewL(TRect aRect,
                                                 CFepUiLayout* aUiLayout,
                                                 TInt aControlId,
                                                 TInt aFontHeight,
                                                 TInt aMaxFontHeight,
                                                 const CFont* aFont);

    virtual ~CFepLayoutMultiLineIcfEditor();

    // EAttColor of charformat
    void SetTextColorL(const TRgb aColor);

    void SetAutoCompleteTextColor(const TRgb aColor);

    // EAttFontHighlightColor of charformat
    void SetTextSelColorL(const TRgb aColor);

    // Tests if the field has focus. 
    // return ETrue if it has focus, otherwise EFalse
    TBool IsFocused();

    // Sets the text to be displayed
    // cursorSel Current cursor selection
    // aText The text to be set in fep ui
    void SetTextL(const TFepInputContextFieldData& aData);
            
    // Sets dim
    // aDimFlag Dim flag, ETrue for dim
    void SetDimmed(TBool aDimFlag);

    void SetFontL(TInt aFontHeight, TInt aMaxFontHeight, const CFont* aFont);

    // Set editor focus
    void SetFocus(TBool aFlag = ETrue);    

    // Set cursor visibility
    // aFlag ETrue for visisble, EFalse for invisible
    void SetCursorVisible(TBool aCursorVisibleFlag = ETrue);    

    // Get cursor visibility
    // return ETrue for visisble, EFalse for invisible
    TBool CursorVisible() const;

    void Draw();
	
	void ReDrawRect(const TRect& aRect);
	
    void SetRectL(const TRect& aRect);

    void SizeChangedL(const TRect& aRect,
                               TInt aFontHeight,
                               TInt aMaxFontHeight,
                               const CFont* aFont);

    virtual CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
    
    virtual CFepUiBaseCtrl* HandlePointerMoveEventL(const TPoint& aPoint);

    virtual CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);
    
    virtual void CancelPointerDownL();
    
    virtual void HandlePointerLeave(const TPoint& aPoint);
    
    void OnActivate();
    
    void OnDeActivate();
            
    void HandleArrowBtnEventL(CFepLayoutMultiLineIcf::TArrowBtnEvent aEventType);
    
    void UpdateValidRegion(CFepUiBaseCtrl* aCtrl,TBool aRemoveFlag);
    
    TBool Contains(const TPoint& aPt);

    void SetOverLapState(TBool aSupportOverlap, TRect aRect = TRect(0, 0, 0, 0));
    
    void AddOverLapState(TRect aRect);

    virtual TInt GetEditorFieldMaxLen();
    
    void SetTextAlignmentL(TInt aAlignment, TInt aLanguage);

    // Move conntrol, aOffset The offset to be moved
    void Move(const TPoint& aOffset);

    void SetPromptTextL(const TDesC& aPromptText, TBool aCleanContent );
    
    const HBufC* PromptText();


    void InlineTextPos(TPoint& aPosBelow, TPoint& aPosRight);

    inline void EnableTextSelection(TBool aEnable);
    
    inline void SetTextIsSecret(TBool aIsSecret);

    inline TBool TextIsSecret() const;
    
    // ICF using frame as bg image
    inline void SetBgImgSkinId(TAknsItemID aSkinId);

    TAknsItemID BgImgSkinId() const;

    // set whether there is line separator between prompt text and normal text
    inline void SetLineSeparatorAfterPrompt(TBool aLineSeparator);
    
    inline void SetLanguageId( TInt aLanguageId );
    
    inline TBool NomatchState();
    
    IMPORT_C TBool IsEmpty();

    inline TBool InlineStateOn();
    
    inline void SetTextMargin
        (TInt aLeftMargin, TInt aRightMargin, TInt aTopMargin, TInt aBottomMargin);
    
    inline void SetLineSpace( TInt aLineSpace );
    
    IMPORT_C void ExtractText(TDes &aBuf, TInt aPos, TInt aLength);

    IMPORT_C CBubbleCtrl* MsgBubbleCtrl();
    IMPORT_C CBubbleCtrl* InfoBubbleCtrl();
    IMPORT_C void ShowBubble(const TDesC& aText, const TRect& aRect);
    IMPORT_C void ShowByteWarningBubble(const TDesC& aInfo);
    IMPORT_C void HideBubble();
    IMPORT_C void HideInfoBubble();
    IMPORT_C void SetMsgBubbleCtrlSize(const TSize& aSize);
    IMPORT_C void SetInfoBubbleCtrlSize(const TSize& aSize);
    
    // For addition of ITI features on FSQ
    void ShowTooltipL(const TDesC& aText);
    void HideTooltip();
    /**
     * Returen the auto-completion state
     * 
     * @return ETrue: AutoCompletion state is On
     *         EFalse: AutoCompletion state is Off
     */
    inline TBool AutoCompletionStateOn() const;

    
    void DrawIcfBackground( CFbsBitGc& aGc, const TRect& aDrawnRect, 
                            TBool aBmpStretchFlag = ETrue );
    inline void SetMsgBubble(CBubbleCtrl* aCtrl);
    inline void SetInfoBubble(CBubbleCtrl* aCtrl);
    inline void SetTooltip(CTooltipBubbleCtrl* aCtrl);
    
    IMPORT_C virtual void PrepareForFocusLoss();

    IMPORT_C virtual TBool PrepareForFocusGain();    

    /**
     * Handle pen UI graphic device size change event
     *
     * @since S60 V5.0
     */
    virtual IMPORT_C void GraphicDeviceSizeChanged();
    void UpdateSecretTextL();
    
    void SetNumberGroupingL(TBool aIsNumberGrouping);
    
    /**
     * remove padding in custom draw text.
     * 
     * @since S60 v5.0 
     * @param aText text to be drawn
     * @return None
     */
    void TrimCustomDrawText( TDes& aText );
    
    /**
     * test if smiley is enabled
     * 
     * @since S60 v5.0
     * @return ETrue if smiley is enabled
     */
    TBool SmileyEnabled();
    
    /**
     * Get smiley manager
     *  
     * @since S60 v5.0
     * @return smiley manager
     */
    CPeninputSmileyManager* SmileyManager();
    
    
    /**
     * Calculate draw region, excluding smiley icons   
     * 
     * @since S60 v5.0
     * @param aRgn clip region
     * @param aGc graphic context 
     * @param aFont font to draw text
     * @param aText text to be drawn
     * @param aBasePoint base point if text (left,bottom)
     * @param aTopLine top margin of text
     * @param aBottomLine bottom margin of text
     */
    void CalcSmileyClipRegionL( RRegion& aRgn, CGraphicsContext& aGc, 
                                CFont& aFont, const TDesC& aText, 
                                TPoint& aBasePoint,
                                TInt aTopLine, TInt aBottomLine );
    
    /**
     * Draw smiley icons, called by custom draw interface 
     * 
     * @since S60 v5.0
     * @param aGc graphic context 
     * @param aFont font to draw text
     * @param aText text to be drawn
     * @param aBasePoint base point if text (left,bottom)
     * @param aTopLine top margin of text
     * @param aBottomLine bottom margin of text
     */
    void CustomDrawSmileyL( CGraphicsContext& aGc, CFont& aFont,  
                            const TDesC& aText, TPoint& aBasePoint, 
                            TInt aTopLine, TInt aBottomLine );
    
private:    // Methods

    CFepLayoutMultiLineIcfEditor(TRect aRect,
                           CFepUiLayout* aUiLayout,
                           TInt aControlId);

    void BaseConstructL(TInt aFontHeight, 
                        TInt aMaxFontHeight,
                        const CFont* aFont);

    void InitTextLayoutL();

    // set text margin to view rect
    //void SetTextMarginL();

    void RecalcualteTextPositionL();
                           
    inline TBool TextSelectionOn();

    void SetInlineStateL(TBool aInline, 
                         TInt aStartPos = 0, 
                         TInt aEndPos = 0, 
                         TBool aNoMatchState = EFalse, 
                         const TDesC& aNoMatchText = KNullDesC);

    void SetAutoCompleteStateL(TBool aAutoComplete, 
                               TInt aStartPos = 0, 
                               TInt aMidPos = 0,
                               TInt aEndPos = 0);

    void SetSpecialStateL(TBool aStateOn, 
                          TInt aStartPos, 
                          TInt aEndPos, 
                          TInt& aPreStartPos,
                          TInt& aPreEndPos,
                          TTextFormatAttribute aAttribute,
                          TBool aNoMatchState = EFalse,
                          const TDesC& aNoMatchText = KNullDesC);

    inline TBool AutoCompleteStateOn();
    
    /**
     * Save current cursor state
     *
     * @since S60 v4.0
     * @param aCtrl The control which may affects the cursor shown state
     */    
    void SaveCursorState(CFepUiBaseCtrl* aCtrl);
    
    /**
     * Restore cursor state when overlapped area is removed.
     *
     * @since S60 v4.0
     * @param aCtrl The control which may affects the cursor shown state     
     */    
    void RestoreCursorState(CFepUiBaseCtrl* aCtrl);        

    /**
     * Restore cursor state when overlapped area is removed.
     *
     * @since S60 v4.0
     * @param aIndex The control index in the list which may affects the cursor shown state     
     */    
    void RestoreCursorState(TInt aIndex);        

    /**
     * Find whether the given control is one of the overlapped control.
     *
     * @since S60 v4.0
     * @return The index in the list. KErrNotFound if not fouond
     */    
    TInt FindOverlappedCtrl(CFepUiBaseCtrl* aCtrl);
    
    TBool IsTextPosValid(TInt aPos);
    
    void UpdateTextL(const TFepInputContextFieldData& aData);
    
    static TInt UpdateSecretTextL(TAny* aEditArea);
    
    void SetSelectionL(TCursorSelection aCurSel);

    void UpdateNoMatchTextL(TInt aPos, const TDesC& aNoMatchText, TBool aNoMatchState);
    
    void RemoveOverlappedCtrlIfNeeded();

    void DeletePromptTextL();

    void RecalculatePosByNewPromptTextL(const TDesC& aNewPromptText);

    void AdjustIcfDataForPromptText(TFepInputContextFieldData& aData);

    // used when need to set cursor pos to fep
    void AdjustFepCursorPosForPromptText(  TBool aSyncCursor = EFalse );
    
    // judge if a given pos belong to prompt text
    TBool BelongToPromptText(TInt aPos);

    void SetPromptTextFormatL(TInt aPromptTextLen);

    void SetCursorSizeAndType();

    TFontSpec CursorFontSpec();
    
    // convert current cursor pos to TPoint, aCaretPos carry 
    // the TPoint info on return
    void CalculateCursorPos(TPoint& aCaretPos);
    
    void AdjustBmpDeviceIfNeeded();
    
    void ApplyFormatL();
    
    void AdjustPromptTextL();
    
    TInt TextWidth( const TDesC& aText );
    
    void ResetApplyFont();
    
    void TryDisplayMaxTextL(TInt aCursorPos);
    
    TBool MoveCursorL( const TPoint& aPt, TInt& aCursorPos );
    
    void ResetViewHeightL();
    
    void SetMfneTextL(const TFepInputContextFieldData& aData);
    
    void DrawMfneText();
    
    void SetMsgBubbleRect();
    void SetInfoBubbleRect();
    
    void HideBubbleTemp();
    
    TBool HighlightOverlapWithBubble( const TRect& aRect );
    
    void AdjustSelectionL( const TCursorSelection& aCurSel );
    
    void ApplyLineSpaceFormatL( TInt aLineHeight );
    void CalcPromptTextRectL();        
            
    inline void SetCursorSelVisibility( TBool aCursorSelVisible );

private:  // Data

    struct TOverlappedInfo
        {
        /*
         * The control overlapped with ICF cursor
         */
        CFepUiBaseCtrl* iCtrl;

        /*
         * keep the cursor on state.
         */            
        TBool iCursorStateBeforeOverlapped;
        } ;

    /**
     * list of controls which overlapped with ICF
     */    
    RArray<TOverlappedInfo> iOverlappedCtrlList;
    
    TBool iHasFocus;

    // The control which captures the pointer before
    CFepUiBaseCtrl* iPreCaptureCtrl;

    // Cursor visibility
    TBool iCursorVisible;

    // Cursor selection
    TBool iCursorSelVisible;

    // Editor is dimmed
    TBool iDimmed;

	CRichText* iRichText;
	CParaFormatLayer* iParaFormatLayer;
	CCharFormatLayer* iCharFormatLayer;
	// text layout and view stuff
	CTextLayout* iLayout;
	CTextView* iTextView;

	TBool iEnableTextSelection;
	RWindow* iWin;
	CCoeEnv* iCoeEnv;

	TRgb iTextColor;
	TRgb iTextSelColor;
	TRgb iTextAutoColor;

	TCharFormat iCharFormat;
	TCharFormatMask iCharFormatMask;

	TRect iViewRect;
	TBool iTextSelectionOn;
    RArray<TRect> iOverLapRects;
    TBool iSupportOverLap;

    TBool iTextIsSecret;
    TBool iAutoComplete;

    CFepUiCursor* iInsertionPoint;

    TBool iTextInited;
    TInt iMaxFontHeight;
    TInt iFontHeight;

    CPeriodic* iSecretTextTimer;

    TBool iPntDownOnInline;

    TInt iPreInlineStartPos;
    TInt iPreInlineEndPos;
    TInt iPreAutoStartPos;
    TInt iPreAutoEndPos;
    TInt iPreTextSelStartPos;
    TInt iPreTextSelEndPos;
    TAknsItemID iBgSkinId;
    TBool iSkinIdSet;

    CFepLayoutMIcfCustomDraw* iCustomDraw;
    CFepLayoutMIcfCustomWrap* iTextWrapper;
    
    TBool iNoMatchState;
    const CFont* iFont;
    HBufC* iInitPromptText;
    HBufC* iPromptText;
    TInt iPromptTextLen;
    TBool iLineSeparator;
    TInt iHorizontalMargin;

    // following member variable used to calculate curosr position
	TInt iWeight;                           // width of vertical cursors, height of horizontal ones

	TBool iLeadingEdge;
	
	TCursorSelection iCursorSel;
    
    /**
     * The last cursor position
     */
    TInt iLastCursorPos;
    
    TInt iState;
    
    TInt iLeftMargin;
    CBubbleCtrl* iMsgBubble;
    CBubbleCtrl* iInfoBubble;
    TInt iRightMargin;
    TInt iTopMargin;
    TInt iBottomMargin;
    
    TInt iLineSpace;
    TSize iLastDeviceSize;
    TBool iFormatChange;
    TInt iFormatLineHeight;
    HBufC* iMfneIcfData;
    TBool iRtlMfneEditor;
    
    TInt iLineNumber;
    TInt iLineMaxCount;
    
    TBool iHideBubbleTemp;
    TBool iHideInfoBubbleTemp;
    TBool iSycnFepData;
	TInt iPreLanguageID;
	
	CGraphicsContext::TTextAlign iTextAlign;
	TInt iAlignment;
	
	CAknEdwinFormExtendedInterfaceProvider* iFormExtendedInterfaceProvider;
	CAknInlineTextSource* iPhoneNumberFormatter;
	
	TInt iOffset;
    TRect iPromptTextRect;
	TInt iMaxIcfEditorHeight;
	TBool iSynCursor;
	RBuf iOldMfneText;
	CTooltipBubbleCtrl* iTooltip;
	
	/**
	 * Smiley manager
	 * own
	 */
	CPeninputSmileyManager* iSmileyManager;
    };
    
inline void CFepLayoutMultiLineIcfEditor::EnableTextSelection(TBool aEnable)
    {
    iTextSelectionOn = aEnable;
    }

inline TBool CFepLayoutMultiLineIcfEditor::TextSelectionOn()
    {
    return iTextSelectionOn;
    }

inline TBool CFepLayoutMultiLineIcfEditor::InlineStateOn()
    {
    return iState == CFepLayoutMultiLineIcf::EIcfInline;
    }
    
inline void CFepLayoutMultiLineIcfEditor::SetTextIsSecret(TBool aIsSecret)
    {
    iTextIsSecret = aIsSecret;
    }

inline TBool CFepLayoutMultiLineIcfEditor::TextIsSecret() const
    {
    return iTextIsSecret;
    }

inline TBool CFepLayoutMultiLineIcfEditor::AutoCompleteStateOn()
    {
    return iAutoComplete;
    }

inline void CFepLayoutMultiLineIcfEditor::SetBgImgSkinId(TAknsItemID aSkinId)
    {
    iBgSkinId = aSkinId;
    iSkinIdSet = ETrue;
    }

inline void CFepLayoutMultiLineIcfEditor::SetLineSeparatorAfterPrompt(TBool aLineSeparator)
    {
    if ( aLineSeparator )
        {
        iLineSeparator = aLineSeparator;
        }
    else
        {
        iLineSeparator = ( iPreLanguageID == ELangUrdu || iPreLanguageID == ELangArabic || 
                       iPreLanguageID == ELangFarsi || iPreLanguageID == ELangHebrew );
        }
    }

inline void CFepLayoutMultiLineIcfEditor::SetLanguageId( TInt aLanguageId )
    {
    if (iPreLanguageID == aLanguageId)
        return;
    iPreLanguageID = aLanguageId;
    iLineSeparator = ( aLanguageId == ELangUrdu || aLanguageId == ELangArabic || 
                       aLanguageId == ELangFarsi || aLanguageId == ELangHebrew );
    }

inline TBool CFepLayoutMultiLineIcfEditor::NomatchState()
    {
    return iNoMatchState;
    }

inline void CFepLayoutMultiLineIcfEditor::SetTextMargin
    (TInt aLeftMargin, TInt aRightMargin, TInt aTopMargin, TInt aBottomMargin)
    {
    if( iLeftMargin == aLeftMargin &&
        iRightMargin == aRightMargin &&
        iTopMargin == aTopMargin &&
        iBottomMargin == aBottomMargin )
        {
        return;        
        }

    //iFormatChange = ETrue;
    iLeftMargin = aLeftMargin;
    iRightMargin = aRightMargin;
    iTopMargin = aTopMargin;
    iBottomMargin = ( aBottomMargin >= 0 )? aBottomMargin : 0;
    }

inline void CFepLayoutMultiLineIcfEditor::SetLineSpace( TInt aLineSpace )    
    {
    if( iLineSpace != aLineSpace )   
        {
        iLineSpace = aLineSpace;
        //iFormatChange = ETrue;
        }
    }
    
inline void CFepLayoutMultiLineIcfEditor::SetMsgBubble(CBubbleCtrl* aCtrl)
    {
    iMsgBubble = aCtrl;
    }
    
inline void CFepLayoutMultiLineIcfEditor::SetInfoBubble(CBubbleCtrl* aCtrl)
    {
    iInfoBubble = aCtrl;
    }
inline void CFepLayoutMultiLineIcfEditor::SetTooltip(CTooltipBubbleCtrl* aCtrl)
    {
    iTooltip = aCtrl;
    }

inline TBool CFepLayoutMultiLineIcfEditor::AutoCompletionStateOn() const
    {
    return iState == CFepLayoutMultiLineIcf::EIcfAutoComplete;
    }
    
inline void CFepLayoutMultiLineIcfEditor::SetCursorSelVisibility( TBool aCursorSelVisible )
    {
    iCursorSelVisible = aCursorSelVisible;
    }

#endif // C_PENINPUTLAYOUTMULTILINEICF_H