/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for base control
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

#include "peninputlayoutbubblectrl.h"

#include "peninputlayoutmultilineicf.h"
#include "peninputlayout.h"
#include "peninputlayoutcursor.h"
#include "peninputlayoutmicfcustomdraw.h"
#include "peninputlayoutmicfnomatchesinline.h"

#include <gdi.h>

EXPORT_C CFepLayoutMultiLineIcf* CFepLayoutMultiLineIcf::NewL(TRect aRect,
                                                              CFepUiLayout* aUiLayout,
                                                              TInt aControlId,
                                                              TInt aFontHeight,
                                                              TInt aMaxFontHeight,
                                                              const CFont* aFont)
    {
    CFepLayoutMultiLineIcf* self = new (ELeave) CFepLayoutMultiLineIcf(aUiLayout,aControlId);
    CleanupStack::PushL(self);
    self->BaseConstructL(aRect, aFontHeight, aMaxFontHeight, aFont);
    CleanupStack::Pop(self);
    return self;
    }

EXPORT_C CFepLayoutMultiLineIcf::CFepLayoutMultiLineIcf(CFepUiLayout* aUiLayout,
                                               TInt aControlId)
    :CControlGroup(aUiLayout,aControlId)
    {
    SetControlType(ECtrlEditAreaBase|ECtrlControlGroup);    
    }
    
EXPORT_C void CFepLayoutMultiLineIcf::BaseConstructL(TRect aRect,
                                            TInt aFontHeight, 
                                            TInt aMaxFontHeight, 
                                            const CFont* aFont)
    {
    CControlGroup::ConstructL();

    iMsgBubble = CBubbleCtrl::NewL(TRect(0,0,0,0),UiLayout(),-0xFFFE);

    iInfoBubble = CBubbleCtrl::NewL(TRect(0,0,0,0),UiLayout(),-0xFFFE);
    AddControlL(iMsgBubble);
    
    AddControlL(iInfoBubble);
    iIcfEditor = CFepLayoutMultiLineIcfEditor::NewL(aRect,UiLayout(),-0xFFFD,aFontHeight,
                                                    aMaxFontHeight,aFont);   
    iIcfEditor->SetMsgBubble(iMsgBubble);
    iIcfEditor->SetInfoBubble(iInfoBubble);
    AddControlL(iIcfEditor);
    iIcfEditor->AddEventObserver(this);
    //EnableTextSelection(EFalse);
    iTooltip = CTooltipBubbleCtrl::NewL( TRect(0,0,0,0), UiLayout(), -0xFFFC );
    iTooltip->AddEventObserver(this);
    iIcfEditor->SetTooltip( iTooltip );
    AddControlL( iTooltip );
    }

    
EXPORT_C void CFepLayoutMultiLineIcf::SetTextAlignmentL(TInt aAlignment, TInt aLanguage)
    {
    iIcfEditor->SetTextAlignmentL(aAlignment, aLanguage);
    }

EXPORT_C void CFepLayoutMultiLineIcf::SetTextColorL(const TRgb aColor)
    {
    iIcfEditor->SetTextColorL(aColor);
    }

EXPORT_C void CFepLayoutMultiLineIcf::SetTextSelColorL(const TRgb aColor)
    {
    iIcfEditor->SetTextSelColorL(aColor);    
    }

EXPORT_C void CFepLayoutMultiLineIcf::SetAutoCompleteTextColor(const TRgb aColor)
    {
    iIcfEditor->SetAutoCompleteTextColor(aColor);
    }

EXPORT_C TBool CFepLayoutMultiLineIcf::IsFocused()
    {
    return iIcfEditor->IsFocused();
    }


EXPORT_C const HBufC* CFepLayoutMultiLineIcf::PromptText()
	{
	return iIcfEditor->PromptText();	
	}
	
EXPORT_C void CFepLayoutMultiLineIcf::SetPromptTextL(const TDesC& aPromptText, TBool aCleanContent )
    {
    return iIcfEditor->SetPromptTextL(aPromptText, aCleanContent );
    }

EXPORT_C void CFepLayoutMultiLineIcf::SetTextL(const TFepInputContextFieldData& aData)
    {
    return iIcfEditor->SetTextL(aData);
    }

