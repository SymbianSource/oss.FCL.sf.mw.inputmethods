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
* Description:  Implementation for class RPeninputAnim
*
*/



#include <w32std.h>

#include "rpeninputanim.h"
#include "peninputcmd.h"
#include "peninputcmdparam.h"
#include "peninputcmdparamext.h"
#include "peninputclientserver.h"


// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// RPeninputAnim::RPeninputAnim
// Default consturctor to allow RPeninputAnim to be defined
// as non pointer member. To properly initialize this class
// use the other overload.
// ---------------------------------------------------------------------------
//
RPeninputAnim::RPeninputAnim() : RAnim() 
    {
    }

// ---------------------------------------------------------------------------
// RPeninputAnim::RAnim
// Constructor
// ---------------------------------------------------------------------------
//
RPeninputAnim::RPeninputAnim(RAnimDll& aDll) : RAnim( aDll )
    {
    }

// ---------------------------------------------------------------------------
// RPeninputAnim::Construct
// Second phase constructor.
// ---------------------------------------------------------------------------
//
void RPeninputAnim::ConstructL( const RWsSprite& aDevice )
    {
    User::LeaveIfError( RAnim::Construct( aDevice, 0, KNullDesC8() ) );
    FinishConstructionL();
    }

// ---------------------------------------------------------------------------
// RPeninputAnim::Activate
// send activate command
// ---------------------------------------------------------------------------
//
void RPeninputAnim::Activate()
    {
    CommandReply( EPeninputOpActivate );
    }

// ---------------------------------------------------------------------------
// RPeninputAnim::Deactivate
// Send deactive command
// ---------------------------------------------------------------------------
//
void RPeninputAnim::Deactivate()
    {
    CommandReply( EPeninputOpDeactivate );
    }

// ---------------------------------------------------------------------------
// RPeninputAnim::FinishConstructionL
// Send finishing construction command
// ---------------------------------------------------------------------------
//
void RPeninputAnim::FinishConstructionL()
    {    
    User::LeaveIfError( CommandReply( EPeninputOpFinishConstructionL) );
    }

    
// ---------------------------------------------------------------------------
// RPeniputAnim::LayoutSizeChangedL
// Send size change message
// ---------------------------------------------------------------------------
//
TInt RPeninputAnim::LayoutSizeChangedL(const TSize& aSize)
    {
    TInt err;
    TIpcArgs args (TIpcArgs::ENothing,0);        
    TPckgC<TSize> msg(aSize);
    
    args.Set(KMsgSlot1,&msg);
    err = CommandReply(EPeninputOpLayoutSizeChangedWithSize,KNullDesC8,args);
    return err;
    }

// ---------------------------------------------------------------------------
// RPeniputAnim::SetUiLayoutPos
// Get ui layout position
// ---------------------------------------------------------------------------
//
TInt RPeninputAnim::SetUiLayoutPos(const TPoint& aPt)
    {    
    TPckgC<TPoint> msg(aPt);
    TIpcArgs args(TIpcArgs::ENothing);     
    args.Set(KMsgSlot1,&msg);    
    return CommandReply(EPeninputOpSetLayoutPos,KNullDesC8,args);
    }   

// ---------------------------------------------------------------------------
// RPeniputAnim::UpdateArea
// Update screen
// ---------------------------------------------------------------------------
//    
void RPeninputAnim::UpdateArea(const TRect& aRect,TBool aFullUpdate)    
    {
    TPckgC<TBool> flagMsg(aFullUpdate);    
    TPckgC<TRect> rectMsg(aRect);
    
    TIpcArgs args (TIpcArgs::ENothing);        
    /*TUpdateDataArea s;
    s.iRect = aRect;
    s.iUpdateFlag = aFullUpdate;
    
    TPckgC<TUpdateDataArea> pkg(s);
    */
    args.Set(KMsgSlot1,&rectMsg);
    args.Set(KMsgSlot2,&flagMsg);
    
    //args.Set(KMsgSlot1,&pkg);    
    CommandReply(EPeninputOpLayoutUpdateArea,KNullDesC8,args);
    //Command(EPeninputOpLayoutUpdateArea,pkg);    
    }

// ---------------------------------------------------------------------------
// RPeniputAnim::UpdateArea
// Update screen
// ---------------------------------------------------------------------------
//    
void RPeninputAnim::UpdateArea(const RArray<TUpdateArea>& aArea)
    {
    TInt num = aArea.Count();    
    TUpdateArea* area = new TUpdateArea[num];
    
    if (!area)
        return;

    for(TInt i = 0; i < num; ++i)
        {
        area[i] = aArea[i];
        }
    
    TPtrC8 areaMsg(reinterpret_cast<const TUint8*>(area), num*sizeof(TUpdateArea));
    
    TIpcArgs args (TIpcArgs::ENothing);        
    
    TPckgC<TInt> numMsg(num);    
    args.Set(KMsgSlot1,&numMsg);
    args.Set(KMsgSlot2,&areaMsg);
    
    //args.Set(KMsgSlot1,&pkg);    
    CommandReply(EPeninputOpLayoutUpdateAreaRegion, KNullDesC8, args);
    
    delete []area;
    }
    
// ---------------------------------------------------------------------------
// RPeniputAnim::CapturePointer
// Inform pen input animation dll to capture the pointer
// ---------------------------------------------------------------------------
//      
void RPeninputAnim::CapturePointer(TBool aFlag, TInt aCaptureCtrlID)
    {
    TIpcArgs args (TIpcArgs::ENothing);        
    TPckgC<TBool> msgCaptureFlagData(aFlag);
    TPckgC<TInt> msgCaptureCtrlIDData(aCaptureCtrlID);
    args.Set(KMsgSlot1,&msgCaptureFlagData);
    args.Set(KMsgSlot2,&msgCaptureCtrlIDData);
    //Command(EPeninputOpCapturePointer,msgData);            
    CommandReply(EPeninputOpCapturePointer,KNullDesC8,args);
    }    
    
