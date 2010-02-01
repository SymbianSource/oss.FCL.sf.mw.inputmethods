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
* Description:  Implementation for peninput animation dll
*
*/


/*
* ============================================================================
*
* Refactoring by Steven Yao
* 
* 2007-03-23
*
* ============================================================================
*/

//INCLUDE
#include "peninputanim.h" 
#include "peninputcmdparam.h"
#include "peninputcmdparamext.h"
#include "peninputcmd.h"
// Modify for bug ETMA-7X2C5Y begin
#include "penpointereventsuppressor.h"
// Modify for bug ETMA-7X2C5Y end
// ----------- Touch feedback additions start
#ifdef RD_TACTILE_FEEDBACK
#include <coecntrl.h>
#include <tactilefeedbackserver.h>
#include <featmgr.h>
#endif //RD_TACTILE_FEEDBACK
// ----------- Touch feedback additions start

#include <avkondomainpskeys.h>
//CONSTANT
const TInt KMsgSlot1 = 1;	//msg slot 2 in IPC
const TInt KMsgSlot2 = 2; 	//msg slot 2 in IPC
const TInt KFlushTimerPeriod = 1000;		//1/1000 second
const TInt KResponseQueueWaitTime = 100000;		//1/10 second

// ======== GLOBAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CreateCAnimDllL
// This is called by the window server, at the request of the animation 
// client, to construct the CAnimDll derived server side DLL class.
// ---------------------------------------------------------------------------
//
EXPORT_C CAnimDll *CreateCAnimDllL()
    {
	return(new (ELeave) CPeninputAnimDll()); 
    }

// ======== MEMBER FUNCTIONS ========

// class CAnimDll

// ---------------------------------------------------------------------------
// CPeninputAnimDll::CreateInstanceL
// Factory function for animation DLL.
// ---------------------------------------------------------------------------
//
CAnim* CPeninputAnimDll::CreateInstanceL(TInt /*aType*/)
    {
	return new(ELeave) CPeninputAnim();
    }

// class CPeninputAnim

// ---------------------------------------------------------------------------
// CPeninputAnim::CPeninputAnim
// Constructor
// ---------------------------------------------------------------------------
//
CPeninputAnim::CPeninputAnim()
    {
	iIsActive = EFalse;
	//iSpritePosition and iSpriteSize are initialized to 0 by default constructor
	//iSpritePosition = TPoint(0, 0);
	//iSpriteSize = TSize(0, 0);
	iIsPointerCaptured = EFalse;
	iIsPenDown = EFalse;
	
	iPrimaryTouchPtNum = 0;
	iPrimaryTouchPtDetected = EFalse;

	
#ifdef RD_TACTILE_FEEDBACK	
	TRAPD( err, FeatureManager::InitializeLibL() );
    if( err == KErrNone )
        {
        iTactileSupported =  FeatureManager::FeatureSupported( KFeatureIdTactileFeedback );
        }
	iFeedback = MTactileFeedbackServer::Instance();
#endif // RD_TACTILE_FEEDBACK	
    
    iEnableSprite = ETrue;	
    iDSAState = EFalse;	
    }

// ---------------------------------------------------------------------------
// CPeninputAnim::~CPeninputAnim
// Destructor
// ---------------------------------------------------------------------------
//
CPeninputAnim::~CPeninputAnim()
    {
	Deactivate();

	iMsgBufQueue.Close();
	iKeyMsgResponseQueue.Close();
	iResponseQueueSemaphore.Close();
	
	delete iFlushTimer;
#ifdef RD_TACTILE_FEEDBACK	
	FeatureManager::UnInitializeLib();
	iTactileControl.Close();
	iBackupTactileControl.Close();
#endif // RD_TACTILE_FEEDBACK	
	// Modify for bug ETMA-7X2C5Y begin
	delete iPointerEventSuppressor;
	// Modify for bug ETMA-7X2C5Y end
	if (iDiscreetPopSubscriber)
	    {
	    iDiscreetPopSubscriber->StopSubscribe();
	    }
	iDiscreetPopProperty.Close();
	delete iDiscreetPopSubscriber;
	iAknUiSrv.Close();
    }

