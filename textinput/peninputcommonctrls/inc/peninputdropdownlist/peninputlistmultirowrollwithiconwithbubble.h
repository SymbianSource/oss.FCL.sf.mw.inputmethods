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


#ifndef C_CLISTMULTIROWROLLWITHICONWITHBUBBLE_H
#define C_CLISTMULTIROWROLLWITHICONWITHBUBBLE_H

#include "peninputlistmultirowrollwithicon.h"

// forward declaration
class MFepCtrlDropdownListContext;

NONSHARABLE_CLASS(CListMultiRowRollWithIconWithBubble) : public CListMultiRowRollWithIcon
    {
public: // Methods

    static CListMultiRowRollWithIconWithBubble* NewL(CListManager* aManager, 
                                           MFepCtrlDropdownListContext* iOwner);
    
    virtual ~CListMultiRowRollWithIconWithBubble();

    virtual void HandlePointerDownL(const TPoint& aPoint);

    virtual void HandlePointerUpL(const TPoint& aPoint);
    virtual void HandlePointerDragL(const TPoint& aPoint);

    virtual TBool CompareRect(const TRect& aSrcRect, const TRect& aDesRect);
protected: // Methods

    CListMultiRowRollWithIconWithBubble(CListManager* aManager, MFepCtrlDropdownListContext* aOwner);


    };

#endif // C_CLISTMULTIROWROLLWITHICONWITHBUBBLE_H