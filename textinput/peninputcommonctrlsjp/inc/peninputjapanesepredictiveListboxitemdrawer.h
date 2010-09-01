/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Predictive Candidate list drawer
*
*/


#ifndef CPENINPUTJAPANESEPREDICTIVELISTBOXITEMDRAWER_H
#define CPENINPUTJAPANESEPREDICTIVELISTBOXITEMDRAWER_H

// INCLUDES
#include <eikfrlb.h>  // CFormattedCellListBoxItemDrawer

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION
class CFbsBitmapDevice;

/**
*  CPeninputJapanesePredictiveListBoxItemDrawer draws a single list item
*  and manages list item properties.
*
*  @lib peninputcommonctrlsjp.lib
*  @since 2.6
*/
class CPeninputJapanesePredictiveListBoxItemDrawer
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
        CPeninputJapanesePredictiveListBoxItemDrawer(
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
                      const TPoint& aItemRectPos,
                      TBool aItemIsSelected,
                      TBool aItemIsCurrent,
                      TBool aViewIsEmphasized,
                      TBool aViewIsDimmed,
                      TInt aHighlightStartXPos,
                      TInt aHighlightEndXPos) const;

        /** 
         * Set Bitmap device for sprite
         * @param aBitmapDevice   Bitmap device for sprite
         **/
        void SetBitmapDevice(CFbsBitmapDevice* aBitmapDevice);

    private:
        /** 
         * Bitmap device for sprite
         * Not Own
         **/
        CFbsBitmapDevice* iBitmapDevice;
    };

#endif  // CPENINPUTJAPANESEPREDICTIVELISTBOXITEMDRAWER_H

// End of File
