/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for peninput server
*
*/

#include <coedef.h>
#include "peninputanimclientobj.h"
#include "peninputserver.h"
#include "peninputclientserver.h"
#include "peninputanimcommand.h"
#include <coemain.h>
// the animation dll name
_LIT( KAnimDllName, "peninputanim.dll" );
//animation window group name
//_LIT(KPeninputServerAnimName,"peninputserveranimobj");
class RWsSession;
class RWindowGroup;

void HideWindowGroupL(const RWsSession& aWs,const RWindowGroup& aGroup);

// ---------------------------------------------------------------------------
// CPeninputAnimObj::CPeninputAnimObj
// Default constructor
// ---------------------------------------------------------------------------
//
CPeninputAnimObj::CPeninputAnimObj(TSpriteMember& aSpriteMember)
                    : CActive( EPriorityStandard ),iSpriteMember(aSpriteMember)
    {
    iOnExiting = EFalse;
    }

// ---------------------------------------------------------------------------
// CPeninputAnimObj::NewL
// factory method
// ---------------------------------------------------------------------------
//
CPeninputAnimObj* CPeninputAnimObj::NewL(TSpriteMember& aSpriteMember)
    {    
    CPeninputAnimObj* animObj = new(ELeave) CPeninputAnimObj(aSpriteMember);        
    CleanupStack::PushL(animObj);            
    CActiveScheduler::Add(animObj);
    animObj->ConstructL();
    CleanupStack::Pop(animObj);
    return animObj;
    }

// ---------------------------------------------------------------------------
// CPeninputAnimObj::~CPeninputAnimObj
// Destructor
// ---------------------------------------------------------------------------
//
CPeninputAnimObj::~CPeninputAnimObj()
    {        
    Cancel();
    for(int i = 0 ; i < iAnimCmd.Count(); ++i)
    	{
    	const TAnimCmd* cmd = iAnimCmd[i];
    	delete cmd;
    	}
    iAnimCmd.Close();
    
    // closing the window server session frees all 
    // used resources in the ws session, including:
    //   iSprite
    //   iAnim
    //   iAnimDll
    iSprite.Close();
    iAnim.Close();
    iAnimDll.Close();            
    iWindowGroup.Close();
    iWsSession.Close();
    }

// ---------------------------------------------------------------------------
// CPeninputAnimObj::RunL
// active ao call back
// ---------------------------------------------------------------------------
//
void CPeninputAnimObj::RunL()
    {
    ExecuteAnimCommand();
    }

// ---------------------------------------------------------------------------
// CPeninputAnimObj::DoCancel
// ---------------------------------------------------------------------------
//
void CPeninputAnimObj::DoCancel()
    {
    //do nothing
    }

// ---------------------------------------------------------------------------
// CPeninputAnimObj::RunError
// ---------------------------------------------------------------------------
//    
TInt CPeninputAnimObj::RunError(TInt /*aErr*/)
    {
    //ignore error
    return KErrNone;
    }
    
// ---------------------------------------------------------------------------
// CPeninputAnimObj::ConstructL
// 2nd phase constructor
// ---------------------------------------------------------------------------
//    
void CPeninputAnimObj::ConstructL()
    {
    iAnimCmd.Reset();
    ConstructAnimL();    
    }

// ---------------------------------------------------------------------------
// CPeninputAnimObj::ExecuteAnimCommand
// Execute animation command
// ---------------------------------------------------------------------------
//        
void CPeninputAnimObj::ExecuteAnimCommand()
    {    
    TInt i = 0;
    for(; i < iAnimCmd.Count(); ++i)
        {        
    	const TAnimCmd* cmd = iAnimCmd[i];        
        if(!cmd->ExecuteAnimCommand())
        	break; //execution not finished, need wait to next round. 
        delete cmd;        	
        }   
     
    //remove executed command
    while(--i >= 0)
    	{
    	iAnimCmd.Remove(i);
    	}
    }

