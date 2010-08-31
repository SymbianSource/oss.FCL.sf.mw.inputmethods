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
* Description:
*
*/
#include <peninputcmd.h>
#include <coemain.h>
#include <txtrich.h>
#include <frmtlay.h>
#include <frmtview.h>
#include <eiksbfrm.h>
#include <aknappui.h>
#include <AknUtils.h>
#include <w32std.h>
#include <frmtlay.h>
#include <AknsDrawUtils.h>
#include <avkon.hrh>
#include <AknBidiTextUtils.h>
#include <bidivisual.h>
#include <AknFepGlobalEnums.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <tagmalayoutandsource.h>
#endif
#include "peninputlayoutcursor.h"
#include "peninputlayoutbubblectrl.h"

#include "peninputlayoutmultilineicf.h"
#include "peninputlayout.h"

#include "peninputlayoutmicfcustomdraw.h"
#include "peninputlayoutmicfnomatchesinline.h"
#include "AknPhoneNumberInlineTextSource.h"
#include "AknEdwinFormExtendedInterfaceProvider.h"
#include "AknCompositeInlineTextSource.h"
#include "peninputlayoutsmiley.h"

#include <gdi.h>
const TInt KParagraphSeperator = 0x2029;
const TInt KSecretUpdateTimer = 1000000; // 1s
const TInt KSecretInstantShowTimer = 100000; // 100ms

const TUint16 KSegment = 0x0020;
// separator between prompt text and normal text,
// should use localized character soon
const TUint16 KSeparator = 0x003a;
const TUint16 KSeparatorcn = 0xff1a;
_LIT(KLineSeparator, "\n");
const TInt KStar = '*';

const TInt KMaxMfneDataLen = 30;
// Message bubble position adjust
const TInt KMsgBubblePosAdjustValueX = 3;
const TInt KMsgBubblePosAdjustValueY = 2;
const TInt KPromptAdjustLen = 5;
EXPORT_C CFepLayoutMultiLineIcfEditor* CFepLayoutMultiLineIcfEditor::NewL(TRect aRect,
                                                              CFepUiLayout* aUiLayout,
                                                              TInt aControlId,
                                                              TInt aFontHeight,
                                                              TInt aMaxFontHeight,
                                                              const CFont* aFont)
    {
    CFepLayoutMultiLineIcfEditor* self = new (ELeave) CFepLayoutMultiLineIcfEditor(aRect, 
                                                                       aUiLayout, 
                                                                       aControlId);
    CleanupStack::PushL(self);
    self->BaseConstructL(aFontHeight, aMaxFontHeight, aFont);
    CleanupStack::Pop(self);

    return self;
    }

CFepLayoutMultiLineIcfEditor::CFepLayoutMultiLineIcfEditor(TRect aRect,
                                               CFepUiLayout* aUiLayout,
                                               TInt aControlId)
    :CFepUiBaseCtrl(aRect,aUiLayout,aControlId),
     iTextSelectionOn(ETrue),
     iTextInited(EFalse),
     iPntDownOnInline(EFalse),
     iPreInlineStartPos(0),
     iPreInlineEndPos(0),
     iPreAutoStartPos(0),
     iPreAutoEndPos(0),
     iPreTextSelStartPos(0),
     iPreTextSelEndPos(0),
     iSkinIdSet(EFalse),
     iNoMatchState(EFalse),
     iLineSeparator( EFalse ),
     iLeadingEdge(EFalse),
     iState(CFepLayoutMultiLineIcf::EIcfNormal)
    {
    SetControlType(ECtrlEditAreaBase);
    iCursorVisible = ETrue;
    iCursorSelVisible = ETrue;
    
#ifdef RD_TACTILE_FEEDBACK     
    //register the area for tactile feedback
    if(aUiLayout)
    	{
    	//Advanced Tactile feedback REQ417-47932
    	SetTactileFeedbackType(ETouchFeedbackSensitiveKeypad);    	
    	aUiLayout->RegisterFeedbackArea(reinterpret_cast<TInt>(this),aRect,ETouchFeedbackSensitiveKeypad);
    	}
#endif //RD_TACTILE_FEEDBACK     
    }

CFepLayoutMultiLineIcfEditor::~CFepLayoutMultiLineIcfEditor()
    {
    delete iCustomDraw;
    delete iTextWrapper;
    delete iTextView; // text view
    delete iLayout; // text layout
    delete iRichText; // contained text object
    delete iCharFormatLayer; // char format layer
    delete iParaFormatLayer; // para format layer
    if (iWin)
        {
        iWin->Close();
        delete iWin;
        }
    //delete iContentCtrl;

    iOverlappedCtrlList.Close();
    iOverLapRects.Close();
	iOldMfneText.Close();

    if (iSecretTextTimer)
        iSecretTextTimer->Cancel();
    delete iSecretTextTimer;
    delete iInitPromptText;
    delete iPromptText;
    
    delete iMfneIcfData;
    //delete iCtrl;
    if(iFormExtendedInterfaceProvider)
        {
        delete iFormExtendedInterfaceProvider;
        iFormExtendedInterfaceProvider = NULL;
        }
    
    delete iSmileyManager;
    }

void CFepLayoutMultiLineIcfEditor::InitTextLayoutL()
    {
	iParaFormatLayer=CParaFormatLayer::NewL();
	iCharFormatLayer=CCharFormatLayer::NewL();

	iRichText = CRichText::NewL(iParaFormatLayer, iCharFormatLayer);

	iViewRect = Rect();
	iLayout = CTextLayout::NewL(iRichText, iViewRect.Width());
    //iLayout->SetAmountToFormat( CTextLayout::EFFormatBand );
    //TInt x = 0;
    //iLayout->SetFontHeightIncreaseFactor(10);
    //iLayout->SetMinimumLineDescent( x );
    //CContentControl* ctrl = new(ELeave) CContentControl;
    //iWndControl = ctrl;    
    //ctrl->ConstructL();    
    
    iWin = new(ELeave) RWindow(iCoeEnv->WsSession());
    iWin->Construct(iCoeEnv->RootWin(),reinterpret_cast<TInt>(this));
    //iWin = ctrl->GetWindow();
    //TBool b = iWin->IsRedrawStoreEnabled();
    iWin->EnableRedrawStore(EFalse);
    iWin->SetVisible(EFalse);

    // another method to avoid crash caused by HandleRedrawEvent 
    // when (screen resolution changed)
    //iCtrl = new (ELeave) CCoeControl;
    //iCtrl->SetContainerWindowL(*iWin);    
    //iWin->Construct(iCoeEnv->RootWin(),reinterpret_cast<TInt>(iCtrl));
    //ctrl->SetExtent(iViewRect.iTl, iViewRect.Size());

	iTextView = CTextView::NewL(iLayout, 
	                            iViewRect,
                        		BitmapDevice(),
                        		iCoeEnv->ScreenDevice(),
                        		iWin,
                        		&iCoeEnv->RootWin(),
                        		&iCoeEnv->WsSession()); 
                       		
    iTextView->SetCursorVisibilityL(TCursor::EFCursorInvisible, TCursor::EFCursorInvisible);

    if (!iCustomDraw)
        {
        iCustomDraw = new (ELeave) CFepLayoutMIcfCustomDraw(this);
        iLayout->SetCustomDraw(iCustomDraw);
        }

    if ( !iTextWrapper )
        {
        iTextWrapper = new (ELeave) CFepLayoutMIcfCustomWrap();
        iLayout->SetCustomWrap( iTextWrapper );
        }
        
    //SetTextMarginL(30);
    iTextView->DisableFlickerFreeRedraw();
    iTextView->FormatTextL(); 
    iMfneIcfData = HBufC::NewL( KMaxMfneDataLen );
    }

void CFepLayoutMultiLineIcfEditor::SetTextAlignmentL(TInt aAlignment, TInt aLanguage)
    {
    CParaFormat* paraFormat = CParaFormat::NewL();
    CleanupStack::PushL(paraFormat);
    TParaFormatMask paraFormatMask;
    
    paraFormat->iLanguage = ELangEnglish;
    if ( aAlignment == EAknEditorAlignNone )
        {
        // fix: Prompt text in ICF moves on dimmed VITU-T window after rotating screen.
        aAlignment = iAlignment;
        }
    switch (aAlignment)
        {
        case EAknEditorAlignLeft: 
            paraFormat->iHorizontalAlignment = CParaFormat::EAbsoluteLeftAlign;
            iTextAlign = CGraphicsContext::ELeft;
            break;
        case EAknEditorAlignRight:
            paraFormat->iHorizontalAlignment = CParaFormat::EAbsoluteRightAlign;
            iTextAlign = CGraphicsContext::ERight;
            break;
        case EAknEditorAlignCenter:
            paraFormat->iHorizontalAlignment = CParaFormat::ECenterAlign;
            iTextAlign = CGraphicsContext::ECenter;
            break;
        case EAknEditorAlignBidi:
            {
            paraFormat->iHorizontalAlignment = CParaFormat::ELeftAlign;
            paraFormat->iLanguage = aLanguage;
            iTextAlign = CGraphicsContext::ELeft;
            break;
            }
        default:
            paraFormat->iHorizontalAlignment = CParaFormat::ELeftAlign;
            iTextAlign = CGraphicsContext::ELeft;
            break;
        }

    paraFormatMask.SetAttrib(EAttParaLanguage);
    paraFormatMask.SetAttrib(EAttAlignment);
    
    iRichText->ApplyParaFormatL(paraFormat,
                                paraFormatMask,
                                0,
                                iPromptTextLen);

    iRichText->ApplyParaFormatL(paraFormat,
                                paraFormatMask,
                                iPromptTextLen,
                                iRichText->DocumentLength() - iPromptTextLen);
	iTextView->FormatTextL();                            
    
    CleanupStack::PopAndDestroy(paraFormat);
    
    iAlignment = aAlignment;
    }

void CFepLayoutMultiLineIcfEditor::BaseConstructL(TInt aFontHeight, 
                                            TInt aMaxFontHeight, 
                                            const CFont* aFont)
    {
    CFepUiBaseCtrl::BaseConstructL();

    iCoeEnv = CCoeEnv::Static();
    //iMaxFontHeight = aMaxFontHeight;
    //iFontHeight = aFontHeight;
    //iFont = aFont;
    InitTextLayoutL();
    iInsertionPoint = UiLayout()->CreateCursor();
    iInsertionPoint->SetOwner(this);
    SetFontL(aFontHeight, aMaxFontHeight, aFont);

    iSecretTextTimer = CPeriodic::NewL(CActive::EPriorityStandard);

    SetAutoCompleteTextColor(KRgbGray);
    
    
    //EnableTextSelection(EFalse);
    
    iSmileyManager = CPeninputSmileyManager::NewL();
    }

void CFepLayoutMultiLineIcfEditor::SetTextColorL(const TRgb aColor)
    {
    iCharFormatMask.ClearAttrib( EAttFontUnderline );
    
    iTextColor = aColor;
    iCharFormat.iFontPresentation.iTextColor = aColor;
    iCharFormatMask.SetAttrib(EAttColor);

    iRichText->ApplyCharFormatL(iCharFormat, iCharFormatMask, 0, iRichText->DocumentLength());
    //iRichText->SetInsertCharFormatL(iCharFormat, iCharFormatMask, iRichText->DocumentLength());    
    }

void CFepLayoutMultiLineIcfEditor::SetTextSelColorL(const TRgb aColor)
    {
    iCharFormatMask.ClearAttrib( EAttFontUnderline );
    
    iTextSelColor = aColor;
    iCharFormat.iFontPresentation.iHighlightColor = aColor;
    iCharFormatMask.SetAttrib(EAttFontHighlightColor);

    iRichText->ApplyCharFormatL(iCharFormat, iCharFormatMask, 0, iRichText->DocumentLength());
    //iRichText->SetInsertCharFormatL(iCharFormat, iCharFormatMask, iRichText->DocumentLength());    
    }

void CFepLayoutMultiLineIcfEditor::SetAutoCompleteTextColor(const TRgb aColor)
    {
    iTextAutoColor = aColor;
    }

TBool CFepLayoutMultiLineIcfEditor::IsFocused()
    {
    return iHasFocus;
    }

void CFepLayoutMultiLineIcfEditor::DeletePromptTextL()
    {
    // delete original prompt text
    if (iPromptTextLen > 0)
        {
        iRichText->DeleteL(0, iPromptTextLen);
        iTextView->HandleInsertDeleteL(TCursorSelection(0, 0),
                                       iPromptTextLen);
        }
    }

void CFepLayoutMultiLineIcfEditor::RecalculatePosByNewPromptTextL(const TDesC& aNewPromptText)
    {
    // recalculate cursor pos
    AdjustSelectionL(TCursorSelection(iTextView->Selection().iCursorPos + aNewPromptText.Length() - iPromptTextLen,
                     iTextView->Selection().iAnchorPos + aNewPromptText.Length() - iPromptTextLen));

    // set new cursor position    

    iPreInlineStartPos += (aNewPromptText.Length() - iPromptTextLen);
    iPreInlineEndPos += (aNewPromptText.Length() - iPromptTextLen);
    iPreAutoStartPos += (aNewPromptText.Length() - iPromptTextLen);
    iPreAutoEndPos += (aNewPromptText.Length() - iPromptTextLen);
    iPreTextSelStartPos += (aNewPromptText.Length() - iPromptTextLen);
    iPreTextSelEndPos += (aNewPromptText.Length() - iPromptTextLen);

    }

