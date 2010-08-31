/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for peninput animation command
*
*/

#include "peninputanimcommand.h"
#include "rpeninputanim.h"
#include <peninputcmd.h>
#include "peninputcmdparamext.h"
// ---------------------------------------------------------------------------
// TAnimCmd::TAnimCmd
// Default constructor
// ---------------------------------------------------------------------------
//    
//#define UPDATE_MORE_SMALL_AREA
TAnimCmd::TAnimCmd(RPeninputAnim& aAnim,TBool aFlag)
				: iAnim(aAnim),iReady(aFlag)
	{
	}
	


// ---------------------------------------------------------------------------
// TAnimCmd::ExecuteAnimCommand
// Execute animation command
// ---------------------------------------------------------------------------
//
TBool TAnimCmd::ExecuteAnimCommand() const
	{
	//do nothing		
	return ETrue;
	}


//class TAimActivateUiCmd
// ---------------------------------------------------------------------------
// TAnimActivateUiCmd::TAnimActivateUiCmd
// Default constructor
// ---------------------------------------------------------------------------
//    	
TAnimActivateUiCmd::TAnimActivateUiCmd(RPeninputAnim& aAnim,
												TBool aFlag,TBool aReadyFlag)
								: TAnimCmd(aAnim,aReadyFlag),iActiveFlag(aFlag)
	{	
	}	
  
// ---------------------------------------------------------------------------
// TAnimActivateUiCmd::ExecuteAnimCommand
// Execute animation command
// ---------------------------------------------------------------------------
//    
TBool TAnimActivateUiCmd :: ExecuteAnimCommand() const
	{
	if(!iReady)
		return EFalse;
	
	if(iActiveFlag)
		{
		iAnim.Activate();
		}
	else
		{
		iAnim.Deactivate();
		}						
	return ETrue;		
	}
	
	
//class TAnimChangeSizeCmd
// ---------------------------------------------------------------------------
// TAnimChangeSizeCmd::TAnimChangeSizeCmd
// Default constructor
// ---------------------------------------------------------------------------
//    		
TAnimChangeSizeCmd::TAnimChangeSizeCmd(RPeninputAnim& aAnim,
							const TSize& aSize) : TAnimCmd(aAnim),iSize(aSize)
	{	
	}
	
// ---------------------------------------------------------------------------
// TAnimChangeSizeCmd::ExecuteAnimCommand
// Execute animation command
// ---------------------------------------------------------------------------
//	
TBool TAnimChangeSizeCmd ::ExecuteAnimCommand() const
	{
	TRAP_IGNORE(iAnim.LayoutSizeChangedL(iSize));
	return ETrue;	
	}

//class TAnimSetPosCmd
// ---------------------------------------------------------------------------
// TAnimSetPosCmd::TAnimSetPosCmd
// Default constructor
// ---------------------------------------------------------------------------
//    		
TAnimSetPosCmd::TAnimSetPosCmd(RPeninputAnim& aAnim,const TPoint& aPt)
									: TAnimCmd(aAnim),iPos(aPt)
	{
	}

// ---------------------------------------------------------------------------
// TAnimSetPosCmd::ExecuteAnimCommand
// Execute animation command
// ---------------------------------------------------------------------------
//	
TBool TAnimSetPosCmd ::ExecuteAnimCommand() const
	{
	iAnim.SetUiLayoutPos(iPos);			
	return ETrue;	
	}
	
	
//class TAnimChangeDispModeCmd
// ---------------------------------------------------------------------------
// TAnimChangeDispModeCmd::TAnimChangeDispModeCmd
// Default constructor
// ---------------------------------------------------------------------------
//    		
TAnimChangeDispModeCmd::TAnimChangeDispModeCmd(RPeninputAnim& aAnim,
									RWsSprite& aSprite,TSpriteMember& aMember)
							   		: TAnimCmd(aAnim), iSprite(aSprite),
							   			iSpriteMember(aMember)
	{
	}
	
