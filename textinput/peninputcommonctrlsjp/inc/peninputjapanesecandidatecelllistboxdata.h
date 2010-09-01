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
* Description:  Candidate list CellListBoxData
*
*/


#ifndef CPENINPUTJAPANESECANDIDATECELLLISTBOXDATA_H
#define CPENINPUTJAPANESECANDIDATECELLLISTBOXDATA_H

// INCLUDES
#include <eikfrlbd.h>   // CFormattedCellListBoxData

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  CPeninputJapaneseCandidateCellListBoxDatae class.
*
*  @lib peninputcommonctrlsjp.lib
*  @since 3.2
*/
class CPeninputJapaneseCandidateCellListBoxData
        : public CFormattedCellListBoxData
    {
    public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CPeninputJapaneseCandidateCellListBoxData* NewL();

    public: // Functions from CFormattedCellListBoxData
        /**
        * From CFormattedCellListBoxData Draw() is the most important method
        * in CPeninputJapaneseCandidateCellListBoxData.
        * This is the only method that needs to be implemented in this class
        * and it draws the whole list item.
        * @since 2.6
        * @param aProperties Properties attached to the list item
        * @param aGc Graphics context used for drawing the list item
        * @param aText a text string describing the list item cells
        * @param aRect the area of the screen for the whole list item
        * @param aHighlight whether the list item is selected
        * @param aColors the colours used for drawing the item
        */
        virtual void Draw(TListItemProperties aProperties,
                          CWindowGc& aGc,
                          const TDesC* aText,
                          const TRect& aRect,
                          TBool aHighlight, 
                          const TColors& aColors) const;

        /**
        * From CFormattedCellListBoxData DrawDefaultHighlight() is ...
        * @since 2.6
        * @param aGc 
        * @param aItemRect
        * @param aHighlight
        */
        void DrawDefaultHighlight(CWindowGc& aGc,
                                  const TRect& aItemRect,
                                  TBool aHighlight) const;
    private:
        /**
        * @internal
        * From CFormattedCellListBoxData Draw() is the most important method
        * in CPeninputJapaneseCandidateCellListBoxData.
        * This is the only method that needs to be implemented in this class
        * and it draws the whole list item.
        * @since 2.6
        * @param aProperties Properties attached to the list item
        * @param aGc Graphics context used for drawing the list item
        * @param aText a text string describing the list item cells
        * @param aItemRect the area of the screen for the whole list item
        * @param aHighlight whether the list item is selected
        * @param aColors the colours used for drawing the item
        */
        void DrawFormattedCandidate( TListItemProperties& aProperties,
                           CWindowGc& aGc,
                           const TDesC* aText,
                           const TRect& aItemRect,
                           TBool aHighlight,
                           const TColors& aColors ) const;
    };

#endif  // CPENINPUTJAPANESECANDIDATECELLLISTBOXDATA_H

// End of File
