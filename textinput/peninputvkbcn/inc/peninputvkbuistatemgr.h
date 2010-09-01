/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface of vkb ui state manager
*
*/


#ifndef C_AKNFEPVKBUISTATEMGR_H
#define C_AKNFEPVKBUISTATEMGR_H

// system includes
#include <e32base.h>
#include <w32std.h>

// forward declarations
class MAknFepVkbUiState;
class MAknFepVkbLayoutContext;
class CAknFepVkbUiState;
class CAknFepVkbUiStateStandby;
class CAknFepVkbUiStateStandbycn;
class CAknFepVkbUiStateStandbycnWithChars;
class CAknFepVkbUiStateCompositionNoChars;
class CAknFepVkbUiStateCompositionWithChars;

/**
 *  The interface definition of vkb ui state manager
 *  It provides get method of all vkb ui state and current ui state
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2
 */
class MAknFepVkbUiStateMgr
    {
public:

    /**
     * Get current vkb ui state
     *
     * @since S60 v3.2
     * @return The pointer points to Current vkb ui state
     */
    virtual MAknFepVkbUiState* CurrentUiState() = 0;
    
    /**
     * Set current vkb ui state
     *
     * @since S60 v3.2
     * @param aUiState The state to be set as current ui state
     * @return None
     */
    virtual void SetCurrentUiState(MAknFepVkbUiState* aUiState) = 0;
    
    /**
     * Get initial ui state
     *
     * @since S60 v3.2
     * @return The pointer points to initial ui state
     */
    virtual MAknFepVkbUiState* UiStateInitial() = 0;
    
    /**
     * Get standby ui state
     *
     * @since S60 v3.2
     * @return The pointer points to standby ui state
     */
    virtual MAknFepVkbUiState* UiStateStandby() = 0;
    
    /**
     * Get standby cn ui state
     *
     * @since S60 v3.2
     * @return The pointer points to standby cn ui state
     */
    virtual MAknFepVkbUiState* UiStateStandbycn() = 0;
    
    /**
     * Get standby cn with chars ui state
     *
     * @since S60 v3.2
     * @return The pointer points to standby cn with chars ui state
     */
    virtual MAknFepVkbUiState* UiStateStandbycnWithChars() = 0;
    
    /**
     * Get composition no chars ui state
     *
     * @since S60 v3.2
     * @return The pointer points to composition no chars ui state
     */
    virtual MAknFepVkbUiState* UiStateCompositionNoChars() = 0;
    
    /**
     * Get composition with chars ui state
     *
     * @since S60 v3.2
     * @return The pointer points to composition with chars ui state
     */
    virtual MAknFepVkbUiState* UiStateCompositionWithChars() = 0;
    };

/**
 *  The vkb ui state manager
 *  It provides get method of all vkb ui state and current ui state
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2
 */
class CAknFepVkbUiStateMgr: public CBase, MAknFepVkbUiStateMgr
    {
public:

    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aContext The vkb layout context
     * @return The pointer points to CAknFepVkbUiStateMgr type object
     */
    static CAknFepVkbUiStateMgr* NewL(MAknFepVkbLayoutContext* aContext);
    
    static CAknFepVkbUiStateMgr* NewLC(MAknFepVkbLayoutContext* aContext);

    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    virtual ~CAknFepVkbUiStateMgr();
    
    // from class MAknFepVkbUiStateMgr
    /**
     * Get current vkb ui state
     *
     * @since S60 v3.2
     * @return The pointer points to Current vkb ui state
     */
    MAknFepVkbUiState* CurrentUiState();
    
    /**
     * Set current vkb ui state
     *
     * @since S60 v3.2
     * @param aUiState The state to be set as current ui state
     * @return None
     */
    void SetCurrentUiState(MAknFepVkbUiState* aUiState);
    
    /**
     * Get initial ui state
     *
     * @since S60 v3.2
     * @return The pointer points to initial ui state
     */
    virtual MAknFepVkbUiState* UiStateInitial();
    
    /**
     * Get standby ui state
     *
     * @since S60 v3.2
     * @return The pointer points to standby ui state
     */
    virtual MAknFepVkbUiState* UiStateStandby();
    
    /**
     * Get standby cn ui state
     *
     * @since S60 v3.2
     * @return The pointer points to standby cn ui state
     */
    virtual MAknFepVkbUiState* UiStateStandbycn();
    
    /**
     * Get standby cn with chars ui state
     *
     * @since S60 v3.2
     * @return The pointer points to standby cn with chars ui state
     */
    virtual MAknFepVkbUiState* UiStateStandbycnWithChars();
    
    /**
     * Get composition no chars ui state
     *
     * @since S60 v3.2
     * @return The pointer points to composition no chars ui state
     */
    virtual MAknFepVkbUiState* UiStateCompositionNoChars();
    
    /**
     * Get composition with chars ui state
     *
     * @since S60 v3.2
     * @return The pointer points to composition with chars ui state
     */
    virtual MAknFepVkbUiState* UiStateCompositionWithChars();
                                        
protected:

    /**
     * Symbian second phase constructor.
     *
     * @since S60 v3.2
     * @param aContext The vkb layout context
     * @return None
     */
    void ConstructL(MAknFepVkbLayoutContext* aContext);  

    /**
     * C++ default constructor.
     *
     * @since S60 v3.2
     * @param aContext The vkb layout context
     */
    CAknFepVkbUiStateMgr(MAknFepVkbLayoutContext* aContext);
    
private: // Data

    /**
     * The pointer points to current ui state (Owned)
     */
    MAknFepVkbUiState* iCurrentState;
    
    /**
     * The pointer points to initial ui state (Owned)
     */
    CAknFepVkbUiState* iUiStateInitial;

    /**
     * The pointer points to standby ui state (Owned)
     */
    CAknFepVkbUiStateStandby* iUiStateStandby;

    /**
     * The pointer points to standby cn ui state (Owned)
     */
    CAknFepVkbUiStateStandbycn* iUiStateStandbycn;

    /**
     * The pointer points to standby with chars ui state (Owned)
     */
    CAknFepVkbUiStateStandbycnWithChars* iUiStateStandbyWitchChars;

    /**
     * The pointer points to composition no chars ui state (Owned)
     */
    CAknFepVkbUiStateCompositionNoChars* iUiStateCompositionNoChars;

    /**
     * The pointer points to composition with chars ui state (Owned)
     */
    CAknFepVkbUiStateCompositionWithChars* iUiStateCompositionWithChars;  
      
    /**
     * The pointer points to layout context (Not owned)
     */
    MAknFepVkbLayoutContext* iContext;
    };

#endif // C_AKNFEPVKBUISTATEMGR_H

// End Of File
