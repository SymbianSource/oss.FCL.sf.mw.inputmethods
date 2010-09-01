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
#ifndef C_CLISTMULTIROWROLLWITHICON_H
#define C_CLISTMULTIROWROLLWITHICON_H

#include "peninputlistmultirowwithicon.h"

// forward declaration
class MFepCtrlDropdownListContext;

NONSHARABLE_CLASS(CListMultiRowRollWithIcon) : public CListMultiRowWithIcon
    {
public: // Methods

    static CListMultiRowRollWithIcon* NewL(CListManager* aManager, 
                                           MFepCtrlDropdownListContext* iOwner);
    
    virtual ~CListMultiRowRollWithIcon();

    // txin
    virtual void HandlePointerDownL(const TPoint& aPoint);
    // end

    virtual void HandlePointerUpL(const TPoint& aPoint);

    virtual void SetNextPageHighlightCellAfterOnePageL(const TInt aCellIndex);
    
    virtual void UpdateListL(TBool aDrawFlag = ETrue, TBool aRecalculate = ETrue);

protected: // Methods

    CListMultiRowRollWithIcon(CListManager* aManager, MFepCtrlDropdownListContext* aOwner);

protected:

    typedef struct TLineCandidateTag
        {
        TInt iStartIdx;
        TInt iEndIdx;
        } TLineCandidate;

    TInt StartCandIdxOfLine(TInt aCandIndex);
    
    void ResetStatus();

private:

    virtual TInt CalculatePositionsL();
    
private: // Data member

    // added by txin
    RArray<TLineCandidate> iLines;
    // end adding
    };

#endif // C_CLISTMULTIROWROLLWITHICON_H