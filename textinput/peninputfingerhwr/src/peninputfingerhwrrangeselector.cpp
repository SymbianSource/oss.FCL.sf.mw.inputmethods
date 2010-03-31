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
* Description:  Implementation of char range selector.
*
*/

// INCLUDE
#include <peninputfingerhwrcn.rsg>

#include <coemain.h>
#include <s32mem.h>

#include <aknlayoutscalable_apps.cdl.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <AknLayoutDef.h>
#include <AknUtils.h>
#include <AknsUtils.h>
#include <AknIconUtils.h>
#include <aknfeppeninputenums.h>
#include <AknFepGlobalEnums.h>

#include <peninputlayout.h>
#include <peninputrepeatbutton.h>
#include <peninputmultiimagebutton.h>

#include "peninputfingerhwrevent.h"
#include "peninputfingerhwrcontrolid.h"
#include "peninputfingerhwrstoreconstants.h"

#include "peninputfingerhwrrangeselector.h"


//CONST DEFINATION
const TInt KButtonInnerPadding = 6;

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrRangeSelector* CPeninputFingerHwrRangeSelector::NewL( 
    CFepUiLayout* aUiLayout, TInt aId )
    {
    CPeninputFingerHwrRangeSelector* self = CPeninputFingerHwrRangeSelector::NewLC( 
            aUiLayout, aId );
    
    CleanupStack::Pop( self ); // self;
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrRangeSelector* CPeninputFingerHwrRangeSelector::NewLC( 
    CFepUiLayout* aUiLayout, TInt aId )
    {
    CPeninputFingerHwrRangeSelector* self = new (ELeave) CPeninputFingerHwrRangeSelector( 
            aUiLayout, aId );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// c++ destructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrRangeSelector::~CPeninputFingerHwrRangeSelector()
    {
    iRangeChBtn = NULL;
    iRangeEnBtn = NULL;
    iRangeNumBtn = NULL;
     
    iRangeChMiBtn = NULL;
    iRangeEnMiBtn = NULL;
    iRangeNumMiBtn = NULL;      
    }

// ---------------------------------------------------------------------------
// popup the list.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrRangeSelector::Popup( TInt aDirect )
    {
    CapturePointer( ETrue );
    iPopupVisible = ETrue;
    LayoutRangeButtons( iPermittedRanges, iCurRange, 
            iCurRangeActive, ETrue, aDirect );
    }

// ---------------------------------------------------------------------------
// cancel the popup.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrRangeSelector::CancelPopup()
    {
    TRect extendRect = Rect();
    LayoutRangeButtons( iPermittedRanges, iCurRange, 
            iCurRangeActive, EFalse, EPopDirAuto );
    CapturePointer( EFalse );
    iPopupVisible = EFalse;

    RootControl()->Draw();
    UpdateArea( extendRect, EFalse );
    }

// ---------------------------------------------------------------------------
// get visibility of popup.
// ---------------------------------------------------------------------------
//
TBool CPeninputFingerHwrRangeSelector::IsPopup()
    {
    return iPopupVisible;
    }

// ---------------------------------------------------------------------------
// set permitted char range.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrRangeSelector::SetPermittedRanges( const TInt aPermittedRanges )
    {
    iPermittedRanges = aPermittedRanges;
    
    if ( IsPopup() )
        {
        CancelPopup();
        }
    
    LayoutRangeButtons( iPermittedRanges, iCurRange, iCurRangeActive, 
            EFalse, EPopDirAuto );
    }

// ---------------------------------------------------------------------------
// set current char range.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrRangeSelector::SetCurRange( const TInt aCurRange, 
    const TBool aActive )
    {
    iCurRange = aCurRange;
    iCurRangeActive = aActive;

    if ( IsPopup() )
        {
        CancelPopup();
        }
    
    LayoutRangeButtons( iPermittedRanges, iCurRange, iCurRangeActive, 
            EFalse, EPopDirAuto );
    
    }

// ---------------------------------------------------------------------------
// set base rect of popup.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrRangeSelector::SetBaseRect( const TRect& aRect )
    {
    iPopupBaseRect = aRect;

    LayoutRangeButtons( iPermittedRanges, iCurRange, 
            iCurRangeActive, iPopupVisible, EPopDirAuto );

    UpdateArea( Rect(), EFalse );
    }


// ---------------------------------------------------------------------------
// Handle pointer down event.
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CPeninputFingerHwrRangeSelector::HandlePointerDownEventL(
    const TPoint& aPoint )
    {
    CFepUiBaseCtrl* ctrl = CControlGroup::HandlePointerDownEventL( aPoint );
    
    if ( !ctrl )
        {
        if ( IsPopup() )
            {
            CancelPopup();
            }
        }
    
    return ctrl;
    }

// ---------------------------------------------------------------------------
// Handle pointer up event.
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CPeninputFingerHwrRangeSelector::HandlePointerUpEventL(
    const TPoint& aPoint )
    {
    CFepUiBaseCtrl* ctrl = CControlGroup::HandlePointerUpEventL( aPoint );

    iRangeChBtn->CancelPointerDownL();
    iRangeEnBtn->CancelPointerDownL();
    iRangeNumBtn->CancelPointerDownL(); 
    
    return ctrl;
    }

// ---------------------------------------------------------------------------
// Handle pointer move event.
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CPeninputFingerHwrRangeSelector::HandlePointerMoveEventL(
    const TPoint& /*aPoint*/ )
    {
    //bypass    
    return NULL;
    }

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrRangeSelector::CPeninputFingerHwrRangeSelector( 
    CFepUiLayout* aFepUiLayout, TInt aControlId )
    : CControlGroup( aFepUiLayout, aControlId, EFalse )
    {
 
    }
    
// ---------------------------------------------------------------------------
// Symbian second-phase constructor.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrRangeSelector::ConstructL()
    {
    BaseConstructL(); 
    
    //disable border
    SetBorderSize( TSize(0,0) );
    
    //create buttons
    iRangeChBtn = CreateEventBtnL( EHwrCtrlIdChineseButton, R_AKN_FEP_HWR_RANGE_CHINESE );
    iRangeEnBtn = CreateEventBtnL( EHwrCtrlIdEnglishButton, R_AKN_FEP_HWR_RANGE_ENGLISH );
    iRangeNumBtn = CreateEventBtnL( EHwrCtrlIdNumberButton, R_AKN_FEP_HWR_RANGE_NUMERIC );
    
    iRangeChMiBtn = CreateMultiBtnL( EHwrCtrlIdChineseMiButton, R_AKN_FEP_HWR_RANGE_CH_MI );
    iRangeEnMiBtn = CreateMultiBtnL( EHwrCtrlIdEnglishMiButton, R_AKN_FEP_HWR_RANGE_EN_MI );
    iRangeNumMiBtn = CreateMultiBtnL( EHwrCtrlIdNumberMiButton, R_AKN_FEP_HWR_RANGE_NUM_MI );
    }

// ---------------------------------------------------------------------------
// EventButton creation helper.
// ---------------------------------------------------------------------------
//
CAknFepCtrlEventButton* CPeninputFingerHwrRangeSelector::CreateEventBtnL( 
    TInt aCtrlId, TInt32 aResId, TInt aEvent/*= 0xFFFF*/,TInt aUnicode/*=0*/ )
    {
    CAknFepCtrlEventButton* button = CAknFepCtrlEventButton::NewL( 
        UiLayout(), aCtrlId, aEvent, aUnicode,
        KAknsIIDQsnFrFunctionButtonNormal,
        KAknsIIDQsnFrFunctionButtonPressed,
        KAknsIIDQsnFrFunctionButtonInactive );
    
    button->SetResourceId( aResId );
    button->ConstructFromResourceL();
    button->AddEventObserver( UiLayout() );        
    AddControlL( button );
    return button;
    }

// ---------------------------------------------------------------------------
// MultiImageButton creation helper.
// ---------------------------------------------------------------------------
//
CAknFepCtrlMultiImageButton* CPeninputFingerHwrRangeSelector::CreateMultiBtnL( 
    TInt aCtrlId, TInt32 aResId )
    {
    CAknFepCtrlMultiImageButton* button = CAknFepCtrlMultiImageButton::NewL( 
        UiLayout(), aCtrlId,
        KAknsIIDQsnFrFunctionButtonNormal,
        KAknsIIDQsnFrFunctionButtonPressed,
        KAknsIIDQsnFrFunctionButtonInactive );

    button->SetResourceId( aResId );      
    button->ConstructFromResourceL();
    button->AddEventObserver( UiLayout() );
    AddControlL( button );
    return button;
    }

// ---------------------------------------------------------------------------
//  EventButton layout helper. Move button to specified rect.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrRangeSelector::MoveIconButton( CAknFepCtrlEventButton* aButton, 
    const TRect& aRect, TInt aXPadding, TInt aYPadding, TBool aReloadImages )
    {
    if ( !aButton )
        {
        return;
        }
    
    aButton->SetRect( aRect );
    TRect rcInner = aRect;
    if ( rcInner.Width() > rcInner.Height() )
        {
        TInt dx = ( rcInner.Width() - rcInner.Height() ) / 2;
        rcInner.Move( dx, 0);
        rcInner.SetWidth( rcInner.Height() );
        }
    else
        {
        TInt dy = ( rcInner.Height() - rcInner.Width() ) / 2;
        rcInner.Move( 0, dy );
        rcInner.SetHeight( rcInner.Width() );        
        }
    
    rcInner.Shrink( aXPadding, aYPadding );
    aButton->SizeChanged( aRect, rcInner, aReloadImages );
    }