// ---------------------------------------------------------------------------
// TAnimChangeDispModeCmd::ExecuteAnimCommand
// Execute animation command
// ---------------------------------------------------------------------------
//
TBool TAnimChangeDispModeCmd :: ExecuteAnimCommand() const
	{	
	iSprite.UpdateMember(0, iSpriteMember);			
	return ETrue;	
	}
	
	
//class TAnimUpdateUiRectCmd
// ---------------------------------------------------------------------------
// TAnimUpdateUiRectCmd::TAnimUpdateUiRectCmd
// Default constructor
// ---------------------------------------------------------------------------
//    
TAnimUpdateUiRectCmd::TAnimUpdateUiRectCmd(RPeninputAnim& aAnim,
										const TRect& aRect,TBool aFullFlag)
    :TAnimCmd(aAnim)
    {
    TUpdateArea area = 
        {
        aRect,aFullFlag
        };	
    iArea.Append(area);
    }

// ---------------------------------------------------------------------------
// TAnimUpdateUiRectCmd::TAnimUpdateUiRectCmd
// c++ constructor
// ---------------------------------------------------------------------------
//    
TAnimUpdateUiRectCmd::TAnimUpdateUiRectCmd(RPeninputAnim& aAnim, 
                                TUpdateArea *aArea,TInt aNum)
									: TAnimCmd(aAnim), iRectNum(aNum)                                
    {
    for(TInt i = 0 ; i < aNum; ++i)
        {
        TUpdateArea area =       
            {
            aArea[i].iRect,
            aArea[i].iFullUpdateFlag
            };  
        iArea.Append(area);
        }
    }

TAnimUpdateUiRectCmd::~TAnimUpdateUiRectCmd()
    {
    iArea.Close();    
    }
// ---------------------------------------------------------------------------
// TAnimUpdateUiRectCmd::ExecuteAnimCommand
// Execute animation command
// ---------------------------------------------------------------------------
//    
TBool TAnimUpdateUiRectCmd ::ExecuteAnimCommand() const
	{	
    if (iArea.Count())
        {
#ifdef UPDATE_MORE_SMALL_AREA        
        iAnim.UpdateArea(iArea);
#else
        TUpdateArea area = iArea[0];
        for( TInt i = 1; i < iArea.Count(); ++i )
            {
            area.iRect.BoundingRect(iArea[i].iRect);
            }
        iArea.Reset();
        iArea.Append( area );
        iAnim.UpdateArea(iArea);
#endif //UPDATE_MORE_SMALL_AREA        
        }

	  //otherwise it's already updated.
    iArea.Close();    
    return ETrue;	
    }

//class TAnimCapturePtCmd
// ---------------------------------------------------------------------------
// TAnimCapturePtCmd::TAnimCapturePtCmd
// Default constructor
// ---------------------------------------------------------------------------
//	
TAnimCapturePtCmd::TAnimCapturePtCmd(RPeninputAnim& aAnim,TBool aFlag, TInt aCaptureCtrlID)
								: TAnimCmd(aAnim),iCaptureFlag(aFlag), iCaptureCtrlID(aCaptureCtrlID)
	{
	}	


// ---------------------------------------------------------------------------
// TAnimCapturePtCmd::ExecuteAnimCommand
// Execute animation command
// ---------------------------------------------------------------------------
//	
TBool TAnimCapturePtCmd :: ExecuteAnimCommand() const
	{
	iAnim.CapturePointer(iCaptureFlag, iCaptureCtrlID);			
	return ETrue;	
	}	


//class TAnimSimulateEventCmd
// ---------------------------------------------------------------------------
// TAnimSimulateEventCmd::TAnimSimulateEventCmd
// Default constructor
// ---------------------------------------------------------------------------
//
TAnimSimulateEventCmd::TAnimSimulateEventCmd(
								RPeninputAnim& aAnim,const TRawEvent& aEvent)
								: TAnimCmd(aAnim),iEvent(aEvent)
	{
	}		