const HBufC* CFepLayoutMultiLineIcfEditor::PromptText()
	{
	return iPromptText;	
	}
	
void CFepLayoutMultiLineIcfEditor::SetPromptTextL(const TDesC& aPromptText, TBool aCleanContent)
    {
    
    if ( aCleanContent )
    	{
    	//clean all the content (include prompt text) on the ICF
        TInt length = iRichText->DocumentLength();
        iRichText->DeleteL( 0,  length);
        iTextView->HandleInsertDeleteL(TCursorSelection(0, 0), length);
    	iTextView->SetSelectionL( TCursorSelection(0, 0) );
    	iCursorSel = TCursorSelection(0, 0);
        iNoMatchState = EFalse;
    	}
    else 
        {
        /*if ( ( ( !iInitPromptText && aPromptText.Length() == 0 ) )|| 
            ( iInitPromptText && *iInitPromptText == aPromptText && 
            iPromptText && TextWidth(*iPromptText) <= iViewRect.Width()) )
    	    {
    	    //prompt text has not been change then need not to be reset
    	    //but iLineSeparator may be changed, need reset prompt text
    	    return;
            }*/
        //prompt text need to be reset and not clean the content
        iRichText->DeleteL( 0,  iPromptTextLen );
        iTextView->HandleInsertDeleteL(TCursorSelection(0, 0), iPromptTextLen );
        }
    
    TBool bPromptTextChange = EFalse;
    if ( !iInitPromptText || *iInitPromptText != aPromptText )
        {
        //prompt text has been changed
        delete iInitPromptText;
        iInitPromptText = NULL;
        if ( aPromptText.Length() > 0 )
            {
        	iInitPromptText = HBufC::NewL( aPromptText.Length() );
        	iInitPromptText->Des().Copy( aPromptText );
            }
        bPromptTextChange = ETrue;
        }

    delete iPromptText;
    iPromptText = NULL;

    if (aPromptText == KNullDesC)
        {
        RecalculatePosByNewPromptTextL(KNullDesC);
        iPromptTextLen = 0;
        if( bPromptTextChange )
        	{
        	UpdateArea( Rect(), EFalse );
        	} 
        return;
        }

    // The extra length is separators between prompt text and normal text
    TInt prompttextLen;
    
    if ( !iLineSeparator )
        {
        iLineSeparator = ( TBidiText::TextDirectionality( aPromptText ) == TBidiText:: ERightToLeft );
        }
        
    iLineSeparator ? 
               prompttextLen = aPromptText.Length() + TBidiLogicalToVisual::KMinCharAvailable + 3 :
               prompttextLen = aPromptText.Length() + TBidiLogicalToVisual::KMinCharAvailable + 2;
    iPromptText = HBufC::NewL(prompttextLen);
    TPtr text = iPromptText->Des();

    TBuf<1> ptr0;
    ptr0.Append( KEllipsis );

    TPtrC ptr1(&KSegment, 1);
    TPtrC ptr2(&KSeparator, 1);
    
    TInt separatorLen = TextWidth(ptr1) + TextWidth(ptr2) + TextWidth(ptr0);
    TInt textWidth = TextWidth(aPromptText) + separatorLen;
    TBool ret = EFalse;
    
    if (textWidth >= iViewRect.Width() )
        {
        ret = ETrue;
        AknBidiTextUtils::ConvertToVisualAndClip(aPromptText,
                                                 text,
                                                 *iFont,
                                                 iViewRect.Width() - separatorLen - KPromptAdjustLen,
                                                 iViewRect.Width() - separatorLen - KPromptAdjustLen,
                                                 AknBidiTextUtils::EImplicit,
                                                 0xFFFF);
                                                 
        TInt charNums = iFont->TextCount( text, iViewRect.Width() - separatorLen );
        text = aPromptText.Left( charNums );
        }
    else
        {
        text.Copy(aPromptText);
        }
    
	for ( ;(text.Length() > 0 && text.LocateReverse(KSegment) == text.Length()-1 ); )
	    {
		text.Delete( text.Length()-1, 1 );
	    }
		
    if ((text.LocateReverse(KSeparator) != text.Length() - 1) && 
        (text.LocateReverse(KSeparatorcn) != text.Length() - 1))
        {
        // if sent prompt text already include separator, then no need to 
        // append it again
        if ( ret )
            {
            text.Append( ptr0 );
            }
        text.Append(ptr2);
        }
    else if ( ret )
        {
        text.Insert( text.Length()-1, ptr0 );
        }        

    text.Append(ptr1); // segment between prompt text and official text
    textWidth = TextWidth(text);
    
    iRichText->InsertL(0, text);
    iTextView->HandleInsertDeleteL(TCursorSelection(0, text.Length()),
                                   0);
    
    if (iLineSeparator)
        {
    	iRichText->InsertL(text.Length(), CEditableText::EParagraphDelimiter);
    	iTextView->HandleInsertDeleteL(TCursorSelection(text.Length(), text.Length()+1), 0);
    	text.Append(KLineSeparator);
        }

    RecalculatePosByNewPromptTextL(text);

    iPromptTextLen = text.Length();
    // apply format to prompt text and normal text
    SetPromptTextFormatL(iPromptTextLen);
    CalcPromptTextRectL();
    if( bPromptTextChange )
    	{
    	UpdateArea(Rect(),EFalse);
    	} 
    }

void CFepLayoutMultiLineIcfEditor::AdjustIcfDataForPromptText(TFepInputContextFieldData& aData)
    {
    if (iPromptTextLen == 0)
        return;
    aData.iCurSel.iCursorPos += iPromptTextLen;
    aData.iCurSel.iAnchorPos += iPromptTextLen;
    
    aData.iStartPos += iPromptTextLen;
    }

void CFepLayoutMultiLineIcfEditor::SetMfneTextL(const TFepInputContextFieldData& aData)
    {
    TFepInputContextFieldData icfdata = aData;
    
    TPtr ptr = iMfneIcfData->Des();
    
    switch (icfdata.iCmd)
        {
        case EPeninputICFInitial:
            {
            if ( iRichText->DocumentLength() > iPromptTextLen )
                {
                TInt delLen = iRichText->DocumentLength() - iPromptTextLen;
                iRichText->DeleteL(iPromptTextLen, delLen );
                iTextView->HandleInsertDeleteL(TCursorSelection(iPromptTextLen, iPromptTextLen), 
                                               delLen);
                }
            ptr.Copy(icfdata.iText);
            }
            break;
        case EPeninputICFReplace:
            {
            ptr.Replace( icfdata.iStartPos, icfdata.iLength, icfdata.iText);
            }
            break;
        case EPeninputICFDelete:
            {
            ptr.Delete(icfdata.iStartPos, icfdata.iLength);
            }
            break;
        case EPeninputICFSetCurSel:
            {
            
            }
            break;
        default:
            {
            return;
            }
        }
        
    if ( iSynCursor || iOldMfneText != ptr ||
         iCursorSel.iCursorPos - iPromptTextLen != icfdata.iCurSel.iCursorPos ||
         iCursorSel.iAnchorPos - iPromptTextLen != icfdata.iCurSel.iAnchorPos )
        {
        iCursorSel.iCursorPos = icfdata.iCurSel.iCursorPos + iPromptTextLen;
        iCursorSel.iAnchorPos = icfdata.iCurSel.iAnchorPos + iPromptTextLen;
        DrawMfneText();
        iSynCursor = EFalse;
        iOldMfneText.CreateL(ptr);
        }
        
    iInsertionPoint->SetVisible( EFalse ); 

    SetCursorSelVisibility( icfdata.iCursorSelVisible );    

    SetCursorVisible(aData.iCursorVisibility);
     
    UpdateArea(Rect(),EFalse); 
    
    return;
    }
    
void CFepLayoutMultiLineIcfEditor::SetTextL(const TFepInputContextFieldData& aData)
    {
    if ( aData.iCmd == EPeninputICFSetCurSel && 
         aData.iFlag == EFepICFDataInlineNoMatch && iNoMatchState )
        {
        return;
        }
        
    if ( !iSycnFepData )
        {
        iSycnFepData = ETrue;
        }
    else if ( this == UiLayout()->RootControl()->CtrlCapPointer() )
    	{
    	return;
    	}
    	
    TBool bScroll = EFalse;	
    TFepInputContextFieldData icfdata = aData;
    AdjustIcfDataForPromptText(icfdata);
    
	if (/*(iPreLanguageID == ELangUrdu || iPreLanguageID == ELangArabic || 
         iPreLanguageID == ELangFarsi || iPreLanguageID == ELangHebrew) &&*/
         icfdata.iFlag & EFepICFDataDirectionMFNE )
        {
        iRtlMfneEditor = ETrue;
        iInsertionPoint->SetHeight( iFontHeight + iLineSpace );
        SetMfneTextL( aData );
        return;
        }    

    //iInsertionPoint->SetOn( EFalse );
    	
    iLeadingEdge = icfdata.iLeadingEdge;

    if (icfdata.iFlag != EFepICFDataInlineNoMatch && iNoMatchState)
        {
        UpdateNoMatchTextL(iPreInlineEndPos, KNullDesC, EFalse);
        }

    switch (icfdata.iCmd)
        {
        case EPeninputICFInitial:
            {
            iRtlMfneEditor = EFalse;
            if ( !iLayout->CustomDraw() )
                {
                iLayout->SetCustomDraw(iCustomDraw);    
                }
            
            TInt delLen = iRichText->DocumentLength() - iPromptTextLen;
            iRichText->DeleteL(iPromptTextLen, delLen );
            iTextView->HandleInsertDeleteL(TCursorSelection(iPromptTextLen, iPromptTextLen), 
                                           delLen);
            iRichText->InsertL(iRichText->DocumentLength(), icfdata.iText);
            
            iTextView->HandleInsertDeleteL(
                         TCursorSelection(iPromptTextLen + icfdata.iText.Length(), iPromptTextLen),
                         0);
            iTextInited = ETrue;
            bScroll = ETrue;
            //UpdateArea( Rect() );
            
            iPreInlineStartPos = iPromptTextLen;
            iPreInlineEndPos = iRichText->DocumentLength();
            
            if ( icfdata.iCursorVisibility )
                {
                iInsertionPoint->DelayCursorShown();     
                }
            
            AdjustSelectionL( icfdata.iCurSel );
            RecalcualteTextPositionL();
            SetMsgBubbleRect();
            Draw();
            }
            break;
        case EPeninputICFReplace:
            {
            if (( icfdata.iStartPos < 0 ) || 
                ( icfdata.iStartPos > iRichText->DocumentLength()))
                {
                return;
                }
            
            if (icfdata.iLength > 0)
                {
				TInt count = iRichText->DocumentLength();
				if ( count < 1 )
				    {
				    return;
				    }
                  
                iRichText->DeleteL(icfdata.iStartPos, icfdata.iLength);
                iTextView->HandleInsertDeleteL(
                                            TCursorSelection(icfdata.iStartPos, icfdata.iStartPos),
                                            icfdata.iLength);
                }

            iRichText->InsertL(icfdata.iStartPos, icfdata.iText);
            iTextView->HandleInsertDeleteL(
                 TCursorSelection( icfdata.iStartPos + icfdata.iText.Length(), icfdata.iStartPos ),
                 0);
            bScroll = ETrue;
            if ( icfdata.iCursorVisibility )
                {
                iInsertionPoint->DelayCursorShown();     
                }
            }
            break;
        case EPeninputICFDelete:
            {
            if (( icfdata.iStartPos < 0 ) || 
                ( icfdata.iStartPos > iRichText->DocumentLength()))
                {
                return;
                }
            
            iRichText->DeleteL(icfdata.iStartPos, icfdata.iLength);
            iTextView->HandleInsertDeleteL(TCursorSelection(icfdata.iStartPos, icfdata.iStartPos), 
                                           icfdata.iLength);
            bScroll = ETrue;
            }
            break;
        case EPeninputICFSetCurSel:
            {
            bScroll = ETrue;
            }
            break;
        default:
            {
            return;
            }
        }
        
    UpdateTextL(icfdata);    
    
    // Need to set selection visible before set selection for textview
    SetCursorSelVisibility( icfdata.iCursorSelVisible );    
    
    SetSelectionL(icfdata.iCurSel);
    ResetViewHeightL();
    
    if (icfdata.iMidPos == 0)
        {
        // inline state
        // if there is paragraph separater inside icfdata's iText, then should not
        // set to inline state
        if (icfdata.iText.Locate(KParagraphSeperator) == KErrNotFound)
            {
            SetAutoCompleteStateL(EFalse);
            icfdata.iFlag == EFepICFDataInlineNoMatch ? 
                                                      SetInlineStateL(ETrue, icfdata.iStartPos, 
                                                                      icfdata.iCurSel.iCursorPos, 
                                                                      ETrue, icfdata.iText) :
                                                      SetInlineStateL(ETrue, icfdata.iStartPos, 
                                                                      icfdata.iCurSel.iCursorPos);
            }
        }
    else if (icfdata.iMidPos > 0)
        {
        // auto complete state
        // Q: does insertion point need to be shown when auto complete?
        // on the assumption that insertion point do not shown
        SetAutoCompleteStateL( ETrue, icfdata.iStartPos, 
                               icfdata.iStartPos + icfdata.iText.Length() - icfdata.iMidPos, 
                               icfdata.iCurSel.iCursorPos );
        }
    else
        {
        // icfdata.iMidPos < 0, normal state
        //
        SetInlineStateL(EFalse, icfdata.iStartPos, icfdata.iCurSel.iCursorPos);
        SetAutoCompleteStateL(EFalse);
        
        }    

    // after inline adjust, cusor position may be updated
    if( bScroll )
        {
        TryDisplayMaxTextL(iCursorSel.iCursorPos);
        }
    
    iTextView->DrawL(iViewRect);    
    //if(iMsgBubble)    
//        iMsgBubble->Draw();   

    iInsertionPoint->SetVisible( EFalse ); 

    SetCursorVisible(icfdata.iCursorVisibility);
    
    if (AbleToDraw())
        {
        UpdateAreaImmed(Rect(),EFalse);    
        }
    }

