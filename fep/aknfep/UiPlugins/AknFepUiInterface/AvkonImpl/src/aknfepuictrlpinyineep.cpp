/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
 *      Provides the CAknFepUICtrlPinyinEEP definition 
 *
*/












#include <eikenv.h>
#include <eikappui.h>
#include <AknsSkinInstance.h> 
#include <AknsUtils.h> 
#include <AknsDrawUtils.h> 
#include <skinlayout.cdl.h>

#include "aknlayoutscalable_avkon.cdl.h"
#include "aknfepuictrlpinyineep.h"
#include "aknfepuilayoutdatamgr.h"
#include "aknfepchineseuidataconv.h"

_LIT(KFullStop, "\x002E");

_LIT( KToneMark1, "\x0020");
_LIT( KToneMark2, "\x02CA");
_LIT( KToneMark3, "\x02C7");
_LIT( KToneMark4, "\x02CB");
_LIT( KToneMark5, "\x02D9");
_LIT( KLeftBracket, "\x0028");
const TInt KControlNum = 1;
const TInt KMaxCharNum = 200;
const TInt KMaxDispNum = 200;

// ---------------------------------------------------------
// NewL function.
// ---------------------------------------------------------
//
CAknFepUICtrlPinyinEEP* CAknFepUICtrlPinyinEEP::NewL( RWindow& aParent, CAknFepUiLayoutDataMgr* aLafDataMgr )
    {
    CAknFepUICtrlPinyinEEP* self=CAknFepUICtrlPinyinEEP::NewLC( aParent, aLafDataMgr );
    CleanupStack::Pop(self); // self;
    return self;
    }

// ---------------------------------------------------------
// NewLC function.
// ---------------------------------------------------------
//
CAknFepUICtrlPinyinEEP* CAknFepUICtrlPinyinEEP::NewLC( RWindow& aParent, CAknFepUiLayoutDataMgr* aLafDataMgr )
    {
    CAknFepUICtrlPinyinEEP* self = new(ELeave)CAknFepUICtrlPinyinEEP( aLafDataMgr );
    CleanupStack::PushL( self );
    self->ConstructL( aParent, aLafDataMgr);
    return self;
    }

// ---------------------------------------------------------
// Destructor.
// ---------------------------------------------------------
//
CAknFepUICtrlPinyinEEP::~CAknFepUICtrlPinyinEEP()
    {
    if ( iEditableEntryPane )
        {
        delete iEditableEntryPane;
        iEditableEntryPane = NULL;
        }
//    if ( iBgContext )
//        {
//        delete iBgContext;
//        iBgContext = NULL;
//        }
    if ( iAbsoluteBuffer )
        {
        delete iAbsoluteBuffer;
        iAbsoluteBuffer = NULL;
        }
    if ( iVisualBuffer )
        {
        delete iVisualBuffer;
        iVisualBuffer = NULL;
        }
    iArray.ResetAndDestroy();
    delete iKeystrokeArray;
    delete iShowKeystrokeArray;
    delete iPhraseArray;
    delete iPhraseShowKeyStrokeArray;
    delete iTempArray;
    delete iPhraseZhuYinSpellingArray;
    }

// ---------------------------------------------------------
// From CCoeControl.
// ---------------------------------------------------------
//
TInt CAknFepUICtrlPinyinEEP::CountComponentControls() const
    {
    return KControlNum;
    }

// ---------------------------------------------------------
// From CCoeControl.
// ---------------------------------------------------------
//
CCoeControl* CAknFepUICtrlPinyinEEP::ComponentControl( TInt aIndex ) const
    {
    switch ( aIndex )
        {
        case EEditableEntryPane:
            return iEditableEntryPane;
        default:
            return NULL;
        }
    }

// ---------------------------------------------------------
// From CCoeControl.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::Draw( const TRect& /*aRect*/) const
    {
    }

// ---------------------------------------------------------
// From CCoeControl.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SizeChanged()
    {
    LayoutContainedControls();
    }

// ---------------------------------------------------------
// Set the text buffer and posit the cursor index.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetText( const TDesC& aDes, TInt aCursorIndex, TBool aMiniQwertyZhuyin )
    {
    iMiniQwertyZhuyin = aMiniQwertyZhuyin;
    iAbsoluteUnderlineBeg = -1;
    iAbsoluteUnderlineEnd = -1;
    iAbsoluteHighlightBeg = -1;
    iAbsoluteHighlightEnd = -1;
    iArray.ResetAndDestroy();
    TInt absoluteBufferLen = iAbsoluteBuffer->Length();
    TInt desLen = aDes.Length();

    if ( aCursorIndex > aDes.Length() )
        {
        aCursorIndex = aDes.Length();
        }
    else if ( aCursorIndex < 0 )
        {
        aCursorIndex = 0;
        }

    // cursor is at the end of buffer and add a character 
    if ( aDes.Left( iAbsoluteBuffer->Length() ) == *iAbsoluteBuffer
        && iAbsoluteCursorPos == iAbsoluteBuffer->Length() && aCursorIndex
        == aDes.Length() )
        {
        Append( aDes.Right( aDes.Length() - iAbsoluteBuffer->Length() ) );
        }
    //    delete one character
    else if ( iAbsoluteBuffer->Left( aCursorIndex ).Compare(aDes.Left(aCursorIndex)) == 0
        && aCursorIndex < iAbsoluteCursorPos
        && iAbsoluteBuffer->Compare( aDes ) != 0
        && 0 == iAbsoluteBuffer->Right( absoluteBufferLen - iAbsoluteCursorPos ).Compare( 
            aDes.Right( desLen - aCursorIndex)) )
        {
        Delete( iAbsoluteCursorPos - (iAbsoluteBuffer->Length()
            - aDes.Length()), iAbsoluteBuffer->Length() - aDes.Length() );
        }
    else if ( iAbsoluteBuffer->Length() < aDes.Length()
        && iAbsoluteBuffer->Left( iAbsoluteCursorPos )
            == aDes.Left( iAbsoluteCursorPos )
        && iAbsoluteBuffer->Right( iAbsoluteBuffer->Length()
            - iAbsoluteCursorPos ) == aDes.Right( iAbsoluteBuffer->Length()
            - iAbsoluteCursorPos ) && aCursorIndex - iAbsoluteCursorPos
        == aDes.Length() - iAbsoluteBuffer->Length() )
        {
        TPtrC tmp = aDes.Mid( iAbsoluteCursorPos, aDes.Length()
            - iAbsoluteBuffer->Length() );
        Insert( iAbsoluteCursorPos, tmp );
        }
    else
        {
        SetText( aDes );
        SetCursor( aCursorIndex );
        }
    }

