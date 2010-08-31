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


#ifndef C_CPENINPUTANIMOBJ_H
#define C_CPENINPUTANIMOBJ_H

#include "rpeninputanim.h"

#ifdef RD_TACTILE_FEEDBACK
class TTactileFeedbackArea;
#endif // RD_TACTILE_FEEDBACK

class TAnimCmd; //animation command class

/**
 * An active object implementing high level interface for pen input animation
 * 
 *
 * @since S60 v4.0
 */
class CPeninputAnimObj : public CActive
	{
public:
    /**
     * factory creator. 
     *
     * @since S60 v4.0
     * @aSpriteMember The sprite member assigned to the sprite     
     */        
    static CPeninputAnimObj* NewL(TSpriteMember& aSpriteMember);    
   
    /**
     * Destructor
     *
     * @since S60 v4.0
     */
    ~CPeninputAnimObj();    

    /**
     * From CActive
     * Called when there is event in the queue
     *
     * @since S60 v4.0
     */
    void RunL();

    /**
     * From CActive
     * will be called if RunL leaves
     *
     * @since S60 v4.0
     * @param aError The error number
     */
    TInt RunError(TInt aError);

    /**
     * From CActive
     * will be called when Cancel is issued
     *
     * @since S60 v4.0
     */
    void DoCancel();

	//new function
    /**
     * Add UI activation command to AO scheduler
     *
     * @since S60 v4.0
     * @aUiFalg The ui activation flag
     * @aWaitFlag Flag indicates whther this command should be pended for user response
     * @return ETrue if command added successfully.
     */	
	TBool AddActivationCmd(TBool aUiFlag, TBool aPendingFlag);
	
    /**
     * Add UI size change command to AO scheduler
     *
     * @since S60 v4.0
     * @aSize The new UI size
     * @return ETrue if command added successfully.
     */		
	TBool AddChangeSizeCmd(const TSize& aSize);
	
    /**
     * Add set UI position command to AO scheduler
     *
     * @since S60 v4.0
     * @aPt The new UI position
     * @return ETrue if command added successfully.
     */			
	TBool AddSetPosCmd(const TPoint& aPt);
	
    /**
     * Add change sprite display mode command to AO scheduler
     *
     * @since S60 v4.0
     * @aMember The sprite member to be changed
     * @return ETrue if command added successfully.
     */			
	TBool AddChangeDispModeCmd(TSpriteMember& aMember);
	
    /**
     * Add update UI command to AO scheduler
     *
     * @since S60 v4.0
     * @aRect The rect to be updated
     * @aUpdateFlag The full sprite updating flag. ETrue if whole sprite is updated.
     * @return ETrue if command added successfully.
     */			
	TBool AddUpdateUiCmd(const TRect& aRect, TBool aUpdateFlag);
	
    /**
     *
     * @since S60 v4.0
     * @aArea The pointer which point to 1st TUpdateArea that need to be updated
     * @aNum The count of TUpdateArea that need to be udpated
     * @return ETrue if command added successfully.
     */
    TBool AddUpdateUiCmd(TUpdateArea *aArea, TInt aNum);

    /**
     * Add capture pointer command to AO scheduler
     *
     * @since S60 v4.0
     * @aFlag The capture flag
     * @return ETrue if command added successfully.
     */			
	TBool AddCapturePointerCmd(TBool aFlag, TInt aCaptureCtrlID);
	
	
    /**
     * Add simulate event command to AO scheduler
     *
     * @since S60 v4.0
     * @aEvent The event
     * @return ETrue if command added successfully.
     */			
	TBool AddSimulateEventCmd(const TRawEvent& aEvent);
	
    /**
     * Execute simulate event command
     *
     * @aEvent The event
     * @return ETrue if command added successfully.
     */			
	TBool ExecuteSimulateEventCmd( const TRawEvent& aEvent );	
	
    /**
     * Add flush window session command
     *
     * @since S60 v4.0
     * @return ETrue if command added successfully.
     */			
	TBool AddFlushSessionCmd();
	
    /**
     * Excecute one pending command. Command may be pending due to waiting 
     * for user response.
     *
     * @since S60 v4.0
     */		
	void ExeOnePendingAnimCmd();

#ifdef RD_TACTILE_FEEDBACK	
    /**
     * Add register feedback area command
     *
     * @since S60 v4.0
     * @param aArea The feedback area data.
     * @return ETrue if command added successfully.     
     */			
	TBool AddRegisterFeedbackAreaCmd(const TTactileFeedbackArea& aArea,TBool aCmdFlag);	

   // The API is not open yet
   // TBool AddRegisterFeedbackAreaCmd(const RArray<TTactileFeedbackArea>& aAreaList);
    /**
     * Add de-register feedback area command
     *
     * @since S60 v4.0
     * @param aArea The feedback area data.     
     * @return ETrue if command added successfully.     
     */			
	TBool AddDeRegisterFeedbackAreaCmd(const TTactileFeedbackArea& aArea,TBool aCmdFlag);	

    /**
     * Add register feedback area command
     *
     * @since S60 v4.0
     * @param aArea The feedback area data.
     * @return ETrue if command added successfully.     
     */			
	TBool AddChangeFeedbackAreaCmd(const TTactileFeedbackArea& aArea,TBool aCmdFlag);
	
    /**
     * Add register feedback area command
     *
     * @since S60 v4.0
     * @param aArea The feedback area data.
     * @return ETrue if command added successfully.     
     */			
	TBool AddChangeFeedbackTypeCmd(const TTactileFeedbackArea& aArea,TBool aCmdFlag);	
#endif // RD_TACTILE_FEEDBACK

    TBool AddEnalbeSpriteCmd(TBool aFlag);
    
    RWindowGroup& WindowGroup(); 
    
    void OnExiting();           
    
    void GetDSAState(TBool& aState);
private:
    /**
     * Default constructor.
     *
     * @since S60 v4.0
     */	
    CPeninputAnimObj(TSpriteMember& aSpriteMember);

    /**
     * 2nd phase constructor.
     *
     * @since S60 v4.0
     */	
    void ConstructL();

    /**
     * execute animation command.
     *
     * @since S60 v4.0
     */	
    void ExecuteAnimCommand();    
 
    /**
     * Add an animation command to scheduler.
     *
     * @since S60 v4.0
     */	 
    void AddAnimCommand(TAnimCmd* aCmd);
    
    /**
     * Set animation object active, let this object to be scheduled by AO scheduler.
     *
     * @since S60 v4.0
     */	    
    void SetObjActive();
    
    /**
     * construct pen input animation module.
     *
     * @since S60 v4.0
     */	    
    TInt ConstructAnimL();
    
    //RWindowGroup& WindowGroup();
    
private:
    /** 
     * Polymorphic sprite animation dll 
     */
    RAnimDll iAnimDll;

    /** 
     * Sprite animation client side interface 
     */
    RPeninputAnim iAnim;

    /** 
     * Sprite used by sprite anim 
     */
    RWsSprite iSprite;

    /** 
     * Sprite member belongs to the sprite. 
     */
    TSpriteMember& iSpriteMember;    

    /** 
     * Animation command array. 
     */
    RPointerArray<TAnimCmd> iAnimCmd;   
    
    /** 
     * Window server session  
     */
    RWsSession iWsSession;

    /** 
     * Window group for sprite anim 
     */
    RWindowGroup iWindowGroup; 
    
    TBool iOnExiting;   
	};

#endif //C_CPENINPUTANIMOBJ_H