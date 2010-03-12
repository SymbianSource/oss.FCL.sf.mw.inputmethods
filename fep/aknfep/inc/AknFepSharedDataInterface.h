/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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












#ifndef _AKNFEP_SHARED_DATA_INT_H__
#define _AKNFEP_SHARED_DATA_INT_H__

#include <e32base.h>

#include <e32property.h>
#include <cenrepnotifyhandler.h>
class CRepository;

class CAknFepRepositoryWatcher : public CBase, public MCenRepNotifyHandlerCallback
    {
public:
    static CAknFepRepositoryWatcher* NewL(
        const TUid aUid,
        const TUint32 aKey,
        CCenRepNotifyHandler::TCenRepKeyType aKeyType,
        TCallBack aCallBack,
        CRepository* aRepository);

    static CAknFepRepositoryWatcher* NewL(
        const TUid aUid,
        TCallBack aCallBack,
        CRepository* aRepository);

    ~CAknFepRepositoryWatcher();

    TUint32 ChangedKey();
#ifdef RD_INTELLIGENT_TEXT_INPUT   
    inline TBool IsGenericCenrepChange(){return iIsGeneric;}
#endif    

public: // from MCenRepNotifyHandlerCallback
    void HandleNotifyInt    (TUint32 aKey, TInt aNewValue);
    void HandleNotifyError  (TUint32 aKey, TInt aError, CCenRepNotifyHandler* aHandler);
	void HandleNotifyGeneric(TUint32 aId);    

private:
    CAknFepRepositoryWatcher(
        const TUid aUid,
        const TUint32 aKey,
        TCallBack aCallBack,
        CRepository* aRepository);

    void ConstructL(CCenRepNotifyHandler::TCenRepKeyType aKeyType);

    void ConstructL();

private:
    TUid                    iUid;
    TUint32                 iKey;
    TUint32                 iChangedKey;
    TCallBack               iCallBack;
    CRepository*            iRepository;
    CCenRepNotifyHandler*   iNotifyHandler;
#ifdef RD_INTELLIGENT_TEXT_INPUT    
    TBool                   iIsGeneric;
#endif    
    };


class CAknFepManager;

