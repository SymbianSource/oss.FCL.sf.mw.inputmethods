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
* Description:  GSLangPlugin header file.
*
*/

#ifndef GSLANGPLUGIN_H
#define GSLANGPLUGIN_H

// Includes
#include "GSLangPlugin.hrh"

#include <gsbaseview.h>
#include <gsfwviewuids.h>
#include <gsplugininterface.h>

#include <aknsettingpage.h>
#include <aknview.h>
#include <ConeResLoader.h>
#include <eikclb.h>
#include <eikmenup.h>
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
#include <e32property.h>
#endif

// Classes referenced
class CAknViewAppUi;
class CGSLangContainer;
class CEikDialog;
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
class CRepository;
#endif

// Constants
const TUid KGSLangPluginUID = { 0x10275067 };
_LIT( KGSDefaultLangIconFileName, "Z:\\resource\\apps\\GSLangplugin.mbm");
const TInt KGSBufSize128 = 128;
const TInt KGSMapSize = 8;

_LIT( KGSLangPluginResourceFileName, "z:GSLangPluginRsc.rsc" );

// CLASS DECLARATION

/**
*  CGSLangPlugin view class.
*  @lib GSLangPlugin.dll
*  @since Series60 3.1
*/
class CGSLangPlugin : public CGSBaseView
    {
    public: // Constructors and destructor

        /**
        * Symbian OS two-phased constructor
        * @return
        */
        static CGSLangPlugin* NewL( TAny* aInitParams );

        /**
        * Destructor.
        */
        ~CGSLangPlugin();

    public: // New

        /**
        * Getter for CGSLangPlugin's ccontainer.
        */
        CGSLangContainer* Container();

    public: // From CAknView

        /**
        * See base class.
        */
        TUid Id() const;

        /**
        * See base class.
        */
        void HandleClientRectChange();

        /**
        * See base class.
        */
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );

        /**
        * See base class.
        */
        void DoDeactivate();

        /**
        * See base class.
        */
        void HandleCommandL( TInt aCommand );

    protected:

        /**
        * C++ default constructor.
        */
        CGSLangPlugin();

        /**
        * Symbian OS default constructor.
        */
        void ConstructL();

    public: // From CGSPluginInterface

        /**
        * See base class.
        */
        void GetCaptionL( TDes& aCaption ) const;

        /**
        * See base class.
        */
        TInt PluginProviderCategory() const;
        
        /**
        * See base class.
        */
        CGulIcon* CreateIconL( const TUid aIconType );        
        
    protected: // From MEikMenuObserver

        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

    private: // From CGSBaseView

        /**
        * See base class.
        */
        void NewContainerL();

        /**
        * See base class.
        */
        void HandleListBoxSelectionL();

    private: // New

        /**
        * Confirm Boot of DTL
        */
        void ConfirmRebootDTLL( TInt aCurrentLanguage,
                                const TDesC& aLanguageName );

        /**
        * Check current call state using PubSub or SA.
        */
        TInt CheckCallState();

        /**
        * Shows language selection page.
        */
        void ShowLanguageSelectionSettingPageL();

        /**
        * Shows input selection page.
        */
        void ShowInputSelectionSettingPageL();

        /**
        * Shows default input method selection page.
        */
        void ShowDefaultInputMethodSettingPageL();

        /**
        * Shows number mode setting page for A&H.
        */
        TBool ShowAHNumberModeSettingPageL();

        /**
        * Shows number mode setting page for Hindi.
        */
        TBool ShowHindiNumberModeSettingPageL();
        
        /**
        * Resets dictionary.
        */
        void ResetATOKDictionaryL();
        
        /*
        * Create a Keypress timeout setting page
        */
        void ShowKeypressTimeoutSettingPageL();
        
        /*
        * Change Japanese Qwerty settings
        * @param aFlag CenRep flag for Querty keypad
        * @param aItemId current feature ID
        * @param aShowSettingPage boolean, show/hide setting page.
        */
        void ChangeJapQwertySettingL( const TGSJapQwertyFlags aFlag,
                                      const TInt aItemId,
                                      const TBool aShowSettingPage );
        
        /*
        * Display Japanese Qwerty ON/OFF setting page.
        * @param aSettingPageResourceId setting page resourece ID
        * @param aItemArraryResourceId setting page items resourece ID
        * @param aCurrentValue current selection in setting page
        */
        TBool ShowJapQwertySettingPageL( const TInt aSettingPageResourceId,
                                        const TInt aItemArraryResourceId,
                                        const TBool aCurrentValue );
        
        /**
        * Show CangJie input mode setting page
        */
        void ShowChiCangJieInputModeSettingPageL();
        
        /**
        * Show Predictive mode setting page
        */
        void ShowPredictiveModeSettingPageL( const TBool aShowSettingPage );
        
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef FF_DUAL_LANGUAGE_SUPPORT
        void ShowDualInputLanguageSettingListL();
#endif
        
        void ShowITIMultiSettingPageL(TBool aShowOnOffSettingPage);
        void ShowPredictiveSettingListL(); 
        
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
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
        /**
         * Call back function to handle a change in keyboard layout.
         */ 
        static TInt HandleKeyboardLayoutChangeNotification(TAny* aObj);
        
        void HandleKeyboardLayoutChange();
        
        CSubscriber*    iKeyboardLayoutStatusSubscriber;
        RProperty       iKeyboardLayoutStatusProperty;    
        
        friend class CSubscriber;
#endif //__ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__       
        
        TBool           iCascadeOptionsOpen;
#endif //RD_INTELLIGENT_TEXT_INPUT
        CEikDialog* iDialog;
    };

#endif // GSLANGPLUGIN_H
// End of File