void CFepLayoutMultiLineIcfEditor::SetSelectionL(TCursorSelection aCurSel)
    {
    if ( !IsTextPosValid(aCurSel.LowerPos()) || 
         !IsTextPosValid(aCurSel.HigherPos()) )
        {
        return;
        }
    
    AdjustSelectionL( aCurSel );
    
    if ( iLayout->FirstLineInBand() != 0 && iLineMaxCount > 1 &&
        iPromptTextLen > 0 && aCurSel.iCursorPos == iPromptTextLen )
    	{
        iTextView->SetViewLineAtTopL( 1 );	
    	}
        
    RecalcualteTextPositionL();
    
    if ( iCursorSel.Length() > 0 )
        {
        if ( iMsgBubble->IsShowing() && HighlightOverlapWithBubble( iMsgBubble->Rect() )  ) 
            {
            HideBubbleTemp();    
            }
        else if ( iHideBubbleTemp && !HighlightOverlapWithBubble( iMsgBubble->Rect() ) )
            {
            iMsgBubble->Popup( iMsgBubble->Rect() );
            iHideBubbleTemp = EFalse;
            }
        }
    else
        {
        SetMsgBubbleRect();
        SetInfoBubbleRect();
        }    
    }

TBool CFepLayoutMultiLineIcfEditor::HighlightOverlapWithBubble( const TRect& aRect )
    {
    TBool ret = EFalse;
    
    if ( iCursorSel.Length() == 0 || iCursorSel.iCursorPos < 0 ||
         !iViewRect.Intersects( aRect ) )
        {
        return ret;
        }
  
    if ( aRect.Intersects( iInsertionPoint->CursorRect() ) )
        {
        ret = ETrue;
        return ret;
        }
        
    TRect rect = iViewRect;
    rect.Intersection( aRect );
    
    TPoint leftTopPoint = TPoint( rect.iTl.iX, rect.iTl.iY );
    TPoint leftBottomPoint = TPoint( rect.iTl.iX, rect.iBr.iY );
    TPoint rightTopPoint = TPoint( rect.iBr.iX, rect.iTl.iY );
    TPoint rightBottomPoint = TPoint( rect.iBr.iX, rect.iBr.iY );
    
    TInt leftTopIndex = 0;
    TRAP_IGNORE(leftTopIndex = iTextView->XyPosToDocPosL( leftTopPoint ));
    TInt leftBottomIndex = 0;
    TRAP_IGNORE(leftBottomIndex = iTextView->XyPosToDocPosL( leftBottomPoint ));
    TInt rightTopIndex = 0;
    TRAP_IGNORE(rightTopIndex = iTextView->XyPosToDocPosL( rightTopPoint ));
    TInt rightBottomIndex = 0;
    TRAP_IGNORE(rightBottomIndex = iTextView->XyPosToDocPosL( rightBottomPoint ));
    
    ret =  ( leftTopIndex > iCursorSel.LowerPos() && leftTopIndex < iCursorSel.HigherPos() ) || 
         ( leftBottomIndex > iCursorSel.LowerPos() && leftBottomIndex < iCursorSel.HigherPos() ) ||
         ( rightTopIndex > iCursorSel.LowerPos() && rightTopIndex < iCursorSel.HigherPos() ) ||
         ( rightBottomIndex > iCursorSel.LowerPos() && rightBottomIndex < iCursorSel.HigherPos() );

    return ret;
    }
    
void CFepLayoutMultiLineIcfEditor::HideBubbleTemp()
    {
    HideBubble();
    iHideBubbleTemp = ETrue;
    }
    
// for secret editor
void CFepLayoutMultiLineIcfEditor::UpdateTextL(const TFepInputContextFieldData& aData)
    {
    if (!iTextIsSecret)
        {
        return;
        }

    switch (aData.iCmd)
        {
        case EPeninputICFInitial:
            {
            TPtrC icfPtr = aData.iText;
            TPtr buf( const_cast<TUint16*>( icfPtr.Ptr() ), icfPtr.Length(), icfPtr.Length() );
            buf.Fill( KStar );
            
            TInt delLen = iRichText->DocumentLength() - iPromptTextLen;
            iRichText->DeleteL(iPromptTextLen, delLen);
            iTextView->HandleInsertDeleteL( TCursorSelection( iPromptTextLen, iPromptTextLen ), 
                                            delLen );
            iRichText->InsertL(iRichText->DocumentLength(), buf);
            iTextView->HandleInsertDeleteL
                ( TCursorSelection( iRichText->DocumentLength(), iPromptTextLen ), 0 );
            }
            break;
        case EPeninputICFReplace:
            {                    
            if (iSecretTextTimer->IsActive())
                {
                iSecretTextTimer->Cancel();

                if (aData.iStartPos >= iPromptTextLen + 1)
                    {
                    TBuf<1> buf;
                    buf.Append( '*' );
                    for( TInt i = 0; i < aData.iText.Length(); i++ )
                        {
                        iRichText->DeleteL( aData.iStartPos - 1 + i, 1 );
                        iTextView->HandleInsertDeleteL(
                                        TCursorSelection( aData.iStartPos - 1 + i, 
                                                          aData.iStartPos - 1 + i ), 1 );
                        
                        iRichText->InsertL( aData.iStartPos - 1 + i, buf );
                        iTextView->HandleInsertDeleteL(
                             TCursorSelection( aData.iStartPos - 1 + i, 
                                               aData.iStartPos - 1 + i + buf.Length() ), 0 );
                        }
                    }
                }
            else if( aData.iText.Length() > 1 )
                {
                TBuf<1> buf;
                buf.Append( '*' );
                for( TInt i = 0; i < aData.iText.Length() - 2; i++ )
                    {
                    iRichText->DeleteL( aData.iStartPos + i, 1 );
                    iTextView->HandleInsertDeleteL(
                                    TCursorSelection( aData.iStartPos + i, 
                                                      aData.iStartPos + i ), 1 );
                        
                    iRichText->InsertL( aData.iStartPos + i, buf );
                    iTextView->HandleInsertDeleteL(
                                 TCursorSelection( aData.iStartPos + i, 
                                                   aData.iStartPos + i + buf.Length() ), 0 );
                    }
                }
            iSecretTextTimer->Start(
                                 aData.iText.Length()>1?KSecretInstantShowTimer:KSecretUpdateTimer,
                                 KSecretUpdateTimer, TCallBack(UpdateSecretTextL, this));
            }
            break;
        case EPeninputICFSetCurSel:
        default:
            {
            break;
            }
        }
    }

TInt CFepLayoutMultiLineIcfEditor::UpdateSecretTextL(TAny* aEditArea)
    {
    CFepLayoutMultiLineIcfEditor* editArea = static_cast<CFepLayoutMultiLineIcfEditor*>(aEditArea);

    if ( !editArea->iTextIsSecret )
        {
        editArea->iSecretTextTimer->Cancel();
        return KErrNone;
        }
    TPtrC icftext = editArea->iRichText->Read(editArea->iPromptTextLen, 
                                              editArea->iRichText->DocumentLength());
    TInt startPos = icftext.LocateReverse(KStar) + 1 + editArea->iPromptTextLen;

    TBuf<1> buf;
    buf.Append(KStar);

    TInt textlen = editArea->iRichText->DocumentLength();

    if (startPos < textlen)
        {
        editArea->iRichText->DeleteL(startPos, 1);
        editArea->iTextView->HandleInsertDeleteL(TCursorSelection(startPos, startPos), 1);
        editArea->iRichText->InsertL(startPos, buf);
        editArea->iTextView->HandleInsertDeleteL(TCursorSelection(startPos, startPos+buf.Length()),
                                                 0);

        if (startPos == textlen - 1)
            {
            editArea->iSecretTextTimer->Cancel();
            }
        }
    editArea->UpdateArea(editArea->Rect(),EFalse);
    return KErrNone;
    }
    
void CFepLayoutMultiLineIcfEditor::UpdateSecretTextL()
    {
    if (!iTextIsSecret)
        {
        return;
        }
        
    if (iSecretTextTimer->IsActive())
        {
        iSecretTextTimer->Cancel();
        UpdateSecretTextL( this );
        }
    }

void CFepLayoutMultiLineIcfEditor::SetDimmed(TBool aDimFlag)
    {
    iDimmed = aDimFlag;
    }

void CFepLayoutMultiLineIcfEditor::SetFontL(TInt aFontHeight, 
                                            TInt aMaxFontHeight, 
                                            const CFont* aFont)
    {
    TInt newHeightInPixel = iCoeEnv->ScreenDevice()->VerticalPixelsToTwips(aFontHeight);
    TInt OldHeightInPixel = iCoeEnv->ScreenDevice()->VerticalPixelsToTwips(iFontHeight);
    
    if( newHeightInPixel == OldHeightInPixel )
        {
        return;
        }
    else
        {
        iFormatChange = ETrue;
        }    
    
    iMaxFontHeight = aMaxFontHeight;
    iFontHeight = aFontHeight;
    iFont = aFont;
    
        
    iCharFormatMask.ClearAttrib( EAttFontUnderline );
    iCharFormatMask.SetAttrib(EAttFontHeight); 
    iCharFormatMask.SetAttrib(EAttFontStrokeWeight); 
    iCharFormatMask.SetAttrib(EAttFontTypeface);

    iCharFormat.iFontSpec.iHeight = iCoeEnv->ScreenDevice()->VerticalPixelsToTwips(iFontHeight);
    iCharFormat.iFontSpec.iFontStyle.SetStrokeWeight(EStrokeWeightNormal);
    iCharFormat.iFontSpec.iFontStyle.SetBitmapType(EAntiAliasedGlyphBitmap);

    iRichText->ApplyCharFormatL(iCharFormat, iCharFormatMask, 0, iRichText->DocumentLength());    
    if( InlineStateOn() || AutoCompletionStateOn() )
        {
        iCharFormatMask.ClearAll();
        iCharFormatMask.SetAttrib( EAttFontUnderline );        
        iCharFormat.iFontPresentation.iUnderline = EUnderlineOn ;
        iRichText->ApplyCharFormatL(iCharFormat, iCharFormatMask, iPreInlineStartPos, 
                                    iPreInlineEndPos - iPreInlineStartPos);
        }
    
    //iRichText->SetInsertCharFormatL(iCharFormat, iCharFormatMask, iRichText->DocumentLength() - iPromptTextLen);

    SetPromptTextFormatL( iPromptTextLen );
    
    iFormatChange = ETrue;
    }

void CFepLayoutMultiLineIcfEditor::SetFocus(TBool aFlag)
    {
    if(aFlag == iHasFocus)
        return;

    if(aFlag) //gain focus
        {
        PrepareForFocusGain();
        }
    else
        {
        PrepareForFocusLoss();
        }
    }

void CFepLayoutMultiLineIcfEditor::SetCursorVisible( TBool aCursorVisibleFlag )
    {
    if ( aCursorVisibleFlag != iCursorVisible )
        {
        iCursorVisible = aCursorVisibleFlag;
        iInsertionPoint->SetOn( iCursorVisible && iHasFocus );
        }            
    }

TBool CFepLayoutMultiLineIcfEditor::CursorVisible() const
    {
    return iCursorVisible;
    }

void CFepLayoutMultiLineIcfEditor::Draw()
    {
    //return;
    if (!AbleToDraw())
        {
        return;
        }

    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());

    DrawOpaqueMaskBackground();
	
    if( BackgroundBmp() )
	    {
	    CFbsBitmap* bmp = BackgroundBmp();
	    if( Rect().Size() != bmp->SizeInPixels() )
	        {
	        AknIconUtils::SetSize(bmp, Rect().Size(), EAspectRatioNotPreserved);            
	        }
	    }
    DrawBackground();

    if (iSkinIdSet)
        {
        gc->Clear(Rect());
        TRect innerrect = Rect();
  	    //innerrect.Shrink(4, 4);

        AknsDrawUtils::DrawFrame(AknsUtils::SkinInstance(), 
   		                         *gc, 
   		                         Rect(), 
   		                         innerrect,
   		                         BgImgSkinId(),
   		                         KAknsIIDDefault);
        }
    
    TRAP_IGNORE(iTextView->DrawL(iViewRect));
    
    if ( iRtlMfneEditor )
        {
        DrawMfneText();
        }    
    iInsertionPoint->SetVisible( EFalse );
    }
    
