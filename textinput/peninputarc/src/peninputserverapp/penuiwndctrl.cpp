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
#include <AknSgcc.h>
#include <AknIncallBubbleNotify.h>
#include <avkon.hrh>
#include <eikappui.h>
#include <apgwgnam.h>

#include "penuiwndctrl.h"
#ifdef RD_UI_TRANSITION_EFFECTS_POPUPS
#include <gfxtranseffect/gfxtranseffect.h>
#include <akntransitionutils.h>
#include <akntranseffect.h>

#include <e32property.h>
#include <avkondomainpskeys.h>
#endif
#include "peninputcmd.h"

const TInt KAknCapServerUid = 0x10207218;
const TInt KAknNotifySrvUid = 0x10281EF2;  

const TInt KAutoRefreshTimer = 10; //ms
const TInt KAutoRefreshLongTimer = 1000; //ms

TInt AutoRefreshTimerCallback( TAny* aObject )
    {
    CPenUiWndCtrl* wnd = static_cast<CPenUiWndCtrl*>( aObject );
    wnd->RefreshUI();

    return 0;
    }

CPenUiWndCtrl::CPenUiWndCtrl(RWindowGroup& aWndGroup,CFbsBitmap * aBitmap)
    :iWndGroup(aWndGroup),
     iBitmap(aBitmap),
     iAknUiSrv(CAknSgcClient::AknSrv())
{
}

CPenUiWndCtrl::~CPenUiWndCtrl()
{
    delete iPopupWnd;

	delete iIncallBubble;  
#ifdef RD_UI_TRANSITION_EFFECTS_POPUPS

    if ( GfxTransEffect::IsRegistered( this ) )
        {
        // abort component effects (possibly appear effect) 
        // if aknnote gets destroyed without user interaction or timers
        GfxTransEffect::NotifyExternalState( ENotifyGlobalAbort );
        GfxTransEffect::Deregister(this); //Always deregister in destructor.
        }    
#endif   
    if ( iAutoRefreshTimer && iAutoRefreshTimer->IsActive() )
        {
        iAutoRefreshTimer->Cancel();
        }
    delete iAutoRefreshTimer; 
    

	Clean();

    
	delete iCursorWnd;   
}

void CPenUiWndCtrl::ConstructL()
{
    CreateWindowL(iWndGroup);
    SetComponentsToInheritVisibility();

    Window().SetRequiredDisplayMode( EColor16MA );
    iIncallBubble = CAknIncallBubble::NewL();        

#ifdef RD_UI_TRANSITION_EFFECTS_POPUPS    
    // only change registration, if not registered, don't register
    if (!GfxTransEffect::IsRegistered( this ) ) 
        {
        GfxTransEffect::Register( this, TUid::Uid( 0x2000B477 ));
        }
#endif    
  //  ActivateL();
    iPopupWnd = CPenUiPopWnd::NewL(iWndGroup,iBitmap,this);
    
    iAutoRefreshTimer = CPeriodic::NewL( CActive::EPriorityStandard );

    iCursorWnd = new (ELeave) CCursorWindow(this);
    //iCursorWnd->ConstructL(this);
    iCursorWnd->SetContainerWindowL(*this);
    
    // Update the cursor color
    SetCursorColor();
    
    MakeVisible( EFalse );
}


void CPenUiWndCtrl::Draw(const TRect& aRect) const
    {
    if (iNotUpdating)
        return;

    CWindowGc& gc = SystemGc();
    if (iShowPopup)
        {
        //layout is extended by popup
        TRect rect = aRect;
        rect.Intersection(iLayoutClipRect);
        TPoint pos = rect.iTl - iLayoutClipRect.iTl;
        gc.BitBlt(pos, iBitmap, rect);
#ifdef FIX_FOR_NGA
        //draw bubble
        for ( TInt i = 0; i < iBubblesCtrl.Count(); ++i )
            {
            gc.BitBlt(iBubblesPos[i].iTl, iBubblesArea[i]);
            }
#endif   
        return;
        //layout is extended by popup
        //gc.SetClippingRect( iLayoutClipRect );
        }
#ifdef FIX_FOR_NGA
    gc.BitBlt(TPoint(0, 0), iBitmap, Rect());
    //draw bubble
    for ( TInt i = 0; i < iBubblesCtrl.Count(); ++i )
        {
        gc.BitBlt(iBubblesPos[i].iTl, iBubblesArea[i]);
        }
#else    
    gc.BitBlt(aRect.iTl,iBitmap,aRect);
#endif     

    }