EXPORT_C void CFepLayoutMultiLineIcf::SetDimmed(TBool aDimFlag)
    {
    iIcfEditor->SetDimmed(aDimFlag);
    }

EXPORT_C void CFepLayoutMultiLineIcf::SetFontL(TInt aFontHeight, 
                                               TInt aMaxFontHeight, 
                                               const CFont* aFont)
    {
    iIcfEditor->SetFontL(aFontHeight,aMaxFontHeight,aFont);
    }

EXPORT_C void CFepLayoutMultiLineIcf::SetFocus(TBool aFlag)
    {
    iIcfEditor->SetFocus(aFlag);
    }

EXPORT_C void CFepLayoutMultiLineIcf::SetCursorVisible( TBool aCursorVisibleFlag )
    {
    iIcfEditor->SetCursorVisible(aCursorVisibleFlag);
    }

EXPORT_C TBool CFepLayoutMultiLineIcf::CursorVisible() const
    {
    return iIcfEditor->CursorVisible();
    }
  

EXPORT_C void CFepLayoutMultiLineIcf::SetRect(const TRect& aRect)
    {
    iIcfEditor->SetRect(aRect);
    CFepUiBaseCtrl::SetRect(aRect);
    //SetMsgBubbleRect();
    }

    
EXPORT_C void CFepLayoutMultiLineIcf::SetMsgBubbleCtrlSize(const TSize& aSize)
    {
    TPoint leftTop = TPoint(Rect().iBr.iX-iMsgBubble->Rect().Width(),Rect().iTl.iY);
    iMsgBubble->SetRect( TRect(leftTop,aSize) ) ;        
    }
EXPORT_C void CFepLayoutMultiLineIcf::SetInfoBubbleCtrlSize(const TSize& aSize)
    {
    TPoint leftTop = TPoint(Rect().iBr.iX-iInfoBubble->Rect().Width(),Rect().iTl.iY);
    iInfoBubble->SetRect( TRect(leftTop,aSize) ) ;        
    }


EXPORT_C void CFepLayoutMultiLineIcf::SizeChangedL(const TRect& aRect,
                                                   TInt aFontHeight,
                                                   TInt aMaxFontHeight,
                                                   const CFont* aFont)
    {   
    iIcfEditor->SizeChangedL(aRect,aFontHeight,aMaxFontHeight,aFont);
    CFepUiBaseCtrl::SetRect(aRect);
    return;
    }


EXPORT_C CFepUiBaseCtrl* CFepLayoutMultiLineIcf::HandlePointerDownEventL(const TPoint& aPt)
    {
    CFepUiBaseCtrl* ctrl = ControlForPoints( aPt );
    if ( ctrl == iTooltip )
    	{
        return iTooltip;	
    	}
    return iIcfEditor->HandlePointerDownEventL(aPt);
    }

EXPORT_C CFepUiBaseCtrl* CFepLayoutMultiLineIcf::HandlePointerUpEventL(const TPoint& aPt)
    {
    CFepUiBaseCtrl* ctrl = ControlForPoints( aPt );
    if ( ctrl == iTooltip )
    	{
        return iTooltip->HandlePointerUpEventL( aPt );	
    	}
    return iIcfEditor->HandlePointerUpEventL(aPt);    
    }

EXPORT_C CFepUiBaseCtrl* CFepLayoutMultiLineIcf::HandlePointerMoveEventL(const TPoint& aPt)
    {
    return iIcfEditor->HandlePointerMoveEventL(aPt);    
    }

EXPORT_C void CFepLayoutMultiLineIcf::CancelPointerDownL()
    {   
    iIcfEditor->CancelPointerDownL();
    }

EXPORT_C void CFepLayoutMultiLineIcf::HandlePointerLeave(const TPoint& aPt)
    {
    iIcfEditor->HandlePointerLeave(aPt);
    }

EXPORT_C void CFepLayoutMultiLineIcf::PrepareForFocusLoss()
    {
    iIcfEditor->PrepareForFocusLoss();
    //ReportEvent(EEventControlFocusLost);
    }

EXPORT_C TBool CFepLayoutMultiLineIcf::PrepareForFocusGain()
    {
    iIcfEditor->PrepareForFocusGain();    
    //ReportEvent(EEventControlFocusGained);

    return ETrue;
    }