// ---------------------------------------------------------------------------
// CPeninputAnimObj::ExeOnePendingAnimCmd
// Execute the first pending animation command
// ---------------------------------------------------------------------------
//        
void CPeninputAnimObj::ExeOnePendingAnimCmd()
    {
    //search the first wait command
    for(TInt i = 0 ; i < iAnimCmd.Count(); i++)
        {
        if(!iAnimCmd[i]->Ready())
            {
            iAnimCmd[i]->SetReady(ETrue);
            //break;
            }
        }        
    //CompleteRequest();
    SetObjActive();
    }

// ---------------------------------------------------------------------------
// CPeninputAnimObj::AddAnimCommand
// Add a animation command
// ---------------------------------------------------------------------------
//        
void CPeninputAnimObj::AddAnimCommand(TAnimCmd* aCmd)
    {    
    if(iOnExiting) // do nothing if prepare for exiting
        {
        delete aCmd;
        aCmd = 0;    
        return;
        }
    iAnimCmd.Append(aCmd);
   
    //CompleteRequest();
    SetObjActive();
    }

// ---------------------------------------------------------------------------
// CPeninputAnimObj::SetObjActive
// Set animation object active so as to be rescheduled by AO scheduler.
// ---------------------------------------------------------------------------
//        
void CPeninputAnimObj::SetObjActive()
    {
    if(!IsActive())
        {
    	iStatus=KRequestPending;
    	SetActive();    	
    	TRequestStatus *pS=(&iStatus);
    	
        User::RequestComplete(pS,0);
    	}    
    }


