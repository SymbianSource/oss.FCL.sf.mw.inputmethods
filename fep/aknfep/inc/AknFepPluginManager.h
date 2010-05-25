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
* Description:           
*
*/












#ifndef __AKN_FEP_PLUGIN_MANAGER__
#define __AKN_FEP_PLUGIN_MANAGER__

// INCLUDES
#include <gulicon.h>
#include <barsread.h>
#include <aknfeppensupportinterface.h>
#include <peninputclient.h>
#include <aknextendedinputcapabilities.h>
#include <eikedwob.h>          // for MEikEdwinObserver

#include "AknFepGlobalEnums.h"
#include <aknfeppeninputenums.h>
#include "AknFepPluginMenuManager.h"
#include "AknFepSharedDataInterface.h"
#include "AknFepManager.h"
#include "AknFepCaseManager.h"
#include <peninputsrveventhandler.h>

// FORWARD DECLARATIONS
//class MPeninputServerEventHandler;
class MAknFepManagerInterface;
class CAknFepUiSpellContainer;
class CPenInputGSInterface;
class CConnectAo;

// CLASS DECLARATION


/** 
 * Plugin command type
 */
enum TPluginCmd
    { 
    EPluginCloseMode,
    EPluginSyncFepAwareText,
    EPluginSetDataQueryShown,
    EPluginActivate,
    EPluginMenuCmd,
    EPluginFocusChanged,
    EPluginResourceChanged,
    EPluginFaseSwap,
    EPluginKeyEvent,
    EPluginUpdateIndicator,
    EPluginActivateInGlobalNote,
    EGainForeground,
    ELostForeground,    
    EPluginEditorActivate,
    EPluginForegroundChanged,
    EPluginFocusItemDestroy,
    EPluginLanguageChanged,
    EPluginPreview,
    EPluginPromptChanged,
    EPluginShowCandidate,
    EPluginShowTooltip,
    EPluginHideTooltip,
    EPluginSwitchToPortrait,
    EPluginSwitchToLandscape,
    EPluginSwitchMode,
    EPluginUpdatePenInputITIStatus,
	EPluginEnableFetchDimState,
    EPluginArabicNumModeChanged
    };

/** 
 * Plugin synchronization type
 */
enum TPluginSync
    { 
    EPluginSyncCurSel,
    EPluginSyncUpdatedText,
    EPluginSyncAll
    };
    
#ifdef RD_SCALABLE_UI_V2

/**
 * CAknFepPluginManager class.
 *
 *  @lib AknFep.lib
 *  @since 2.8
 */