// ---------------------------------------------------------
// Set the text buffer and posit the cursor index for zhuyin.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetTextForZhuyin( 
    const TDesC& aDes, 
    TInt aCursor, 
    TInt aChineseCount, 
    const TDesC& aJudgeSeed, 
    TInt aKeystroke, 
    TBool aInvalid,
    TBool aMiniQwertyZhuyin)
    {
    iMiniQwertyZhuyin = aMiniQwertyZhuyin;
    TInt actionType = EZhuyinNULL;
    if ( aKeystroke == 0 && aJudgeSeed.Compare(iJudgeSeed) == 0)
        {
        actionType = EZhuyinMoveCursor;
        }
    else if ( aChineseCount == iChineseCount + 1 )
        {
        actionType = EZhuyinNULL;
        }
    else if ( aKeystroke == 0 && aJudgeSeed.Length() <= iJudgeSeed.Length() - 1)
        {
        actionType = EZhuyinDelete;
        }
    else
        {
        if ( aJudgeSeed.Length() == iJudgeSeed.Length())
            {
            if ( aJudgeSeed.Left(aKeystroke - 1).Compare(iJudgeSeed.Left(aKeystroke - 1)) == 0 
                && aJudgeSeed.Mid(aKeystroke - 1,1).Compare(iJudgeSeed.Mid(aKeystroke - 1,1)) != 0 ) 
                {
                actionType = EZhuyinReplace;
                }
            else if ( aCursor == GetCursor() )
                {
                actionType = EZhuyinReplace;
                }
            else
                {
                actionType = EZhuyinMoveCursor;
                }
            }
        else if ( aJudgeSeed.Length() == iJudgeSeed.Length() + 1)
            {
            actionType = EZhuyinInsert;
            }
        else if ( aJudgeSeed.Length() == iJudgeSeed.Length() - 1)
            {
            actionType = EZhuyinDelete;
            }
        else if ( KErrNotFound != iJudgeSeed.Find( aJudgeSeed ) )
            {
            actionType = EZhuyinDelete;
            }
        else
            {
            actionType = EZhuyinNULL;
            }
        }
    iArray.ResetAndDestroy();
    iJudgeSeed = aJudgeSeed;
    iChineseCount = aChineseCount;
    if ( iLastValid && aInvalid )
        {
        TInt newOffset = 0;
        TPtrC ptr = iAbsoluteBuffer->Des();
        for ( TInt i = 0; i < iOffset; i++ )
            {
            if ( ptr.Mid(i,1).Compare(KFullStop) == 0 )
                {
                continue;
                }
            newOffset++;
            }
        iOffset = newOffset;
        }
    else if ( !iLastValid && !aInvalid )
        {
        TInt newOffset = 0;
        TInt i = 0;
        TPtrC ptr = aDes;
        for ( i = 0; i < iOffset && newOffset < ptr.Length(); )
            {
            if ( ptr.Mid(newOffset,1).Compare(KFullStop) != 0 )
                {
                i++;
                }
            newOffset++;
            }
        iOffset = newOffset;
        }
    iLastValid = !aInvalid;
    switch ( actionType )
        {
        case EZhuyinInsert:
            {
            *iAbsoluteBuffer = aDes;
            iAbsoluteCursorPos = aCursor;
            this->CalculateVisualTextForInsert( 0, aDes );
            this->iEditableEntryPane->SetText( *iVisualBuffer );
            this->CalculateVisualIndex();
            iEditableEntryPane->SetCursorPosition( iAbsoluteCursorPos
                - iOffset );
            }
            break;
        case EZhuyinReplace:
            {
            TRect panerect = iEditableEntryPane->Rect();
            TAknTextLineLayout textPaneTextLayout = 
                    CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData( EEEPTextLine ));
            const CFont* font = AknLayoutUtils::FontFromId(
                textPaneTextLayout.iFont, 
                NULL );
            TInt lenToCursor = font->TextWidthInPixels( aDes.Mid( iOffset,
                aCursor - iOffset ) );
            if ( lenToCursor < panerect.Width() - panerect.Width()
                *KLROffset/KDenominator * 2 )
                {
                *iAbsoluteBuffer = aDes;
                iAbsoluteCursorPos = aCursor;
                TInt stride = 0;
                for (stride = 1;; stride ++ )
                    {
                    if ( iOffset + stride > aDes.Length() )
                        {
                        break;
                        }
                    TInt tmpFontLen = font->TextWidthInPixels( aDes.Mid(
                        iOffset, stride ) );
                    if ( tmpFontLen > panerect.Width() - panerect.Width()
                        *KLROffset/KDenominator * 2 )
                        {
                        break;
                        }
                    }
                *iVisualBuffer = aDes.Mid( iOffset, stride - 1 );
                iEditableEntryPane->SetText( *iVisualBuffer );
                iEditableEntryPane->SetCursorPosition( aCursor-iOffset );
                }
            else
                {
                SetText( aDes, aCursor );
                }
            CalculateVisualIndex();
            break;
            }
        case EZhuyinDelete:
            {
            *iAbsoluteBuffer = aDes;
            iAbsoluteCursorPos = aCursor;
            this->CalculateVisualTextForDelete( 0, 0 );
            this->iEditableEntryPane->SetText( *iVisualBuffer );
            this->CalculateVisualIndex();
            iEditableEntryPane->SetCursorPosition( iAbsoluteCursorPos
                - iOffset );
            }
            break;
        default:
            SetText( aDes, aCursor );
        }
    }

// ---------------------------------------------------------
// Set the text buffer and maintain the current cursor position.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetText( const TDesC& aDes )
    {
    iAbsoluteCursorPos = 0;
    iOffset = 0;
    iAbsoluteBuffer->Des().Zero();
    iVisualBuffer->Des().Zero();
    *iAbsoluteBuffer = aDes;
    CalculateVisualTextForMoveRight();
    iEditableEntryPane->SetText( *iVisualBuffer );
    CalculateVisualIndex();
    }

// ---------------------------------------------------------
// Set the cursor position.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetCursor( const TInt aIndex )
    {
    SetCursorForMoveRight( aIndex );
    }

// ---------------------------------------------------------
// get the position index of cursor.The max cursor index must 
// be larger than the length of text buffer by 1.
// ---------------------------------------------------------
//
TInt CAknFepUICtrlPinyinEEP::GetCursor()
    {
    return iAbsoluteCursorPos;
    }

