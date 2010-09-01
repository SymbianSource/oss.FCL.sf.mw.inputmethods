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
* Description:  Implement of class CTruiHwrBox
*
*/


// INCLUDE FILES
#include <centralrepository.h>
#include <settingsinternalcrkeys.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <layoutmetadata.cdl.h>
#include <AknUtils.h>
#include <AknsSkinInstance.h>
#include <AknsDrawUtils.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <AknsBasicBackgroundControlContext.h>

#include <AknsConstants.h>
#include <trui_icons.mbg>

#include "truihwrbox.h"
#include "truihwrboxobserver.h"
#include "truiappui.h"
#include "truiappui.h"
#include "truiengine.h"

#ifdef __WINS__
static const TInt KWritePeriod = 1500000;
#else
static const TInt KWritePeriod = 1000000;
#endif

#ifdef __WINS__
static const TInt KDrawSpeed = 30000;
#else
static const TInt KDrawSpeed = 30000;
#endif

const TInt KFirstPoint = -1;
const TSize KGuideLinePenSize = TSize( 1, 1 );
const TSize KLinePenSize = TSize( 4, 4 );
const TInt KFirstEventDelay = 0;

_LIT( KTRUIICONSFILENAME, "\\resource\\apps\\trui_icons.mif" );

const TInt KTopGuideLineIndex = 0;
const TInt KBottomGuideLineIndex = 1;

const TInt KGuideLineWidthRadio = 10;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CTruiHwrBox* CTruiHwrBox::NewL( CCoeControl* aParent,
                                MTruiHwrBoxObserver* aHwrBoxObserver,
                                MAknsControlContext* aBgContext )
    {
    CTruiHwrBox* self = CTruiHwrBox::NewLC( aParent, aHwrBoxObserver,
                                            aBgContext );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CTruiHwrBox* CTruiHwrBox::NewLC( CCoeControl* aParent,
                                 MTruiHwrBoxObserver* aHwrBoxObserver,
                                 MAknsControlContext* aBgContex )
    {
    CTruiHwrBox* self = new ( ELeave ) CTruiHwrBox();
    CleanupStack::PushL( self );
    self->ConstructL( aParent, aHwrBoxObserver,
                      aBgContex );
    return self;
    }
        
// -----------------------------------------------------------------------------
// CTruiHwrBox::CTruiHwrBox()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CTruiHwrBox::CTruiHwrBox()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CTruiHwrBox::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTruiHwrBox::ConstructL( CCoeControl* aParent,
                              MTruiHwrBoxObserver* aHwrBoxObserver,
                              MAknsControlContext* aBgContext )
    {
    if ( aParent ) 
        {
        SetContainerWindowL( *aParent );
        }
    else
        {
        CreateWindowL();
        }
        
    SetHwrBoxObserver( aHwrBoxObserver );
    
    // Create tooltip
    iTooltip = CAknInfoPopupNoteController::NewL();    
    iTooltip->SetTooltipModeL( ETrue );
    iTooltip->AddObserverL( *this );
    
    iBoxState = ENewModelStop;    
    iTimeToSave = CPeriodic::NewL( CActive::EPriorityHigh ); // neutral priority
    iTimeToDrawNext = CPeriodic::NewL( CActive::EPriorityStandard ); // neutral priority
    
    iPointArrayDraw.Reset();
    iPointArrayModel.Reset();
    
    LoadBorderBitmapsL();
    LoadShadowBitmapsL();
    LoadGuideLineBitmapsL();
    
    EnableDragEvents();    
    
    iBgContext = static_cast<CAknsBasicBackgroundControlContext*>( aBgContext );
    
    }

// -----------------------------------------------------------------------------
// CTruiHwrBox::~CTruiHwrBox()
// Destructor.
// -----------------------------------------------------------------------------
//
CTruiHwrBox::~CTruiHwrBox()
    {
    if ( iTooltip )
        {
        iTooltip->RemoveObserver( *this );
        }    
    HideInfoPopupNote();
    
    // Cancel and delete timer.
    if ( iTimeToSave )
        {
        iTimeToSave->Cancel();
        delete iTimeToSave;
        }
      
    if ( iTimeToDrawNext )
        {
        iTimeToDrawNext->Cancel();
        delete iTimeToDrawNext;
        }
    
    iPointArrayDraw.Close();
    iPointArrayModel.Close();
    delete iTooltip;
    
    iBorderBitmaps.ResetAndDestroy();
    iBorderBitmapMasks.ResetAndDestroy();
    iBorderLayout.Close();
    
    iShadowBitmaps.ResetAndDestroy();
    iShadowBitmapMasks.ResetAndDestroy();
    iShadowLayout.Close();
        
    iGuideLineBitmaps.ResetAndDestroy();
    iGuideLineBitmapMasks.ResetAndDestroy();
    iGuideLineLayout.Close();
    }

// -----------------------------------------------------------------------------
// Draws control to given area
// -----------------------------------------------------------------------------
//
void CTruiHwrBox::Draw( const TRect& /*aRect*/ ) const
    {
    // Get the standard graphics context 
    CWindowGc& gc = SystemGc();        
    
    // Set context
    PrepareContext( gc );
    
    // Gets the control's extent
    TRect rect = Rect();
    if ( iBgContext )
        {
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        if ( !AknsDrawUtils::Background( skin, iBgContext, 
                                         gc, rect ) ) 
            {
            gc.Clear( rect );
            }
        }
        
    if ( iFlag & EFrameHwrBox == EFrameHwrBox )
        {
        // Draw border
        DrawBorders( gc );
        // Draw shawdow
        DrawShadows( gc ); 
        }
    
    // Get repository
    CRepository* repository = NULL;
    TRAPD( err, repository = CRepository::NewL( KCRUidPersonalizationSettings ) );
    if ( err == KErrNone )
        {
        TInt guideLine;
        repository->Get( KSettingsPenInputGuideLine, guideLine );
        delete repository;
        repository = NULL;
        // Draw guideline
        if ( guideLine )
            {
            // Draw guidelines            
            DrawGuideLines( gc );   
            }        
        }        
    // Draw work area  
    gc.SetPenSize( KLinePenSize );
    DrawTrails( gc );
    }
        
