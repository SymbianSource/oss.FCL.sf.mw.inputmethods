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
* Description:  Defination for peninput server
*
*/


#ifndef C_CPENINPUTANIM_CMD_H
#define C_CPENINPUTANIM_CMD_H
#include <e32def.h>
#include <e32cmn.h>
#include <w32std.h>
//pen input animation client
class RPeninputAnim;
#include "peninputcmdparamext.h"
const TInt KMaxNum = 1;
/**
 * Class for handling animation command. This is the base class for all command
 * 
 *
 * @since S60 v4.0
 */
class TAnimCmd  
{
public:
    /**
     * Default constructor. 
     *
     * @since S60 v4.0
     * @param aAnim The animation cliet which executes the command
     * @param aReayFlag The waiting flag indicats whether this command should be 
     * executed immediately. Deafult value is ETrue, i.e, execute the command.
     */    
    TAnimCmd(RPeninputAnim& aAnim,TBool aReayFlag = ETrue);
    
    /**
     * Set command waiting flag.
     *
     * @since S60 v4.0
     * @param aFlag new waiting flag
     */            
    inline void SetReady(TBool aFlag); 
    
    /**
     * Test whether this command is executable now.
     *
     * @since S60 v4.0
     * @return ETrue if command is ready to run
     */    
    inline TBool Ready() const;
    
    /**
     * Execute the command
     *
     * @since S60 v4.0
     * @return ETrue if command has been executed successfully.
     */     
    virtual TBool ExecuteAnimCommand() const;                
protected:
    /**
     * animation object which execute the command. Not owned.
     */       	
	RPeninputAnim& iAnim;
	/**
	 * flags tells whether this command can be execute immediately
	 */
	TBool iReady; 
} ;
   

//class TAnimActivateUiCmd
/**
 * Class for handling animation activation/deactivation command. 
 * 
 *
 * @since S60 v4.0
 */
class TAnimActivateUiCmd : public TAnimCmd
	{
public:
    /**
     * Default constructor. 
     *
     * @since S60 v4.0
     * @param aAnim The animation cliet which executes the command     
     * @param aUiFlag The UI activation flag
     * @param aReadyFlag indicates whether this command is ready. Default value is false
     */   
	TAnimActivateUiCmd(RPeninputAnim& aAnim,TBool aUiFlag,TBool aReadyFlag = EFalse);
	
	/**
     * Execute the command
     *
     * @since S60 v4.0
	 * @return ETrue if command has been executed successfully.     
     */     
    virtual TBool ExecuteAnimCommand() const;

private:
	/**
	 * The activation flag.
	 */
	TBool iActiveFlag;		
	};
	

/**
 * Class for handling changing animation sprite size command. 
 * 
 *
 * @since S60 v4.0
 */	
class TAnimChangeSizeCmd : public TAnimCmd
	{
public:
    /**
     * Default constructor. 
     *
     * @since S60 v4.0
     * @param aAnim The animation cliet which executes the command     
     * @param aSize The new sprite size
     */   
	TAnimChangeSizeCmd(RPeninputAnim& aAnim,const TSize& aSize);
	/**
     * Execute the command
     *
     * @since S60 v4.0
	 * @return ETrue if command has been executed successfully.          
     */     
 	virtual TBool ExecuteAnimCommand() const;

private:
	/**
	 * sprite new size
	 */
	TSize iSize;		
	};
	
/**
 * Class for handling set animation sprite position command. 
 * 
 *
 * @since S60 v4.0
 */		
class TAnimSetPosCmd : public TAnimCmd
	{
public:
    /**
     * Default constructor. 
     *
     * @since S60 v4.0
     * @param aAnim The animation cliet which executes the command          
     * @param aPt The new position
     */   
	TAnimSetPosCmd(RPeninputAnim& aAnim,const TPoint& aPt);
	
	/**
	 * Execute the command
	 *
	 * @since S60 v4.0
	 * @return ETrue if command has been executed successfully.     	 
	 */     
	virtual TBool ExecuteAnimCommand() const;

private:
	/**
	 * The new sprite position
	 */
	TPoint iPos;		
	};