// ---------------------------------------------------------------------------
// CPeninputAnimObj::ConstructSpriteAnimL
// ---------------------------------------------------------------------------
//
TInt CPeninputAnimObj::ConstructAnimL()
    {                
    User::LeaveIfError( iWsSession.Connect() );   
    iWindowGroup = RWindowGroup( iWsSession);
    iWindowGroup.Construct( RThread().Id(), EFalse );    
   
    iWindowGroup.SetOrdinalPosition( -1, -1);    
    iWindowGroup.SetOrdinalPriorityAdjust( ECoeWinPriorityAlwaysAtFront );
    iWindowGroup.EnableReceiptOfFocus( EFalse );
    iWindowGroup.AutoForeground(EFalse);
    
	HideWindowGroupL(iWsSession,iWindowGroup);  
    TInt id = iWindowGroup.Identifier();    
    iSprite = RWsSprite( iWsSession);
    User::LeaveIfError( iSprite.Construct( iWindowGroup,
                                           TPoint( 0, 0 ),ESpriteNoChildClip | 
                                                            ESpriteNoShadows ) );
    // add sprite member
    User::LeaveIfError( iSprite.AppendMember( iSpriteMember ));
         
    // load peninput animation dll
    iAnimDll = RAnimDll( iWsSession );
    User::LeaveIfError( iAnimDll.Load( KAnimDllName ) );    
    
    // create peninput sprite animation class
    iAnim = RPeninputAnim( iAnimDll);
    iAnim.ConstructL( iSprite );    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CPeninputAnimObj::AddActivationCmd
// Add activation command
// ---------------------------------------------------------------------------
//
TBool CPeninputAnimObj::AddActivationCmd(TBool aUiFlag, TBool aWaitFlag)
	{
	TAnimActivateUiCmd* cmd = new TAnimActivateUiCmd(iAnim,aUiFlag,!aWaitFlag);
	if(!cmd)
		return EFalse;
	AddAnimCommand(cmd);
	return ETrue;
	}

// ---------------------------------------------------------------------------
// CPeninputAnimObj::AddChangeSizeCmd
// Add changing size command
// ---------------------------------------------------------------------------
//	
TBool CPeninputAnimObj::AddChangeSizeCmd(const TSize& aSize)
	{
	TAnimChangeSizeCmd* cmd = new TAnimChangeSizeCmd(iAnim,aSize);
	if(!cmd)
		return EFalse;
	AddAnimCommand(cmd);
	return ETrue;	
	}
	
// ---------------------------------------------------------------------------
// CPeninputAnimObj::AddSetPosCmd
// Add setting position command
// ---------------------------------------------------------------------------
//	
TBool CPeninputAnimObj::AddSetPosCmd(const TPoint& aPt)
	{
	TAnimSetPosCmd* cmd = new TAnimSetPosCmd(iAnim,aPt);
	if(!cmd)
		return EFalse;
	AddAnimCommand(cmd);
	return ETrue;
	}
	
// ---------------------------------------------------------------------------
// CPeninputAnimObj::AddChangeDispModeCmd
// Add changing display mode command
// ---------------------------------------------------------------------------
//		
TBool CPeninputAnimObj::AddChangeDispModeCmd(TSpriteMember& aMember)
	{
    TAnimChangeDispModeCmd* cmd = new	TAnimChangeDispModeCmd(iAnim,
                                                             iSprite,
                                                             aMember);

	if(!cmd)
		return EFalse;
	AddAnimCommand(cmd);
	return ETrue;	
	}
	
// ---------------------------------------------------------------------------
// CPeninputAnimObj::AddUpdateUiCmd
// Add updaing UI command
// ---------------------------------------------------------------------------
//			
TBool CPeninputAnimObj::AddUpdateUiCmd(const TRect& aRect, TBool aUpdateFlag)
	{
	TAnimUpdateUiRectCmd* cmd = new TAnimUpdateUiRectCmd(iAnim,aRect,aUpdateFlag);
	if(!cmd)
		return EFalse;
	AddAnimCommand(cmd);
	return ETrue;		
	}

// ---------------------------------------------------------------------------
// CPeninputAnimObj::AddUpdateUiCmd
// Add updaing UI command
// ---------------------------------------------------------------------------
//
TBool CPeninputAnimObj::AddUpdateUiCmd(TUpdateArea *aArea,TInt aNum)
    {
	  TAnimUpdateUiRectCmd* cmd = new TAnimUpdateUiRectCmd(iAnim,aArea,aNum);
	  if (!cmd)
		    return EFalse;
	  AddAnimCommand(cmd);
	  return ETrue;		
    }

// ---------------------------------------------------------------------------
// CPeninputAnimObj::AddCapturePointerCmd
// Add capturing pointer command
// ---------------------------------------------------------------------------
//				
TBool CPeninputAnimObj::AddCapturePointerCmd(TBool aFlag, TInt aCaptureCtrlID)
	{
	TAnimCapturePtCmd* cmd = new TAnimCapturePtCmd(iAnim,aFlag, aCaptureCtrlID);	
	if(!cmd)
		return EFalse;
	AddAnimCommand(cmd);
	return ETrue;		
	}
	
// ---------------------------------------------------------------------------
// CPeninputAnimObj::AddSimulatedEventCmd
// Add ignoring similated event command
// ---------------------------------------------------------------------------
//					
TBool CPeninputAnimObj::AddSimulateEventCmd(const TRawEvent& aEvent)
	{
    TAnimSimulateEventCmd* cmd = new TAnimSimulateEventCmd(iAnim, aEvent);
    if (!cmd)
        return EFalse;
    AddAnimCommand(cmd);
    return ETrue;	
	}

// ---------------------------------------------------------------------------
// Execute simulate event command
// ---------------------------------------------------------------------------
//					
TBool CPeninputAnimObj::ExecuteSimulateEventCmd( const TRawEvent& aEvent )
	{
    TAnimSimulateEventCmd* cmd = new TAnimSimulateEventCmd( iAnim, aEvent );    
    if (!cmd)
        return EFalse;
    cmd->ExecuteAnimCommand();
    delete cmd;
    cmd = NULL;
    return ETrue;		
	}

// ---------------------------------------------------------------------------
// CPeninputAnimObj::AddFlushSessionCmd
// Add flushing session command
// ---------------------------------------------------------------------------
//						
TBool CPeninputAnimObj::AddFlushSessionCmd()
	{
	TAnimFlushWsSessionCmd* cmd = new TAnimFlushWsSessionCmd(iAnim,iWsSession);
	if(!cmd)
		return EFalse;
	AddAnimCommand(cmd);
	return ETrue;		
	}

#ifdef RD_TACTILE_FEEDBACK
	
// ---------------------------------------------------------------------------
// CPeninputAnimObj::AddRegisterFeedbackAreaCmd
// Add feedback registeration command
// ---------------------------------------------------------------------------
//							
TBool CPeninputAnimObj::AddRegisterFeedbackAreaCmd(const TTactileFeedbackArea& aArea,
                                                TBool aCmdFlag)		
    { 
	TAnimFeedbackAreaCmd* cmd = new TAnimFeedbackAreaCmd(iAnim,aArea,
	                                                       TAnimFeedbackAreaCmd::TFeedbackAreaOpAdd,
	                                                       aCmdFlag);
	if(!cmd)
		return EFalse;
	AddAnimCommand(cmd);
	return ETrue;	        
    }


// ---------------------------------------------------------------------------
// CPeninputAnimObj::AddRegisterFeedbackAreaCmd
// Add feedback registeration command
// ---------------------------------------------------------------------------
//							
/*TBool CPeninputAnimObj::AddRegisterFeedbackAreaCmd(const RArray<TTactileFeedbackArea>& aAreaList)		
    { 
	TAnimFeedbackAreaCmd* cmd = new TAnimFeedbackAreaCmd(iAnim,aAreaList,
	                                                       TAnimFeedbackAreaCmd::TFeedbackAreaOpAdd,
	                                                       ETrue);
	if(!cmd)
		return EFalse;
	AddAnimCommand(cmd);
	return ETrue;	        
    }*/


// ---------------------------------------------------------------------------
// CPeninputAnimObj::AddDeRegisterFeedbackAreaCmd
// Remove feedback registeration command
// ---------------------------------------------------------------------------
//							
TBool CPeninputAnimObj::AddDeRegisterFeedbackAreaCmd(const TTactileFeedbackArea& aArea,
                                            TBool aCmdFlag)
    { 
	TAnimFeedbackAreaCmd* cmd = new TAnimFeedbackAreaCmd(iAnim,aArea,
	                                                       TAnimFeedbackAreaCmd::TFeedbackAreaOpRemove,
	                                                       aCmdFlag);
	if(!cmd)
		return EFalse;
	AddAnimCommand(cmd);
	return ETrue;	        
    }
    
// ---------------------------------------------------------------------------
// CPeninputAnimObj::AddChangeFeedbackAreaCmd
// Add feedback registeration command
// ---------------------------------------------------------------------------
//							
TBool CPeninputAnimObj::AddChangeFeedbackAreaCmd(const TTactileFeedbackArea& aArea,
                                            TBool aCmdFlag)
    { 
	TAnimFeedbackAreaCmd* cmd = new TAnimFeedbackAreaCmd(iAnim,aArea,
	                                                       TAnimFeedbackAreaCmd::TFeedbackAreaOpChange,
	                                                       aCmdFlag);
	if(!cmd)
		return EFalse;
	AddAnimCommand(cmd);
	return ETrue;	        
    }  

// ---------------------------------------------------------------------------
// CPeninputAnimObj::AddChangeFeedbackTypeCmd
// Add feedback registeration command
// ---------------------------------------------------------------------------
//							
TBool CPeninputAnimObj::AddChangeFeedbackTypeCmd(const TTactileFeedbackArea& aArea,
                                            TBool aCmdFlag)
    { 
	TAnimFeedbackAreaCmd* cmd = new TAnimFeedbackAreaCmd(iAnim,aArea,
	                                                       TAnimFeedbackAreaCmd::TFeedbackTypeOpChange,
	                                                       aCmdFlag);
	if(!cmd)
		return EFalse;
	AddAnimCommand(cmd);
	return ETrue;	        
    }  

#endif // RD_TACTILE_FEEDBACK
TBool CPeninputAnimObj::AddEnalbeSpriteCmd(TBool aFlag)
    {
	TAnimEnableSpriteCmd* cmd = new TAnimEnableSpriteCmd(iAnim,aFlag);
	if(!cmd)
		return EFalse;
	AddAnimCommand(cmd);
	return ETrue;	                
    }

RWindowGroup& CPeninputAnimObj::WindowGroup()
    {
    return iWindowGroup;
    }
    
void CPeninputAnimObj::OnExiting()
    {
    iOnExiting = ETrue;
    }
    
void CPeninputAnimObj::GetDSAState(TBool& aState)
    {
    iAnim.GetDSAState(aState);    
    }
//end of file
