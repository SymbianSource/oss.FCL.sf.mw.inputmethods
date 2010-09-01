/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the MAknFepUICtrlCandidatePaneInterface definition.
*
*/












#ifndef __AKN_FEP_UI_CTRL_CANDIDATEPANE_INTERFACE_H__
#define __AKN_FEP_UI_CTRL_CANDIDATEPANE_INTERFACE_H__

#include <e32std.h>
#include <badesca.h> //CDesCArrayFlat

enum TAknFepUICtrlScroll
    {
    EScrollNone,
    EScrollLeft,
    EScrollRight,
    EScrollUp,
    EScrollDown
    };


class MAknFepUICtrlCandidatePaneInterface
    {
public:
    /**
     * Destructor.
     */
    virtual ~MAknFepUICtrlCandidatePaneInterface() {};
    /**
     * Show Candidate List
     *
     * @param   aItems      The disposition of an item
     * @param   aIndex      The index of the cursor position when an initial display
     * @param   aLeftSideOfBaseLine
     * @param   aHeight     
     * @param   aAscent
     */
    virtual void ShowCandidateListL(CDesCArrayFlat* aItems,
                                    TInt aIndex, TInt aResourceID) = 0;
    /**
     * Close Candidate List
     */
    virtual void HideWindow() = 0;
    /**
     * 
     */
    virtual TInt CurrentItemIndexOfCandidate() = 0;
    /**
     * 
     */
    virtual TInt MoveItem(TAknFepUICtrlScroll aDirection) = 0;
    /**
     * 
     */
    virtual void SetWindowPosition(const TPoint& aLeftSideOfBaseLine, TInt aHeight, TInt aAscent) = 0;
    };

#endif

// End of file