EXPORT_C void CFepLayoutMultiLineIcf::Move(const TPoint& aOffset)
    {
    CFepUiBaseCtrl::Move(aOffset);
    iIcfEditor->Move(aOffset);
    //How about the msg bubble?
    }

EXPORT_C void CFepLayoutMultiLineIcf::HandleArrowBtnEventL(TArrowBtnEvent aEventType)
    {
    iIcfEditor->HandleArrowBtnEventL(aEventType);
    }

EXPORT_C TBool CFepLayoutMultiLineIcf::Contains(const TPoint& aPt)
    {
    return iIcfEditor->Contains(aPt);
    }

EXPORT_C void CFepLayoutMultiLineIcf::SetOverLapState(TBool aSupportOverLap, TRect aRect)
    {
    iIcfEditor->SetOverLapState(aSupportOverLap, aRect);
    }

EXPORT_C void CFepLayoutMultiLineIcf::AddOverLapState(TRect aRect)
    {
    iIcfEditor->AddOverLapState(aRect);
    }

EXPORT_C TInt CFepLayoutMultiLineIcf::GetEditorFieldMaxLen()
    {
    return iRect.Width()/2;
    }

EXPORT_C TBool CFepLayoutMultiLineIcf::IsEmpty()
    {
    return iIcfEditor->IsEmpty();
    }

EXPORT_C void CFepLayoutMultiLineIcf::InlineTextPos(TPoint& aPosBelow, TPoint& aPosRight)
    {
    iIcfEditor->InlineTextPos(aPosBelow,aPosRight);
    }
    
    
EXPORT_C CBubbleCtrl* CFepLayoutMultiLineIcf::MsgBubbleCtrl()
    {
    return iMsgBubble;
    }
EXPORT_C CBubbleCtrl* CFepLayoutMultiLineIcf::InfoBubbleCtrl() 
    {
    return iInfoBubble;
    }
EXPORT_C void CFepLayoutMultiLineIcf::ShowBubble(const TDesC& aText, const TRect& aRect)
    {
    return iIcfEditor->ShowBubble(aText,aRect);
    }
EXPORT_C void CFepLayoutMultiLineIcf::ShowByteWarningBubble(const TDesC& aInfo) 
    {
    iIcfEditor->ShowByteWarningBubble(aInfo);
    } 
    
EXPORT_C void CFepLayoutMultiLineIcf::HideBubble()
    {
    iIcfEditor->HideBubble();
    }
EXPORT_C void CFepLayoutMultiLineIcf::HideInfoBubble()
    {
    iIcfEditor->HideInfoBubble();
    }
    
EXPORT_C void CFepLayoutMultiLineIcf::ExtractText(TDes &aBuf, TInt aPos, TInt aLength)
    {
    iIcfEditor->ExtractText(aBuf, aPos, aLength);
    } 
    
    
EXPORT_C void CFepLayoutMultiLineIcf::EnableTextSelection(TBool aEnable)
    {
    return iIcfEditor->EnableTextSelection(aEnable);
    }
/*
EXPORT_C TBool CFepLayoutMultiLineIcf::TextSelectionOn()
    {
    return iIcfEditor->TextSelectionOn();
    }
  */
EXPORT_C TBool CFepLayoutMultiLineIcf::InlineStateOn()
    {
    return iIcfEditor->InlineStateOn();
    }
    
EXPORT_C void CFepLayoutMultiLineIcf::SetTextIsSecret(TBool aIsSecret)
    {
    return iIcfEditor->SetTextIsSecret(aIsSecret);
    }

EXPORT_C TBool CFepLayoutMultiLineIcf::TextIsSecret() const
    {
    return iIcfEditor->TextIsSecret();
    }

/*EXPORT_C TBool CFepLayoutMultiLineIcf::AutoCompleteStateOn()
    {
    return iIcfEditor->AutoCompleteStateOn();
    }
  */
EXPORT_C void CFepLayoutMultiLineIcf::SetBgImgSkinId(TAknsItemID aSkinId)
    {
    iIcfEditor->SetBgImgSkinId(aSkinId);
    }

EXPORT_C void CFepLayoutMultiLineIcf::SetLineSeparatorAfterPrompt(TBool aLineSeparator)
    {
    iIcfEditor->SetLineSeparatorAfterPrompt(aLineSeparator);
    }

