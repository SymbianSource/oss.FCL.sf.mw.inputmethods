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
* Description:  drop-down list control
*
*/


#ifndef C_SPLITITUTUIMGRBASE_H
#define C_SPLITITUTUIMGRBASE_H

#include <e32base.h>
#include <PtiDefs.h>

class CSplitItutUiStateBase;
class MSplitItutLayoutContext;
class CSplitItutDataMgr;
class CFepUiBaseCtrl;
class CSplitItutWindowManager;

/**
 *  class CSplitItutUiMgrBase.
 *
 *  Split itut ui manager base class.
 *
 *  @lib peninputsplititut.lib
 *  @since S60 v5.0
 */
class CSplitItutUiMgrBase : public CBase
    {
public:
    enum TUiState
        {
        EStateNull,
        // chinese ui state
        EStateStandby,
        EStateComposition,
        EStatePredict,
        // western ui state
        EStateWesternStandby,
        EStateSpelling
        };

public:
    /**
     * standard c++ destructor.
     *
     * @since S60 v5.0
     * @return None
     */
    virtual ~CSplitItutUiMgrBase();

    /**
     * handle command
     *
     * @since S60 v5.0
     * @param aCmd
     * @param aData
     * @return TBool
     */
    virtual TInt HandleCommandL(TInt aCmd, TUint8* aData);

    /**
     * handle control event
     *
     * @since S60 v5.0
     * @param aEventType
     * @param aCtrl
     * @param aEventData
     * @return TBool
     */
    virtual TBool HandleCtrlEventL( TInt aEventType, 
                                    CFepUiBaseCtrl* aCtrl, 
                                    const TDesC& aEventData);

    /**
     * set current state
     *
     * @since S60 v5.0
     * @param aNewState
     * @return None
     */
    virtual void SetCurrentStateL(TUiState aNewState) = 0;

    /**
     * active ui
     *
     * @since S60 v5.0
     * @return None
     */
    virtual void ActivateUiL() = 0;

    /**
     * deactive ui
     *
     * @since S60 v5.0
     * @return None
     */
    virtual void DeactivateUI() = 0;
    
    /**
     * handle long press number key
     *
     * @since S60 v5.0
     * @param aKey
     * @return None
     */
    virtual void HandleLongPressNumberKeyL(TPtiKey aKey);

    /**
     * Set number long press valit
     *
     * @since S60 v5.0
     * @param aValid
     * @return None
     */
    inline void SetNumLongPressValid(TBool aValid);

    /**
     * get number long press valit
     *
     * @since S60 v5.0
     * @return TBool
     */
    inline TBool NumLongPressValid();

    /**
     * get layout context
     *
     * @since S60 v5.0
     * @return layout context
     */
    inline MSplitItutLayoutContext* LayoutContext();

    /**
     * get data mgr
     *
     * @since S60 v5.0
     * @return data mgr
     */
    inline CSplitItutDataMgr* DataMgr();

    /**
     * get current state
     *
     * @since S60 v5.0
     * @return current state
     */
    inline CSplitItutUiStateBase* CurrentState();
    
    /**
     * is control consumed
     *
     * @since S60 v5.0
     * @return TBool
     */
    inline TBool ClrConsumed();
    
    /**
     * set control consumed
     *
     * @since S60 v5.0
     * @param aConsume
     * @return None
     */
    inline void SetClrConsume(TBool aConsume);
    
    /**
     * get ui manager
     *
     * @since S60 v5.0
     * @return UI mgr
     */
    inline CSplitItutWindowManager* UiManager();

protected:
    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @return None
     */
    CSplitItutUiMgrBase(MSplitItutLayoutContext* aContext,
                          CSplitItutDataMgr* aDataMgr,
                          CSplitItutWindowManager* aUiManager);
      
    /**
     * Symbian second-phase constructor
     *
     * @since S60 v5.0
     * @return None
     */
    virtual void ConstructL();

protected:

    /**
     * The layout context
     * not own
     */
    MSplitItutLayoutContext* iLayoutContext;

    /**
     * The data mgr
     * not own
     */
    CSplitItutDataMgr* iDataMgr;

    /**
     * current state
     * not own
     */
    CSplitItutUiStateBase* iCurrentState;

    /**
     * number long press valid
     */
    TBool iNumLongPressValid;
    
    /**
     * control consume
     */
    TBool iClrConsume;
    
    /**
     * UI mgr
     * not own
     */
    CSplitItutWindowManager* iUiMgr;
    };

// ---------------------------------------------------------------------------
// CSplitItutUiMgrBase::LayoutContext
// ---------------------------------------------------------------------------
//
inline MSplitItutLayoutContext* CSplitItutUiMgrBase::LayoutContext()
    {
    return iLayoutContext;
    }

// ---------------------------------------------------------------------------
// CSplitItutUiMgrBase::DataMgr
// ---------------------------------------------------------------------------
//
inline CSplitItutDataMgr* CSplitItutUiMgrBase::DataMgr()
    {
    return iDataMgr;
    }

// ---------------------------------------------------------------------------
// CSplitItutUiMgrBase::CurrentState
// ---------------------------------------------------------------------------
//
inline CSplitItutUiStateBase* CSplitItutUiMgrBase::CurrentState()
    {
    return iCurrentState;
    }
    
// ---------------------------------------------------------------------------
// CSplitItutUiMgrBase::SetNumLongPressValid
// ---------------------------------------------------------------------------
//  
inline void CSplitItutUiMgrBase::SetNumLongPressValid(TBool aValid)
    {
    iNumLongPressValid = aValid;
    }

// ---------------------------------------------------------------------------
// CSplitItutUiMgrBase::NumLongPressValid
// ---------------------------------------------------------------------------
//  
inline TBool CSplitItutUiMgrBase::NumLongPressValid()
    {
    return iNumLongPressValid;
    }

// ---------------------------------------------------------------------------
// CSplitItutUiMgrBase::ClrConsumed
// ---------------------------------------------------------------------------
//  
inline TBool CSplitItutUiMgrBase::ClrConsumed()
    {
    return iClrConsume;
    }

// ---------------------------------------------------------------------------
// CSplitItutUiMgrBase::SetClrConsume
// ---------------------------------------------------------------------------
//  
inline void CSplitItutUiMgrBase::SetClrConsume(TBool aConsume)
    {
    iClrConsume = aConsume;
    }

// ---------------------------------------------------------------------------
// CSplitItutUiMgrBase::UiManager
// ---------------------------------------------------------------------------
//  
inline CSplitItutWindowManager* CSplitItutUiMgrBase::UiManager()
    {
    return iUiMgr;   
    }

#endif // C_SPLITITUTUIMGRBASE_H
