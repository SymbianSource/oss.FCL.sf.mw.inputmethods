/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:             Contents of the exact word tooltip shown above inline editor
*                when the exact word differs from the inline word in Western
*                QWERTY predictive mode.
*
*/












#include <eiklabel.h>
#include <AknUtils.h>
#include <AknsUtils.h>
#include <AknPreviewPopUpController.h>
#include "AknFepUiAvkonCtrlExactWordPopupContent.h"


const TInt KArrowWidth = 10;
const TInt KArrowHeight = 5;

CAknFepExactWordPopupContent* CAknFepExactWordPopupContent::NewL()
    {
    CAknFepExactWordPopupContent* self = new (ELeave) CAknFepExactWordPopupContent;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
    
    
CAknFepExactWordPopupContent::CAknFepExactWordPopupContent()
    : iIsPopUpVisible(EFalse)
    , iLabel( NULL )
    , iPopupController( NULL )
    , iArrowDirection( EUpwards )
    {
    }


void CAknFepExactWordPopupContent::ConstructL()
    {
    iLabel = new (ELeave) CEikLabel;
    iLabel->SetTextL(KNullDesC);

  	const CFont* font = AknLayoutUtils::FontFromId( AknLayout::Link_shortcut_title_texts_Line_1().FontId() );
    iLabel->SetFont(font);

    TRgb textColor(KRgbBlack);
    iLabel->GetColor(EColorLabelText, textColor);
    AknsUtils::GetCachedColor(AknsUtils::SkinInstance(), textColor, 
                              KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG20);
    TRAP_IGNORE( AknLayoutUtils::OverrideControlColorL(*iLabel, EColorLabelText, textColor) );
    
    TRect screenRect;
 	AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EScreen, screenRect );
    iLabel->SetRect( screenRect );
    SetRect( screenRect ); 
     
    iPopupController = CAknPreviewPopUpController::NewL( *this, 
                                                        CAknPreviewPopUpController::EPermanentMode );
    iPopupController->SetPopUpShowDelay(0);   
    }

MAknFepUiWordPopupContent::~MAknFepUiWordPopupContent()
	{
		
	}
    
CAknFepExactWordPopupContent::~CAknFepExactWordPopupContent()
    {
    delete iLabel;
    delete iPopupController;
    }
    
        
void CAknFepExactWordPopupContent::SetTextL( const TDesC& aText )
    {
    iLabel->SetTextL(aText);
    }


void CAknFepExactWordPopupContent::SetArrowDirection( MAknFepUiWordPopupContent::TArrowDirection aDirection )
    {
    iArrowDirection = aDirection;
    }


TSize CAknFepExactWordPopupContent::MinimumSize()
    {
    // Ensure we are on top. Otherwise the popup would be left on background in many applications
    // (e.g. Notes, ActiveNotes, Messaging, Contacts).
    Window().SetOrdinalPosition(0, ECoeWinPriorityAlwaysAtFront);

    TSize minSize = iLabel->MinimumSize();
    //minSize.iHeight *= 2;
    return minSize;
    }
    
        
void CAknFepExactWordPopupContent::Draw( const TRect& aRect ) const
    {
    TRect labelRect(aRect);

    if ( iPopupController )
        {
        labelRect.SetSize( iPopupController->Size() );
        }
    
    // Reserve some space for the arrow, if it is going to be shown below the text
    if ( iArrowDirection == EUpwards )
        {
        labelRect.iTl.iY -= KArrowHeight;
        }
        
    iLabel->SetRect( labelRect );
    iLabel->Draw( labelRect );

    TRAP_IGNORE( DrawArrowL( aRect ) );
    }
        

void CAknFepExactWordPopupContent::DrawArrowL( const TRect& aTextRect ) const
    {
    CWindowGc& gc = SystemGc();
    gc.SetPenColor( iEikonEnv->ControlColor(EColorLabelText,*this) );
    gc.SetBrushColor( iEikonEnv->ControlColor(EColorControlBackground,*this) );
    gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
            
    CArrayFix<TPoint>* arrow = new (ELeave) CArrayFixFlat<TPoint>(3);
    CleanupStack::PushL(arrow);

    TPoint offset( aTextRect.iTl.iX + aTextRect.Width()/2 - KArrowWidth/2, 0 );

    TPoint angle1;
    TPoint angle2;
    TPoint angle3;


    if ( iArrowDirection == EDownwards )
        {
        offset.iY = aTextRect.iTl.iY - KArrowHeight;
        angle1.SetXY( 0, 0 );
        angle2.SetXY( KArrowWidth, 0 );
        angle3.SetXY( KArrowWidth/2, KArrowHeight );
        }
    else // EUpwards
        {
        offset.iY = aTextRect.iBr.iY;
        angle1.SetXY( 0, KArrowHeight );
        angle2.SetXY( KArrowWidth, KArrowHeight );
        angle3.SetXY( KArrowWidth/2, 0 );
        }

    arrow->AppendL( angle1 + offset );
    arrow->AppendL( angle2 + offset );
    arrow->AppendL( angle3 + offset );

    gc.DrawPolygon(arrow);
    CleanupStack::PopAndDestroy(arrow);
    }

void CAknFepExactWordPopupContent::SetPopUpShowDelay( const TTimeIntervalMicroSeconds32& aDelay )
	{
	iPopupController->SetPopUpShowDelay( aDelay );
	}
void CAknFepExactWordPopupContent::ShowPopUp()
	{
	iIsPopUpVisible = ETrue;
	iPopupController->ShowPopUp();
	}
void CAknFepExactWordPopupContent::HidePopUp()
	{
	iIsPopUpVisible = EFalse;
	iPopupController->HidePopUp();	
	}
void CAknFepExactWordPopupContent::SetPosition( const TPoint& aPoint )
	{
	iPopupController->SetPosition( aPoint );
	}
void CAknFepExactWordPopupContent::UpdateContentSize()
	{
	iPopupController->UpdateContentSize();	
	}
TSize CAknFepExactWordPopupContent::Size() const 
	{
	return iPopupController->Size();
	}
TBool CAknFepExactWordPopupContent::IsPopUpVisible()
	{
	return iIsPopUpVisible;	
	}
	
void CAknFepExactWordPopupContent::HandlePointerEventL( const TPointerEvent& aPointerEvent )	
	{
	if( !AknLayoutUtils::PenEnabled() )
		{
		return;
		}

	if (TPointerEvent::EButton1Down == aPointerEvent.iType)
		{
       	TKeyEvent keyUp = {EKeyUpArrow, EStdKeyUpArrow, 0, 0};
    	CCoeEnv::Static()->WsSession().SimulateKeyEvent(keyUp);
		}
	}
