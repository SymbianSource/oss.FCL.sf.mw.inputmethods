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


const TInt KAknCapServerUid = 0x10207218;
const TInt KAknNotifySrvUid = 0x10281EF2;  
    
CPenUiWndCtrl::CPenUiWndCtrl(RWindowGroup& aWndGroup,CFbsBitmap * aBitmap)
    :iWndGroup(aWndGroup),
     iBitmap(aBitmap),
     iAknUiSrv(CAknSgcClient::AknSrv())
{
}

CPenUiWndCtrl::~CPenUiWndCtrl()
{
    delete iPopupWnd;

#ifdef RD_UI_TRANSITION_EFFECTS_POPUPS

    if ( GfxTransEffect::IsRegistered( this ) )
        {
        // abort component effects (possibly appear effect) 
        // if aknnote gets destroyed without user interaction or timers
        GfxTransEffect::NotifyExternalState( ENotifyGlobalAbort );
        GfxTransEffect::Deregister(this); //Always deregister in destructor.
        }
#endif    	
}

void CPenUiWndCtrl::ConstructL()
{
    CreateWindowL(iWndGroup);
    SetComponentsToInheritVisibility();

    Window().SetRequiredDisplayMode( EColor16MA );
    MakeVisible( EFalse );
#ifdef RD_UI_TRANSITION_EFFECTS_POPUPS    
    // only change registration, if not registered, don't register
    if (!GfxTransEffect::IsRegistered( this ) ) 
        {
        GfxTransEffect::Register( this, TUid::Uid( 0x2000B477 ));
        }
#endif    
  //  ActivateL();
    iPopupWnd = CPenUiPopWnd::NewL(iWndGroup,iBitmap,this);
}


void CPenUiWndCtrl::Draw(const TRect& aRect) const
    {
    CWindowGc& gc = SystemGc();
        
    if ( iShowPopup ) 
        {
        //layout is extended by popup
        gc.SetClippingRect( iLayoutClipRect );
        gc.BitBlt( aRect.iTl, iBitmap, aRect );
        // Add to fix NGA refresh problem
        CCoeEnv::Static()->WsSession().Flush();
        CCoeEnv::Static()->WsSession().Finish(); 
        return;
        }
    
    gc.BitBlt(aRect.iTl,iBitmap,aRect);
    // Add to fix NGA refresh problem
    CCoeEnv::Static()->WsSession().Flush();
    CCoeEnv::Static()->WsSession().Finish(); 
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
    if (iResourceChange)
        {
        ActivateL();
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
   // MakeVisible( ETrue );
	ClaimPointerGrab( ETrue );
	
	
#ifdef RD_UI_TRANSITION_EFFECTS_POPUPS      
	
    if ( GfxTransEffect::IsRegistered( this ) && !IsVisible())
        {
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
        MakeVisible( EFalse );
        return;
        }
#ifdef RD_UI_TRANSITION_EFFECTS_POPUPS           
    if ( GfxTransEffect::IsRegistered( this ))
        {

        GfxTransEffect::NotifyExternalState( ENotifyGlobalAbort ); 

        //If still visible, do a transition to invisible state.
        CAknTransitionUtils::SetAllParents(this);
        GfxTransEffect::Begin(this, KGfxControlDisappearAction);
        GfxTransEffect::NotifyExternalState(ECaptureComponentsBegin, (const TDesC8*)this);
        
        TRect demarcation;
        CAknTransitionUtils::GetDemarcation(CAknTransitionUtils::EPopup, 
                                            demarcation);
        GfxTransEffect::SetDemarcation(this, demarcation);
        
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

    //iResourceChange = EFalse;
    iShowPopup = EFalse;
    iWndGroup.SetOrdinalPosition( -1, ECoeWinPriorityNormal-1 );
    DrawableWindow()->SetOrdinalPosition(-1, ECoeWinPriorityNormal-1 );
    //MakeVisible( EFalse );
      
}


void CPenUiWndCtrl::ShowPopup( const TRect& aRectInScreen, const TRect& aRectInLayout, const TRect& aLayoutTrimRect )
    {
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
    
    if(iShowPopup)
        {        
        iPopupWnd->MakeVisible(EFalse);//This is a temproary fix due to pop up UI not updated.
        iPopupWnd->Invalidate(aRect);
        iPopupWnd->MakeVisible(ETrue);//This is a temproary fix due to pop up UI not updated.
        iPopupWnd->Invalidate(aRect);
        iPopupWnd->DrawNow();
        }
    else
        //DrawNow(aRect);
        {
        iInvalidateRect = aRect;
        Window().Invalidate(aRect);
        }
    }

void CPenUiWndCtrl::OnActivate(EditorType aType)
    {
    //TBool dim = IsDimmed();
    RestoreSystemFadeStatus();
    //have to call this, otherwise pen UI is faded
    iWndGroup.SetNonFading(ETrue);
    
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
    // Add to fix NGA refresh problem
    CCoeEnv::Static()->WsSession().Flush();
    CCoeEnv::Static()->WsSession().Finish(); 
    }