// ---------------------------------------------------------------------------
// CPeninputAnim::ConstructL
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPeninputAnim::ConstructL(TAny* /*aParameters*/)
    {
	// the animation feature is not used
	iFunctions->SetInterval(0);
	iFlushTimer = CPeriodic::NewL(CActive::EPriorityStandard);
	
    iFunctions->RegisterForNotifications(EDirectScreenAccess);
	// Modify for bug ETMA-7X2C5Y begin
    iPointerEventSuppressor = CPenPointerEventSuppressor::NewL();
    // Modify for bug ETMA-7X2C5Y end
    User::LeaveIfError(iDiscreetPopProperty.Attach(KPSUidAvkonDomain, 
                                         KAknGlobalDiscreetPopupNumChanged));
    iDiscreetPopSubscriber = new (ELeave) CSubscriber(
			TCallBack( DiscreetPopChangeNotification, this), 
			iDiscreetPopProperty);
    iDiscreetPopSubscriber->SubscribeL();
	
	User::LeaveIfError(iAknUiSrv.Connect());
	iDiscreetPopArea = iAknUiSrv.GetInUseGlobalDiscreetPopupRect();
    if(iDiscreetPopArea.Size().iWidth > 0)
    	{
		iDiscreetPoped = ETrue;
    	}
    }


// ---------------------------------------------------------------------------
// CPeninputAnim::Animate
// Create layout
// ---------------------------------------------------------------------------
//
void CPeninputAnim::Animate(TDateTime* /*aDateTime*/)
    {
    // do nothing
    }


void CPeninputAnim::HandleNotification(const TWsEvent& aEvent)
    {
    switch (aEvent.Type())
        {
        case EEventDirectScreenAccessBegin:
            SetDSAState(ETrue);
            break;
        case EEventDirectScreenAccessEnd:
            SetDSAState(EFalse);
            break;
        default:
            break;
        }
    }
    
void CPeninputAnim::SetDSAState(TBool aFlag)
    {
    iDSAState = aFlag;
    if(aFlag && iEnableSprite) //close sprite when DSA is on
        {
        TRawEvent event;
        event.Set( TRawEvent::ENone );
        event.SetTip( ETrue );
        PostRawEvent( event );
        FlushRawEventBuffer();
        //Deactivate(); 
        //iEnableSprite = EFalse;
        }
    /*else
        {
        iEnableSprite = iESPrevState;
        }*/
    }
// ---------------------------------------------------------------------------
// CPeninputAnim::OfferRawEvent
// handle raw events, e.g. key presses, pen events, power buttons etc. 
// if the event can be handled then return ETrue, or return EFalse
// ---------------------------------------------------------------------------
//
TBool CPeninputAnim::OfferRawEvent(const TRawEvent& aRawEvent)
    {
    if( iIsSimulatedEvent )
        {
    	return EFalse;
        }
    
    // Suppress multi-touch events
    switch ( aRawEvent.Type() )
        {
        case TRawEvent::EButton1Down:
        case TRawEvent::EButton1Up:
        case TRawEvent::EPointerMove:
            {
            if ( iPrimaryTouchPtDetected )
                {
                if ( aRawEvent.PointerNumber() != iPrimaryTouchPtNum )
                    {
                    return ETrue;
                    }
                }
            else
                {
                iPrimaryTouchPtNum = aRawEvent.PointerNumber();
                iPrimaryTouchPtDetected = ETrue;
                }
            }
            break;
        default:
            break;
        }

    
	// Suppress unexpected drag events, refer to error ETMA-7X2C5Y
    TPointerEvent pointerEvent;
    switch ( aRawEvent.Type() )
        {
        case TRawEvent::EButton1Down:
            {
            pointerEvent.iType = TPointerEvent::EButton1Down;
            pointerEvent.iPosition = aRawEvent.Pos();
            break;
            }
        case TRawEvent::EButton1Up:
            {
            pointerEvent.iType = TPointerEvent::EButton1Up;
            break;
            }
        case TRawEvent::EPointerMove:
            {
            pointerEvent.iType = TPointerEvent::EDrag;
            pointerEvent.iPosition = aRawEvent.Pos();
            break;
            }
        default:
            pointerEvent.iType = TPointerEvent::ENullType;
        }
    
    TBool result = iPointerEventSuppressor->SuppressPointerEvent( pointerEvent );
    if ( result )
        {
        return ETrue;
        }

    
	switch(aRawEvent.Type())
	    {
		case TRawEvent::EKeyUp:
		case TRawEvent::EKeyDown:
		    {
			return OnRawKeyEvent(aRawEvent);        	
		    }
		case TRawEvent::EButton1Down:
		    {
			return OnRawButton1Down(aRawEvent);
		    }
		case TRawEvent::EButton1Up:
		    {
			return OnRawButton1Up(aRawEvent);
		    }
		case TRawEvent::EPointerMove:
		    {
			return OnRawPointerMove(aRawEvent);
		    }
		default:
		    {
			return EFalse;
		    }            
	    }	
    }