// ---------------------------------------------------------
// move cursor to the left by a 1 stride.
// ---------------------------------------------------------
//
TBool CAknFepUICtrlPinyinEEP::MoveCursorLeft( TInt aStride  )
    {
    if ( iAbsoluteCursorPos - aStride < 0 )
        {
        return EFalse;
        }
    if ( iEditableEntryPane->MoveCursorLeft( aStride ) == 0 )
        {
        iAbsoluteCursorPos = iAbsoluteCursorPos - aStride;
        }
    else
        {
        iAbsoluteCursorPos = iAbsoluteCursorPos - aStride;
        SetCursorForMoveLeft( iAbsoluteCursorPos );
        }
    return ETrue;
    }

// ---------------------------------------------------------
// move cursor to the right by a 1 stride.
// ---------------------------------------------------------
//
TBool CAknFepUICtrlPinyinEEP::MoveCursorRight( TInt aStride )
    {
    if ( iAbsoluteCursorPos + aStride > iAbsoluteBuffer->Length() )
        {
        return EFalse;
        }
    if ( iEditableEntryPane->MoveCursorRight( aStride ) == 0 )
        {
        iAbsoluteCursorPos = iAbsoluteCursorPos + aStride;
        }
    else
        {
        iAbsoluteCursorPos = iAbsoluteCursorPos + aStride;
        SetCursorForMoveRight( iAbsoluteCursorPos );
        }
    return ETrue;
    }

// ---------------------------------------------------------
// active the cursor,the cursor will be black and with a wink
// this cursor state represent this control is focused.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::ActiveCursor()
    {
    iEditableEntryPane->ActiveCursor();
    }

// ---------------------------------------------------------
// deactive the cursor, the cursor will be gray and no wink this cursor
// state represent this control with no focus.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::DeactiveCursor()
    {
    iEditableEntryPane->DeactiveCursor();
    }

// ---------------------------------------------------------
// enable the cursor,the cursor will be drawn.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::EnableCursor()
    {
    iEditableEntryPane->EnableCursor();
    }

// ---------------------------------------------------------
// deactive the cursor, the cursor will not be drawn.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::DisableCursor()
    {
    iEditableEntryPane->DisableCursor();
    }

// ---------------------------------------------------------
// set the beginning and ending index for the underline text.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetUnderline( TInt aBeg, TInt aEnd )
    {
    CAknFepUICtrlEEPControl::TSegment* tmp =
        new CAknFepUICtrlEEPControl::TSegment ( aBeg, aEnd, CAknFepUICtrlEEPControl::EUnderline );
    iArray.Append( tmp );

    TInt visualUnderlineBeg = aBeg - iOffset;
    TInt visualUnderlineEnd = aEnd - iOffset;

    if (
        visualUnderlineBeg < 0 &&
        visualUnderlineEnd < 0
        ||
        visualUnderlineBeg> iVisualBuffer->Length() - 1 &&
        visualUnderlineEnd> iVisualBuffer->Length() - 1
    )
        {
        visualUnderlineBeg = -1;
        visualUnderlineEnd = -1;
        }
    else
        {
        if ( visualUnderlineBeg < 0 )
            {
            visualUnderlineBeg = 0;
            }
        if ( visualUnderlineEnd> iVisualBuffer->Length() - 1 )
            {
            visualUnderlineEnd = iVisualBuffer->Length() - 1;
            }
        }
    iEditableEntryPane->SetUnderline( visualUnderlineBeg, visualUnderlineEnd );

    iAbsoluteUnderlineBeg = aBeg;
    iAbsoluteUnderlineEnd = aEnd;
    }

// ---------------------------------------------------------
// get the indexes for text with underline.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::GetUnderlineIndex( TInt& aBeg, TInt& aEnd )
    {
    aBeg = iAbsoluteUnderlineBeg;
    aEnd = iAbsoluteUnderlineEnd;
    }

// ---------------------------------------------------------
// set the indexes for the text with highlight
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetHighlight( TInt aBeg, TInt aEnd )
    {
    CAknFepUICtrlEEPControl::TSegment* tmp =
        new CAknFepUICtrlEEPControl::TSegment ( aBeg, aEnd, CAknFepUICtrlEEPControl::EHighlight );
    iArray.Append( tmp );

    TInt visualHighlightBeg = aBeg - iOffset;
    TInt visualHighlightEnd = aEnd - iOffset;

    if (
        visualHighlightBeg < 0 &&
        visualHighlightEnd < 0
        ||
        visualHighlightBeg> iVisualBuffer->Length() - 1 &&
        visualHighlightEnd> iVisualBuffer->Length() - 1
    )
        {
        visualHighlightBeg = -1;
        visualHighlightEnd = -1;
        }
    else
        {
        if ( visualHighlightBeg < 0 )
            {
            visualHighlightBeg = 0;
            }
        if ( visualHighlightEnd> iVisualBuffer->Length() - 1 )
            {
            visualHighlightEnd = iVisualBuffer->Length() - 1;
            }
        }
    iEditableEntryPane->SetHighlight( visualHighlightBeg, visualHighlightEnd );

    iAbsoluteHighlightBeg = aBeg;
    iAbsoluteHighlightEnd = aEnd;
    }

// ---------------------------------------------------------
// get the indexes for the text with highlight
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::GetHighlightIndex( TInt& aBeg, TInt& aEnd )
    {
    aBeg = iAbsoluteHighlightBeg;
    aEnd = iAbsoluteHighlightEnd;
    }

// ---------------------------------------------------------
// set keystroke index. 
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetCursorIndexOfKeystroke( const TInt aStartIndex )
    {
    iIndex = aStartIndex;
    }

// ---------------------------------------------------------
// get the keystroke index
// ---------------------------------------------------------
//
TInt CAknFepUICtrlPinyinEEP::GetCursorIndexOfKeystroke()
    {
    return iIndex;
    }

// ---------------------------------------------------------
// whether this control will be actived.ETrue represent enable 
// otherwise disable.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::Enable( TBool aEnable )
    {
    iEnabled = aEnable;
    if ( aEnable == EFalse )
        {
        Reset();
        DrawNow();
        }
    iEditableEntryPane->MakeVisible( aEnable );
    MakeVisible( aEnable );
    }
	
// ---------------------------------------------------------
// return whether this control is enabled.
// ---------------------------------------------------------
//
TBool CAknFepUICtrlPinyinEEP::IsEnabled() const
    {
    return iEnabled;
    }

// ---------------------------------------------------------
// Get the info before cursor.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::GetText( TDes& aText )
    {
    aText = *iAbsoluteBuffer;
    }

