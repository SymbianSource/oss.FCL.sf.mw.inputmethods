/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for full screen UI layout
*
*/


#include <w32std.h>
#include <peninputcmd.h>
#include "peninputfullscrlayout.h"
#include "peninputlayoutrootctrl.h"
#include "peninputlayouttimer.h"
#include "peninputlayouthwrwnd.h"
#include <coemain.h>

const TInt KDefaultMaxStep = 3;
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CFepFullScreenLayout::CFepFullScreenLayout
// Constructor
// ---------------------------------------------------------------------------
//

EXPORT_C CFepFullScreenLayout::CFepFullScreenLayout(MLayoutOwner* aLayoutOwner,
                                TInt aDuration)
                               :CFepUiLayout(aLayoutOwner),
                               iBufferEvent(EFalse),iXLeftPos(0),iXRightPos(0),
                               iYTopPos(0),iYBottomPos(0),
                               iEventBufDruation(aDuration)

    {
    const TInt KDefaultBufferTimer = 1000*250; //1/4 seconds            
    if(iEventBufDruation <= 0)
        iEventBufDruation = KDefaultBufferTimer;
    }

// ---------------------------------------------------------------------------
// CFepFullScreenLayout::~CFepFullScreenLayout
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CFepFullScreenLayout::~CFepFullScreenLayout()
    {
    delete iBufferTimer;    
    } 

// ---------------------------------------------------------------------------
// CFepFullScreenLayout::BaseConstructL
// Do basic contructor
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepFullScreenLayout::BaseConstructL()
    {
    iBufferTimer = CLayoutTimer::NewL(this,CLayoutTimer::EOthers);
   
    CFepUiLayout::BaseConstructL();
    }

// ---------------------------------------------------------------------------
// CFepUiLayout::HandleEventL
// handle event from window server
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CFepFullScreenLayout::HandleEventL(TEventType aType, const TAny* aData)
    {   
    TRawEvent event=*((TRawEvent*)aData);    
    
    //if pointer is captured, don't buffer the event.
    if(RootControl()->CtrlCapPointer())
        return CFepUiLayout::HandleEventL(aType,aData);        
    
    if((ERawEvent == aType) && (TRawEvent::EButton1Down == event.Type() ||
                                TRawEvent::EButton1Up == event.Type() || 
                                TRawEvent::EPointerMove == event.Type()) )
        {
        if(iBufferEvent)
            {
            if(TRawEvent::EButton1Up == event.Type())        
                {
                //is the buffer timer still active? if so, cancel it and
                //simulate the event to others
                if(iBufferTimer->IsActive())
                    {
                    iEventBuf.Append(event);
                    SimulateEvent();
                    return ETrue;
                    }
                else
                    {
                    //is this a valid trace?
                    iEventBuf.Append(event);
                    if(IsValidStroke())
                        HandleBufferedEventL();
                    else
                        SimulateEvent();
                    return ETrue;
                    }
                }

            BufferEventL(event);
            return ETrue;
            }
        
        
        //Check whether buffering should starts.
        //Only when pen downs in the area where trace can be started,
        //e.g, not in the button, should we start the buffering.
        if(!(RootControl()->NonHwrStartingPtRegion().Contains(event.Pos())))
            {            
            if(IsHwrNewCharacter() && TRawEvent::EButton1Down == event.Type())
                {                
                StartBufferEvent(event);
                return ETrue;
                }
            }
        }
    return CFepUiLayout::HandleEventL(aType,aData);    
    }
    
// ---------------------------------------------------------------------------
// CFepFullScreenLayout::StartBufferEvent
// Start bufferring the event
// ---------------------------------------------------------------------------
//    
void CFepFullScreenLayout::StartBufferEvent(TRawEvent& aEvent)
    {
    iBufferEvent = ETrue;
    iXLeftPos = aEvent.Pos().iX;
    iXRightPos = aEvent.Pos().iX;
    iYTopPos = aEvent.Pos().iY;
    iYBottomPos = aEvent.Pos().iY;
    iEventBuf.Append(aEvent);
    iHwrWndPenSize = iHwrWnd->PenSize();
    iHwrWnd->SetPenSize(TSize(0,0));
    iBufferTimer->SetTimer(iEventBufDruation); 
    }