class CAknFepPluginManager : 
    public CBase,
    public MAknFepPenSupportInterface,
    public MPeninputServerEventHandler,
    public MEikEdwinObserver
    { 
public:
	
	/**
	 * Simulate event mode
	 * @remark: Used in SubmitUiPluginTextL()
	 */	
	enum TSimulateEventMode
		{
		EIgnor,
		ERawEvent,
		EKeyEvent	
		};
		
     /**
     * NewL first phase construction
     *
     * @param aFepMan fep manager
     *
     */
    static CAknFepPluginManager* NewL( CAknFepManager& aFepMan,
                                      CAknFepSharedDataInterface& aSharedData,
                                      CAknFepLanguageManager& aLangMan,
                                      CAknFepCaseManager& aCaseMan );

     /**
     * Destructor
     */
    virtual ~CAknFepPluginManager();
    
private:

     /**
     * private constructor
     */
    CAknFepPluginManager( CAknFepManager& aFepMan,
                         CAknFepSharedDataInterface& aSharedData,
                         CAknFepLanguageManager& aLangMan,
                         CAknFepCaseManager& aCaseMan );     
    
     /**
     * Construct
     *
     * @param aFepMan fep manager
     *
     */
    void ConstructL();

public :
     
    enum TSpellCBA
        {
        ESpellCBANone,
        ESpellCBACancelEmpty,
        ESpellCBAOkCancel,
        };
    
public:

// From MAknFepPenSupportInterface
    
    /**
     *  From MAknFepPenSupportInterface
     *  Activate pen input method
     *
     *  @since 3.2
     *  @Return None
     */
    void ActivatePenInputL();

    /**
     *  From MAknFepPenSupportInterface
     *  Deactivate pen input method
     *
     *  @since 3.2  
     *  @Return None
     */
    void DeactivatePenInputL();

// From MPeninputServerEventHandler
        
    /**
     * From MPeninputServerEventHandler
     * Handle pen input server events.
     *
     * @since 3.2
     * @param aEventId Event to handle.
     * @return ETrue If server event handled
     */
    TBool HandleServerEventL(TInt aEventId);
    
    /**
     *  Handle events from FEP
     *  suggested mode.
     *  
     *  @since 3.2
     *  @param aEventType, event type from FEP
     *  @param aEventData, event data from FEP
     *  @return ETrue successful, EFalse otherwise
     */
    void HandleEventsFromFepL( TInt aEventType, TInt aEventData );

    /**
     *  Change current input method to plugin input method with
     *  suggested mode.
     *  
     *  @since 3.2
     *  @return ETrue successful, EFalse otherwise
     */
    TBool TryChangePluginInputModeByModeL(TPluginInputMode aSuggestMode, TInt aOpenMode, 
                                          TInt aSuggestRange);
    
    /**
     *  Close plugin input mode, reset iPluginInputMode
     *  to EPluginInputModeNone
     *
     *  @since 3.2
     *  @param aRestore, If ETrue, restor rembered HKB mode
     *  @return None
     */
    void ClosePluginInputModeL(TBool aRestore);
    
        /**
     *  Close plugin input UI, only close UI
     *
     *  @param aResetState If ETrue, reset UI state
     *  to initial.
     *  @return None.
     */
    void ClosePluginInputUiL(TBool aResetState);
       
    /**
     * Handle resource change.
     * 
     * @param aType The resource type
     * @return None.
     */
    void OnResourceChangedL(TInt aType);
         
    /**
     *  Synchronize plugin UI text with editor
     *
     *  @since 3.2
     *  @return None
     */
    void SyncFepAwareText( TPluginSync aSyncType = EPluginSyncCurSel, TBool aSendNow = ETrue );

    /**
     * Processes commands from the edit menu
     * 
     * @since 3.2
     * @param aCommandId The menu command id
     * @return None.
     */
    void ProcessMenuCommandL(TInt aCommandId);

    /**
     * Handle key events
     * 
     * @since 3.2
     * @param aCommandId The key id
     * @return None.
     */
    void HandleKeyEventL(TInt aKeyId);
    
    /**
     * initialize the edit menu depending on fep and editor state
     * 
     * @since 3.2
     * @param aMenuPane The pointer to menu pane
     * @return None.
     */
    void InitMenuPaneL( CAknEdwinState* aEditorState, 
        CAknFepUiInterfaceMenuPane* aMenuPane, TInt aResourceId );
    
    /**
     * Handles notifications that the UI has changed focus
     * 
     * @since 3.2
     * @param aFepFullyConstructed The flag of fep fully constructed
     * @return None.
     */
    void OnFocusChangedL( TBool aGainForeground );

            
    /**
     * Set ICF data
     *
     * @since 3.2
     * @param aTextChanged If editor text changed
     * @param aIcfData ICF data
     * @return None
     */
    void SendIcfDataL(  TPluginSync aSyncType = EPluginSyncCurSel );            
    /**
     * Set editor cursor selection according plugin UI 
     * cursor state
     *
     * @since 3.2
     * @param aCurSel Cursor position.
     * @param aSyncCursor ETrue if plug in UI need update cursor, EFalse otherwise
     * @return None
     */
    void SetCursorSelectionL(const TCursorSelection& aCurSel, TBool aSyncCursor);
    
    HBufC*  GetNumberKeymapResource(TInt aCaseMode);
    
    HBufC*  DoGetNumberKeymapResourceL(TInt aCaseMode);
    
    MAknFepManagerInterface* CurrentFepInputUI();
    
    void SetFingerMatchSelectionState(TBool aMatchStateOn);
    TBool IsVisible();

    // send all predictive candidates at once
    void ShowAllCandidates();
    
    void ResetCandidateList();
    
    void UpdateFSQIndicator();
    void UpdateITUTIndicator();
    
    void SetITUTSpellingStateL(TBool aState);
    
    void DisplaySpellEditor(const TInt aEditorFlag, const TDesC& aInitText, 
                            TCursorSelection aCurSel);
    void DisplaySpellEditorL(const TInt aEditorFlag, const TDesC& aInitText, 
                           TCursorSelection aCurSel);                            
    
    void HideSpellEditor();
    
    HBufC* SpellTextInput();
    void SetTCursorSelection();
    
    void HandleEdwinEventL(CEikEdwin* , TEdwinEvent aEventType);
    
    TInt GetSoftKeyResID();
    
    void HasNoFreeSpace();

    TBool EnterMatchSelectionState();
    
    TPluginInputMode PluginInputMode();
    
    void UpdateCaseMode();
    
    TBool IsGlobleNotes();
    TBool IsDimed();
    /**
     *  Save the cursor visibility. This function is called from fep side, 
     *  because of sync cursor visibility
     *
     *  @since v5.0
     */
    void SetCursorVisibility(TBool aVisibility, TBool aReportPlugin = EFalse);
    /**
     * Get the correct indicator for VKB;
     *
     * @return The correct indicator state for VKB.     
     */
    TAknEditingState VKBIndicatorState( TAknEditingState aOldState );
    void ClosePeninputSetting();

    /**
     * Show tooltip on FSQ to show the best guess word shown inline,
     * when it is necessary
     *
     * @param aText The tooltip text to be displayed
     * @return None
     */
    void ShowTooltipOnFSQL( TInt aSecondaryIdx );    

    /**
     * Hide tooltip on FSQ if necessary
     *
     * @return None
     */
     void HideTooltipOnFSQL();  
     
    /**
     * Open a candidate list to show contents in aItemArray
     *
     * @param aCandidateList ITI candidate data
     * @return None
     */
    void ShowCandidateListL( TFepITICandidateList aCandidateList );
    
    /**
     * Hide candidate list
     *
     */
    void HideCandidateListL();

    /**
     * Commit candidate word on FSQ
     * 
     * @param aIndex the index in candidate lists;
     * @return None
     */    
    void CommitCandidateWordOnFSQL( TInt aIndex );
    
    /**
     * Check if ITI features is enabled when FSQ is opened.
     */
    TBool EnableITIOnFSQ();

    /**
     * Check if ITI can be supported by FSQ.
     */
    TBool IsSupportITIOnFSQ();
    
    /**
     * Restore some configuration after closing FSQ.
     *     
     */
    void ResetItiStateL();    

/*    
#ifdef RD_TACTILE_FEEDBACK
    TBool VKBIsOpened();
#endif // RD_TACTILE_FEEDBACK
*/

    void HandleiDimGainForeground(TBool aGain);

    void OnServerReady(TInt aErr);
    
    /**
     *  Remove last focused window,which launches touch input, from open list.
     *
     *  @since v5.0
     */
    void RemoveLastFocusedWinFromOpenList();
    
public: // inline    
    
    /**
     * Get current plugin input UI
     *
     * @since 3.2
     * @return The pointer to MAknFepManagerInterface
     */      
    inline MAknFepManagerInterface* CurrentPluginInputFepUI();
    
    /**
     * Get current plugin input mode
     *
     * @since 3.2
     * @return current plugin input mode
     */      
    
    inline TBool PluginNativeRange();
    
    inline void SetPluginNativeRange( TBool aPluginNativeRange );
    
    inline TBool SyncWithPluginRange();
    
    inline RPointerArray<HBufC>& MatchCandidateList();    

    inline TBool IsMatchState();  
    
    inline TBool IsSpellVisible();
    
    inline void SetCursorSelection(TCursorSelection& aCursorSel); 
    
    inline TCursorSelection CursorSelection(); 
    
    inline void SetCaseUpdatesSupressed(TBool aCaseUpdatesSupressed);
    inline TBool CaseUpdatesSupressed();
    
    inline void SetBeforeSpell(TBool aBeforeSpell);
    inline TBool BeforeSpell();
    inline HBufC* GetSpellText();
    inline void SpellText(HBufC* aSpellText);
    inline TBool IsNonLatinLanguage(TLanguage aLang);
    inline TInt PreviousPermitMode();
    /**
     *  In T9 predictive text input, if user press "Spell" button, a edit word query dialog is opened.  
     *  This Flag is set for this case to indicate whether edit wod query dialog is opened.
     *  @param aIsEditWordQueryDlg  ETrue means dialog is open,
                                    EFalse means dialog is not open.
     *  @since v5.0
     */    
    inline void SetInEditWordQueryFlag(TBool aIsEditWordQueryDlg );    
    void LaunchPenInputLanguageSelectionL( TBool aLaunchedByTouchWin = EFalse );
    void LaunchPenInputRecognitionWithDictionarySelectionL();
    void LaunchPenInputWritingSpeedSelectionL();
    void LaunchPenInputGuidingLineSelectionL();
  inline TBool IsInGlobleNoteEditor();
    void SetMenuState(TBool aUpdateEditor = ETrue);
    void ResetMenuState(TBool aUnDim = ETrue);
    
    /**
     * When ITI setting opened, set this flag to ETrue
     *  
     * @param aOpen  ETrue means dialog is open,
     *               EFalse means dialog is not open.
     * @return None  
     */    
    inline void ITISettingDialogOpen( TBool aOpen );
    
    /**
     * Check if tooltip box on FSQ is opened.
     *
     * @return if ETrue: Tooltip box on FSQ is enabled.
     */    
    inline TBool IsTooltipOpenOnFSQ();
    
    /**
     * Get whether inMenu has been opened.
     * InMenu is that any pop-up dialog, e.g. menu, SCT, is launched by fep.
     * 
     * @since 5.0
     * @return whether inMenu has been opened
     */     
    inline TBool IsInMenuOpen();
    
    /**
     * Get current plugin input mode even if current window is close.
     * The function is defferent from PluginInputMode(), PluginInputMode() 
     * return EPluginInputModeNone if current input window is closed
     * 
     * @since 5.0
     * @return current plugin input mode
     */      
    inline TPluginInputMode CurrentPluginInputMode();
    
    inline void SetLaunchSCTInSpell( TBool aLaunchSCTInSpell );
private: 

    /**
     *  Initialize plugin UI, set permitted ranges, primary
     *  range, case mode and etc.
     *  
     *  @since 3.2
     *  @Return None
     */
    void InitializePluginInputL(TInt aOpenMode, TInt aSuggestRange, TBool aCleanContent);

    /**
     * Launch plugin menu
     *
     * @since 3.2
     * @param aResource Menu resource id.
     * @return None
     */
    void LaunchPenInputMenuL(TInt aResourceId, TBool aRemeber = ETrue);
        
    /**
     *  Show pen input language setting of general setting view
     *
     *  @since 3.2
     *  @return None
     */
        
    /**
	 *  Show pen input setting of general setting view
     *  @since 3.2
     *  @param aData Key data
     *  @return None
     */
    void LaunchPenInputSettingL();
       
    /**
     * Parse editor text passed to plugin ICF
     * Only return current paragraph of current cursor position.
     *
     *  @since 3.2
     *  @param aBuffer Editor text
     *  @param aCurrent Current cursor position.
     *  @param aFrom Returned paragraph start pos.
     *  @param aTo Returned paragraph end pos.
     *  @return None
     */
    void ParseInputContextContent(TDes& aBuffer, TInt aCurrent, 
                                  TInt& aFrom, TInt& aTo);
    
                                      
    /**
     *  Submit text input from plugin UI to editor
     *
     *  @since 3.2
     *  @param aData Key data
     *  @param aForcedMode Forced simluate event mode.
     *  @return None
     */
    void SubmitUiPluginTextL(const TDesC& aData, 
    						 TSimulateEventMode aForcedMode = EIgnor);

    /**
     *  Handle key event sent from pen input server
     *
     *  @since 3.2
     *  @param aData Key data
     *  @return None
     */
    void OnPenInputServerKeyEventL(const TDesC& aData);   
    
    /**
     *  Sync indicator with plugin range
     *  
     *  @since 3.2
     *  @Return None
     */
    void SyncIndicatorWithPluginRangeL();
     
    /**
     *  Get current suggest mode
     *  
     *  @since 3.2
     *  @param aSuggestMode The mode suggested
     *  @Return None
     */    
    TBool GetCurSuggestMode( TPluginInputMode& aSuggestMode );
    
    /**
     *  Connect server
     *  
     *  @since 3.2
     *  @Return ETrue If connection sucess
     */
    TBool ConnectServer();
    
    /**
     *  Notify layout
     *  
     *  @since 3.2
     *  @param aOpenMode The current input mode
     *  @param aSuggestRange The suggest range
     *  @Return None
     */
    void NotifyLayoutL(TInt aOpenMode, TInt aSuggestRange, TBool aCleanContent);
    
    /**
     *  verify if mode switch button enable
     *  
     *  @since 3.2
     *  @Return ETrue if mode switch button enable
     */
    TBool IsEnableModeSwitchBtn();
    
    /**
     *  verify if setting buttons enable
     *  
     *  @since 3.2
     *  @Return ETrue if setting buttons enable
     */
    TBool IsEnableSettingBtn();
    
    /**
     *  Notify layout the editor's keymapping
     *  
     *  @since 3.2
     *  @Return None
     */
    void NotifyLayoutKeymappingL();
             
    /**
	 *  Test data query show status
     *  @since 3.2
     *  
     *  @return ETrue if data query is showing now, EFalse otherwise
     */
    TBool IsDisplayDataQuery();
    
    /**
	 *  Show pen input setting of general setting view
	 *
     *  @since 3.2
     *  @param aForce ETrue means adjusting data query no matter current showing status,
                      EFalse means adjusting data query window only first time
     *  @return None
     */
    void RepositionDataQuery(TBool aForce);
    
    /**
     * Timer procedure for adjust data query position
     *
     * @param aPtr The pointer of CAknFepManager itself
     * @since 3.2
     * @return 0.(meaningless)
     */
    static TInt AdjustDataQueryCallBackL(TAny *aPtr);
    
    
    static void FepObserverHandleCompletionOfTransactionL(MCoeFepObserver& aFepObserver);
    
    void CreateFingerItutChineseUiL();

    /**
     * Launch Hwr Training UI
     * 
     * @since 3.2
     * @return None
     */
    void LaunchHwrTrainUiL();
    
    TBool GetIndicatorImgID(const TInt IndicatorUID,TInt &aImage, TInt &aMask);
    TBool GetIndicatorImgIDL(const TInt IndicatorUID,TInt &aImage, TInt &aMask);
    
    void SetPromptText( TBool aCleanContent = ETrue );
    
    void SetIcfAlignment();
    
    void ConvertDigitToDigittype(TDigitType& digittype,TDes& aConvertedNumeral);

    void GetAllPredictiveCandidate();
    void GetAllPredictiveCandidateL();
    
    void AddCurrentWinToOpenListL();
    
    void AddWinToOpenListL( RDrawableWindow* aWin);
    
    void RemoveCurrentWinFromOpenList();
    
    TBool IsCurrentWindInOpenList();
    
    TBool ExistInlineText();
    
    TBool SetSyncCurSelIcfDataL( TFepInputContextFieldData& aIcfData );
    
    TBool SetSyncIcfDataL( TFepInputContextFieldData& aIcfData, 
                          const TDesC& aLastEditorContent,
                          const TDesC& aCurrentEditorContent );
                          
    TBool TextInlined( const TDesC& aLastEditorContent, const TDesC& aCurrentEditorContent);
    
    TBool TextInserted( const TDesC& aLastEditorContent, const TDesC& aCurrentEditorContent);
                                          
    TBool TextDeleted( const TDesC& aLastEditorContent, const TDesC& aCurrentEditorContent);

    TBool TextMultiTapped( const TDesC& aLastEditorContent, const TDesC& aCurrentEditorContent);

	void AdjustDataCase( TDes& aData );
	
	TBool IsDimArrowKeys();
	
	void LaunchHelpL();
	void DimAllModeAndCase(CAknFepUiInterfaceMenuPane* aMenuPane);
	
	TInt RepeatStringFromLeft(const TDesC& aSubmitData, TDes& aEditorData);

    void InformMfneUiStatusL( TBool aOpened );
    
    void UpdatePredicState();
                            
    TBool NotifyInGlobalNoteEditorL();

    TBool IsChineseIndicator( TAknEditingState aOldState );
    
    TBool IsEditorCursorDisabled();

    /**
     * Check if the cusror in editor is visible.
     *
     * @return ETrue: the cursos in editor is visible.
     *         EFalse: the cursos in editor is invisible.
     */
    TBool IsEditorCursorVisible();
    

    /**
     * Check if curor selection in editor is visible.
     *
     * @return ETrue: the cursos in editor is visible.
     *         EFalse: the cursos in editor is invisible.
     */    
    TBool IsEditorCursorSelVisible();
       
    /**
     * Get scan code from hw keymapping.
     *
     * @param aKeyCode unicode of each virutal key text.
     */
    TInt GetScanCodeFromHwKeymapping( TUint aKeyCode );
    
    /**
     * Check if those keys are IIT-supported keys
     *
     * @param aKeyEvent The keys to be checked.
     */
    TBool IsITISupportedKey( const TKeyEvent& aKeyEvent );
    
    TPtiTextCase CaseForMappedCharacter(TChar aCharacter);
    
    TBool IsNeedToChangeTextCase( const TPtiTextCase& aTextCase );
    
    /**
     * Do some configuration to make FSQ support ITI features
     *     
     */
    void SetItiStateL();
    
    /**
      * Config keyboard layout for ITI
      *     
      */
    void SetItiKeyboardLayoutL();

    /**
     * Restore keyboard layout after closing FSQ.
     */
    void ResetItiKeyboardLayoutL();
    
    /**
     * Restore predict state.
     */
    void RestorePredictStateL();
    
    /**
     * Check if the keycode belongs to dead keys.
     *     
     * @param aKeyCode: key code to be checked
     * @return ETrue: belongs to ETrue
     *         EFalse: doesn't belong to EFalse;
     */
    TBool IsDeadKeyCode( TUint aKeyCode );
    
    /**
     * Notify app touch window state.
     *     
     * @param aParam
     * @return None
     */
    void NotifyAppUiImeTouchWndStateL( const TBool aTouchState );
        
    /**
     * Check if current editor support partial screen IMEs
     * 
     * @return ETrue if support  
     */
    TBool IsEditorSupportSplitIme();
    
    void SetICFTextForSpellL();
    
    void SendMatchListCmd(const RArray<TPtrC>& aList);
	/**
	 * Initialize the Menu item in Option Menu for Arabic Finger HWR
	 *
	 * @CAknFepUiInterfaceMenuPane* aMenuPane The menu pane used to add more menu items
	 */
	void InitMenuItemForArabicFingerHwrL(CAknFepUiInterfaceMenuPane* aMenuPane);
	
private:    // Data
    
    /**
     * The reference of fep manager
     */
    CAknFepManager& iFepMan;
    
    /**
     * The reference of language manager
     */
    CAknFepLanguageManager& iLangMan;
    
    /**
     * The reference of case manager
     */
    CAknFepCaseManager& iCaseMan;
    
    /**
     * The reference of shared data manager
     */
    CAknFepSharedDataInterface& iSharedData;


    /**
     * Current plugin UI interface
     */
    MAknFepManagerInterface* iCurrentPluginInputFepUI;

    /**
     * Current plugin input mode
     */
    TPluginInputMode iPluginInputMode;
    
    /**
     * Current plugin primary range
     */
    TInt iPluginPrimaryRange;

    /**
     * Update editor cursor timer
     */
    CPeriodic* iAdjustDataQueryTimer;
    
    /**
     * HKB predictive state before starting plugin IME
     */
    TBool iT9UsedBeforePluginInput;
    
    /**
     * Indicating connection state to pen input server
     */
    TBool iAutoCompBeforePluginInput;
    /**
     * Indicating connection state to pen input server
     */
    TBool iPenInputSvrConnected;
    
    /**
     * flags of layout UI submit status
     */
    TInt iLastSubmitCount;
    
    /**
     * Data query showing status
     */
    TBool iDataQueryAlreadyShow;
    
    /**
     * Plugin menu manager object
     */
    CAknFepPluginMenuManager* iPenInputMenu;

    /**
     * Pen input server handle
     */
    RPeninputServer iPenInputServer;
    
    /**
     * flags of language has been changed by setting
     */
    TInt iCurLanguage;
    
    /**
     * flags of fast swap by menu
     */
    TBool iFastSwapByMenu;
    
    MAknFepManagerInterface* iFingerItutChineseUI;
    
    TCursorSelection iCurSelPre;
    
    TCursorSelection iCurSelCur;
    
    TPluginSync iSyncType;
    
    HBufC* iIcfDataBuf;
    
    RPointerArray<HBufC> iCandidateList;
    TInt iCandidateIndex;

    TFepInputAllCandidates iSendAllList;
    
    TBool iMatchState; 
    
    TInt iIndicatorImgID;
    CAknFepUiSpellContainer* iSpell;
    
    TBool iSpellOn; 
    
    TCursorSelection iCursorSel;

    TInt iIndicatorTextID;
    
    TBool iCaseUpdatesSupressed;
    
    TBool iBeforeSpell;
    
    HBufC* iSpellText;
    
    TSpellCBA iSpellCba;
    TBool iPenUiDimmed;
    TBool iIsForeground;
    
    TInt iPreDocumentLengthForFep;
    TInt iOpenPenUiFlag;
    
    TBool iPluginNativeRange;
    
    TBool iSyncWithPluginRange;
    
    RArray<TInt> iOpenWindowList;
    TInt         iLastFocusedWin;
    
    HBufC* iLastEditorContentBuf;
    
    TBool iModeChanged;
    
    TInt iPreviousCoseMode;
    
    TBool iLaunchMenu;
    TBool iInMenu; // need to check whether it can be replaced by iLaunchMenu
    TUint iCurMFNECap;
    MCoeFepAwareTextEditor* iCurEditor;
    MCoeFepAwareTextEditor* iOldFepAwareEditor;

    RDrawableWindow *    iLastDataQueryWin;
    
    TInt iDefaultOrientation;
    RBuf iEventData;
    TBool iDisableSyncText;
    TBool iPendingSyncReq;
    TBool iHasSWEventCap;
    TInt iCurPermitModes;
    TBool isLanuchSCT;
    TBool iInGlobleNoteEditor;
    TBool iOrientationChanged;
    TBool iInlineStateOn;
    TInt iCharStartPostion;
    /**
     * Cursor Visibility
     */
    TInt iEditorPriority;
    TBool iDimGainForeground;
    TBool iEnableIndicatorBtn;
    TBool iEnableSettingBtn;
    TBool iEnableArrowBtn;
    TBool iCursorVisibility;
    TBool iForegroundChange;
    TBool iResourceChange;

    TBool iLaunchHelp;
    TInt iCurPermittedModes;
    MCoeFepAwareTextEditor* iMfne;
    TBool iMfneChanged;
    TBool iIsInEditWordQueryDlg;
    
	TBool iPreferredUiMode;

    CRepository* iAvkonRepository;
    
    TBool iClosePluginInputMode;
	
	TBool iOrientationChangedfromUI;
	
	CPenInputGSInterface* iGsInterface;
    RBuf iPreCaption;
    TBool iIsPassWord;

    /**
     * Store the last keyboard layout.
     */
    TInt iLastKeyboardLayout;
    

    /**
     * Indicate if predictive setting dialog opened.
     */
    TBool iITISettingDialogOpen;
    
    /**
     * Indicate if current input mode is qwerty mode.
     */
    TBool iIsITIConfigured;
    
    /**
     * Indicate if tooltip on FSQ is opened
     */       
    TBool iTooltipOpenOnFSQ;    
	
	TInt iAlignment;

    CConnectAo* iConnectAo;    
	
    /**
     * Indicate if need fetch the dim state from peninputserver
     */ 	
	TBool iNeedFetchDimState;

    /**
     * save the current dim state
     */ 	
    TBool iCurrentDimState;
    
    TBool iLaunchSCTInSpell;
	
    TBool iFocuschangedForSpellEditor;
    };

