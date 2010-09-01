/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Data model for GSLangPlugin.
*
*/

#ifndef  GSLANGMODEL_H
#define  GSLANGMODEL_H

// INCLUDES
#include    "GSLangPlugin.hrh"

#include    <backgroundimage.h>
#include    <badesca.h>
#include    <bldvariant.hrh>
#include    <centralrepository.h>
#include    <cenrepnotifyhandler.h>
#include    <e32base.h>
#include    <generalsettingsvariant.hrh>
#ifdef RD_INTELLIGENT_TEXT_INPUT
#include    <PtiDefs.h>
#endif
//class CCommsDatabase;
class CGSLangContainer;

#ifdef RD_INTELLIGENT_TEXT_INPUT
class CCoeEnv;
#endif

// CONSTANTS
const TInt  KGSLanguageMask = 0x03FF;
const TInt  KGSPredTxtFlagDefault = 1; // Default ON
#ifdef RD_INTELLIGENT_TEXT_INPUT
const TInt  KGSPredTxtBitMaskDefault = 0xFFFF;
#endif

// For switching values in SwitchValue()
const TInt KGSSettingOff = 0;

_LIT( KGSWelcomeNoteImgPath, "c:\\resource\\apps\\welcomeimage.mbm" );

// Multitap timer default value
const TInt  KGSMultiTapTimerDefault = 1000000;   // 1.0sec
// Japanese Special Char Flag default value
const TInt  KGSJapaneseSpecialCharFlagDefault = 0x000F;

