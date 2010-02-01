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


#ifndef C_SPLITITUTUISTATEBASE_H
#define C_SPLITITUTUISTATEBASE_H

#include <e32base.h>
#include <ptidefs.h>

#include "peninputsplititutdata.h"
#include "peninputsplititutuimgrbase.h"

class MSplitItutLayoutContext;
class CSplitItutDataMgr;
class CFepUiBaseCtrl;

/**
 *  class CSplitItutUiStateBase.
 *
 *  split itut ui sate base.
 *
 *  @lib peninputsplititut.lib
 *  @since S60 v5.0
 */
class CSplitItutUiStateBase : public CBase
    {
public:
    /**
     * standard c++ destructor.
     *
     * @since S60 5.0
     * @return none
     */
    virtual ~CSplitItutUiStateBase();

    /**
     * exit current state.
     *
     * @since S60 5.0
     * @return none
     */
    virtual void OnExit() =  0;

    /**
     * entry current state.
     *
     * @since S60 5.0
     * @return none
     */
    virtual void OnEntryL() = 0;

    /**
     * handle control event.
     *
     * @since S60 5.0
     * @param aEventType
     * @param aCtrl
     * @param aEventData
     * @return TBool
     */
    virtual TBool HandleCtrlEventL( TInt aEventType, CFepUiBaseCtrl* aCtrl, 
                                    const TDesC& aEventData);

    /**
     * get state type.
     *
     * @since S60 5.0
     * @return state type
     */
    virtual CSplitItutUiMgrBase::TUiState StateType() = 0;

    /**
     * handle key event
     *
     * @since S60 5.0
     * @param aCmd
     * @param aKey
     * @return TBool
     */
    virtual TBool HandleKeyL(TInt aCmd, TInt aKey);
    
    /**
     * handle command
     *
     * @since S60 5.0
     * @param aCmd
     * @param aData
     * @return TInt
     */
    virtual TInt HandleCommandL(TInt aCmd, TUint8* aData);

protected:
    /**
     * C++ constructor
     *
     * @since S60 5.0
     * @param aOwner
     * @return none
     */
	CSplitItutUiStateBase(CSplitItutUiMgrBase* aOwner);

    /**
     * Symbian second-phase constructor
     *
     * @since S60 5.0
     * @return none
     */
    virtual void ConstructL();

protected: 

    /**
     * ui manager
     * not own
     */
    CSplitItutUiMgrBase* iOwner;
    };

#endif //C_SPLITITUTUISTATEBASE_H