EXPORT_C void CFepLayoutMultiLineIcf::SetLanguageId( TInt aLanguageId )
    {
    iIcfEditor->SetLanguageId(aLanguageId);
    }

EXPORT_C TBool CFepLayoutMultiLineIcf::NomatchState()
    {
    return iIcfEditor->NomatchState();
    }

EXPORT_C void CFepLayoutMultiLineIcf::SetTextMargin
    (TInt aLeftMargin, TInt aRightMargin, TInt aTopMargin, TInt aBottomMargin)
    {
    iIcfEditor->SetTextMargin(aLeftMargin, aRightMargin, aTopMargin, aBottomMargin);
    }

EXPORT_C void CFepLayoutMultiLineIcf::SetLineSpace( TInt aLineSpace )    
    {
    iIcfEditor->SetLineSpace( aLineSpace );
    }    
    
void CFepLayoutMultiLineIcf::UpdateValidRegion(CFepUiBaseCtrl* aCtrl,TBool aRemoveFlag)    
    {
    if(iIcfEditor)
        iIcfEditor->UpdateValidRegion(aCtrl,aRemoveFlag);
    }
    
EXPORT_C void CFepLayoutMultiLineIcf::HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* /*aCtrl*/,
                                                const TDesC& aEventData)
    {
    ReportEvent(aEventType,aEventData);    
    }

EXPORT_C void CFepLayoutMultiLineIcf::SetBackgroundBitmapL(CFbsBitmap* aBmp)
    {
    if(iIcfEditor)
        iIcfEditor->SetBackgroundBitmapL(aBmp);
    }
    
EXPORT_C void CFepLayoutMultiLineIcf::UpdateSecretTextL()
    {
    if(iIcfEditor)
        iIcfEditor->UpdateSecretTextL();
    }
    
EXPORT_C void CFepLayoutMultiLineIcf::ReDrawRect(const TRect& aRect)
    {   
	if(!AbleToDraw())
        return;
    
    if(aRect.IsEmpty())
        return;
    if(aRect == Rect())
        {
        Draw();    
        }
    else
        {   
        //find out the controls within the rect
        if(aRect.Intersects( iIcfEditor->Rect()))
            {
            iIcfEditor->ReDrawRect(aRect);
            if(iMsgBubble->IsShowing())
                {
                iMsgBubble->Draw();
                }
            if ( iTooltip && iTooltip->IsShowing() )
                {
                iTooltip->Draw();
                }    
            iInfoBubble->Draw();
            }            
        }        
            
    }    

EXPORT_C void CFepLayoutMultiLineIcf::SetNumberGrouping(TBool aIsNumberGrouping)
    {
    TRAP_IGNORE(iIcfEditor->SetNumberGroupingL(aIsNumberGrouping));
    }

// ---------------------------------------------------------------------------
// Return the pointer to Toolitp control
// ---------------------------------------------------------------------------
//
EXPORT_C CTooltipBubbleCtrl* CFepLayoutMultiLineIcf::TooltipBubbleCtrl()
    {
    return iTooltip;
    }

// ---------------------------------------------------------------------------
// Show tooltip of ICF
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutMultiLineIcf::ShowTooltipL( const TDesC& aText )
    {
    if ( !iTooltip || !iIcfEditor )
        {
        return;
        }
    iIcfEditor->ShowTooltipL( aText );
    }

// ---------------------------------------------------------------------------
// Hide tooltip of ICF
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepLayoutMultiLineIcf::HideTooltip()
    {
    if ( !iTooltip || !iIcfEditor  )
        {
        return;
        }    
    if ( iTooltip->IsShowing() )
        {
        iIcfEditor->HideTooltip();
        }   
    }

// ---------------------------------------------------------------------------
// Find what is the control of aPoint. 
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CFepLayoutMultiLineIcf::ControlForPoints( const TPoint& aPoint )
	{
	if ( !iTooltip || !iIcfEditor )
	    {
	    return this;
	    }
	if ( iTooltip->IsShowing() && iTooltip->Contains( aPoint ) )
		{
		return iTooltip;
		}
	if ( iIcfEditor->Contains( aPoint ) )
		{
		return iIcfEditor;
		}
	return this;
	}

// End Of File