/**
 * Class for handling change animation sprite display mode command. 
 * 
 *
 * @since S60 v4.0
 */		
class TAnimChangeDispModeCmd : public TAnimCmd
	{
public:
    /**
     * Default constructor. 
     *
     * @since S60 v4.0
     * @param aAnim The animation cliet which executes the command          
     * @param aSprite The sprite which needs to be changed.
     * @param aMember The sprite member to be changed.
     */   
	TAnimChangeDispModeCmd(RPeninputAnim& aAnim,
							RWsSprite& aSprite,TSpriteMember& aMember);
	/**
	 * Execute the command
	 *
	 * @since S60 v4.0
	 * @return ETrue if command has been executed successfully.     	 
	 */     
	virtual TBool ExecuteAnimCommand() const;

private:
	/**
	 * The sprite to be changed.
	 */
	RWsSprite& iSprite;
	
	/**
	 * The sprite member to be changed.
	 */	
	TSpriteMember& iSpriteMember;		
};
	
	
/**
 * Class for handling update animation sprite command. 
 * 
 *
 * @since S60 v4.0
 */		    
class TAnimUpdateUiRectCmd : public TAnimCmd
	{
public:
    /**
     * Default constructor. 
     *
     * @since S60 v4.0
     * @param aAnim The animation cliet which executes the command          
     * @param aRect The rect to be updated
     * @param aFullFlag Flag indicates whether full sprite should be updated. 
     */   
	TAnimUpdateUiRectCmd(RPeninputAnim& aAnim,
									const TRect& aRect,TBool aFullFlag);
	
    /**
     *
     * @since S60 v4.0
     * @param aAnim The animation client which executes the command
     * @param aArea* The pointer point to 1st TUpdateArea that need to be udpated
     * @param aNum The count of TUpdateArea that need to be updated
     */   
    TAnimUpdateUiRectCmd(RPeninputAnim& aAnim, TUpdateArea *aArea, TInt aNum);

    /**
     * Destructor. 
     *
     * @since S60 v4.0
     */   
    ~TAnimUpdateUiRectCmd();

    /**
     * Execute the command
     *
     * @since S60 v4.0
	   * @return ETrue if command has been executed successfully.          
     */     
    virtual TBool ExecuteAnimCommand() const;

private:
	  /**
	   * The rect to be updated
	   */
	  mutable RArray<TUpdateArea> iArea;
	/**
	 * The flag for full sprite updating. iRect is ignored if this flag is true
	 */
	TBool iFullUpdateFlag;		
	
	TInt iRectNum;
	};
	
/**
 * Class for handling capture pointer command. 
 * 
 *
 * @since S60 v4.0
 */			
class TAnimCapturePtCmd : public TAnimCmd
	{
public:
    /**
     * Default constructor. 
     *
     * @since S60 v4.0
     * @param aAnim The animation cliet which executes the command          
     * @param aFlag flag indicates capturing or releasing the point capture
     */   
	TAnimCapturePtCmd(RPeninputAnim& aAnim,TBool aFlag, TInt aCaptureCtrlID);
	/**
     * Execute the command
     *
     * @since S60 v4.0
	 * @return ETrue if command has been executed successfully.          
     */     
    virtual TBool ExecuteAnimCommand() const;

private:
	/**
	 * The capture flag
	 */
	TBool iCaptureFlag;		
	
	TInt iCaptureCtrlID;
	};	
	
/**
 * Class for handling ignoring simuated event command. 
 * 
 *
 * @since S60 v4.0
 */			