// ---------------------------------------------------------------------------
// CFepFullScreenLayout::BufferEventL
// Bufferring theevent
// ---------------------------------------------------------------------------
//
void CFepFullScreenLayout::BufferEventL(TRawEvent& aEvent)
    {
    iEventBuf.AppendL(aEvent);
    if(IsValidStroke())
        HandleBufferedEventL();
    }

// ---------------------------------------------------------------------------
// CFepFullScreenLayout::SimulateEvent
// Simulates the bufferred event to other app
// ---------------------------------------------------------------------------
//
void CFepFullScreenLayout::SimulateEvent()
    {

    iBufferEvent = EFalse;
    //resotre the hwr window pen size
    iHwrWnd->SetPenSize(iHwrWndPenSize);    

    for(TInt i = 0; i < iEventBuf.Count(); i++)
        { 
    	TPtrC data;
    	data.Set(reinterpret_cast<TUint16*>(&iEventBuf[i]),sizeof(iEventBuf[i])/sizeof(TUint16));
        
        SignalOwner(ESignalSimulateEvent,data);
        } 
    iEventBuf.Reset();            
    }

// ---------------------------------------------------------------------------
// CFepFullScreenLayout::HandleBufferedEventL
// Forward event to iput UI
// ---------------------------------------------------------------------------
//    
void CFepFullScreenLayout::HandleBufferedEventL()
    {
    iBufferEvent = EFalse;
    //resotre the hwr window pen size
    iHwrWnd->SetPenSize(iHwrWndPenSize);    

    for(TInt i = 0; i < iEventBuf.Count(); i++)
        {
        CFepUiLayout::HandleEventL(ERawEvent,&iEventBuf[i]);
        }
        
    iEventBuf.Reset();
    }
    
EXPORT_C void CFepFullScreenLayout::HandleTimerOut(TInt /*aTimeType*/)
    {
    iBufferEvent = EFalse;
    //change pen width for hwr window
    TRAP_IGNORE(HandleBufferedEventL());
    }

// ---------------------------------------------------------------------------
// CFepFullScreenLayout::InitL
// Initialize the layout
// ---------------------------------------------------------------------------
//
EXPORT_C TRect CFepFullScreenLayout::Init()
    {
    //find hwr window
    iHwrWnd =(CHwrWndBase*)(RootControl()->ControlForKindOfType(ECtrlTransparentHwrWnd));
    __ASSERT_ALWAYS(iHwrWnd, User::Panic(_L("peninputserver"), EUiFullscreenWithoutHWRWnd));
    return CFepUiLayout::Init();
    }

// ---------------------------------------------------------------------------
// CFepFullScreenLayout::IsHwrNewCharacter
// Test whether a new character is starting
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CFepFullScreenLayout::IsHwrNewCharacter()
    {
    return iHwrWnd->StrokeList().Count() <= 0;
    }

// ---------------------------------------------------------------------------
// CFepFullScreenLayout::SemiTransparencyRequired
// Tell whether this layout require trancparency
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//       
EXPORT_C TBool CFepFullScreenLayout::SemiTransparencyRequired()    
    {
    //full screen HWR using full-transparency, so black-white mask is OK.
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CFepFullScreenLayout::IsValidStroke
// Tests whether current pen trace is valid for a character
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CFepFullScreenLayout::IsValidStroke()
    {
    if(iEventBuf.Count() >= KMaxBufferEventNum)
        {
        return ETrue; //buffer overflow, this is considering to be a valid stroke.
        }
    
    TPoint pt = iEventBuf[iEventBuf.Count()-1].Pos();
    if(pt.iX < iXLeftPos)
        {
        iXLeftPos = pt.iX;
        }
    else
        {
        if(pt.iX > iXRightPos)
            {
            iXRightPos = pt.iX;
            }
        }
        
    if(pt.iY < iYTopPos)
        {
        iYTopPos = pt.iY;
        }
    else
        {
        if(pt.iY > iYBottomPos)
            {
            iYBottomPos = pt.iY;
            }
        }    
    if((iXRightPos - iXLeftPos) >= KDefaultMaxStep || 
       (iYBottomPos - iYBottomPos) >= KDefaultMaxStep)
        {        
        //HandleBufferedEventL();
        return ETrue;
        }
    else
        return EFalse;    
    }
//end of file