void CFepLayoutMultiLineIcfEditor::DrawMfneText()
    {
    if ( iCursorSel.LowerPos() < 0 || iCursorSel.HigherPos() < 0 )
        {
        return;
        }
        
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    
    TPoint tl = iViewRect.iTl;
    TPoint br = TPoint( iViewRect.iBr.iX, iViewRect.iTl.iY+iFontHeight + iLineSpace );
    
    if ( iPromptTextLen > 0 )
        {
        tl.iY += iFontHeight + iLineSpace;
        br.iY += iFontHeight + iLineSpace;
        }
    
    if( iTextAlign == CGraphicsContext::ECenter )   
        {
        iOffset = (iViewRect.Width() - iFont->TextWidthInPixels(iMfneIcfData->Des()))/2;
        }
    else if ( iTextAlign == CGraphicsContext::ERight )
        {
        iOffset = iViewRect.Width() - iFont->TextWidthInPixels(iMfneIcfData->Des());
        }
    else
    	{
    	iOffset = 0;
    	}
    
    TRect rect = TRect( tl, br );
    
    TPtrC ptr1 = iMfneIcfData->Des().Left(iCursorSel.iCursorPos-iPromptTextLen);
    TInt width1 = iFont->TextWidthInPixels( ptr1 ); 
    iInsertionPoint->SetPosition( TPoint(rect.iTl.iX + width1 + iOffset,rect.iTl.iY ));
    
    TInt baseLine = iFont->AscentInPixels()/2 + rect.Height()/2;
    
    TRect clipRect = 
        TRect( TPoint(Rect().iTl.iX, rect.iTl.iY), TPoint( Rect().iBr.iX, rect.iBr.iY));    
    
    gc->SetClippingRect( clipRect );
    gc->Clear( clipRect );
    
    DrawOpaqueMaskBackground( clipRect );
    DrawBackground( clipRect );

    if (iSkinIdSet)
        {
   		AknsDrawUtils::DrawFrame(AknsUtils::SkinInstance(), 
    	                         *gc, 
    	                         Rect(), 
    	                         Rect(),
    	                         BgImgSkinId(),
    	                         KAknsIIDDefault);
   		                         
        }
	
    gc->UseFont( iFont );
    gc->SetPenStyle(CGraphicsContext::ESolidPen);
    gc->SetPenColor(iTextColor);
    gc->SetBrushStyle(CGraphicsContext::ENullBrush);
    gc->DrawText(iMfneIcfData->Des(), rect, baseLine, iTextAlign );//printPos, 
    gc->DiscardFont();
    
    if ( iCursorSel.Length() > 0 )
        {
        TRect selRect(rect);
        
        TPtrC ptr1 = iMfneIcfData->Des().Left(iCursorSel.LowerPos()-iPromptTextLen);
        TPtrC ptr2 = iMfneIcfData->Des().Left(iCursorSel.HigherPos()-iPromptTextLen);
        
        TInt width1 = 
            iFont->TextWidthInPixels( ptr1 );
        TInt width2 = 
            iFont->TextWidthInPixels( ptr2 );
        
        selRect.iTl.iX = iViewRect.iTl.iX + width1 + iOffset;
        selRect.iBr.iX = iViewRect.iTl.iX + width2 + iOffset;
        
        // draw selection by inverting colors in the selected text rectancle
        gc->SetBrushStyle(CGraphicsContext::ESolidBrush);
        gc->SetPenStyle(CGraphicsContext::ENullPen);
        gc->SetBrushColor(KRgbBlack);
        gc->SetDrawMode(CGraphicsContext::EDrawModeNOTSCREEN);
        gc->DrawRect(selRect);

        // restore normal draw mode
        gc->SetDrawMode(CGraphicsContext::EDrawModePEN);
        gc->SetBrushStyle(CGraphicsContext::ENullBrush); 
        }
    gc->CancelClipping();
    }

void CFepLayoutMultiLineIcfEditor::SetRectL(const TRect& aRect)
    {
    if ( aRect == TRect() )
        {
        return;
        }
    CFepUiBaseCtrl::SetRect(aRect);
    
    iTextView->SetViewRect( aRect );
    iLayout->SetWrapWidth(aRect.Width());
    //iWndControl->SetExtent(aRect.iTl, aRect.Size());
    
    iViewRect.iTl.iX = aRect.iTl.iX + iLeftMargin;
    iViewRect.iBr.iX = aRect.iBr.iX - iRightMargin;

    iViewRect.iTl.iY = aRect.iTl.iY + iTopMargin ;
    iViewRect.iBr.iY = aRect.iBr.iY - iBottomMargin;

    iMaxIcfEditorHeight = iViewRect.Height();
    
    // calculate the viewrect height
    const TInt lineCount=iLayout->NumFormattedLines();
    const TInt height=iLayout->FormattedHeightInPixels();
    TInt lineHeight = (height+lineCount-1) / lineCount;
    if ( lineHeight > iMaxIcfEditorHeight )
        {
        lineHeight = iMaxIcfEditorHeight;
        }
    if ( iFormatLineHeight != lineHeight )
        {
        iFormatLineHeight = lineHeight;
        ApplyLineSpaceFormatL( iFormatLineHeight );
        }
    
    iInsertionPoint->SetHeight( iFormatLineHeight );
    
    iLineMaxCount = iViewRect.Height()/iFormatLineHeight;
    TInt viewHeight = iFormatLineHeight * iLineMaxCount;
    
    TInt margin =  iViewRect.Height() - viewHeight;
    if ( iViewRect.iBr.iY - margin > Rect().iBr.iY )
    	{
    	iViewRect.iTl.iY += margin;
    	}
    else
    	{
    	iViewRect.iBr.iY -= margin;
    	}
    //iViewRect.iBr.iX -= 50;
    iTextView->SetViewRect(iViewRect);
    iLayout->SetWrapWidth(iViewRect.Width());
//    iWndControl->SetExtent(iViewRect.iTl, iViewRect.Size());
    
    if ( iCursorSel.iCursorPos >= 0 && !iRtlMfneEditor )
        {
        TPoint cursorPt;
        TRAP_IGNORE(iTextView->DocPosToXyPosL(iCursorSel.iCursorPos, cursorPt));
        CalculateCursorPos(cursorPt);
        if ( !iViewRect.Contains( cursorPt ) )
            {
            TRAP_IGNORE(iTextView->SetViewLineAtTopL( iLineNumber+1 ));
            }
        }

    TRAP_IGNORE(RecalcualteTextPositionL());        
    SetMsgBubbleRect();
    }

void CFepLayoutMultiLineIcfEditor::ResetViewHeightL()
    {
    const TInt height = iLayout->FormattedHeightInPixels();
    const TInt lineCount=iLayout->NumFormattedLines();
    if ( lineCount <= 0 )
        {
        return;
        }
        
    TInt lineHeight = (height+lineCount-1) / lineCount;
    
    if ( iFormatLineHeight == lineHeight )
        {
        return;
        }
        
    if ( lineHeight > iMaxIcfEditorHeight )
        {
        lineHeight = iMaxIcfEditorHeight;
        }
    if ( iFormatLineHeight != lineHeight )
        {
        iFormatLineHeight = lineHeight;
        ApplyLineSpaceFormatL( iFormatLineHeight );
        }
            
    iInsertionPoint->SetHeight( iFormatLineHeight );
    
    iViewRect.iTl.iY = Rect().iTl.iY + iTopMargin;
    iViewRect.iBr.iY = Rect().iBr.iY - iBottomMargin;
    
    // calculate the max line count    
    iLineMaxCount = iViewRect.Height()/iFormatLineHeight;
    
    // calculate the viewrect height
    TInt viewHeight = iFormatLineHeight * iLineMaxCount;
    
    TInt margin =  iViewRect.Height() - viewHeight;
    if ( iViewRect.iBr.iY - margin > Rect().iBr.iY )
    	{
    	iViewRect.iTl.iY += margin;
    	}
    else
    	{
    	iViewRect.iBr.iY -= margin;
    	}
    iTextView->SetViewRect(iViewRect);
    }

void CFepLayoutMultiLineIcfEditor::SetMsgBubbleRect()
    {
    if( ( !iMsgBubble->IsShowing() && !iHideBubbleTemp ) ||
        iLineMaxCount < 1 || iCursorSel.iCursorPos < 0 )
        {
        return;
        }
    
    TBool rtlLanguage = EFalse;
    if ( iPreLanguageID == ELangUrdu || iPreLanguageID == ELangArabic || 
         iPreLanguageID == ELangFarsi || iPreLanguageID == ELangHebrew )
        {
        rtlLanguage = ETrue;
        }
    TPoint startPoint;
    TSize size = iMsgBubble->Rect().Size();
    TRect rect1;
    TRect rect2;
    
    if ( iLineMaxCount == 1 )
        {
        TRect rectLeft;
        TRect rectRight;
        
		// top left corner of ICF
        startPoint = TPoint(Rect().iTl.iX, Rect().iTl.iY );
        startPoint += TPoint( KMsgBubblePosAdjustValueX, KMsgBubblePosAdjustValueY );
        rectLeft = TRect(startPoint,size);
                    
        // top right corner of ICF
        startPoint = TPoint(Rect().iBr.iX-iMsgBubble->Rect().Width(), Rect().iTl.iY);
        startPoint += TPoint( -KMsgBubblePosAdjustValueX, KMsgBubblePosAdjustValueY );
        rectRight = TRect(startPoint,size);
        
        if ( rtlLanguage ) //rtl
            {
            rect1 = rectLeft;
            rect2 = rectRight;
            }
        else
            {
            rect1 = rectRight;
            rect2 = rectLeft;
            }  
        }
    else
        {
        TRect rectTopLeft;
        TRect rectTopRight;
        TRect rectBottomLeft;
        TRect rectBottomRight;
        
        // bottom left corner of ICF
        startPoint = TPoint(Rect().iTl.iX, Rect().iBr.iY-iMsgBubble->Rect().Height() );
        startPoint += TPoint( KMsgBubblePosAdjustValueX, -KMsgBubblePosAdjustValueY );
        rectBottomLeft =  TRect(startPoint,size);
       
        // bottom right corner of ICF
        startPoint = TPoint(Rect().iBr.iX-iMsgBubble->Rect().Width(), 
                            Rect().iBr.iY-iMsgBubble->Rect().Height());
        startPoint += TPoint( -KMsgBubblePosAdjustValueX, -KMsgBubblePosAdjustValueY );
        rectBottomRight =  TRect(startPoint,size);
        
        // top left corner of ICF
        startPoint = TPoint(Rect().iTl.iX, Rect().iTl.iY );
        startPoint += TPoint( KMsgBubblePosAdjustValueX, KMsgBubblePosAdjustValueY );
        rectTopLeft =  TRect(startPoint,size);
        
        // top right corner of ICF
        startPoint = TPoint(Rect().iBr.iX-iMsgBubble->Rect().Width(), Rect().iTl.iY);
        startPoint += TPoint( -KMsgBubblePosAdjustValueX, KMsgBubblePosAdjustValueY );
        rectTopRight =  TRect(startPoint,size);
        
        TBool firstLine =  iLayout->FirstLineInBand() == iLineNumber ;
            
        if ( rtlLanguage ) //rtl
            {
            rect1 = firstLine ? rectBottomLeft : rectTopLeft;
            rect2 = firstLine ? rectTopLeft : rectBottomLeft;
            }
        else
            {
            rect1 = firstLine ? rectBottomRight : rectTopRight;
            rect2 = firstLine ? rectTopRight : rectBottomRight;
            }
        }

    TBool rect1Shown = !rect1.Intersects( iInsertionPoint->CursorRect() ) && 
                       !HighlightOverlapWithBubble( rect1 );
    if ( rect1Shown && iLayout->FirstLineInBand() == 0 )
        {
        rect1Shown = rect1Shown && !rect1.Intersects( iPromptTextRect );
        }
    TBool rect2Shown = !rect2.Intersects( iInsertionPoint->CursorRect() ) && 
                       !HighlightOverlapWithBubble( rect2 );
    if ( rect2Shown && iLayout->FirstLineInBand() == 0 )
        {
        rect2Shown =  rect2Shown && !rect2.Intersects( iPromptTextRect );
        }
    
    if ( !rect1Shown && !rect2Shown )
        {
        if ( iMsgBubble->IsShowing() )
            {
            HideBubbleTemp();
            }
        return;
        }
    
    TRect bubbleRect = rect1Shown? rect1 : rect2;
    
    if ( iMsgBubble->IsShowing() )
        {
        if ( bubbleRect != iMsgBubble->Rect() )
            {
            HideBubbleTemp();
            }
        iMsgBubble->Popup( bubbleRect );
        
        iHideBubbleTemp = EFalse;
        return;
        }
        
    if ( iHideBubbleTemp )
        {
        iMsgBubble->Popup( bubbleRect);
        iHideBubbleTemp = EFalse;
        }
    
    return;
    }
