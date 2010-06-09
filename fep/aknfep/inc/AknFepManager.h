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
* Description:           
*       Provides the CAknFepManager definition.
*
*/












#ifndef __AKN_FEP_MANAGER_H__
#define __AKN_FEP_MANAGER_H__

#include "AknFepGlobalEnums.h"          //TKeyPressLength
#include "AknFepManagerUIInterface.h"   //MAknFepManagerUIInterface
#include "AknFepLanguageManager.h"      //CAknFepLanguageManager
#include <gulicon.h>  
#include "aknfepuiinterface.h"
#include "aknfepuimenus.h"
#include <fepbase.h>                    //CCoeFep
#include <coeinput.h>                   //TCoeInputCapabilities
#include <fepitfr.h>                    //MFepInlineTextFormatRetriever
#include <aknedstsobs.h>                //MAknEdStateObserver
#include <uikon.hrh>                    //TAknEditorNumericKeymap
#include <aknEditStateIndicator.h>      //TAknEditingState
#include <frmtlay.h>
#include <biditext.h>
#include <PtiDefs.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <tagmalayoutandsource.h>
#endif

#include <aknextendedinputcapabilities.h>
#ifdef RD_SCALABLE_UI_V2
#include <peninputclient.h>
#endif //#ifdef RD_SCALABLE_UI_V2

#include <AknNullService.h>             // CAknNullService
#include <AknServerApp.h>               // MAknServerAppExitObserver
#include <aknfeppensupportinterface.h>
#include "avkon.hrh"
#ifdef RD_HINDI_PHONETIC_INPUT	
#include <PtiIndicDefs.h>
#endif
#ifdef RD_INTELLIGENT_TEXT_INPUT
// Predictive QWERTY (XT9) changes ---->
class MAknFepUiWordPopupContent;
#include "AknFepCandidatePopup.h"
#include "AknFepCandidatePopupCallback.h"
// Predictive QWERTY (XT9) changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT
class CAknFepPluginManager;
class CAknFepKeyCatcherInterface;
class MAknFepManagerInterface;
class CAknFepIndicator;
class CAknEdwinState;
class CPeriodic;
class MAknEditingStateIndicator;
class CAknFepCaseManager;
class CAknFepHashKeyManager;
class CAknKeySoundSystem;
class CPtiEngine;
class CAknFepSharedDataInterface;
class CApaDocument; // User Dictionary
class CAknFepInlineTextDecorator;
class CTextView;
class MPeninputServerEventHandler;
class MAknFepDocumentNavigation;
class CAknFepUIInterface;
#ifdef RD_INTELLIGENT_TEXT_INPUT
class CAknFepFnKeyManager;
#endif
//phrase creation
class CZhuyinKeyHandler;
class CAknFepZhuyinAnalyser;
class CZhuyinAnalyserDbFeed;
//User DB view
class CAknFepUserdbDlg;
class CAknFepThaiSCTSelector;
class CAknNavigationDecorator;
// This could be moved inside FepManager class?
enum TWesternNaviEvent
    {
    ENullNaviEvent,
    ELeftNaviEvent,
    ERightNaviEvent,
    EBackspaceEvent,
    EUpNaviEvent,
    EDownNaviEvent
    };
enum TSpecialTextField
		{
 		/**
 		* Constraints
 		*/
    ESplAny,
    ESplMailAddr,
    ESplNumeric,
    ESplPhoneNumber,
    ESplUrl,
    ESplDecimal,
    /**
     * Modifiers
     */
    ESplPassword=0x10000,
    ESplUneditable=0x20000,
    ESplSensitive=0x40000,
    ESplNonPredictive=0x80000,
    ESPlInitialCapsWord=0x100000,
    ESplInitialCapsWordSentence=0x200000,
    ESplConstraintMask=0xFFFF
 		};    

const TInt KChrKeyMultitapTimeout = 1000000;
/**
 * Translates ET9Eng language codes into localised descriptors and command Id's
 */
class TFepLanguageTranslation
    {
    public:
        void ReadFromResource(TResourceReader& aReader);
    public:
        TPtrC           iName;
        TInt            iLanguageId;
        TInt            iCommandId;
    };
class CChrMultiTapTimer : public CTimer
    {
    public:
        static CChrMultiTapTimer* NewL(TCallBack aCallback);
        void After(TTimeIntervalMicroSeconds32 aInterval);
    private:
        CChrMultiTapTimer(TCallBack aCallback);
        void ConstructL();
        // from CActive
        virtual void RunL();
    private:
        TCallBack iCallback;
    };
/**
 * The CCoeFep derived class of AknFep
 * Controls mode in the Fep. Handles all interaction with the editor
 * Creates and owns the UI Manger, the Key Catcher and the edit state indicators
 */
class CAknFepManager :
    public CCoeFep,
    public MAknFepManagerUIInterface,
    public MFepInlineTextFormatRetriever,
    public MFepPointerEventHandlerDuringInlineEdit,
    public MAknFepUiInterfaceMenuObserver,
    public MAknServerAppExitObserver,   //embedding
    private MAknEdStateObserver,
#ifdef RD_SCALABLE_UI_V2  
    public CAknExtendedInputCapabilities::MAknEventObserver,
#endif
    private MFormCustomDraw
#ifdef RD_INTELLIGENT_TEXT_INPUT
    ,public MAknFepCandidatePopupCallback // Predictive QWERTY (XT9) changes
