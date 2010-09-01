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
* Description:  common layout window for UI interface of VKB and HWR : japanese
*
*/


#ifndef C_PENINPUTLAYOUTWINDOWJP_H
#define C_PENINPUTLAYOUTWINDOWJP_H

// System includes
#include <peninputlayoutwindow.h>

class CFepInputContextFieldJp;
class CPeninputJapaneseView;
class CPeninputJapaneseCandidateWnd;
class CPeninputJapanesePredictiveWnd;

/**
 *  Pen input Common layout for Japanese class
 *
 *  @lib peninputcommonctrlsjp.dll
 *  @since S60 v3.2
 */
class CPeninputLayoutWindowJp : public CPeninputLayoutWindow
    {

public:

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual ~CPeninputLayoutWindowJp();

    /**
     * From CPeninputLayoutWindow
     * Handle control event
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aCtrl The control who sends the event
     * @param aEventData The event data
     * @return None
     */
    IMPORT_C void HandleControlEvent( TInt aEventType,
                                     CFepUiBaseCtrl* aCtrl,
                                     const TDesC& aEventData );

    /**
     * Create VKB and HWR common controls
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual void CreateAllControlsL();

    /**
     * Set control fonts for VKB and HWR common controls
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual void SetControlsFont();

    /** 
     * From CPeninputLayoutWindow
     * ConstructFromResourceL load settings from resource.
     * Before calling this function, the resource id should be set
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual void ConstructFromResourceL();

    /**
     * Handle pointer down event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.
     */
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);

    /**
     * Get id of prediction pane scroll up button
     *
     * @since S60 v3.2
     * @return The id of prediction pane scroll up button resource
     */
    IMPORT_C virtual TInt GetScrollUpBtnResId() = 0;

    /**
     * Get id of prediction pane scroll down button
     *
     * @since S60 v3.2
     * @return The id of prediction pane scroll down button resource
     */ 
    IMPORT_C virtual TInt GetScrollDownBtnResId() = 0;

    /**
     * Handle pointer up event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.
     */
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);

    /**
     * Handle pointer move event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.
     */
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerMoveEventL(const TPoint& aPoint);

    /**
     * Get candidate window
     *
     * @since Series 60 3.2
     * @return candidate window
     */ 
    IMPORT_C CPeninputJapaneseCandidateWnd* CandidateWindow();

    /**
     * Get predictive window
     *
     * @since Series 60 3.2
     * @return predictive window
     */
    IMPORT_C CPeninputJapanesePredictiveWnd* PredictiveWindow();

    /**
     * Get CharacterRange window
     *
     * @since Series 60 3.2
     * @return CharacterRange window
     */ 
    IMPORT_C CPeninputJapaneseCandidateWnd* CharacterRangeWindow();

    /**
     * Get input context field
     *
     * @since S60 v3.2
     * @return The input context field
     */
    IMPORT_C CFepInputContextFieldJp* InputContextField();

    /**
     * Set layoutwindow opened
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void Opened();

    /**
     * Set layoutwindow opened
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void Closed();

    /**
     * DisableRegionUpdating on ICF
     *
     * @since S60 v5.0
     * @return None
     */
    IMPORT_C void ICFDisableRegionUpdating();

    /**
     * EnableRegionUpdating on ICF
     *
     * @since S60 v5.0
     * @return None
     */
    IMPORT_C void ICFEnableRegionUpdating();

protected:

    /**
     * constructor
     *
     * @since S60 v3.2
     * @param aUiLayout The pointer to CFepUiLayout object
     * @param aLayoutContext The pointer to MPeninputLayoutContext
     * @return None
     */
    IMPORT_C CPeninputLayoutWindowJp( CFepUiLayout* aUiLayout, 
                                    MPeninputLayoutContext* aLayoutContext );

private:
    /**
     * Add button to client area
     *
     * @since S60 v3.2
     * @param aControlId The control id
     * @param aEventId The control's event id
     * @param aResId The control's resource id
     * @param aUnicode The control's unicode
     * @param aIsRepeat The TBool indicating whether the
     *        control is repeated or not
     * @return The pointer to CAknFepCtrlEventButton object
     */
    CAknFepCtrlEventButton* AddButtonL( const TInt aControlId,
                                        const TInt aEventId,
                                        const TInt aResId,
                                        const TInt aUnicode = 0,
                                        const TBool aIsRepeat = EFalse );

    /**
     * Add Candidate window into the layout window
     *
     * @since Series 60 3.2
     * @return None
     */
    void AddCandidateWindowL();

    /**
     * Add Predictive window into the layout window
     *
     * @since Series 60 3.2
     * @return None
     */
    void AddPredictiveWindowL();

    /**
     * Add CharacterRange window into the layout window
     *
     * @since Series 60 3.2
     * @return None
     */
    void AddCharacterRangeWindowL();

protected:  // data

    /**
     * The popup window
     * Own  (AddControlL())
     */
    CPeninputJapaneseCandidateWnd* iCandidateWnd;

    /**
     * The popup window
     * Own  (AddControlL())
     */
    CPeninputJapanesePredictiveWnd* iPredictiveWnd;

    /**
     * The popup window (Hwr)
     * Own  (AddControlL())
     */
    CPeninputJapaneseCandidateWnd* iCharacterRangeWnd;

    /**
     * parent of the popup window
     * Own
     */
    CPeninputJapaneseView* iViewCandidate;

    /**
     * parent of the popup window
     * Own
     */
    CPeninputJapaneseView* iViewPrediction;

    /**
     * parent of the popup window
     * Own
     */
    CPeninputJapaneseView* iViewCharacterRange;

    /**
     * lauoutwindow opend
     */
    TBool iOpened;

    };

#endif // C_PENINPUTLAYOUTWINDOWJP_H
