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
* Description:  Predictive Candidate list cellist data box
*
*/


#ifndef CPENINPUTJAPANESEPREDICTIVECELLLISTBOXDATA_H
#define CPENINPUTJAPANESEPREDICTIVECELLLISTBOXDATA_H

// INCLUDES
#include <eikfrlbd.h>   // CFormattedCellListBoxData

// CONSTANTS

// FORWARD DECLARATIONS
class CPeninputJapanesePredictiveWnd;

// CLASS DECLARATION

/**
*  CPeninputJapanesePredictiveCellListBoxData class.
*
*  @lib peninputcommonctrlsjp.lib
*  @since 2.6
*/
class CPeninputJapanesePredictiveCellListBoxData
        : public CFormattedCellListBoxData
    {
    public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CPeninputJapanesePredictiveCellListBoxData* NewL();

    public: // New functions
        /**
        * Set a starting x-point and ending to highlight
        * @since 2.6
        * @param aHighlightStartXPos    a starting x-point
        * @param aHighlightEndXPos      a ending x-point
        */
        void SetHighlightArea(TInt aHighlightStartXPos,
                              TInt aHighlightEndXPos);

    public: // Functions from CFormattedCellListBoxData

        /**
        * From CFormattedCellListBoxData Draw() is the most important method in
        * CPeninputJapanesePredictiveCellListBoxData.
        * This is the only method that needs to be implemented in this class and
        * it draws the whole listi tem.
        * @since 0.0
        * @param aProperties Properties attached to the list item
        * @param aGc Graphics context used for drawing the list item
        * @param aText a text string describing the list item cells
        * @param aRect the area of the screen for the whole list item
        * @param aHighlight whether the list item is selected
        * @param aColors the colours used for drawing the item
        * @return None.
        */
        virtual void Draw(TListItemProperties aProperties,
                          CWindowGc& aGc,
                          const TDesC* aText,
                          const TRect& aRect,
                          TBool aHighlight,
                          const TColors& aColors) const;

        /**
        * From CFormattedCellListBoxData
        * Show hightlight area in listbox
        * @since 2.6
        * @param aGc        Gc
        * @param aItemRect  rect to highlight
        * @param aHighlight ETrue is highlight. EFalse is non-highlight.
        */
        void DrawDefaultHighlight(CWindowGc &aGc,
                                  const TRect &aItemRect,
                                  TBool aHighlight) const;

        void SetWnd(CPeninputJapanesePredictiveWnd* aWnd);

    private:    // Data
        TInt iHighlightStartXPos;
        TInt iHighlightEndXPos;
        CPeninputJapanesePredictiveWnd* iWnd;
    };

#endif  // CPENINPUTJAPANESEPREDICTIVECELLLISTBOXDATA_H

// End of File
