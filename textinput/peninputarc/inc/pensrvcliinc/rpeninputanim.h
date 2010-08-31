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
* Description:  Interface to animation
*
*/


#ifndef R_RPENINPUTANIM_H
#define R_RPENINPUTANIM_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <w32std.h>
#include <peninputcmd.h>
#include "peninputcmdparamext.h"
/**
 * Class for implementing animation functinality. 
 *
 * This is the client side interface to the CSpriteAnim derived server side class.
 *
 * @since S60 v4.0
 */
class RPeninputAnim : public RAnim
    {
public:
    // Constructors and destructor

    /**
     * Default constructor. This is defined just to allow RPeninputAnim
     * to be defined as non-pointer member. Use the other overloaded
     * constructor for actual initialization.
     *
     * @since S60 v4.0
     */
    RPeninputAnim();

    /**
     * Constructor.
     *
     * @since S60 v4.0
     * @param aDll reference to loaded RAnimDll
     */
     RPeninputAnim( RAnimDll& aDll );

    /**
     * Second phase construction.
     *
     * @since S60 v4.0
     * @param aDevice reference to window server sprite.
     */
    void ConstructL( const RWsSprite& aDevice );

    /**
     * Activates the sprite ie. makes it visible.
     *
     * @since S60 v4.0
     */
    void Activate();

    /**
     * De-activates the sprite ie. makes it invisible.
     *
     * @since S60 v4.0
     */
    void Deactivate();


    /**
     * Inform pen input animation thata the UI layout size has been changed
     *
     * @since S60 v4.0
     * @param aSize The new layout size
     * @return The operation code
     */
    TInt LayoutSizeChangedL(const TSize& aSize);        

    /**
     * set ui layout postion 
     *
     * @since S60 v4.0
     * @param aPt The layout position
     * @return The operation result
     */
    TInt SetUiLayoutPos(const TPoint& aPt);

    
    /**
     * Infor animation that display mode changed
     *
     * @since S60 v4.0
     */
    void ChangeDisplayMode();

    /**
     * Update screen area
     *
     * @since S60 v4.0
     * @param aRect The area to be updated.
     * @param aFullUpdate The full-updating flag.
     */    
    void UpdateArea(const TRect& aRect,TBool aFullUpdate);

    /**
     * Update screen area
     *
     * @since S60 v4.0
     * @param aArea The array of area to be updated
     * @return None
     */    
    void UpdateArea(const RArray<TUpdateArea>& aArea);

    /**
     * Ask window server to capture the pointer
     *
     * @since S60 v4.0     
     * @param aFlag ETrue if need to capture the pointer
     */        
    void CapturePointer(TBool aFlag, TInt aCaptureCtrlID);
    
    /**
     * Ask animation simulate the event
     *
     * @since S60 v4.0     
     * @param aEvent The event to be simulated
     */            
    void SimulateEvent(const TRawEvent& aEvent);

#ifdef RD_TACTILE_FEEDBACK

    /**
     * Add feedback area. Send command to pen input animation
     *
     * @since S60 v4.0     
     * @param aArea The area data
     * @param aCount area num
     * @param Flag tells whether the command will be bufferred.
     */                
    void AddFeedbackAreaL(const TTactileFeedbackArea* aArea, TInt aCount,TBool aFlag);
    
    /**
     * Remove feedback area. Send command to pen input animation
     *
     * @since S60 v4.0     
     * @param aArea The area data
     * @param aCount area num
     * @param Flag tells whether the command will be bufferred.
     */                    
    void RemoveFeedbackAreaL(const TTactileFeedbackArea* aArea, TInt aCount,TBool aFlag);
    
    /**
     * Change feedback area. Send command to pen input animation
     *
     * @since S60 v4.0     
     * @param aArea The area data
     * @param aCount area num
     * @param Flag tells whether the command will be bufferred.
     */                    
    void ChangeFeedbackAreaL(const TTactileFeedbackArea* aArea, TInt aCount,TBool aFlag);
    
    /**
     * Change feedback type. Send command to pen input animation
     *
     * @since S60 v4.0     
     * @param aArea The area data
     * @param aCount area num
     * @param Flag tells whether the command will be bufferred.
     */  
    void ChangeFeedbackTypeL(const TTactileFeedbackArea* aArea, TInt aCount,TBool aFlag);

#endif // RD_TACTILE_FEEDBACK
    
    void EnableSprite(TBool aFlag);    
    
    void GetDSAState(TBool& aState);
private:

    /**
     * Completes the sprite anim construction.
     *
     * @since S60 v4.0
     */
    void FinishConstructionL();

#ifdef RD_TACTILE_FEEDBACK

    /**
     * Send feedback command to animation dll by Command
     *
     * @since S60 v4.0
     * @param aArea The area data
     * @param aCount The area number
     * @param aOp The command id
     */    
    void DoFeedbackAreaOpL(const TTactileFeedbackArea* aArea, TInt aCount,TPeninputOpcodes aOp);
    
    /**
     * Send feedback command to animation dll by CommandReplyL
     *
     * @since S60 v4.0
     * @param aArea The area data
     * @param aCount The area number
     * @param aOp The command id
     */        
    void DoFeedbackAreaOpReplyL(const TTactileFeedbackArea* aArea, 
                                TInt aCount,TPeninputOpcodes aOp);    
    
#endif // RD_TACTILE_FEEDBACK    
    };
#endif //R_RPENINPUTANIM_H

// End of File