#endif //RD_INTELLIGENT_TEXT_INPUT
    {

private:
    class TTextDirectionalInfo
        {
        public:

            enum TTextDirectionalStatus
                {
                EUnknown,
                    ENotInDocument,
                    ENoDirectionalChunks,
                    ERightToLeft,
                    ELeftToRight
                };

            enum TDirectionalBlockEnvironment
                {
                EUndefined,
                    EInsideBlock,
                    EAtRightEndOfLine,
                    EAtLeftEndOfLine,
                    EAtRightEndOfBlockFacingOppositeDirectionalBlock,
                    EAtLeftEndOfBlockFacingOppositeDirectionalBlock,
                    EAtRightEndOfBlockFacingSameDirectionBlock,
                    EAtLeftEndOfBlockFacingSameDirectionBlock
                };
        public:
            // Constructor
            TTextDirectionalInfo();
            void SetDirectionFromChunk( const CTmTextLayout::TTmChunkDescription& aChunk);
        public:
            TTextDirectionalStatus iDirectionalStatus;
            TDirectionalBlockEnvironment iBlockEnvironment;
        };

public:
    /**
     * values for the flags required to maintain the internal
     * state of the Fep Manager during operation
     */
    enum
        {
        EFlagInsideMultitapInlineEditingTransaction     =0x00000001,
        EFlagLaunchEditMenu                             =0x00000002,
        EFlagLongShiftKeyPress                          =0x00000004,
        EFlagNoActionDuringShiftKeyPress                =0x00000008,
        EFlagShiftKeyDepressed                          =0x00000010,
        EFlagInsideInlineEditingTransaction             =0x00000020,
        EFlagPassNextKey                                =0x00000040,
        EFlagQwertyChrKeyDepressed                      =0x00000080,  // Not used. Qwerty mode is not supported.
        EFlagLineFeedCharacter                          =0x00000100,
        EFlagEditorFull                                 =0x00000200,
        EFlagNewSharedDataInputMode                     =0x00000400,
        EFlagNoActionDuringChrKeyPress                  =0x00000800,

        EFlagSupressAutoUpdateAtEditorStart             =0x00001000,
        EFlagNoMatches                                  =0x00002000,
        EFlagSuppressInputStateChange                   =0x00004000,
        EFlagCompoundWord                               =0x00008000,
        EFlagChangeInputMode                            =0x00010000,
        EFlagForegroundUIComponentVisible               =0x00020000,
        EFlagMenuPaneVisible                            =0x00040000,
        EFlagRemoveMatchesMenuItem                      =0x00080000,
        EFlagInlineEditInBackground                     =0x00100000,
        EFlagNewSharedDataInputLanguage                 =0x00200000,
        EFlagLastCharacterInEditor                      =0x00400000,

        EFlagSpaceCharacter                             =0x00800000,        // adding for japanese
        EFlagFullWidthSpaceCharacter                    =0x01000000,        // adding for japanese
        EFlagBidiCursorIsInitialized                    =0x02000000,
        EFlagCursorPointsRightToLeft                    =0x04000000,
        EFlagAtDirectionallyAmbiguousPoint              =0x08000000,
        EFlagCharacterAdded                             =0x10000000,
        EFlagNoInlineEditFormatting                     =0x20000000,  // Not really used yet!
        EFlagQwertyShiftMode                            =0x40000000,  // Not used. Qwerty mode is not supported.
        EFlagSupressAutoUpdate                          =0x80000000
        };
    
     enum
     	{
		EExtendedFlagShortPressHashKey                    =0x00000001,
     	EExtendedFlagNoModeChange                         =0x00000002,
     	EExtendedFlagShiftReleasedOnPopup                 =0x00000004,
     	EExtendedFlagOkKeyPressOnCandidatePopup           =0x00000008,
     	/* Flag to take care of graceful exit for FEP launched dialog when the underneath editor is closed */
        EExtendedFlagEdwinEditorDestroyed                 =0x00000010,
        /* Flag to set decide when to set multiple observer for FEP. Set this flag whenever a FEP aware dialog is launched */
        EExtendedFlagFepAwareDialogLaunched               =0x00000020,
        /* Flag to specify that pointer event type EDrag has happened */ 
        EExtendedFlagPointerEventTypeEDrag                =0x00000040
    	};		

    //public constants

    enum
        {
        ESingleCharacter                            =1,
        EMaximumFepWordLength                       =63
        };

private:
    /**
     * private constants
     */
    enum
        {
        EShiftKeyRepeatDelay                        =800000, // 0.8s
        EQueryBufferLength                          =63,
        EDefaultNumericCharMapResId                 =0,
        EDefaultSCTResourceId                       =-1,
        ENoCharacters                               =0,
        EAknFepUid                                  =0x100056de,
        EStarKeyUnicodeValue                        =0x2A,
        EHashKeyUnicodeValue                        =0x23,
        ELeftSoftkeyIndex                           =CAknFepUIInterface::ELeftSoftkeyIndex,
        ERightSoftkeyIndex                          =CAknFepUIInterface::ERightSoftkeyIndex,
        EMiddleSoftkeyIndex                         =CAknFepUIInterface::EMiddleSoftkeyIndex,
        EWordConcatenationTimeout                   =2000000, // 2.0s
        ELanguageArrayGranularity                   =20,
        EChrKeyRepeatDelay                          =1000000, // 1.0s
        EHalfQwertyChrKeyRepeatDelay                =600000   //0.6 secs from Default actions UI spec
        };

    enum TChineseFepCursorType
        {
        EActive,
        EPassive
        };
        
public:            
	enum
		{
		ECcpuStateNone       = 0,
		ECcpuStateStartCopy  = 0x01,
		ECcpuStateCopy       = 0x02,	
		ECcpuStateStartCut   = 0x04,
		ECcpuStateCut        = 0x08,
		ECcpuStateButton     = 0x10,
		ECcpuStatePosted               = 0x20,
		ECcpuStateHashDown             = 0x40,
		ECcpuStateSelectionEventPosted = 0x80,
		ECcpuStateEdwinInSelectionMode = 0x100,
		ECcpuStateIgnoreStarUp          = 0x200,
		ECcpuSupressEditMenuFromShiftUp = 0x400,		
		ECcpuStateCbaSymbol             = 0x800,
		ECcpuStateIgnoreNextFocusChange = 0x1000,
		ECcpuStataCommitPredictiveWord  = 0x2000,
		ECcpuStateIgnoreNextEdwinSyncEvent = 0x4000,
		ECcpuStateLosingFocus			   = 0x8000,	
		ECcpuStateUncommitWhenFocused = 0x10000,				
        ECcpuStateHashKeyDeleteDone = 0x20000,
        ECcpuStateSupressCursorMoveToEnd = 0x40000,
        ECcpuStateChangeToPredictionMode = 0x80000,
		ECcpuStateNewPredictiveWord = 0x100000,
		ECcpuStateShiftkeyWasPressedBeforeLosingFocus = 0x200000,
		ECcpuStateCbaShown = 0x400000
        };

	enum TShowSctMode 
	    {
	    EShowSctFirst=0, 
	    EShowPctFirst, 
	    EShowSmileyFirst 
	    };
	
private:
	 // These are needed to remember editor's ccpu state 
	 // for editing options submenu.
     enum
     	{
     	ECcpuStatusFlagCanCopy  = 0x01,
     	ECcpuStatusFlagCanCut   = 0x02,
     	ECcpuStatusFlagCanPaste = 0x04,     	
     	};
  

private:
    class TLanguageCapabilities
        {
        public:
            TInt iInputLanguageCode;        // Symbian language code.
            TBool iLocalInputLanguageInUse;
            TBool iArabicIndicDigitsAllowed;
            TBool iEasternArabicIndicDigitsAllowed;
            TBool iIndicDigitsAllowed;	
            TDigitType iLocalDigitType;
            TDigitType iNumericEditorDigitType;
            TBool iSupportsCase;
            TBool iSupportsWesternPredictive;
#ifdef RD_INTELLIGENT_TEXT_INPUT
            // Predictive QWERTY changes ---->
            TBool iSupportsWesternQwertyPredictive; // predicitve QWERTY
            // Predictive QWERTY changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT
            TBool iRightToLeftLanguage;
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef __HALF_QWERTY_KEYPAD            
            TBool iSupportsWesternHalfQwertyPredictive;
#endif //__HALF_QWERTY_KEYPAD            
#endif //RD_INTELLIGENT_TEXT_INPUT 
        };

public:
    /**
     * C++ Constructor
     */
    CAknFepManager(CCoeEnv& aConeEnvironment);

    /**
     *  Second phase construction
     */
    void ConstructL(const CCoeFepParameters& aFepParameters);

    /**
     * destructor
     */
    virtual ~CAknFepManager();


public: //from MAknFepManagerUIInterface
#ifdef RD_INTELLIGENT_TEXT_INPUT
     /**
     Returns the keyboard layout
     */
     TPtiKeyboardType KeyboardLayout() const;
     /**
     Returns the current keyboard layout in use.
     */	 
     CAknFepFnKeyManager::TFnKeyState FnKeyState();
     void SetFnKeyState(CAknFepFnKeyManager::TFnKeyState aState);
    /**
     * Returns the resource ID of the numeric editor in use
     */
    
    TInt GetNumericSCTResID();
    

#ifdef __REVERSE_FN_KEY_SUPPORTED
     /**
    Returns if the input is in reverse Fn key input mode.
    */
    virtual TBool IsReverseFnkeyInput();
    
    void SetReverseFnkeyInputMode(TBool iIsOn);
    
    /**
     * Returns true if the aChar is allowed in the numeric editor
     */
    TBool IsValidCharInNumericEditorL( TChar aChar ) const;
    
    /**
     * Filter the char for Fn reverse mode in case of
     * numeric editor. This function should call
     * in case of numeric editor reverse mapping.
     */
    TBool IsValidCharForNumericInFnReverseL(TInt aKey, TPtiTextCase aCase) ;

#endif //__REVERSE_FN_KEY_SUPPORTED
    virtual void AddOneSpaceOrMoveCursorL();
#endif
	virtual TBool IsAllowedKeymappingForNumberMode(TChar aChar) const;
	/**
     * closes the UI
     * commits any character in multitap
     */
    virtual TBool TryCloseUiL();

    /**
     * Used by the UI manager to inform the Fep Manager that the UI has activated.
     */
    virtual void SendUIActivatedNotification();

    /**
     * Used by the UI manager to inform the Fep Manger that the UI has not activated.
     */
    void SendUIDeActivatedNotification();


    /**
     * Plays sound
     */
    virtual void PlaySound(TAvkonSystemSID aSound) const;
    
    /**
     * Set current long clear key press
     */     
    virtual void SetLongClearAfterCloseUI(TBool aLongClear);

	/**
	 * Align the Logical and Visual cursor positions.
	 *
     * @since 3.2
     * @param aType Cursor type.
     * @param aToLeft ETrue if the position to the left is to be found,
     *			 	  EFalse if the position to the right is to be found.
     * @return None
	 */
	void AlignLogicalAndVisualCursorL( TTmDocPosSpec::TType aType, TBool aToLeft );
	
	/**
	 * This method removes the Repha character. The function goes to the 
	 * start of the syllable, checks if repha character is present and 
	 * removes it.
	 *
	 * @since 3.2
	 * @param None.
	 * @return None.
	 */							
	void RemoveRephaCharacterL();
	
	/**
	 * Checks if the Repha character has been attached to the syllable
	 *
	 * @since 3.2
	 * @param None.
	 * @return ETrue if present else EFalse.
	 */	
	TBool IsRephaPresent();
	
	/**
	 * Retrieve the Previous to previous character during multitapping.
	 *
	 * @since 3.2
	 * @param aContextSensitive EFalse is the default parameter.
	 *						    ETrue is used for requesting the function
	 *						    to check for extraction of the character
	 *						    two positions before the seleted text.
	 *
	 * @return The previous to previous character if any.
	 */
	TText PreviousToPreviousChar( TBool aContextSensitive = EFalse );

	 /**
	 * This method is called for removing the Rakar character. The function 
	 * checks if Rakar (Virama+Ra)  is present.
	 *
	 * @since 3.2
	 * @param None.
	 * @return None.
	 */
	void RemoveRakarCharacterL();

    /**
	 * This method is called for removing the N characters from the 
	 * specified position. If the text to be removed is not yet
	 * committed, it is updated with NULL descriptor and committed.
	 *
	 * @since 5.0
	 * @param TInt aPos position to start searching from. By default
	 *              Searching will start from the cursor position.
	 * @param TInt aNumOfCharacters Number of characters to remove.
	 * @param TBool Should cursor be positioned at the original location.
	 * @return None.
	 */
    void RemoveTextFromEditorL( TInt aNumOfCharacters, TInt aPos = 0, 
                                TBool aIsToAlignCursor = ETrue );
    
   	/**
	 * Align the Logical and Visual cursor positions.
	 * Overloaded method which uses FindDocPos
	 *
     * @since 5.0
     * @param None
     * @return None
	 */
	void AlignLogicalAndVisualCursorL();
	
	CAknFepPluginManager* PluginUIManager();
    //Phrase creation	
	MZhuyinKeyHandler* ZhuyinKeyHandler();
	CAknFepZhuyinAnalyser* ZhuyinAnalyser();
	void UpdateStickyState();
	
#ifdef RD_MARATHI
	/**
	 * This function checks if the previous two characters are
	 * the combination of Chandra-A
	 *
	 * @since 5.0
	 * @param None
	 * @return TBool.
	 */
	TBool IsChandraAPresentL();

	/**
	 * This function checks if the previous Chandra-A is present
	 * before the cursor position
	 *
	 * @since 5.0
	 * @param None
	 * @return TBool.
	 */
	TBool IsEyeLashRaPresentL();
#endif // RD_MARATHI
#ifdef RD_INTELLIGENT_TEXT_INPUT
    /**
	 * This function returns the current case.
	 * Required for Shift key handling.
	 *
	 * @since 3.2
	 * @param None
	 * @return TBool.
	 */
	TInt GetCurrentCase();
	
	 /**
	 * This function set the flag of chr composition key
	 * in mini qwerty
	 *
	 * @since 3.2
	 * @param aFlag ETrue if it is chr composition key.
	 * @return None.
	 */
	void  SetComposChrFlag( TBool aFlag );
	
	/**
	 * This function get the flag of chr composition key
	 * in mini qwerty
	 *
	 * @since 3.2
	 * @param None.
	 * @return ETrue if it is chr composition key..
	 */
	TBool GetComposChrFlag();
	/**
	 * Hides the exact word popup.
	 */
	void HideExactWordPopUp();
	/**
    * @return   ETrue   If the exact word pop up is shown.
    */
	
	TBool IsExactWordPopUpShown();		
#endif
#ifdef RD_HINDI_PHONETIC_INPUT
    //hindi_phonetic_fixes
   	/*
    * This functions toggles the case from upper to lower
    * @since 5.0
    * @param None
    * @return  void
    */
    virtual void HandleIndicCaseL();
#endif

public: //interface to the key catcher
    /**
     * Handles key events passed from the Key Catcher, excluding the shift key event
     * which is dealt with separately in the HandleShiftKeyEventL function.
     * State will always be Intital or UIActive.
     */
    TKeyResponse HandleKeyEventL(TUint aCode, TKeyPressLength aLength, TEventCode aEventCode = EEventKey);

    /**
     *  Shift keys are dealt with in a different way to other keys, the action is on the up
     *  key event.
     */
    TKeyResponse HandleShiftKeyEventL(TEventCode aEventCode);

    /**
     *  Thai 0 key is handled differently. If 0 key is up before long keypress, we show SCT.
     */
    TKeyResponse HandleThai0KeyEvent(TEventCode aEventCode, TKeyPressLength aLength, 
                                     TBool& aThai0KeyHandling);

    /**
     *  Chr key handling
     */
    TBool HandleQwertyChrKeyEventL( TEventCode aEventCode );
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef __HALF_QWERTY_KEYPAD
	/**
	*	Chr Key handling in half qwerty
	*/
	TKeyResponse HandleChrKeyForHalfQwertyL( TEventCode aEventCode);
#endif //__HALF_QWERTY_KEYPAD
#endif //RD_INTELLIGENT_TEXT_INPUT    
    /**
    *  Control key handling
    */
    TKeyResponse HandleQwertyControlKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aEventCode);

    /**
    * Handles key events for qwerty input mode.
    *
    * @since 3.0
    * @param aKeyEvent An event to be handled.
    * @param aResponse Repsonse (consumed / not consumed) will be stored here.
    * @return ETrue if evenat was handled by this method.
    *         EFalse otrherwise.
    */
    TBool HandleQwertyKeyEventL(const TKeyEvent& aKeyEvent, TKeyResponse& aResponse);
    
  	/**
    * Returns boolean value indicating whether it is ok to set edwin into "copy/paste"
    * mode.
    *    
    * @since 3.1
    * @return ETrue if it is ok to editor into selectuion mode.
    *         EFalse otherwise.
    */
	TBool OkToActivateSelectionMode() const;    
	
    /**                
    * Returns the status of arabic-indic digit mode setting.
    *
    * @since 3.0
    */
    TBool ArabicIndicDigitsInUse() const;    	
	
	/**                
    * Returns the status of Eastern arabic-indic digit mode setting.
    *
    * @since 3.0
    */
	TBool EasternArabicIndicDigitsInUse() const;
	void NumberModeChangeGSNotification();
	void DimInputmodeMenuItems(CAknFepUiInterfaceMenuPane* aMenuPane);
	 
	 /**
     * Dims the item corresponding to the mode aMode, in the edit mode menu
     *
     * @param aMenuPane the edit menu pane
     * @param aMode the mode to be dimmed
     */
    void DimMenuItem(CAknFepUiInterfaceMenuPane* aMenuPane, TInt aMode);
    
    TInt FepShowVkbPreviewStatus();

    TInt PluginInputMode() const;
private: // from CCoeFep
    /**
     * from CCoeFep - not used
     */
    virtual void CancelTransaction();

    /**
     * from CCoeFep - not used
     */
    virtual void IsOnHasChangedState();

    /**
     * from CCoeFep - not used
     */
    virtual void OfferKeyEventL(TEventResponse& aEventResponse, const TKeyEvent& aKeyEvent, 
                                TEventCode aEventCode);

    /**
     * from CCoeFep - not used
     */
    virtual void OfferPointerEventL(TEventResponse& aEventResponse, 
                                    const TPointerEvent& aPointerEvent, 
                                    const CCoeControl* aWindowOwningControl);

    /**
     * from CCoeFep - not used
     */
    virtual void OfferPointerBufferReadyEventL(TEventResponse& aEventResponse, 
                                               const CCoeControl* aWindowOwningControl);

private:    // from MFepAttributeStorer (via CCoeFep)
    /**
     * from MFepAttributeStorer (via CCoeFep) - not used
     */
    virtual TInt NumberOfAttributes() const;

    /**
     * from MFepAttributeStorer (via CCoeFep) - not used
     */
    virtual TUid AttributeAtIndex(TInt aIndex) const;

    /**
     * from MFepAttributeStorer (via CCoeFep) - not used
     */
    virtual void WriteAttributeDataToStreamL(TUid aAttributeUid, RWriteStream& aStream) const;

    /**
     * from MFepAttributeStorer (via CCoeFep) - not used
     */
    virtual void ReadAttributeDataFromStreamL(TUid aAttributeUid, RReadStream& aStream);

private:    // from MCoeForegroundObserver (via CCoeFep)
    /**
     * from MCoeForegroundObserver (via CCoeFep) - not used
     */
    virtual void HandleGainingForeground();

    /**
     * from MCoeForegroundObserver (via CCoeFep) - not used
     */
    virtual void HandleLosingForeground();

private:        // from MCoeFocusObserver (via CCoeFep)

    virtual void HandleChangeInFocus();

    /**
     * in queries the editor is destroyed before the call to HandleChangeInFocus
     * so NULL the iInputCapabilities variables to prevent trying to save to an unreferenced object
     */
    virtual void HandleDestructionOfFocusedItem();

public: // from MAknEdStateObserver
    /**
     * from MAknEdStateObserver - not used
     */
    virtual void HandleAknEdwinStateEventL(CAknEdwinState* aAknEdwinState, 
                                           EAknEdwinStateEvent aEventType);

private:    // from MEikCommandObserver (via MAknFepUiInterfaceMenuObserver)
    /**
     * from MEikCommandObserver
     * Processes user commands, in this case only commands from the edit menu
     */
    virtual void ProcessCommandL(TInt aCommandId);

    /**
     * Constructs the component fully. This method is called when a
     * fep aware editor is first time focused for text input.
     */
    void ConstructFullyL();

    /**
     * Deletes the objects constructed with ConstructFullyL() method.
     */
    void CommonDestroyFep();

private:    // from MAknFepUiInterfaceMenuObserver
    /*
     * not used as DynInitMenuPaneL() is now called before SetEmphasis() and HandleChangeInFocus()
     */
    virtual void SetEmphasis(CBase* aMenuControl,TBool aEmphasis);

    /**
     * removes app menu panes and adds the FEP edit menu pane to the menu bar
     */
    virtual void DynInitMenuBarL(TInt aResourceId,CAknFepUiInterfaceMenuBar* aMenuBar);

    /**
     * dynamically changes the edit menu depending on fep and editor state
     */
    virtual void DynInitMenuPaneL(TInt aResourceId,CAknFepUiInterfaceMenuPane* aMenuPane);

