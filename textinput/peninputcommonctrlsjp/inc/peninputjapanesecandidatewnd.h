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
* Description:  Candidate Window
*
*/

#ifndef C_CPENINPUTJAPANESECANDIDATEWND_H
#define C_CPENINPUTJAPANESECANDIDATEWND_H
#include <peninputpluginutils.h>

// INCLUDES
#include <eiklbo.h>
#include <peninputlayoutpopupwnd.h>


// CLASS DECLARATION
class CPeninputJapaneseCandidateListBox;
class CTextListBoxModel;
class CFepInputContextFieldJp;

class CPeninputJapaneseCandidateWnd: public CFepLayoutPopupWnd, public MEikListBoxObserver, public MAknFepTimerHandler
    {
public:
    enum TPeninputUICtrlScroll
        {
        EScrollNone,
        EScrollUp,
        EScrollDown
        };
    /**
    * Two-phased constructor.
    */
    IMPORT_C static CPeninputJapaneseCandidateWnd* NewL(CFepUiLayout* aUiLayout,
                                                          TInt aControlId, const CCoeControl* aParent);

    /**
    * Destructor.
    */
    virtual ~CPeninputJapaneseCandidateWnd();

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
    * @since 2.6
    * @param aTPoint aPos     display position
    *
    */
    IMPORT_C void SetPosition(const TPoint& aPos);

    /**
    * @since 2.6
    * @param aInputContextField
    *
    */
    void SetIcf(CFepInputContextFieldJp* aInputContextField);

    /**
    * From MAknFepUICtrlCandidatePaneInterface
    * Show candidate list popup window
    * @since 2.6
    * @param aItems     array of candidate list
    * @param aIndex     selecting number when displaying
    *
    */
    IMPORT_C void ShowPopupL(CDesCArrayFlat* aItems, TInt aIndex);

    /**
    * From MAknFepUICtrlCandidatePaneInterface
    * Hide candidate list popup window
    * @since 2.6
    * @param aItems     array of candidate list
    * @return None
    *
    */
    IMPORT_C void HidePopup();

    /**
    * @return  a selected candidate item number currently
    * @since 2.6
    */
    IMPORT_C TInt CurrentItemIndex() const;

    /**
    * Update Maxrow accoding to font
    * @param aUnitHeight
    * @param aUnitCount
    * @return None
    */
    void UpdateMaximumRows(TInt aUnitHeight, TInt aUnitCount);

    /**
    * @param aData Carry information of key pressed
    * @return None
    *
    */
    void MoveToL(TPeninputUICtrlScroll aDirection);

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
    /**
     * Handle pointer up event
     *
     * @since S60 V4.0
     * @param aListBox ListBox control
     * @param aEventType
     * @return None
     */
    virtual void HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType);

    /**
     * Cancel ScrollBar repeat
     *
     * @since S60 v3.2
     * @return None
     */
    void CancelScrollBarRepeat();

    /**
     * from MAknFepTimerHandler
     * HandleTimerOut. Handling time out event
     *
     * @since S60 v3.2
     * @param aTimer The timer which sending the request
     * @return None
     */
    void HandleTimerOut(const CAknFepTimer* aTimer);

    /**
    * @return  listbox control
    * @since 2.6
    */
    CPeninputJapaneseCandidateListBox* ListBox() const;

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
    CPeninputJapaneseCandidateWnd(CFepUiLayout* aUiLayout,
                                  TInt aControlId);

private:
    /**
    * Symbian 2nd phase constructor can leave.
    * @return None
    */
    void ConstructL(const CCoeControl* aParent);

    /**
    * This function returns maximum columns to be able to display in listbox, this function
    * find longest candidate string in candidate list. Return 11 columns in case more than 11 columns, and minimum is 3 columns.
    * @since 2.6
    * @param aModel listbox model object
    * @return maximum columns
    */
    TInt MaximumColumns(const CTextListBoxModel* aModel) const;

    /**
    * This function returns maximum rows to be able to display in listbox.
    * Maximum is 7 rows and  minimum is 3 columns.
    * @since 2.6
    * @param aModel listbox model object
    * @return maximum line
    */
    TInt MaximumRows(const CTextListBoxModel* aModel) const;

    /**
    * from CFepUiBaseCtrl
    * Called by the framework to draw the control
    * @param aRect  Rectangle in which the Cone framework believes drawing
    *               is needed
    */
    void Draw();

    /**
     * ReDraw
     */
    void ReDraw();

    /**
     * Draw Title
     */
    void DrawTitle();

    /**
     * Send pointer event to ListBox.
     * Used for e.g. selecting an item from the list box.
     *
     * @param aPointerEvent Details of the pointer event that is being handled.
     * @param aFlags   1: to ListBox otherwise:to ScrollBar
     * @param aDraw   ETrue: Draw if to ScrollBar EFalse: not Draw
     */
    void SendPointerEventL(const TPointerEvent& aPointerEvent, TInt aFlags, TBool aDraw);

private:

    TInt iMaximumColumns;
    TInt iMaximumRows;

    /**
     * listbox
     * Own
     */
    CPeninputJapaneseCandidateListBox* iListBox;

    TPoint iPos;

    /**
     * The input context field
     * Not Own
     */
    CFepInputContextFieldJp* iInputContextField;

    /**
     * The maximum row according to font
     */
    TInt iFontMaximumRows;

    TSize iTopLeft;

    TPointerEvent iScrollBarPointerEvent;

    /**
     * long press timer (Own)
     */
    CAknFepTimer* iLongPressTimer;

    /**
     * repeat timer (Own)
     */
    CAknFepTimer* iRepeatTimer;

    /** 
     * Server application GC. It's used to keep the original window GC.
     * Not Own
     */    
    CWindowGc* iAppGc; 
    };

#endif //C_CPENINPUTJAPANESECANDIDATEWND_H

// End Of File