// ---------------------------------------------------------------------------
// CPeninputAnim::Command
// The window server calls this function in response to application 
// calls to the client side command function RAnim::Command()
// ---------------------------------------------------------------------------
//
void CPeninputAnim::Command(TInt aOpcode, TAny* aParams)
    {
    switch(aOpcode)
        {
		case EPeninputOpActivate:
            {
            Activate();
            break;
            }        
        case EPeninputOpDeactivate:
            {
            Deactivate();
            break;
            }        
        case EPeninputOpLayoutUpdateArea:
            {
            PanicClientIfNoParam(aParams);	                
			TUpdateDataArea* area = static_cast<TUpdateDataArea*>(aParams);
			iSpriteFunctions->UpdateMember( 0, area->iRect, area->iUpdateFlag );
			break;
		    }			
        case EPeninputOpCapturePointer:
            {
            PanicClientIfNoParam(aParams);            
	        iIsPointerCaptured = *(static_cast<TBool*>(aParams));
	        break;
            }
        case EPeninputOpSimulateEvent:
            {
            PanicClientIfNoParam(aParams);
            TRawEvent event = *(static_cast<TRawEvent*>(aParams));
            iIsSimulatedEvent = ETrue;
            iFunctions->PostRawEvent(event);
            iIsSimulatedEvent = EFalse;

            break;
            }

#ifdef RD_TACTILE_FEEDBACK         
        case EPeninputOpRegisterFeedbackArea:
            {
            //add the rect to feedback list
            PanicClientIfNoParam(aParams);
            TTactileFeedbackArea area = *(static_cast<TTactileFeedbackArea*>(aParams));
            
            //Advanced Tactile feedback REQ417-47932
            TTactileControlInfo controlitem(area.iId, area.iNewRect, area.iTactileType);
            iTactileControl.Append(controlitem); 
            }
            break;
        case EPeninputOpDeRegisterFeedbackArea:
            {
            PanicClientIfNoParam(aParams);
            TTactileFeedbackArea area = *(static_cast<TTactileFeedbackArea*>(aParams));
            const TInt KRemoveAllId = -1;
            
            if( area.iId == KRemoveAllId && area.iNewRect == TRect(0,0,0,0) )
                {
                //remove all
                iTactileControl.Reset();
                break;
                }
            TTactileControlInfo controlitem(area.iId, area.iNewRect, area.iTactileType);
            TInt idx = iTactileControl.Find(controlitem, 
            		TIdentityRelation<TTactileControlInfo>(CPeninputAnim::MatchItemByControlID));
            if(idx != KErrNotFound)
                {  
                iTactileControl.Remove(idx);
                }
            }
            break;  
        case EPeninputOpChangeFeedbackArea:
            {
            PanicClientIfNoParam(aParams);
            TTactileFeedbackArea area = *(static_cast<TTactileFeedbackArea*>(aParams));
            TTactileControlInfo controlitem(area.iId, area.iPrevRect, area.iTactileType);           
            TInt idx = iTactileControl.Find(controlitem, 
                    TIdentityRelation<TTactileControlInfo>(CPeninputAnim::MatchItemByControlIDAndArea));
            if(idx != KErrNotFound)
                {                                
                //change the rect
                iTactileControl[idx].iArea = area.iNewRect;
                }  
            }
            break; 
        case EPeninputOpChangeFeedbackType:
        	{
        	PanicClientIfNoParam(aParams);
            TTactileFeedbackArea area = *(static_cast<TTactileFeedbackArea*>(aParams));
            TTactileControlInfo controlitem(area.iId, area.iNewRect, area.iTactileType);
            TInt idx = iTactileControl.Find(controlitem,                    
					TIdentityRelation<TTactileControlInfo>(CPeninputAnim::MatchItemByControlID));
            if(idx != KErrNotFound)
                {                                
                //change the tactile type
                iTactileControl[idx].iTactileType = area.iTactileType;
                }        	
        	}
            break;
#endif // RD_TACTILE_FEEDBACK
        case EPeninputOpEnalbeSprite:
            {
            PanicClientIfNoParam(aParams);
            iEnableSprite = *(static_cast<TBool*>(aParams));            
            }
            break;
          

        default:
            {
            //panic the client for unknown command
            iFunctions->Panic();
            break;
            }
	    }
    }