// ---------------------------------------------------------
// Get the info before cursor.
// ---------------------------------------------------------
//
TBool CAknFepUICtrlPinyinEEP::GetDesBeforeCursor( TDes& aOut )
    {
    if ( iAbsoluteCursorPos == 0 )
        {
        return EFalse;
        }
    else
        {
        aOut = iAbsoluteBuffer->Mid( (iAbsoluteCursorPos - 1 ), 1 );
        return ETrue;
        }
    }

// ---------------------------------------------------------
// From MAknFepUICtrlEditPane.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetToneMark( const TDesC& /*aToneMark*/)
    {
    }

// ---------------------------------------------------------
// From MAknFepUICtrlEditPane.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetToneMarkValidity( TBool /*aValid*/)
    {
    }

// ---------------------------------------------------------
// From MAknFepUICtrlEditPane.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetOverrideFontId( TInt /*aFontId*/)
    {
    }

// ---------------------------------------------------------
// From MAknFepUICtrlEditPane.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetFlag( TInt /*aFlag*/)
    {
    }

// ---------------------------------------------------------
// From MAknFepUICtrlEditPane.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::ClearFlag( TInt /*aFlag*/)
    {
    }

// ---------------------------------------------------------
// From MAknFepUICtrlEditPane.
// ---------------------------------------------------------
//
TBool CAknFepUICtrlPinyinEEP::IsFlagSet( TInt /*aFlag*/) const
    {
    return EFalse;
    }

// ---------------------------------------------------------
// Set the cursor index for moving to the left.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetCursorForMoveLeft( const TInt aIndex )
    {
    iAbsoluteCursorPos = aIndex;
    CalculateVisualTextForMoveLeft();
    iEditableEntryPane->SetText( *iVisualBuffer );
    iEditableEntryPane->SetCursorPosition( iAbsoluteCursorPos - iOffset );
    CalculateVisualIndex();
    }

// ---------------------------------------------------------
// Set the cursor index for moving to the right.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetCursorForMoveRight( const TInt aIndex )
    {
    iAbsoluteCursorPos = aIndex;
    CalculateVisualTextForMoveRight();
    iEditableEntryPane->SetText( *iVisualBuffer );
    iEditableEntryPane->SetCursorPosition( iAbsoluteCursorPos - iOffset );
    CalculateVisualIndex();
    }

// ---------------------------------------------------------
// Add some text at the end of buffer.
// ---------------------------------------------------------
//
TBool CAknFepUICtrlPinyinEEP::Append( const TDesC& aDes )
    {
    if ( this->iAbsoluteCursorPos != this->iAbsoluteBuffer->Length() )
        {
        return EFalse;
        }
    else
        {
        if ( iAbsoluteBuffer->Length() + aDes.Length()
            > iAbsoluteBuffer->Des().MaxLength() )
            {
            return EFalse;
            }
        iAbsoluteBuffer->Des().Append( aDes );
        iAbsoluteCursorPos = iAbsoluteCursorPos + aDes.Length();
        this->CalculateVisualTextForAppend( aDes );
        this->iEditableEntryPane->SetText( *iVisualBuffer );
        this->CalculateVisualIndex();
        iEditableEntryPane->SetCursorPosition( iAbsoluteCursorPos - iOffset );
        return ETrue;
        }
    }

// ---------------------------------------------------------
// delete the content begin with aBeg index, and with an aLen length.
// ---------------------------------------------------------
//
TBool CAknFepUICtrlPinyinEEP::Delete( TInt aBeg, TInt aLen )
    {
    if ( aBeg < 0 || aLen < 0 || aBeg + aLen> iAbsoluteBuffer->Length() )
        {
        return EFalse;
        }
    else
        {
        iAbsoluteBuffer->Des().Delete( aBeg, aLen );
        iAbsoluteCursorPos = aBeg;
        this->CalculateVisualTextForDelete( aBeg, aLen );
        this->iEditableEntryPane->SetText( *iVisualBuffer );
        this->CalculateVisualIndex();
        iEditableEntryPane->SetCursorPosition( iAbsoluteCursorPos - iOffset );
        return ETrue;
        }
    }

// ---------------------------------------------------------
// Insert some text after aIndex character.
// ---------------------------------------------------------
//
TBool CAknFepUICtrlPinyinEEP::Insert( TInt aIndex, const TDesC& aDes )
    {
    if ( iAbsoluteBuffer->Length() + aDes.Length() > iAbsoluteBuffer->Des().MaxLength() )
        {
        return EFalse;
        }
    iAbsoluteBuffer->Des().Insert( aIndex, aDes );
    iAbsoluteCursorPos = iAbsoluteCursorPos + aDes.Length();
    this->CalculateVisualTextForInsert( aIndex, aDes );
    this->iEditableEntryPane->SetText( *iVisualBuffer );
    this->CalculateVisualIndex();
    iEditableEntryPane->SetCursorPosition( iAbsoluteCursorPos - iOffset );
    return ETrue;
    }

// ---------------------------------------------------------
// Replace text from aBeg index to aBeg + aLen with aDes.
// ---------------------------------------------------------
//
TBool CAknFepUICtrlPinyinEEP::Replace( TInt /*aBeg*/, TInt /*aLen*/,
    const TDesC& /*aDes*/)
    {
    return EFalse;
    }

// ---------------------------------------------------------
// Reset private member viariable.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::Reset()
    {
    iIndex = 0;
    iEnabled = EFalse;
    iAbsoluteCursorPos = 0;
    iOffset = 0;
    iAbsoluteUnderlineBeg = -1;
    iAbsoluteUnderlineEnd = -1;
    iAbsoluteHighlightBeg = -1;
    iAbsoluteHighlightEnd = -1;
    iAbsoluteBuffer->Des().Zero();
    iVisualBuffer->Des().Zero();
    iEditableEntryPane->ResetAll();
    iArray.ResetAndDestroy();
    iJudgeSeed.Zero();
    iLastValid = ETrue;
    iChineseCount = 0;
    }

// ---------------------------------------------------------
// Layout contained controls.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::LayoutContainedControls()
    {
    TRect outerRect;
    TRect innerRect;
    CalculateFrameRects( outerRect, innerRect );
    TRect rect = Rect();
    iEditableEntryPane->SetRect( rect );
    }

// ---------------------------------------------------------
// layout the background frames.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::LayoutFrames()
    {
    TRect outerRect;
    TRect innerRect;
//    CalculateFrameRects( outerRect, innerRect );
//    iBgContext->SetFrameRects( outerRect, innerRect );
//    iBgContext->SetParentPos( PositionRelativeToScreen() );
    }