void CPenUiWndCtrl::RefreshUI()
    {
    StopRefreshTimer();
    
    if ( iShowPopup )
        {
        iPopupWnd->DrawNow();
        }
    else
        {
        DrawNow();
        }
    CCoeEnv::Static()->WsSession().Flush();
    CCoeEnv::Static()->WsSession().Finish();
    
    }

void CPenUiWndCtrl::Clean()
    {
    iCursorBmp = NULL;
    iBubblesArea.Close();
    iBubblesCtrl.Close();
    iBubblesPos.Close();
	iBubblesMaskArea.Close();
	iPopRegion.Close();
    iChangedBmp= NULL;
    iBackground = EFalse;
    iNotUpdating = EFalse;
    }

CCursorWindow::CCursorWindow(CPenUiWndCtrl* aParent)
                            :iParent(aParent) 
    {
    }

void CCursorWindow::SetCursorVisible(TBool aFlag)
    {
    iCursorVisible = aFlag;
    DrawNow();
    }
void CCursorWindow::Draw(const TRect &aRect ) const
    {
    if(!iCursorVisible)
        return;
   
    CWindowGc& gc = SystemGc();
    iParent->DrawCursor(gc);  
    }

// ---------------------------------------------------------------------------
// CPenUiWndCtrl::SetCursorColor
// ---------------------------------------------------------------------------
//
void CPenUiWndCtrl::SetCursorColor()
	{
	// Set default cursor color to black
	TRgb color( KRgbBlack );
	// if the skin is enabled
    if ( AknsUtils::AvkonSkinEnabled())
        {
        // Get the text color under current theme
        AknsUtils::GetCachedColor( AknsUtils::SkinInstance(),
	                               color, 
	                               KAknsIIDQsnTextColors, 
	                               EAknsCIQsnTextColorsCG60 );
        }
    
    // Set the cursor color
    iCursorColor = color;
	}

void CPenUiWndCtrl::DrawCursor(CWindowGc& aGc) const
    {
    aGc.SetPenColor( iCursorColor );
    aGc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    aGc.SetBrushColor( iCursorColor );
    aGc.SetDrawMode(CGraphicsContext::EDrawModeNOTSCREEN);
    aGc.SetPenStyle(CGraphicsContext::ESolidPen);
    aGc.SetPenSize( TSize(1,1));
    
    TRect drawRect = iCursorRect;
    if(!iPopRegion.IsEmpty() && iPopRegion.Intersects(iCursorRect))
        {
        RRegion r;
        r.AddRect(iCursorRect);
        const TRect* rl = iPopRegion.RectangleList();
        
        for(TInt i = 0 ; i < iPopRegion.Count(); ++i)
            {
            drawRect = iCursorRect;
            drawRect.Intersection(rl[i]);
            if(!drawRect.IsEmpty())
                r.SubRect(drawRect);
            }
        
        for(TInt ii = 0; ii < r.Count(); ++ii)
            {
        aGc.DrawRect(r.RectangleList()[ii]);
            }
        r.Close();
        }
    else
        aGc.DrawRect(iCursorRect);

    // restore normal draw mode
    aGc.SetDrawMode(CGraphicsContext::EDrawModePEN);
    aGc.SetBrushStyle(CGraphicsContext::ENullBrush);
    
    }