// ---------------------------------------------------------------------------
// CPeninputAnim::CommandReplyL
// The window server calls this function in response to application calls 
// to the client side command function RAnim::CommandReplyL(). 
// This function can't leave. Otherwise window server panic
// ---------------------------------------------------------------------------
//
TInt CPeninputAnim::CommandReplyL( TInt aOpcode, TAny* /*aParams*/)
    {
    TInt retValue = KErrNone;
    const RMessagePtr2* msg = iFunctions->Message();
    switch ( aOpcode )
        {
        case EPeninputOpFinishConstructionL:
            {
            iFunctions->SetSync( MAnimGeneralFunctions::ESyncNone );
            
            retValue = iMsgBufQueue.OpenGlobal(KMsgEventBufferQueue);
            //event queue for key event response from pen input servers
            iKeyMsgResponseQueue.OpenGlobal(KMsgEventResponseQueue);
            iResponseQueueSemaphore.OpenGlobal(KAnimKeyEventSemaphore);

			break;
            }

        case EPeninputOpActivate:
            {
            Activate();
            break;
            }
        
        case EPeninputOpDeactivate:
            {
            Deactivate();
            break;
            }       
            
        case EPeninputOpLayoutUpdateArea:
        	{
        	//TUpdateDataArea s;
			//TPckg<TUpdateDataArea> pkg(s);        	
			TRect rect;
			TBool flag;
		    TPckg<TRect> rectMsg(rect);			
		    TPckg<TBool> flagMsg(flag);    
			
			msg->ReadL(KMsgSlot1,rectMsg);
			msg->ReadL(KMsgSlot2,flagMsg);
			iSpriteFunctions->UpdateMember(0, rect,flag);			
        	  }
        	  break;
        case EPeninputOpLayoutUpdateAreaRegion:
            {
            TInt num;
            TPckg<TInt> numMsg(num);    
            msg->ReadL(KMsgSlot1,numMsg);
            TInt len = msg->GetDesLengthL(KMsgSlot2);
            TUint8* buf = new (ELeave) TUint8[len*sizeof(TUpdateArea)];
            CleanupArrayDeletePushL( buf );
            
            TPtr8 ptr(buf,len);
            msg->ReadL(KMsgSlot2,ptr);

            TUpdateArea* area = reinterpret_cast<TUpdateArea*>(buf);

            for(TInt i = 0; i < num; ++i)
                {
                iSpriteFunctions->UpdateMember(0, area[i].iRect,area[i].iFullUpdateFlag);
                }

            CleanupStack::PopAndDestroy( buf );
        	}
        	break;
        case EPeninputOpSetLayoutPos:
            {
            TPoint pt;
            TPckg<TPoint> msgData(pt);
            msg->ReadL(KMsgSlot1,msgData);
            SetPosition(pt);            
            }
            break;
            
        case EPeninputOpCapturePointer:
            {
            TInt captureCtrlID;
			TPckg<TBool> msgData(iIsPointerCaptured);
			TPckg<TInt> msgCaptureCtrlIDData(captureCtrlID);
            
            msg->ReadL(KMsgSlot1,msgData);
            msg->ReadL(KMsgSlot2,msgCaptureCtrlIDData);
#ifdef RD_TACTILE_FEEDBACK
			// When pointer is captured by some ctrl,
			// only the captured ctrl has tactile feedback ablity,
			// disable all other one's.
            if (iIsPointerCaptured)
            	{
            	// Back up all the tactile area
            	iBackupTactileControl.Reset();        	
		        TInt count = iTactileControl.Count();
		        for(TInt i = 0; i < count; ++i)
		            {
		            iBackupTactileControl.Append(iTactileControl[i]);
		            }
		        
		        // Clean up the original tactile area
		        iTactileControl.Reset();		       
		        
		        // Find the captured ctrl, restore its tactile ablity
		        for(TInt i = 0; i < count; ++i)
		            {
		            if (iBackupTactileControl[i].iId == captureCtrlID)
		            	{
		            	iTactileControl.Append(iBackupTactileControl[i]);
		            	}
		            }

            	}
            // When pointer released captured, 
            // restore all original tactile area.
            else 
            	{
            	if (iBackupTactileControl.Count() != 0)
            		{
            		iTactileControl.Reset();			       
	            	
			        TInt count = iBackupTactileControl.Count();
			        for(TInt i = 0; i < count; ++i)
			            {
			            iTactileControl.Append(iBackupTactileControl[i]);
			            }
			        iBackupTactileControl.Reset();
            		}
            	}
#endif            	
            }
            break;
        case EPeninputOpLayoutSizeChangedWithSize:
                {
                TPckg<TSize> msgData(iSpriteSize);
                msg->ReadL(1,msgData); 
                iSpriteFunctions->SizeChangedL();
                }
            break;
        case EPeninputOpSimulateEvent:
            {
            TRawEvent event;
            TPckg<TRawEvent> msgData(event);
            msg->ReadL(KMsgSlot1,msgData); 
            iIsSimulatedEvent = ETrue;
            iFunctions->PostRawEvent(event);
            iIsSimulatedEvent = EFalse;
            
            }
            break;    
            
#ifdef RD_TACTILE_FEEDBACK             
        case EPeninputOpRegisterFeedbackArea:
            {						
            GetFeedbackAreaDataL(iTactileControl,msg);            
            }
            break;
            
        case EPeninputOpDeRegisterFeedbackArea:
            {
            //Advanced Tactile feedback REQ417-47932
            RArray<TTactileControlInfo> controlarray;
            CleanupClosePushL(controlarray);
            
            GetFeedbackAreaDataL(controlarray,msg);
            
            //remove all registered feedback area
            const TInt KRemoveAllId = -1;           
            if(controlarray.Count() && controlarray[0].iId == KRemoveAllId && 
										controlarray[0].iArea == TRect(0,0,0,0))
                {
                //remove all
                iTactileControl.Reset();
                }
            else
                {
                //remove the area
                for(TInt i = 0; i < controlarray.Count(); ++i)
                    {
                    //find id first
                    TInt idx = iTactileControl.Find(controlarray[i], 
								TIdentityRelation<TTactileControlInfo>(CPeninputAnim::MatchItemByControlID));
                    
                    if(idx != KErrNotFound)
                        {
                        iTactileControl.Remove(idx);                       
                        }
                    }
                }
                
            CleanupStack::PopAndDestroy( 1, &controlarray );
            }
            break;
        
            
        case EPeninputOpChangeFeedbackArea:            
            {
            //Advanced Tactile feedback REQ417-47932
            RArray<TTactileControlInfo> controlarray;
            RArray<TRect> prevArea;
            
            CleanupClosePushL(controlarray);
            CleanupClosePushL(prevArea);
            
            GetFeedbackAreaDataL(controlarray,msg,&prevArea);
            //change the area one by one            
            for(TInt i = 0; i < controlarray.Count(); ++i)
                {
                //find id first
                TInt idx = iTactileControl.Find(controlarray[i], 
                		   TIdentityRelation<TTactileControlInfo>(CPeninputAnim::MatchItemByControlID));
                                    
                if(idx != KErrNotFound)
                    {
                    //check the rect
                    if(prevArea[i] == iTactileControl[idx].iArea)
                        {
                        //change the rect
                        iTactileControl[idx].iArea = controlarray[i].iArea;
                        break;
                        }
                    }
                }
            
            CleanupStack::PopAndDestroy( 2, &controlarray );
            }
        
            break;
            
        //Advanced Tactile feedback REQ417-47932    
        case EPeninputOpChangeFeedbackType:            
            {           
            RArray<TTactileControlInfo> controlarray;
            CleanupClosePushL(controlarray);
            
            GetFeedbackAreaDataL(controlarray,msg);
            //change the area one by one
            for(TInt i = 0; i < controlarray.Count(); ++i)
                {
                //find id first
                TInt idx = iTactileControl.Find(controlarray[i],                 		   
						   TIdentityRelation<TTactileControlInfo>(CPeninputAnim::MatchItemByControlID));
                if(idx != KErrNotFound)
                    {
                    //change the tactile feedback type
					iTactileControl[idx].iTactileType = controlarray[i].iTactileType;
                    break;
                    }
                }
            
            CleanupStack::PopAndDestroy( 1, &controlarray );
            }
  
            break;
            
#endif // RD_TACTILE_FEEDBACK 
        case EPeninputOpRequestDSAState:                        
            {
		    TPckgC<TBool> flagMsg(iDSAState);    
			
			msg->WriteL(KMsgSlot1,flagMsg);
            
            }
            break;
        default:
            // unsupported opcode, panic the client
            {                
            iFunctions->Panic();
            retValue = KErrNotSupported;
            }
            break;
        }

    return retValue;
    }

