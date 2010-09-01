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
* Description:  Predictive Window
*
*/

#ifndef C_CPENINPUTJAPANESEPREDICTIVEWND_H
#define C_CPENINPUTJAPANESEPREDICTIVEWND_H

#include <eiklbo.h>
#include <peninputlayoutpopupwnd.h>
#include "peninputjapanesepredictivelistboxmodel.h"
                // TypeDef:CPredictivePopupLineInfoArray,CPredictivePopupElementArray

class CPeninputJapanesePredictiveListBox;
class CTextListBoxModel;
class CAknFepCtrlEventButton;

class CPeninputJapanesePredictiveWnd: public CFepLayoutPopupWnd,
                     public MEikListBoxObserver,
                     public MEventObserver
    {
    public: // Constructors and destructor

    enum TPeninputUICtrlScroll
        {
        EScrollNone,
        EScrollLeft,
        EScrollRight,
        EScrollUp,
        EScrollDown
        };
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CPeninputJapanesePredictiveWnd* NewL(CFepUiLayout* aUiLayout,
                                                          TInt aControlId, const CCoeControl* aParent);

        /**
        * Destructor.
        */
        virtual ~CPeninputJapanesePredictiveWnd();

        //from base control CFepUiBaseCtrl
        /**
         * From CFepUiBaseCtrl
         * Set control's rectangle
         *
         * @since S60 V4.0
         * @param aRect The new control area
         */
        IMPORT_C virtual void SetRect(const TRect& aRect);

        /**
         * From CFepUiBaseCtrl
         * Set control's rectangle
         *
         * @since S60 V4.0
         * @param aFlag ETrue if want to hide the control.
         */
         virtual void Hide(TBool aFlag);

        /**
        * set scroll up/down button
        * @since 2.6
        * @param aScrollUpBtn
        * @param aScrollDownBtn
        */
        void SetButtons(CAknFepCtrlEventButton* aScrollUpBtn, CAknFepCtrlEventButton* aScrollDownBtn);

        /**
        * Specify the cursor position of a pop-up list
        * @since 2.6
        * @param aItemIndex  The index of the cursor position
        */
        void SetCurrentItemIndex(TInt aItemIndex) const;

        /**
         * From MEventObserver
         * Handle control event
         *
         * @since S60 v3.2
         * @param aEventType The event type
         * @param aCtrl The control who sends the event
         * @param aEventData The event data
         * @return None
         */
        void HandleControlEvent( TInt aEventType,
                                     CFepUiBaseCtrl* aCtrl,
                                     const TDesC& aEventData );

        /**
        * From MAknFepUICtrlPredictivePaneInterface
        * Show candidate list popup window
        * @since 2.6
        * @param aItems     array of candidate list
        * @param aIndex     selecting number when displaying
        * 
        */
        void ShowPopupL(const CDesCArrayFlat* aItems, TInt aIndex);

        /**
        * From MAknFepUICtrlPredictivePaneInterface
        * Hide candidate list popup window
        * @since 2.6
        * @param aItems     array of candidate list
        * 
        */
        void HidePopup();

        /**
        * Return a selected candidate item number currently
        * @since 2.6
        */
        TInt CurrentItemIndexOfCandidate() const;

        /**
        * @param aData Carry information of key pressed
        * @since 2.6
        * 
        */
        void MoveTo(TPeninputUICtrlScroll aDirection);

        /**
        * Update Size
        * @param aWidth
        * @param aUnitWidth
        * @param aUnitHeight
        */
        void SizeChanged(TInt aWidth, TInt aUnitWidth, TInt aUnitHeight);

        /**
         * Get item whose region contains the point.
         *
         * @since S60 V4.0
         * @param aPoint The point position the layout
         * @return The index of choice item. -1 if no item contains
         *         the point
         */
         TInt HitTest(const TPoint& aPoint);

        /**
         * Handle pointer down event
         *
         * @since S60 V4.0
         * @param aPoint The point position relative the layout
         * @return The control which handles the event.
         */
        virtual CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);

        /**
         * Handle pointer move event
         *
         * @since S60 V4.0
         * @param aPoint The point position relative the layout
         * @return The control which handles the event.
         */
        virtual CFepUiBaseCtrl* HandlePointerMoveEventL(const TPoint& aPoint);

        /**
         * Handle pointer up event
         *
         * @since S60 V4.0
         * @param aPoint The point position relative the layout
         * @return The control which handles the event.
         */
        virtual CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);

        // from MEikListBoxObserver 
        virtual void HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType);

        /**
        * Set linecount
        *
        */
        void SetLineCount(TInt aLineCount);

        /**
        * called when listbox updated
        * @param aRect  Rectangle in which the Cone framework believes drawing
        *               is needed
        */
        void ReDraw();

        /**
         * Cancel repeat
         *
         * @since S60 v3.2
         * @return None
         */
        void CancelRepeat();

        /**
        * @return  listbox control
        * @since 2.6
        */
        CPeninputJapanesePredictiveListBox* ListBox() const;

    /**
     * From CFepUiBaseCtrl
     * DeActivate popup window
     *
     * @since S60 V4.0
     */ 
    void OnDeActivate();

    protected:

        /**
         * Constructor.
         *
         * @since S60 V4.0
         * @param aUiLayout Ui layout who contains this control.Ownership not transferred
         * @param aControlId control Id
         */                  
        CPeninputJapanesePredictiveWnd(CFepUiLayout* aUiLayout,
                                      TInt aControlId);

    private:
        /**
        * Symbian 2nd phase constructor can leave.
        */
        void ConstructL(const CCoeControl* aParent);

        /**
        * Setup array with line information in candidate list
        * @since 2.6
        * @param aItems                     array with candidate string
        * @param aMaxDisplayWidthInPixel    maximum width in pixel
        * @param aFontId                    font id
        */
        void CreatePredictivePopupLineInfoArrayL(const CDesCArrayFlat* aItems,
                                            const TInt aMaxDisplayWidthInPixel,
                                            const TInt aFontId);

        /**
        * Append to array with line information
        * @since 2.6
        * @param aLineText      string to show in a line
        * @param aElementArray  array with element information
        */
        void PredictivePopupAppendLineL(const TPtr& aLineText,
                                   CPredictivePopupElementArray* aElementArray);

        /**
        * Create arrary with string to show in a line
        * @since 2.6
        * @param aLineArray     arrary with line information
        * @return   string array
        */
        void CreateListTextArrayL(const CPredictivePopupLineInfoArray& aLineArray);

        /**
        * Called by the framework to draw the control
        */
        void Draw();

        /**
        * called when prediction pane is not active
        */
        void ClearList();

        /**
        * activateButtons or disactivate
        */
        void ActivateButtons(TBool aActive) const;

    private:
        /**
        * listbox
        * Own
        */    
        CPeninputJapanesePredictiveListBox* iListBox;

        /**
        * LineInfoArray
        * Own
        */    
        CPredictivePopupLineInfoArray* iLineInfoArray;

        /**
        * ListTextArray
        * Own
        */    
        CDesCArrayFlat* iListTextArray;

        /**
        * scroll up button
        * Not Own
        */    
        CAknFepCtrlEventButton* iScrollUpBtn;

        /**
        * scroll down button
        * Not Own
        */    
        CAknFepCtrlEventButton* iScrollDownBtn;

        /**
        * The unit width
        */
        TInt iUnitWidth;

        /**
        * The unit height
        */
        TInt iUnitHeight;
    };

#endif //C_CPENINPUTJAPANESEPREDICTIVEWND_H

// End Of File
