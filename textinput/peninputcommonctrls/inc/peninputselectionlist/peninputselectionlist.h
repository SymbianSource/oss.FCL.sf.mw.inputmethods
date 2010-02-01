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
#ifndef C_CPENINPUTSELECTIONLIST_H
#define C_CPENINPUTSELECTIONLIST_H

#include <peninputlayoutbasecontrol.h>
#include <AknLayout2Def.h>
#include <AknsConstants.h>

class CPenInputSelectionList : public CFepUiBaseCtrl
    {
public:
    struct SItem
        {
        SItem(TRect aRect, const TDesC& aText)
            :iRect(aRect), iText(aText)
            {
            }

        TRect iRect;
        const TDesC& iText;
        };    

    enum TSelectionListBmpType
        {
        EBmpNormal,
        EBmpNormalMask,
        EBmpHighlight,
        EBmpHighlightMask,
        EBmpLastType = EBmpHighlightMask
        };

public:
    IMPORT_C static CPenInputSelectionList* NewL(CFepUiLayout* aUiLayout,
                                                 TInt aControlId,
                                                 TPoint aTopLeftPoint,
                                                 TSize aItemSize,
                                                 TInt aItemHorizontalMargin);

    IMPORT_C ~CPenInputSelectionList();

    IMPORT_C void SizeChanged(TPoint aTopLeftPoint,
                              TSize aItemSize,
                              TInt aItemHorizontalMargin);
    
    // control rect is calculated by item count
    IMPORT_C void SetItemsL(const RPointerArray<HBufC>& aItemList);

    IMPORT_C void AddItemL(const TDesC& aItem);

    IMPORT_C void RemoveItem(TInt aItemIndex);
    
    IMPORT_C void RemoveAllItems();

    IMPORT_C void ConstructFromResourceL();

    IMPORT_C void SetTextProperty(const CFont* aFont,
                                  TRgb aFontColor);

    IMPORT_C CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
    IMPORT_C CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);
    IMPORT_C CFepUiBaseCtrl* HandlePointerMoveEventL(const TPoint& aPoint);

    IMPORT_C void Display();

    IMPORT_C void CloseWindow();

    inline void SetTextLineLayout(TAknTextLineLayout aTextLine);
    
    inline void SetEventIdForItemSelected(TInt aEventId);
    
    inline void SetBgFrId(TAknsItemID aFrId, TAknsItemID aCenterFrId);
    
    void Draw();

private:

    CPenInputSelectionList(CFepUiLayout* aUiLayout,
                           TInt aControlId,
                           TPoint aTopLeftPoint,
                           TSize aItemSize,
                           TInt aItemHorizontalMargin);

    void ConstructL();

    void RecalculateCtrlRect();

    // get item whose region contains the point
    TInt HitTest(const TPoint& aPoint);

    void DrawItem(TInt aItemIndex);

	void CalculateFrameRects(const TRect aRect, TRect& aOuterRect, TRect& aInnerRect) const;
private:

    RArray<SItem> iItemList;
    TPoint iTopLeftPoint;
    TSize iItemSize;
    TInt iItemHorizontalMargin;
    TInt iBaselineOffset;
    const CFont* iFont;
    TRgb iFontColor;
    TAknTextLineLayout iTextLine;

    TInt iFocusItemIndex;

    RPointerArray<CFbsBitmap> iBmpList;
    
    TInt iEventId;
    TBool iTextLineSet;
    TAknsItemID iFrId;
    TAknsItemID iCenterFrId;
    };

inline void CPenInputSelectionList::SetTextLineLayout(TAknTextLineLayout aTextLine)
    {
    iTextLine = aTextLine;
    iTextLineSet = ETrue;
    }

inline void CPenInputSelectionList::SetEventIdForItemSelected(TInt aEventId)
    {
    iEventId = aEventId;
    }

inline void CPenInputSelectionList::SetBgFrId(TAknsItemID aFrId, TAknsItemID aCenterFrId)
    {
    iFrId = aFrId;
    iCenterFrId = aCenterFrId;
    }

#endif // C_CPENINPUTSELECTIONLIST_H