// ---------------------------------------------------------------------------
// TAnimSimulateEventCmd::ExecuteAnimCommand
// Execute animation command
// ---------------------------------------------------------------------------
//	
TBool TAnimSimulateEventCmd :: ExecuteAnimCommand() const
	{
	iAnim.SimulateEvent(iEvent);
	return ETrue;			
	}		
	
	
//class TAnimFlushWsSessionCmd
// ---------------------------------------------------------------------------
// TAnimFlushWsSessionCmd::TAnimFlushWsSessionCmds
// Default constructor
// ---------------------------------------------------------------------------
//
TAnimFlushWsSessionCmd::TAnimFlushWsSessionCmd(RPeninputAnim& aAnim,
										RWsSession& aWs)
										: TAnimCmd(aAnim),iWs(aWs)
	{
	}
	
// ---------------------------------------------------------------------------
// TAnimFlushWsSessionCmd::ExecuteAnimCommand
// Execute animation command
// ---------------------------------------------------------------------------
//
TBool TAnimFlushWsSessionCmd :: ExecuteAnimCommand() const
	{
	iWs.Flush();			
	return ETrue;	
	}			


#ifdef RD_TACTILE_FEEDBACK

//class TAnimFeedbackAreaCmd
// ---------------------------------------------------------------------------
// TAnimFlushWsSessionCmd::TAnimFeedbackAreaCmd
// Execute animation command
// ---------------------------------------------------------------------------
//
TAnimFeedbackAreaCmd::TAnimFeedbackAreaCmd(RPeninputAnim& aAnim,
                             const TTactileFeedbackArea &aArea,
                             TFeedbackAreaOpEnum aOp,TBool aReplyFlag)
                            : TAnimCmd(aAnim),iAreaCount(0),iCmdOp(aOp),
                            iCmdReplyFlag(aReplyFlag)
    {
    iFeedbackAreaArray[iAreaCount++] = aArea;
    }


/*TAnimFeedbackAreaCmd::TAnimFeedbackAreaCmd(RPeninputAnim& aAnim,
                          const RArray<TTactileFeedbackArea>& aArea,
                          TFeedbackAreaOpEnum aOp,TBool aReplyFlag)
                          : TAnimCmd(aAnim),iAreaCount(0),iCmdOp(aOp),
                            iCmdReplyFlag(aReplyFlag)
    {
    for(TInt i = 0; i < aArea.Count(); ++i)
        {        
        iFeedbackAreaArray[iAreaCount++] = aArea[i];
        }
    }*/

TBool TAnimFeedbackAreaCmd :: ExecuteAnimCommand() const
	{
	switch(iCmdOp)
	    {
	        case  TFeedbackAreaOpAdd:
                TRAP_IGNORE(iAnim.AddFeedbackAreaL(iFeedbackAreaArray,iAreaCount,iCmdReplyFlag));
	            break;
	        case TFeedbackAreaOpRemove:
	            TRAP_IGNORE(iAnim.RemoveFeedbackAreaL(iFeedbackAreaArray,iAreaCount,iCmdReplyFlag));
	            break;
	        case TFeedbackAreaOpChange:
                TRAP_IGNORE(iAnim.ChangeFeedbackAreaL(iFeedbackAreaArray,iAreaCount,iCmdReplyFlag));      	        
	            break;
	        case TFeedbackTypeOpChange:
	        	TRAP_IGNORE(iAnim.ChangeFeedbackTypeL(iFeedbackAreaArray,iAreaCount,iCmdReplyFlag));
	        	break;
	        default:	          
	            return EFalse;
	    }
	
	return ETrue;	
	}			    
#endif // RD_TACTILE_FEEDBACK
TAnimEnableSpriteCmd::TAnimEnableSpriteCmd(RPeninputAnim& aAnim,TBool aFlag)
                        : TAnimCmd(aAnim),iSpriteFlag(aFlag)
    {										   
    }
    
TBool TAnimEnableSpriteCmd :: ExecuteAnimCommand() const
	{
	iAnim.EnableSprite(iSpriteFlag);
	return ETrue;
	}
    