private:        //from MFepPointerEventHandlerDuringInlineEdit
    /**
     * from MFepPointerEventHandlerDuringInlineEdit - not needed, has to be implemented for MCoeFepAwareTextEditor interface
     * No pointer in Chinese Series 60
     */
    virtual void HandlePointerEventInInlineTextL(TPointerEvent::TType aType, TUint aModifiers, 
                                                 TInt aPositionInInlineText);

private:        //from MFepInlineTextFormatRetriever
    /**
     * from MFepInlineTextFormatRetriever - not needed, has to be implemented for MCoeFepAwareTextEditor interface
     */
    virtual void GetFormatOfFepInlineText(TCharFormat& aFormat, 
                                          TInt& aNumberOfCharactersWithSameFormat, 
                                          TInt aPositionOfCharacter) const;
#ifdef RD_INTELLIGENT_TEXT_INPUT

// Predictive QWERTY changes (XT9) ---->
public:         
	//from MAknFepCandidatePopupCallback ---->
    /**
    * Callback the get the current set of candidates
    * @param    aArray      An output parameter for the candidate strings
    * @param    aActiveIdx  An output parameter for the index of the 
    *                       currently active candidate
    */
    virtual void GetCandidatesL( CDesCArray& aArray, TInt& aActiveIdx );
	
	/**
	* Callback get new position for candidate list
	* @param aRect  New rect position for candidate list.
	*/
    virtual void GetUpdateCandidatePositionL(TRect& aRect);
    
	//from MAknFepManagerUIInterfaceWestern ---->
    
    /**
     * Function to get the text direction for current language.
     * @return ETrue if language id RTL, EFalse if it is not
     */
    TBool IsRightToLeftLanguage();
    TBool IsLanguageSupportPrediction();
// Predictive QWERTY changes (XT9) <----
#endif //RD_INTELLIGENT_TEXT_INPUT

public:

    /**
     * Launches the mode selection menu
     */
    void LaunchSelectModeMenuL();
    
    /**
     * Launches the edit word query in western predictive text mode
     */
    void LaunchEditWordQueryL();
    
	/**
	* Returns the status of given extended input capability flag.
	*/
    TUint ExtendedInputCapabilities() const;	

	/**
	* Exit plugin itut spell mode by press ok btn
	*/
    void ExitPluginSpellModeByOk();

	/**
	* Exit plugin itut spell mode by press cancel btn
	*/
    void ExitPluginSpellModeByCancel();
    
    void SynCCPSoftKey();
    
     /**
     * checks that the editor the editor has free space to add another character
     * An editor of unlimited length will return its maximum size as zero
     */
    TBool EditorHasFreeSpace( TInt aNumberOfCharacter = 0 ) const;
		TBool IsSpecialNumericEditor();
		
	void TryPopExactWordInICFL();
	
	void HandleChangeInFocusForSettingFep();

private:

    /**
     * Handles notifications that the UI has changed focus
     * <ul>
     * <\li>When moving away from a fep aware text editor saves the mode of the old editor
     * <\li>When moving to fep aware text editor gets the the mode if it has previously been saved
     * and checks if the editor supports secret text
     * <\ul>
     */
    void HandleChangeInFocusL();

    /**
     * cleans up the fep after trapping a leave
     */
    void CleanUpFep();

    /**
     *
     * @param aKeyCode the value of the key to simulate
     */
    void SimulateKeyEventL(TUint aKeyCode, TBool aActiveObj=EFalse);

    /**
     * Tries to Reset the Shift Key Monitor
     * Cleans up FEP if failed
     *
     * @return KErrNone if succeed, KErrDied if failed
     */
    static TInt ResetShiftKeyMonitorCallback(TAny* aObj);

    /**
     * Cancels the shift monitor
     * tells the editor to up date its cba
     * to enable copy/paste support on cba. We simulate via CCoeEnv
     * to avoid fep SimulateL adding shift modifiers
     */
    void ResetShiftKeyMonitorL();

    /**
     * Cancels the shift key and concatenation timers if they are active
     */
    void CancelAllTimerActivity();

    /**
     * checks whether a mode is permitted by the current editor
     *
     * @return ETrue is aMode is permitted, EFalse if it is not
     */
    TBool IsModePermitted(TInt aMode, TWidthChar aWidth = ENoneWidthChar) const;

    /**
     *
     *
     * @return
     */
    TUint EditorMode(TInt aMode, TWidthChar aWidth) const;

    /**
     * Synchronises the states of the Fep Manager and the Key Catcher
     *
     * @return System wide error code.
     */
    TInt SyncStates(TAknFepManagerState aState);

    /**
     * Launches the matches popup list
     */
    void LaunchMatchesPopupListL();

#ifdef RD_INTELLIGENT_TEXT_INPUT
    // Predictive QWERTY (XT9) changes ---->
    /**
     * Launches the compact match candidate list which follows the inline editor.
     * @param   aFocusedIndex   The index of the word focused by default. 
     *                          If KErrNotFound is given, the currently active
     *                          word will be focused.
     */
    void LaunchCandidatePopupListL( TInt aFocusedIndex = KErrNotFound );
    void StartInlineEditingWithSelectedWord(TDesC& aTextToUncommit);
    /**
     * Shows the exactly typed word above the inline editor if it differs from the 
     * best guess word shown inline.
     */
    void ShowExactWordPopupIfNecessaryL();
    
    /**
     * Launches the predictive setting dialog.
     */
    void LaunchPredictiveSettingDialogL();
#ifdef FF_DUAL_LANGUAGE_SUPPORT
    void  LaunchWritingLanguageSettingDialogL();
    
    void  LaunchDualLanguageSettingDialogL();
#endif //FF_DUAL_LANGUAGE_SUPPORT
    
    // Predictive QWERTY (XT9) changes <----
    
    /**
     * Handle shift loop in chinese variant for half qwerty
     */
    
    void HandleShiftHalfQwertyChineseL(TEventCode aEventCode);
    
    /**
     * Handle shift key loop in chinese variant for qwerty and mini qwerty
     */
    void HandleShiftQwertyChineseL(TEventCode aEventCode);
#endif //RD_INTELLIGENT_TEXT_INPUT    
    
    void GetCandidatesWithIndexL(CDesCArray* aArray, 
						TInt& aActiveIdx, TInt& aSecondaryIdx);
    void TryPopExactWordInOtherPlaceL();
    
    /**
     * Launches the insert word query in western predictive text mode
     */
    void LaunchInsertWordQueryL(const TDesC& aInitialText, TCursorSelection aTextSpanToReplace);

    /**
     * Actual query dialog for LaunchInsertWordQueryL and LaunchEditWordQueryL
     */
    void LaunchFepQueryDialogL(TInt aResourceId, const TDesC& aInitialText, 
                               TCursorSelection aTextSpanToReplace);

     /**
     * Inserts a text directly into the editor which previously had focus
     */
    void InsertTextFromDialogL(const TDesC& aTextFromDialog, 
                               TCursorSelection aExtentOfTextToReplace);

    /**
     * Launch the KutenCodeQuery
     */
    void LaunchKutenCodeQueryL();

    /**
     * Returns the resource ID for the numeric mode special character map
     * available in the current editor.
     */
    TInt NumericModeSCTResourceId() const;

    /**
    * Handles the hash key
    * <ul>
    * <\li>a short hash cycles through the modes
    * <\li>a long key press moves to number mode unless the mode is already number, in which case
    * it goes to default
    * <\ul>
    */
    TKeyResponse HandleHashKeyL(TKeyPressLength aLength);

    /**
    *
    * @return Editor's numeric keymap for # and * keys of ITU-T mode.
    */
    TInt EditorNumericKeymap() const;

    /**
     * dims the item on the edit menu corresponding to the current mode
     * the user can press cancel to remain in the same mode
     *
     * @param aMenuPane the edit menu pane
     */
    void DimEditMenuModeItems(CAknFepUiInterfaceMenuPane* aMenuPane);

    /**
     * Launch confirmation note.
     *
     * @param aResourceId Confirmation note resource Id.
     */
    void LaunchConfirmationNoteL(TInt aResourceId);

    /**
     * Enters a new character into the editing window
     * <p> Secret Text Editors do not use the MCoeFepAwareTextEditorInterface,
     * they interact with the editor by simlulating key events. When in Multitap, simultating a
     * numeric key event causes an extra key to be added to the control stack because the numeric
     * keys exist on the Series 60 keypad; the flag EFlagPassNextKey is used to ignore this.
     * <p> When secret editing in multitap, a backspace is simulated to replace the last
     * character in the editor.
     */
    void NewCharacterL(const TDesC& aChar);
    /**
     * Enters a new phrase into the editing window
     *
     */
    void NewTextL(const TDesC& aText);

    /**
     * Commits given string in the editing window. This function checks that
     * the editor has enough space to insert given string. If it not,
     * given string is cut, then it is committed.
     */
    void CommitInlineEditL(const TDesC& aText, TInt aUncommitedTextChange);

    /**
      * Cansel inline input.
      */
    void CancelInlineEdit();

    /**
      * Used by editing window when it draws inline input characters.
      */
    void GetScreenCoordinatesL(TPoint& aLeftSideOfBaseLine, TInt& aHeight,
                               TInt& aAscent, TInt aDocumentOffset);

    /**
      * Used by Chinese UI to get cursor baseline position.
      */
    void GetScreenCoordinatesL(TPoint& aLeftSideOfBaseLine,TInt& aHeight,TInt& aAscent);



    /**
     * checks that the editor the editor has free space to add another character
     * An editor of unlimited length will return its maximum size as zero
     */
    TBool IsEditorHasFreeSpace()const;

    /**
     * Returns the free space left in the Editor.
     *
     * @param aUnlimit.	Set to EFalse in case of Editors with limited text 
	 *				  	size
	 *		  isToCountUncommittedTextLength. Pass ETrue if the lenght of
	 *					Uncommitted text is to be counted.
	 *
	 * @return TInt. Free space left in the Editor.
     */
    TInt EditorFreeSpace(TBool& aUnlimit, 
    				TBool isToCountUncommittedTextLength = EFalse ) const;
#ifdef RD_INTELLIGENT_TEXT_INPUT    
    /**
     * Returns the free space left in the Editor. ITI Specific Implementation
     *
     * @param aUnlimit.	Set to EFalse in case of Editors with limited text 
	 *				  	size
	 *		  isToCountUncommittedTextLength. Pass ETrue if the lenght of
	 *					Uncommitted text is to be counted.
	 *
	 * @return TInt. Free space left in the Editor.
     */
	TInt EditorFreeSpaceForAutoWordCompletion(TBool& aUnlimit, 
                                            TBool isToCountUncommittedTextLength = EFalse ) const;
#endif    																	
  	
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT    
    /**
    * Gives the amount of characters in the document.
    * @return   The number of characters in the active editor.
    */
    TInt DocumentLength() const;
    
    /**
    * Tells if the given key event will replace the latest character in editor
    * because of multitapping or long press handling.
    */
    TBool KeyEventWillReplaceCharacter( const TKeyEvent& aKeyEvent );

#endif
#endif    																	

    /**
     * Uses TLocale to access the localized decimal separator in the kernel
     */
    TChar CurrentDecimalSeparator() const;

    /**
     * Sets up the FEP state from the Editor State
     * Called by HandleChangeInFocusL()
     */
    void ConfigureFEPFromEditorStateL();

    /**
     * Sets up the FEP State(character width) from the Editor Mode
     * @return FepMode
     */
    TInt ConfigureFepModeFromEditorMode(TInt aEditorMode);

    /**
     * launch query which contains instructions on how to enter
     * chinese text using the FEP
     */
    void LaunchHelpTextQueryL();

    /**
     * ensure that the UI is aware of the current editor context
     */
    void UpdateEditorContext() const;

    /**
     * query whether the text is valid in the current editor
     */
    TBool TextIsValidInEditor(const TDesC& aText);


    /**
     * Sets the style of the cursor to aType
     */
    void SetCursorType(TChineseFepCursorType aType);

   /**
    * Returns the current editing state indicator
    */
    MAknEditingStateIndicator* EditingStateIndicator() const;

    /**
    * Adds space at the end of the buffer if the feature is enabled.
    */
    TBool TryHandleArrowRightEventL(TInt aDocumentLength);

    /**
    * Remove spaces from the end of the buffer if the cursor
    * is in last position. Only in Japanese variant.
    * Opposite functionality of TryHandleArrowRightEventL().
    */
    TBool TryHandleArrowLeftEventL(TInt aDocumentLength);

    /**
    * Adds enter to the buffer if the cursor is at the end of the buffer and
    * feature KAknFepEnterWithScrollDown is enabled.
    *
    * @since 2.6
    * @param aDocumentLength Editor's document length
    * @return Boolean if the down key event was handled.
    */
    TBool TryHandleArrowDownEventL(TInt aDocumentLength);

    inline void ResetFlags();

    /**
     * Handle the Chr Key Monitor
     * for Japanese variant only.
     *
     * @since 3.0
     * @return KErrNone if succeed, KErrDied if failed
     */
    static TInt HandleChrKeyMonitorCallback(TAny* aObj);

	/**
	 * Enters a new ligature into the editing window.
	 *
     * @since 3.2
     * @param aText Unicode value of the ligature.
     * @return None
	 */
	void NewLigatureL( const TDesC& aText );
   /**
     * Sets the current state to aState based on the input mode 
     * used to show the indicator
     * @since 3.2
     */
    void SetPredictiveIndicatorState(TAknEditingState& aState);
    static TInt HandleResourceChangedCallback(TAny* aObj);
    
    void ResourceChangedTimeroutL();
    /**
    * Exit plugin itut spell mode by press ok btn
    */
    void ExitPluginSpellModeByOkL();    
	
