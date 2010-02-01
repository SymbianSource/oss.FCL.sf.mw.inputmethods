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
* Description:            Candidate list
*
*/












#ifndef CAKNFEPUICTRLJAPANESECANDIDATEPANE_H
#define CAKNFEPUICTRLJAPANESECANDIDATEPANE_H

// INCLUDES
#include <aknPopup.h>   // CAknPopupList
#include "AknFepUICtrlCandidatePaneInterface.h"
                        // MAknFepUICtrlCandidatePaneInterface

// CONSTANTS

// FORWARD DECLARATIONS
// CLASS DECLARATION
class CAknFepUIManagerJapanese;

/**
*  CAknFepUICtrlJapaneseCandidatePane class.
*
*  @lib AknFep.lib
*  @since 2.6
*/
NONSHARABLE_CLASS(CAknFepUICtrlJapaneseCandidatePane) :
    public CAknPopupList,
    public MAknFepUICtrlCandidatePaneInterface
    {
    public: // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CAknFepUICtrlJapaneseCandidatePane* NewL(CAknFepUIManagerJapanese* aUiMng);

        /**
        * Destructor.
        */
        virtual ~CAknFepUICtrlJapaneseCandidatePane();

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
        void CloseCandidateList();

        /**
        * Specify the cursor position of a pop-up list
        * @since 2.6
        * @param aItemIndex  The index of the cursor position
        * @param aDraw  ETrue is redraw.
        */
        void SetCurrentItemIndexL(TInt aItemIndex, TBool aDraw);

        /**
        * Specify the display position of a pop-up list
        * @since 2.6
        * @param aOffset     Offset of a window display position
        */
        void SetOffsetPosition(TPoint aOffset);

    public: // Functions from MAknFepUICtrlCandidatePaneInterface
        /**
        * From MAknFepUICtrlCandidatePaneInterface
        * Set position to display candidate list
        * @since 2.6
        * @param aLeftSideOfBaseLine    left side of font
        * @param aHeight                font hight
        * @param aAscent                Ascent position
        */
        void SetWindowPosition(const TPoint& aLeftSideOfBaseLine,
                              TInt aHeight,
                              TInt aAscent);

        /**
        * From MAknFepUICtrlCandidatePaneInterface
        * Show candidate list popup window
        * @since 2.6
        * @param aItems     array of candidate list
        * @param aIndex     selecting number when displaying
        *
        */
        void ShowCandidateListL(CDesCArrayFlat* aItems, TInt aIndex, TInt aResourceID=0);

        /**
        * From MAknFepUICtrlCandidatePaneInterface
        * Hide candidate popup window
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

        /**
        * from CAknPopupList of MEikCommandObserver
        * @since 3.2
        * @param aCommandId aCommandId  Event Id from the soft-key
        */
        void ProcessCommandL(TInt aCommandId);

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
        CAknFepUICtrlJapaneseCandidatePane(CAknFepUIManagerJapanese* aUiMng);

        /**
        * Symbian 2nd phase constructor can leave.
        */
        void ConstructL();

        /**
        * Set the title for the selection list
        * @since 2.6
        * @param aTitle      The character string of a title
        */
        void SetTitleL(const TDesC& aTitle);

        /**
        * The handler function for size change
        * @since 2.6
        * @param aDef                    layout information of popup-window
        * @param aHeading                heading Pane object
        * @param aListBox                listbox object
        * @param aWindowOwningControl    owner control
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
        * @param aListBox        listbox object
        * @param aHeadingOrNull  heading Pane object
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
        * @param heading     ETrue:with heading pane/ EFalse:without heading pane
        */
        void SetupMenuPopupWindow(TAknPopupWindowLayoutDef& aDef,
                                  TInt num,
                                  TBool heading);

        /**
        * The handler function for size and a location
        * @since 2.6
        * @param aDef                layout information of popup-window
        * @param aContentInsidePopup
        * @param aHeadingOrNull      heading pane object
        */
        static void HandleSizeAndPositionOfComponents(
                                const TAknPopupWindowLayoutDef& aDef,
                                CCoeControl* aContentInsidePopup,
                                CAknPopupHeadingPane* aHeadingOrNull);

        /**
        * Draws a window shadow
        * @since 2.6
        * @param aCoverRect the area covered by the shadow
        * @param aSecondShadowRect the area used for second shadow
        * @param aFirstShadowRect  the area of first shadow
        * @param aOutliineFrameRect the area of black outline frame
        * @param aInsideAreaRect   the area of content inside the window
        */
        static void DrawWindowShadow(
                                CWindowGc& aGc,
                                const TAknLayoutRect& aCoverRect,
                                const TAknLayoutRect& aSecondShadowRect,
                                const TAknLayoutRect& aFirstShadowRect,
                                const TAknLayoutRect& aOutlineFrameRect,
                                const TAknLayoutRect& aInsideAreaRect);

        /**
        * This function returns maximum columns to be able to display in listbox, this function
        * find longest candidate string in candidate list. Return 11 columns in case more than 11 columns, and minimum is 3 columns.
        * @since 2.6
        * @param aModel listbox model object
        * @return maximum columns
        */
        TInt MaximumColumns(CTextListBoxModel* aModel);

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
        * @param aRect  Rectangle in which the Cone framework believes drawing
        *               is needed
        */
        virtual void Draw(const TRect& aRect) const;

        /**
        * Handle key events. When a key event occurs,
        * CONE calls this function for each control on the control stack,
        * until one of them returns EKeyWasConsumed to indicate that
        * it processed the key event.
        *
        * @param aKeyEvent  The key event.
        * @param aType      The type of the event:
        *                           EEventKey, EEventKeyUp or EEventKeyDown.
        * @return           Indicates whether or not the key event was used
        *                   by this control.
        */
        virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,
                                    TEventCode aType);

        /**
        * show cba
        * @since 2.6
        */
        void ShowWindowCbaL();

        /**
        * Changing the cba
        * @since 3.2
        * @param aResourceID
        */
        void UpdateCbaL(TInt aResourceID);

    private:    // Data
        TRect  iMainPaneRect;
        TPoint iOffsetPosition;
        TInt   iSrcTextHeight;

        TInt iMaximumColumns;
        TInt iMaximumRows;
        CAknFepUIManagerJapanese* iUiMng;
    };

#endif  // CAKNFEPUICTRLJAPANESECANDIDATEPANE_H

// End of File
