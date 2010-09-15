/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
*      Provides the TAknFepInputMiniQwertySogouStrokePhrasePlugin declaration.
*
*/


#ifndef T_AKNFEPUIINPUTMINIQWERTYSOGOUSTROKEPHRASEPLUGIN_H
#define T_AKNFEPUIINPUTMINIQWERTYSOGOUSTROKEPHRASEPLUGIN_H

class TAknFepInputMiniQwertySogouStrokePhrasePlugin
    {
public:
    
    /**
     * C++ default constructor
     *
     * @since S60 v3.2.3
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     * @param aState current state machine
     */
    TAknFepInputMiniQwertySogouStrokePhrasePlugin(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer,
        TUIState aState
        );
    
    /**
     * A patch to force the vtable to be copied during an assignment.
     * 
     * @param aState
     */
    void operator=(const TAknFepInputMiniQwertySogouStrokePhrasePlugin& aPlugin);
    
public:       
        
        /**
         * Add phrase pinyin to the core.
         *  
         * @since S60 v3.2.3.
         * @param aPhraseAdd that need to be added to the UDB.
         * @return None.
         */ 
        void AddPhrasePinyinToPti(); 
        
        /**
         * Commint text to EEP or editor
         *
         * @since S60 V3.2.3
         * @param aDes Text had been select from candidate pane
         * @return ETrue if keep state, otherwise need to change to pridictive or editor
         */
        TBool CommitInlineEEPL( const TDesC& aDes );        
        
        /**
         * Enable or disable the plugin.
         *
         * @since S60 v3.2.3
         * @param aValid ETrue: enable the plugin, EFalse: disable the plugin.
         * @return None
         */
        void Enable( TBool aValid );
        
        /**
         * Detect whether the plugin enable or not.
         *
         * @since S60 v3.2.3
         * @param None
         * @return ETrue if the plugin is enable, others return EFalse.
         */
        TBool IsEnable();        
        
private:
        /**
         * The ui manager
         */        
        MAknFepUIManagerStateInterface* iOwner;
        
        /**
         * The ui container.
         */
        MAknFepUICtrlContainerChinese* iUIContainer;
        
        /**
         * The machine state
         */           
        TUIState iState;
        
        /**
         * The flag for judge the plugin enable or not
         */               
        TBool iValid;
    };		

#endif //T_AKNFEPUIINPUTMINIQWERTYSOGOUSTROKEPHRASEPLUGIN_H

// End of file
