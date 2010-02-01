/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:               interface for peninput server event handler
*
*/
















#ifndef M_PENINPUTSERVEREVENTHANDLER_H
#define M_PENINPUTSERVEREVENTHANDLER_H

/**
 * MPeninputServerEventHandler
 *
 * Interface for server event handler
 *
 * @since S60 V4.0
 */
class MPeninputServerEventHandler
    {
public:
    /**
     * server event handler
     *
     * @since S60 v4.0
     * @param aEventId The event id
     * @return ETrue if request next server event. Otherwise EFalse
     */
    virtual TBool HandleServerEventL(TInt aEventId) = 0;
    };
    
//class MPenUiActivationHandler 
/**
 * Peninput UI handler. The handler is called when pen input UI is 
 * activated or deactivated.
 *
 * @lib peninputclient.lib
 * @since S60 v4.0 
 */
class MPenUiActivationHandler 
    {
public:
    /** 
     * Called when pen input UI is about to be open
     *
     * @since S60 v4.0
     */    
    virtual void OnPeninputUiDeactivated() = 0;
    
    /** 
     * Called when pen input UI is about to be clsed
     *
     * @since S60 v4.0
     */        
    virtual void OnPeninputUiActivated() = 0;        
    };
    
#endif //M_PENINPUTSERVEREVENTHANDLER_H