TInt CTruiHwrBox::TimeToSaveL( TAny* aObject )
    {
    ( (CTruiHwrBox*)aObject )->SaveModelL(); // cast, and call non-static function
    return KErrNone;
    }
    
TInt CTruiHwrBox::TimeToDrawNextL( TAny* aObject )
    {
    ( (CTruiHwrBox*)aObject )->DrawNextL(); // cast, and call non-static function
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CTruiHwrBox::TimedOut()
// Timer control, called everytime the time times out.
// -----------------------------------------------------------------------------
//
void CTruiHwrBox::SaveModelL()
    {
    if ( iBoxState == ENewModelDraw )
        {
        // Restore all screen to invalid region.
        Window().Invalidate();
        //Cancel the timer
        iTimeToSave->Cancel();
        iBoxState = EExistModelStop;
        SetPointerCapture( EFalse );
        iPointArrayModel.Reset();
        for ( TInt i=0; i<iPointArrayDraw.Count(); i++ )
            {
            iPointArrayModel.Append( iPointArrayDraw[i] );
            }
        //can't save an empty model
        if ( iPointArrayModel.Count() > 0 )
            {                        
            if ( iHwrBoxObserver )
                {
                iHwrBoxObserver->SaveNewModelL();
                }            
            }
        else
            {
            ClearExistModel();
            }
        iStartSave = EFalse;
        }
    }
    
// -----------------------------------------------------------------------------
// Compare the numbers of points of two arrays at first. if they are not same, 
// add a new point from iPointArrayModel to iPointArrayDraw. then draw a new line
// -----------------------------------------------------------------------------
//
void CTruiHwrBox::DrawNextL()
    {
    // Check HwrBox state at first and then Check if model is drawn acompletely
    if ( iBoxState == EExistModelDraw &&
        iPointArrayModel.Count() > iPointArrayDraw.Count() )
        {
        // Add new point to iPointArrayDraw
        iPointArrayDraw.AppendL( iPointArrayModel[ iPointArrayDraw.Count() ] );
        // Update the screen
        DrawModel();
        }
    else
        {
        iBoxState = EExistModelStop;
        iTimeToDrawNext->Cancel();
        Window().Invalidate();
        }
    }
    
// ---------------------------------------------------------------------------
// Draw an existing model
// ---------------------------------------------------------------------------
//
void CTruiHwrBox::PlayAnimation( TBool aIsScale, const TSize& aRefSize )
    {
    if ( ( iBoxState == EExistModelStop || iBoxState == EExistModelDraw ) 
        && iPointArrayModel.Count() )
        {
        // Check and scale model points         
        if ( aIsScale )
            {
            // Scale model points
            ScaleModel( aRefSize, Rect().Size(), iPointArrayModel );
            }
            
        //Cancel the timer at first
        iTimeToDrawNext->Cancel();
        
        //Clear the box
        iPointArrayDraw.Reset();
        if ( Parent() )
            {            
            Parent()->DrawNow();            
            }        
        
        //Set index of point need to be drawn to the beginning
        iLastAnimatedPoint = KFirstPoint;
        
        //Set animate timer
        iTimeToDrawNext->Start( KFirstEventDelay, KDrawSpeed, TCallBack( TimeToDrawNextL, this ) );
        
        //Change box state
        iBoxState = EExistModelDraw;
        }
    }

// ---------------------------------------------------------------------------
// Clear the existing model
// ---------------------------------------------------------------------------
//
void CTruiHwrBox::ClearExistModel()
    {
    switch ( iBoxState )
        {
        case EExistModelDraw:
            {
            //animation is playing, cancel the timer before clear it
            iTimeToDrawNext->Cancel();
            break;
            }
        case ENewModelDraw:
            {
            //new model is inputting, cancel the timer before clear it
            iTimeToSave->Cancel();
            iStartSave = EFalse;
            break;
            }
        case EExistModelStop:
        case ENewModelStop:
        default:
            {
            }
        }
    iPointArrayModel.Reset();
    iPointArrayDraw.Reset();
    iBoxState = ENewModelStop;
    Window().Invalidate();
    }
        
// ---------------------------------------------------------------------------
// Handles an event from an observed control.
// ---------------------------------------------------------------------------
//
void CTruiHwrBox::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {        
    if ( iFlag & EReadOnlyHwrBox )
        {
        // Readonly, don't handle pointer event.
        return;
        }
            
    CCoeControl::HandlePointerEventL( aPointerEvent );    
    if ( AknLayoutUtils::PenEnabled() )
        {
        switch( aPointerEvent.iType )
            {
            case TPointerEvent::EButton1Down:
                {
                Window().Invalidate();
                if ( IsContained( aPointerEvent.iPosition ) )
                    {   
                    HideInfoPopupNote();                 
                    iPenInRect = ETrue;
                    if ( iBoxState == ENewModelStop )
                        {
                        //change box state
                        iBoxState = ENewModelDraw;
                        SetPointerCapture( ETrue );
                        //start from the point where button is down
                        iPointArrayDraw.Append( aPointerEvent.iPosition - Position() );
                        iLastAnimatedPoint = KFirstPoint;
                        DrawModel();
                        }
                    else if ( iBoxState == ENewModelDraw )
                        {
                        //cancel the iTimeToSave
                        iTimeToSave->Cancel();
                        iStartSave = EFalse;
                        //start from the point where button is down
                        iPointArrayDraw.Append( aPointerEvent.iPosition - Position() );
                        DrawModel();
                        }
                    else
                        {
                        if ( iHwrBoxObserver )
                            {
                            iHwrBoxObserver->HandleEventL( MTruiHwrBoxObserver::EModelExist );
                            }                        
                        }
                    }
                else
                    {
                    iPenInRect = EFalse;
                    }
                break;    
                }
            case TPointerEvent::EButton1Up:    
                {                
                if ( iBoxState == ENewModelDraw )
                    {
                    iPointArrayDraw.Append( KTail ); 
                    if ( !iStartSave )
                        {
                        iStartSave = ETrue;
                        iTimeToSave->Start( KWritePeriod,
                                            KWritePeriod,
                                            TCallBack( TimeToSaveL, this ) );
                        }
                    }
                break;
                }
            case TPointerEvent::EDrag:
            case TPointerEvent::EMove:
                {
                if ( iBoxState == ENewModelDraw )
                    {                    
                    if ( IsContained( aPointerEvent.iPosition ) )
                        {
                        Window().Invalidate();
                        HideInfoPopupNote(); 
                        //Pen go into available rect again, start an new trace
                        if ( iPenInRect == EFalse )
                            {
                            iTimeToSave->Cancel();
                            iStartSave = EFalse;
                            }
                        iPointArrayDraw.Append( aPointerEvent.iPosition - Position() );
                        DrawModel();
                        iPenInRect = ETrue;
                        }
                    else
                        {
                        if ( iPenInRect )
                            {
                            iPointArrayDraw.Append( KTail );                           
                            if ( !iStartSave )
                                {
                                iStartSave = ETrue;
                                iTimeToSave->Start( KWritePeriod,
                                                    KWritePeriod,
                                                    TCallBack( TimeToSaveL, 
                                                               this ) );
                                }
                            iPenInRect = EFalse;
                            }
                        }
                    }
                break;
                }
            default:
                break;
            }
        }
    }

// ---------------------------------------------------------------------------
// Handles an event from an observed control.
// ---------------------------------------------------------------------------
//
TKeyResponse CTruiHwrBox::OfferKeyEventL( const TKeyEvent& /*aKeyEvent*/,
                                                TEventCode /*aType*/ )
    {
    return EKeyWasConsumed;
    }
    
// --------------------------------------------------------------------------
// CTruiHwrBox::SetHwrBoxObserver
// --------------------------------------------------------------------------
//
void CTruiHwrBox::SetHwrBoxObserver( MTruiHwrBoxObserver* aHwrBoxObserver )
    {
    iHwrBoxObserver = aHwrBoxObserver;
    }
    
// --------------------------------------------------------------------------
// Get the HwrBox's model.
// --------------------------------------------------------------------------
//
RArray<TPoint>& CTruiHwrBox::Model()
    {
    return iPointArrayModel;
    }
    
// --------------------------------------------------------------------------
// Draw character model
// --------------------------------------------------------------------------
//
void CTruiHwrBox::DrawModel()
    {
    if ( IsActivated() && iPointArrayDraw.Count() &&
         iLastAnimatedPoint + 1 < iPointArrayDraw.Count() )
        {
        // Get system graphic content
        CWindowGc& gc = SystemGc();
        RWindow& window = Window();
        gc.Activate( window );  
        window.Invalidate( Rect() );
        window.BeginRedraw( Rect() );
        Draw( Rect() );
        window.EndRedraw();
        gc.Deactivate();        
        }
    }
    
// ---------------------------------------------------------------------------
// Responds to changes to the size and position of the contents of this control.
// ---------------------------------------------------------------------------
//
void CTruiHwrBox::SizeChanged()
    {
    LoadBorderLayout( 0 );
    for ( TInt i = 0; i < iBorderLayout.Count()
                      && i < iBorderBitmaps.Count()
                      && i < iBorderBitmapMasks.Count(); i++ )
        {
        TSize size = iBorderLayout[i].Rect().Size();
	    AknIconUtils::SetSize( iBorderBitmaps[i], size, EAspectRatioNotPreserved );
	    AknIconUtils::SetSize( iBorderBitmapMasks[i], size, EAspectRatioNotPreserved );        
        }
    
    LoadShadowLayout( 0 );
    for ( TInt i = 0; i < iShadowLayout.Count()
                      && i < iShadowBitmaps.Count()
                      && i < iShadowBitmapMasks.Count(); i++ )
        {
        TSize size = iShadowLayout[i].Rect().Size();
	    AknIconUtils::SetSize( iShadowBitmaps[i], size, EAspectRatioNotPreserved );
	    AknIconUtils::SetSize( iShadowBitmapMasks[i], size, EAspectRatioNotPreserved );
        }
    
    LoadGuideLineLayout( 0 );    
    // Guide line position
    TInt margin = Size().iWidth / KGuideLineWidthRadio;
    
    iGuideLineBottomL.iX = Position().iX + margin;
    iGuideLineTopL.iX = Position().iX + margin;
    
    for ( TInt i = 0; i < iGuideLineLayout.Count()
                      && i < iGuideLineBitmaps.Count()
                      && i < iGuideLineBitmapMasks.Count(); i++ )
        {
        TSize size = iGuideLineLayout[i].Rect().Size();
        size.iWidth = Size().iWidth - margin * 2;
	    AknIconUtils::SetSize( iGuideLineBitmaps[i], size, EAspectRatioNotPreserved );
	    AknIconUtils::SetSize( iGuideLineBitmapMasks[i], size, EAspectRatioNotPreserved );
        }
    
    // Caculate hwr_training_write_pane_g7 for drawable rect
    TRect rect = Rect();
    TAknWindowComponentLayout drawable_rect_layout = 
               AknLayoutScalable_Apps::hwr_training_write_pane_g7( 0 );
    TAknLayoutRect drawable_rect_layout_rect;
    drawable_rect_layout_rect.LayoutRect( rect, drawable_rect_layout );
    iDrawabelRect = drawable_rect_layout_rect.Rect();
    iDrawabelRect.Shrink( KLinePenSize.iWidth ,
                      KLinePenSize.iHeight );
    iPenColor = drawable_rect_layout_rect.Color();    

    TInt guidinglinetop(0);
    TInt guidinglinebottom(0);
    CTruiEngine* engine = static_cast<CTruiAppUi*>( iEikonEnv->AppUi() )->HwrEngine();
    engine->GetGuidingLinePos( iDrawabelRect.Size(), guidinglinetop, guidinglinebottom );
    SetGuidingLine( guidinglinetop, guidinglinebottom );    
    }

// ---------------------------------------------------------------------------
// From class CCoeControl.
// Handles a change to the control's resources.
// ---------------------------------------------------------------------------
//
void CTruiHwrBox::HandleResourceChange( TInt aType )
    {
    CCoeControl::HandleResourceChange( aType );
    if ( aType == KEikMessageUnfadeWindows )
        {
        DrawDeferred();
        }
    if ( aType == KAknsMessageSkinChange )
        {
        TRAP_IGNORE(
            LoadBorderBitmapsL();
            LoadShadowBitmapsL();
            LoadGuideLineBitmapsL();        
            )
        SizeChanged();
        }    
    }

// ---------------------------------------------------------------------------
// From class CCoeControl.
// Handle focus changed.
// ---------------------------------------------------------------------------
//
void CTruiHwrBox::FocusChanged( TDrawNow /*aDrawNow*/ )
    {    
    if ( !IsFocused() && iBoxState == ENewModelDraw )    
        {
        iPointArrayDraw.Append( KTail );
        TRAP_IGNORE( SaveModelL() );
        }
    }

// ---------------------------------------------------------------------------
// When load model from outside, set box state
// ---------------------------------------------------------------------------
//
void CTruiHwrBox::ModelIsReady( TBool aReady )
    {
    if ( aReady )
        {
        iBoxState = EExistModelStop;
        }
    else
        {
        iBoxState = ENewModelStop;
        }
    }

TRect CTruiHwrBox::DrawableRect() const
    {
    return iDrawabelRect;
    }

// ---------------------------------------------------------------------------
// Set editing mode to readonly or writable
// ---------------------------------------------------------------------------
//
void CTruiHwrBox::SetHwrBoxReadOnly( TBool aReadOnly )
    {
    TUint flag = aReadOnly ? EReadOnlyHwrBox : EWritableHwrBox;
    iFlag &= ( ~EReadOnlyHwrBox & ~EWritableHwrBox );
    iFlag |= flag;     
    }
    
// ---------------------------------------------------------------------------
// Get the Hwrbox's state
// ---------------------------------------------------------------------------
//
TInt CTruiHwrBox::BoxState() const
    {
    return iBoxState;
    }

// ---------------------------------------------------------------------------
// Pause the playing of animation
// ---------------------------------------------------------------------------
//
void CTruiHwrBox::PausePlaying()
    {
    if ( iBoxState == EExistModelDraw  )
        {
        iBoxState = EExistModelStop;
        iTimeToDrawNext->Cancel();
        Window().Invalidate();
        }
    }
    
// ---------------------------------------------------------------------------
// Resume the playing of animation
// ---------------------------------------------------------------------------
//
void CTruiHwrBox::ResumePlaying()
    {
    // Check HwrBox state at first and then Check if model is drawn acompletely
    if ( iBoxState == EExistModelStop &&
        iPointArrayModel.Count() > iPointArrayDraw.Count() )
        {
        
        //Cancel the timer at first
        iTimeToDrawNext->Cancel();

        //Set animate timer
        iTimeToDrawNext->Start( KFirstEventDelay, KDrawSpeed, 
                                TCallBack(TimeToDrawNextL, this) );
        
        //Change box state
        iBoxState = EExistModelDraw;
        }
    }

// ---------------------------------------------------------------------------
// Set Guiding Line
// ---------------------------------------------------------------------------
//
TInt CTruiHwrBox::SetGuidingLine( TInt aTop, TInt aBottom )
    {
    TRect rect = DrawableRect();
    if ( Size().iHeight <= aBottom || aBottom <= 0
         || Size().iHeight <= aTop || aTop <= 0 )
        {
        return KErrArgument;
        }
            
    iGuideLineTopL.iY = rect.iTl.iY + aTop;
    iGuideLineBottomL.iY = rect.iTl.iY + aBottom;
    return KErrNone;
    }
    
// ---------------------------------------------------------------------------
// Check if the point is contained in drawing area
// ---------------------------------------------------------------------------
// 
TBool CTruiHwrBox::IsContained( const TPoint& aPoint ) const
    {
    return DrawableRect().Contains( aPoint );
    }
   
// ---------------------------------------------------------------------------
// Adjust shortcut model's point to fit the popup hwr box. 
// ---------------------------------------------------------------------------
//    
void CTruiHwrBox::ScaleModel( const TSize& aOriginSize, 
                              const TSize& aRealSize, 
                              RArray<TPoint>& aModel )
    {
    CTruiEngine* engine = static_cast<CTruiAppUi*>( iEikonEnv->AppUi() )->HwrEngine();
    TInt originTop;    
    TInt originBottom;
    engine->GetGuidingLinePos( aOriginSize, originTop, originBottom );
    TInt realTop;
    TInt realBottom;
    engine->GetGuidingLinePos( aRealSize, realTop, realBottom );
    for ( TInt i =0; i < aModel.Count(); i++ )
        {                
        if ( aModel[i] != KTail )
            {
            aModel[i].iX = aModel[i].iX * realBottom / originBottom;
            aModel[i].iY = aModel[i].iY * realBottom / originBottom;            
            }        
        }
    }
    
// -----------------------------------------------------------------------------
// Draws all points.
// -----------------------------------------------------------------------------
//
void CTruiHwrBox::DrawTrails( CWindowGc& aGc ) const
    {
    // Draw model
    if ( iPointArrayDraw.Count() )
        {
        //Draw from the first point in the array
        TInt lastAnimatedPoint( 0 );
        TPoint relativePos;
        if ( OwnsWindow() )
            {
            relativePos = TPoint( 0, 0 );
            }
        else
            {
            relativePos = Position();
            }        
        aGc.Plot( iPointArrayDraw[lastAnimatedPoint] + relativePos );
                
        // Check if there is point need to be drawn
        // Plus 1 to lastAnimatedPointIndex since zero implies the first point
        // in the iLastAnimatedPoint
        while ( lastAnimatedPoint + 1 < iPointArrayDraw.Count() )
            {
            //Check if this is a new arc and increase the index counter
            if ( iPointArrayDraw[lastAnimatedPoint++] != KTail )
                {
                //It is not a new arc, then check if this is the end of an arc
                if ( iPointArrayDraw[lastAnimatedPoint] != KTail )
                    {
                    //It is not the end of an arc, so draw line
                    aGc.DrawLineTo( iPointArrayDraw[lastAnimatedPoint]
                                   + relativePos );
                    }
                }
            else
                {
                //It is a new arc, so move the pen to the new point 
                aGc.Plot( iPointArrayDraw[lastAnimatedPoint]
                         + relativePos );
                }
            }
        }
    }   

// --------------------------------------------------------------------------
// Set context for this control.
// --------------------------------------------------------------------------
//
void CTruiHwrBox::PrepareContext( CWindowGc& aGc ) const
    {
    TRgb color;
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    // Set pen color to the color of test in main area
    TInt error = AknsUtils::GetCachedColor( skin,
                                            color,
                                            KAknsIIDQsnTextColors,
                                            EAknsCIQsnTextColorsCG6 );
    if ( !error )
        {
        aGc.SetPenColor( color );
        }
    }
    
// --------------------------------------------------------------------------
// Load bitmaps for border.
// --------------------------------------------------------------------------
//
void CTruiHwrBox::LoadBorderBitmapsL()
    {
    iBorderBitmaps.ResetAndDestroy();
    iBorderBitmapMasks.ResetAndDestroy();
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    CFbsBitmap* bitmapLeft;
    CFbsBitmap* bitmapLeftMask;
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnGrafFepBgColour,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG14,
                                 bitmapLeft,
                                 bitmapLeftMask,
                                 KTRUIICONSFILENAME,
                                 EMbmTrui_iconsQgn_graf_fep_bg_colour,
                                 EMbmTrui_iconsQgn_graf_fep_bg_colour_mask,
                                 KRgbBlack
                                 );    
    iBorderBitmaps.Append( bitmapLeft );
    iBorderBitmapMasks.Append( bitmapLeftMask );

    CFbsBitmap* bitmapTop;
    CFbsBitmap* bitmapTopMask;
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnGrafFepBgColour,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG14,
                                 bitmapTop,
                                 bitmapTopMask,
                                 KTRUIICONSFILENAME,
                                 EMbmTrui_iconsQgn_graf_fep_bg_colour,
                                 EMbmTrui_iconsQgn_graf_fep_bg_colour_mask,
                                 KRgbBlack
                                 );    
    iBorderBitmaps.Append( bitmapTop );
    iBorderBitmapMasks.Append( bitmapTopMask );

    CFbsBitmap* bitmapBottom;
    CFbsBitmap* bitmapBottomMask;
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnGrafFepBgColour,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG14,
                                 bitmapBottom,
                                 bitmapBottomMask,
                                 KTRUIICONSFILENAME,
                                 EMbmTrui_iconsQgn_graf_fep_bg_colour,
                                 EMbmTrui_iconsQgn_graf_fep_bg_colour_mask,
                                 KRgbBlack
                                 );    
    iBorderBitmaps.Append( bitmapBottom );
    iBorderBitmapMasks.Append( bitmapBottomMask );

    CFbsBitmap* bitmapRight;
    CFbsBitmap* bitmapRightMask;
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnGrafFepBgColour,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG14,
                                 bitmapRight,
                                 bitmapRightMask,
                                 KTRUIICONSFILENAME,
                                 EMbmTrui_iconsQgn_graf_fep_bg_colour,
                                 EMbmTrui_iconsQgn_graf_fep_bg_colour_mask,
                                 KRgbBlack
                                 );    
    iBorderBitmaps.Append( bitmapRight );
    iBorderBitmapMasks.Append( bitmapRightMask );
    }

