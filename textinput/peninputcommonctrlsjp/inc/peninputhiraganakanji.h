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
* Description:  CPeninputHiraganaKanji
*
*/


#ifndef C_CPENINPUTHIRAGANAKANAKANJI_H
#define C_CPENINPUTHIRAGANAKANAKANJI_H

// INCLUDES
#include <e32base.h>
#include <w32std.h>
#include <peninputcmdparam.h>
#include <badesca.h>
#include <peninputpluginutils.h>


class CPtiEngine;
class CFepUiLayout;
class CFepInputContextFieldJp;
class CPeninputJapaneseCandidateWnd;
class CPeninputJapanesePredictiveWnd;

class CPeninputHiraganaKanji: public CBase, public MAknFepTimerHandler
    {

public:
    /**
     *  HiraganaKanji Status.
     *
     *  @lib peninputcommonctrlsjp.lib
     *  @since S60 v3.2 
     */
    enum THiraganaKanjiStatus
        {
        EStatusInitial = 0,
        EStatusTransitoryInput,
        EStatusPredictiveTransitoryInput,
        EStatusPredictiveTransitoryInputWithNoChar,
        EStatusConversion,
        EStatusCandidate,
        EStatusChangeDivision,        // Change Conversion Unit Division
        EStatusComplete,              // wait SetEditorTextL
        EStatusCompleteAll,            // wait SetEditorTextL
        EStatusCompleteAllfromUiLayout // wait SetEditorTextL
        };
    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aPtiEngine
     * @param aInputContextField
     * @param aCandidateWnd
     * @param aPredictiveWnd
     * @return Pointer to created CPeninputHiraganaKanji object
     */
    IMPORT_C static CPeninputHiraganaKanji* NewL(CPtiEngine* aPtiEngine
                                             , CFepUiLayout* aUiLayout
                                             , CFepInputContextFieldJp* aInputContextField
                                             , CPeninputJapaneseCandidateWnd* aCandidateWnd
                                             , CPeninputJapanesePredictiveWnd* aPredictiveWnd);

    /**
     * destructor
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual ~CPeninputHiraganaKanji();

    /**
     * HandleControlEventJpL
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data.
     * @return None
     */
    IMPORT_C void HandleControlEventJpL(TInt aEventType, const TDesC& aEventData);

    /**
     * Handling keypress events. only judgment
     *
     * @since S60 v3.2
     * @param aData Carry information of key pressed
     * @return ETrue means event was responsed, otherwise EFalse
     */
    IMPORT_C TBool HandleKeyEvent(const TRawEvent &aData);

    /**
     * Handling keypress events.
     *
     * @since S60 v3.2
     * @return ETrue means event was responsed, otherwise EFalse
     */
    IMPORT_C TBool HandlePendingEventL();

    /**
     * Sets the text from the editor to ContextField
     *
     * @since S60 V4.0
     * typedef struct TFepInputContextFieldData_Tag
     *    {    
     *    TInt iTotalTextLen; //total text length in app editor
     *    TInt iTextStartPos; //The start position for text sending to input context field
     *     TCursorSelection iCurSel; //current cursor selection 
     *    TPtrC iText; // The text sent to input context field
     *    TBool iCursorVisibility; // Cursor visibility flag, indicates whether cursor need 
     *                           // be shown in input context field.
     *    } TFepInputContextFieldData;
     */
    IMPORT_C void SetEditorTextL(const TFepInputContextFieldData& aData);

    /**
     * Cancel repeat
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void CancelRepeat();

    /**
     * start key repeat
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void RepeatClearKeyStart();

    /**
     * from MAknFepTimerHandler
     * HandleTimerOut. Handling time out event
     *
     * @since S60 v3.2
     * @param aTimer The timer which sending the request
     * @return None
     */                
    void HandleTimerOut(const CAknFepTimer* aTimer);

private:

    /**
     * C++ constructor
     *
     * @since S60 v3.2
     * @param aPtiEngine
     * @param aInputContextField
     * @param aCandidateWnd
     * @param aPredictiveWnd
     * @return None
     */            
    CPeninputHiraganaKanji(CPtiEngine* aPtiEngine
                                , CFepUiLayout* aUiLayout
                                , CFepInputContextFieldJp* aInputContextField
                                , CPeninputJapaneseCandidateWnd* aCandidateWnd
                                , CPeninputJapanesePredictiveWnd* aPredictiveWnd);

    /**
     * Second phase constructor
     *
     * @since S60 v3.2
     * @return None
     */
    void ConstructL();

    /**
     * Insert a Character
     *
     * @param aEventData character data
     * @return None
     */
    void HandleInsertL(const TDesC& aEventData);

    /**
     * Clear Characters(not used)
     *
     * @since S60 v3.2
     * @return None
     */
    void ClearTextL();

    /**
     * complete
     *
     * @since S60 v3.2
     * @return None
     */
    void CompleteL();

    /**
     * complete all
     *
     * @since S60 v3.2
     * @return None
     */
    void CompleteAllL();

    /**
     * do complete
     *
     * @since S60 v3.2
     * @return None
     */
    void CompleteSubL();

    /**
     * delete one character
     *
     * @since S60 v3.2
     * @return None
     */
    void HandleBackL();

    /**
     * Update InputContextFieldL
     *
     * @since S60 v3.2
     * @return None
     */
    void UpdateContextFieldL();

    /**
     * Update Popup
     *
     * @since S60 v3.2
     * @return None
     */
    void UpdateCandidateListL();

    /**
     * Cancel Transitory Input
     *
     * @since S60 v3.2
     * @return None
     */
    void CancelL();

private:  //data
    /**
     * The pti engine
     * Not Own
     */
    CPtiEngine* iPtiEngine;

    /**
     * UiLayout
     * Not Own
     */
    CFepUiLayout* iUiLayout;

    /**
     * The input context field
     * Not Own
     */
    CFepInputContextFieldJp* iInputContextField;

    /**
     *  HiraganaKanji Status.
     */
    THiraganaKanjiStatus iStatus;

    /**
     * The candidate popup window
     * Not Own
     */
    CPeninputJapaneseCandidateWnd* iCandidateWnd;

    /**
     * The predictive popup window
     * Not Own
     */
    CPeninputJapanesePredictiveWnd* iPredictiveWnd;

    /**
     * The predictive/candidate array
     * Own
     */
    CDesCArrayFlat* iCandidateArray;

    /**
     * The next Char
     * Own
     */
    HBufC* iChar; // own

    /**
     * The pending event
     */    
    TInt iPendingEvent;  // 0:nothing

    /**
     * The pending event is repeated or not
     */    
    TInt iRepeat;  // 1:repeated

    /**
     * long press timer (Own)
     */
    CAknFepTimer* iLongPressTimer;

    /**
     * repeat timer (Own)
     */
    CAknFepTimer* iRepeatTimer;
    };

#endif //C_CPENINPUTHIRAGANAKANAKANJI_H

//  End Of File
