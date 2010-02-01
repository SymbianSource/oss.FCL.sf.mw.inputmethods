/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for CFepUiLayout
*
*/


#include <w32std.h>
#include <s32mem.h>
#include <peninputcmd.h>
#include "peninputcmdparamext.h"
#include "peninputlayout.h"
#include "peninputlayoutbutton.h"
#include "peninputlayoutrootctrl.h"

#include <aknfeppeninputenums.h>
#include <AknDef.h>
#include <AknsConstants.h>
#include <coemain.h>

#ifdef RD_TACTILE_FEEDBACK
#include <touchfeedback.h>
#include <featmgr.h>
#endif //RD_TACTILE_FEEDBACK
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CFepUiLayout::CFepUiLayout
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiLayout::CFepUiLayout(MLayoutOwner* aLayoutOwner)
                           :iLayoutOwner(aLayoutOwner),
                           iDragbarNum(0),iLockedArea(TRect(0,0,0,0)),
                           iLayoutReady(EFalse)
    {
    TPixelsTwipsAndRotation ptSize;             
    CCoeEnv::Static()->ScreenDevice()->
                            GetDefaultScreenSizeAndRotation(ptSize);
    //set the screen size in case any one need use it.                            
    iScreenSize = ptSize.iPixelSize;  
#ifdef RD_TACTILE_FEEDBACK
    iTactileSupported = EFalse;
#endif // RD_TACTILE_FEEDBACK
    }

// ---------------------------------------------------------------------------
// CFepUiLayout::~CFepUiLayout
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiLayout::~CFepUiLayout()
    {
#ifdef RD_TACTILE_FEEDBACK                  
    //remove all registered area
    SignalOwner(ESignalDeRegisterAllFeedbackArea);
#endif // RD_TACTILE_FEEDBACK 
    delete iRootCtrl;        
    } 

// ---------------------------------------------------------------------------
// CFepUiLayout::BaseConstructL
// Do basic contructor
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepUiLayout::BaseConstructL()
    {
    iRootCtrl = CFepUiLayoutRootCtrl::NewL(this);
    __ASSERT_DEBUG(iLayoutOwner,User::Leave(EUiLayoutNotReady));     
    }

// ---------------------------------------------------------------------------
// CFepUiLayout::HandleEventL
// handle event from window server
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CFepUiLayout::HandleEventL(TEventType aType, const TAny* aData)
    {   
    TRawEvent event=*(static_cast<const TRawEvent*>(aData));
    if(ERawEvent == aType)
        {
        //handle raw key event
        if(TRawEvent::EKeyUp == event.Type() || TRawEvent::EKeyDown == event.Type())
            {            
            return HandleRawKeyEventL(event);
            }
        else
            {
            if(TRawEvent::EButton1Down == event.Type())
                {
                if(!iRootCtrl->CtrlCapPointer() && 
                   !LayoutOwner()->LayoutRect().Contains(event.Pos()))
                    {
                    //point outside of ui layout
                    OnPointerEventOutsideLayout(event);
                    return EFalse;
                    } 
                }
            }
        }
        
    TBool handled = iRootCtrl->HandleEventL(aType,aData);   
    return handled;    
    }