// ---------------------------------------------------------
// layout its rectangles
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::LayoutRects()
    {
    TRect windowRect = 
        CAknFepChineseUIDataConv::AnyToRect( iLafDataMgr->RequestData( EEEPWindowRect ));
    TInt height = windowRect.Height();
    TInt width = windowRect.Width();
    windowRect.iTl.iY = 0;
    windowRect.iTl.iX = 0;
    windowRect.SetHeight( height );
    windowRect.SetWidth( width );
    iSize.SetSize( width, Rect().Height() );
    }

// ---------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------
//
CAknFepUICtrlPinyinEEP::CAknFepUICtrlPinyinEEP( CAknFepUiLayoutDataMgr* aLafDataMgr )
    {
    iIndex = 0;
    iEnabled = EFalse;
    iAbsoluteCursorPos = 0;
    iAbsoluteUnderlineBeg = -1;
    iAbsoluteUnderlineEnd = -1;
    iAbsoluteHighlightBeg = -1;
    iAbsoluteHighlightEnd = -1;
    iOffset = 0;
    iJudgeSeed.Zero();
    iLastValid = ETrue;
    iChineseCount = 0;
    iChangeFlag = 0;
    iAutoChangeFlag = 1;
    iAllValidFlag = 1;
    iInvalidKeystrokeIndex = 0;
    iCandidateRefreshFlag = 0;
    iDelimiterCount = 0;
    iLafDataMgr = aLafDataMgr;
    }

// ---------------------------------------------------------
// Two-phrase constructor.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::ConstructL( RWindow& aParent, CAknFepUiLayoutDataMgr* aLafDataMgr )
    {
    SetContainerWindowL( aParent );
    iEditableEntryPane = CAknFepUICtrlEEPControl::NewL( Window(), aLafDataMgr );
    iAbsoluteBuffer = HBufC::New( KMaxCharNum );
    iVisualBuffer = HBufC::New( KMaxDispNum );
    TRect outerRect = TRect();
    TRect innerRect = TRect();
    CalculateFrameRects( outerRect, innerRect );
//    iBgContext = CAknsFrameBackgroundControlContext::NewL(
//        KAknsIIDQsnFrPopupSub, outerRect, innerRect, ETrue );
    iKeystrokeArray = new(ELeave)CDesCArrayFlat(1);
    iShowKeystrokeArray = new(ELeave)CDesCArrayFlat(1);
    iPhraseArray = new(ELeave)CDesCArrayFlat(1);
    iPhraseShowKeyStrokeArray = new(ELeave)CDesCArrayFlat(1);
    iTempArray = new(ELeave)CDesCArrayFlat(1);
    iMiniQwertyZhuyin = EFalse;
    iNeedClearDeliberateSelection = EFalse;
    iPhraseZhuYinSpellingArray =new(ELeave)CDesCArrayFlat(1);
    }

// ---------------------------------------------------------
// Calculate visual text for appending.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::CalculateVisualTextForAppend( const TDesC& aDes )
    {
    TRect panerect = iEditableEntryPane->Rect();
    TAknTextLineLayout textPaneTextLayout = 
            CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData( EEEPTextLine ));
    const CFont* font = AknLayoutUtils::FontFromId( textPaneTextLayout.iFont, 
    NULL );

    TInt lenToEnd = font->TextWidthInPixels( iAbsoluteBuffer->Right( iVisualBuffer->Length()
        + aDes.Length() ) );
    if ( lenToEnd <= panerect.Width() - panerect.Width()*KLROffset
        /KDenominator * 2 )
        {
        *iVisualBuffer = iAbsoluteBuffer->Right( iVisualBuffer->Length()
            + aDes.Length() );
        }
    else
        {
        this->CalculateVisualTextForMoveRight();
        }
    }

// ---------------------------------------------------------
// Calculate visual text for deleting.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::CalculateVisualTextForDelete( TInt /*aBeg*/,
    TInt /*aLen*/)
    {
    TRect panerect = iEditableEntryPane->Rect();
    TAknTextLineLayout textPaneTextLayout = 
            CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData( EEEPTextLine ));
    const CFont* font = AknLayoutUtils::FontFromId( textPaneTextLayout.iFont, 
    NULL );
    iOffset = iOffset > iAbsoluteBuffer->Length()?iAbsoluteBuffer->Length():iOffset;
    TInt lenToEnd = font->TextWidthInPixels( iAbsoluteBuffer->Right( iAbsoluteBuffer->Length()
        - iOffset ) );
    
    TInt offsetLen = iAbsoluteCursorPos - iOffset;
    if ( offsetLen < 0 )
        {
        offsetLen = 0;
        }
    TInt LenOffsetToCursor = font->TextWidthInPixels( iAbsoluteBuffer->Mid( 
            iOffset, offsetLen ) );

    if ( lenToEnd > 0 && iAbsoluteCursorPos >= iOffset && LenOffsetToCursor > panerect.Width()*KLROffset/KDenominator )
        {
        TInt stride = 0;
        for (stride = 1;; stride ++ )
            {
            if ( iOffset + stride > iAbsoluteBuffer->Length() )
                {
                break;
                }
            TInt tmpFontLen = font->TextWidthInPixels( iAbsoluteBuffer->Mid(
                iOffset, stride ) );
            if ( tmpFontLen > panerect.Width() - panerect.Width()
                *KLROffset/KDenominator * 2 )
                {
                break;
                }
            }
        *iVisualBuffer = iAbsoluteBuffer->Mid( iOffset, stride - 1 );
        }
    else
        {
        this->CalculateVisualTextForMoveLeft();
        }
    }

// ---------------------------------------------------------
// Calculate visual text for inserting.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::CalculateVisualTextForInsert( TInt /*aIndex*/,
    const TDesC& /*aDes*/)
    {
    TRect panerect = iEditableEntryPane->Rect();
    TAknTextLineLayout textPaneTextLayout = 
            CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData( EEEPTextLine ));
    const CFont* font = AknLayoutUtils::FontFromId( textPaneTextLayout.iFont, 
    NULL );
    
    TInt offsetLen = iAbsoluteCursorPos - iOffset;
    if ( offsetLen < 0 )
        {
        offsetLen = 0;
        }

    TInt lenToCursor = font->TextWidthInPixels( iAbsoluteBuffer->Mid(
        iOffset, offsetLen ) );
    if ( lenToCursor < panerect.Width()* KCursorResetStartOffset / KDenominator
        || iAbsoluteCursorPos == iAbsoluteBuffer->Length()
        && lenToCursor <= panerect.Width() - panerect.Width()*KLROffset/KDenominator * 2)
        {
        TInt stride = 0;
        for (stride = 1;; stride ++ )
            {
            if ( iOffset + stride > iAbsoluteBuffer->Length() )
                {
                break;
                }
            TInt tmpFontLen = font->TextWidthInPixels( iAbsoluteBuffer->Mid(
                iOffset, stride ) );
            if ( tmpFontLen > panerect.Width() - panerect.Width()
                *KLROffset/KDenominator * 2 )
                {
                break;
                }
            }
        *iVisualBuffer = iAbsoluteBuffer->Mid( iOffset, stride - 1 );
        }
    else
        {
        this->CalculateVisualTextForMoveRight();
        }
    }