void CPenUiWndCtrl::UpdateCursor(TBool aOnFlag,const CFbsBitmap* aCursorBmp,const TRect& aRect)
    {
    if(iNotUpdating || !IsReadyToDraw())
        {
        //some times when layout is diabled, the cursor is still change its pos, and draw to the last place
        //when layout enabled. So we set the cursor invisible, and wait for next cursor updating event.
		iCursorWnd->SetCursorVisible(EFalse);
        return;
        }
    
    iCursorPos = aRect.iTl;
	
    if(iCursorRect != aRect)
        {
        iCursorRect = aRect;
        iCursorWnd->SetRect(iCursorRect);
        }
    iCursorWnd->SetCursorVisible(aOnFlag);
    }

// ---------------------------------------------------------------------------
// CPenUiWndCtrl::UpdateBubble
// ---------------------------------------------------------------------------
//
void CPenUiWndCtrl::UpdateBubble( const TUint32 aCtrl, 
		                          const CFbsBitmap* aBmp,
		                          const CFbsBitmap* aMask,
                                  const TRect& aPos,
                                  TBool aFlag )
    {

	// Check whether the Ctrl address is exist
	TInt idx = iBubblesCtrl.Find( aCtrl );

    
    if(aFlag)
        {
        if(KErrNotFound == idx)
            {

            iBubblesCtrl.Append( aCtrl );

            iBubblesArea.Append(aBmp);
            iBubblesMaskArea.Append(aMask);
            iBubblesPos.Append(aPos);
            }
        else
            {
            iBubblesPos[idx] = aPos;
            }

        }
    else
        {
        //remove
        if(idx != KErrNotFound)
            {

            iBubblesCtrl.Remove( idx );

            iBubblesArea.Remove(idx);
            iBubblesMaskArea.Remove(idx);
            iBubblesPos.Remove(idx);            
            }        
        }
    Invalidate(Rect(), ETrue);
    }
void CPenUiWndCtrl::UpdateICFArea(const CFbsBitmap* aBmp,const TPoint& aPos)
    {
    iICFBmp = const_cast<CFbsBitmap*>(aBmp);
    iIcfPos = aPos;
    Invalidate(Rect(), ETrue);   
    }


// ---------------------------------------------------------------------------
// CPenUiWndCtrl::UpdateChangedArea
// ---------------------------------------------------------------------------
//
void CPenUiWndCtrl::UpdateChangedArea( const TUint32 aCtrl, 
		const CFbsBitmap* aBmp,const TRect& aPos,TBool aFlag)
    {
    UpdateBubble( aCtrl, aBmp, 0, aPos, aFlag );
    return;    
    }


void CPenUiWndCtrl::SetPopupArea(const TRect& aRect, TBool aFlag)
    {
    if(aFlag) //add pop area
        {
        iPopRegion.AddRect(aRect);
        }
    else
        {
        iPopRegion.SubRect(aRect);
        }
    }

TInt CPenUiWndCtrl::WndPriority()
    {
    return iPriority;
    }
    
TUid CPenUiWndCtrl::WindowGroupNameAppUidL( RWsSession& aWS, TInt aWGId )
    {
    CApaWindowGroupName* wg = CApaWindowGroupName::NewLC( aWS,aWGId );
    TUid id = wg->AppUid();
    CleanupStack::PopAndDestroy(wg);
    return id;
    }
    
TInt CPenUiWndCtrl::GetWndPriority()
    {
    RWsSession &ws = CCoeEnv::Static()->WsSession();
    TInt wgId =ws.GetFocusWindowGroup();    
    
    TUid id = {0x00000000};
    TRAP_IGNORE( id = WindowGroupNameAppUidL( ws, wgId ) );
    
    TBool isGlobalServs = ( id.iUid == KAknCapServerUid || id.iUid == KAknNotifySrvUid );
    if( ( iInGlobalEditorState && isGlobalServs ) || !isGlobalServs )
        {
         //TUid id = GetFocusAppUid();
        //set the window group priority so as pen UI can be shown and global notes can also
        //be shown.
        TInt priority = ws.GetWindowGroupOrdinalPriority(wgId);
        (priority == ECoeWinPriorityNormal) ? ++priority : priority;// + 1;
        iPreEditorPriority = priority;
        return priority;
        }
        
    return iPreEditorPriority;
    }
    