// ---------------------------------------------------------------------------
// CPeninputAnim::Activate
// Makes PENINPUT window visible
// ---------------------------------------------------------------------------
//
void CPeninputAnim::Activate()
    {
	if(!iIsActive)
	    {
		iSpriteFunctions->SetPosition(iSpritePosition);
		if(iEnableSprite)
		    iSpriteFunctions->Activate(ETrue);
		iFunctions->GetRawEvents(ETrue);
		iIsActive = ETrue;		

		StartTimer();
	    }        
    }

// ---------------------------------------------------------------------------
// CPeninputAnim::Deactivate
// Makes PENINPUT window invisible
// ---------------------------------------------------------------------------
//
void CPeninputAnim::Deactivate()
    {
	if(iIsActive)
	    {
	    if(iEnableSprite)
		    iSpriteFunctions->Activate(EFalse);
		iFunctions->GetRawEvents(EFalse);        
		iIsActive = EFalse;
		
		StopTimer();
	    }        
    }
    
#ifdef RD_TACTILE_FEEDBACK
// ---------------------------------------------------------------------------
// CPeninputAnim::DoTactileFeedBack
// Give tactile feedback
// ---------------------------------------------------------------------------
//						
void CPeninputAnim::DoTactileFeedBack(const TPoint& aPos)
    {
    if ( iFeedback ) 
        {
        TInt count = iTactileControl.Count();
        for(TInt i = 0; i < count; ++i)
            {
            if(iTactileControl[i].iArea.Contains(aPos))
                {
                iFeedback->InstantFeedback((TTouchLogicalFeedback)iTactileControl[i].iTactileType);
                return;
                }                 
        	}           
        }
    }
