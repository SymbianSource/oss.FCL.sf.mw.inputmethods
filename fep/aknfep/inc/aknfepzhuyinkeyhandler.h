/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:                  
*
*/



#ifndef C_AKNFEPZHUYINKEYHANDLER_H
#define C_AKNFEPZHUYINKEYHANDLER_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib
#include <PtiDefs.h>
#include "aknfepzhuyinkeyhandlerinterface.h"

class ZhuyinKeyTable
    {
public:
    /**
     *  C++ constructor.
     * 
     * @return state.
     */
    ZhuyinKeyTable();

    /**
     *  Cancel and destroy
     * 
     * @return state.
     */
    ~ZhuyinKeyTable();
    
    /**
     *  Get butten loop length.
     * 
     * @parameter aKey the input key.
     * @return loop length.
     */
    static TInt GetBtLoopLength( TPtiKey aKey );
    
    /**
     *  Get zhuyin symbol.
     * 
     * @parameter aKey the input key
     * @parameter aClkCount input key count
     * @parameter aOutput the zhuyin symbol
     * @return state.
     */
    static TBool GetUnicode( TPtiKey aKey, TInt aClkCount, TDes& aOutput );
    
    };

class CZhuyinKeyHandler : public CActive, public MZhuyinKeyHandler
    {
public:
     
    /**
     *  Cancel and destroy
     * 
     * @return state.
     */
    ~CZhuyinKeyHandler();

    /**
     *  Two-phased constructor.
     * 
     * @return state.
     */
    static CZhuyinKeyHandler* NewL();

    /**
     *  Two-phased constructor.
     * 
     * @return state.
     */
    static CZhuyinKeyHandler* NewLC();

public:

    /**
     * Function for making the initial request.
     * 
     * @return state.
     */
    void Start();
    
    /**
     * Get Zhuyin symbol according to the input key.
     * 
     * @param aKey. the input key.
     * @param aOutBuf. an output parameter which will receive the zhuyin symbol unicode.
     * @return 0 identify the output should replace the old symbol.
     *         1 identify the output is a new symbol.
     *        -1 identify error
     */
    TInt GetZhuyinSymbol( TPtiKey aKey, TDes& aOutBuf );
    
    /**
     * Reset all the member variables.
     * @param None.
     * @return None.
     */
    void Reset();
    
    /**
     * Get tonemark according to symbol in front of cursor.
     * 
     * @param. aBaseSymbol is the symbol in front of cursor.
     * @param. aOutBuf is the output buffer for rcving tonemark.
     * @return. 0 identify the output should replace symbol in front of cursor
     *          1 identify the output should be added.
     *         -1 identify error
     */
    TInt GetToneMark( const TDes& aBaseSymbol, TDes& aOutBuf );
    
    /**
     * Set whether state change from entry to spelling editing is automatical.
     * 
     * @param aState.1 for automatical, 0 for manual.
     * @return None.
     */
    void SetState( TInt aState );
    
    /**
     * Get state change states.
     * 
     * @return state.
     */
    TInt GetState();
    
    /**
     * Set status change flag. 
     * this function is used to deal with long press on Down key or Clear key.
     * 
     * @param aChgFlag.
     * @return None.
     */
    void SetStatusChgFlag( TStatusChgFlag aChgFlag );
    
    /**
     * Get the status change flag.
     * @prama None.
     * @return.
     */
    TStatusChgFlag GetStatusChgFlag();
    
private:
    
    /**
     * C++ constructor.
     * @prama None.
     * @return.
     */
    CZhuyinKeyHandler();

    /**
     * Second-phase constructor.
     * @prama None.
     * @return.
     */
    void ConstructL();

private:

    /**
     * From CActive Handle completion.
     * @prama None.
     * @return.
     */
    void RunL();

    /**
     * How to cancel me.
     * @prama None.
     * @return.
     */
    void DoCancel();

    /**
     * Override to handle leaves from RunL(). Default implementation causes.
     * the active scheduler to panic.
     * @prama None.
     * @return.
     */
    TInt RunError( TInt aError );

private:
    
    /**
     * Provides async timing service
     */
    RTimer iTimer;
    
    /**
     * Last pressed key.
     */
    TPtiKey iLastKey;
    
    /**
     * if ETrue, it identify you don't touch any key in the specified time.
     */
    TBool iTimeOut;
    /**
     * the times you pressed current key.
     */
    TInt iBtClickNum;
    
    /**
     * the max loop length for pressed key.
     */
    TInt iBtLoopLength;
    
    /**
     * whether the state change from entry to spelling editing is automatical.
     */
    TInt iState;
    
    /**
     * state change flag.used to handle long press on down key and clear key currently.
     */
    TStatusChgFlag iStatusChgFlag;
    
    };

#endif // C_AKNFEPZHUYINKEYHANDLER_H

// End of file