#ifdef RD_SCALABLE_UI_V2
	/**
     * Unregisters Fep as observer
     */
    void UnregisterObserver();
    
    /**
     * Unregisters Fep as observer
     */
    void RegisterObserver();
    
#endif // RD_SCALABLE_UI_V2	    
private:
    TUint EditorModeFromFepMode(TInt aFepMode);

    // returns sound system
    CAknKeySoundSystem* SoundSystem() const;

    void SetHashKeyStyle();
    void ReadHashKeyLoopL();

    TBool IsValidInLineCharacter(TChar aCharacter) const;
    TBool HandleLoopingInNaviEventL(TWesternNaviEvent aNaviEvent);

    static TInt WordConcatenationTimerTimeoutCallback(TAny* aObj);
    void WordConcatenationTimerTimeoutL();
    
	void AddInputLanguageItemL(CAknFepUiInterfaceMenuPane* aMenuPane, TInt aIndex);    
	void SetQwertyModeToInputcapbility();
	void SetExtendedInputCapabilities( TUint aCapabilities);
#ifdef RD_INTELLIGENT_TEXT_INPUT

    // Predictive QWERTY (XT9) changes ---->
    /**
    * Adds the "Input Options" option and its sub menu to the given menu.
    * @param    aMenuPane   The menu where the option should be added.
    * @param    aIndex      The location in the menu where the option should be added.
    */
   
    void AddPredictiveModeOptionsL(CAknFepUiInterfaceMenuPane* aMenuPane, TInt aIndex) const;
    
    /**
     * Adds the XT9 Edit menu options to the given menu.
     * @param    aMenuPane   The menu where the option should be added.
     * @param    aIndex      The location in the menu where the option should be added.
     */
    
     void AddPredictiveModeEditMenuL(CAknFepUiInterfaceMenuPane* aMenuPane, TInt aIndex) const;
    
    // Predictive QWERTY (XT9) changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT
    /* Sets default number mode when arabic language is used.
    * @param aMode either Latin or Arabic
    * @param aNbrModeType number mode type from GSLangPlugin.hrh
    */
    void SetDefaultNumberMode( TInt aMode, TInt aNbrModeType );

    /**
    * Loads an icon from avkon bitmap file.
    * @param aIcons The array into which the icon will be added. Icons will be 
    *        owned by this array.
    * @param aBitmapId ID of the actual bitmap.
    * @param aMaskId ID of the mask.
    */
    void LoadIconL( CArrayPtr<CGulIcon>* aIcons, TInt aBitmapId, TInt aMaskId );

    /**
      * Add a menu item for User DB management. 
      * phrase creation
      * @since S60 5.0 S60_version
      * @param aMenuPane The menu into which the mune item will be added. 
      * @param aIndex Index of the menu item to insert.
      * @return none
      */
     void AddUserDBDlgItemL( CAknFepUiInterfaceMenuPane* aMenuPane, TInt aIndex );

     /**
       * Return wether ipnut mode is change by press shift+space. 
       * This is for half-qwerty.
       * @return ETure is change by press shift+space, otherwise EFalse.
       */
     TBool IsChangeModeByShiftAndSpace();
     
     /**
       * Set wether ipnut mode is change by press shift+space. 
       * This is for half-qwerty.
       * @return none
       */
     void SetChangeModeByShiftAndSpace( TBool aFlag );

public:
    //Hash Key Manager Interface
    TBool IsOnlyNumericPermitted() const;
    void TryIncrementModeL(TInt aCurrentMode);
    void TryIncrementModeChineseQwertyL(TInt aCurrentMode);
    void TryIncrementChineseModeForQwertyL(TInt aCurrentMode);
    void TryChangeToSharedDataModeL();
    void TryChangeToModeBeforeL();
    TBool TryChangeModeL(TInt aMode);
    void ChangeInputLanguageL(TInt aInputLanguage);
    void RemovePreviousCharacterL();
    void  TryChangePredictiveInputModeL(TBool aFlag);
    
    /**
     * To Handle Indic Hash key 
     * 
     *
     * @since 3.2
     * @return the language id of the Input Language as in Cenral Repository (General Settings)
     */
    TLanguage GetInputLanguageFromSharedDataInterface();

    
    /**
     * To Handle Hash key 
     * 
     *
     * @since 3.2
     * @return ETrue if the Last Key press was Hash key.
     */
    TBool WasLastKeyPressAHashKey();

    /**
     * To Handle Hash key 
     * To set or Reset the status maintained to know if last key pressed was Hash Key	 
     *
     * @since 3.2
     * @return nothin
     */
	void SetLastPressedHashKeyStatus(TBool aStatus);

    //Case Manager Interface
    MCoeFepAwareTextEditor* FepAwareTextEditor() const;
    
    TBool IsFepAwareTextEditor() const;
    
    TCursorSelection UncommittedText() const;
    void SetCase(TCase aCase);

    //made public for western

    /**
     * Retreives the State of the current editor
     *
     * @return the current editors state object
     */
    CAknEdwinState* EditorState() const;

    /**
     * update the edit indicators when moving into a new input mode
     */
    void UpdateIndicators();

    /**
     * Checks if the editor is in a state where a Special Character Table can be launched
     */
    TBool IsAbleToLaunchSCT() const;
    
    /**
     * Get SCT Chars' Length to judge it's null
     */
    TBool GetSctLengthL(TInt resourceId)const;
    
    /**
     * Get current editor's SCT resource id
     */
    TInt GetCurrentEditorSCTResId() const;
    
    /**
     * Launch the Special Character Table
     */
    void LaunchSpecialCharacterTableL(TInt aResourceId = EDefaultNumericCharMapResId,
                                      TBool aLaunchedByTouchWin=EFalse, 
                                      TBool aSmileyFirst=EFalse);

    /**
     * Checks if the editor is in a state where a Pictograph Character Table can be launched
     */
    TBool IsAbleToLaunchPCT() const;
    
    /**
     * Checks if the editor is in a state where a Pictograph Character Table can be launched
     */
    TBool IsAbleToLaunchSmiley() const;

    /**
     * Launch the Pictograph Character Table
     */
    void LaunchPictographCharacterTableL();

    //MAknFepManagerUIInterfaceWestern
    virtual void UpdateInlineEditL(const TDesC& aNewInlineText, 
                                   TInt aPositionOfInsertionPointInInlineText);
    virtual void StartInlineEditL();
    virtual void StartInlineEditL(const TDesC& aText);
    virtual void StartInlineEditL(TCursorSelection aCursorSelection, 
                                  const TDesC& aInitialInlineText, 
                                  TInt aPositionOfInsertionPointInInlineText, 
                                  TBool aCursorVisibility);

    virtual TBool CloseUiIfWordDeletedL();
    virtual TBool TryGetTextToUncommitL(TDes& aText, TInt aCode, TBool& aComsumeKey);

    /**
     * Addition for ITI features on FSQ.
     * To check which characters will be underlined.
     */
    TBool TryGetTextToUncommitL(TDes& aText, TBool& aComsumeKey);
    
    virtual void UpdateCbaL(TInt aResourceId);
    virtual TBool TryRemoveNoMatchesIndicatorL();
    virtual TBool IsMoreGoodWordsComing(TInt aNewWordLength) const;
#ifndef RD_INTELLIGENT_TEXT_INPUT
    virtual TBool IsMoreGoodAutoCompleteWordsComing(TInt aInputMode, TInt aPreviousWordLengh, 
                                                    TInt aNewWordLength) const;
#endif //RD_INTELLIGENT_TEXT_INPUT
    virtual void TryStartCompoundWord();
    virtual void AddCompoundWordToUdbL();

    //MAknFepManagerUIInterfaceWestern made public for western
    inline TBool IsFlagSet(TInt aFlag) const;
    inline void SetFlag(TInt aFlag);
    inline void ClearFlag(TInt aFlag);
    
    inline TBool IsExtendedFlagSet(TInt aExtendedFlag) const;
    inline void SetExtendedFlag(TInt aExtendedFlag);
    inline void ClearExtendedFlag(TInt aExtendedFlag);
    inline void ResetExtendedFlags();

    inline TCoeInputCapabilities& InputCapabilities();

    void SetWesternPredictive( const TBool aWesternPredictive );
    void SetWesternAutoComplete( const TBool aWesternAutoComplete );
    void SetJapanesePredictive(const TBool aJapanesePredictive);
    void SetQwertyMode(const TBool aQwertyMode); // Empty implementation. Qwerty mode is not supported.

    TBool WesternPredictive(TInt aMode = 0) const;
    TBool IsPredictive(TInt aMode = 0) const;
    TBool Japanese() const;
    TBool IsOnlyFullWidthCharacterPermitted() const;
    TBool IsOnlyHalfWidthCharacterPermitted() const;
    TBool IsHalfAndFullKatakanaPermitted() const;
    TBool IsHalfAndFullLatinPermitted() const;
    TBool IsHalfAndFullNumberPermitted() const;
    virtual TWidthChar CharacterWidth() const;

    virtual CPtiEngine* PtiEngine() const;
    virtual void SetInlineEditingCursorVisibilityL(TBool aCursorVisibility);

    TInt  InputMode() const;
    TInt CangJieMode() const;

    TBool InputLanguageSupportsCaseChanges() const;

    /**
     * Queries supportting of KAknFepScrollLatinPredictive feature
     *
     * @since 2.6
     * @return ETrue if ScrollLatinPredictive was supported
     */
    TBool IsAbleScrollLatinPredictive() const;

    /**
     * Previous case is returned.
     *
     * @since 2.6
     * @return TInt  EAknEditorTextCase or EAknEditorUpperCase or EAknEditorLowerCase
     *               If case cannot be saved, 0 value is returned.
     */
    inline TInt CaseBefore() const;

    /**
    * @return true if current input language is one of Chinese input languages.
    */
    inline TBool IsChineseInputLanguage() const;
    
    /**
    * @return true if current input language is arabic input languages.
    */
    inline TBool IsArabicInputLanguage() const;    
    
    /**
    * @return true if current input language is Korean input language.
    */
    
    inline TBool IsKoreanInputLanguage() const;

    /**
     * Check Qwerty keypad
     *
     * @since 3.0
     * @return ETrue is Qwerty
     */
    inline TBool IsQwerty() const;
    
    
    /**
    * Returns boolean value indicating whether hash key is in selection mode or
    * in traditional mode.
    *  
    * @since 3.1
    * @return  ETrue if hash key is in text selection mode.
    *          EFalse otherwise.
    */
    TBool HashKeySelectionInUse() const;

    /**
    * Returns the UI interface implementation used in the current app.
    *
    * @since 3.2
    * @return  The UI interface object used in the current app.
    */
    CAknFepUIInterface* UiInterface();
  
    /**
    * Returns boolean value indicating whether edit submenu functionality is in use.    
    *  
    * @since 3.1
    * @return  ETrue if edit submenu functionality is in use.
    *          EFalse otherwise.
    */        
    TBool EditSubmenuInUse() const; 
    
    /**
     * Set MultiTap Timer
     *
     * @since 3.0
     * @param aMultiTapTimer value of KAknFepMultiTapTimer setting
     */
    void SetMultiTapTimer(const TInt aMultiTapTimer);

    /**
     * Set Japanese Qwerty Flags
     *
     * @since 3.0
     * @param aJapaneseQwertyFlags value of KAknFepJapaneseSpecialCharFlag setting
     */
    void SetJapaneseQwertyFlags(const TInt aJapaneseQwertyFlags);

	/**
	* Calling this method will raise a flag which will cause predictive 
	* word to be commited during next call to TryCloseUiL-method. This
	* is needed because of Japanese ReadingTextL. 
	*
	* @since 3.1
	*/	
	void SetDelayedCommit();

    /**
     * from MAknFepManagerUIInterface
     * query whether the special char is valid in the current editor
     *
     * @param aChar Checking a chracter
     */
    TBool CharIsValidInEditor(TChar aChar);

   /**
     * from MAknFepManagerUIInterface
    * Returns the status of predictive input.
    *
    * @since 3.2
    * @return ETrue is available predictive input.
    */
    TBool IsAbleToChangePrediction() const;
    
    /**
     * Commits uncommitted text in the editing window. For secret text editors,
     * only inline editing flags are changed
     */
    void CommitInlineEditL();    
    
    TBool GetIndicatorImgID(TInt& aIndicatorImgID, TInt& aIndicatorTextID);   
    
    TInt CurrentInputLangCode();

    inline TFepSymbolOfHardwareOne SymbolInfoOfHardKey1();
public:
#ifdef RD_INTELLIGENT_TEXT_INPUT
    /*
    Sets the keyboard layout.
    **/
    void SetKeyboardLayout(TPtiKeyboardType aType);
#endif
    virtual TText PreviousChar( TBool aContextSensitive = EFalse );
    virtual TText NextChar();
    virtual TBool IsZWSCharacterPresent( TBool aLigaturePresent = EFalse );
    void RemoveZWSCharacterL( TBool aIsViramaInputted, 
                              TBool aIsInMultitappingHalant = EFalse, 
                              TBool aIsCharModifier = EFalse, 
                              TBool aIsLigaturePresent = EFalse );
    void NewCharacterSequenceL(const TDesC& aText, TIndicInputResponse aResponse);
    TBool IsValidInlineIndicCharacter(TChar aCharacter) const;
#ifdef RD_HINDI_PHONETIC_INPUT	
    TBool IsIndicPhoneticInputLanguage() const;
    TInt SetPhoneticIndicator(TLanguage aInputLanguage);
    void TryChangePhoneticModeL();
#endif
    TDigitType LocalDigit();
