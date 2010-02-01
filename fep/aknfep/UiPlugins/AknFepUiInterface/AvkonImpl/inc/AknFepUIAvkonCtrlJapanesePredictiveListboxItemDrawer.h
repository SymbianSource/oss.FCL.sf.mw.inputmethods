/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Predictive Candidate list
*
*/












#ifndef CAKNFEPUICTRLJAPANESEPREDICTIVELISTBOXITEMDRAWER_H
#define CAKNFEPUICTRLJAPANESEPREDICTIVELISTBOXITEMDRAWER_H

// INCLUDES

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  CAknFepUICtrlJapanesePredictiveListboxItemDrawer draws a single list item
*  and manages list item properties.
*
*  @lib AknFep.lib
*  @since 2.6
*/
NONSHARABLE_CLASS(CAknFepUICtrlJapanesePredictiveListboxItemDrawer)
        : public CFormattedCellListBoxItemDrawer
    {
    public:  // Constructors and destructor
        /**
        * Constructors
        * @since 2.6
        * @param aTextListBoxModel
        * @param aFont
        * @param aFormattedCellData
        */
        CAknFepUICtrlJapanesePredictiveListboxItemDrawer(
                                    MTextListBoxModel* aTextListBoxModel,
                                    const CFont* aFont,
                                    CFormattedCellListBoxData* aFormattedCellData);

    public: // New functions
        /**
        * Drawing listbox items
        * @since 2.6
        * @param aItemIndex         a item index to draw
        * @param aItemRectPos       a rect to draw
        * @param aItemIsSelected    a flag to select
        * @param aItemIsCurrent     a flag to current
        * @param aViewIsEmphasized  
        * @param aViewIsDimmed      
        * @param aHighlightStartXPos    a starting x-position to hightlight
        * @param aHighlightEndXPos      a ending x-position to hightlight
        */
        void DrawItem(TInt aItemIndex,
                      TPoint aItemRectPos,
                      TBool aItemIsSelected,
                      TBool aItemIsCurrent,
                      TBool aViewIsEmphasized,
                      TBool aViewIsDimmed,
                      TInt aHighlightStartXPos,
                      TInt aHighlightEndXPos) const;
    };

#endif  // CAKNFEPUICTRLJAPANESEPREDICTIVELISTBOXITEMDRAWER_H

// End of File