void CPenUiWndCtrl::ShowPenUiL(TBool /*aDimmed*/)
    {
    iCursorWnd->SetCursorVisible(EFalse);
    if (iResourceChange)
        {
        TInt curPriority = iWndGroup.OrdinalPriority();
        if ( curPriority == -1 )
            {
            // Make sure pen ui must be seen.
            LiftUpPriority();
            }
        ActivateL();
        iIncallBubble->SetIncallBubbleFlagsL( EAknStatusBubbleInputShow );
        MakeVisible( ETrue );
    	ClaimPointerGrab( ETrue );
        return;
        }

    TInt priority = GetWndPriority();
    iPriority =  priority;
    iWndGroup.SetOrdinalPosition( 0, iPriority);
    DrawableWindow()->SetOrdinalPosition( 0, iPriority);
    //OnActivate();
    iWndGroup.SetNonFading(ETrue);
    //ChangeSysteFadeStatus();
    ActivateL();
    iIncallBubble->SetIncallBubbleFlagsL( EAknStatusBubbleInputShow );
   // MakeVisible( ETrue );
	ClaimPointerGrab( ETrue );
	
	
#ifdef RD_UI_TRANSITION_EFFECTS_POPUPS      
	
    if ( iEnableGfxTransEffect && GfxTransEffect::IsRegistered( this ) && !IsVisible())
        {
        iEnableGfxTransEffect = EFalse;
        this->MakeVisible(EFalse);
        GfxTransEffect::NotifyExternalState( ENotifyGlobalAbort ); 

        //If still visible, do a transition to invisible state.
        CAknTransitionUtils::SetAllParents(this);
        GfxTransEffect::Begin(this, KGfxControlAppearAction);
        GfxTransEffect::NotifyExternalState(ECaptureComponentsBegin, (const TDesC8*)this);
        
        TRect demarcation;
        CAknTransitionUtils::GetDemarcation(CAknTransitionUtils::EPopup, 
                                            demarcation);
        GfxTransEffect::SetDemarcation(this, demarcation);

        //todo fix NGA effects error
        CCoeEnv::Static()->WsSession().Finish();
        User::After( 1 );
        
        this->MakeVisible(ETrue);
        
        GfxTransEffect::NotifyExternalState(ECaptureComponentsEnd, (const TDesC8*)this);
        GfxTransEffect::End(this);
         }
   

    else
        {
#endif // RD_UI_TRANSITION_EFFECTS_POPUPS   
        MakeVisible( ETrue );
#ifdef RD_UI_TRANSITION_EFFECTS_POPUPS            
        }
#endif // RD_UI_TRANSITION_EFFECTS_POPUPS       
    }

void CPenUiWndCtrl::ClosePenUi(TBool aResChanging)
    {
    if (aResChanging)
        {
        TRAP_IGNORE(iIncallBubble->SetIncallBubbleFlagsL( EAknStatusBubbleInputHide ));
        MakeVisible( EFalse );
        return;
        }
#ifdef RD_UI_TRANSITION_EFFECTS_POPUPS           
    if ( iEnableGfxTransEffect && GfxTransEffect::IsRegistered( this ) )
        {
        iEnableGfxTransEffect = EFalse;
        GfxTransEffect::NotifyExternalState( ENotifyGlobalAbort ); 

        //If still visible, do a transition to invisible state.
        CAknTransitionUtils::SetAllParents(this);
        GfxTransEffect::Begin(this, KGfxControlDisappearAction);
        GfxTransEffect::NotifyExternalState(ECaptureComponentsBegin, (const TDesC8*)this);
        
        TRect demarcation;
        CAknTransitionUtils::GetDemarcation(CAknTransitionUtils::EPopup, 
                                            demarcation);
        GfxTransEffect::SetDemarcation(this, demarcation);

        //todo fix NGA effects error
        CCoeEnv::Static()->WsSession().Finish();
        User::After( 1 );
        
        this->MakeVisible(EFalse);
        
        GfxTransEffect::NotifyExternalState(ECaptureComponentsEnd, (const TDesC8*)this);
        GfxTransEffect::End(this);
         }
    else
        {
#endif // RD_UI_TRANSITION_EFFECTS_POPUPS   
        MakeVisible( EFalse );
#ifdef RD_UI_TRANSITION_EFFECTS_POPUPS            
        }
#endif // RD_UI_TRANSITION_EFFECTS_POPUPS

    iShowPopup = EFalse;
    iWndGroup.SetOrdinalPosition( -1, ECoeWinPriorityNormal-1 );
    DrawableWindow()->SetOrdinalPosition(-1, ECoeWinPriorityNormal-1 );
    TRAP_IGNORE(iIncallBubble->SetIncallBubbleFlagsL( EAknStatusBubbleInputHide ));
}