void CFepLayoutMultiLineIcfEditor::SetInfoBubbleRect()
    {
    if( ( !iInfoBubble->IsShowing() && !iHideInfoBubbleTemp ) ||
        iLineMaxCount < 1 || iCursorSel.iCursorPos < 0 )
        {
        return;
        }
    
    TBool rtlLanguage = EFalse;
    if ( iPreLanguageID == ELangUrdu || iPreLanguageID == ELangArabic || 
         iPreLanguageID == ELangFarsi || iPreLanguageID == ELangHebrew )
        {
        rtlLanguage = ETrue;
        }
    TPoint startPoint;
    TSize size = iInfoBubble->Rect().Size();
    TRect rect1;
    TRect rect2;
    if ( iLineMaxCount == 1 )
        {
        TRect rectLeft;
        TRect rectRight;
        startPoint = TPoint(Rect().iTl.iX, Rect().iTl.iY );
        startPoint += TPoint( KMsgBubblePosAdjustValueX, KMsgBubblePosAdjustValueY );
        rectLeft = TRect(startPoint,size);
        startPoint = TPoint(Rect().iBr.iX-iInfoBubble->Rect().Width(), Rect().iTl.iY);
        startPoint += TPoint( -KMsgBubblePosAdjustValueX, KMsgBubblePosAdjustValueY );
        rectRight = TRect(startPoint,size);
        if ( rtlLanguage ) //rtl
            {
            rect1 = rectLeft;
            rect2 = rectRight;
            }
        else
            {
            rect1 = rectRight;
            rect2 = rectLeft;
            }  
        }
    else
        {
        TRect rectTopLeft;
        TRect rectTopRight;
        TRect rectBottomLeft;
        TRect rectBottomRight;
        startPoint = TPoint(Rect().iTl.iX, Rect().iBr.iY-iInfoBubble->Rect().Height() );
        startPoint += TPoint( KMsgBubblePosAdjustValueX, -KMsgBubblePosAdjustValueY );
        rectBottomLeft =  TRect(startPoint,size);
        startPoint = TPoint(Rect().iBr.iX-iInfoBubble->Rect().Width(), 
                            Rect().iBr.iY-iInfoBubble->Rect().Height());
        startPoint += TPoint( -KMsgBubblePosAdjustValueX, -KMsgBubblePosAdjustValueY );
        rectBottomRight =  TRect(startPoint,size);
        startPoint = TPoint(Rect().iTl.iX, Rect().iTl.iY );
        startPoint += TPoint( KMsgBubblePosAdjustValueX, KMsgBubblePosAdjustValueY );
        rectTopLeft =  TRect(startPoint,size);
        startPoint = TPoint(Rect().iBr.iX-iInfoBubble->Rect().Width(), Rect().iTl.iY);
        startPoint += TPoint( -KMsgBubblePosAdjustValueX, KMsgBubblePosAdjustValueY );
        rectTopRight =  TRect(startPoint,size);
        TBool firstLine =  iLayout->FirstLineInBand() == iLineNumber ;
        if ( rtlLanguage ) //rtl
            {
            rect1 = firstLine ? rectBottomLeft : rectTopLeft;
            rect2 = firstLine ? rectTopLeft : rectBottomLeft;
            }
        else
            {
            rect1 = firstLine ? rectBottomRight : rectTopRight;
            rect2 = firstLine ? rectTopRight : rectBottomRight;
            }
        }
       TBool rect1Shown = !rect1.Intersects( iInsertionPoint->CursorRect() ) && !HighlightOverlapWithBubble( rect1 );
       TBool rect2Shown = !rect2.Intersects( iInsertionPoint->CursorRect() ) && !HighlightOverlapWithBubble( rect2 );
    if ( !rect1Shown && !rect2Shown )
        {
        if ( iInfoBubble->IsShowing() )
            {
            HideBubbleTemp();
            }
        return;
        }
    TRect bubbleRect = rect1Shown? rect2 : rect1;
    if ( iInfoBubble->IsShowing() )
        {
        if ( bubbleRect != iInfoBubble->Rect() )
            {
            HideBubbleTemp();
            }
        iInfoBubble->Popup( bubbleRect );
        iHideInfoBubbleTemp = EFalse;
        return;
        }
    if ( iHideInfoBubbleTemp )
        {
        iInfoBubble->Popup( bubbleRect);
        iHideInfoBubbleTemp = EFalse;
        }
    return;
    }
void CFepLayoutMultiLineIcfEditor::SetMsgBubbleCtrlSize(const TSize& aSize)
    {
    TPoint leftTop = TPoint(Rect().iBr.iX-iMsgBubble->Rect().Width(),Rect().iTl.iY);
    iMsgBubble->SetRect( TRect(leftTop,aSize) ) ;        
    }
void CFepLayoutMultiLineIcfEditor::SetInfoBubbleCtrlSize(const TSize& aSize)
    {
    TPoint leftTop = TPoint(Rect().iBr.iX-iInfoBubble->Rect().Width(),Rect().iTl.iY);
    iInfoBubble->SetRect( TRect(leftTop,aSize) ) ;        
    }

void CFepLayoutMultiLineIcfEditor::ShowTooltipL(const TDesC& aText)
	{
	if ( !iTooltip )
	    {
	    return;
	    }

    iTooltip->SetTextL( aText );
    TPoint tl = iInsertionPoint->Position();
    if ( iLineMaxCount > 1 )
	    {
		TBool firstLine =  iLayout->FirstLineInBand() == iLineNumber;
		if ( firstLine )
		    {
			tl.iY += iInsertionPoint->Height();
			}
		else
		    {
			tl.iY -= iTooltip->Rect().Height();
			}
		}
    TRect tooltipRect = TRect( tl, iTooltip->Rect().Size() );
    
    TInt moveX = tooltipRect.iBr.iX - Rect().iBr.iX;
    if ( moveX > 0 )
    	{
    	tooltipRect.iBr.iX -= moveX;
    	tooltipRect.iTl.iX -= moveX;
    	}
		
	if ( tooltipRect.Intersects( iMsgBubble->Rect() ) )
	    {
		moveX = tooltipRect.iBr.iX - iMsgBubble->Rect().iTl.iX;
		if ( moveX > 0 && tooltipRect.iTl.iX - moveX >= Rect().iTl.iX )
	    	{
	    	tooltipRect.iBr.iX -= moveX;
	    	tooltipRect.iTl.iX -= moveX;
	    	}
		moveX = iMsgBubble->Rect().iBr.iX - tooltipRect.iTl.iX;
		if ( moveX > 0 && tooltipRect.iBr.iX + moveX <= Rect().iBr.iX )
	    	{
	    	tooltipRect.iBr.iX += moveX;
	    	tooltipRect.iTl.iX += moveX;
	    	}
	    }
    iTooltip->Popup( tooltipRect );
	}

void CFepLayoutMultiLineIcfEditor::HideTooltip()
	{
	if ( iTooltip )
	    {
	    iTooltip->Close();
	    }	
	}

void CFepLayoutMultiLineIcfEditor::RemoveOverlappedCtrlIfNeeded()
    {
    for (TInt i = iOverlappedCtrlList.Count()-1; i >= 0 ; i--)
        {
        if (!iOverlappedCtrlList[i].iCtrl->Rect().Intersects(Rect()))
            {
            RestoreCursorState(i);
            }
        }
    }

void CFepLayoutMultiLineIcfEditor::SizeChangedL(const TRect& aRect,
                                                TInt aFontHeight,
                                                TInt aMaxFontHeight,
                                                const CFont* aFont)
    {   
    TRect oriRect = Rect();
    
    AdjustBmpDeviceIfNeeded();
    // apply font formats 
    SetFontL(aFontHeight, aMaxFontHeight, aFont);
    
    if ( !iFormatChange && oriRect == aRect )
        {
        return;
        }
        
    if( BackgroundBmp() )
        {
        CFbsBitmap* bmp = BackgroundBmp();
        if( aRect.Size() != bmp->SizeInPixels() )
            {
            AknIconUtils::SetSize(bmp, aRect.Size(), EAspectRatioNotPreserved);            
            }
        }
   
    RemoveOverlappedCtrlIfNeeded();
 
    ApplyFormatL();
    
    SetRectL(aRect);
    
    AdjustPromptTextL();
    

    iLayout->SetAmountToFormat(CTextLayout::EFFormatAllText);    
    
    if ( iFormatChange )
        {
        iTextView->HandleGlobalChangeNoRedrawL();
        }
    
    TBool ready = Ready();
    if( ready )
        {
        SetReady( EFalse );     
        }
    
	RootControl()->ReDrawRect( oriRect );
	if( ready )
	    {
	    SetReady( ETrue );    
	    }

    if ( iFormatChange )	    
        {
        iTextView->SetSelectionL( iTextView->Selection() );
        RecalcualteTextPositionL();
        SetMsgBubbleRect();
        Draw();
        }
    }

TBool CFepLayoutMultiLineIcfEditor::BelongToPromptText(TInt aPos)
    {
    if (!iPromptText || iPromptText->Length() == 0 )
        {
        return EFalse;
        }

    return (aPos >= 0 && aPos <= (iPromptText->Length() - 1));
    }

CFepUiBaseCtrl* CFepLayoutMultiLineIcfEditor::HandlePointerDownEventL(const TPoint& aPt)
    {
    TBool isNormalState = ( iState == CFepLayoutMultiLineIcf::EIcfNormal );
    TBool isCursorDisabled = !iCursorVisible && !iCursorSelVisible && isNormalState;
    
    if ( iDimmed || isCursorDisabled )
        {
        return NULL;
        }

    if(!iViewRect.Contains(aPt))
        {
        return this;
        }
    
    CFepUiBaseCtrl::HandlePointerDownEventL(aPt);
    CapturePointer(ETrue);  
    iSycnFepData = EFalse;
    
    // For keeping previous cursor pos.
    TInt preCursorPos = iCursorSel.iCursorPos;
    if ( iRtlMfneEditor )
        {
        TInt cursorPos = iFont->TextCount(iMfneIcfData->Des(), aPt.iX - iViewRect.iTl.iX - iOffset );
        iCursorSel = TCursorSelection(cursorPos+iPromptTextLen, cursorPos+iPromptTextLen);
        AdjustFepCursorPosForPromptText( ETrue );
        return this;
        }

    TInt cursorPos = iTextView->XyPosToDocPosL(const_cast<TPoint&>(aPt));

    iPntDownOnInline = EFalse;

    if (BelongToPromptText(cursorPos))
        {
        ReportEvent(EEventControlFocusGained);
        return this;
        }
    SetSelectionL(TCursorSelection(cursorPos, cursorPos)); 
    iTextView->DrawL(iViewRect);  
    iInsertionPoint->SetVisible( EFalse );
    UpdateArea(Rect(),EFalse);
    SetFocus(ETrue);

    switch (iState)
        {
        case CFepLayoutMultiLineIcf::EIcfInline:
            {
            TInt inlineEndPos =  iNoMatchState ? iPreInlineEndPos + 1 : iPreInlineEndPos;
            
            if (iTextView->Selection().iCursorPos >= iPreInlineStartPos && 
                iTextView->Selection().iCursorPos < inlineEndPos )
                {
                // when pointer down on non-text, the pointer down pos is considerred
                // last character in ICF
                // open match selection list
                iPntDownOnInline = ETrue;
                CapturePointer(EFalse); 
                if (iNoMatchState)
                    {
                    ReportEvent(EEventPointerDownOnNomatch);
                    }
                    
                else
                    {
                    if (iTooltip && iTooltip->IsShowing())
                        {
                        iTooltip->Hide(ETrue);
                        }
                    else
                        {
                        ReportEvent(EEventPointerDownOnInline);
                        }
                    
                    }
                    
                }
             
    
    	    if (!iPntDownOnInline)
    	        {
                TPoint point = aPt;
                TInt cursorPos = iTextView->XyPosToDocPosL( point );
    	        
    	        if(cursorPos == iRichText->DocumentLength() ||
    	           BelongToPromptText(cursorPos))
    	            {
                    if (iTooltip && !iTooltip->IsShowing())
                        {
 //                       ReportEvent(EEventInsertSpace);    
                        }
    	            }
    	        else
    	            {
    	            /*
                    if (iTooltip && !iTooltip->IsShowing())
                        {
            	        ReportEvent(EEventControlFocusGained);               
                        UiLayout()->SignalOwner( ESignalCommitInlineText );
        			    SetInlineStateL(EFalse, iPreInlineStartPos, iPreInlineEndPos);
                        }
                    */
    	            }
    	        
                }
   
            }
            break;
        case CFepLayoutMultiLineIcf::EIcfAutoComplete:
            {
            if (iTextView->Selection().iCursorPos >= iPreInlineStartPos && 
                iTextView->Selection().iCursorPos < iPreInlineEndPos )
                {
                iPntDownOnInline = ETrue;
                CapturePointer(EFalse);                 
                if (iTooltip && iTooltip->IsShowing())
                    {
                    iTooltip->Hide(ETrue);
                    }
                else
                    {
                    ReportEvent(EEventPointerDownOnInline);
                    }
                }
            else
                {
                // do nothing
                //submit autocomplete when set text
                if (iTooltip && !iTooltip->IsShowing())
                    {
    	            ReportEvent( EEventControlFocusGained );
                    UiLayout()->SignalOwner( ESignalCommitITIInlineText );
			        SetInlineStateL( EFalse, iPreInlineStartPos, iPreInlineEndPos );
			        SetAutoCompleteStateL( EFalse );
                    }
                }
            }
            break;
        case CFepLayoutMultiLineIcf::EIcfNormal:
    		{
		    if (preCursorPos != iCursorSel.iCursorPos)
			   {
			   ReportEvent(EEventSetAppCursorSelection);
			   }		   			
    		}
    		break;
        default:
            break;
        }
        
    iLastCursorPos = iCursorSel.iCursorPos;
    return this;
    }

void CFepLayoutMultiLineIcfEditor::AdjustFepCursorPosForPromptText( TBool aSyncCursor )
    {
    TCursorSelection cursor;
    
    if ( iRtlMfneEditor )
        {
        cursor = iCursorSel;
        }
    else
        {
        cursor = iTextView->Selection();
        }
    
    cursor.iCursorPos -= iPromptTextLen;
    cursor.iAnchorPos -= iPromptTextLen;

    UiLayout()->SetAppEditorCursor(cursor, aSyncCursor);
    iSynCursor = ETrue;
    }