public:
     /**
     * Auto word Completion is Enabled
     *
     * @since 3.2
     */    
    void SetAutoCompletionState(TInt aValue);
#ifndef RD_INTELLIGENT_TEXT_INPUT
    void RemoveSuggestedCompletionL();
#endif	//RD_PRED_AW_COMPLETION
#ifdef RD_INTELLIGENT_TEXT_INPUT
    TBool IsFnKeyMappedL(TPtiKeyboardType aKeyboardType = EPtiKeyboardNone);
    TBool IsFnKeyMapped();
    void SetFnKeyMappingState();
    TBool KeyMapsDifferentCharacterWithFn( TPtiKey aKey ) const;
     
     /**
	 * Setter for the advanced predictive typing correction.
	 * @param    aLevel   The level to be set.
	 */
	 void SetTypingCorrectionLevel(TInt aLevel);
	
	 /**
	 * Resetter for the advanced predictive Number Candidate setting.
	 * @param    aValue   The value to be set.
	 */
	 void SetNumberCandidateState(TInt aValue);
	 
	 /**
	* Setes the primary candidate from GS to the local state
	*/
	 void SetPrimaryCandidate(TInt aValue);
#endif
       
    void SetStopProcessFocus(TBool aStop, TBool aClose = ETrue);

    TBool StopProcessFocus();
    
    TBool CloseUiOnFocusChange();
    

public:
    void HandleOwnedSimulateKeyL();
    
    TBool HandleCcpuModeKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aEventCode,
                                 TKeyResponse& aRetCode, TBool aLongPressFlag = EFalse);
    
	/**
 	*  Sets aFlag bitfield in iCcpuFlags
 	*/
	inline void SetCcpuFlag(TInt aFlag);

	/**
 	* Clears aFlag bitfield in iCcpuFlags
 	*/
	inline void ClearCcpuFlag(TInt aFlag);

	/**
 	* Clears all flags in iCcpuFlags
 	*/
	inline void ResetCcpuFlags();

	/**
 	* Returns ETrue if the aFlag bitfield in iCcpuFlags is set, EFalse if it
 	* is clear
 	*/
	inline TBool IsCcpuFlagSet(TInt aFlag) const;    
        /**
     * Check if Auto word Completion is Enabled
     *
     * @since 3.2
     * @return ETrue if Auto word Completion is enabled
     */
    inline TBool IsAutoCompleteOn() const;
#ifdef RD_INTELLIGENT_TEXT_INPUT

    
    
    /**
    * @return	TInt 	Typing correction level.
    */
    inline TInt AdvancedPredictiveTypingCorrectionLevel() const;
    
    /**
    * @return   ETrue   If the number candidate shown. EFalse otherwise.
    */
    inline TBool IsAdvancedPredictiveNumberCandidateShown() const;
    
    /**
    * Cuts off the automatically completed tail of the suggested word candidate.
    */
    virtual void RemoveSuggestedAdvanceCompletionL();
    
    /**
    * @return   ETrue   If the number candidate shown. EFalse otherwise.
    */
    inline TBool AdvancedPredictivePrimaryCandidate() const;
    TBool LongPressNumberEntryOnQwerty() const;
    
    // Predictive QWERTY (XT9) changs <----
#endif //RD_INTELLIGENT_TEXT_INPUT
    /**
    * Returns ETrue if Feature manager supports Japanese.
    */
    inline TBool IsFeatureSupportedJapanese() const;
    
 	/**
    * Cancels shift key timer.
    */        
	void CancelShiftKeyTimer();
    
    /**
     * Queries supportting of SecretText
     *
     * @since 2.6
     * @return ETrue if SecretText was supported
     */
    TBool IsSupportsSecretText() const;
    
    void SendEventsToPluginManL( TInt aEventType, TInt aEventData = 0 );
#ifdef RD_INTELLIGENT_TEXT_INPUT
    /**
    Sends the key event received to the Fn key handler and sets the Fn key state.
    */
    TKeyResponse HandleFnKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aEventCode );
#endif
    
    MAknFepManagerInterface* FepUI() const;
  
    TTmDocPosSpec DocPos();
    
    void HandleCopyCutEventL(TInt aCommandId);
    
    void LaunchPenSupportMenuL(); 

