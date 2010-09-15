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
*      Provides the TAknFepInputMiniQwertySogouPinyinPhrasePlugin declaration.
*
*/


#ifndef T_AKNFEPUIINPUTMINIQWERTYSOGOUPINYINPHRASEPLUGIN_H
#define T_AKNFEPUIINPUTMINIQWERTYSOGOUPINYINPHRASEPLUGIN_H

class TAknFepInputMiniQwertySogouPinyinPhrasePlugin
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
    TAknFepInputMiniQwertySogouPinyinPhrasePlugin(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer,
        TUIState aState
        );
    
    /**
     * A patch to force the vtable to be copied during an assignment.
     * 
     * @param aState
     */
    void operator=(const TAknFepInputMiniQwertySogouPinyinPhrasePlugin& aPlugin);
    
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
         * Analyse spelling after user press key.
         * 
         * @since S60 v3.2.3
         * @param None.
         * @return None.
         */
        void AnalyseL();
        
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
        
        /**
         * Get the candidate info.
         * 
         * @since S60 v3.2.3.
         * @param None.
         * @return none.
         */            
        void GetCandidateL();
        
        /**
         * Set whether to convert all stroke.
         * 
         * @since S60 v3.2.3.
         * @param None.
         * @return none.
         */            
        void CanConvertAll( TBool aEnable );
        
        /**
         * Set the convert count.
         * 
         * @since S60 v3.2.3.
         * @param None.
         * @return none.
         */            
        void ConvertCount( TInt aCount );
        
private:
        /**
         * Check the delimiter of spelling.
         * 
         * @since S60 v3.2.3.
         * @param aKeystroke the keystroke.
         * @return ETrue if the param is delimiter, EFalse others.
         */         
        TBool CheckSpellingDLT( const TDesC& aKeystroke );
        
        /**
         * Clear Keystroke of PtiEngine.
         *
         * @since S60 v3.2.3.
         * @param None.
         * @return None.
         */
        void ClearPtiEngineKeystroke();
        
        /**
         * Get pinyin tone mark.
         * 
         * @since S60 v3.2.3.
         * @param aKeystroke,The tone mark.
         * @return pinyin tone mark.
         */         
        TInt ToneMark( const TDesC& aKeystroke );
        
        /**
         * Check the spelling after add tone mark to the PTI.
         * 
         * @since S60 v3.2.3.
         * @param aToneMark,The tone mark.
         * @return ETue,if the splling is not phrase spelling,EFalse others.
         */         
        TBool CheckSpellingAddToneMarkToPTIL( const TInt aToneMark );        
        
        /**
         * get the key code.
         * 
         * @since S60 v3.2.3
         * @param aKey, the keycode.
         * @param aKeystroke, the show keystroke.
         * @return none.
         */
        void GetKeystrokeCode( TInt& aKey, const TDesC& aKeystroke );
        
        /**
         * get the spell of least delimiter
         *
         * @since S60 v3.2.3
         * @return the spell of least delimiter
         */
        TPtrC getCurrentLeastDLTSpell();    
        
        /**
         * check result of tonemark.
         *
         * @since S60 v3.2.3
         * @param aDes The result of tonemark
         * @param aIndex The index
         * @return NULL
         */
        void CheckResultOfToneMark( const TDesC16& aDes,
                                    TInt& aIndex );
        
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
        
        /**
         * The flag for adding DLT at the first index.
         */ 
        TBool iNeedAddDLT;
        
        /**
         * The flag for convert all stroke.
         */ 
        TBool iCanConvertAll;
        
        /**
         * The count of stroke need to be convert.
         */ 
        TInt iConvertCount;
    };		

#endif //T_AKNFEPUIINPUTMINIQWERTYSOGOUPINYINPHRASEPLUGIN_H

// End of file