// --------------------------------------------------------------------------
// Load bitmaps for shadows.
// --------------------------------------------------------------------------
//
void CTruiHwrBox::LoadShadowBitmapsL()
    {
    iShadowBitmaps.ResetAndDestroy();
    iShadowBitmapMasks.ResetAndDestroy();    
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    CFbsBitmap* bitmapTl;
    CFbsBitmap* bitmapTlMask;
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnGrafFepPopupShadowTl,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG14,
                                 bitmapTl,
                                 bitmapTlMask,
                                 KTRUIICONSFILENAME,
                                 EMbmTrui_iconsQgn_graf_fep_popup_shadow_tl,
                                 EMbmTrui_iconsQgn_graf_fep_popup_shadow_tl_mask,
                                 KRgbBlack
                                 );    
    iShadowBitmaps.Append( bitmapTl );
    iShadowBitmapMasks.Append( bitmapTlMask );

    CFbsBitmap* bitmapT;
    CFbsBitmap* bitmapTMask;
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnGrafFepPopupShadowT,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG14,
                                 bitmapT,
                                 bitmapTMask,
                                 KTRUIICONSFILENAME,
                                 EMbmTrui_iconsQgn_graf_fep_popup_shadow_t,
                                 EMbmTrui_iconsQgn_graf_fep_popup_shadow_t_mask,
                                 KRgbBlack
                                 );    
    iShadowBitmaps.Append( bitmapT );
    iShadowBitmapMasks.Append( bitmapTMask );

    CFbsBitmap* bitmapTr;
    CFbsBitmap* bitmapTrMask;
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnGrafFepPopupShadowTr,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG14,
                                 bitmapTr,
                                 bitmapTrMask,
                                 KTRUIICONSFILENAME,
                                 EMbmTrui_iconsQgn_graf_fep_popup_shadow_tr,
                                 EMbmTrui_iconsQgn_graf_fep_popup_shadow_tr_mask,
                                 KRgbBlack
                                 );    
    iShadowBitmaps.Append( bitmapTr );
    iShadowBitmapMasks.Append( bitmapTrMask );

    CFbsBitmap* bitmapL;
    CFbsBitmap* bitmapLMask;
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnGrafFepPopupShadowL,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG14,
                                 bitmapL,
                                 bitmapLMask,
                                 KTRUIICONSFILENAME,
                                 EMbmTrui_iconsQgn_graf_fep_popup_shadow_l,
                                 EMbmTrui_iconsQgn_graf_fep_popup_shadow_l_mask,
                                 KRgbBlack
                                 );    
    iShadowBitmaps.Append( bitmapL );
    iShadowBitmapMasks.Append( bitmapLMask );

    CFbsBitmap* bitmapBl;
    CFbsBitmap* bitmapBlMask;
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnGrafFepPopupShadowBl,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG14,
                                 bitmapBl,
                                 bitmapBlMask,
                                 KTRUIICONSFILENAME,
                                 EMbmTrui_iconsQgn_graf_fep_popup_shadow_bl,
                                 EMbmTrui_iconsQgn_graf_fep_popup_shadow_bl_mask,
                                 KRgbBlack
                                 );    
    iShadowBitmaps.Append( bitmapBl );
    iShadowBitmapMasks.Append( bitmapBlMask );

    CFbsBitmap* bitmapR;
    CFbsBitmap* bitmapRMask;
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnGrafFepPopupShadowR,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG14,
                                 bitmapR,
                                 bitmapRMask,
                                 KTRUIICONSFILENAME,
                                 EMbmTrui_iconsQgn_graf_fep_popup_shadow_r,
                                 EMbmTrui_iconsQgn_graf_fep_popup_shadow_r_mask,
                                 KRgbBlack
                                 );    
    iShadowBitmaps.Append( bitmapR );
    iShadowBitmapMasks.Append( bitmapRMask );

    CFbsBitmap* bitmapB;
    CFbsBitmap* bitmapBMask;
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnGrafFepPopupShadowB,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG14,
                                 bitmapB,
                                 bitmapBMask,
                                 KTRUIICONSFILENAME,
                                 EMbmTrui_iconsQgn_graf_fep_popup_shadow_b,
                                 EMbmTrui_iconsQgn_graf_fep_popup_shadow_b_mask,
                                 KRgbBlack
                                 );    
    iShadowBitmaps.Append( bitmapB );
    iShadowBitmapMasks.Append( bitmapBMask );

    CFbsBitmap* bitmapBr;
    CFbsBitmap* bitmapBrMask;
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnGrafFepPopupShadowBr,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG14,
                                 bitmapBr,
                                 bitmapBrMask,
                                 KTRUIICONSFILENAME,
                                 EMbmTrui_iconsQgn_graf_fep_popup_shadow_br,
                                 EMbmTrui_iconsQgn_graf_fep_popup_shadow_br_mask,
                                 KRgbBlack
                                 );    
    iShadowBitmaps.Append( bitmapBr );
    iShadowBitmapMasks.Append( bitmapBrMask );
    }

