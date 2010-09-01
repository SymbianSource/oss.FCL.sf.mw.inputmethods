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
* Description:            Predictive Candidate list
*
*/












#ifndef CAKNFEPUICTRLJAPANESEPREDICTIVEPANE_H
#define CAKNFEPUICTRLJAPANESEPREDICTIVEPANE_H

// INCLUDES
#include <aknPopup.h>
#include "AknFepUICtrlCandidatePaneInterface.h" // MAknFepUICtrlCandidatePaneInterface
#include "AknFepUIAvkonCtrlJapanesePredictiveListBoxModel.h"

// CONSTANTS
const TInt KJapPredictiveVisibleListRowsMax = 3;
const TInt KJapPredictiveVisibleListRowsMin = 1;

// FORWARD DECLARATIONS
class CAknFepUIManagerJapanese;

// CLASS DECLARATION

/**
*  CAknFepUICtrlJapanesePredictivePane class.
*
*  @lib AknFep.lib
*  @since 2.6
*/
NONSHARABLE_CLASS(CAknFepUICtrlJapanesePredictivePane) :
    public CAknPopupList,
    public MAknFepUICtrlCandidatePaneInterface
    {
    public: // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CAknFepUICtrlJapanesePredictivePane* NewL(CAknFepUIManagerJapanese* aUiMng);

        /**
        * Destructor.
        */
        virtual ~CAknFepUICtrlJapanesePredictivePane();

    public: // New functions
        /**
        * Show Pop-up list
        * @since 2.6
        */
        void ShowWindow();

        /**
        * Close Pop-up list
        * @since 2.6
        */
        void ClosePredictiveList();

        /**
        * Specify the cursor position of a pop-up list
        * @since 2.6
        * @param aItemIndex  The index of the cursor position
        */
        void SetCurrentItemIndex(TInt aItemIndex);

        /**
        * Specify the display position of a pop-up list
        *
        * @since 2.6
        * @param aOffset     Offset of a window display position
        */
        void SetOffsetPosition(TPoint aOffset);

public: // Functions from MAknFepUICtrlCandidatePaneInterface
        /**
        * From MAknFepUICtrlCandidatePaneInterface
        * Set position to show candidate list
        * @since 2.6
        * @param aLeftSideOfBaseLine    left side of font
        * @param aHeight                font height
        * @param aAscent                ascent position
        */
        void SetWindowPosition(const TPoint& aLeftSideOfBaseLine,
                               TInt aHeight,
                               TInt aAscent);

        /**
        * From MAknFepUICtrlCandidatePaneInterface
        * Show candidate list popup window
        * @since 2.6
        * @param aItems     array including each candidate string
        * @param aIndex     selecting candidate number to display first time
        *
        */
        void ShowCandidateListL(CDesCArrayFlat* aItems, TInt aIndex, TInt aResourceID);

        /**
        * From MAknFepUICtrlCandidatePaneInterface
        * Hide candidate list popup window
        * @since 2.6
        */
        void HideWindow();

        /**
        * From MAknFepUICtrlCandidatePaneInterface
        * Return a selected candidate item number currently
        * @since 2.6
        */
        TInt CurrentItemIndexOfCandidate();

        /**
        * From MAknFepUICtrlCandidatePaneInterface
        * Move a current item in an appointed direction
        * @since 2.6
        * @param aDirection     direction
        */
        TInt MoveItem(TAknFepUICtrlScroll aDirection);

    protected: // Functions from CAknPopupList
        /**
        * From CAknPopupList Set up a window layout type
        * @since 2.6
        * @param aType       Layout type
        */
        virtual void SetupWindowLayout(AknPopupLayouts::TAknPopupLayouts aType);

        /**
        * From CAknPopupList Set the title for the selection list
        * @since 2.6
        * @param aAccept
        */
        virtual void AttemptExitL(TBool aAccept);

        TTypeUid::Ptr MopSupplyObject(TTypeUid aId);

    protected: // from MEikListBoxObserver
        /**
         * Handle pointer up event
         *
         * @since S60 V3.2
         * @param aListBox ListBox control
         * @param aEventType
         * @return None
         */
        virtual void HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType);

    private:
        /**
        * constructor.
        */
        CAknFepUICtrlJapanesePredictivePane(CAknFepUIManagerJapanese* aUiMng);

        /**
        * Symbian 2nd phase constructor can leave.
        */
        void ConstructL();

        /**
        * Create array with line information in candidate list
        * @since 2.6
        * @param aItems                     array with candidate string
        * @param aMaxDisplayWidthInPixel    maximum width in pixel
        * @param aFontId                    font id
        * @return array with line information
        */
        CPredictivePopupLineInfoArray*
        CreatePredictivePopupLineInfoArrayL(CDesCArrayFlat* aItems,
                                            const TInt aMaxDisplayWidthInPixel,
                                            const TInt aFontId);

        /**
        * Append to array with line information
        * @since 2.6
        * @param aLineArray     array with line information
        * @param aLineText      string to show in a line
        * @param aElementArray  array with element information
        */
        void PredictivePopupAppendLineL(CPredictivePopupLineInfoArray* aLineArray,
                                   const TDesC& aLineText,
                                   CPredictivePopupElementArray* aElementArray);

        /**
        * Create arrary with string to show in a line
        * @since 2.6
        * @param aLineArray     arrary with line information
        * @return   string array
        */
        CDesCArrayFlat* CreateListTextArrayL(
                                      CPredictivePopupLineInfoArray* aLineArray);

        /**
        * The handler function for size change
        * @since 2.6
        * @param aDef                    layout information of popup-window
        * @param aHeading                heading pane object
        * @param aListBox                listbox object
        * @param aWindowOwningControl    Owner control
        */
        void HandleSizeChanged(TAknPopupWindowLayoutDef& aDef,
                               CAknPopupHeadingPane* aHeading,
                               CEikListBox* aListBox,
                               CCoeControl* aWindowOwningControl
                               );

        /**
        * The handler function for Draw
        * @since 2.6
        * @param aEikEnv         EikEnv
        * @param aGc             Gc
        * @param aDef            layout information of popup-window
        * @param aListBox        Listbox object
        * @param aHeadingOrNull  Heading Pane object
        */
        static void HandleDraw(CEikonEnv* aEikEnv,
                   CWindowGc& aGc,
                   const TAknPopupWindowLayoutDef& aDef,
                   CEikListBox* aListBox,
                   CAknPopupHeadingPane* aHeadingOrNull);

        /**
        * Set up the information on a pop-up window
        * @since 2.6
        * @param aDef        layout information of popup-window
        * @param aRect       rect
        * @param num         total cantidate number
        */
        void CalcPopupMenuWindow(TAknPopupWindowLayoutDef& aDef,
                                 const TRect& aRect,
                                 TInt num);

        /**
        * Set up the information on a pop-up window
        * @since 2.6
        * @param aDef        layout information of popup-window
        * @param num         total cantidate number
        * @param heading     ETrue id with heading area, EFalse is without one.
        */
        void SetupMenuPopupWindow(TAknPopupWindowLayoutDef& aDef,
                                  TInt num,
                                  TBool heading);

        /**
        * The handler function for size and a location
        * @since 2.6
        * @param aDef                layout information of popup-window
        * @param aContentInsidePopup
        * @param aHeadingOrNull      Heading Pane object
        */
        static void HandleSizeAndPositionOfComponents(
                            const TAknPopupWindowLayoutDef& aDef,
                            CCoeControl* aContentInsidePopup,
                            CAknPopupHeadingPane* aHeadingOrNull);

        /**
        * Draws a window shadow
        * @since 2.6
        * @param aCoverRect         the area covered by the shadow
        * @param aSecondShadowRect  the area used for second shadow
        * @param aFirstShadowRect   the area of first shadow
        * @param aOutliineFrameRect the area of black outline frame
        * @param aInsideAreaRect    the area of content inside the window
        */
        static void DrawWindowShadow(CWindowGc& aGc,
                                     const TAknLayoutRect& aCoverRect,
                                     const TAknLayoutRect& aSecondShadowRect,
                                     const TAknLayoutRect& aFirstShadowRect,
                                     const TAknLayoutRect& aOutlineFrameRect,
                                     const TAknLayoutRect& aInsideAreaRect);

        /**
        * This function returns maximum rows to be able to display in listbox.
        * Maximum is 7 rows and  minimum is 3 columns.
        * @since 2.6
        * @param aModel listbox model object
        * @return maximum line
        */
        TInt MaximumRows(CTextListBoxModel* aModel);

        /**
        * Called by the framework to draw the control
        * @since 2.6
        * @param aRect  Rectangle in which the Cone framework believes drawing
        *               is needed
        */
        void Draw(const TRect& aRect) const;

        /**
        * Handle key events. When a key event occurs,
        * CONE calls this function for each control on the control stack,
        * until one of them returns EKeyWasConsumed to indicate that
        * it processed the key event.
        *
        * @since 2.6
        * @param aKeyEvent  The key event.
        * @param aType      The type of the event:
        *                           EEventKey, EEventKeyUp or EEventKeyDown.
        * @return           Indicates whether or not the key event was used
        *                   by this control.
        */
        TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,
                                    TEventCode aType);

        /**
        * selecting a previous candidate
        * @since 2.6
        * @return selected candidate number
        */
        TInt MovePreviousItem();

        /**
        * selecting a next candidate
        * @since 2.6
        * @return selected candidate number
        */
        TInt MoveNextItem();

        /**
        * selecting a previous line candidate
        * @since 2.6
        * @return selected candidate number
        */
        TInt MovePreviousLineItem();

        /**
        * selecting a next line candidate
        * @since 2.6
        * @return selected candidate number
        */
        TInt MoveNextLineItem();

        /**
        * Changing the cba
        * @since 2.6
        * @param aResourceID
        */
        void UpdateCbaL(TInt aResourceID);

        /**
        * from CAknPopupList of MEikCommandObserver
        * @since 2.6
        * @param aCommandId aCommandId  Event Id from the soft-key
        */
        void ProcessCommandL(TInt aCommandId);

        /**
        * show cba
        * @since 2.6
        */
        void ShowWindowCbaL();

    private:    // Data
        TRect   iMainPaneRect;
        TPoint  iOffsetPosition;
        TInt    iSrcTextHeight;

        TInt    iMaximumRows;
        CPredictivePopupLineInfoArray* iLineInfoArray;
        CDesCArrayFlat* iListTextArray;
        CAknFepUIManagerJapanese* iUiMng;
    };

#endif  // CAKNFEPUICTRLJAPANESEPREDICTIVEPANE_H

// End of File