/**
*  Data model for GSLangPlugin.
*  @lib GSLangPlugin.dll
*  @since Series60 3.1
*/
class CGSLangModel : public CBase,
                     public MCenRepNotifyHandlerCallback
   {
    public:  // Constructor and destructor
        /**
        * Two-phased constructor
        */
        static CGSLangModel* NewL( CGSLangContainer* aContainer );

        /**
        * Destructor
        */
        ~CGSLangModel();

    public: // Enums
        enum TLanguage
            {
            // Phone language
            EGSUiLanguage,
            // Writing language
            EGSInputLanguage
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef FF_DUAL_LANGUAGE_SUPPORT
            // Secondary writing language
            ,EGSInputLanguageSecondary
#endif            
#endif
            };

        enum TPredictiveType
            {
            // Japanese predictive
            EGSLangJapanese,
            // predictive for other (common) languages
            EGSLangOther
            };
    public: // New

        /**
        * Get the list of supported languages from PTIEngine
        * @param aInstalledLangCodes array of installed lang codes
        *                            from SysLangUtil
        * @param aSeedLanguages array of Pti Language codes
        * @param aLanguageNames array for language names
        *
        * @since 2.6
        */
        void GetLanguagesFromPtiL(
                      CArrayFix<TInt>* aInstalledLangCodes,
                      CArrayFix<TInt>* aSeedLanguages,
                      CDesCArray* aLanguageNames );

        /**
        * Display/Input Text Language Selection.
        * @param aType Language type (UI/Input)
        * Returns language value(Symbian OS value) from HAL, or from CenRep
        * in some special cases.
        * @return 0: Automatic
        *         1: English
        *    2: French
        *    3. German
        *    4. Spanish
        *    5. Italian
        *    6. Swedish
        *    7. Danish
        *    8. Norwegian
        *    9. Finnish
        *   13. Portuguese
        *   14. Turkish
        *   15. Icelandic
        *   16. Russian
        *   17. Hungarian
        *   18. Dutch
        *   25. Czech
        *   26. Slovak
        *   27. Polish
        *   28. Slovenian
        *   39. Tagalog
        *   42. Bulgarian
        *   45. Croatian
        *   49. Estonian
        *   54. Greek
        *   59. Indonesian
        *   67. Latvian
        *   68. Lithuanian
        *   70: Malaysia
        *   78. Romanian
        *   79. Serbian
        *   93: Ukrainian
        */
        TInt LanguageL( TLanguage aType );

        /**
        * Set Display/Input Text Language value to HAL and/or CenRep
        * @param aType Language type (UI/Input)
        * @param aLanguage Symbian OS Value of Display Text Language.
        *
        * @return ETrue: no errors
        *         EFalse: an error has occurred
        */
        TBool SetLanguageL( TLanguage aType, TInt aLanguage );

        /**
        * Predictive Text Language selection.
        * Get setting value from CenRep
        * @param aPredictive Japanese/Other
        * @return 0: Off
        *         1: On
        */
        TInt PredictiveOnOff( TPredictiveType aPredictive );

        /**
        * Set Predictive Text Lanuage value to CenRep
        * @param aPredictive Japanese/Other
        * @param aMode OnOff Value of T9/FEP
        *
        * @return ETrue: no errors
        *         EFalse: an error has occurred
        */
        TBool SetPredictiveOnOff( TPredictiveType aPredictive, TInt aMode );

        /*
        * Returns Bool value according to predictive dictionary availability.
        * PTI Engine used for lang avaibility retrieving
        * @return ETrue: no errors
        *         EFalse: an error has occurred
        * @since 2.6
        */
        TBool CheckDictionaryFromPtiL(TInt aLangCode, TBool aCheckEngine = EFalse);

#ifdef RD_INTELLIGENT_TEXT_INPUT
        /*
        * Returns Bool value according to predictive dictionary availability.
        * PTI Engine used for lang avaibility retrieving
        * @return ETrue: no errors
        *         EFalse: an error has occurred
        */
        TBool CheckT9FromPtiLForPredictionL(TPtiEngineInputMode inputMode = EPtiEngineInputModeNone, TInt aLangCode = 0);
#endif      

        /* Gets default number mode when arabic language is used.
        * @param aNbrModeType number mode type from GSLangPlugin.hrh
        * @return TInt corresponding value in language variant
        */
        TInt DefaultNumberMode( TInt aNbrModeType );

        /* Sets default number mode when arabic language is used.
        * @param aMode either Latin or Arabic
        * @param aNbrModeType number mode type from GSLangPlugin.hrh
        */
        void SetDefaultNumberMode( TInt aMode, TInt aNbrModeType );

        /*
        * Default Input Method Selection.
        * Returns input method value(In Hexadecimal value) from CenRep.
        * @return 0x10:  (Pinyin)
        *         0x20:  (Zhuyin)
        *         0x40:  (Stroke)
        *         0x80:  (Latin Upper Case)
        *         0x100: (Latin Lower Case)
        */
        TInt DefaultInputMethodL();

        /*
        * Sets default Input Method.
        * @param aMethod Permanent input method .
        */
        void SetDefaultInputMethodL( TInt aMethod );

        /**
        * Predictive On/Off
        * @param
        * @param
        */
        void ChangePredictiveOnOffSettingL( TBool aShowOnOffSettingPage,
                                            TPredictiveType aPredictive );

        /**
        * Shows predictive setting page.
        */
        TBool ShowPredictiveOnOffSettingPageL( TInt& aOnOffcurrent );

        /**
        * Checks call state.
        */
        TInt CheckCallStateL();

        /**
        * Keypress timeout (means Multitap timer).
        * Get setting value
        * @since 3.0
        * @return milli second
        *
        */
        TInt KeypressTimeout();

        /**
        * Set Keypress timeout value to CenRep 
        * Keypress timeout value means Multitap timer.
        * @since 3.0
        * @param aMilliSecond
        * @return ETrue: no errors
        *         EFalse: an error has occurred
        */
        TBool SetKeypressTimeout( TInt aMilliSecond );

        /**
        * Japanese qwerty specific flags.
        * Get setting value of each apanese qwerty specific flags
        * @since 3.0
        * @param aFlag one of flag is set
        * @return TBool ON/OFF or Full/Half-width
        *  (Flags)        : (Arguments)                    : (Return value)
        *  Comma          : EGSJapQwertyFlagComma          : ETrue-Japanese comma
        *  Period         : EGSJapQwertyFlagPeriod         : ETrue-Japanese period
        *  Width Of Space : EGSJapQwertyFlagSpaceFullWidth : ETrue-Full width
        */
        TBool JapanQwertyFlags( const TGSJapQwertyFlags aFlag );

        /**
        * Set Keypress timeout value to CenRep 
        * Set value of each Japanese qwerty specific flags
        * @since 3.0
        * @param aFlag one of flag is set
        * @param aValue is set ON/OFF
        * @return ETrue: no errors
        *         EFalse: an error has occurred
        */
        TBool SetJapanQwertyFlags( const TGSJapQwertyFlags aFlag, 
                                   TBool aValue );
        
        /**
        * Get Chinese CangJie input method value
        * @return currently selected setting page index
        */
        TInt ChiCangJieInputL();
        
        /**
        * Set Chinese CangJie input method value
        * @param newly selected setting page index
        */
        void SetChiCangJieInputL( TInt aMode );
        
        /**
        * Check if Chinese Cangjie input is supported
        * @param newly selected setting page index
        */
        TBool CheckCangjieSupportL( TInt aLangCode );
        
        /*
        * Get Predictive mode value
        * 0 --> Normal 
        * 1 --> Autocompletion
        * @return currently selected setting page index
        */
        TInt PredictiveModeL();
            
        /*
        * Set Predictive mode value 
        * 0 --> Normal 
        * 1 --> Autocompletion
        * @param newly selected setting page index
        */
        void SetPredictiveModeL( TInt aMode );
        
        /*
        * Checks free disk space on device for language switch
        * @return ETrue if there is not enough free space
        * @return EFalse if there is enough free space
        */
        TBool CheckDiskSpaceL();
        
        /*
        * Checks for autoword completion feature support
        * @return ETrue if supported
        * @return EFalse if not supported
        */
        TBool CheckAutoWordCompletionSupportL();
        
#ifdef RD_INTELLIGENT_TEXT_INPUT
        /*
        * Checks for multiple keyboards
        * @return ETrue if device has multiple keyboards
        * @return EFalse if device has one keyboard
        */
        TBool DeviceHasMultipleKeyboardsL();

        TInt PredictionBitMaskL();

        void SetPredictionBitMaskL(TInt aBitMask);

        TInt PhysicalKeyboardsL();
        
        /**
        * Get the list of supported secondary input languages
        * Some input languages will not be on this list, e.g.
        * Chinese, Japanese etc.
        * @param aSeedLanguages array of Pti Language codes
        * @param aLanguageNames array for language names
        *
        * @since 3.2
        */
        void GetSecondaryInputLanguagesL(
                      CArrayFix<TInt>* aSeedLanguages,
                      CDesCArray* aLanguageNames,
                      CCoeEnv* aCoeEnv);
#endif
        
    private: // from MCenRepNotifyHandlerCallback
        void HandleNotifyInt( TUint32 aId, TInt aNewValue );
        
    private: // Private constructors

        /**
        * Default C++ contructor
        */
        CGSLangModel( CGSLangContainer* aContainer );

        /**
        * Symbian OS default constructor
        * @return void
        */
        void ConstructL();

    private:
        void InitializeCentralRepositoriesL();
        void CloseCentralRepositories();
        void HandleCenrepValueChangedL( TUint32 aId );

    private: // Data

        // Pointer to container. Not owned.
        CGSLangContainer* iContainer;

        CRepository* iAknFepRepository;
        CRepository* iCommonEngineRepository;
        CRepository* iPersonalizationRepository;
        CRepository* iLocaleRepository;
        CRepository* iStartupConfRepository;
        CRepository* iScreensaverRepository;        
        // CenRep notify handler for input language
        CCenRepNotifyHandler* iNotifyHandlerForITL;
        // CenRep notify handler for dictionary
        CCenRepNotifyHandler* iNotifyHandlerForT9;
#ifdef RD_INTELLIGENT_TEXT_INPUT
        // CenRep notify handler for prediction extension
        CCenRepNotifyHandler* iNotifyHandlerForPredExt;
#endif
        // CenRep notify handler for Japanese predictive mode
        CCenRepNotifyHandler* iNotifyHandlerForJapanesePredTxt;
    };


#endif // GSLANGMODEL_H
// End of File
