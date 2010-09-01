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


#ifndef C_CLISTONEROWROLLWITHICONWITHBUBBLE_H
#define C_CLISTONEROWROLLWITHICONWITHBUBBLE_H

#include "peninputlistonerowrollwithicon.h"

NONSHARABLE_CLASS(CListOneRowRollWithIconWithBubble) : public CListOneRowRollWithIcon
    {

public: // Methods

    static CListOneRowRollWithIconWithBubble* NewL(CListManager* aManager,
                                         MFepCtrlDropdownListContext* iOwner);

    virtual ~CListOneRowRollWithIconWithBubble();

    // not needed, dim next button same as its base class
    // virtual void Draw();

    virtual void HandlePointerDownL(const TPoint& aPoint);

    virtual void HandlePointerUpL(const TPoint& aPoint);

    virtual void HandlePointerDragL(const TPoint& aPoint);

    virtual void HandlePointerLeave(const TPoint& aPoint);
    virtual TBool CompareRect(const TRect& aSrcRect, const TRect& aDesRect);

protected: // Methods

    CListOneRowRollWithIconWithBubble(CListManager* aManager, MFepCtrlDropdownListContext* aOwner);
    };

#endif // C_CLISTONEROWROLLWITHICON_H