void CPenUiWndCtrl::ShowPopup( const TRect& aRectInScreen, const TRect& aRectInLayout, const TRect& aLayoutTrimRect )
    {
    TPoint offset = aLayoutTrimRect.iTl;
    offset += Position();
    this->SetPosition( offset );
    this->SetSize( aLayoutTrimRect.Size() );
    
    iPopupWnd->PopUp( aRectInScreen, aRectInLayout );
    iLayoutClipRect = aLayoutTrimRect;
    iShowPopup = ETrue;
    }

void CPenUiWndCtrl::ClosePopup()
    {
    iShowPopup = EFalse;    
    iPopupWnd->Close();
    }
TKeyResponse CPenUiWndCtrl::OfferKeyEventL( const TKeyEvent& /*aKeyEvent*/, TEventCode /*aType*/ )
    {
    return EKeyWasNotConsumed;
    }
    
void CPenUiWndCtrl::Invalidate(const TRect& aRect,TBool /*aFullUpdate*/)
    {
    //Window().Invalidate(aRect);
    /*********************************************************************/
    //Invalidate() should be used. But I don't know why there the rect in 
    // Draw is always full-screen in such case. So I change to DrawNow
    // for temprory solution.
    /*********************************************************************/ 
    if ( !IsVisible() )
        {
        return;
        }
    
    if(iShowPopup)
        {        
        iPopupWnd->MakeVisible(EFalse);//This is a temproary fix due to pop up UI not updated.
        iPopupWnd->Invalidate(aRect);
        iPopupWnd->MakeVisible(ETrue);//This is a temproary fix due to pop up UI not updated.
        iPopupWnd->Invalidate(aRect);
        }
    else
        {
        iInvalidateRect = aRect;
        Window().Invalidate(aRect);
        }
#ifdef FIX_FOR_NGA
    RestartRefreshTimer();
#endif
    }

void CPenUiWndCtrl::OnActivate(EditorType aType)
    {
    //TBool dim = IsDimmed();
    RestoreSystemFadeStatus();
	
	// if we were opened under a global note
	// we need to dim pen ui
	if(aType != EGlobleNotesWithEditor && aType != EGlobleNotes)
	   {
	   //have to call this, otherwise pen UI is faded
       iWndGroup.SetNonFading(ETrue);
	   }
    
    if (iResourceChange)
        {
        return;    
        }
    TInt priority = GetWndPriority();
    
    switch (aType)
        {
        case EGlobleNotesWithEditor:
            {
            iInGlobalEditorState = ETrue;
            iWndGroup.SetOrdinalPosition( 0, priority);
            DrawableWindow()->SetOrdinalPosition( 0, priority);
            iPriority = priority;                
            }
        break;
        case EGlobleNotes:
        default:
            {
            iInGlobalEditorState = EFalse;
            if( priority != iPriority)
                {
                //iDimForClientMenu = EFalse;
                iWndGroup.SetOrdinalPosition( 0, priority);
                DrawableWindow()->SetOrdinalPosition( 0, priority);
                iPriority = priority;
                }                
            }
        }
    }

void CPenUiWndCtrl::OnDeactivate()
    {
    iWndGroup.SetNonFading(EFalse);
    }    

void CPenUiWndCtrl::ChangeSysteFadeStatus()
    {
    TBool faded = IsDimmed();//EFalse;
    //iAknUiSrv->IsSystemFaded(faded);

    if (faded)
        {
        iChangeSystemFadeStatus = ETrue;
        iAknUiSrv->SetSystemFaded(EFalse);
        }
    }