// ---------------------------------------------------------
// Calculate visual text for replacing.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::CalculateVisualTextForReplace( TInt /*aBeg*/,
    TInt /*aLen*/, const TDesC& /*aDes*/)
    {
    }

// ---------------------------------------------------------
// Calculate visual text for moving to left.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::CalculateVisualTextForMoveLeft()
    {
    TRect panerect = iEditableEntryPane->Rect();
    TAknTextLineLayout textPaneTextLayout = 
            CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData( EEEPTextLine ));
    const CFont* font = AknLayoutUtils::FontFromId( textPaneTextLayout.iFont, 
    NULL );
    TInt absolutelength = font->TextWidthInPixels( *iAbsoluteBuffer );
    TInt
        cursorToBegLength =
            font->TextWidthInPixels( iAbsoluteBuffer->Left( iAbsoluteCursorPos ) );
    if ( iAbsoluteCursorPos > iAbsoluteBuffer->Length() + 1 )
        {
        iAbsoluteCursorPos = iAbsoluteBuffer->Length() + 1;
        }
    TInt lenPixelToVisualBeg = panerect.Width() * KCursorResetOffset
        / KDenominator;
    TInt stride = 0;
    TInt visualBeg = 0;
    TInt visualEnd = 0;
    for (stride = 1;; stride ++ )
        {
        if ( 0 > iAbsoluteCursorPos - stride )
            {
            visualBeg = 0;
            break;
            }
        TInt tmpFontLen = font->TextWidthInPixels( iAbsoluteBuffer->Mid(
            iAbsoluteCursorPos - stride, stride ) );
        if ( tmpFontLen > lenPixelToVisualBeg )
            {
            visualBeg = iAbsoluteCursorPos - stride + 1;
            break;
            }
        }
        
    //let adjustment of visualBeg before caculation of visualEnd. 
    //For one, calculation always start before "(" for miniqwertyzhuyin
    //For two, because characters(markers) might have different length in pixel, it's better recalculate visualEnd
    // after changing visualBeg, therefore move changing visualBeg ahead of caculation take effect too.
    if ( iMiniQwertyZhuyin && 
        iAbsoluteBuffer->Length() > 0 && 
        visualBeg >= iPhraseArray->Count() )
        {
        while ( visualBeg > 0 &&
            iAbsoluteBuffer->Mid( visualBeg, 1 ).Compare( KLeftBracket ) != 0 &&
            iAbsoluteBuffer->Mid( visualBeg, 1 ).Compare( KToneMark1 ) != 0 &&
            iAbsoluteBuffer->Mid( visualBeg, 1 ).Compare( KToneMark2 ) != 0 &&
            iAbsoluteBuffer->Mid( visualBeg, 1 ).Compare( KToneMark3 ) != 0 &&
            iAbsoluteBuffer->Mid( visualBeg, 1 ).Compare( KToneMark4 ) != 0 &&
            iAbsoluteBuffer->Mid( visualBeg, 1 ).Compare( KToneMark5 ) != 0 )
            {
            visualBeg--;
            }
        }

    for (stride = 1;; stride ++ )
        {
        if ( visualBeg + stride > iAbsoluteBuffer->Length() )
            {
            visualEnd = visualBeg + stride - 1;
            break;
            }
        TPtrC tempString = iAbsoluteBuffer->Mid(visualBeg, stride );
        TInt tmpFontLen = font->TextWidthInPixels( tempString );
        if ( tmpFontLen > panerect.Width() - panerect.Width() *KLROffset
            /KDenominator * 2 )
            {
            visualEnd = visualBeg + stride - 1;
            //visualEnd = visualBeg + stride;
            break;
            }
        }
    *iVisualBuffer = iAbsoluteBuffer->Mid( visualBeg, visualEnd - visualBeg );
    this->iOffset = visualBeg;
    }