// ---------------------------------------------------------------------------
// CFepUiLayout::SendEditorTextAndCursorPosL
// handle layout command
// ---------------------------------------------------------------------------
//
void CFepUiLayout::SendEditorTextAndCursorPosL(TUint8* aData)
    {
    RDesReadStream readStream;
    
    TPtr8 countPtr( aData, 2*sizeof(TInt), 2*sizeof(TInt) );            
	readStream.Open(countPtr);
	CleanupClosePushL(readStream);
    const TInt dataCount = readStream.ReadInt32L();
	const TInt textCount = readStream.ReadInt32L();
    CleanupStack::PopAndDestroy(&readStream);
    
    TPtr8 ptr( aData+2*sizeof(TInt), dataCount+textCount, dataCount+textCount );            
	readStream.Open(ptr);
	CleanupClosePushL(readStream);
	
	HBufC8* dataBuf = HBufC8::NewLC(dataCount);
	TPtr8 dataBufPtr = dataBuf->Des();
	readStream.ReadL(dataBufPtr, dataCount);

    TFepInputContextFieldData* pIcfData = 
        reinterpret_cast<TFepInputContextFieldData*>(const_cast<TUint8*>(dataBufPtr.Ptr()));

    HBufC8* textBuf = HBufC8::NewLC(textCount);
	TPtr8 textBufPtr = textBuf->Des();
	readStream.ReadL(textBufPtr, textCount);
    pIcfData->iText.Set( reinterpret_cast<const TUint16*>(textBufPtr.Ptr()),
                         textCount/2);
                         
    OnAppEditorTextComing(*pIcfData);
    
    CleanupStack::PopAndDestroy(textBuf);
    CleanupStack::PopAndDestroy(dataBuf);
	CleanupStack::PopAndDestroy(&readStream);
    }