#endif // RD_TACTILE_FEEDBACK

// ---------------------------------------------------------------------------
// CPeninputAnim::OnRawButton1Down
// Handles raw pen down events
// Can't leave, since this is run in window server side.
// return value indicating whether the event was handled by this function    
// ---------------------------------------------------------------------------
//

TBool CPeninputAnim::OnRawButton1Down(const TRawEvent& aRawEvent)
    {
	if(iIsSimulatedEvent)
	    {
		return EFalse;
	    }
		
    if(iDirty )
        {
        Refresh();
        }
    
	if(iDiscreetPoped && iDiscreetPopArea.Contains(aRawEvent.Pos()))
		{
		iDiscreetPopedCapture = ETrue;
		return EFalse;
		}
	TRect rect(iSpritePosition, iSpriteSize);        
	if(rect.Contains(aRawEvent.Pos()))
	    {                
#ifdef RD_TACTILE_FEEDBACK		    
	    //give tactile feedback	    
	    if(iTactileSupported)
            DoTactileFeedBack(aRawEvent.Pos() - rect.iTl);
#endif // RD_TACTILE_FEEDBACK	
		iIsPenDown = ETrue;
		PostRawEvent(aRawEvent); 
		return ETrue;
	    }

	if(iIsPointerCaptured)
	    {                    
		PostRawEvent(aRawEvent);
		return ETrue;
	    }

	//pointer outside of input area,inform layout
	SendRawEvent(aRawEvent);

	return EFalse;
    }

TBool CPeninputAnim::OnRawButton1Up(const TRawEvent& aRawEvent)
    {
	if(iIsSimulatedEvent)
	    {
		return EFalse;
	    }

	//For discreetPop
	if(iDiscreetPopedCapture )
		{
		iDiscreetPopedCapture = EFalse;
		return EFalse;
		}
	if(iIsPenDown)
	    {      
		iIsPenDown = EFalse;
		
		TRect rect(iSpritePosition, iSpriteSize);        
		if(rect.Contains(aRawEvent.Pos()))
		    {
			//send pen up event immediately
			SendRawEvent(aRawEvent);
			return ETrue;                    
		    }
	    }
	
	if(iIsPointerCaptured)
	    {                    
		PostRawEvent(aRawEvent);
		return ETrue;
	    }
    
	//pointer outside of input area,inform layout
	SendRawEvent(aRawEvent);

	return EFalse;	
    }

TBool CPeninputAnim::OnRawPointerMove(const TRawEvent& aRawEvent)
    {
	if(iIsSimulatedEvent)
	    {
		return EFalse;
	    }
	if(iDiscreetPopedCapture)
	    {
	    return EFalse;
	    }
#ifdef RD_TACTILE_FEEDBACK		    
	TRect rect(iSpritePosition, iSpriteSize);        
	if( iIsPenDown && iIsPointerCaptured)
	    {                
	    //give tactile feedback	    

		PostRawEvent(aRawEvent); 
		return ETrue;
	    }
#endif // RD_TACTILE_FEEDBACK	

	if(iIsPointerCaptured || iIsPenDown)
	    {                    
		PostRawEvent(aRawEvent);
		return ETrue;
	    }
	
	return EFalse;
    }