// ---------------------------------------------------------------------------
//  MultiIconButton layout helper. Move button to specified rect.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrRangeSelector::MoveMultiIconButton( 
    CAknFepCtrlMultiImageButton* aButton, const TRect& aRect,
    TInt aXPadding, TInt aYPadding, TBool aReloadImages )
    {
    if ( !aButton )
        {
        return;
        }
    
    aButton->SetRect( aRect );
    TRect rcInner=aRect;
    if ( rcInner.Width() > rcInner.Height() )
        {
        TInt dx = ( rcInner.Width() - rcInner.Height() ) / 2;
        rcInner.Move( dx, 0);
        rcInner.SetWidth( rcInner.Height() );
        }
    else
        {
        TInt dy = ( rcInner.Height() - rcInner.Width() ) / 2;
        rcInner.Move( 0, dy );
        rcInner.SetHeight( rcInner.Width() );        
        }
    
    rcInner.Shrink( aXPadding, aYPadding );
    aButton->SizeChanged( aRect, rcInner, aReloadImages );
    }

// ---------------------------------------------------------------------------
//  layout buttons.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrRangeSelector::LayoutRangeButtons( TInt aPermittedRanges, 
    TInt aCurRange, TBool aActive, TBool aShowDropList, TInt aDropDirect )
    {
    
    TRect rcBound  = UiLayout()->Rect();
    TRect rcBase = iPopupBaseRect;
    TRect rcTarget = rcBase;
    TInt pdx = KButtonInnerPadding;
    TInt pdy = KButtonInnerPadding;
    
   
    TInt count = 0;
    
    count += aPermittedRanges & ERangeNative  ? 1 : 0;
    count += aPermittedRanges & ERangeEnglish ? 1 : 0;
    count += aPermittedRanges & ERangeNumber  ? 1 : 0;
    
    if ( count == 1 || aShowDropList )
        {
        iRangeChMiBtn->Hide( ETrue );
        iRangeEnMiBtn->Hide( ETrue );
        iRangeNumMiBtn->Hide( ETrue );        
        }
    
    if ( count < 1 )
        {
        return;
        }
    
    if ( count == 1 )
        {

        switch( aCurRange )
            {
            case EFingerHwrNativeRange:
                {
                MoveIconButton( iRangeChBtn, rcBase, pdx, pdy, ETrue );
                }
                break;
            case EFingerHwrEnglishRange:
                {
                MoveIconButton( iRangeEnBtn, rcBase,  pdx, pdy, ETrue );
                }
                break;
            case EFingerHwrNumberRange:
                {
                MoveIconButton( iRangeNumBtn, rcBase,  pdx, pdy, ETrue );
                }
                break;
            default:
                break;
            }
        iRangeChBtn->Hide( aCurRange != EFingerHwrNativeRange );
        iRangeEnBtn->Hide( aCurRange != EFingerHwrEnglishRange );
        iRangeNumBtn->Hide( aCurRange != EFingerHwrNumberRange );
        
        SetRect( rcTarget );
        
        iRangeChBtn->SetHighlight( EFalse );
        iRangeEnBtn->SetHighlight( EFalse );
        iRangeNumBtn->SetHighlight( EFalse );
        
        return;
        }

    if ( aShowDropList )
        {
        TRect rcBtn = rcBase;
        
        TPoint direct = CalcDropListDirection( rcBound, rcBtn.iTl, 
                rcBtn.Size(), count, aDropDirect );
  
        TInt dx = direct.iX;
        TInt dy = direct.iY;
        
        CalcDropListRect( rcTarget, dx * count, dy * count );
        SetRect( rcTarget );
        
        if ( aPermittedRanges & ERangeNative )
            {
            MoveIconButton( iRangeChBtn, rcBtn, pdx, pdy, ETrue );
            rcBtn.Move( dx, dy );
            }
            
        if ( aPermittedRanges & ERangeEnglish )
            {
            MoveIconButton( iRangeEnBtn, rcBtn,  pdx, pdy, ETrue );
            rcBtn.Move( dx, dy );
            }
            
        if ( aPermittedRanges & ERangeNumber )
            {
            MoveIconButton( iRangeNumBtn, rcBtn,  pdx, pdy, ETrue );
            rcBtn.Move( dx, dy );
            }
        
        iRangeChBtn->Hide( !(aPermittedRanges & ERangeNative) );
        iRangeEnBtn->Hide( !(aPermittedRanges & ERangeEnglish) );
        iRangeNumBtn->Hide( !(aPermittedRanges & ERangeNumber) );
        
        iRangeChBtn->SetHighlight( aActive && aCurRange == EFingerHwrNativeRange );
        iRangeEnBtn->SetHighlight( aActive && aCurRange == EFingerHwrEnglishRange );
        iRangeNumBtn->SetHighlight( aActive && aCurRange == EFingerHwrNumberRange  ); 
        }
    else
        {
        SetRect( rcTarget );
        
        switch( aCurRange )
            {
            case EFingerHwrNativeRange:
                {
                MoveMultiIconButton( iRangeChMiBtn, rcBase, pdx, pdy, ETrue );
                }
                break;     
            case EFingerHwrEnglishRange:
                {
                MoveMultiIconButton( iRangeEnMiBtn, rcBase,  pdx, pdy, ETrue );
                }
                break;
            case EFingerHwrNumberRange:
                {
                MoveMultiIconButton( iRangeNumMiBtn, rcBase,  pdx, pdy, ETrue );
                }
                break;
            default:
                break;
            }
        iRangeChMiBtn->Hide( aCurRange != EFingerHwrNativeRange );
        iRangeEnMiBtn->Hide( aCurRange != EFingerHwrEnglishRange );
        iRangeNumMiBtn->Hide( aCurRange != EFingerHwrNumberRange );
        
        iRangeChBtn->Hide( ETrue );
        iRangeEnBtn->Hide( ETrue );
        iRangeNumBtn->Hide( ETrue );        
        }
    }