// ---------------------------------------------------------------------------
// RPeniputAnim::SimulateEvent
// ---------------------------------------------------------------------------
//      
void RPeninputAnim::SimulateEvent(const TRawEvent& aEvent)
    {
    TIpcArgs args (TIpcArgs::ENothing);        
    TPckgC<TRawEvent> msgData(aEvent);
    args.Set(KMsgSlot1,&msgData);
    CommandReply(EPeninputOpSimulateEvent,KNullDesC8,args);
    //Command(EPeninputOpSimulateEvent,msgData);            
    }        

#ifdef RD_TACTILE_FEEDBACK     
// ---------------------------------------------------------------------------
// RPeniputAnim::DoFeedbackAreaOpL
// ---------------------------------------------------------------------------
//       
void RPeninputAnim::DoFeedbackAreaOpL(const TTactileFeedbackArea* aArea, 
                                              TInt aCount,TPeninputOpcodes aOp)
    {    
    for(TInt i = 0 ; i < aCount; ++i)
        {
        TTactileFeedbackArea oneArea = aArea[i];
        TPckgC<TTactileFeedbackArea> msgData(oneArea);
        Command(aOp,msgData);        
        }        
    }

// ---------------------------------------------------------------------------
// RPeniputAnim::DoFeedbackAreaOpReplyL
// ---------------------------------------------------------------------------
//       
void RPeninputAnim::DoFeedbackAreaOpReplyL(const TTactileFeedbackArea* aArea, 
                                              TInt aCount,TPeninputOpcodes aOp)
    {    
    TIpcArgs args (TIpcArgs::ENothing);           
    TPckgC<TInt> areaNum(aCount);        
    args.Set(KMsgSlot1,&areaNum);
    
    TInt memByteLen = aCount*sizeof(TTactileFeedbackArea);
    
    TUint8 *buf = new(ELeave) TUint8[memByteLen];
    
    TTactileFeedbackArea *areaBuf = reinterpret_cast<TTactileFeedbackArea *>(buf);
    for(TInt i = 0; i < aCount; ++i)
        {
        areaBuf[i] = aArea[i];
        }
        
    TPtrC msgData(reinterpret_cast<TUint16*>(buf),memByteLen/2);

    args.Set(KMsgSlot2,&msgData);  
    
    CommandReply(aOp,KNullDesC8,args);
    delete []buf;

    }


// ---------------------------------------------------------------------------
// RPeniputAnim::AddFeedbackAreaL
// ---------------------------------------------------------------------------
//       
void RPeninputAnim::AddFeedbackAreaL(const TTactileFeedbackArea* aArea, 
                                              TInt aCount,TBool aReplyFlag)
    {
    if(aReplyFlag)
        DoFeedbackAreaOpReplyL(aArea,aCount,EPeninputOpRegisterFeedbackArea);    
    else
        DoFeedbackAreaOpL(aArea,aCount,EPeninputOpRegisterFeedbackArea);    
    }
    
// ---------------------------------------------------------------------------
// RPeniputAnim::RemoveFeedbackAreaL
// ---------------------------------------------------------------------------
//           
void RPeninputAnim::RemoveFeedbackAreaL(const TTactileFeedbackArea* aArea,
                                             TInt aCount,TBool aReplyFlag)
    {
    if(aReplyFlag)
        DoFeedbackAreaOpReplyL(aArea,aCount,EPeninputOpDeRegisterFeedbackArea);    
    else
        DoFeedbackAreaOpL(aArea,aCount,EPeninputOpDeRegisterFeedbackArea);        
    }    
    
// ---------------------------------------------------------------------------
// RPeniputAnim::ChangeFeedbackAreaL
// ---------------------------------------------------------------------------
//               
void RPeninputAnim::ChangeFeedbackAreaL(const TTactileFeedbackArea* aArea, 
                                             TInt aCount,TBool aReplyFlag)
    {
    if(aReplyFlag)
        DoFeedbackAreaOpReplyL(aArea,aCount,EPeninputOpChangeFeedbackArea);    
    else
        DoFeedbackAreaOpL(aArea,aCount,EPeninputOpChangeFeedbackArea);    
    }   
// ---------------------------------------------------------------------------
// RPeniputAnim::ChangeFeedbackTypeL
// ---------------------------------------------------------------------------
// 
void RPeninputAnim::ChangeFeedbackTypeL(const TTactileFeedbackArea* aArea, 
											 TInt aCount,TBool aReplyFlag)
	{
    if(aReplyFlag)
        DoFeedbackAreaOpReplyL(aArea,aCount,EPeninputOpChangeFeedbackType);    
    else
        DoFeedbackAreaOpL(aArea,aCount,EPeninputOpChangeFeedbackType);
	}

#endif // RD_TACTILE_FEEDBACK 
    
void RPeninputAnim::EnableSprite(TBool aFlag)
    {
    TPckgC<TBool> msgData(aFlag);
    Command(EPeninputOpEnalbeSprite,msgData);
    }

// ---------------------------------------------------------------------------
// RPeniputAnim::GetDSAState
// ---------------------------------------------------------------------------
//      
void RPeninputAnim::GetDSAState(TBool& aState)
    {
    TIpcArgs args (TIpcArgs::ENothing);        
    TPckg<TBool> msgData(aState);
    args.Set(KMsgSlot1,&msgData);
    CommandReply(EPeninputOpRequestDSAState,KNullDesC8,args);
    //Command(EPeninputOpSimulateEvent,msgData);            
    }        
    
// End of File