// ---------------------------------------------------------------------------
// CPeninputAnim::SetPosition
// called when sprite has been moved
// ---------------------------------------------------------------------------
//

TPoint CPeninputAnim::SetPosition(const TPoint& aNewPos)
    {
	if(aNewPos != iSpritePosition)
	    {
		iSpritePosition = aNewPos;
		iSpriteFunctions->SetPosition(iSpritePosition);	
	    }
	return iSpritePosition;
    }

// ---------------------------------------------------------------------------
// CPeninputAnim::UpdateArea
// Update sprite rect
// ---------------------------------------------------------------------------
//
void CPeninputAnim::UpdateArea(const TRect& aRect, TBool aFullUpdate)
    {
	if(iIsActive)
        {
	    // if function is called without specifying the parameter aRect
	    // (using default param value) the whole area drawn to is updated
	    iSpriteFunctions->UpdateMember(0, aRect, aFullUpdate);
        }
    }

// ---------------------------------------------------------------------------
// CPeninputAnim::TimerCallBack
// Callback function of CPeriodic
// ---------------------------------------------------------------------------
//
TInt CPeninputAnim::FlushTimerCallBack(TAny* aPtr)
    {
    CPeninputAnim* self = static_cast<CPeninputAnim*>(aPtr);            
    self->FlushRawEventBuffer();            
    return KErrNone;
    }
  
// ---------------------------------------------------------------------------
// CPeninputAnim::SendRawEvent
// Send event to pen input server by buffering it
// ---------------------------------------------------------------------------
//    
void CPeninputAnim::SendRawEvent(const TRawEvent& aRawEvent)
    {
    //save to buffer first
    iEventBuffer.iEvent[iEventBuffer.iNum] = aRawEvent;
    iEventBuffer.iNum++;

    FlushRawEventBuffer();
    }

// ---------------------------------------------------------------------------
// CPeninputAnim::PostRawEvent
// Save the event to the buffer
// ---------------------------------------------------------------------------
//        
void CPeninputAnim::PostRawEvent(const TRawEvent& aRawEvent)
    {
    if(iEventBuffer.iNum >= KMaxEvent)
        {
        FlushRawEventBuffer();
	    }
	
    iEventBuffer.iEvent[iEventBuffer.iNum] = aRawEvent;
    iEventBuffer.iNum++;
    }

// ---------------------------------------------------------------------------
// CPeninputAnim::FlushRowEventBuffer
// Send pointer event to pen input serrver, and return error code
// ---------------------------------------------------------------------------
//
TInt CPeninputAnim::FlushRawEventBuffer()
    {
	TInt ret = KErrNone;
	
    if(iEventBuffer.iNum > 0)
	    {
        ret = iMsgBufQueue.Send(iEventBuffer);
       	iEventBuffer.iNum = 0;        	
        }
    
    return ret;
    }

// ---------------------------------------------------------------------------
// CPeninputAnim::OnRawKeyEvent
// Send raw key event to pen input server for processing.
// Wait until it's processed or timer out
// ---------------------------------------------------------------------------
//    
TBool CPeninputAnim::OnRawKeyEvent(const TRawEvent& aRawEvent)
    {
	TInt ret = KErrNone;
	
	//first sends all buffered event out
    FlushRawEventBuffer();
    
    //clear the signal and the response msg queue
    TBool isHandled = EFalse;
    ret = iKeyMsgResponseQueue.Receive(isHandled);
    while(ret == KErrNone)
	    {
        iResponseQueueSemaphore.Wait();
        ret = iKeyMsgResponseQueue.Receive(isHandled);
        }

	//send the event directly        
    iEventBuffer.iNum = 1;
    iEventBuffer.iEvent[0] = aRawEvent;
    ret = FlushRawEventBuffer();
    
    isHandled = EFalse;
    if(ret == KErrNone) 
        {
	    //wait for the key event processed.    
	    ret = iResponseQueueSemaphore.Wait(KResponseQueueWaitTime);
	    if(ret == KErrNone)
	        {        
	        //read the response from msg queue
	        iKeyMsgResponseQueue.Receive(isHandled);
	        }
        }

    return isHandled;
    }