// ---------------------------------------------------------
// Calculate visual text for moving to the right.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::CalculateVisualTextForMoveRight()
    {
    TRect panerect = iEditableEntryPane->Rect();
    TAknTextLineLayout textPaneTextLayout = 
            CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData( EEEPTextLine ));
    const CFont* font = AknLayoutUtils::FontFromId( textPaneTextLayout.iFont, 
    NULL );
    TInt absolutelength = font->TextWidthInPixels( *iAbsoluteBuffer );
    TInt cursorToEndLength =
        font->TextWidthInPixels( iAbsoluteBuffer->Mid( iAbsoluteCursorPos,
            iAbsoluteBuffer->Length() - iAbsoluteCursorPos ) );
    if ( iAbsoluteCursorPos > iAbsoluteBuffer->Length() + 1 )
        {
        iAbsoluteCursorPos = iAbsoluteBuffer->Length() + 1;
        }
    if ( absolutelength <= panerect.Width() - panerect.Width()*KLROffset
        /KDenominator * 2 )
        {
        *iVisualBuffer = *iAbsoluteBuffer;
        }
    else if ( cursorToEndLength < panerect.Width() * KCursorResetOffset
        / KDenominator )
        {
        TInt tmp70VisualEnd = iAbsoluteCursorPos;
        TInt visualBeg = 0;
        TInt visualEnd = 0;
        TInt stride = 0;
        visualEnd = iAbsoluteBuffer->Length();
        for (stride = 1;; stride ++ )
            {
            TInt tmpFontLen = font->TextWidthInPixels( iAbsoluteBuffer->Mid(
                tmp70VisualEnd - stride, stride ) );
            if ( tmpFontLen > panerect.Width() * (KDenominator - 
                KLROffset * 2 - KCursorResetOffset ) / KDenominator )
                {
                visualBeg = tmp70VisualEnd - stride + 1;
                break;
                }
            }
        if ( iMiniQwertyZhuyin &&
            iAbsoluteBuffer->Length( ) > 0 &&
            visualBeg >= iPhraseArray->Count() )
            {
            while ( iAbsoluteBuffer->Mid( visualBeg, 1 ).Compare( KLeftBracket ) != 0 &&
                iAbsoluteBuffer->Mid( visualBeg, 1 ).Compare( KToneMark1 ) != 0 &&
                iAbsoluteBuffer->Mid( visualBeg, 1 ).Compare( KToneMark2 ) != 0 &&
                iAbsoluteBuffer->Mid( visualBeg, 1 ).Compare( KToneMark3 ) != 0 &&
                iAbsoluteBuffer->Mid( visualBeg, 1 ).Compare( KToneMark4 ) != 0 &&
                iAbsoluteBuffer->Mid( visualBeg, 1 ).Compare( KToneMark5 ) != 0 )
                {
                visualBeg++;
                //                visualEnd++;
                }
            }
        *iVisualBuffer = iAbsoluteBuffer->Right( visualEnd - visualBeg );
        this->iOffset = visualBeg;
        }
    else
        {
        TInt absoluteOffset = font->TextWidthInPixels( 
            iAbsoluteBuffer->Left( iAbsoluteCursorPos ) );
        if ( absoluteOffset < panerect.Width() * 
            KCursorResetStartOffset / KDenominator)
            {
            TInt stride = 0;
            for (stride = 1;; stride ++ )
                {
                TInt tmpFontLen = font->TextWidthInPixels( 
                    iAbsoluteBuffer->Left( stride ) );
                if ( tmpFontLen > panerect.Width() - panerect.Width()
                    *KLROffset /KDenominator * 2 )
                    {
                    *iVisualBuffer = iAbsoluteBuffer->Left( stride - 1 );
                    this->iOffset = 0;
                    break;
                    }
                }
            }
        else
            {
            TInt lenPixelToVisualEnd = panerect.Width() * KCursorResetOffset
                / KDenominator;
            TInt stride = 0;
            TInt visualBeg = 0;
            TInt visualEnd = 0;
            for (stride = 1;; stride ++ )
                {
                if ( iAbsoluteBuffer->Length() < iAbsoluteCursorPos + stride )
                    {
                    visualEnd = iAbsoluteBuffer->Length();
                    break;
                    }
                TInt tmpFontLen = font->TextWidthInPixels( 
                    iAbsoluteBuffer->Mid( iAbsoluteCursorPos, stride ) );
                if ( tmpFontLen > lenPixelToVisualEnd )
                    {
                    visualEnd = iAbsoluteCursorPos + stride - 1;
                    break;
                    }
                }
            for (stride = 1;; stride ++ )
                {
                TInt tmpFontLen = font->TextWidthInPixels( 
                    iAbsoluteBuffer->Mid( visualEnd - stride, stride ) );
                if ( tmpFontLen > panerect.Width() - panerect.Width()
                    *KLROffset /KDenominator * 2 )
                    {
                    visualBeg = visualEnd - stride + 1;
                    break;
                    }
                }
            if ( iMiniQwertyZhuyin && 
                iAbsoluteBuffer->Length( ) > 0 &&
                visualBeg >= iPhraseArray->Count() )
                {
                while ( iAbsoluteBuffer->Mid( visualBeg, 1 ).Compare( KLeftBracket ) != 0 &&
                    iAbsoluteBuffer->Mid( visualBeg, 1 ).Compare( KToneMark1 ) != 0 &&
                    iAbsoluteBuffer->Mid( visualBeg, 1 ).Compare( KToneMark2 ) != 0 &&
                    iAbsoluteBuffer->Mid( visualBeg, 1 ).Compare( KToneMark3 ) != 0 &&
                    iAbsoluteBuffer->Mid( visualBeg, 1 ).Compare( KToneMark4 ) != 0 &&
                    iAbsoluteBuffer->Mid( visualBeg, 1 ).Compare( KToneMark5 ) != 0 )
                    {
                    visualBeg++;
                    //                    visualEnd++;
                    }
                }
            *iVisualBuffer = iAbsoluteBuffer->Mid( visualBeg, visualEnd
                - visualBeg );
            this->iOffset = visualBeg;
            }
        }
    }

// ---------------------------------------------------------
// Calculate the underline indexes and highlight indexes according to the
// absolute value.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::CalculateVisualIndex()
    {
    for (TInt i = 0; i < iArray.Count(); i++ )
        {
        TInt left = iArray[i]->iBegin - iOffset;
        TInt right = iArray[i]->iEnd - iOffset;
        if (
            left < 0 &&
            right < 0
            ||
            left> iVisualBuffer->Length() - 1 &&
            right> iVisualBuffer->Length() - 1
           )
            {
            left = -1;
            right = -1;
            }
        else
            {
            if ( left < 0 )
                {
                left = 0;
                }
            if ( right> iVisualBuffer->Length() - 1 )
                {
                right = iVisualBuffer->Length() - 1;
                }
            }
        if ( iArray[i]->iStyle == CAknFepUICtrlEEPControl::EHighlight )
            {
            iEditableEntryPane->SetHighlight( left, right );
            }
        else if ( iArray[i]->iStyle == CAknFepUICtrlEEPControl::EUnderline )
            {
            iEditableEntryPane->SetUnderline( left, right );
            }
        }
    }

// ---------------------------------------------------------
// layout the background frames.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::CalculateFrameRects( TRect& aOuterRect,
    TRect& aInnerRect ) const
    {
    TRect windowRect = CAknFepChineseUIDataConv::AnyToRect( iLafDataMgr->RequestData( EPopupRectEEP ));
    TAknLayoutRect eepFirstHorizLineLayoutRect = CAknFepChineseUIDataConv::AnyToLayoutRect( iLafDataMgr->RequestData( EEEPFirstHorizLine ));
    TRect eepFirstHorizLineRect = eepFirstHorizLineLayoutRect.Rect();
    TInt rectHeight = eepFirstHorizLineRect.iBr.iY - windowRect.iTl.iY + 2;
    TInt rectWidth = windowRect.Width( );
    windowRect.iTl.iX = 0;
    windowRect.iTl.iY = 0;
    windowRect.SetHeight( rectHeight );
    windowRect.SetWidth( rectWidth );

    TAknLayoutRect topLeft;
    topLeft.LayoutRect( windowRect,
        SkinLayout::Submenu_skin_placing_Line_2() );

    TAknLayoutRect bottomRight;
    bottomRight.LayoutRect( windowRect,
        SkinLayout::Submenu_skin_placing_Line_5() );

    aOuterRect = TRect( topLeft.Rect().iTl, bottomRight.Rect().iBr );
    aInnerRect = TRect( topLeft.Rect().iBr, bottomRight.Rect().iTl );
    }

// ---------------------------------------------------------
// Return kestroke array.
// ---------------------------------------------------------
//
CDesCArrayFlat* CAknFepUICtrlPinyinEEP::KeystrokeArray()
    {
    return iKeystrokeArray;
    }