class CAknFepSharedDataInterface : public CBase
    {
public:
    /**
     * Two phase constructor for this class
     */
    static CAknFepSharedDataInterface* NewL(CAknFepManager* aFepManager);

    /**
     * C++ Destructor
     */
    ~CAknFepSharedDataInterface();

public:
    /**
     * Queries the value of PredictiveTextOn from shared data
     *
     * @return 1 for on or 0 for off
     */
    TInt PredictiveTextOn();

    /**
     * Queries the value of Japanese PredictiveTextOn from shared data
     *
     * @since 2.6
     * @return 1 for on or 0 for off
     */
    TInt PredictiveTextJapaneseOn();

    /**
     * Queries the value of InputMode from shared data
     *
     * @since 2.6
     * @return value of current input mode
     */
    TInt InputMode();

    /**
     * Sets the value of InputMode in shared data to the passed value
     *
     * @since 2.6
     * @param aValue new value set to InputMode  shared data item which
     *               represents the input mode currently being used
     */
    void SetInputMode(TInt aInputMode);

    /**
     * Queries the value of CangJieMode from shared data
     *
     * @since 3.0
     * @return value of current input mode
     */
    TInt CangJieMode();

    /**
     * Sets the value of CangJieMode in shared data to the passed value
     *
     * @since 3.0
     * @param aValue new value set to CangJieMode shared data item which
     *               represents the input mode currently being used
     */
    void SetCangJieMode(TInt aNewCangJieMode);

    /**
     * Queries the value of iInputTextLang from shared data
     *
     * @return language code of currently used language
     */
    TInt InputTextLanguage();

    /**
     * Sets the value of InputTextLanguage in shared data to the passed value
     *
     * @param aValue new value set to InputTextLanguage shared data item which
     *               represents the language code currently being used
     */
    void SetInputTextLanguage(TInt aValue);

    /**
     * Queries the value of iDisplayLang from shared data
     *
     * @return language code of currently display language
     */
    TInt DisplayLanguage();
#ifdef RD_INTELLIGENT_TEXT_INPUT
    /**
     * Sets the Fn key state and copies it to Cenrep.
     *
     * @return Nothing
     */
    void SetFnKeyState(TInt aState);
    TInt GetFnKeyState();
#ifdef FF_DUAL_LANGUAGE_SUPPORT
    /**
     * Queries the value of secondary input language from shared data
     *
     * @return language code of currently used language
     */
    TInt InputTextLanguageSecondary();
    /**
     * Sets the value of secondary input language in shared data to the passed value
     *
     * @param aValue new value set to InputTextLanguageSecondary shared data item
     *               which represents the language code currently being used
     */
    void SetInputTextLanguageSecondary(TInt aValue);
#endif // FF_DUAL_LANGUAGE_SUPPORT
#endif

    /**
     * Sets the value of PredictiveTextOn to 1 in shared data - this
     * is used to represent predictive text being on
     */
    void SetPredictiveTextOn();

    /**
     * Sets the value of PredictiveTextOn to 0 in shared data - this
     * is used to represent predictive text being off
     */
    void ResetPredictiveTextOn();

    /**
     * Sets the value of Japanese PredictiveTextOn to 1 in shared data
     * - this is used to represent Japanese predictive text being on
     *
     * @since 2.6
     */
    void SetJapanesePredictiveTextOn();

    /**
     * Sets the value of Japanese PredictiveTextOn to 0 in shared data
     * - this is used to represent Japanese predictive text being off
     *
     * @since 2.6
     */
    void ResetJapanesePredictiveTextOn();

    /**
     * Query if KAknFepScrollLatinPredictive feature is enabled in shared data.
     *
     * @since 2.6
     * @return Value of KAknFepScrollLatinPredictive setting.
     */
    TInt ScrollLatinPredictiveCandidates() const;

    /**
     * Query if KAknFepSpaceWithScrollRight feature is enabled in shared data.
     *
     * @since 2.6
     * @return Value of KAknFepSpaceWithScrollRight setting.
     */
    TInt SpaceWithScrollRight() const;

    /**
     * Query if KAknFepEnterWithScrollDown feature is enabled in shared data.
     *
     * @since 2.6
     * @return Value of KAknFepEnterWithScrollDown setting.
     */
    TInt EntersWithScrollDown() const;

    /**
     * Query Qwerty input mode.
     *
     * @since 2.6
     * @return Value of KAknQwertyInputModeActive setting.
     */
    TInt QwertyInputMode() ; //const;
#ifdef RD_INTELLIGENT_TEXT_INPUT
    /**
     * Reads Cenrep and returns the keyboard layout 
     *
     */
    TInt KeyboardLayout() ;//const
#endif
    
    
    /**
    * Returns a value indicating whether edit-submenu functionality should be 
    * used in options menu.
    *
    * @since 3.1
    * @return Value indicating whether edit submenu should be used.
    */
	TInt EditSubmenuInUse() const;
	
	/**
    * Returns a value indicating whether edit-submenu functionality should be 
    * used in options menu.
    *
    * @since 3.1
    * @return Value indicating whether edit submenu should be used.
    */	
	TInt HashKeySelectionInUse();	

    /**
     * Multitap timer value. 
     * Replaces the constant KMultiTapTimeoutMS in Japanese variant.
     *
     * @since 3.0
     * @return Value of KAknFepMultiTapTimer setting.
     */
    TInt MultiTapTimer() const;
    /**
     * Japanese Qwerty Flags value. 
     * Replaces the constant KAknFepJapaneseSpecialCharFlag in Japanese variant.
     *
     * @since 3.0
     * @return Value of KAknFepJapaneseSpecialCharFlag setting.
     */
    TInt JapaneseQwertyFlags() const;

    /**
     * Get the last used plugin input mode
     *
     * @since 3.2
     * @return Value of KAknFepLastUsedInput setting.
     */
    TInt PluginInputMode() const;

    /**
     * Replaces the last used plugin input mode
     *
     * @since 3.2
     * @return Replace value of KAknFepLastUsedInput setting.
     */
    void SetPluginInputMode(TInt aId);


    /**
     * Get the last used plugin UI language
     *
     * @since 3.2
     * @return Value of KAknFepLastUsedUILanguage setting.
     */
    TInt PluginUILanguage() const;

    /**
     * Replaces the last used plugin UI language
     *
     * @since 3.2
     * @param aLanguage Replace value of KAknFepLastUsedUILanguage setting.
     */
    void SetPluginUILanguage(TInt aLanguage);


    /**
     * Get current pen input language
     * @since 3.0
     *
     * @return Value of KSettingsPenInputLang
     */
    TInt PenInputLanguage() const;
    
    /**
     * Replaces the value of number input mode
     *
     * @since 3.2
     * @param aMode (KSettingsDefaultNumberMode or KSettingsIndicDefaultNumberMode)
     * @param aValue a value
     */
	void SetDefaultNumberMode( TInt aMode, TInt aValue );

    /**
     * Returns a value of current clear direction.
     *
     * @since 3.2
     * @return Value of KAknFepClearDirection setting.
     */
    TInt ClearDirection() const;

    /**
     * Set a value of current clear direction.
     *
     * @since 3.2
     * @param aValue new value set to Clear Direction shared data item which
     *               represents the direction currently being used
     */
    void SetClearDirection(TInt aValue);
    /**
     * Queries the value of PredictiveText Auto Word Completion state to be On from shared data
     *
     * @return 1 for on or 0 for off
 	 * Since 5.0    
     */
	TInt CAknFepSharedDataInterface::PredictiveTextAutoCompleteOn();

    
    /**
     * Sets the value of PredictiveText Auto Word Completion state to 1 in shared data - this
     * is used to represent predictive text Auto Word Completion being on
 	 * Since 5.0    
     */    
	void CAknFepSharedDataInterface::SetPredictiveTextAutoCompleteOn();


    /**
     * Sets the value of PredictiveText Auto Word Completion state to 0 in shared data - this
     * is used to represent predictive text Auto Word Completion being off
 	 * Since 5.0    
     */    
	void CAknFepSharedDataInterface::ResetPredictiveTextAutoCompleteOn();
#ifndef RD_INTELLIGENT_TEXT_INPUT

    /**
     * Handles a change in predictive text Auto Word Completion setting from general settings
 	 * Since 5.0    
     */    
	void CAknFepSharedDataInterface::HandlePredictiveTextAutoCompleteSettingGSChange();
#endif
#ifdef RD_INTELLIGENT_TEXT_INPUT
  
    /**
     * Handles a change in predictive text Auto Word Completion setting from general settings
 	 * Since 5.0    
     */    
	void HandlePredictiveTextAutoCompleteSettingGSChange();
#endif	//

    /**
     * Returns a value of Hwr conversion ON/OFF.
     *
     * @since 3.2
     * @return Value of KAknFepJapaneseHwrConversion value
     */
    TInt JapaneseHwrConversion() const;

    /**
     * Set a value of Hwr conversion ON/OFF.
     *
     * @since 3.2
     * @param aValue new value set to Hwr conversion ON/OFF shared data item which
     *               represents the conversion ON/OFF currently being used
     */
    void SetJapaneseHwrConversion(TInt aValue);
#ifdef RD_INTELLIGENT_TEXT_INPUT
    /**
     * Queries the value of PredictiveText Typing Correction Level from shared data
     *
     * @return Typing Correction Level
 	 */
	TInt PredictiveTextTypingCorrectionLevel();

    
    /**
     * Sets the value of PredictiveText Typing Correction Level state to aCorrectionLevel in shared data 
 	 */    
	void SetPredictiveTextTypingCorrectionLevel(TInt aCorrectionLevel);


     /**
     * Handles a change in predictive text Typing Correction Level setting.
 	 */    
	void HandlePredictiveTextTypingCorrectionLevelChange();

    /**
     * Queries the value of PredictiveText Number Candidate Shown state to be On from shared data
     *
     * @return 1 for shown or 0 for not shown
 	 */
	TInt PredictiveTextNumberCandidateShown();

    
    /**
     * Sets the value of PredictiveText Number Candidate Shown state to 1 in shared data - this
     * is used to represent predictive text Number Candidate Shown being shown.
 	 */    
	void SetPredictiveTextNumberCandidateShown();


    /**
     * Sets the value of PredictiveText Number Candidate Shown state to 0 in shared data - this
     * is used to represent predictive text Number Candidate Shown being not shown.
 	 */    
	void ResetPredictiveTextNumberCandidateShown();


    /**
     * Handles a change in predictive text NumberCandidateShown setting.
 	 */    
	void HandlePredictiveTextNumberCandidateSettingChange();
	
	/**
     * Queries the value of PredictiveText Primary candidate setting from shared data
     *
     * @return Primary candidate setting(0 for Exact or 1 for Suggested)
 	 */
	TInt PredictiveTextPrimaryCandidate();

    
    /**
     * Sets the value of PredictiveText Primary candidate setting state to aPrimaryCandidate in shared data 
 	 */    
	void SetPredictiveTextPrimaryCandidate(TInt aPrimaryCandidate);


     /**
     * Handles a change in predictive text Primary candidate setting.
 	 */    
	void HandlePredictiveTextPrimaryCandidateChange();
#endif	// RD_INTELLIGENT_TEXT_INPUT

    TInt FepShowFsqPreviewStatus();

    void SetFepShowFsqPreviewStatus(TInt aValue);

    TInt FepShowVkbPreviewStatus();

    void SetFepShowVkbPreviewStatus(TInt aValue);

    TInt FepLastUseVkbModeForLandscape();

    void SetFepLastUseVkbModeForLandscape(TInt aValue);

    TBool AutoRotateEnabled();
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT
	/**
	* Queries if numbers (and other characters found behind Fn) can be entered with long key press on QWERTY.
	*/
	TBool LongPressNumberEntryOnQwerty() const;
	/**
	* Queries if copy-pasting with shift+softkeys is available also on QWERTY.
	*/
	TBool ShiftCopyPastingOnQwerty() const;
#endif // RD_INTELLIGENT_TEXT_INPUT
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef  RD_SCALABLE_UI_V2
    /**
     * Get a value of current default on screen vkb.
     * It depends on input lanage is PRC/HW/TW Chinese.
     *
     * @return Value of current default on screen vkb
     */
    TInt DefaultOnScreenVKB();
#endif
    
    /**
     * Returns a value of default arabic finger hwr orientation.
     *
     * @since 5.2
     * @return 0 is portrait.
     *         1 is landscape.
     */
    TInt DefaultArabicFingerHwrOrientation() const;
    
    /**
     * Set a value of default arabic finger hwr orientation.
     *
     * @since 5.2
     * @param aValue new value set to default arabic finger hwr orientation shared data item which
     *               represents the default orientation.
     *               0 is portrait.
     *               1 is landscape.
     */
    void SetDefaultArabicFingerHwrOrientation(TInt aValue);
private:
    /**
     * C++ Constructor
     */
    CAknFepSharedDataInterface(CAknFepManager* aFepManager);
    /**
     * Second phase constructor for this class
     */
    void ConstructL();

private:

    /**
     * Handles a change in hash key selection setting
     */
    static TInt HashKeySelectionNotification(TAny* aObj);
    
    /**
     * Handles a change in input mode from general settings
     */
    static TInt InputModeChangeGSNotification(TAny* aObj);

    /**
     * Handles a change in input language from general settings
     */
    static TInt InputTextLangGSNotification(TAny* aObj);

    /**
     * Handles a change in predictive text setting from general settings
     */
    static TInt PredictiveTextSettingChangeGSNotification(TAny* aObj);
#ifndef RD_INTELLIGENT_TEXT_INPUT

    /**
     * Handles a change in predictive text Auto Word Completion setting from general settings
 	 * Since 5.0    
     */    
	static TInt PredictiveTextAutoCompleteSettingChangeGSNotification(TAny* aObj);
#endif	// RD_PRED_AW_COMPLETION
#ifdef RD_INTELLIGENT_TEXT_INPUT
    /**
    Handles the change in keyboard layout.
    */
    void HandleKeyboardLayoutChange();
    /**
     * Call back function to handle a change in keyboard layout.
     */	
    static TInt HandleKeyboardLayoutChangeNotification(TAny* aObj);
#endif
#ifdef RD_INTELLIGENT_TEXT_INPUT

    /**
     * Handles a change in predictive text Auto Word Completion setting from general settings
 	 * Since 3.2    
     */    
	static TInt PredictiveTextAutoCompleteSettingChangeGSNotification(TAny* aObj);
#endif	// RD_INTELLIGENT_TEXT_INPUT
    /**
     * Handles a change in Japanese predictive text setting from general settings
     */
    static TInt JapanesePredictiveTextSettingChangeGSNotification(TAny* aObj);
#ifdef RD_INTELLIGENT_TEXT_INPUT
    /**
     * Handles a change in predictive text Typing Correction Level setting.
 	 */    
	static TInt PredictiveTextTypingCorrectionLevelChangeNotification(TAny* aObj);
	
	/**
     * Handles a change in predictive text Number Candidate setting.
 	 */    
	static TInt PredictiveTextNumberCandidateSettingChangeNotification(TAny* aObj);
	
	/**
     * Handles a change in predictive text Primary Candidate setting.
 	 */    
	static TInt PredictiveTextPrimaryCandidateChangeNotification(TAny* aObj);
#endif	// RD_INTELLIGENT_TEXT_INPUT

    /**
     * Handles a change in qwerty input mode setting.
     */
    static TInt QwertyModeChangeNotification(TAny* aObj);

    /**
     * Handles a change in hash key selection
     */
    void HandleHashKeySelectionChange();
    
    /**
     * Handles a change in input mode from general settings
     */
    void HandleInputModeGSChange();

    /**
     * Handles a change in input language from general settings
     */
    void HandleInputTextLanguageGSChange();

    /**
     * Handles a change in predictive text setting from general settings
     */
    void HandlePredictiveTextSettingGSChange();

    /**
     * Handles a change in Japanese predictive text setting from general settings
     */
    void HandleJapanesePredictiveTextSettingGSChange();

    /**
     * Call back function to handle a change in MultiTapTimer from general settings
     */
    static TInt MultiTapTimerChangeGSNotification(TAny* aObj);
    /**
     * Handles a change in MultiTapTimer from general settings
     */
    void HandleMultiTapTimerChangeGSNotification();

    /**
     * Call back function to handle a change in Japanese Qwerty Flags from general settings
     */
    static TInt JapaneseQwertyFlagsChangeGSNotification(TAny* aObj);
    /**
     * Handle change notification for Japanese Qwerty Flags. 
     */
    void HandleJapaneseQwertyFlagsChangeGSNotification();

    /**
     * Handles a change in qwerty input mode setting.
     */
    void HandleQwertyModeChangeNotification();
    TInt NumberModeChangesGSNotification();

    static TInt HandleRepositoryCallBack(TAny* aPtr);
#ifdef RD_INTELLIGENT_TEXT_INPUT    
    static TInt HandleGenericRepositoryChange(TAny* aPtr);
#endif    
    static TInt HandleLocaleRepositoryCallBack(TAny* aPtr);

    class CSubscriber : public CActive
        {
    public:
        CSubscriber(TCallBack aCallBack, RProperty& aProperty);
        ~CSubscriber();

    public: // New functions
        void SubscribeL();
        void StopSubscribe();

    private: // from CActive
        void RunL();
        void DoCancel();

    private:
        TCallBack   iCallBack;
        RProperty&  iProperty;
        };

    CSubscriber*    iQwertyModeStatusSubscriber;
    RProperty       iQwertyModeStatusProperty;
#ifdef RD_INTELLIGENT_TEXT_INPUT
    CSubscriber*    iKeyboardLayoutStatusSubscriber;
    RProperty       iKeyboardLayoutStatusProperty;
#endif

    friend class CSubscriber;

private:
    CAknFepManager* iFepManager; //not owned

    CRepository*                iCommonEngineRepository;
    CRepository*                iAknFepRepository;
    CRepository*                iGsPenSettings;   //for global setting 
    CRepository*				iLocaleRepository; // locale related settings
    CRepository*                iSensorRepository;
    CAknFepRepositoryWatcher*   iAknFepRepositoryWatcher;
    CAknFepRepositoryWatcher*   iAknFepLocaleRepositoryWatcher;

    TInt iSharedDataScrollLatinPredictive;
    TInt iSharedDataSpaceWithScrollRight;
    TInt iSharedDataEnterWithScrollDown;
    
    TInt iHashKeySelectionInUse;
    };

#endif

// End of File