// --------------------------------------------------------------------------
// Load bitmaps for guidelines.
// --------------------------------------------------------------------------
//
void CTruiHwrBox::LoadGuideLineBitmapsL()
    {
    iGuideLineBitmaps.ResetAndDestroy();
    iGuideLineBitmapMasks.ResetAndDestroy();    
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    CFbsBitmap* bitmapTop;
    CFbsBitmap* bitmapTopMask;
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnGrafFepBgColour,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG14,
                                 bitmapTop,
                                 bitmapTopMask,
                                 KTRUIICONSFILENAME,
                                 EMbmTrui_iconsQgn_graf_fep_bg_colour,
                                 EMbmTrui_iconsQgn_graf_fep_bg_colour_mask,
                                 KRgbBlack
                                 );    
    iGuideLineBitmaps.Append( bitmapTop );
    iGuideLineBitmapMasks.Append( bitmapTopMask );

    CFbsBitmap* bitmapBottom;
    CFbsBitmap* bitmapBottomMask;
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnGrafFepBgColour,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG14,
                                 bitmapBottom,
                                 bitmapBottomMask,
                                 KTRUIICONSFILENAME,
                                 EMbmTrui_iconsQgn_graf_fep_bg_colour,
                                 EMbmTrui_iconsQgn_graf_fep_bg_colour_mask,
                                 KRgbBlack
                                 );    
    iGuideLineBitmaps.Append( bitmapBottom );
    iGuideLineBitmapMasks.Append( bitmapBottomMask );
    }

