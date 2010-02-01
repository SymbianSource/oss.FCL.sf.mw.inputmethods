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
* Description:  The class of handwriting box container implemention.
*
*/


#include <peninputlayout.h>
#include <aknfeppeninputenums.h>    //command from fep or IME to plugin. for vkb/hwr : ESignalCapturePointer

#include "peninputhwrboxgroup.h"
#include "peninputhwrevent.h"

// Implementation of Class CPeninputHwrBoxGroup
// The container class of HWR boxes

// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxGroup* CPeninputHwrBoxGroup::NewL(CFepUiLayout* aUiLayout,
                                             TInt aBoxGroupId,
                                             TInt aBoxCount)
    {
    CPeninputHwrBoxGroup* self = new (ELeave) CPeninputHwrBoxGroup(aUiLayout, aBoxGroupId);
    CleanupStack::PushL(self);
    self->ConstructL(aBoxCount);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::NewLC
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxGroup* CPeninputHwrBoxGroup::NewLC(CFepUiLayout* aUiLayout,
                                              TInt aBoxGroupId,
                                              TInt aBoxCount)
    {
    CPeninputHwrBoxGroup* self = new (ELeave) CPeninputHwrBoxGroup(aUiLayout, aBoxGroupId);
    CleanupStack::PushL(self);
    self->ConstructL(aBoxCount);
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::~CPeninputHwrBoxGroup
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxGroup::~CPeninputHwrBoxGroup()
    {
    // base class CControlGroup frees all controls in the list
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxGroup::HandlePointerDownEventL
// Handle pointer down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
CFepUiBaseCtrl* CPeninputHwrBoxGroup::HandlePointerDownEventL(const TPoint& aPoint)
    {
    if(iDimmed)
        {
        CapturePointer();
        return NULL;
        }
    return CControlGroup::HandlePointerDownEventL(aPoint);
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxGroup::HandlePointerMoveEventL
// Handle pointer move event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
CFepUiBaseCtrl* CPeninputHwrBoxGroup::HandlePointerMoveEventL(const TPoint& aPoint)
    {
    if(iDimmed)
        {
        return NULL;
        }
    return CControlGroup::HandlePointerMoveEventL(aPoint);
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxGroup::HandlePointerUpEventL
// Handle pointer up event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
CFepUiBaseCtrl* CPeninputHwrBoxGroup::HandlePointerUpEventL(const TPoint& aPoint)
    {
    if(iDimmed)
        {
        TBool capture = ETrue;
        TPtrC captureData;
        captureData.Set((const TUint16*)&capture,sizeof(TBool)/sizeof(TUint16));
        CapturePointer(EFalse);
        UiLayout()->SignalOwner(ESignalCapturePointer,captureData);
        return NULL;
        }
    return CControlGroup::HandlePointerUpEventL(aPoint);
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxGroup::HandlePointerLeave
// Handle pointer leave  event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
void CPeninputHwrBoxGroup::HandlePointerLeave(const TPoint& aPoint)
    {
    if(iDimmed)
        {
        return;
        }
    CControlGroup::HandlePointerLeave(aPoint);
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxGroup::HandlePointerEnter
// Handle pointer enter  event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
void CPeninputHwrBoxGroup::HandlePointerEnter(const TPoint& aPoint)
    {
    if(iDimmed)
        {
        return;
        }
    CControlGroup::HandlePointerEnter(aPoint);
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::BoxCount
// Clear current handwriting trail if any.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPeninputHwrBoxGroup::BoxCount()
    {
    return NumOfControls();
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::CancelWriting
// Clear current handwriting trail if any.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputHwrBoxGroup::CancelWriting()
    {
    if( iActiveBox == NULL )
        {
        //there is nothing to cancel
        return;
        }

    iActiveBox->SetWndTransparencyFactor(iInactiveTrans);
    iActiveBox->CancelCharWriting();
    }

const RArray<TPoint>& CPeninputHwrBoxGroup::StrokeList()
    {
    ASSERT( iActiveBox != NULL );

    return iActiveBox->StrokeList();
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::SizeChanged
// Hand size change event.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputHwrBoxGroup::SizeChanged(const TRect& aRect)
    {
    TInt boxWidth = aRect.Width() / NumOfControls();
    TInt curLeft = aRect.iTl.iX;

    SetRect(aRect);

    for( TInt j = 0; j < NumOfControls(); j++, curLeft += boxWidth )
        {
        CTransparentHwrWnd* ctrl = static_cast<CTransparentHwrWnd*>(At(j));
        ctrl->SetRect( TRect(curLeft, aRect.iTl.iY,
                              curLeft + boxWidth,
                              aRect.iBr.iY) );
        // Set guide line information
        TRect rect(TPoint(0,0), TSize(boxWidth, aRect.Height()));
        TInt arc = (rect.Width() < rect.Height())? rect.Width() : rect.Height();
        TInt unit = arc / 8;

        TRect toprect( rect.Width()/2 - unit*4, unit + aRect.iTl.iY,
                    rect.Width()/2 + unit*4, unit + aRect.iTl.iY );
        TRect bottomrect( rect.Width()/2 - unit*4, unit*7 + aRect.iTl.iY,
                    rect.Width()/2 + unit*4, unit*7 + aRect.iTl.iY);


        // default black
        }

    RefreshUI();
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::SetBoxBackColor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputHwrBoxGroup::SetBoxBackColor(const TRgb& aCol)
    {
    for( TInt j = 0; j < NumOfControls(); j++ )
        {
        At(j)->SetBkColor(aCol);
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::SetBoxFrameColor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputHwrBoxGroup::SetBoxFrameColor(const TRgb& aCol)
    {
    for( TInt j = 0; j < NumOfControls(); j++ )
        {
        static_cast<CTransparentHwrWnd*>(At(j))->SetFrameCol(aCol);
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::SetStrokEndMark
// Iterate to set each HWR box stroke end mark.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputHwrBoxGroup::SetStrokeEndMark(const TPoint& aEndMard)
    {
    for( TInt j = 0; j < NumOfControls(); j++ )
        {
        static_cast<CTransparentHwrWnd*>(At(j))->SetStrokeEndMark(aEndMard);
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::SetTransparency
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputHwrBoxGroup::SetTransparency(const TUint8 aActiveTrans, const TUint8 aInactiveTrans)
    {
#ifdef __ALPHA_TRANCEPARENCY__
    iActiveTrans = aActiveTrans;
    iInactiveTrans = aInactiveTrans;
#else
    iActiveTrans = 255 - aActiveTrans;
    iInactiveTrans = 255 - aInactiveTrans;
#endif

    for( TInt j = 0; j < NumOfControls(); j++ )
        {
        if( At(j) != iActiveBox)
            {
            static_cast<CTransparentHwrWnd*>(At(j))->SetWndTransparencyFactor(iInactiveTrans);
            }
        else
            {
            static_cast<CTransparentHwrWnd*>(At(j))->SetWndTransparencyFactor(iActiveTrans);
            }
        }
    }

// -----------------------------------------------------------------------------
// from base class MEventObserver
// CPeninputHwrBoxGroup::HandleEvent
// Clear current handwriting trail if any.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputHwrBoxGroup::HandleControlEvent(TInt aEventType,
                                           CFepUiBaseCtrl* aCtrl,
                                           const TDesC& /*aEventData*/)
    {
    CTransparentHwrWnd* hwrWnd = static_cast<CTransparentHwrWnd*>( aCtrl );

    switch( aEventType )
        {
        case EEventHwrStrokeStarted:
            ReportEvent(aEventType);   // for DisableCaseChange
            OnStrokeStarted(hwrWnd);
            break;
        case EEventHwrStrokeFinished:
            OnStrokeFinished(hwrWnd);
            break;
        case EEventHwrCharacterTimerOut:
            ReportEvent(aEventType);   // for EnableCaseChange
            OnStrokeCharacterTimerOut(hwrWnd);
            break;
        case EEventHwrStrokeCanceled:
            ReportEvent(aEventType);   // for EnableCaseChange
            OnStrokeCanceled(hwrWnd);
            break;
        default:
            // not handle other event type
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::CPeninputHwrBoxGroup
// C++ constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPeninputHwrBoxGroup::CPeninputHwrBoxGroup(CFepUiLayout* aUiLayout, TInt aBoxGroupId)
    : CControlGroup(aUiLayout, aBoxGroupId),
    iDimmed(EFalse)
    {
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::ConstructL
// Symbian secode phase constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputHwrBoxGroup::ConstructL(TInt aBoxCount)
    {
    //leave if invalid aBoxCount
    ASSERT( aBoxCount > 0 );

    //call base construct
    CControlGroup::ConstructL();

    //create two box hwr window
    for( TInt i = 0; i < aBoxCount; ++i )
        {
        CTransparentHwrWnd* aBox = CTransparentHwrWnd::NewL(TRect(),
                                                            UiLayout(),
                                                            0,
                                                            EFalse);
        CleanupStack::PushL( aBox );
        aBox->AddEventObserver( this );
        aBox->EnableTraceOutsideWindow(EFalse);
        AddControlL(aBox);
        CleanupStack::Pop(aBox); //aBox
       }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::SetWritingSpeed
// Iterate to set each HWR box writting speed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputHwrBoxGroup::SetWritingSpeed(const TTimeIntervalMicroSeconds32& aCharDelay)
    {
    for( TInt j = 0; j < NumOfControls(); j++ )
        {
        CTransparentHwrWnd* hwrWnd = static_cast<CTransparentHwrWnd*>(At(j));

        if( hwrWnd != NULL )
            {
            hwrWnd->SetCharacterDelay(aCharDelay);
            }
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::SetStrokeDelay
// Iterate to set each HWR box writting speed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputHwrBoxGroup::SetStrokeDelay(const TTimeIntervalMicroSeconds32& aCharDelay)
    {
    for( TInt j = 0; j < NumOfControls(); j++ )
        {
        CTransparentHwrWnd* hwrWnd = static_cast<CTransparentHwrWnd*>(At(j));

        if( hwrWnd != NULL )
            {
            hwrWnd->SetStrokeDelay(aCharDelay);
            }
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::SetBoxPenSize
// Iterate to set each HWR box trail drawing pen size.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputHwrBoxGroup::SetBoxPenSize(const TSize& aSize)
    {
    for( TInt j = 0; j < NumOfControls(); j++ )
        {
        At(j)->SetPenSize(aSize);
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::SetBoxPenColor
// Iterate to set each HWR box trail drawing pen color.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputHwrBoxGroup::SetBoxPenColor(const TRgb& aCol)
    {
    for( TInt j = 0; j < NumOfControls(); j++ )
        {
        At(j)->SetPenColor(aCol);
        }
    }
// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::OnStrokeStarted
// Handle EEventHwrStrokeStarted event.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputHwrBoxGroup::OnStrokeStarted(CTransparentHwrWnd* aBoxFrom)
    {
    TBool refresh = EFalse;

    if( iActiveBox == NULL )
        {
        //change state from not writing to begin writing
        iActiveBox = aBoxFrom;
        refresh = ETrue;

        ReportEvent(EPeninputLayoutHwrEventBeginWriting);
        //set current box transparency
        iActiveBox->SetWndTransparencyFactor(iActiveTrans);
        }
    else
        {
        if( iActiveBox != aBoxFrom )
            {
            // current active box will turn into inactive state
            iActiveBox->SetWndTransparencyFactor(iInactiveTrans);
            // user switches to another box
            if( iActiveBox->HasNewTrace() )
                {
                OnStrokeFinished(iActiveBox);
                }
            // handler of EPeninputLayoutHwrEventNewWriting submit default candidate
            // reset state
            ReportEvent(EPeninputLayoutHwrEventNewWriting);
            // clear writing of the last box
            CancelWriting();

            iActiveBox = aBoxFrom;
            refresh = ETrue;

            //set current box transparency
            iActiveBox->SetWndTransparencyFactor(iActiveTrans);
            }
        else
            {
            ReportEvent(EPeninputLayoutHwrEventBeginWriting);
            }
        }

    if( refresh )
        {
        // only active box need update
        // inactive box is already update by CancelWriting
        RefreshUI(iActiveBox);
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::StrokeStrokeFinished
// Handle EEventHwrStrokeFinished event.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputHwrBoxGroup::OnStrokeFinished(CTransparentHwrWnd* /* aBoxFrom */)
    {
    // Get stroke list is called by event handler
    TPtrC ptr;
    RArray<TPoint>* strokeList = const_cast<RArray<TPoint>* >(&(StrokeList()));
    ptr.Set(reinterpret_cast<TText*>(strokeList), sizeof(strokeList)/sizeof(TUint16));
    ReportEvent(EPeninputLayoutHwrEventEndStroke, ptr);
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::StrokeCharacterTimerOut
// Handle EEventHwrCharacterTimerOut event.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputHwrBoxGroup::OnStrokeCharacterTimerOut(CTransparentHwrWnd* aBoxFrom)
    {
    aBoxFrom->SetWndTransparencyFactor(iInactiveTrans);
    ReportEvent(EPeninputLayoutHwrEventEndWriting);

    if( iActiveBox == aBoxFrom )
        {
        iActiveBox = NULL;
        }

    RefreshUI(aBoxFrom);
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::OnStrokeCanceled
// Handle EEventHwrStrokeCanceled event.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputHwrBoxGroup::OnStrokeCanceled(const CTransparentHwrWnd* aBoxFrom)
    {
    if( iActiveBox == aBoxFrom )
        {
        iActiveBox = NULL;
        }
    }
// -----------------------------------------------------------------------------
// CPeninputHwrBoxGroup::RefreshUI
// Explicit refresh HWR boxes UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputHwrBoxGroup::RefreshUI(CTransparentHwrWnd* aBox)
    {
    if( aBox == NULL )
        {
        if( Hiden() )
            {
            return;
            }

        Draw();
        UpdateArea(Rect(), EFalse);
        }
    else
        {
        if( !aBox->Hiden() )
            {
            CTransparentHwrWnd* box = static_cast<CTransparentHwrWnd*>(aBox);
            box->Draw();
            UpdateArea(box->Rect(), EFalse);
            }
        }
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxGroup::SetDimmed
// Dim button
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxGroup::SetDimmed(TBool aDimFlag)
    {
    iDimmed = aDimFlag;
    }

//End Of File