class TAnimSimulateEventCmd : public TAnimCmd
	{
public:
    /**
     * Default constructor. 
     *
     * @since S60 v4.0
     * @param aAnim The animation cliet which executes the command          
     * @param aFlag The flag
     */   
	TAnimSimulateEventCmd(RPeninputAnim& aAnim,const TRawEvent& aEvent);
	
	/**
     * Execute the command
     *
     * @since S60 v4.0
	 * @return ETrue if command has been executed successfully.          
     */     
    virtual TBool ExecuteAnimCommand() const;

private:
	/**
	 * Event to be simulated. 
	 */
	TRawEvent iEvent; 
	};			
	
/**
 * Class for handling flush ws session command. 
 * 
 *
 * @since S60 v4.0
 */			
class TAnimFlushWsSessionCmd : public TAnimCmd
	{
public:		
    /**
     * Default constructor. 
     *
     * @since S60 v4.0
     * @param aAnim The animation cliet which executes the command          
     * @aWs The session to be flushed
     */   
	TAnimFlushWsSessionCmd(RPeninputAnim& aAnim,RWsSession& aWs);
	
	/**
     * Execute the command
     *
     * @since S60 v4.0
	 * @return ETrue if command has been executed successfully.          
     */     
    virtual TBool ExecuteAnimCommand() const;

private:
	/**
	 * The window session.
	 */
	RWsSession& iWs;		
	};			

#ifdef RD_TACTILE_FEEDBACK
//class TAnimFeedbackAreaCmd
/**
 * Class for handling register/de-register feedback area command
 * 
 *
 * @since S60 v4.0
 */
 
class TAnimFeedbackAreaCmd : public TAnimCmd
	{
public:
    enum TFeedbackAreaOpEnum
        {
        TFeedbackAreaOpAdd,
        TFeedbackAreaOpRemove,
        TFeedbackAreaOpChange,
        TFeedbackTypeOpChange
        };

public:
    /**
     * Default constructor. 
     *
     * @since S60 v4.0
     * @param aAnim The animation cliet which executes the command     
     * @param aArea The feedback area data
     * @param aOp command op tells whether register or deregister area.
     * @param aReplyFlag Flag tells how the command sent to server.
     */   
	  TAnimFeedbackAreaCmd(RPeninputAnim& aAnim,
	                       const TTactileFeedbackArea& aArea,
	                       TFeedbackAreaOpEnum aOp,
	                       TBool aReplyFlag);

    /**
     * The constructor is not open yet. 
     *  constructor. 
     *
     * @since S60 v4.0
     * @param aAnim The animation cliet which executes the command     
     * @param aArea The feedback area data
     * @param aOp command op tells whether register or deregister area.
     * @param aReplyFlag Flag tells how the command sent to server.     
     */   
     /*TAnimFeedbackAreaCmd(RPeninputAnim& aAnim,
	                    const RArray<TTactileFeedbackArea>& aArea,
	                       TFeedbackAreaOpEnum aOp,
	                       TBool aReplyFlag);*/

	/**
     * Execute the command
     *
     * @since S60 v4.0
	 * @return ETrue if command has been executed successfully.     
     */     
    virtual TBool ExecuteAnimCommand() const;

private:
	/**
	 * The feedback area list.
	 */
    TTactileFeedbackArea	iFeedbackAreaArray[KMaxNum];
	
	/**
	 * count of the area.
	 */	
    TInt iAreaCount;	
	
	/**
	 * command op tells whether register or deregister area.
	 */
	TFeedbackAreaOpEnum iCmdOp;
	
	/**
	 * Flags tells whether this command is send by Command or CommandReply.
	 */	
	TBool iCmdReplyFlag;
	};
#endif // RD_TACTILE_FEEDBACK
	
class TAnimEnableSpriteCmd :  public TAnimCmd
    {
public:
    TAnimEnableSpriteCmd(RPeninputAnim& aAnim,TBool aFlag);
	/**
     * Execute the command
     *
     * @since S60 v4.0
	 * @return ETrue if command has been executed successfully.     
     */     
    virtual TBool ExecuteAnimCommand() const;
private:
    TBool iSpriteFlag;    
    };
	
#include "peninputanimcommand.inl"
	
#endif //C_CPENINPUTANIM_CMD_H