// --------------------------------------------------------------------------
// Load layouts for borders from LAF.
// --------------------------------------------------------------------------
//
void CTruiHwrBox::LoadBorderLayout( TInt aVarity )
    {
    TRect rect = Rect();
    iBorderLayout.Reset();
    // Caculate hwr_training_write_pane_g1
    TAknWindowComponentLayout border_left_layout = 
               AknLayoutScalable_Apps::hwr_training_write_pane_g1( aVarity );
    TAknLayoutRect border_left_layout_rect;
    border_left_layout_rect.LayoutRect( rect, border_left_layout );    
    iBorderLayout.Append( border_left_layout_rect );

    TAknWindowComponentLayout border_top_layout = 
               AknLayoutScalable_Apps::hwr_training_write_pane_g2( aVarity );
    TAknLayoutRect border_top_layout_rect;
    border_top_layout_rect.LayoutRect( rect, border_top_layout );    
    iBorderLayout.Append( border_top_layout_rect );
    
    TAknWindowComponentLayout border_bottom_layout = 
               AknLayoutScalable_Apps::hwr_training_write_pane_g3( aVarity );
    TAknLayoutRect border_bottom_layout_rect;
    border_bottom_layout_rect.LayoutRect( rect, border_bottom_layout );    
    iBorderLayout.Append( border_bottom_layout_rect );

    TAknWindowComponentLayout border_right_layout = 
               AknLayoutScalable_Apps::hwr_training_write_pane_g4( aVarity );
    TAknLayoutRect border_right_layout_rect;
    border_right_layout_rect.LayoutRect( rect, border_right_layout );    
    iBorderLayout.Append( border_right_layout_rect );
    }