private: //navigation
    void FindStartOfWord(TInt& aAnchorPos) const;
    void FindEndOfWord(TInt& aCursorPos) const;
    TBool CursorInsideWord();
    void MoveCursorToStartOfWordL();
    void MoveCursorToEndOfWordL();
    TKeyResponse HandleNaviEventOutsideInlineEditL(TUint aCode, TKeyPressLength aLength);
    TKeyResponse HandleWesternPredictiveArrowKeyL(TInt aCode, TKeyPressLength aLength);
    TBool TryHandleTextSelectedNaviEventL(TInt aCode, TKeyResponse& aResponse);
    TBool TryHandleCommonArrowAndBackspaceFunctionalityL(TInt aCode, TKeyResponse& aResponse);
    void TransferFepStateToEditorL(TBool aFnLockSync=ETrue);
    inline MAknFepManagerInterface* InternalFepUI();


    void LaunchLanguagesPopupListL(TBool aLaunchedByTouchWin = EFalse);
    void LaunchRecognitionWithDictionaryPopupListL();
    void LaunchWritingSpeedPopupListL();
    void LaunchGuidingLinePopupListL();
    TBool IsInputModeAvailable(TInt aMode) const;
    TInt NewInputModeAfterLanguageChange() const;

    /**
      * Launch User DB dialog
      * phrase creation
      * @since S60 5.0 S60_version
      * @param none
      * @return none
      */
     void LaunchUserDBDlgL();

    /**
    * Set current input language and the capabilities of the language to
    * the iLanguageCapabilities member variable.
    *
    * @param aInputLanguage New (current) input language.
    */
    void SetInputLanguageCapabilities(const TInt aInputLanguage);

    /**
    * Returns input language code that corresponds to UI language.
    *
    * @param aUiLanguage Ui language code.
    * @return Input language code that corresponds to the UI language code.
    */
    TInt SubstituteSublanguageId(const TInt aUiLanguage) const;

    CTextView* TextView() const;
    CTextLayout* TextLayout() const;
    CPlainText* PlainText() const;

    void DoWesternMenu(CAknFepUiInterfaceMenuPane* aMenuPane);
    void DoChineseMenu(CAknFepUiInterfaceMenuPane* aMenuPane);
    void DoJapaneseMenu(CAknFepUiInterfaceMenuPane* aMenuPane);
    void DoChineseSubMenu(CAknFepUiInterfaceMenuPane* aMenuPane);

    TBool IsRightToLeftParagraph(TTmDocPosSpec aPos) const;
    void FindAdjacentChunks(const TTmDocPosSpec& aPos,
                            CTmTextLayout::TTmChunkDescription& aLeft, 
                            CTmTextLayout::TTmChunkDescription& aRight) const;

    TBool GetNextVisualRightCharacter( TInt& aPosition );
    TBool GetNextVisualLeftCharacter( TInt& aPosition );
    void FindVisualRightOfWord(TInt& aPosition);
    void FindVisualLeftOfWord(TInt& aPosition);

    //
    // Remaining methods here are candidates for turning into TextViewUtility methods. Some
    // have been made static already in anticipation of this.
    //
    /**
    * Obtains the visual beginning of the document. This may be the left or right the right
    * end of the first line, depending on first paragraph's direction. If the beginning of the document
    * is currently not formatted, then EFalse is returned
    *
    * @param    aEndPos     DocPosSpec is returned for the visual beginning of the document.
    * @return   ETrue if the value is successfully calculated, that is the first line is formatted
    */
    TBool GetVisualDocStart( TTmDocPosSpec& aStartPos ) const;

    /**
    * Obtains the visual end of the document. This may be the left or right the right
    * end of the last line, depending on last paragraphs direction. If the end of the document
    * is currently not formatted, then EFalse is returned
    *
    * @param    aEndPos     DocPosSpec is returned for the visual end of the document.
    * @return   ETrue if the value is successfully calculated, that is the last line is formatted
    */
    TBool GetVisualDocEnd( TTmDocPosSpec& aEndPos ) const;

    /**
    * Get the document position of the visual extrem of the line in which the passed position
    * is in.
    *
    * @param aPos       A position within the line you are interested in
    * @param aToRight   ETrue if you want the right hand end.
    * @param aExtreme   Output value of the doc pos spec for the end of the line
    * @return           ETrue if the line was formatted. If False, the value of eExtreme
    * cannot be trusted.
    */
    TBool GetAknFepLineExtreme( const TTmDocPosSpec& aPos, TBool aToRight,
        TTmDocPosSpec& aExtreme) const;

    /**
    * This is a very general routine for finding the visual extreme (according to
    * Avkon rules) of a piece of selected text.
    * - Text may in general have many varying directional sections.
    * - It may extend from one paragraph to another.
    * - If the paragraph directions are different the current input language is
    *
    * @param aSelection     Logical range for which the visual extreme is required
    * @param aEvent         The direction being navigated (Right/Left/Up/Down); eg. ELeftNaviEvent if moving to the left end
    * @param aLimitPos      returned position in document
    * @param aEffectiveRightOrLeftEvent  Pointer to TWesternNaviEvent, if passed from the client, will
    *                       return either ERightNaviEvent or ELeftNaviEvent
    */
    void GetAvkonDefinedVisualLimitsOfSelection(
        const TCursorSelection& aSelection,
        TWesternNaviEvent aEvent,
        TTmDocPosSpec& aLimitPos,
        TWesternNaviEvent* aEffectiveRightOrLeftEvent) const;

    /**
    * Returns the position at the visual left extreme end for a logical range
    *
    * @param    aStartPos   Starting position for the search
    * @param    aMinPos     minimum logical position of the logical range
    * @param    aMaxPos     maximum logical position of the logical range
    * @return   TTmDocPosSpec for the visual left end
    */
    TTmDocPosSpec LeftVisualExtremePos( const TTmDocPosSpec& aStartPos, 
                                        TInt aMinPos, TInt aMaxPos ) const;

    /**
    * Returns the position at the visual right extreme end for a logical range. The range may consist
    * of many directional chunks.  They are iterated through until the visual end is found.
    *
    * @param    aStartPos   Starting position for the search
    * @param    aMinPos     minimum logical position of the logical range
    * @param    aMaxPos     maximum logical position of the logical range
    * @return   TTmDocPosSpec for the visual right end
    */
    TTmDocPosSpec RightVisualExtremePos( const TTmDocPosSpec& aStartPos, 
                                         TInt aMinPos, TInt aMaxPos ) const;

    /**
    * Gets the visual left-most position in a chunk within a logical range
    */
    TBool LeftEndOfChunkWithinRange( const CTmTextLayout::TTmChunkDescription& aChunk, 
                                     TInt aMinPos, TInt aMaxPos, TTmDocPosSpec& aNewPos) const;

    /**
    * Gets the visual right-most position in a chunk within a logical range
    */
    TBool RightEndOfChunkWithinRange( const CTmTextLayout::TTmChunkDescription& aChunk, 
                                      TInt aMinPos, TInt aMaxPos, TTmDocPosSpec& aNewPos) const;

    /**
    * Adjust the passed docpos to reflect the avkon principal that after a left navigation
    * the cursor should adhere to the bordering directional run on the left of that point.
    * This may mean that the logical position and leading/trailing feature of aPos are adjusted
    * This method does not perform the navigation, but may be called after a navigation.
    */
    void AdjustPosSpecForAmbiguityAfterLeftOrRightNavigation( TTmDocPosSpec& aPos, 
                                                              TWesternNaviEvent aEvent ) const;

    /**
    * Do not move the visual position, but ensure that the docpos passed is adhering to the left
    * side of the right chunk passed in.
    */
    static void AttachToRightChunk( TTmDocPosSpec& aPos, 
                                    const CTmTextLayout::TTmChunkDescription& aRightChunk );

    /**
    * Do not move the visual position, but ensure that the docpos passed is adhering to the
    * right side of the left chunk passed in.
    */
    static void AttachToLeftChunk( TTmDocPosSpec& aPos, 
                                   const CTmTextLayout::TTmChunkDescription& aRightChunk );

    /**
    * Modify the cursor position after a jump to the start or end of a document.
    * The current input direction is used to modify the position, so as to avoid attaching the
    * new cursor position to a block of the opposite input directionality
    *
    * @param aPos   target position of the jump; may be modified by the routine.
    * @param aEvent navigation event that caused the loop.
    * @param aLoopToTop ETrue if the looping is from bottom to top; EFalse if from top to bottom
    *
    */
    void AdjustPosSpecForInputLanguageAfterDocumentLoop( TTmDocPosSpec& aPos, 
                                                         TWesternNaviEvent aEvent, 
                                                         TBool aLoopToTop ) const;

    /**
    * Tests a chunk for validity.
    *
    * @return ETrue if the chunk is valid - is a real chunk.
    */
    static TBool ChunkIsValid( const CTmTextLayout::TTmChunkDescription& aChunk );

    /**
    * Finds the current input direction. This is determined as best as it can using
    * - iT9Interface as the preferred source of information
    * if that is not active/defined then
    * - shared data (via AknTextUtils) if that fails
    * - localLanguage overrides may be in place
    *
    * - when fep is in numeric mode it returns LTR
    * - if FEP is in an irelevant mode, then LTR is returned
    *
    * Note that secret alpha modes return LTR always, as entry is alwas LTR for these
    *
    * @return TDirectionality::ERightToLeft when in an alpha mode and direction is RTL;
    *                           Otherwise returns ELeftToRight
    */
    TBidiText::TDirectionality CurrentInputDirectionality() const;

    /**
    * Access the local language if defined
    *
    * @param aLanguage - returns the Symbian OS language code for the local language
    * @return EFalse iff no local language is in force
    */
    TBool GetLocalLanguage( TLanguage& aLanguage ) const;

    /**
    * Looks through the FEP aware editor's text starting at character aPos, looking for a
    * strongly directional character.  Direction of search can be set.
    * Search is carried out until a strong character is found or the end (or beginning)
    * of the editor text has been reached.
    * The value returned in aIsRightToLeft should not be used unless the method returns ETrue
    * @param aPos       First character to look at
    * @param aForward   If EFalse, then search is backward (decreasing index) in logical buffer
    * @param aIsRightToLeft     Set to ETrue on return if first strong character found is RTL
    *
    * @return           EFalse if no strong character was found.
    */
    TBool GetExposedDirectionOfText( TInt aPos, TBool aForward, TBool& aIsRightToLeft ) const;

    /**
    * Looks through the passed descriptor, looking for a strongly directional character.
    * Direction of search can be set. Search starts at the begining if searching forward;
    * starts at the end if searching backwards.
    *
    * Search is carried out until a strong character is found or the end (or beginning)
    * of the descriptor has been reached.
    * The value returned in aIsRightToLeft should not be used unless the method returns ETrue
    *
    * @param aText      Descriptor to search.
    * @param aForward   If EFalse, then search is backward (decreasing index) in logical buffer
    * @param aIsRightToLeft     Set to ETrue on return if first strong character found is RTL
    *
    * @return           EFalse if no strong character was found.
    */
    TBool GetExposedDirectionOfTextInDescriptor( const TDesC& aText, TBool aForward, 
                                                 TBool& aIsRightToLeft ) const;

    /**
    * Access character in editor text at index aPos.
    *
    * @param    aPos    Index of character to access. Must be a valid index or panic may ensue.
    * @return           charater at index aPos; returns TChar(0) if cannot get at the text.
    */
    TChar CharAt( TInt aPos ) const;

    /**
    * Navigate off of selected text using an Up/Down/Right/Left navigation event
    *
    * The code takes account of the paragraph directionality that the passed selection is
    * found in.
    *
    * The new cursor position is resolved for ambiguity and then set using SetCursorPosition, and
    * so it has the same side-effects:
    *   iUncommittedText is set to the zero-width "cursor" state with the new cursor position
    *   iUncommittedText is set into the FepAwareTextEditor state
    *   iCaseManager is updated with a ENullNaviEvent
    *   the doc pos is set in TextView object if present
    *
    * @param    aSelection  currently selected text
    * @param    aNaviEvent  navigation event. One of ERight/Left/Down/UpNaviEvent. Others are ignored
    * @param    aPos        Ouput, new position of the cursor
    * @return   EKeyWasNotConsumed if the key is not consumed
    */
    TKeyResponse NavigateFromSelectionL(
        const TCursorSelection& aSelection,
        TWesternNaviEvent aNaviEvent,
        TTmDocPosSpec& aPos );


    /**
    * This method is the standard way for the FEP to move the cursor when there is
    * no inline edit or selection.
    * Side effects:
    * iUncommittedText is set to the zero-width "cursor" state with the passed position
    * iUncommittedText is set into the FepAwareTextEditor state
    * iCaseManager is updated with a ENullNaviEvent
    * the doc post is set in TextView object if present
    *
    * @param aNewCursorPos - defines the position to set the cursor at, including leading/trailing
    * @param aDragSelectOn - iff EFalse, do not keep old anchor position (that is, cancel any selection)
    * properties.
    */
    void SetCursorPositionL( const TTmDocPosSpec& aNewCursorPos, TBool aDragSelectOn = EFalse );

    /**
    * Returns ETrue if the passed position is formatted and is in the first line
    */
    TBool InFirstLineAndFormatted( const TTmDocPosSpec& aPos ) const;

    /**
    * Returns ETrue if the passed position is formatted and is in the last line
    */
    TBool InLastLineAndFormatted( const TTmDocPosSpec& aPos ) const;

    /**
    * Returns ETrue if the passed position is in the
    * @param aCheckFirstLine    if ETrue, check for first line; else check last line
    * @return ETrue if the passed position is formatted and is first (or last - depending on switch)
    */
    TBool DocPosInFirstOrLastLineAndFormatted( const TTmDocPosSpec& aPos, 
                                               TBool aCheckFirstLine ) const;

    /**
    * Method to determine if the passed position is equivalent visually to the "canonical"
    * value determined by the limit algorithms.  The methods allow two visually equivalent,
    * but logically different TTmDocPosSpecs to be be checked for equivalence.
    * @param aCurrentPos    Position in document as returned by GetDocPos
    * @param aLimitPos      Visual start of document as returned by GetVisualDocStart
    * @returns              ETrue if the positions are visully equivalent
    */
    TBool AtVisualStart( const TTmDocPosSpec& aCurrentPos, const TTmDocPosSpec& aLimitPos ) const;

    /**
    * Method to determine if the passed position is equivalent visually to the "canonical"
    * value determined by the limit algorithms.  The methods allow two visually equivalent,
    * but logically different TTmDocPosSpecs to be be checked for equivalence.
    * @param aCurrentPos    Position in document as returned by GetDocPos
    * @param aLimitPos      The visual end of document as returned by GetVisualDocStart
    * @returns              ETrue if the positions are visully equivalent
    */
    TBool AtVisualEnd( const TTmDocPosSpec& aCurrentPos, const TTmDocPosSpec& aLimitPos, 
                       TInt aDocLength ) const;

    void SetCursorType(TBool aIsLeftToRight);
    TBool DeviceSupportsRTLLanguageL();

	/**
	 * Performs calculations of digit mode; 
	 * used in UpdateLocalDigitMode() and in UpdateNumericEditorDigitType()
	 */
	void CalculateEditorDigitType(TDigitType& aDestination);

    void UpdateNumericEditorDigitType();

    /**
    * Updates FEP digit mode from global locale settings.
    */
    void UpdateLocalDigitMode();

    /**
    * Checks to see if the cursor position is at an ambiguous point (one where the text blocks
    * on either side are of different directionality) and potentially moves the cursor from
    * one block to the other, depending on the key identity.
    *
    * To be called before key event is processed by the FEP in the usual way.
    * If the key is acted upon to move the cursor, then the event is "eaten" and it is not
    * to be passed on for processing.
    *
    * @param aCode      Key code from standard TKeyEvent key event structure
    * @return           EKeyWasConsumed if a cursor adjustement was made;
    *                   else EKeyWasNotConsumed
    */
    // TKeyResponse AttemptCursorFlipAtAmbiguousPointL( const TKeyEvent& aKeyEvent, TEventCode aEventCode );
    TKeyResponse AttemptCursorFlipAtAmbiguousPointL( const TUint aCode );

    /**
    * Call back method for the "Post Event Check" mechanism. Called from a CIdle after
    * any event that might required a check of the new cursor position or other invariant
    */
    static TInt PostEventCheckCallback(TAny* aObj);

    /**
    * Access method for whether bidirectional cursor is to be used
    *
    * @return EFalse iff the bidi cursor is not to be used
    */
    TBool BidiCursorRequired() const;

    /**
    * Since FEP does not complete the navigation of an event (editor and form do that), FEP
    * has to ensure after all navigation is finished if the cursor is displayed correctly.
    *
    * <p> This method, called from the post event callback, implements the action to check the
    * current cursor directionality against the text directionality at the cursor position,
    * and alters the cursor appearance as required.
    */
    void DoCursorDirectionCheckL();

    /**
    * Check the directionality of text in the vicinity of the current document position
    * and ensure that the cursor is indicated appropriately.
    *
    * This routine has no side-effect on the cursor position. It merely changes the
    * appearance of the cursor.
    */
    void AdjustCursorTypeForCurrentPosition();

    /**
    * Kicks off the post event check idle event
    *
    * @param aCode Key event code
    */
    void SchedulePostEventCheckL(TUint aCode);

    /**
    * Check the directionality of text in the vicinity of the supplied document position
    * and ensure that the cursor is indicated appropriately.
    *
    * This routine has no side-effect on the cursor position. It merely changes the
    * appearance of the cursor.
    *
    * @param    aDocPos     document position that is to be analysed and acted on
    */
    void AdjustCursorTypeForPosition( const TTmDocPosSpec& aDocPos );

    /**
    * "Hook" method for managing a bidi cursor according to the text directionality at the cursor
    * Only one of this method or SetCursorTypeForInputDirection will be in operation at any
    * time.
    *
    * @param aLeftToRight   Supply ETrue iff cursor required is LTR cursor
    */
    void SetCursorTypeForTextDirection( TBool aLeftToRight );

    /**
    * "Hook" method for managing a bidi cursor according to the current input direction
    * Only one of this method or SetCursorTypeForTextDirection will be in operation at any
    * time.
    *
    * @param aLeftToRight   Supply ETrue iff cursor required is LTR cursor
    */
    void SetCursorTypeForInputDirection( TBool aLeftToRight );

    /**
    * Routine to analyse the directionalities surrounding the supplied document position
    * and to return the results.
    *
    * @param    aDocPos     document position that is to be analysed
    * @return               Structure containing the results of the analysis
    */
    TTextDirectionalInfo LocalTextDirectionalStatus( const TTmDocPosSpec& aDocPos ) const;


    /**
    *
    * This method is used to add directional markers to protect neutrals that may be
    * exposed to the opposite directional formatting to their situation before the deletion
    *
    * Redecoration is carried out if a neutral is exposed by the deletion to a directionality
    * different to that which is was already marked.
    *
    * An inline edit may be stated and then committed.
    *
    * iUncommittedText is required to be set to the cursor position that existed after
    * the deletion.
    *
    * iUncommittedText will be updated to the new cursor position.
    *
    * @param aTextThatWasDeleted Informs the method of what text has just been removed
    */
    void RedecorateAfterDeletionL( const TDesC& aTextThatWasDeleted );


    TBool EditorSupportsNeutralProtection();


    /**
    * Wrapper function for the MCoeFepAwareTextEditor commit method.
    * This method terminates the inline by accepting the current inline edit into the buffer.
    *
    * At entry, iUncommittedText is required to have the span of the current inline edit.
    * iAnchorPos should be the index of the first character in the inline edit.
    * iCursorPos should be the index of the next character after the inline edit.
    *
    * At exit, iUncommittedText has 0-length and represents the current cursor position.
    * That is, iAnchorPos = iCursorPos = index of the character next after the cursor
    *
    * These indices may be more easily though of as "gap indices" where gap 0 precedes
    * character 0, gap 1 follows character 0, gap 2 follows charcacter 1, etc.. until gap
    * N follows character (N-1).
    *
    * This routine does not attempt to consolidate any directional markers that may be
    * extraneous in the new buffer.  It is therefore used when it is important to know what
    * span after the commit, e.g. to make it reversible.
    *
    * @param aFepAwareTextEditor    Editor for which there is a current inline edit.
    * @param aCursorVisibility      EFalse if the cursor is to be suppressed during the commit.
    */
    void CommitInlineEditWithoutResolutionL(MCoeFepAwareTextEditor& aFepAwareTextEditor, 
                                            TBool aCursorVisibility, TBool aClearPti = ETrue);


    TKeyResponse HandlePredictiveNaviEventOutsideInlineEditL(TUint aCode, TKeyPressLength aLength);

    static TInt DoSimulateKey(TAny* aThisPtr);

    /**
    * This method is that the label of CBA is updated by aTextResId.
    * @param aPosition   CBA position, either ELeftSoftkeyIndex or ERightSoftkeyIndex
    * @param aCommandId  command id for softkey
    * @param aTextResId  resource id to set string on label.
    * @return TBool The return value is ETrue in case it is necessary to update CBA.
    */
    TBool UpdateCBALabelL(TInt aPosition, TInt aCommandId, TInt aTextResId);

    /**
    * Launches special character table (SCT) and pictograph character table (PCT).
    *
    * @param aResourceId   Resource of SCT content. NULL if the default SCT content
    *                      is used. At the moment only Thai language sets non-default
    *                      content.
    * @param aShowSctParam Set the show mode for sct. EShowSctFirst or EShowPctFirst or EShowSmileyFirst
    */
    void DoLaunchSctAndPctL(TInt aResourceId, TShowSctMode aShowSctMode);

    /**
     * Update CangJie mode
     *
     * @param aNewMode   New CangJie mode
     * @since 3.0
     */
	void UpdateCangJieState(TInt aNewMode);

    /**
     * Launch CangJie setting dialog
     *
     * @since 3.0
     * @return  Selected CangJie level or KErrGeneral if user cancel the selection
     */
	TInt LaunchCangJieOptionDlgL();
    
    
	void AddEditSubmenuL(CAknFepUiInterfaceMenuPane* aMenuPane);
	void DisableEditSubmenuItems(CAknFepUiInterfaceMenuPane* aMenuPane);
	void StartCcpuModeL(TBool aCopyMode);

    /**
     * Handle the Chr Key Monitor
     * for Japanese variant only.
     *
     * @since 3.0
     */
    void HandleChrKeyMonitorL();
    
    TBool IsCursorAtEndOfWord();  
    
    /* This method should be called after launching any dialog by FEP */
    void PrepareFepAfterDialogExitL(TUid aFepUid);
    /* This method should be called before launching any fep aware dialog by FEP */
    void PrepareFepForFepAwareDialogLaunch();
    /* This method should be called after launching any fep aware dialog by FEP */
    void PrepareFepAfterFepAwareDialogExitL(TUid aFepUid);