// ---------------------------------------------------------------------------
// CFepUiLayout::HandleCommand
// handle layout command
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CFepUiLayout::HandleCommand(TInt aCmd, TUint8* aData)
    { 
    TInt ret = KErrNone;
    switch( aCmd )
        {
        case ECmdPenInputSendEditorTextAndCurPos:
            {
            TRAP_IGNORE(SendEditorTextAndCursorPosL(aData));
            }
            break;
        default: 
            {
            ret = -1;
            }	
            break;
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// CFepUiLayout::InitL
// Initialize the layout
// ---------------------------------------------------------------------------
//
EXPORT_C TRect CFepUiLayout::Init()
    {
    //Init control    
    TRect r = iRootCtrl->InitControl();
    r.Move( Position() );
    for(TInt i = 0; i < iDragbarNum; i++)
        {
        iDragbarList[i]->AddPositionObserver(static_cast<MPositionObserver*>
                                                                (iRootCtrl)); 
        }
    //iLayoutReady = ETrue;
    return r;
    }

// ---------------------------------------------------------------------------
// CFepUiLayout::AddControlL
// Add a control to layout
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepUiLayout::AddControlL(CFepUiBaseCtrl* aControl,
                                                    TBool aEventObserverFlag)
    {
    if(!aControl)
        return;
    
    iRootCtrl->AddControlL(aControl);
    //add event observer
    if(aEventObserverFlag)
        aControl->AddEventObserver(this);
    }

// ---------------------------------------------------------------------------
// CFepUiLayout::AddControlL
// Add a control to layout
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepUiLayout::AddControlL(CCoeControl* aControl,
                                                    TBool aEventObserverFlag)
    {
    if(!aControl)
        return;
    
    CFepUiBaseCtrl* ctrl = iRootCtrl->AddControlL(aControl);
    //add event observer
    if(aEventObserverFlag && ctrl)
        ctrl->AddEventObserver(this);
    }


// ---------------------------------------------------------------------------
// CFepUiLayout::RemoveControl
// Remove a control to layout
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepUiLayout::RemoveControl(CFepUiBaseCtrl* aControl)
    {    
    if(!aControl)
        return;
    //check whether the control is a dragbar
    if(aControl->IsKindOfControl(ECtrlDragBar))
        {
        for(TInt i = 0; i < iDragbarNum; i++)
            {
            if(aControl == iDragbarList[i])
                {
                iDragbarNum --;
                for(TInt j = i+1; j < iDragbarNum; j++)
                    {                    
                    iDragbarList[j-1] = iDragbarList[j];
                    }
                break;
                }                
            }
        }
    //Remove from event list
    if(aControl)
        aControl->RemoveEventObserver(this);        
        
    //remove control        
    iRootCtrl->RemoveControl(aControl);    
    //aControl is deleted, so reset the pointer.
    aControl = 0;
    }

// ---------------------------------------------------------------------------
// CFepUiLayout::Draw
// Draw layout
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepUiLayout::Draw()
    {
    iRootCtrl->Draw();
    
    }

// ---------------------------------------------------------------------------
// CFepUiLayout::HandleRawKeyEventL
// Handle key event
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CFepUiLayout::HandleRawKeyEventL(const TRawEvent& /*aKeyEvent*/)
    {    
    //let application process the key.
    return EFalse; 
    }

// ---------------------------------------------------------------------------
// CFepUiLayout::SignalOwner
// Report event to owner
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//            
EXPORT_C void CFepUiLayout::SignalOwner(TInt aEventType, const TDesC& aEventData)
    {
    iLayoutOwner->SignalOwner(aEventType,aEventData);
    }

// ---------------------------------------------------------------------------
// CFepUiLayout::OnAppEditorTextComing
// Set editor area text when application send text to ui layout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//         
EXPORT_C TInt CFepUiLayout::OnAppEditorTextComing(const TFepInputContextFieldData& /*aData*/)
    {    
    //do nothing
    return KErrNone;
    }
    
// ---------------------------------------------------------------------------
// CFepUiLayout::GetEditorMaxLength
// Get the max editor length of the input context field
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CFepUiLayout::GetInputContextFieldMaxLen()
    {
    return iRootCtrl->GetInputContextFieldMaxLen();
    }

// ---------------------------------------------------------------------------
// CFepUiLayout::IsValidDestination
// Test whether the rect conflicts with other controls
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C TBool CFepUiLayout::IsValidDestination(const TRect& aRect,
                    CDragBar* aDragBar, TBool& aVInfo,TBool& aHInfo,TBool aFlag)
    {
    //default implementation.
    if(aFlag) //layout is moving
        {
        TRect screen(TPoint(0,0),iScreenSize);        
        
        screen.Intersection(aRect);
        if(screen != aRect)  //any part of dragbar can't be outside of screen
            {
            aVInfo = EFalse;
            aHInfo = EFalse;            
            return EFalse;
            }
        else
            {            
            aVInfo = ETrue;
            aHInfo = ETrue;            
            return ETrue;
            }
        }
        
    //check for component moving       
    return iRootCtrl->IsValidDestination(aRect,aDragBar,aVInfo,aHInfo);    
    }
    

// -----------------------------------------------------------------------------
// CFepUiLayout::SetRect
// Set layout rect
// -----------------------------------------------------------------------------
//
EXPORT_C void CFepUiLayout::SetRect(const TRect& aRect)
    {
    TRect layoutRect = Rect();
    layoutRect.Move(Position());
    if(layoutRect != aRect)
        {        
        iRootCtrl->SetRect(aRect);
        LayoutOwner()->LayoutSizeChanged( aRect.Size() );
        LayoutOwner()->SetPosition( aRect.iTl );
        iRootCtrl->GraphicDeviceSizeChanged();
        }
    }

// ---------------------------------------------------------------------------
// CFepUiLayout::Rect
// Deprecated function
// Get layout rect
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C TRect CFepUiLayout::Rect()
    {
    return iRootCtrl->Rect();
    }

// ---------------------------------------------------------------------------
// CFepUiLayout::Position
// Get layout start position
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C TPoint CFepUiLayout::Position()
    {
    return LayoutOwner()->LayoutRect().iTl;
    }
   
// ---------------------------------------------------------------------------
// CFepUiLayout::SetAppEditorCursor
// Handle changing app editor cursor event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CFepUiLayout::SetAppEditorCursor(const TCursorSelection& aCurSel,
                                                           TBool aReqMoreText)
    {
    TPtrC16 ptr;

    TInt size = sizeof(TBool) + sizeof(TCursorSelection) + 1;
    
    TUint8* packedData = new TUint8[size];
    if(!packedData)
        return;

    TInt cpLen = sizeof(TCursorSelection);
    Mem::Copy(packedData,&aCurSel,cpLen);
    Mem::Copy(packedData+cpLen,&aReqMoreText,sizeof(TBool));

    ptr.Set(reinterpret_cast<TUint16*>(packedData),size / 2);    

    SignalOwner(ESignalSetAppEditorCursor,ptr);
    
    delete []packedData;
    }
    
// ---------------------------------------------------------------------------
// CFepUiLayout::Destroy
// Destroy the layout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CFepUiLayout::Destroy()
    {
    delete this;
    }    

// ---------------------------------------------------------------------------
// CFepUiLayout::OnActivate
// Response to layout activation
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CFepUiLayout::OnActivate()
    {
    iLayoutReady = ETrue;
    iRootCtrl->OnActivate();
#ifdef RD_TACTILE_FEEDBACK  
    iTactileSupported = FeatureManager::FeatureSupported( KFeatureIdTactileFeedback );
#endif // RD_TACTILE_FEEDBACK      
    }
    
// ---------------------------------------------------------------------------
// CFepUiLayout::OnDeActivate
// Response to layout de-activation
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//         
EXPORT_C void CFepUiLayout::OnDeActivate()
    {
    iLayoutReady = EFalse;
    iRootCtrl->OnDeActivate();
    }    
// ---------------------------------------------------------------------------
// CFepUiLayout::SetLayoutPos
// Set layout position
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CFepUiLayout::SetLayoutPos(const TPoint& aPt)
    {
    if(aPt != Position())    
        iLayoutOwner->SetPosition(aPt);    
    }

// ---------------------------------------------------------------------------
// CFepUiLayout::EnableLayoutMoving
// Enable that the layout can be dragged by a dragbar
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CFepUiLayout::EnableLayoutMoving(CDragBar* aDragbar)
    {
    if(aDragbar && (iDragbarNum < KMaxDragBarNum))
        {        
        for(TInt i = 0; i < iDragbarNum; i++)
            {
            if(aDragbar == iDragbarList[i])
                return; //do nothing if already there
            }
        iDragbarList[iDragbarNum] = aDragbar;
        iDragbarNum++;
        //set rootcontrol as an observer of the dragbar
        aDragbar->AddEventObserver(iRootCtrl);
        }
    }
    
    
// ---------------------------------------------------------------------------
// CFepUiLayout::SetShadowRect
// Set layout's shadow rect
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//         
EXPORT_C void CFepUiLayout::SetShadowRect(const TRect& aRect)
    {
    iRootCtrl->SetShadowRect(aRect);
    }
    
// ---------------------------------------------------------------------------
// CFepUiLayout::SetShadowAreaRect
// Set layout's shadow rect for an area
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//         
EXPORT_C void CFepUiLayout::SetShadowAreaRect(const TRect& aRect, 
                                                    TShadowBitmapIndex aIndex)
    {
    iRootCtrl->SetShadowAreaRect(aRect,aIndex);
    }
        
// CFepUiLayout::SetShadowBmp
// set layout's shadow bitmap
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//             
EXPORT_C void CFepUiLayout::SetShadowBmp(CFbsBitmap* aBmp,CFbsBitmap* aMaskBmp,
                                TShadowBitmapIndex aIndex)
    {
    iRootCtrl->SetShadowBmp(aBmp,aMaskBmp,aIndex);
    }

// CFepUiLayout::LockArea
// Lock an area
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//             
EXPORT_C void CFepUiLayout::LockArea(const TRect& aRect, CFepUiBaseCtrl* aOwner)
    {
    iLockedArea = aRect;
    iLockedAreaOwner = aOwner;
    }

// CFepUiLayout::UnLockArea
// unlock an area
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//   
EXPORT_C void CFepUiLayout::UnLockArea(const TRect& /*aRect*/, 
                                                        CFepUiBaseCtrl* aCtrl)
    {
    if(aCtrl == iLockedAreaOwner)
        {
        iLockedArea.SetRect(0,0,0,0);
        }
    }
// ---------------------------------------------------------------------------
// CFepUiLayout::UpdateArea
// Update layout area
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//       
EXPORT_C void CFepUiLayout::UpdateArea(const CFepUiBaseCtrl* aCtrl,
                        const TRect& aRect,TBool aUpdateFlag,TBool aImmedFlag)
    {
    //do nothing if it's locked and aCtrl is not the owner.
    TBool bUpdate = ETrue;
    if(!iLockedArea.IsEmpty() )
        {
        bUpdate = EFalse;
        if( iLockedAreaOwner == aCtrl )
            {
            bUpdate = ETrue;
            }
        else if( iLockedAreaOwner->IsKindOfControl(ECtrlControlGroup) )
            {
            CControlGroup* ctrlGroup = static_cast<CControlGroup*>(iLockedAreaOwner);
            if( ctrlGroup->ControlList().Find( aCtrl ) != KErrNotFound )
                {
                bUpdate = ETrue;
                }
            }
        }
    if( bUpdate )
        {
        if(iRootCtrl!=aCtrl)
            iRootCtrl->RedrawRectIfNeeded(aRect,const_cast<CFepUiBaseCtrl*>(aCtrl));
        iLayoutOwner->UpdateArea(aRect,aUpdateFlag,aImmedFlag);            
        }
    }    

// ---------------------------------------------------------------------------
// CFepUiLayout::OnPointerEventOutsideLayout
// Call back when the pointer down/up event happends outside of the layout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//       
EXPORT_C TBool CFepUiLayout::OnPointerEventOutsideLayout(
                                                  const TRawEvent& /*aEvent*/)
    {
    //doesn't process the event
    return EFalse;    
    }
    
// ---------------------------------------------------------------------------
// CFepUiLayout::IsSemiTransparencyRequired
// Tell whether this layout require trancparency
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//       
TBool CFepUiLayout::IsSemiTransparencyRequired(const RPointerArray<CFepUiBaseCtrl>&
                                            aCtrlList)    
    {
    TBool ret = EFalse;
    for(TInt i = 0; i < aCtrlList.Count() && !ret; i++)
        {
        //is a control group?
        if(aCtrlList[i]->IsKindOfControl(ECtrlControlGroup))
            {
            //check each control
            CControlGroup* ctrlGroup = static_cast<CControlGroup*>(aCtrlList[i]);
            ret = IsSemiTransparencyRequired(ctrlGroup->ControlList());
            }
        else
            {                        
            if(aCtrlList[i]->IsKindOfControl(ECtrlTransparentHwrWnd))
                {                
                ret =  ETrue;
                }
            }
        }
    return ret;
    }
    
// ---------------------------------------------------------------------------
// CFepUiLayout::SemiTransparencyRequired
// Tell whether this layout require trancparency
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//       
EXPORT_C TBool CFepUiLayout::SemiTransparencyRequired()    
    {
    return IsSemiTransparencyRequired(ControlList());
    }
    
// ---------------------------------------------------------------------------
// CFepUiLayout::OnResourceChange
// Handle system resource change
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
EXPORT_C TInt CFepUiLayout::OnResourceChange(TInt aType)
    {
    TInt err = KErrNone;
    switch(aType)
        {
        case KEikDynamicLayoutVariantSwitch:
            {                
            TPixelsTwipsAndRotation ptSize;             
            CCoeEnv::Static()->ScreenDevice()->
                                    GetDefaultScreenSizeAndRotation(ptSize);
            iScreenSize = ptSize.iPixelSize;
            err = SizeChanged(&ptSize);
            }
            break;
        case KAknsMessageSkinChange:
            err = OnSkinChange();
        default:
            break;            
        }
    //let controls to handle resource change
    iRootCtrl->HandleResourceChange(aType);        
    return err;
    
    }

// ---------------------------------------------------------------------------
// CFepUiLayout::SizeChanged
// Handle screen size change
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
EXPORT_C TInt CFepUiLayout::SizeChanged(const TAny* /*aData*/)
    {
    //do nothing
    return KErrNone;
    }
    
// ---------------------------------------------------------------------------
// CFepUiLayout::OnSkinChange
// Handle skin change
// ---------------------------------------------------------------------------
//           
EXPORT_C TInt CFepUiLayout::OnSkinChange()
    {
    //ask root control to handle skin change
    TRAP_IGNORE(iRootCtrl->ConstructFromResourceL());
    return KErrNone;
    }



// ---------------------------------------------------------------------------
// CFepUiLayout::RegisterFeedbackArea
// Register the feedback area
// ---------------------------------------------------------------------------
//  
EXPORT_C void CFepUiLayout::RegisterFeedbackArea(TInt aId,const TRect& aRect)
    {
#ifdef RD_TACTILE_FEEDBACK
    TTactileFeedbackArea area= {aId,aRect,TRect(0,0,0,0)};
    TPtrC data(reinterpret_cast<TUint16*>(&area),sizeof(area)/2);
    SignalOwner(ESignalRegisterFeedbackArea,data);
#endif // RD_TACTILE_FEEDBACK
    }
	
// ---------------------------------------------------------------------------
// CFepUiLayout::RegisterFeedbackArea
// Register the feedback area
// Advanced Tactile feedback REQ417-47932
// ---------------------------------------------------------------------------
//  
EXPORT_C void CFepUiLayout::RegisterFeedbackArea(TInt aId,const TRect& aRect, TInt aTactileType)
    {
#ifdef RD_TACTILE_FEEDBACK
    TTactileFeedbackArea area= {aId,aRect,TRect(0,0,0,0),aTactileType};
    TPtrC data(reinterpret_cast<TUint16*>(&area),sizeof(area)/2);
    SignalOwner(ESignalRegisterFeedbackArea,data);
#endif // RD_TACTILE_FEEDBACK
    }
     
// ---------------------------------------------------------------------------
// CFepUiLayout::DeRegisterFeedbackArea
// De-Register the feedback area
// ---------------------------------------------------------------------------
//  
EXPORT_C void CFepUiLayout::DeRegisterFeedbackArea(TInt aId,const TRect& aRect)
    {
#ifdef RD_TACTILE_FEEDBACK    
    TTactileFeedbackArea area= {aId,aRect,TRect(0,0,0,0)};
    TPtrC data(reinterpret_cast<TUint16*>(&area),sizeof(area)/2);
    SignalOwner(ESignalDeRegisterFeedbackArea,data);
#endif // RD_TACTILE_FEEDBACK
    }
     

// ---------------------------------------------------------------------------
// CFepUiLayout::ChangeFeedbackArea
// De-Register the feedback area
// ---------------------------------------------------------------------------
//      
EXPORT_C void CFepUiLayout::ChangeFeedbackArea(TInt aId,const TRect& aNew, 
                                                            const TRect& aOld)
    {
#ifdef RD_TACTILE_FEEDBACK
    if(aNew == aOld)
        return;
    
	TInt InvalidType = 0;
    TTactileFeedbackArea area= {aId,aNew,aOld,InvalidType};
    TPtrC data(reinterpret_cast<TUint16*>(&area),sizeof(area)/2);
    SignalOwner(ESignalChangeFeedbackArea,data);    
#endif // RD_TACTILE_FEEDBACK    
    }   

// ---------------------------------------------------------------------------
// CFepUiLayout::ChangeFeedbackType
// De-Register the feedback area
// ---------------------------------------------------------------------------
//    

EXPORT_C void CFepUiLayout::ChangeFeedbackType(TInt aId, TInt aNewTactileType)
    {
#ifdef RD_TACTILE_FEEDBACK
    TTactileFeedbackArea area= {aId,TRect(0,0,0,0),TRect(0,0,0,0),aNewTactileType};
    TPtrC data(reinterpret_cast<TUint16*>(&area),sizeof(area)/2);
    SignalOwner(ESignalChangeFeedbackType,data);    
#endif // RD_TACTILE_FEEDBACK    
    }   

    
// ---------------------------------------------------------------------------
// CFepUiLayout::SupportTactileFeedback
// This function can be used to check, whether touch feedback is 
// supported at all in the device.
// ---------------------------------------------------------------------------
//      
EXPORT_C TBool CFepUiLayout::SupportTactileFeedback()
    {
    TBool tactileSupported;
#ifdef RD_TACTILE_FEEDBACK
    tactileSupported = iTactileSupported;
#endif // RD_TACTILE_FEEDBACK
	return tactileSupported;
    }
    
// ---------------------------------------------------------------------------
// CFepUiLayout::DoTactileFeedback
// Gives instant feedback.
// ---------------------------------------------------------------------------
//   
EXPORT_C void CFepUiLayout::DoTactileFeedback(TInt aType)
    {
#ifdef RD_TACTILE_FEEDBACK
    MTouchFeedback::Instance()->InstantFeedback((TTouchLogicalFeedback)aType);
#endif // RD_TACTILE_FEEDBACK    
    }

// ---------------------------------------------------------------------------
// CFepUiLayout::DoTactileFeedback
// Gives instant feedback.
// ---------------------------------------------------------------------------
// 
EXPORT_C void CFepUiLayout::DoTactileFeedback(TInt aType, TBool aVibraEnable, TBool aAudioEnable)
	{
#ifdef RD_TACTILE_FEEDBACK
	if (aAudioEnable && aVibraEnable)
		{
		MTouchFeedback::Instance()->InstantFeedback((TTouchLogicalFeedback)aType);
		}
	else
		{
		TBool vibraEnabled = MTouchFeedback::Instance()->FeedbackEnabledForThisApp( ETouchFeedbackVibra );
		TBool audioEnabled = MTouchFeedback::Instance()->FeedbackEnabledForThisApp( ETouchFeedbackAudio );
		
		MTouchFeedback::Instance()->SetFeedbackEnabledForThisApp(aVibraEnable, aAudioEnable);
		MTouchFeedback::Instance()->InstantFeedback((TTouchLogicalFeedback)aType);
		MTouchFeedback::Instance()->SetFeedbackEnabledForThisApp(vibraEnabled, audioEnabled);
		}
#endif // RD_TACTILE_FEEDBACK  
	}

// ---------------------------------------------------------------------------
// CFepUiLayout::GetEnabledPluginMode
// ---------------------------------------------------------------------------
//         
EXPORT_C TInt CFepUiLayout::GetEnabledPluginMode( )
    {

    TInt mode = 0;
    TPtrC data( reinterpret_cast<TUint16*>(&mode) ,sizeof( TInt )/sizeof( TUint16) );
        
    iLayoutOwner->SignalOwner( ESignalGetAllowedInputMode,data );
    
    return mode;
    }

// ---------------------------------------------------------------------------
// CFepUiLayout::HandleAppInfoChange
// ---------------------------------------------------------------------------
//         
EXPORT_C void CFepUiLayout::HandleAppInfoChange(const TDesC& /*aInfo*/, 
                                                   TPeninputAppInfo /*aType*/)
    {
    //do nothing
    }
    
// ---------------------------------------------------------------------------
// CFepUiLayout::SetPositionFromOutside
// ---------------------------------------------------------------------------
//         
EXPORT_C void CFepUiLayout::SetPositionFromOutside(const TPoint& /*aNewPos*/) 
    {
    //do nothing
    }
    
EXPORT_C CFepUiCursor* CFepUiLayout::CreateCursor()
    {
    return iRootCtrl->CreateCursor();
    }
//end of file