// ---------------------------------------------------------
// Return kescode array.
// ---------------------------------------------------------
//
RArray<TInt>* CAknFepUICtrlPinyinEEP::KeycodeArray()
    {
    return &iKeycodeArray;
    }

// ---------------------------------------------------------
// Return kescode array.
// ---------------------------------------------------------
//
RArray<TInt>* CAknFepUICtrlPinyinEEP::PhraseKeycodeArray()
    {
    return &iPhraseKeycodeArray;
    }

// ---------------------------------------------------------
// Return show kestroke array.
// ---------------------------------------------------------
//
CDesCArrayFlat* CAknFepUICtrlPinyinEEP::ShowKeystrokeArray()
    {
    return iShowKeystrokeArray;
    }

// ---------------------------------------------------------
// Return choose chinese charater array.
// ---------------------------------------------------------
//
CDesCArrayFlat* CAknFepUICtrlPinyinEEP::PhraseArray()
    {
    return iPhraseArray;
    }

// ---------------------------------------------------------
// Return choose chinese charater show keystroke array.
// ---------------------------------------------------------
//
CDesCArrayFlat* CAknFepUICtrlPinyinEEP::PhraseShowKeyStrokeArray()
    {
    return iPhraseShowKeyStrokeArray;
    }
// ---------------------------------------------------------
// Return temp array.
// ---------------------------------------------------------
//
CDesCArrayFlat* CAknFepUICtrlPinyinEEP::TempArray()
    {
    return iTempArray;
    }

// ---------------------------------------------------------
// Reset private member viariable array.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::ResetAllArray()
    {
    iKeystrokeArray->Reset();
    iKeystrokeArray->Compress();
    iKeycodeArray.Reset();
    iShowKeystrokeArray->Reset();
    iShowKeystrokeArray->Compress();
    iPhraseArray->Reset();
    iPhraseArray->Compress();
    iPhraseKeycodeArray.Reset();
    iPhraseShowKeyStrokeArray->Reset();
    iPhraseShowKeyStrokeArray->Compress();
    iTempArray->Reset();
    iTempArray->Compress();
    iAllValidFlag = 1;
    iInvalidKeystrokeIndex = 0;
    iCandidateRefreshFlag = 0;
    iDelimiterCount = 0;
    iPhraseZhuYinSpellingArray->Reset();
    iPhraseZhuYinSpellingArray->Compress();
    }

// ---------------------------------------------------------
// change state flag.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetChangeState( TBool aState )
    {
    iChangeFlag = aState;
    }

// ---------------------------------------------------------
// check change state flag.
// ---------------------------------------------------------
//
TBool CAknFepUICtrlPinyinEEP::IsChangeState()
    {
    return iChangeFlag;
    }

// ---------------------------------------------------------
// Set auto change state flag.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetAutoChangeStateFlag( TBool aState )
    {
    iAutoChangeFlag = aState;
    }

// ---------------------------------------------------------
// Set auto change state flag.
// ---------------------------------------------------------
//
TBool CAknFepUICtrlPinyinEEP::GetAutoChangeStateFlag()

    {
    return iAutoChangeFlag;
    }

// ---------------------------------------------------------
// Set all the keystroke valid flag.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetAllValidFlag( TBool aState )
    {
    iAllValidFlag = aState;
    }

// ---------------------------------------------------------
// Set all the keystroke valid flag.
// ---------------------------------------------------------
//
TBool CAknFepUICtrlPinyinEEP::GetAllValidFlag()
    {
    return iAllValidFlag;
    }

// ---------------------------------------------------------
// Set is Phrase creation.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetPhraseCreationFlag( TBool aState )
    {
    iPhraseCreation = aState;
    }

// ---------------------------------------------------------
// Is Phrase creation.
// ---------------------------------------------------------
//
TBool CAknFepUICtrlPinyinEEP::GetPhraseCreationFlag()
    {
    return iPhraseCreation;
    }

// ---------------------------------------------------------
// Set the keystroke invalid index.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetInvalidIndex( TInt aInvalidIndex )
    {
    iInvalidKeystrokeIndex = aInvalidIndex;
    }

// ---------------------------------------------------------
// Get all the invalid keystroke index.
// ---------------------------------------------------------
//
TInt CAknFepUICtrlPinyinEEP::GetInvalidIndex()
    {
    return iInvalidKeystrokeIndex;
    }

// ---------------------------------------------------------
// Set Phrase Candidate state need refresh UI flag.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetCandidateRefreshFlag( TBool aState )
    {
    iCandidateRefreshFlag = aState;
    }

// ---------------------------------------------------------
// Get Phrase Candidate state need refresh UI flag.
// ---------------------------------------------------------
//
TInt CAknFepUICtrlPinyinEEP::GetCandidateRefreshFlag()
    {
    return iCandidateRefreshFlag;
    }

// ---------------------------------------------------------
// Set the delimiter count of keystroke.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetDelimiterCount( TInt aCount )
    {
    iDelimiterCount = aCount;
    }

// ---------------------------------------------------------
// Get the delimiter count of keystroke.
// ---------------------------------------------------------
//
TInt CAknFepUICtrlPinyinEEP::GetDelimiterCount()
    {
    return iDelimiterCount;
    }

// ---------------------------------------------------------
// Get current cba resource id. 
// ---------------------------------------------------------
//
TInt CAknFepUICtrlPinyinEEP::GetCurrentCBAResID()
    {
    return iCurrentCBAResId;
    }

// ---------------------------------------------------------
// Set current cba resource id. 
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinEEP::SetCurrentCBAResID( TInt aResourceId )
    {
    iCurrentCBAResId = aResourceId;
    }

void CAknFepUICtrlPinyinEEP::SetEffictiveLength( TInt aLength )
    {
    iEffictiveLength = aLength;
    }

TInt CAknFepUICtrlPinyinEEP::GetEffictiveLength()
    {
    return iEffictiveLength;
    }

void CAknFepUICtrlPinyinEEP::AddEffictiveLength()
    {
    iEffictiveLength++;
    }

void CAknFepUICtrlPinyinEEP::DecEffictiveLength()
    {
    iEffictiveLength--;
    }

void CAknFepUICtrlPinyinEEP::SetNeedClearDeliberateSelection( TBool aNeedClearDeliberateSelection )
    {
    iNeedClearDeliberateSelection = aNeedClearDeliberateSelection;
    }

TBool CAknFepUICtrlPinyinEEP::NeedClearDeliberateSelection()
    {
    return iNeedClearDeliberateSelection;
    }
CDesCArrayFlat* CAknFepUICtrlPinyinEEP::PhraseZhuYinSpellingArray()
	{
	return iPhraseZhuYinSpellingArray;
	}
// End of file