// --------------------------------------------------------------------------
// Load layouts for shadows from LAF.
// --------------------------------------------------------------------------
//
void CTruiHwrBox::LoadShadowLayout( TInt aVarity )
    {
    TRect rect = Rect();
    iShadowLayout.Reset();
    // Caculate bg_frame_shadow_pane
    TAknWindowComponentLayout shadow_pane_layout = 
               AknLayoutScalable_Apps::bg_frame_shadow_pane( aVarity );
    TAknLayoutRect shadow_pane_layout_rect;
    shadow_pane_layout_rect.LayoutRect( rect, shadow_pane_layout );
    TRect shadowPaneRect = shadow_pane_layout_rect.Rect();
    
    // Caculate bg_frame_shadow_pane_g1
    TAknWindowComponentLayout shadow_pane_tl_layout = 
               AknLayoutScalable_Apps::bg_frame_shadow_pane_g1( aVarity );
    TAknLayoutRect shadow_pane_tl_layout_rect;
    shadow_pane_tl_layout_rect.LayoutRect( shadowPaneRect, shadow_pane_tl_layout );    
    iShadowLayout.Append( shadow_pane_tl_layout_rect );

    // Caculate bg_frame_shadow_pane_g2
    TAknWindowComponentLayout shadow_pane_t_layout = 
               AknLayoutScalable_Apps::bg_frame_shadow_pane_g2( aVarity );
    TAknLayoutRect shadow_pane_t_layout_rect;
    shadow_pane_t_layout_rect.LayoutRect( shadowPaneRect, shadow_pane_t_layout );
    iShadowLayout.Append( shadow_pane_t_layout_rect );

    // Caculate bg_frame_shadow_pane_g3
    TAknWindowComponentLayout shadow_pane_tr_layout = 
               AknLayoutScalable_Apps::bg_frame_shadow_pane_g3( aVarity );
    TAknLayoutRect shadow_pane_tr_layout_rect;
    shadow_pane_tr_layout_rect.LayoutRect( shadowPaneRect, shadow_pane_tr_layout );
    iShadowLayout.Append( shadow_pane_tr_layout_rect );

    // Caculate bg_frame_shadow_pane_g4
    TAknWindowComponentLayout shadow_pane_l_layout = 
               AknLayoutScalable_Apps::bg_frame_shadow_pane_g4( aVarity );
    TAknLayoutRect shadow_pane_l_layout_rect;
    shadow_pane_l_layout_rect.LayoutRect( shadowPaneRect, shadow_pane_l_layout );
    iShadowLayout.Append( shadow_pane_l_layout_rect );

    // Caculate bg_frame_shadow_pane_g5
    TAknWindowComponentLayout shadow_pane_bl_layout = 
               AknLayoutScalable_Apps::bg_frame_shadow_pane_g5( aVarity );
    TAknLayoutRect shadow_pane_bl_layout_rect;
    shadow_pane_bl_layout_rect.LayoutRect( shadowPaneRect, shadow_pane_bl_layout );
    iShadowLayout.Append( shadow_pane_bl_layout_rect );

    // Caculate bg_frame_shadow_pane_g6
    TAknWindowComponentLayout shadow_pane_r_layout = 
               AknLayoutScalable_Apps::bg_frame_shadow_pane_g6( aVarity );
    TAknLayoutRect shadow_pane_r_layout_rect;
    shadow_pane_r_layout_rect.LayoutRect( shadowPaneRect, shadow_pane_r_layout );
    iShadowLayout.Append( shadow_pane_r_layout_rect );

    // Caculate bg_frame_shadow_pane_g7
    TAknWindowComponentLayout shadow_pane_b_layout = 
               AknLayoutScalable_Apps::bg_frame_shadow_pane_g7( aVarity );
    TAknLayoutRect shadow_pane_b_layout_rect;
    shadow_pane_b_layout_rect.LayoutRect( shadowPaneRect, shadow_pane_b_layout );
    iShadowLayout.Append( shadow_pane_b_layout_rect );

    // Caculate bg_frame_shadow_pane_g8
    TAknWindowComponentLayout shadow_pane_br_layout = 
               AknLayoutScalable_Apps::bg_frame_shadow_pane_g8( aVarity );
    TAknLayoutRect shadow_pane_br_layout_rect;
    shadow_pane_br_layout_rect.LayoutRect( shadowPaneRect, shadow_pane_br_layout );
    iShadowLayout.Append( shadow_pane_br_layout_rect );
    }

