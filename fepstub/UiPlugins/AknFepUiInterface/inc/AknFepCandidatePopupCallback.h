/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:             A callback interface for the candidate popup
*
*/












#ifndef __AKN_FEP_CANDIDATE_POPUP_CALLBACK_H
#define __AKN_FEP_CANDIDATE_POPUP_CALLBACK_H

class MAknFepCandidatePopupCallback
    {
public:
    /**
    * Callback the get the current set of candidates
    * @param    aArray      An output parameter for the candidate strings
    * @param    aActiveIdx  An output parameter for the index of the 
    *                       currently active candidate
    */
    virtual void GetCandidatesL( CDesCArray& aArray, TInt& aActiveIdx ) = 0;  
    
    /**
     * Callback get current position of candidate list.
     * @param  aRect  New posion of candidate list
     */
    virtual void GetUpdateCandidatePositionL(TRect& aRect) = 0;
    };
    
#endif // __AKN_FEP_CANDIDATE_POPUP_CALLBACK_H