public:

    /**
     * Handle layout change.
     * When screen layout change, Key Catcher calls this method
     *
     */
    void HandleResourceChange(TInt aType);

    TBool IsHybridAplhaEditor() const;
    TBool IsHybridAlphaModeChangedtoAplhanumeric() const;

    TBool QueryPredictiveState(const TAknEditingState aState);
    TInt EvaluateState(TAknEditingState aState);
    TBool QueryPredictiveAutoCompleteState(TAknEditingState aState);
	 CAknExtendedInputCapabilities::TEditorType EditorType() const;
	 TUint MIDPConstraint() const;
	/**
	* Tells if the current editor is phone number editor
	* @return ETrue if the current editor is phone number editor
	*/
	TBool IsPhoneNumberEditor() const;

    
#ifdef RD_SCALABLE_UI_V2

    /**
     * Pen input support functions
     */
public:

    void HandlePointerEventL(const TPointerEvent &aPointerEvent);
    
    
    /**
     * Handle arrow key event when some chars have been selected
     *
     * @since S60 v3.2
     * @param aKeyEvent
     * @param aEventCode
     * @param aRetCode
     */    
    TBool HandleSelModeArrowKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aEventCode,
                                      TKeyResponse& aRetCode);
    
    /**
     * From CAknExtendedInputCapabilities::MAknEventObserver
     * Handle an event
     *
     * @since S60 v3.2
     * @param aEvent An event, see TInputCapabilitiesEvent
     * @param aParams Event dependent parameters
     */
    void HandleInputCapabilitiesEventL( TInt aEvent, TAny* aParams );
    
    void SubmitInlineTextL( const TDesC& aData );
    
    inline TInt PermittedModes() const;
    
    inline void SetNotifyPlugin( TBool aNotifyFlag );
            
    inline void RemeberEditorState();
    
    inline TBool IsMfneEditor() const;
    
    inline TBool IsFindPaneEditor() const;
    inline TBool IsSupportedAdaptiveSearch() const;
    //for japanese
    void NotifyJapaneseSetting();
    
    
    void ProcessEditorMenuCommand(TInt aCommand);
    
    TInt GetPermittedEditorMenu(TBool aOnlyCount = EFalse);

    void UpdateTouchCaseMode();

	void DimInputmodeTouchMenuItems(CAknFepUiInterfaceMenuPane* aMenuPane);
	
	inline TLanguageCapabilities InputLanguageCapabilities() const;

    inline TAknFepManagerState& FepManState();
    
    inline TBool IsSupportNativeNumber() const;

	void NeedDisableKeySound(const TKeyEvent& aKeyEvent, TEventCode aEventCode);
	
	inline void SetGainForeground( TBool aGainForeground );
	void HandleEndKeyL();
	inline void SetCancelPopupInQwerty( TBool aCancelPopupInQwerty );
		
    /**
     * Return the chinese input mode used last time
     */
	inline TInt LastChineseInputMode() const;

    /**
     * Return active input language, only used for languages popup list
     */
    inline TInt ActiveInputLanguage() const;

    /**
     * Set active input language, only used for languages popup list
     */	    
    inline void SetActiveInputLanguage(TInt aInputLanguage);
    
private:
       
    /**
    * Checks if the current editor is fully FEP aware.
    *
    * @since 3.2
    *
    * @return ETrue if editor if fully FEP aware.
    */
    TBool FullyFepAwareTextEditor() const;

    /**
    * Checks if the current editor is partially FEP aware.
    *
    * @since 3.2
    *
    * @param aAtLeast if ETrue then ETrue is returned for also fully FEP aware editors.
    *
    * @return ETrue if editor is partially FEP aware.
    */
    TBool SemiFepAwareTextEditor( TBool aAtLeast = EFalse ) const;         
        
    void DoWesternTouchMenu(CAknFepUiInterfaceMenuPane* aMenuPane);    
    void DoChineseTouchMenu(CAknFepUiInterfaceMenuPane* aMenuPane);
    
    /**
    * Checks if the current input mode belongs to chinese input modes.        
    *
    * @param aMode The current input mode.
    *
    * @return ETrue if the current input mode belongs to chinese input mode..
    */
    TBool IsChineseInputMode( TInt aMode );
    
#endif //RD_SCALABLE_UI_V2
    
     /**
    * Returns the type of the focused editor.
    *
    * @since 3.2
    *
    * @return Editor type.
    */
    //CAknExtendedInputCapabilities::TEditorType EditorType() const;
        
    /**
     *  Get the MAknFepDocumentNavigation object
     *
     *  @since 3.2
     *  @return pointer to MAknFepDocumentNavigation accessed through
     *  the input capabilites object provider.
     */
	MAknFepDocumentNavigation* AknFepDocumentNavigation() const;
	
	TInt GetPermittedEditorMenuL(TBool aOnlyCount = EFalse);
		
private:// from MPeninputServerEventHandler

    void UpdateLatinIndicator( TAknEditingState& aNewState );
    void UpdateNumberIndicator( TAknEditingState& aNewState );
    void UpdateHindiIndicator( TAknEditingState& aNewState );
    void HandleCopyCutStateL();
 	/**    
	* Returns ETrue if hash key selection is on and hash key mode selection should
	* be blocked.
	*
	* @since 3.1
	*/  
    TBool HashKeyModeChangeBlockedInSearchField() const;                                  
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

	/**
    * An internal helper method for matches popup list.
    * 
    * The index of currently selected prediction candidate.
    * 
    * @since S60 3.2
    */ 
    TInt ResolveCurrentCandidateListIndex(CDesCArrayFlat* aCandList);

    /**
	*  Removes underlining from predictive word and moves cursor the end of the word.
	*
    *  @since 3.2
    *  @param aData Key data
    *  @return None
    */    
    void DeactivatePredicitveWordAndMoveCursorL();
    
 	/**
	*  Returns the Ccpu status of given editor state as a flag combination.
	*
    *  @since 3.2
    *  @param aEdwinState
    *  @return editor ccpu status
    */    
	TInt EditorCcpuStatus(CAknEdwinState* aEdwinState);
	
	/**
	*  Cancels editing options copy / paste mode initialted from editing options -submenu.
	*
    *  @since 3.2
    */    	
	void CancelCcpuMode();

#ifdef RD_SCALABLE_UI_V2    
    /**
    *  Launches stylus Ccpu-menu
    *
    *  @since 5.0
    *  @param aClickPoint Point where stylus was clicked.
    */
    void LaunchStylusCcpuMenuL(TPoint &aClickPoint);
    
    void DynInitTouchMenuPaneL(CAknFepUiInterfaceMenuPane* aMenuPane);
#endif    

    /**
    * Has Japanese prediction input mode in PtiEngine
    */
    TBool HasJapanesePredictionInputMode() const;
    
    
    void InitPreviewMenuPane(CAknFepUiInterfaceMenuPane* aMenuPane);

public:
    void StopDisplayingMenuBar();

private:
    void ConvertCharToKey(TChar aIn, TUint16& aKey) const;

private:
    // Data owned by this class
    CAknFepKeyCatcherInterface* iKeyCatcher;
    CAknFepIndicator* iIndicator;
    CPeriodic* iShiftKeypressMonitor;
    CAknFepLanguageManager* iLangMan;           // own
    MAknFepManagerInterface* iCurrentFepUI;     // not own
    CAknFepCaseManager* iCaseMan;
    CAknFepInlineTextDecorator* iInlineTextDecorator;

    CAknFepHashKeyManager* iHashKeyMan;
    CPeriodic* iConcatenationTimer;
    //fep delay the resource changed message for getting the updated coordinate from editor.
    CAsyncCallBack iAsyncResouceChanged;

    // shared data support
    CAknFepSharedDataInterface* iSharedDataInterface;

    MAknEditingStateIndicator*  iPreviousIndicator;
    MAknEditingStateIndicator* iEditIndicatorAtLastUpdate;
    TAknEditingState iPreviousEditingState;

    TLanguage iUiLanguage;  // Symbian language code for UI language.
    TLanguageCapabilities iLanguageCapabilities; // Input language and capabilities of the language.
    TUint iFlags;
    TUint iExtendedFlags;
    TAknFepManagerState iFepManState;
    TCoeInputCapabilities iInputCapabilities;
    TInt iPermittedInputModes;
    TInt iMode;
    TInt iModeBefore;
    TInt iCaseBefore;
    TInt iResId;
    TInt iVariantPermittedModes;
    TInt iSpaceCharacterCode;               // adding for japanese
    TInt iFullWidthSpaceCharacterCode;      // adding for japanese
    TAknEditorNumericKeymap iAknEditorNumericKeymap;
    TInt iAknEditorFlags;
    TCursorSelection iUncommittedText;
    RArray<TInt> iHashKeyLoop;
    TBool iWesternPredictive;
    TKeyPressLength iKeyRepeat;
    TBool iIsAutoCompleteOn;				// for Auto Word Completion
#ifdef RD_INTELLIGENT_TEXT_INPUT
    TInt iKeyBackSpaceHit;
    TInt  iTypingCorrectionLevel;			// for typing correction level
    TBool iIsNumberCandidateShown;			// for number candidates
    TBool iPrimaryCandidate;    			// for primary candidate
    TBool iKeyPressedDuringShift;           
    TPtiKeyboardType iKeyboardType; 		// for checking the keyboard layout
    TBool iSupressCursorMoveToEndChrKeyPressed; // For supressing cursor movement when SCT launched
#endif //RD_INTELLIGENT_TEXT_INPUT
    TBool iJapanesePredictive;
    TBool iDeviceSupportsRtlLanguage; // Could be removed
    CIdle* iPostEventCheck; // Owned

    TWidthChar iCharWidth;      // adding for japanese
    CPtiEngine* iPtiEngine;     // adding for japanese

    TUint iSimulateKey;
    CApaDocument* iEmbedded; // User Dictionary
    CAknNullService*  iNullService; // User Dictionary
    CAsyncCallBack iAsyncOwnSimulateKey;
    TInt iFreeSpaceOfEditor;
    TBool iQwertyInputMode; // Not used. The value is always EFalse.
    TBool iLongThai0Key;
    TBool iFepFullyConstructed;
    CAknEdwinState* iRememberEditorState; // For "editing options" submenu
    TInt iRememberLength;   // For "editing options" submenu.
    TInt iEditorCcpuStatus; // For "editing options" submenu.
    TInt iCcpuMode;         // For "editing options" submenu    
    TInt  iMultiTapTimer;   // for Japanese only: store MultiTap Timer
 	CAknFepUiInterfaceMenuBar* iEditMenuBar;   // Remember menu bar instance for closing. 
 	HBufC* iSctEditChars;   // Buffer for characters selected in the edit menu
 	TPtr iEditCharsPtr;     // Pointer to the buffer that holds all the selected characters.
 	TInt iStarScan;         // Remember scan code for star key.
    TInt  iJapaneseQwertyFlags;   // for Japanese only: store Japanese Qwerty Flags

    CPeriodic* iChrKeypressMonitor; // for Japanese only: Chr key monitor timer
	TBool iLastKeyPressedWasHashKey;
    TBool iStopProcessFocus;
    TBool iClosePeninputUi;
    /**
     * User DB dialog
     * Own, but doesn't need delete by myself
     */
    CAknFepUserdbDlg* iUserdbdlg;
    
    CAknNavigationDecorator* iTapGroup;

    TInt iEditorCommandList[30];

	TBool iComposChrFlag;
    //User db view(phrase creation)
    TInt iIsUserdbdlgActive;
    /**
     * * Used for handle key event for zhuyin method.(phrase creation)
     * 
     */
    CZhuyinKeyHandler* iZhuyinKeyHandler;
    CAknFepZhuyinAnalyser* iZhuyinAnalyser;
    CZhuyinAnalyserDbFeed* iZhuyinAnalyserDb;

#ifdef RD_SCALABLE_UI_V2

    /**
     * Used by itut spell mode, initial text in spell mode
     */
	HBufC* iSpellInitText;

    /**
     * Used by itut spell mode, initial text selection in spell mode, 
     * whose position is relative to whole text in original editor
     */
	TCursorSelection iSpellInitCurSel;
   
    CAknFepPluginManager* iFepPluginManager;
    
    /**
     * HKB predictive state before starting plugin IME
     */
    TBool                    iT9UsedBeforePluginInput;
    
    
    /**
    * Stylus ccpu menu
    */ 
    CAknFepUiInterfaceStylusPopUpMenu*     iStylusCcpuMenu; 
    
    /**
    * Last known clik point, needed for stylus ccpu menu launch.
    */   
    TPoint                   iClickPoint;
    
    TBool iGainForeground;
    
    TBool iLoseForeAndGainFocus;
    
    TBool iNotifyPlugin;
    
    TBool iCancelPopupInQwerty;
    
    TBool iStarKeyPressed;
    
    TInt iLastChineseInputMode;
    
    // Indicate Stroke is used in QWERTY, when default input mode is Cangjie
    TBool iStrokeUsedInQWERTY;
    // Save that which editor use stroke
    CAknEdwinState* iEditorStateStrokeUsed;

#endif //RD_SCALABLE_UI_V2

    /**
     * Long press clear key after close UI;
     */
    TBool                    iLongClearAfterCloseUI;      
    
    TBool iFeatureSupportedJapanese;    // Save the value of KFeatureIdJapanese

	// stores a reference to previous editor state for pointer comparision
	CAknEdwinState* iPreviousEditorState; 
	   
    CAknFepUIInterface* iUiInterface;
