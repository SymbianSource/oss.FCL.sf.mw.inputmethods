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


#ifndef C_GENERICITUTUIMGRBASE_H
#define C_GENERICITUTUIMGRBASE_H

#include <e32base.h>
#include <PtiDefs.h>

class CGenericItutUiStateBase;
class MItutLayoutContext;
class CGenericItutDataMgr;
class CFepUiBaseCtrl;
class CGenericItutWindowManager;

class CGenericItutUiMgrBase : public CBase
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
        EStateCustomization,
        EStateMatchSelection,
        EStateSpelling
        };

public:

    virtual ~CGenericItutUiMgrBase();

    // called by ui layout
    virtual TInt HandleCommandL(TInt aCmd, TUint8* aData);

    // handle control event
    virtual TBool HandleCtrlEventL( TInt aEventType, 
                                    CFepUiBaseCtrl* aCtrl, 
                                    const TDesC& aEventData);

    virtual void SetCurrentStateL(TUiState aNewState) = 0;

    virtual void ActivateUiL() = 0;

    virtual void DeactivateUI() = 0;
    
    virtual void HandleLongPressNumberKeyL(TPtiKey aKey);

    inline void SetNumLongPressValid(TBool aValid);

    inline TBool NumLongPressValid();

    inline MItutLayoutContext* LayoutContext();

    inline CGenericItutDataMgr* DataMgr();

    inline CGenericItutUiStateBase* CurrentState();
    
    inline TBool ClrConsumed();
    
    inline void SetClrConsume(TBool aConsume);
    
    inline CGenericItutWindowManager* UiManager();

protected:

    CGenericItutUiMgrBase(MItutLayoutContext* aContext,
                          CGenericItutDataMgr* aDataMgr,
                          CGenericItutWindowManager* aUiManager);
                          
    virtual void ConstructL();

protected:

    // not own
    MItutLayoutContext* iLayoutContext;

    // not own
    CGenericItutDataMgr* iDataMgr;

    CGenericItutUiStateBase* iCurrentState;

    TBool iNumLongPressValid;
    
    TBool iClrConsume;
    
    CGenericItutWindowManager* iUiMgr;
    };

inline MItutLayoutContext* CGenericItutUiMgrBase::LayoutContext()
    {
    return iLayoutContext;
    }

inline CGenericItutDataMgr* CGenericItutUiMgrBase::DataMgr()
    {
    return iDataMgr;
    }
    
inline CGenericItutUiStateBase* CGenericItutUiMgrBase::CurrentState()
    {
    return iCurrentState;
    }
    
inline void CGenericItutUiMgrBase::SetNumLongPressValid(TBool aValid)
    {
    iNumLongPressValid = aValid;
    }

inline TBool CGenericItutUiMgrBase::NumLongPressValid()
    {
    return iNumLongPressValid;
    }

inline TBool CGenericItutUiMgrBase::ClrConsumed()
    {
    return iClrConsume;
    }
    
inline void CGenericItutUiMgrBase::SetClrConsume(TBool aConsume)
    {
    iClrConsume = aConsume;
    }
    
inline CGenericItutWindowManager* CGenericItutUiMgrBase::UiManager()
    {
    return iUiMgr;   
    }

#endif // C_GENERICITUTUIMGRBASE_H