void CPenUiWndCtrl::Fade()
    {
    //iAknUiSrv->SetSystemFaded(ETrue) ;
    iWndGroup.SetFaded(ETrue,RWindowTreeNode::EFadeIncludeChildren);
    }
    
TBool CPenUiWndCtrl::IsDimmed()
    {
    TBool faded = EFalse;
    iAknUiSrv->IsSystemFaded(faded);    
    return faded;    
    }
void CPenUiWndCtrl::RestoreSystemFadeStatus()
    {
    if(iChangeSystemFadeStatus)
        {
        iAknUiSrv->SetSystemFaded(EFalse);
        iChangeSystemFadeStatus = EFalse;
        }
    }
    
void CPenUiWndCtrl::DimPenUiForMenu()    
    {
    iWndGroup.SetNonFading(EFalse);
    Window().SetFaded(ETrue,RWindowTreeNode::EFadeWindowOnly);
    //return;
    /*RWsSession &ws = CCoeEnv::Static()->WsSession();
    TInt wgId =ws.GetFocusWindowGroup();    
    TInt priority = ws.GetWindowGroupOrdinalPriority(wgId);
    if(iPriority != priority)
        {
        //TInt p1 = DrawableWindow()->FullOrdinalPosition();
        //TInt p2 = DrawableWindow()->OrdinalPosition();
        
        iWndGroup.SetOrdinalPosition( 0, priority);
        DrawableWindow()->SetOrdinalPosition( 0, priority);
        //p1 = DrawableWindow()->FullOrdinalPosition();
        //p2 = DrawableWindow()->OrdinalPosition();
        }*/
    }
static TBool bTest = EFalse;
    
void CPenUiWndCtrl::DimPenUiForMenu(TInt aPriority)    
    {
    if(bTest)
        {
        MakeVisible(EFalse);
        return;
        }
    
    if (!iResourceChange)
        {
        if(iPriority != aPriority)
            {
            iWndGroup.SetOrdinalPosition( 0, aPriority);
            DrawableWindow()->SetOrdinalPosition( 0, aPriority);
            iPriority = aPriority;
            }
        DimPenUiForMenu(); 
        }
    
   
    }
 
void CPenUiWndCtrl::DimInternalPenUiForMenu(TInt aPriority)    
    {
    if(iPriority != aPriority)
        {
        iWndGroup.SetOrdinalPosition( 0, aPriority);
        DrawableWindow()->SetOrdinalPosition( 0, aPriority);
        iPriority = aPriority;
        }

    return;
    } 
       
void CPenUiWndCtrl::SetResourceChange(TBool aResourceChange)
    {
    iResourceChange = aResourceChange;
    }
    