class CConnectAo : public CActive
    {
public:
	
    CConnectAo(CAknFepPluginManager* aClient);
    
    /**
     * From CActive
     * will be called when stroke timer ends
     *
     * @since S60 v4.0
     */
    void RunL();

    /**
     * From CActive
     * will be called if RunL leaves
     *
     * @since S60 v4.0
     */
    TInt RunError(TInt aError);

    /**
     * From CActive
     * will be called when stroke timer has been cancelled
     *
     * @since S60 v4.0
     */
    void DoCancel();
    void RequestConnect();
    TRequestStatus& RequestStatus(); 
    
    CAknFepPluginManager* iClient;
    };


// ---------------------------------------------------------------------------
// CAknFepPluginManager::CurrentPluginInputFepUI
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//           
inline MAknFepManagerInterface* CAknFepPluginManager::CurrentPluginInputFepUI()
    {
    return iCurrentPluginInputFepUI;
    }
    
inline TBool CAknFepPluginManager::PluginNativeRange()
    {
    return iPluginNativeRange;
    }

inline TBool CAknFepPluginManager::SyncWithPluginRange()
    {
    return iSyncWithPluginRange;
    }

inline void CAknFepPluginManager::SetPluginNativeRange( TBool aPluginNativeRange )
    {
    iPluginNativeRange = aPluginNativeRange;
    }