// --------------------------------------------------------------------------
// Load layouts for guidelines from LAF.
// --------------------------------------------------------------------------
//
void CTruiHwrBox::LoadGuideLineLayout( TInt aVarity )
    {
    TRect rect = Rect();
    iGuideLineLayout.Reset();
    // Caculate hwr_training_write_pane_g5
    TAknWindowComponentLayout top_guideline_layout = 
               AknLayoutScalable_Apps::hwr_training_write_pane_g5( aVarity );
    TAknLayoutRect top_guideline_layout_rect;
    top_guideline_layout_rect.LayoutRect( rect, top_guideline_layout );
    iGuideLineLayout.Append( top_guideline_layout_rect );

    // Caculate hwr_training_write_pane_g6
    TAknWindowComponentLayout bottom_guideline_layout = 
               AknLayoutScalable_Apps::hwr_training_write_pane_g6( aVarity );
    TAknLayoutRect bottom_guideline_layout_rect;
    bottom_guideline_layout_rect.LayoutRect( rect, bottom_guideline_layout );
    iGuideLineLayout.Append( bottom_guideline_layout_rect );
    }

// --------------------------------------------------------------------------
// Draw borders.
// --------------------------------------------------------------------------
//
void CTruiHwrBox::DrawBorders( CWindowGc& aGc ) const
    {
    for ( TInt i = 0; i < iBorderLayout.Count(); i++ )
        {
        iBorderLayout[i].DrawImage( aGc, iBorderBitmaps[i], iBorderBitmapMasks[i] );        
        }
    }