// ---------------------------------------------------------------------------
//  calculates popup direction.
// ---------------------------------------------------------------------------
//
TPoint CPeninputFingerHwrRangeSelector::CalcDropListDirection( const TRect& aBound, 
    const TPoint& aTarget, const TSize& aItemSize, 
    TInt aItemCount, TInt aDirect )
    {
    TInt dirx = 0;
    TInt diry = 0;

    TInt direct = aDirect;
    if( aDirect == EPopDirAuto )
        {
        //determine pop diretion, in this order: down, up, right, left
        const TInt minx = aBound.iTl.iX;
        TInt miny = aBound.iTl.iY;
        TInt maxy = aBound.iBr.iY;
        
        if( aTarget.iY + aItemSize.iHeight * aItemCount <= maxy )
            {
            direct = EPopDirDown;         
            }
        else if( aTarget.iY -  aItemSize.iHeight * ( aItemCount - 1 ) >= miny )
            {
            direct = EPopDirTop;       
            }
        else if( aTarget.iX +  aItemSize.iWidth * aItemCount <= maxy )
            {
            direct = EPopDirRight; 
            }
        else if( aTarget.iX -  aItemSize.iWidth * ( aItemCount - 1 ) >= minx )
            {
            direct = EPopDirLeft; 
            }
        else 
            {
            direct = EPopDirDown;
            }
        }
    
    switch( direct )
        {
        case EPopDirDown: 
            {
            diry = 1;
            }
            break;
        case EPopDirTop:
            {
            diry = -1;
            }
            break;
        case EPopDirRight:
            {
            dirx = 1;
            }
            break;
        case EPopDirLeft: 
            {
            dirx = -1;
            }
            break;
        default:
            {
            dirx = 0;
            diry = 1;
            }
            break;
        }
    return TPoint( dirx * aItemSize.iWidth, diry * aItemSize.iHeight );
    }


// ---------------------------------------------------------------------------
//  calculates popup rect.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrRangeSelector::CalcDropListRect( TRect& aRect, 
    const TInt aWidth, const TInt aHeight )
    {
    if ( aWidth > 0 )
        {
        aRect.SetWidth( aWidth );
        }
    else if ( aWidth < 0 )
        {
        aRect.Move( aWidth + aRect.Width(), 0 );
        aRect.SetWidth( - aWidth );
        }
    
    if ( aHeight > 0 )
        {
        aRect.SetHeight( aHeight );
        }
    else if ( aHeight < 0 )
        {
        aRect.Move( 0, aHeight + aRect.Height() );
        aRect.SetHeight( - aHeight );
        }    
    }
// End Of File