inline RPointerArray<HBufC>& CAknFepPluginManager::MatchCandidateList()
    {
    return iCandidateList;    
    }

inline TBool CAknFepPluginManager::IsMatchState()
    {
    return iMatchState;
    }

inline TBool CAknFepPluginManager::IsSpellVisible()
    {
    return iSpellOn;
    }

inline void CAknFepPluginManager::SetCursorSelection(TCursorSelection& aCursorSel)
    {
    iCursorSel = aCursorSel;
    }
    
inline TCursorSelection CAknFepPluginManager::CursorSelection()
    {
    return  iCursorSel;
    }
    
inline void CAknFepPluginManager::SetCaseUpdatesSupressed(TBool aCaseUpdatesSupressed)
    {
     iCaseUpdatesSupressed = aCaseUpdatesSupressed;
    }

inline TBool CAknFepPluginManager::CaseUpdatesSupressed()
    {
    return iCaseUpdatesSupressed;
    }    

inline void CAknFepPluginManager::SetBeforeSpell(TBool aBeforeSpell)
    {
    iBeforeSpell = aBeforeSpell;
    }

inline TBool CAknFepPluginManager::BeforeSpell()
    {
    return iBeforeSpell;
    }
    
inline HBufC* CAknFepPluginManager::GetSpellText()
    {
    return iSpellText;
    }