CFepUiBaseCtrl* CFepLayoutMultiLineIcfEditor::HandlePointerUpEventL(const TPoint& aPt)
    {
    TBool isNormalState = ( iState == CFepLayoutMultiLineIcf::EIcfNormal );
    TBool isCursorDisabled = !iCursorVisible && !iCursorSelVisible && isNormalState;
    
    if ( iDimmed || isCursorDisabled || !PointerDown() )
        {
        return NULL;
        }
   
    CFepUiBaseCtrl::HandlePointerUpEventL(aPt);
    CapturePointer(EFalse); 
    
    if ( iRtlMfneEditor )
        {
        TInt cursorPos = iFont->TextCount(iMfneIcfData->Des(), aPt.iX - iViewRect.iTl.iX - iOffset );
        iCursorSel = TCursorSelection(cursorPos+iPromptTextLen, 
                                      iCursorSel.iAnchorPos);
        AdjustFepCursorPosForPromptText( ETrue );
        return this;
        }
        
    if (!iPntDownOnInline)
        {
        if(iViewRect.Contains(aPt))
            {
            TPoint point = aPt;
            TInt cursorPos = iTextView->XyPosToDocPosL( point );
            if ( !Rect().Contains(aPt) || 
                 ( Rect().Contains(aPt) && BelongToPromptText(cursorPos) ) )
                {
                cursorPos = iCursorSel.iCursorPos;
                }
            
            if ( cursorPos != iCursorSel.iCursorPos )
                {
                SetSelectionL( TCursorSelection(cursorPos,iCursorSel.iAnchorPos) );
                }
            }
            
        if (iTooltip && iTooltip->IsShowing())
            {
            iTooltip->Hide(ETrue);
            }
        else
            {
            AdjustFepCursorPosForPromptText( !iPntDownOnInline );
            if(iState != CFepLayoutMultiLineIcf::EIcfNormal)
                {
                UiLayout()->SignalOwner( ESignalStartInlineText );
                }
            }
        }

    iPntDownOnInline = EFalse;    
    
    ReportEvent(EEventIcfPointerUpEvent);   
    
    return this;
    }

TBool CFepLayoutMultiLineIcfEditor::MoveCursorL( const TPoint& aPt, TInt& aCursorPos )
	{
	TBool selectionUpdated = ETrue;
	
	if ( aPt.iX < iViewRect.iTl.iX )
        {
        // selection was dragged beyond the left edge of iCorrectedRect -> scroll left
        TInt prev = iCursorSel.iCursorPos;
        aCursorPos = iCursorSel.iCursorPos-1 > iPromptTextLen ? 
            iCursorSel.iCursorPos-1 : iPromptTextLen;
        if(prev != aCursorPos)
            {
            TCursorPosition::TMovementType move = TCursorPosition::EFLeft;
            iTextView->MoveCursorL(move,EFalse);
            }
        else
        	{
        	selectionUpdated = EFalse;
        	}
        } 
    else if ( aPt.iX > iViewRect.iBr.iX )
        {
        // selection was dragged beyond the right edge of iCorrectedRect -> scroll right
        TInt prev = iCursorSel.iCursorPos;
        aCursorPos = iCursorSel.iCursorPos+1 < iRichText->DocumentLength() ?
            iCursorSel.iCursorPos+1 : iRichText->DocumentLength();
        if(prev != aCursorPos)            
            {
            TCursorPosition::TMovementType move = TCursorPosition::EFRight;
            iTextView->MoveCursorL(move,EFalse);
            } 
        else
        	{
        	selectionUpdated = EFalse;
        	}     
        }
    else if ( aPt.iY > iViewRect.iBr.iY )
        {
        TInt prev = iCursorSel.iCursorPos;
    	if ( prev < iRichText->DocumentLength() )
        	{
        	TCursorPosition::TMovementType move = TCursorPosition::EFLineDown;
        	iTextView->MoveCursorL( move, EFalse );
	        TPoint point = aPt;
	        aCursorPos = iTextView->XyPosToDocPosL( point );
	        if ( aCursorPos == prev )
	        	{
	        	selectionUpdated = EFalse;
	        	}
        	}
        else
        	{
        	selectionUpdated = EFalse;
        	}
        }
    else if ( aPt.iY < iViewRect.iTl.iY )
    	{
    	TInt prev = iCursorSel.iCursorPos;
    	if ( prev > iPromptTextLen )
        	{
        	TCursorPosition::TMovementType move = TCursorPosition::EFLineUp;
        	iTextView->MoveCursorL( move, EFalse );
	        TPoint point = aPt;
	        aCursorPos = iTextView->XyPosToDocPosL( point );
	        if ( aCursorPos == prev )
	        	{
	        	selectionUpdated = EFalse;
	        	}
        	}
        else
        	{
        	selectionUpdated = EFalse;
        	}
    	}
    	
    return selectionUpdated;
	}
CFepUiBaseCtrl* CFepLayoutMultiLineIcfEditor::HandlePointerMoveEventL(const TPoint& aPt)
    {       
    TBool isNormalState = ( iState == CFepLayoutMultiLineIcf::EIcfNormal );
    TBool isCursorDisabled = !iCursorVisible && !iCursorSelVisible && isNormalState;
    
    if ( !PointerDown() || iDimmed || isCursorDisabled || iPntDownOnInline )
    	{
        return this;
    	}
    
    if ( iRtlMfneEditor )
        {
        TInt cursorPos = iFont->TextCount(iMfneIcfData->Des(), aPt.iX - iViewRect.iTl.iX - iOffset );
        iCursorSel = TCursorSelection(cursorPos+iPromptTextLen, 
                                      iCursorSel.iAnchorPos);
        AdjustFepCursorPosForPromptText( ETrue );
        return this;
        }
        
    TPoint point = aPt;
    
    TInt cursorPos = iTextView->XyPosToDocPosL( point );

    if( Rect().Contains(aPt) && 
        ( BelongToPromptText(cursorPos) || cursorPos == iCursorSel.iCursorPos ) )
        {
        return this;
        }
    
    iInsertionPoint->DelayCursorShown();
    
    TBool selectUpdated = MoveCursorL( aPt, cursorPos );
    
    if ( selectUpdated )
    	{
    	SetSelectionL(TCursorSelection(cursorPos,iCursorSel.iAnchorPos) ); 
    	}
    	
    if( !iViewRect.Contains(aPt))
    	{
        AdjustFepCursorPosForPromptText();
    	}
        
	UpdateArea(Rect(),EFalse);
#ifdef RD_TACTILE_FEEDBACK
    if (UiLayout()->SupportTactileFeedback())
        {
        if ( iLastCursorPos != iCursorSel.iCursorPos ) 
            {
            UiLayout()->DoTactileFeedback(ETouchFeedbackSensitiveKeypad);
            iLastCursorPos = iCursorSel.iCursorPos;
            }
        }
#endif //RD_TACTILE_FEEDBACK

    return this;
    }

void CFepLayoutMultiLineIcfEditor::CancelPointerDownL()
    {
    if (PointerDown())
        {
        SetPointerDown(EFalse);
        CapturePointer(EFalse); 
        }
    }

void CFepLayoutMultiLineIcfEditor::HandlePointerLeave(const TPoint& aPt)
    {
    TBool isInline = ( iState == CFepLayoutMultiLineIcf::EIcfInline );
    TBool isCursorDisabled = !iCursorVisible && !iCursorSelVisible && !isInline;
    
    if ( !PointerDown() || iDimmed || isCursorDisabled )    
        {
        return;
        }

    UiLayout()->SetAppEditorCursor(iCursorSel);
    CFepUiBaseCtrl::HandlePointerLeave(aPt);
    }

void CFepLayoutMultiLineIcfEditor::PrepareForFocusLoss()
    {
    iInsertionPoint->SetOn(EFalse);

    iHasFocus = EFalse;

    ReportEvent(EEventControlFocusLost);
    }

TBool CFepLayoutMultiLineIcfEditor::PrepareForFocusGain()
    {
    iHasFocus = ETrue;

    iInsertionPoint->SetOn( iCursorVisible );
    
    ReportEvent(EEventControlFocusGained);

    return ETrue;
    }

void CFepLayoutMultiLineIcfEditor::Move(const TPoint& aOffset)
    {
    CFepUiBaseCtrl::Move(aOffset);

    iViewRect.Move(aOffset);
    iTextView->SetViewRect(iViewRect);
    
    if (iInsertionPoint)
        {
        TPoint curPos = iInsertionPoint->Position();
        curPos += aOffset;
        iInsertionPoint->SetPosition(curPos);
        }
    }

void CFepLayoutMultiLineIcfEditor::OnDeActivate()
    {    
    CFepUiBaseCtrl::OnDeActivate();
    iTextInited = EFalse;
    TRAP_IGNORE(UpdateSecretTextL());
    //cancel the insertion pointer timer
    if(iHasFocus)
        {
        iInsertionPoint->SetOn(EFalse);
        }

    // avoid crash caused by HandleRedrawEvent 
    // (called when screen resolution changed)
    iWin->SetVisible(EFalse);
    }

void CFepLayoutMultiLineIcfEditor::OnActivate()
    {
    CFepUiBaseCtrl::OnActivate();
    EnableRegionUpdating();

    AdjustBmpDeviceIfNeeded();

    //set cursor if needed    
    //cancel the insertion pointer timer
    TBool on = (iCursorVisible && iHasFocus );//&& (iOverlappedCtrlList.Count() <= 0))
    for(TInt i = 0 ; i < iOverlappedCtrlList.Count(); ++i)
        {
        on = on && iOverlappedCtrlList[i].iCtrl->Hiden();
        }
    iInsertionPoint->SetOn(on); 
    iWin->SetVisible(ETrue);
    }

void CFepLayoutMultiLineIcfEditor::HandleArrowBtnEventL(
                                                CFepLayoutMultiLineIcf::TArrowBtnEvent aEventType)
    {
    TCursorPosition::TMovementType movement;    

    switch (aEventType)
        {
        case CFepLayoutMultiLineIcf::EArrowUp:
            {
            movement = TCursorPosition::EFLineUp;
            iTextView->MoveCursorL(movement, EFalse);
            AdjustSelectionL( iTextView->Selection() );
            }
            break;
        case CFepLayoutMultiLineIcf::EArrowDown:
            {
            movement = TCursorPosition::EFLineDown;
            iTextView->MoveCursorL(movement, EFalse);
            }
            break;
        default:
            return;
        }

    AdjustFepCursorPosForPromptText();
    }

TBool CFepLayoutMultiLineIcfEditor::Contains(const TPoint& aPt)
    {
    TBool contain = EFalse;

    if (iSupportOverLap)
        {
        TBool isOverlap = EFalse;
        
        for (TInt i = 0; i < iOverLapRects.Count(); i++)
            {
            if (iOverLapRects[i].Contains(aPt))
                {
                isOverlap = ETrue;
                break;	
                }
            }

        contain = iRect.Contains(aPt) && !isOverlap;
        return contain;
        }

    contain = iRect.Contains(aPt);
    return contain;
    }

void CFepLayoutMultiLineIcfEditor::SetOverLapState(TBool aSupportOverLap, TRect aRect)
    {
    iSupportOverLap = aSupportOverLap;
    iOverLapRects.Reset();

    if (iSupportOverLap)
        {
        iOverLapRects.Append(aRect);
        }
    }

void CFepLayoutMultiLineIcfEditor::AddOverLapState(TRect aRect)
    {
    if (iSupportOverLap)
        {
        iOverLapRects.Append(aRect);
        }
    }

TInt CFepLayoutMultiLineIcfEditor::GetEditorFieldMaxLen()
    {
    return iRect.Width()/2;
    }

void CFepLayoutMultiLineIcfEditor::SetCursorSizeAndType()
    {
    iWeight = AknLayoutUtils::CursorWidthFromFont(CursorFontSpec());
    }

TFontSpec CFepLayoutMultiLineIcfEditor::CursorFontSpec()
    {
    TCharFormat charFormat;
    TCharFormatMask notUsed;
    iRichText->GetCharFormat(charFormat, notUsed, iTextView->Selection().iCursorPos, 0);
    return charFormat.iFontSpec;
    }

void CFepLayoutMultiLineIcfEditor::CalculateCursorPos(TPoint& aCaretPos)
    {
    TInt caret_width, caret_ascent, caret_descent;

    TTmLineInfo info;

    TInt pos = iTextView->Selection().iCursorPos >= iPromptTextLen ? 
        iTextView->Selection().iCursorPos : iPromptTextLen;

    iLayout->TagmaTextLayout().GetCursor(TTmDocPos(pos, iLeadingEdge), 
                                         ECursorVertical,
                                         info,
                                         aCaretPos,
                                         caret_width,
                                         caret_ascent,
                                         caret_descent);
    
    iLineNumber = info.iLineNumber;
    aCaretPos.iY = info.iInnerRect.iTl.iY;
    
    // get bandtop of CTextLayout
    TPoint band;
    iLayout->GetOrigin(band);
    TInt bandtop = -(band.iY);
    aCaretPos.iY -= bandtop;
   
    aCaretPos += iViewRect.iTl;
    
    // adjust which side the cursor hands around the origin
    // based on the sign of the 'width' from tagma
    if (caret_width < 0)
        aCaretPos.iX -= iWeight;
    }