void CPenUiWndCtrl::HandleNGASpecificSignal(TInt aEventType, const TDesC& aEventData)
    {
    switch(aEventType)
        {
        case ESignalPopupWndClosed:
            {               
            ClosePopup();
            }               
            break;
        case ESignalUpdateCursor :
            {
            struct SData
                {
                TBool onOff;
                CFbsBitmap* bmp;
                TRect rect;
                } data;
            
            data = * (reinterpret_cast<SData*>( const_cast<TUint16*>( aEventData.Ptr() )));
            UpdateCursor(data.onOff,data.bmp,data.rect);
              
            }
            break;
        case ESignalPopupArea:
            {
            struct SData
                {
                TRect rr;
                TBool flag;
                } data;
            
            data = * (reinterpret_cast<SData*>( const_cast<TUint16*>( aEventData.Ptr() )));
            SetPopupArea(data.rr,data.flag);
            }
            break;
        case ESignalUpdateICFArea:            
            {
            struct SData
                {
                CFbsBitmap* bmp;                        
                TPoint pos;
                } data;
            
            data = * (reinterpret_cast<SData*>( const_cast<TUint16*>( aEventData.Ptr() )));
            UpdateICFArea(data.bmp,data.pos);
            }
            break;
        case ESignalUpdateBubble:
            {
            struct SData
                {
            	TUint32 ctrl;
                TBool flag;
                TRect pos;
                CFbsBitmap* bmp;
                CFbsBitmap* mask;
                } data;
            data = * (reinterpret_cast<SData*>( const_cast<TUint16*>( aEventData.Ptr() )));
            UpdateBubble( data.ctrl, data.bmp, data.mask, data.pos, data.flag );
            }
            break;
        case ESignalUpdateChangedArea:
            {
            struct SData
                {
            	TUint32 ctrl;
                TBool flag;
                CFbsBitmap* bmp;
                TRect pos;
                } data;
            data = * (reinterpret_cast<SData*>( const_cast<TUint16*>( aEventData.Ptr() )));
            UpdateChangedArea( data.ctrl, data.bmp, data.pos, data.flag );
            }
            break;
        case ESignalRegisterBkControl:
            {
            
            struct SData
                {
                //TBool bChangeFrameId;
        
                TAknsItemID frameID;
                TAknsItemID centerID;        
                RArray<TCommonBgCtrlSubItem> *subItems;
                } data;
            data = * (reinterpret_cast<SData*>( const_cast<TUint16*>( aEventData.Ptr() )));
            iSubItems = data.subItems;
            iFrameID = data.frameID;
            iCenterID = data.centerID;
            iBackground = ETrue;
            }
            break;
        case ESignalDisableUpdating:
            {
            iNotUpdating = * (reinterpret_cast<TBool*>( const_cast<TUint16*>( aEventData.Ptr() )));
			if(iNotUpdating)
			    {
				UpdateCursor(EFalse,NULL,iCursorWnd?iCursorWnd->Rect():TRect(TPoint(0,0),TSize(0,0)));
				}
            }
			break;
        case ESignalDrawBackground:
            {
            struct SData
                {
                CFbsBitGc* gc;
                CFbsBitmap* bmp;
                TRect rect;
                } data;
            data = * (reinterpret_cast<SData*>( const_cast<TUint16*>( aEventData.Ptr() )));
            DrawBubbleAsBackground(data.gc,data.bmp,data.rect);
            }
            break;
        default:
            break;
        }
    
    }

void CPenUiWndCtrl::DrawBkground(CWindowGc& aGc,const TRect& aRect)  const
    {
    
    DrawFrame(aGc,aRect, iFrameID, iCenterID);
    
    for( TInt i = 0; i < (*iSubItems).Count(); i++ )
        {
        if( (*iSubItems)[i].iIsShow )
            {
            DrawFrame(aGc, (*iSubItems)[i].iRect, 
                       (*iSubItems)[i].iFrameID, 
                       (*iSubItems)[i].iCenterID );                
            }
        }       
    
    }

void CPenUiWndCtrl::DrawBubbleAsBackground(CFbsBitGc* aGc, CFbsBitmap* aBmp, const TRect& aRect)
    {
    for(TInt i = 0 ; i < iBubblesArea.Count(); ++i)
        {
        if(iBubblesArea[i] != aBmp) //not draw same bitmap
            {
            //gc.BitBlt(iBubblesPos[i],iBubblesArea[i]);
            TRect r = aRect;
            TRect rect2 = iBubblesPos[i];
            r.Intersection(rect2);
            if(!r.IsEmpty())
                {
                TRect src = r;
                r.Move(TPoint(-aRect.iTl.iX,-aRect.iTl.iY));
                src.Move(TPoint(-rect2.iTl.iX,-rect2.iTl.iY));
                aGc->BitBlt(r.iTl, iBubblesArea[i], src);
                }
            }
        }
    
    }


CCoeControl* CPenUiWndCtrl::ComponentControl(TInt) const
    {
    return iCursorWnd;
    }
TInt CPenUiWndCtrl::CountComponentControls() const
    {
    return 1;
    }


void CPenUiWndCtrl::DrawFrame(CWindowGc& aGc, const TRect& aRect,TAknsItemID aFrameID,
                                                TAknsItemID aCenterID )  const
    {    
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    TRect innerRect = aRect;
    innerRect.Shrink( 4, 4 );
    
    AknsDrawUtils::DrawFrame(
                            skin, aGc, 
                            aRect, innerRect, 
                            aFrameID, aCenterID);
    }