inline void CAknFepPluginManager::SpellText(HBufC* aSpellText)
    {
    if (aSpellText)
        {
        delete iSpellText;
        iSpellText = aSpellText->Alloc();   
        }
    else
        {
        delete iSpellText;
        iSpellText = NULL;
        }
    }

inline TBool CAknFepPluginManager::IsNonLatinLanguage(TLanguage aLang)
    {
    return (aLang == ELangRussian) || (aLang == ELangBulgarian ) ||
           (aLang == ELangUkrainian) || (aLang == ELangUrdu ) ||
           (aLang == ELangVietnamese) || (aLang == ELangThai ) ||
           (aLang == ELangArabic) || (aLang == ELangFarsi ) ||
           (aLang == ELangHebrew) ;
    }
    
inline TInt CAknFepPluginManager::PreviousPermitMode()
    {
    return iCurPermitModes;   
    }
inline void CAknFepPluginManager::SetInEditWordQueryFlag(TBool aIsEditWordQueryDlg )
    {
    iIsInEditWordQueryDlg = aIsEditWordQueryDlg;
    }
    
inline TBool CAknFepPluginManager::IsInGlobleNoteEditor()
    {
    return iInGlobleNoteEditor;    
    }

inline void CAknFepPluginManager::ITISettingDialogOpen( TBool aOpen )
    {
    iITISettingDialogOpen = aOpen;
    }

inline TBool CAknFepPluginManager::IsTooltipOpenOnFSQ()
	{
	return iTooltipOpenOnFSQ;
	}

inline TBool CAknFepPluginManager::IsInMenuOpen()
    {
    return iInMenu;
    }

inline TPluginInputMode CAknFepPluginManager::CurrentPluginInputMode()
	{
	return iPluginInputMode;
	}

inline void CAknFepPluginManager::SetLaunchSCTInSpell( TBool aLaunchSCTInSpell )
	{
	iLaunchSCTInSpell = aLaunchSCTInSpell;
	}

#endif //RD_SCALABLE_UI_V2    

#endif // __AKN_FEP_PLUGIN_MANAGER__

// End of file
