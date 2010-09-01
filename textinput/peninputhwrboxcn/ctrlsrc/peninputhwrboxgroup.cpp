/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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


#include <AknLayoutDef.h>
#include <AknsUtils.h>
#include <coemain.h>
#include <barsread.h>

#include "peninputhwrboxgroup.h"
#include "peninputhwrctrlid.h"
#include "peninputhwrevent.h"

const TUint32 KDefaultWriteBoxBgColor = 0xdddddd;
const TUint32 KDefaultWriteBoxFrameColor = 0x000000;
const TInt32 KInvalidResId = -1;

// constant definition
const TInt KDefaultStrokeEndMarkX = -65536;
const TInt KDefaultStrokeEndMarkY = -65536;

// Implementation of Class CAknFepHwrBoxGroup 
// The container class of HWR boxes

// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepHwrBoxGroup* CAknFepHwrBoxGroup::NewL(CFepUiLayout* aUiLayout,
                                             TInt aBoxGroupId,
                                             TInt aBoxCount)
    {
    CAknFepHwrBoxGroup* self = new (ELeave) CAknFepHwrBoxGroup(aUiLayout, aBoxGroupId);
    CleanupStack::PushL(self);
    self->ConstructL(aBoxCount);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::NewLC
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepHwrBoxGroup* CAknFepHwrBoxGroup::NewLC(CFepUiLayout* aUiLayout,
                                              TInt aBoxGroupId,
                                              TInt aBoxCount)
    {
    CAknFepHwrBoxGroup* self = new (ELeave) CAknFepHwrBoxGroup(aUiLayout, aBoxGroupId);
    CleanupStack::PushL(self);
    self->ConstructL(aBoxCount);
    return self;
    }

// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::~CAknFepHwrBoxGroup
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepHwrBoxGroup::~CAknFepHwrBoxGroup()
    {
    // base class CControlGroup frees all controls in the list
    }

// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::ConstructFromResourceL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::ConstructFromResourceL( TResourceReader& aReader)
    {
    // get write box bg color
    TInt32 colorMajorSkinId = aReader.ReadInt32();
    TInt skinitemid= aReader.ReadInt16();
    TInt coloridx = aReader.ReadInt16();

    TAknsItemID colorid;
    colorid.Set(TInt(colorMajorSkinId), skinitemid);

    TRgb boxcolor;
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();
    TInt error = AknsUtils::GetCachedColor(skininstance, 
                                           boxcolor, 
                                           colorid, 
                                           coloridx);

    if (error != KErrNone)
        {
    	boxcolor = TRgb(KDefaultWriteBoxBgColor);
        }

    SetBoxBackColor(boxcolor); 
        
    skinitemid = aReader.ReadInt16();
    coloridx = aReader.ReadInt16();
    
    colorid.Set(TInt(colorMajorSkinId), skinitemid);
    
    TRgb boxframecolor;
    error = AknsUtils::GetCachedColor(skininstance, 
                                      boxframecolor, 
                                      colorid, 
                                      coloridx);

    if (error != KErrNone)
        {
    	boxframecolor = TRgb(KDefaultWriteBoxFrameColor);
        }

    SetBoxFrameColor(boxframecolor);

    iFirstTimeConstruct = EFalse;
    }
    
// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::ConstructFromResourceL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::ConstructFromResourceL()
    {
    if (iResourceId == KInvalidResId)
    	{
    	User::Leave(KErrArgument);
    	}

    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, iResourceId);

    // get write box bg color
    TInt32 colorMajorSkinId = reader.ReadInt32();
    TInt skinitemid = reader.ReadInt16();
    TInt coloridx = reader.ReadInt16();

    TAknsItemID colorid;
    colorid.Set(TInt(colorMajorSkinId), skinitemid);

    TRgb boxcolor;
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();
    TInt error = AknsUtils::GetCachedColor(skininstance, 
                                           boxcolor, 
                                           colorid, 
                                           coloridx);

    if (error != KErrNone)
        {
        boxcolor = TRgb(KDefaultWriteBoxBgColor);
        }
    
    SetBoxBackColor(boxcolor);

    skinitemid = reader.ReadInt16();
    coloridx = reader.ReadInt16();
    
    colorid.Set(TInt(colorMajorSkinId), skinitemid);
    
    TRgb boxframecolor;
    error = AknsUtils::GetCachedColor(skininstance, 
                                      boxframecolor, 
                                      colorid, 
                                      coloridx);

    if (error != KErrNone)
        {
    	boxframecolor = TRgb(KDefaultWriteBoxFrameColor);
        }
    
    SetBoxFrameColor(boxframecolor);

    CleanupStack::PopAndDestroy(); // reader
    
    iFirstTimeConstruct = EFalse;
    }

// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::BoxCount
// Clear current handwriting trail if any.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CAknFepHwrBoxGroup::BoxCount()
    {
    return NumOfControls();
    }

// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::CancelWriting
// Clear current handwriting trail if any.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::CancelWriting()
    {
    if( !iActiveBox )
        {
        //there is nothing to cancel
        return;
        }

    iActiveBox->SetWndTransparencyFactor(iInactiveTrans);
    iActiveBox->CancelCharWriting();
    }
    
const RArray<TPoint>& CAknFepHwrBoxGroup::StrokeList()
    {
    ASSERT( iActiveBox != NULL );
    
    return iActiveBox->StrokeList();
    }

// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::SizeChanged
// Hand size change event.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::SizeChanged(const TRect& aRect)
    {
    TInt boxWidth = aRect.Width() / NumOfControls();
    TInt curLeft = aRect.iTl.iX;
    
    SetRect(aRect);
    
    for( TInt j = 0; j < NumOfControls(); j++, curLeft += boxWidth )
        {
        At(j)->SetRect( TRect(curLeft, aRect.iTl.iY, 
                              curLeft + boxWidth,
                              aRect.iBr.iY) );
        }
        
    RefreshUI();
    }

// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::SizeChanged
// Hand size change event.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::SizeChanged()
    {
    //load Laf data and set all boxes size 
    //TRect aRect = somecode_load_laf_data;
    //SizeChanged(aRect);
    }

// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::SetBoxBackColor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::SetBoxBackColor(const TRgb aCol)
    {
    for( TInt j = 0; j < NumOfControls(); j++ )
        {
        At(j)->SetBkColor(aCol);
        }
    }

// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::SetBoxFrameColor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::SetBoxFrameColor(const TRgb aCol)
    {
    for( TInt j = 0; j < NumOfControls(); j++ )
        {
        static_cast<CTransparentHwrWnd*>(At(j))->SetFrameCol(aCol);
        }
    }

// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::SetTransparency
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::SetTransparency(const TUint8 aActiveTrans, const TUint8 aInactiveTrans)
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
// from base class MAknFepHwrPropertySubscriber
// CAknFepHwrBoxGroup::SetPropertyL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::SetPropertyL(const TAknFepHwrProperty aPropertyName, 
                                     const TDesC& aPropertyValue)
    {
    switch( aPropertyName )
        {
        case EAknFepHwrPropertyWriteSpeed:
            {
            const TTimeIntervalMicroSeconds32 speed = 
                                           *(TTimeIntervalMicroSeconds32 *)(aPropertyValue.Ptr());//lint !e826
            SetWritingSpeed( speed );
            }
            break;
        case EAknFepHwrPropertyPenSize:
            {
            const TSize penSize = *(TSize *)(aPropertyValue.Ptr());//lint !e826
            SetBoxPenSize( penSize );
            }
            break;
        case EAknFepHwrPropertyPenColor:
            {
            const TRgb color = *(TRgb *)(aPropertyValue.Ptr());//lint !e826
            SetBoxPenColor( color );
            }
            break;
        case EAknFepHwrPropertyStrokeEndMark:
            {
            const TPoint endMark = *(TPoint*)(aPropertyValue.Ptr());
            SetStrokeEndMark(endMark);
            }
            break;
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// from base class MEventObserver
// CAknFepHwrBoxGroup::HandleEvent
// Clear current handwriting trail if any.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::HandleControlEvent(TInt aEventType, 
                                           CFepUiBaseCtrl* aCtrl, 
                                           const TDesC& /*aEventData*/)
    {
    CTransparentHwrWnd* hwrWnd = static_cast<CTransparentHwrWnd*>( aCtrl );

    switch( aEventType )
        {
        case EEventHwrStrokeStarted:
            OnStrokeStarted(hwrWnd);
            break;
        case EEventHwrStrokeFinished:
            OnStrokeFinished(hwrWnd);
            break;
        case EEventHwrCharacterTimerOut:
            OnStrokeCharacterTimerOut(hwrWnd);
            break;
        case EEventHwrStrokeCanceled:
            OnStrokeCanceled(hwrWnd);
            break;
        default:
            //todo: any comments left here
            break; 
        }
    }//lint !e715

// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::CAknFepHwrBoxGroup
// C++ constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepHwrBoxGroup::CAknFepHwrBoxGroup(CFepUiLayout* aUiLayout, TInt aBoxGroupId)
: CControlGroup(aUiLayout, aBoxGroupId)
    {
    }

// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::ConstructL
// Symbian secode phase constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::ConstructL(TInt aBoxCount)
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
        
        // set default stroke end mark
        aBox->SetStrokeEndMark(TPoint(KDefaultStrokeEndMarkX, KDefaultStrokeEndMarkY));

        CleanupStack::Pop(aBox); //aBox
       }
    }
    
// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::SetWritingSpeed
// Iterate to set each HWR box writting speed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::SetWritingSpeed(const TTimeIntervalMicroSeconds32& aCharDelay)
    {
    for( TInt j = 0; j < NumOfControls(); j++ )
        {
        CTransparentHwrWnd* hwrWnd = static_cast<CTransparentHwrWnd*>(At(j));
        
        if( hwrWnd )
            {
            hwrWnd->SetCharacterDelay(aCharDelay);
            }
        }    
    }

// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::SetBoxPenSize
// Iterate to set each HWR box trail drawing pen size.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::SetBoxPenSize(const TSize aSize)
    {
    for( TInt j = 0; j < NumOfControls(); j++ )
        {
        At(j)->SetPenSize(aSize);
        }
    }

// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::SetBoxPenColor
// Iterate to set each HWR box trail drawing pen color.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::SetBoxPenColor(const TRgb aCol)
    {
    for( TInt j = 0; j < NumOfControls(); j++ )
        {
        At(j)->SetPenColor(aCol);
        }
    }

// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::SetStrokEndMark
// Iterate to set each HWR box stroke end mark.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::SetStrokeEndMark(const TPoint& aEndMard)
    {
    for( TInt j = 0; j < NumOfControls(); j++ )
        {
        static_cast<CTransparentHwrWnd*>(At(j))->SetStrokeEndMark(aEndMard);
        }
    }
// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::OnStrokeStarted
// Handle EEventHwrStrokeStarted event.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::OnStrokeStarted(CTransparentHwrWnd* aBoxFrom)
    {
    TBool refresh = EFalse;
    
    if( !iActiveBox )
        {
        //change state from not writing to begin writing
        iActiveBox = aBoxFrom;
        refresh = ETrue;
        
        ReportEvent(EHwrEventBeginWriting);
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
            // handler of EHwrEventNewWriting submit default candidate
            // reset state
            ReportEvent(EHwrEventNewWriting);
            // clear writing of the last box
            CancelWriting();
            
            iActiveBox = aBoxFrom;
            refresh = ETrue;
            
            //set current box transparency  
            iActiveBox->SetWndTransparencyFactor(iActiveTrans);
            }
        else
            {
            ReportEvent(EHwrEventBeginWriting);
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
// CAknFepHwrBoxGroup::StrokeStrokeFinished
// Handle EEventHwrStrokeFinished event.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::OnStrokeFinished(CTransparentHwrWnd* /* aBoxFrom */)
    {
    // Get stroke list is called by event handler
    TPtrC ptr;
    RArray<TPoint>* strokeList = const_cast<RArray<TPoint>* >(&(StrokeList()));
    ptr.Set(reinterpret_cast<TText*>(strokeList), sizeof(strokeList));
    ReportEvent(EHwrEventEndStroke, ptr);
    }

// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::StrokeCharacterTimerOut
// Handle EEventHwrCharacterTimerOut event.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::OnStrokeCharacterTimerOut(CTransparentHwrWnd* aBoxFrom)
    {
    aBoxFrom->SetWndTransparencyFactor(iInactiveTrans);
    ReportEvent(EHwrEventEndWriting);
    
    if( iActiveBox == aBoxFrom )
        {
        iActiveBox = NULL;
        }

    RefreshUI(aBoxFrom);
    }

// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::OnStrokeCanceled
// Handle EEventHwrStrokeCanceled event.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::OnStrokeCanceled(CTransparentHwrWnd* aBoxFrom)
    {
    if( iActiveBox == aBoxFrom )
        {
        iActiveBox = NULL;
        }
    }
// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::RefreshUI
// Explicit refresh HWR boxes UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::RefreshUI(CTransparentHwrWnd* aBox)
    {
    if( !aBox )
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
            aBox->Draw();
            UpdateArea(aBox->Rect(), EFalse);        
            }
        }
    }

// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::SubmitWriting
// Submit first candidate in candidate buffer immediatelly.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepHwrBoxGroup::SubmitWriting()
    {
    if( !iActiveBox )
        {
        return;
        }
        
    //iActiveBox->SetTransparency(iInactiveTrans);
    //todo call some base method to clear and submit
    //iActiveBox->EndStrokeDirectly();
    }

// -----------------------------------------------------------------------------
// CAknFepHwrBoxGroup::StrokeEndMark
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TPoint CAknFepHwrBoxGroup::StrokeEndMark()
    { 
    if (NumOfControls() >= 1)
        {
    	return static_cast<CTransparentHwrWnd*>(At(0))->StrokeEndMark();
        }
        
    return TPoint(KDefaultStrokeEndMarkX, KDefaultStrokeEndMarkY);
    }

//End Of File