void CPenUiWndCtrl::RestartRefreshTimer()
    {
    StopRefreshTimer();

    TTimeIntervalMicroSeconds32 t1 = KAutoRefreshTimer * 1000;
    TTimeIntervalMicroSeconds32 t2 = KAutoRefreshLongTimer * 1000;
    iAutoRefreshTimer->Start( t1, t2, TCallBack(AutoRefreshTimerCallback, this) );
    }

void CPenUiWndCtrl::StopRefreshTimer()
    {
    if ( iAutoRefreshTimer->IsActive() )
        {
        iAutoRefreshTimer->Cancel();
        }
    }
	
void CPenUiWndCtrl::LiftUpPriority()
    {
	TInt priority = GetWndPriority();
    iPriority =  priority;
    iWndGroup.SetOrdinalPosition( 0, iPriority);
    DrawableWindow()->SetOrdinalPosition( 0, iPriority);
	}
	
//End Of File
// class CInternalBkCtrl
CInternalBkCtrl::CInternalBkCtrl(RWindowGroup& aWndGroup)    
            :iWndGroup(aWndGroup)
    {
    }
 
void CInternalBkCtrl::ConstructL()
    {
    CreateWindowL(iWndGroup);
    SetComponentsToInheritVisibility(EFalse);
    MakeVisible(EFalse);
    //SetCanDrawOutsideRect();   
    }
//class for pop up window

CPenUiPopWnd* CPenUiPopWnd::NewL(RWindowGroup& aWndGroup,CFbsBitmap * aBitmap,
                                                          CPenUiWndCtrl* aCtrl)
    {
    CPenUiPopWnd* self = new (ELeave) CPenUiPopWnd(aWndGroup,aBitmap,aCtrl);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;    
    }
CPenUiPopWnd::CPenUiPopWnd(RWindowGroup& aWndGroup,CFbsBitmap * aBitmap,
                                                        CPenUiWndCtrl* aCtrl)
    :iWndGroup(aWndGroup),
     iBitmap(aBitmap),
     iAknUiSrv(CAknSgcClient::AknSrv()),
     iBackgroundCtrl(aCtrl)
{
}

CPenUiPopWnd::~CPenUiPopWnd()
{
}

void CPenUiPopWnd::ConstructL()
{
    CreateWindowL(iWndGroup);
    SetComponentsToInheritVisibility();

    Window().SetRequiredDisplayMode( EColor16MA );
    //Window().EnableRedrawStore(EFalse);    
    MakeVisible( EFalse );
    ActivateL();
    
}

void CPenUiPopWnd::PopUp(TInt aPriority)
    {
    //iWndGroup.SetOrdinalPosition( 0, ECoeWinPriorityNormal);    
    Window().SetFaded(EFalse,RWindowTreeNode::EFadeWindowOnly);
    DrawableWindow()->SetOrdinalPosition( 0, aPriority);      
    MakeVisible( ETrue );
    Window().Invalidate();
    }
void CPenUiPopWnd::Invalidate(const TRect& aRect)
    {
    //Window().Invalidate(aRect);
    DrawNow(aRect);
    }

void CPenUiPopWnd::PopUp(const TRect& aRectInScreen, const TRect aRectInLayout )
    {
    iRectInLayout = aRectInLayout;
    SetRect( aRectInScreen );
    iWndGroup.SetNonFading(ETrue);
    PopUp(iBackgroundCtrl->WndPriority());   
    Window().FadeBehind(ETrue); 
    }

void CPenUiPopWnd::Close()
    {
    Window().FadeBehind(EFalse);
    MakeVisible(EFalse);   
    iWndGroup.SetNonFading(ETrue);    
    Window().ClearRedrawStore();
    }
    
void CPenUiPopWnd::Draw(const TRect& /*aRect*/) const
    {
    CWindowGc& gc = SystemGc();
    gc.BitBlt( TPoint(0,0), iBitmap, iRectInLayout ); 
    }