// --------------------------------------------------------------------------
// Draw shadows.
// --------------------------------------------------------------------------
//
void CTruiHwrBox::DrawShadows( CWindowGc& aGc ) const
    {
    for ( TInt i = 0; i < iShadowLayout.Count(); i++ )
        {
        iShadowLayout[i].DrawImage( aGc, iShadowBitmaps[i], iShadowBitmapMasks[i] );
        }
    }
    
// --------------------------------------------------------------------------
// Draw guidelines.
// --------------------------------------------------------------------------
//
void CTruiHwrBox::DrawGuideLines( CWindowGc& aGc ) const
    {
    if ( iTopGuideLineDisplay && KTopGuideLineIndex < iGuideLineLayout.Count()
                              && KTopGuideLineIndex < iGuideLineBitmaps.Count()
                              && KTopGuideLineIndex < iGuideLineBitmapMasks.Count() )
        {        
        TSize pixSize = iGuideLineBitmaps[KTopGuideLineIndex]->SizeInPixels();
        aGc.BitBltMasked( iGuideLineTopL, iGuideLineBitmaps[KTopGuideLineIndex], 
                         TRect( TPoint( 0, 0 ), pixSize ), 
                         iGuideLineBitmapMasks[KTopGuideLineIndex], EFalse );        
        }
        
    if ( iBottomGuideLineDisplay && KBottomGuideLineIndex < iGuideLineLayout.Count()
                              && KBottomGuideLineIndex < iGuideLineBitmaps.Count()
                              && KBottomGuideLineIndex < iGuideLineBitmapMasks.Count() )
        {
        TSize pixSize = iGuideLineBitmaps[KBottomGuideLineIndex]->SizeInPixels();                
        aGc.BitBltMasked( iGuideLineBottomL, iGuideLineBitmaps[KBottomGuideLineIndex], 
                         TRect( TPoint( 0, 0 ), pixSize ), 
                         iGuideLineBitmapMasks[KBottomGuideLineIndex], EFalse );
        }
    }  
    
// -----------------------------------------------------------------------------
// Show trails.
// -----------------------------------------------------------------------------
//
void CTruiHwrBox::ShowTrails( const RArray<TPoint>& aPointArray, 
                              TBool aIsScale,
                              const TSize& aRefSize )
    {    
    // Set values fo iPointArrayDraw
    if ( aPointArray.Count() > 0 )
        {
        iPointArrayDraw.Reset();
        for ( TInt i = 0; i < aPointArray.Count(); i++ )
            {
            iPointArrayDraw.Append( aPointArray[i] );
            }
        }
    
    // Adjust the trails in terms of HwrBox size
    if ( aIsScale )
        {
        // Scale model points
        ScaleModel( aRefSize, iDrawabelRect.Size(), iPointArrayDraw );
        }
    DrawNow();
    }
    
// --------------------------------------------------------------------------
// CTruiHwrBox::ShowTooltipL
// --------------------------------------------------------------------------
//
void CTruiHwrBox::ShowTooltipL( const TDesC& aText, 
                                const TPoint& aPosition,
                                const TInt aBeforeTimeout,
                                const TInt aInViewTimeout )
    {
    if ( aText.Length() < 0 )
        {
        return;
        }
    iTooltip->SetTextL( aText );
    iTooltip->SetTimeDelayBeforeShow( aBeforeTimeout );
    iTooltip->SetTimePopupInView( aInViewTimeout );
    iTooltip->SetPositionAndAlignment( aPosition, EHCenterVTop );

    iTooltip->ShowInfoPopupNote();        
    }

void CTruiHwrBox::SetTooltipPosition( const TPoint& aPosition )
    {
    if ( iTooltip )
        {
        iTooltip->SetPositionAndAlignment( aPosition, EHCenterVTop );
        }
    }

// --------------------------------------------------------------------------
// Hide tooltip.
// --------------------------------------------------------------------------
//
void CTruiHwrBox::HideInfoPopupNote()
    {
    if ( iTooltip )
        {
        iTooltip->HideInfoPopupNote();
        }    
    }

// --------------------------------------------------------------------------
// Handles events reported from info popup note.
// --------------------------------------------------------------------------
//
void CTruiHwrBox::HandleInfoPopupNoteEvent( CAknInfoPopupNoteController* aController,
        TAknInfoPopupNoteEvent aEvent )
    {
    if ( aController == iTooltip && aEvent == EInfoPopupNoteHidden )
        {        
        DrawNow();        
        }
    }

// -----------------------------------------------------------------------------
// Save the new inputting model by manual.
// -----------------------------------------------------------------------------
//
void CTruiHwrBox::SaveModelByManuallyL()
    {
    if ( iPointArrayDraw.Count() && !iStartSave )
        {
        // Has drawn
        if ( iPointArrayDraw[iPointArrayDraw.Count() - 1] != KTail )
            {
            iPointArrayDraw.Append( KTail );            
            }
        SaveModelL();
        }        
    }