#ifdef RD_INTELLIGENT_TEXT_INPUT
    CAknFepFnKeyManager     *iFnKeyManager; // Owned
    TBool iIsFnKeyMapped;
#ifdef __REVERSE_FN_KEY_SUPPORTED
    TBool iIsReverseFnkeyInput;
#endif //__REVERSE_FN_KEY_SUPPORTED
	
	// Predictive QWERTY (XT9) changes ---->
    //! The exact word tooptip shown above the inline editor when the exact word differs from the best guess candidate
   // CAknPreviewPopUpController* iExactWordPopup;
    
    //! The contents of the exact word popup.
	MAknFepUiWordPopupContent* iExactWordPopupContent;
	MAknFepCandidatePopup*	iCandidatePopup;
	TBool iSListLaunchedFromMenu;
	TInt iAutoCmptnTailLength;
	TInt iIsLastResourceEditMenu;
	// Predictive QWERTY (XT9) changes <----
#ifdef    __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__    
    CAknFepUiInterfaceMenuBar* iOptionsMenuBar;
#endif	
#endif //RD_INTELLIGENT_TEXT_INPUT
    //phrase creation    
    TInt iPhraseCreation;
    TInt iPinyinPhraseCreation;
    TInt iKeyStar;
    TInt iEntryPhrase;
    TBool iPenSupportMenu;
    TFepSymbolOfHardwareOne iSymbolData;
      
    /**
     * Flag for wether input mode is changed by shift+space
     */
    TBool iShiftAndSpace;
    CAknFepThaiSCTSelector* iAknFepThaiSCTSelector;
    
    /**
     * Active input language, only used for languages pop up list,
     * can not changed in other cases, e.g. in latin-only editor
     */
    TInt iActiveInputLanguage;
    
    /**
     * indicate whether matches popup list was lauched,
     * when rotating the screen, we need to consider it, e.g V-ITUT  to FSQ
     */
    TBool iMatchesListLaunched;


public:

    //the follow 8 functions for phrase creation 
    /**
     * Set the state to pinyin phrase creation
     */
    void PinyinPhraseCreation( TBool aPinyinPhraseCreation );
    
    /**
     * Is the state is in pinyin phrase creation
     *
     * @return if not enabled, return EFalse
     */
    TBool IsPinyinPhraseCreation() const;
    
    /**
     * Set the state to phrase creation
     */
    void PhraseCreation( TBool aPhraseCreation );
    
    /**
     * Is the state is in phrase creation
     *
     * @return if not enabled, return EFalse
     */
    TBool IsPhraseCreation() const;
    
    /**
     * Set the state to input phrase
     */
    void EntryPhrase( TBool aEntryPhrase );
    
    /**
     * Is the state is in input phrase
     *
     * @return if not enabled, return EFalse
     */
    TBool IsEntryPhrase() const;
    
    /**
     * Set the state to enable star key
     */
    void EnableKeyStar( TBool aEnable );
    
    /**
     * Is the state is enable star key
     *
     * @return if not enabled, return EFalse
     */
    TBool IsEnableKeyStar() const;
#ifdef RD_INTELLIGENT_TEXT_INPUT
public:
   /**
    * multitaps thru the chars in the SCT.
    * @param aCallback, the callback when the timer expires
    */
    void MultitapThroughSCTCharL(TCallBack aCallBack);       	 
private:        
    TBool NumericResourceMultiTapTimerTimeoutL();    
    
    /**
     * CAknFepManager::FindAndRemoveInputOptionsMenuItemL
     * Helper method to check if the "Input options" menu item exists and
     * if so, remove it from the menu pane.
     *  Params - 
     *      aMenuPane : pointer to the menu pane to be checked for the resource id
     */
    void FindAndRemoveInputOptionsMenuItemL( CAknFepUiInterfaceMenuPane* aMenuPane );
    
    /**
      * CAknFepManager::FindAndRemoveEditSubMenuItemL
      * Helper method to check if the "Edit Text" menu item exists and
      * if so, remove it from the menu pane.
      *  Params - 
      *      aMenuPane : pointer to the menu pane to be checked for the resource id
      */
    void FindAndRemoveEditSubMenuItemL( CAknFepUiInterfaceMenuPane* aMenuPane );
    
    /**
     * CAknFepManager::AddInputOptionsMenuItemL
     * Helper method to add the "Input options" menu item in appropriate location
     * Appropriate location is :
     *  - if Avkon Help command exists, then immediately above it
     *  - else if Avkon Exit command exists, then immediately above it
     *  - else if there are 3 or more items in the Options menu, then 3rd from last
     *  - else it is the last item. 
     *  Params - 
     *      aMenuPane : pointer to the menu pane to be checked for the resource id
     */
    void AddInputOptionsMenuItemL( CAknFepUiInterfaceMenuPane* aMenuPane );
        
private:        
    CChrMultiTapTimer* iNumericResourceTimer;
    HBufC* iResourceString;
    TInt iChrCharIndex;    
#ifdef __HALF_QWERTY_KEYPAD
	TKeyEvent iHalfQwertyLastKeyEvent;
// this variable is set whenever a long key press of chr happens
	TBool iChrLongKeyPress;
    TBool iFnCharInsertedForShift;	
#endif //__HALF_QWERTY_KEYPAD
    TInt iNumericResourceId;
#endif //RD_INTELLIGENT_TEXT_INPUT
    TBool iHybridAplphaChangedToAlphanumeric;
    CAknEdwinState* iLastFocusedEditor;
    CAknEdwinState* iFepAwareDialogParentEditor;
    /**
     * Indicate that the focused control is destroyed.     
     */  
    TBool iFocusedItemDestroy;    
    };

/**
 *  Sets aFlag bitfield in iFlags
 */
inline void CAknFepManager::SetFlag(TInt aFlag)
    {
    iFlags|=aFlag;
    }

/**
 * Clears aFlag bitfield in iFlags
 */
inline void CAknFepManager::ClearFlag(TInt aFlag)
    {
    iFlags&=~aFlag;
    }

/**
 * Clears all flags down
 */
inline void CAknFepManager::ResetFlags()
    {
    iFlags=0;
    }

/**
 * Returns ETrue if the aFlag bitfield in iFlags is set, EFalse if it
 * is clear
 */
inline TBool CAknFepManager::IsFlagSet(TInt aFlag) const
    {
    return iFlags&aFlag;
    }

/**
 *  Sets aExtendedFlag bitfield in iExtendedFlags
 */
inline void CAknFepManager::SetExtendedFlag(TInt aExtendedFlag)
    {
    iExtendedFlags|=aExtendedFlag;
    }

/**
 * Clears aExtendedFlag bitfield in iExtendedFlags
 */
inline void CAknFepManager::ClearExtendedFlag(TInt aExtendedFlag)
    {
    iExtendedFlags&=~aExtendedFlag;
    }

/**
 * Clears all ExtendedFlags down
 */
inline void CAknFepManager::ResetExtendedFlags()
    {
    iExtendedFlags=0;
    }

/**
 * Returns ETrue if the aExtendedFlag bitfield in iExtendedFlags is set, EFalse if it
 * is clear
 */
inline TBool CAknFepManager::IsExtendedFlagSet(TInt aExtendedFlag) const
    {
    return iExtendedFlags&aExtendedFlag;
    }

/**
 * Returns previous case information
 */
inline TInt CAknFepManager::CaseBefore() const
    {
    return iCaseBefore;
    }
    
inline TBool CAknFepManager::IsChineseInputLanguage() const
    {
    return ( iLanguageCapabilities.iInputLanguageCode == ELangPrcChinese ||
        iLanguageCapabilities.iInputLanguageCode == ELangHongKongChinese ||
        iLanguageCapabilities.iInputLanguageCode == ELangTaiwanChinese);
    }

inline TBool CAknFepManager::IsKoreanInputLanguage() const
    {
    return iLanguageCapabilities.iInputLanguageCode == ELangKorean;
    }
inline TBool CAknFepManager::IsArabicInputLanguage() const
    {
    return ( iLanguageCapabilities.iInputLanguageCode == ELangArabic);
    }
/**
 * Returns the valid of Qwerty
 */
inline TBool CAknFepManager::IsQwerty() const
    {
    return iQwertyInputMode;
    }


/**
 *  Sets aFlag bitfield in iCcpuFlags
 */
inline void CAknFepManager::SetCcpuFlag(TInt aFlag)
    {
    iCcpuMode|=aFlag;
    }

/**
 * Clears aFlag bitfield in iCcpuFlags
 */
inline void CAknFepManager::ClearCcpuFlag(TInt aFlag)
    {
    iCcpuMode&=~aFlag;
    }

/**
 * Clears all flags in iCcpuFlags
 */
inline void CAknFepManager::ResetCcpuFlags()
    {
    iCcpuMode=0;
    }

/**
 * Returns ETrue if the aFlag bitfield in iCcpuFlags is set, EFalse if it
 * is clear
 */
inline TBool CAknFepManager::IsCcpuFlagSet(TInt aFlag) const
    {
    return iCcpuMode&aFlag;
    }
     
inline MAknFepManagerInterface* CAknFepManager::InternalFepUI()
    {
    return iCurrentFepUI;
    }

#ifdef RD_SCALABLE_UI_V2

inline void CAknFepManager::SetNotifyPlugin( TBool aNotifyFlag )
	{
	iNotifyPlugin = aNotifyFlag;
	}
inline TInt CAknFepManager::PermittedModes() const
	{
	return iPermittedInputModes;
	}    

inline void CAknFepManager::RemeberEditorState()
	{
	iRememberEditorState = EditorState();
	}  	

inline TBool CAknFepManager::IsMfneEditor() const
    {
    return EditorType() == CAknExtendedInputCapabilities::EMFNEBased;
    }

inline TBool CAknFepManager::IsFindPaneEditor() const
    {
    return iAknEditorFlags & EAknEditorFlagFindPane;
    }
    
inline CAknFepManager::TLanguageCapabilities CAknFepManager::InputLanguageCapabilities() const
    {
    return iLanguageCapabilities;
    }

inline TAknFepManagerState& CAknFepManager::FepManState()
    {
    return iFepManState;
    }

inline void CAknFepManager::SetCancelPopupInQwerty( TBool aCancelPopupInQwerty )
    {
    iCancelPopupInQwerty = aCancelPopupInQwerty;
    }
inline TInt CAknFepManager::LastChineseInputMode() const
    {
    return iLastChineseInputMode;
    };
#endif //RD_SCALABLE_UI_V2

/**
* Returns ETrue if Feature manager supports Japanese.
*/
inline TBool CAknFepManager::IsFeatureSupportedJapanese() const
    {
    return iFeatureSupportedJapanese;
    }

/**
* Returns ETrue if Auto Word Completion is enabled in the edtior
*/    
inline TBool CAknFepManager::IsAutoCompleteOn() const
    {
    return iIsAutoCompleteOn;
    }
#ifndef RD_INTELLIGENT_TEXT_INPUT
/**
* Setes the current state from GS to the local state
*/    
    
inline void CAknFepManager::SetAutoCompletionState(TInt aValue) 
{
	iIsAutoCompleteOn = aValue;
}
#endif
#ifdef RD_INTELLIGENT_TEXT_INPUT
/**
* @return	TInt 	Typing correction level.
*/
inline TInt CAknFepManager::AdvancedPredictiveTypingCorrectionLevel() const
	{
		return iTypingCorrectionLevel;
	}
    
/**
* @return   ETrue   If the number candidate shown. EFalse otherwise.
*/
inline TBool CAknFepManager::IsAdvancedPredictiveNumberCandidateShown() const
	{
	return iIsNumberCandidateShown;
	}
  
/**
* @return   ETrue   If the number candidate shown. EFalse otherwise.
*/
inline TBool CAknFepManager::AdvancedPredictivePrimaryCandidate() const
	{
	return iPrimaryCandidate;
	} 
#endif //RD_INTELLIGENT_TEXT_INPUT

inline TBool CAknFepManager::WasLastKeyPressAHashKey()
	{
	return iLastKeyPressedWasHashKey;
	}
    
inline void CAknFepManager::SetLastPressedHashKeyStatus(TBool aStatus)
	{
	iLastKeyPressedWasHashKey = aStatus;
	}
	
inline TCoeInputCapabilities& CAknFepManager::InputCapabilities()
    {
    return iInputCapabilities;    
    }

inline TFepSymbolOfHardwareOne CAknFepManager::SymbolInfoOfHardKey1()
    {
    return iSymbolData;
    }
inline TBool CAknFepManager::IsSupportNativeNumber() const
    {
    return ( iLanguageCapabilities.iLocalDigitType == EDigitTypeArabicIndic
        || iLanguageCapabilities.iLocalDigitType == EDigitTypeEasternArabicIndic
        || iLanguageCapabilities.iLocalDigitType == EDigitTypeDevanagari );
}

inline TBool CAknFepManager::IsSupportedAdaptiveSearch() const
    {
    return iAknEditorFlags & EAknEditorFlagAdaptiveSearch;
    }
    
inline void CAknFepManager::SetGainForeground( TBool aGainForeground )  
    {
    iGainForeground = aGainForeground;
    }

/**
* Return active input language, only used for languages popup list
*/    
inline TInt CAknFepManager::ActiveInputLanguage() const
    {
    return iActiveInputLanguage;
    }

/**
* Set active input language, only used for languages popup list
*/ 
inline void CAknFepManager::SetActiveInputLanguage( TInt aInputLanguage )  
    {
    iActiveInputLanguage = aInputLanguage;
    }

#endif

// End of file