void CFepLayoutMultiLineIcfEditor::RecalcualteTextPositionL()
    {
    TPoint newcursorPos;
    CalculateCursorPos(newcursorPos);
        
    newcursorPos = TPoint(newcursorPos.iX, newcursorPos.iY );//- iCursorHeight iFontHeight

    if (newcursorPos.iY < iViewRect.iTl.iY)
        {
        newcursorPos.iY = iViewRect.iTl.iY;
        }
    else if (newcursorPos.iY + iInsertionPoint->Height()> iViewRect.iBr.iY)
        {
        newcursorPos.iY = iViewRect.iBr.iY - iInsertionPoint->Height();
        }
    if ( newcursorPos.iX + KCursorPosCorrectionX < iViewRect.iTl.iX )
		{
		newcursorPos.iX = iViewRect.iTl.iX - KCursorPosCorrectionX;
		}
    else if ( newcursorPos.iX + KCursorPosCorrectionX + KCursorWidth > iViewRect.iBr.iX )
        {
        newcursorPos.iX = iViewRect.iBr.iX - KCursorPosCorrectionX - KCursorWidth; 
        }
        
    iInsertionPoint->SetPosition(newcursorPos);
    }

void CFepLayoutMultiLineIcfEditor::UpdateValidRegion(CFepUiBaseCtrl* aCtrl,TBool aRemoveFlag)
    {
    if (!iUpdateFlagEnabled || !aCtrl || aCtrl == this)
        {
        return;
        }
    
    if (aRemoveFlag)
        {
        RestoreCursorState(aCtrl);
        }
    else
        {
        if( aCtrl->IsKindOfControl(ECtrlBackgroundCtrl) )
            {
            return;
            }
            
        if (iTextInited)
            {
            //a shown control changes rect
            TInt idx = FindOverlappedCtrl(aCtrl);

            if (idx != KErrNotFound)
                {
                if (aCtrl->Rect().Intersects(iInsertionPoint->CursorRect()))
                    return;
                //restore
                RestoreCursorState(idx);
                }
            else  //new control
                {
                TRect intersectRect = aCtrl->Rect();
                intersectRect.Intersection(iInsertionPoint->CursorRect());
                if ( iInsertionPoint->CursorRect() != intersectRect )                
                    {
                    return;
                    }
                    
                //check whether it's parent
                CFepUiBaseCtrl* parent = ParentCtrl();
                while(parent && parent != aCtrl) parent = parent->ParentCtrl();
                
                if(parent)  // the given control is the ancestor of ICF.
                    return;
                
                SaveCursorState(aCtrl);
                //disable cursor
                iInsertionPoint->SetOn(EFalse);
                }
            }
        }    
    }

void CFepLayoutMultiLineIcfEditor::SaveCursorState(CFepUiBaseCtrl* aCtrl)
    {
    for(TInt i = 0 ; i < iOverlappedCtrlList.Count(); ++i)
        {        
        if(aCtrl == iOverlappedCtrlList[i].iCtrl)
            return;
        }
    TOverlappedInfo ctrlInfo={aCtrl,iInsertionPoint->IsOn()};
    
    iOverlappedCtrlList.Append(ctrlInfo);
    }

void CFepLayoutMultiLineIcfEditor::RestoreCursorState(CFepUiBaseCtrl* aCtrl)
    {
    //need check whether that control previously overlapped with ICF    
    TInt idx = FindOverlappedCtrl(aCtrl);
    if(idx != KErrNotFound)
        RestoreCursorState(idx);
    }

void CFepLayoutMultiLineIcfEditor::RestoreCursorState(TInt aIndex)
    {
    //no need to check index. as it's checked before called.							   
    iInsertionPoint->SetOn(iOverlappedCtrlList[aIndex].iCursorStateBeforeOverlapped);
    iOverlappedCtrlList.Remove(aIndex);
    }

TInt CFepLayoutMultiLineIcfEditor::FindOverlappedCtrl(CFepUiBaseCtrl* aCtrl)
    {
    for(TInt i = 0 ; i < iOverlappedCtrlList.Count(); ++i)
        {        
        if (aCtrl == iOverlappedCtrlList[i].iCtrl)
            return i;
        }

    return KErrNotFound;
    }

TBool CFepLayoutMultiLineIcfEditor::IsTextPosValid(TInt aPos)
    {
    return (aPos >= 0 && aPos <= iRichText->DocumentLength());
    }

void CFepLayoutMultiLineIcfEditor::UpdateNoMatchTextL(TInt aPos, const TDesC& aNoMatchText, 
                                                      TBool aNoMatchState)
    {
    if ( iNoMatchState == aNoMatchState )
        {
        return;
        }
        
    if (aNoMatchState)
        {
        iNoMatchState = aNoMatchState;
        iRichText->InsertL(aPos, aNoMatchText);
        iTextView->HandleInsertDeleteL(TCursorSelection(aPos+aNoMatchText.Length(), aPos),0);
        SetSelectionL( TCursorSelection( aPos+aNoMatchText.Length(), aPos+aNoMatchText.Length() ));
        }
    else
        {
        iNoMatchState = aNoMatchState;

        iRichText->DeleteL(aPos, 1);
        iTextView->HandleInsertDeleteL(TCursorSelection(aPos, aPos),1);
        SetSelectionL( TCursorSelection( aPos - 1, aPos - 1 ) );
        }         
    }

void CFepLayoutMultiLineIcfEditor::SetPromptTextFormatL(TInt aPromptTextLen)
    {
    if ( aPromptTextLen <= 0 )
        {
        return;
        }
    iCharFormatMask.ClearAll();
    iCharFormat.iFontSpec.iFontStyle.SetStrokeWeight(EStrokeWeightNormal);//EStrokeWeightBold
    iCharFormat.iFontPresentation.iUnderline = EUnderlineOff;
    iCharFormatMask.SetAttrib(EAttFontStrokeWeight);
    iCharFormatMask.SetAttrib( EAttFontUnderline );
    iCharFormatMask.SetAttrib( EAttColor );
    iCharFormat.iFontPresentation.iTextColor = iTextColor;

    // set prompt text as bold
    iRichText->ApplyCharFormatL(iCharFormat, iCharFormatMask, 0, aPromptTextLen );//- 1
    iTextView->HandleRangeFormatChangeL(TCursorSelection(0, aPromptTextLen ), ETrue);//- 1

    // set remain text as non-bold
    iCharFormat.iFontSpec.iFontStyle.SetStrokeWeight(EStrokeWeightNormal);
    //iRichText->SetInsertCharFormatL(iCharFormat, iCharFormatMask, aPromptTextLen - 1);
    }

void CFepLayoutMultiLineIcfEditor::SetSpecialStateL(TBool aStateOn, 
                                              TInt aStartPos, 
                                              TInt aEndPos, 
                                              TInt& aPreStartPos,
                                              TInt& aPreEndPos,
                                              TTextFormatAttribute aAttribute,
                                              TBool aNoMatchState,
                                              const TDesC& aNoMatchText)
    {
    iCharFormatMask.ClearAll();

    if (aAttribute == EAttFontUnderline)
        {
        // inline
        // set original inline as non-inline
        iCharFormat.iFontPresentation.iUnderline = EUnderlineOff;
        }
    else if (aAttribute == EAttColor)
        {
        // auto complete
        // set original auto as non-auto
        iCharFormat.iFontPresentation.iTextColor = iTextColor;
        }
    else
        {
        return;
        }
        
    iCharFormatMask.SetAttrib(aAttribute);

    if (IsTextPosValid(aPreStartPos) && IsTextPosValid(aPreEndPos))
        {
        iRichText->ApplyCharFormatL(iCharFormat, iCharFormatMask, 0, 
                                    iRichText->DocumentLength());
        //iTextView->HandleRangeFormatChangeL(TCursorSelection(aPreStartPos, aPreEndPos), ETrue);    
        } 

    aPreStartPos = 0;
    aPreEndPos = 0;

    if (aStateOn)
        {
        if (aAttribute == EAttFontUnderline)
            {
            iCharFormat.iFontPresentation.iUnderline = EUnderlineOn;
            UpdateNoMatchTextL(aEndPos, 
               TFepLayoutMIcfNoMatchesInline::TextForUnknownPredictiveTextIndication(aNoMatchText),
               aNoMatchState);
            }
        else if (aAttribute == EAttColor)
            {
            iCharFormat.iFontPresentation.iTextColor = iTextAutoColor;
            }
        iCharFormatMask.SetAttrib(aAttribute);

        if (aNoMatchState)
            {
            iRichText->ApplyCharFormatL(iCharFormat, iCharFormatMask, 
                                        Min(aStartPos,aEndPos), Abs( aEndPos - aStartPos ) + 1);
            iTextView->HandleRangeFormatChangeL(TCursorSelection(aStartPos, aEndPos + 1), ETrue);
            }
        else
            {
            iRichText->ApplyCharFormatL(iCharFormat, iCharFormatMask, 
                                        Min(aStartPos,aEndPos), Abs( aEndPos - aStartPos ) );
            iTextView->HandleRangeFormatChangeL(TCursorSelection(aStartPos, aEndPos), ETrue);
            }

        aPreStartPos = aStartPos;
        aPreEndPos = aEndPos;
        }
    }

void CFepLayoutMultiLineIcfEditor::SetInlineStateL(TBool aInline, 
                                             TInt aStartPos, 
                                             TInt aEndPos, 
                                             TBool aNoMatchState, 
                                             const TDesC& aNoMatchText)
    {
    if (aInline)
        iState = CFepLayoutMultiLineIcf::EIcfInline;
    else
        {
        iState = CFepLayoutMultiLineIcf::EIcfNormal;
        UpdateNoMatchTextL(iPreInlineEndPos, KNullDesC, EFalse);
        }

    SetSpecialStateL(aInline, 
                     aStartPos, 
                     aEndPos, 
                     iPreInlineStartPos, 
                     iPreInlineEndPos, 
                     EAttFontUnderline, 
                     aNoMatchState, 
                     aNoMatchText);
    }

void CFepLayoutMultiLineIcfEditor::SetAutoCompleteStateL(TBool aAutoComplete, 
                                                   TInt aStartPos, 
                                                   TInt aMidPos,
                                                   TInt aEndPos)
    {
    iAutoComplete = aAutoComplete;
    
    if ( aAutoComplete )
        {
        iState = CFepLayoutMultiLineIcf::EIcfAutoComplete;
        }
    else
        {
        iState = CFepLayoutMultiLineIcf::EIcfNormal;
        }
        
    SetSpecialStateL(aAutoComplete, 
                     aStartPos, 
                     aMidPos, 
                     iPreInlineStartPos, 
                     iPreInlineEndPos, 
                     EAttFontUnderline);
                     
    SetSpecialStateL(aAutoComplete, 
                     aMidPos, 
                     aEndPos, 
                     iPreAutoStartPos, 
                     iPreAutoEndPos, 
                     EAttColor);
    }

TAknsItemID CFepLayoutMultiLineIcfEditor::BgImgSkinId() const
    {
    if (iSkinIdSet)
        {
        return iBgSkinId;
        }

    return KAknsIIDNone;
    }
TBool CFepLayoutMultiLineIcfEditor::IsEmpty()
    {
    TPtrC icftext = iRichText->Read(0, iRichText->DocumentLength());
    return (icftext.Length() - iPromptTextLen <= 0);
    }

void CFepLayoutMultiLineIcfEditor::InlineTextPos(TPoint& aPosBelow, TPoint& aPosRight)
    {
    if ( InlineStateOn() || AutoCompletionStateOn() )
        {
        TRAP_IGNORE(iTextView->DocPosToXyPosL(iPreInlineStartPos, aPosBelow));
        TRAP_IGNORE(iTextView->DocPosToXyPosL(iPreInlineEndPos, aPosRight));
        }
    }
    
void CFepLayoutMultiLineIcfEditor::AdjustBmpDeviceIfNeeded()
    {
    TSize deviceSz = BitmapDevice()->SizeInPixels();
    if( ! (deviceSz == iLastDeviceSize) )
        {
        TRAP_IGNORE(iTextView->SetDisplayContextL(BitmapDevice(), NULL, NULL, NULL));
        iLastDeviceSize = deviceSz;
        //reset font
        //ResetApplyFont();
        iFormatChange = ETrue;
        }    
    }
    
CBubbleCtrl* CFepLayoutMultiLineIcfEditor::MsgBubbleCtrl()
    {
    return iMsgBubble;
    }
            
void CFepLayoutMultiLineIcfEditor::ShowBubble(const TDesC& aText, const TRect& /*aRect*/)
    {
    TRAP_IGNORE(iMsgBubble->SetTextL(aText));
    iHideBubbleTemp = ETrue;
    SetMsgBubbleRect();
    }
    
void CFepLayoutMultiLineIcfEditor::ShowByteWarningBubble(const TDesC& aInfo)
    {
    TSize size = iInfoBubble->Rect().Size();
    CFont::TMeasureTextInput*  input = new (ELeave) CFont::TMeasureTextInput;
    CleanupStack::PushL(input);
    input->iMaxBounds = iRect.Width();
    TInt width = iFont->MeasureText(aInfo, input, NULL);
    CleanupStack::PopAndDestroy();
    size.SetSize(width, size.iHeight);
    SetInfoBubbleCtrlSize(size);
    TRAP_IGNORE(iInfoBubble->SetTextL(aInfo));
    iHideInfoBubbleTemp = ETrue;
    SetInfoBubbleRect();
    }