// ---------------------------------------------------------------------------
// CPeninputAnim::StartTimer
// Initial and start timer
// ---------------------------------------------------------------------------
// 
void CPeninputAnim::StartTimer()
    {
	iFlushTimer->Start(
		KFlushTimerPeriod, 
		KFlushTimerPeriod, 
		TCallBack(FlushTimerCallBack, this));	
    }
	
// ---------------------------------------------------------------------------
// CPeninputAnim::StopTimer
// Cancel timer
// ---------------------------------------------------------------------------
// 
void CPeninputAnim::StopTimer()
    {
	iFlushTimer->Cancel();	
    }

#ifdef RD_TACTILE_FEEDBACK
void CPeninputAnim::GetFeedbackAreaDataL(RArray<TTactileControlInfo>& aTactileControlArray,
										 const RMessagePtr2* msg, 
                                         RArray<TRect>* aPrevRectArray)
    {
    TInt num;
    TPckg<TInt> numBuf(num);
    msg->ReadL(KMsgSlot1,numBuf);
    TTactileFeedbackArea* area = new(ELeave) TTactileFeedbackArea[num];

    CleanupArrayDeletePushL( area );

    TPtr msgData(reinterpret_cast<TUint16*>(area), num*sizeof(TTactileFeedbackArea)/2);
    msg->ReadL(KMsgSlot2,msgData);           
            
    for(TInt i = 0 ; i < num; ++i)
        {
        //Advanced Tactile feedback REQ417-47932
        aTactileControlArray.Append(TTactileControlInfo(area[i].iId,
									area[i].iNewRect,area[i].iTactileType));      
        //aRectArray.Append(area[i].iNewRect);                
        if(aPrevRectArray)
            aPrevRectArray->Append(area[i].iPrevRect);                    
        }

    CleanupStack::PopAndDestroy( area );            
    }

// ---------------------------------------------------------------------------
// CPeninputAnim::MatchItemByControlID
// Compare the Item by ID
// ---------------------------------------------------------------------------
// 
TBool CPeninputAnim::MatchItemByControlID (const TTactileControlInfo& aFirst, const TTactileControlInfo& aSecond)
	{
	if (aFirst.iId == aSecond.iId)
		return ETrue;
	return EFalse;
	}

// ---------------------------------------------------------------------------
// CPeninputAnim::MatchItemByControlIDAndArea
// Compare the Item by ID and Area
// ---------------------------------------------------------------------------
// 
TBool CPeninputAnim::MatchItemByControlIDAndArea (const TTactileControlInfo& aFirst, const TTactileControlInfo& aSecond)
	{
	if ((aFirst.iId == aSecond.iId) && (aFirst.iArea == aSecond.iArea))
		return ETrue;
	return EFalse;
	}
#endif // RD_TACTILE_FEEDBACK    
TInt CPeninputAnim::DiscreetPopChangeNotification(TAny* aObj)
    {
    if (aObj)
        {
        static_cast<CPeninputAnim*>(aObj)->HandleDiscreetPopNotification();
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }

void CPeninputAnim::Refresh()
    {
    iDiscreetPopArea = iAknUiSrv.GetInUseGlobalDiscreetPopupRect();
    if(iDiscreetPopArea.Size().iWidth > 0)
        {
        iDiscreetPoped = ETrue;
        }    
    else
        {
        iDiscreetPoped = EFalse;
        iDiscreetPopedCapture = EFalse;
        }
    iDirty = EFalse;
    }

void CPeninputAnim::HandleDiscreetPopNotification()
    {
    iDirty = ETrue;
    }
    
CSubscriber::CSubscriber(TCallBack aCallBack, RProperty& aProperty)
    :
    CActive(EPriorityNormal), iCallBack(aCallBack), iProperty(aProperty)
    {
    CActiveScheduler::Add(this);
    }

CSubscriber::~CSubscriber()
    {
    Cancel();
    }

void CSubscriber::SubscribeL()
    {
    if (!IsActive())
        {
        iProperty.Subscribe(iStatus);
        SetActive();
        }
    }

void CSubscriber::StopSubscribe()
    {
    Cancel();
    }

void CSubscriber::RunL()
    {
    if (iStatus.Int() == KErrNone)
        {
        iCallBack.CallBack();
        SubscribeL();
        }
    }

void CSubscriber::DoCancel()
    {
    iProperty.Cancel();
    }
// End of File