void CFepLayoutMultiLineIcfEditor::HideBubble()
    {
    if ( iMsgBubble->IsShowing() )
        {
        iMsgBubble->Hide( ETrue );
        }
	iHideBubbleTemp = EFalse;
    }
void CFepLayoutMultiLineIcfEditor::HideInfoBubble()
    {
   if ( iInfoBubble->IsShowing() )
        {
        iInfoBubble->Hide( ETrue );
        }
    iHideInfoBubbleTemp = EFalse;
    }
    
void CFepLayoutMultiLineIcfEditor::ApplyFormatL()
    {
    if( !iFormatChange )
        {
        return;
        }
    //apply line space
    CParaFormat* paraFormat=CParaFormat::NewL();
    CleanupStack::PushL( paraFormat );
    TParaFormatMask paraFormatMask;
    paraFormat->iVerticalAlignment = CParaFormat::ECenterAlign;
    paraFormatMask.SetAttrib(EAttVerticalAlignment);
    paraFormat->iLineSpacingInTwips = iCoeEnv->ScreenDevice()->VerticalPixelsToTwips(
                                                               iLineSpace+iFontHeight);
    iFormatLineHeight = 0;
    
    paraFormatMask.SetAttrib(EAttLineSpacing);
    iRichText->ApplyParaFormatL(paraFormat,
	                            paraFormatMask,
	                            0,
	                            iRichText->DocumentLength());
    CleanupStack::PopAndDestroy(paraFormat);
    
    //apply font
    SetCursorSizeAndType();
    
    iFormatChange = EFalse;

    iTextView->FormatTextL();
    }

TInt CFepLayoutMultiLineIcfEditor::TextWidth( const TDesC& aText )
    {
    return iFont->TextWidthInPixels( aText );
    }
    
void CFepLayoutMultiLineIcfEditor::AdjustPromptTextL()
    {
    if ( iInitPromptText )
        {
        SetPromptTextL( *iInitPromptText, EFalse );	
        }
    }
    
void CFepLayoutMultiLineIcfEditor::ResetApplyFont()
    {
    TInt fontHeight = iFontHeight; 
    TInt maxFontHeight = iMaxFontHeight;
    const CFont *font = iFont;
    
    iFontHeight = 0;
    TRAP_IGNORE(SetFontL(fontHeight, maxFontHeight, font));
    }

void CFepLayoutMultiLineIcfEditor::TryDisplayMaxTextL( TInt aCursorPos )
    {
    iTextView->FinishBackgroundFormattingL();
    TInt lineIndex = iLayout->GetLineNumber( aCursorPos );

    if ( aCursorPos >= 0 )
        {
        TPoint cursorPt;
        TRAP_IGNORE(iTextView->DocPosToXyPosL(iCursorSel.iCursorPos, cursorPt));
        CalculateCursorPos(cursorPt);
        if ( !iViewRect.Contains( cursorPt ) && ( lineIndex + 1 ) > ( iLineMaxCount - 1 ) )
            {
            iTextView->SetViewLineAtTopL(( lineIndex + 1 ) - ( iLineMaxCount - 1 ));
            RecalcualteTextPositionL();
            SetMsgBubbleRect();
            }
        }
    
    const TInt lineCount = iLayout->NumFormattedLines();
   
    if( lineIndex == lineCount - 1 )
        {
        TInt pageCount = iViewRect.Height()/iFormatLineHeight;
        TPoint ptCursor = iInsertionPoint->Position() ;
        TInt lineCursor = ( ptCursor.iY + iInsertionPoint->Height() / 2 
                            - iViewRect.iTl.iY) / iFormatLineHeight + 1;
        if( lineCursor < pageCount )
            {
            
            iTextView->SetViewLineAtTopL( 
                                   lineCount - pageCount + 1 < 1 ? 1 : lineCount - pageCount + 1 );
            
            RecalcualteTextPositionL();
            SetMsgBubbleRect();
            }
        }
    }
    
void CFepLayoutMultiLineIcfEditor::DrawIcfBackground( CFbsBitGc& /*aGc*/, const TRect& aDrawnRect, 
                                                      TBool /*aBmpStretchFlag*/ )
	{
	DrawBackground( aDrawnRect );
	}
	
void CFepLayoutMultiLineIcfEditor::GraphicDeviceSizeChanged()
    {
    AdjustBmpDeviceIfNeeded();    
    }

void CFepLayoutMultiLineIcfEditor::ExtractText(TDes &aBuf, TInt aPos, TInt aLength)
    {
    if ( aPos < 0 )
        {
        aBuf = KNullDesC();
        return;
        }
    iRichText->Extract( aBuf, aPos + iPromptTextLen, aLength);
    } 

void CFepLayoutMultiLineIcfEditor::AdjustSelectionL( const TCursorSelection& aCurSel )
    {
    if (!IsTextPosValid(aCurSel.LowerPos()) || 
    !IsTextPosValid(aCurSel.HigherPos()) )
		{
		return;
		}

    iCursorSel = aCurSel;   
   
    if ( BelongToPromptText(aCurSel.iCursorPos) )
        {
        //pls refer to bug: ELWG-7MZ5EZ, why to use iPromptText->Length() instead of iPromptTextLen 
        //iCursorSel.iCursorPos = iPromptTextLen;
        iCursorSel.iCursorPos = iPromptText->Length();
        }
    if ( BelongToPromptText(aCurSel.iAnchorPos) )
        {
        //pls refer to bug: ELWG-7MZ5EZ, why to use iPromptText->Length() instead of iPromptTextLen
        //iCursorSel.iAnchorPos = iPromptTextLen;
        iCursorSel.iAnchorPos = iPromptText->Length();
        }
 
    iTextView->SetSelectionL( iCursorSel );
    iTextView->SetSelectionVisibilityL( iCursorSelVisible );
    }  

void CFepLayoutMultiLineIcfEditor::ReDrawRect(const TRect& aRect)
    {
    Draw();
    
    if(aRect.Intersects(iInsertionPoint->Rect()))
        {
        iInsertionPoint->ReDraw();
        }
    }
void CFepLayoutMultiLineIcfEditor::SetNumberGroupingL(TBool aIsNumberGrouping)
    {
    if(aIsNumberGrouping)
        {
        if(!iFormExtendedInterfaceProvider)
            {
            iFormExtendedInterfaceProvider = CAknEdwinFormExtendedInterfaceProvider::NewL();
            }
        if(!iPhoneNumberFormatter && iRichText)
            {
            CAknPhoneNumberInlineTextSource* phoneNumberFormatter = 
            CAknPlainPhoneNumberInlineTextSource::NewL( *iRichText );
            CleanupStack::PushL( phoneNumberFormatter );
            iFormExtendedInterfaceProvider->
                CompositeInlineTextSource().InstallInlineTextSourceL( phoneNumberFormatter );
            CleanupStack::Pop( phoneNumberFormatter );
            iPhoneNumberFormatter = phoneNumberFormatter;
            
            // regist custerm form
            iLayout->SetInterfaceProvider( iFormExtendedInterfaceProvider );
            }
        }
    else
        {
        // un-regist custerm form
        iLayout->SetInterfaceProvider( NULL );
        delete iFormExtendedInterfaceProvider;
        iFormExtendedInterfaceProvider = NULL;
        iPhoneNumberFormatter = NULL;
        }
    }

void CFepLayoutMultiLineIcfEditor::TrimCustomDrawText( TDes& aText )
    {
    TInt index = -1;
    const TText KTrimChar = 0xFFFF;
    for ( TInt i = 0; i < aText.Length() && aText[i] == KTrimChar; i++ )
        {        
        index = i;
        }
    if ( index != -1 )
        {
        aText.Delete( 0, index + 1 );
        }
    index = -1;
    for ( TInt i = aText.Length() - 1; i >= 0 && aText[i] == KTrimChar; i-- )
        {
        index = i;
        }
    if ( index != -1 )
        {
        aText.Delete( index, aText.Length() - index );
        }        
    }

TBool CFepLayoutMultiLineIcfEditor::SmileyEnabled()
    {
    return ETrue;
    }

CPeninputSmileyManager* CFepLayoutMultiLineIcfEditor::SmileyManager()
    {
    return iSmileyManager;
    }

void CFepLayoutMultiLineIcfEditor::CalcSmileyClipRegionL( RRegion& aRgn, 
                                                          CGraphicsContext& aGc, 
                                                          CFont& aFont,  
                                                          const TDesC& aText, 
                                                          TPoint& aBasePoint, 
                                                          TInt aTopLine, 
                                                          TInt aBottomLine )
    {
    TRect rectCurLine =iViewRect;
    rectCurLine.iTl.iY = aTopLine;
    rectCurLine.iBr.iY = aBottomLine;
    aRgn.AddRect( rectCurLine );
    
    HBufC* buf = aText.Alloc();
    CleanupStack::PushL( buf );
    TPtr text = buf->Des();
    TrimCustomDrawText( text );
    
    TInt docpos = iTextView->XyPosToDocPosL( aBasePoint );
    
    for ( TInt i = 0; i < text.Length(); i++ )
        {
        if ( iSmileyManager->IsSmileyCode( text[i] ) )
            {
            TPoint point(0,0);
            iTextView->DocPosToXyPosL( docpos + i, point );
            
            TInt charwidth = aFont.CharWidthInPixels( text[i] );
            
            TInt x = point.iX;
            TRect rc( TPoint(x,aTopLine), TSize(charwidth,aBottomLine - aTopLine) );
            aRgn.SubRect( rc, NULL );
            }
        }
    
    CleanupStack::PopAndDestroy( buf );
    }

void CFepLayoutMultiLineIcfEditor::CustomDrawSmileyL( CGraphicsContext& aGc, 
                                                      CFont& aFont, 
                                                      const TDesC& aText, 
                                                      TPoint& aBasePoint,
                                                      TInt aTopLine, 
                                                      TInt aBottomLine )
    {
    if( &aGc != static_cast<CGraphicsContext*>(iTextView->BitmapContext()) )
        {
        return;
        }
    
    CBitmapContext& gc = static_cast<CBitmapContext&>( aGc );
    
    TInt smileyWidth = aFont.TextWidthInPixels( iSmileyManager->SmileySizeTemplate() );
    
    TInt startDocPos = iTextView->XyPosToDocPosL( aBasePoint );
    
    HBufC* buf = aText.Alloc();
    CleanupStack::PushL( buf );
    
    TPtr text = buf->Des();
    
    TrimCustomDrawText( text );
    
    for ( TInt i = 0; i < text.Length(); i++ )
        {
        if ( iSmileyManager->IsSmileyCode( text[i] ) )
            {
            TPoint pt;
            iTextView->DocPosToXyPosL( startDocPos + i, pt );
            TInt x = pt.iX;
            TRect rect( x, aTopLine, x + smileyWidth, aBottomLine );
            
            TInt code = text[i];
            
            CPeninputSmileyImage* smiley = iSmileyManager->SmileyImage( code );
            if ( smiley )
                {
                gc.SetBrushColor( TRgb(255,0,0) );
                gc.SetBrushStyle( CGraphicsContext::ENullBrush );
                
                smiley->SetImageSize( rect.Size() );
                
                if ( smiley->MaskBitmap() )
                    {
                    TRect rcsrc( TPoint(0,0), rect.Size() );
                    gc.BitBltMasked( rect.iTl, smiley->ImageBitmap(), rcsrc, 
                                     smiley->MaskBitmap(), EFalse );
                    }
                else
                    {
                    gc.BitBlt( rect.iTl, smiley->ImageBitmap() );
                    }
                }
            }
        }
    
    CleanupStack::PopAndDestroy( buf );
    }

void CFepLayoutMultiLineIcfEditor::ApplyLineSpaceFormatL( TInt aLineHeight )
    {
    //apply line space
    CParaFormat* paraFormat=CParaFormat::NewL();
    CleanupStack::PushL( paraFormat );
    TParaFormatMask paraFormatMask;
    paraFormat->iLineSpacingInTwips = 
        iCoeEnv->ScreenDevice()->VerticalPixelsToTwips( aLineHeight );
    paraFormatMask.SetAttrib(EAttLineSpacing);
    iRichText->ApplyParaFormatL(paraFormat,
	                            paraFormatMask,
	                            0,
	                            iRichText->DocumentLength());
    CleanupStack::PopAndDestroy(paraFormat);
    
    iTextView->FormatTextL();
    }

void CFepLayoutMultiLineIcfEditor::CalcPromptTextRectL()
    {
    if ( iPromptTextLen <= 0 )
        {
        iPromptTextRect.SetSize( TSize( 0,0 ) );
        return;
        }
    
    TPoint tlBaseLine( 0,0 );
    TPoint brBaseLine( 0,0 );
    TPoint tl( 0,0 );
    TPoint br( 0,0 );
    TRAP_IGNORE( iTextView->DocPosToXyPosL(0, tlBaseLine) );
    TRAP_IGNORE( iTextView->DocPosToXyPosL(iPromptTextLen - 1, brBaseLine) );
    
    if ( tlBaseLine.iY != brBaseLine.iY )
        {
        return;
        }
    TInt topY = tlBaseLine.iY - iFont->AscentInPixels();
    TInt bottomY = br.iY = tlBaseLine.iY + iFont->DescentInPixels();
    
    iPromptTextRect = TRect( tlBaseLine.iX, topY, brBaseLine.iX, bottomY );
    }
// End Of File
