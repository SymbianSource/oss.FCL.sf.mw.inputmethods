/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateEntryMiniQwertyZhuyinPhrase,
*       TAknFepInputStateEditingMiniQwertyZhuyinPhrase,
*       TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase definition.
*
*/












#ifndef AKNFEPUIINPUTSTATEMINIQWERTYZHUYINPHRASE_H_
#define AKNFEPUIINPUTSTATEMINIQWERTYZHUYINPHRASE_H_

#include "aknfepuiinputminiqwertyzhuyinphrasebase.h"

/**
 *  TAknFepInputStateEntryMiniQwertyZhuyinPhrase class.
 *
 *  Implementation of Zhuyin phrase editing state
 *
 *  @lib avkonfep.dll
 *  @since S60 v3.2
 */

class TAknFepInputStateEditingMiniQwertyZhuyinPhrase : public TAknFepInputMiniQwertyZhuyinPhraseBase
    {
public:
    /**
     * C++ default constructor
     *
     * @since S60 V3.2
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepInputStateEditingMiniQwertyZhuyinPhrase(
            MAknFepUIManagerStateInterface* aOwner,
            MAknFepUICtrlContainerChinese* aUIContainer );
    
    /**
     * From TAknFepInputStateCandidateBasePhrase
     * Handle system key press
     *
     * @since S60 v3.2
     * @param aKey System key event
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
     TBool HandleKeyL( TInt aKey, TKeyPressLength aLength );
     
     /**
      * Handling Command events.
      * This Method is called by CAknFepUIManagerJapanese.
      *
      * @param aCommandId Command ID value
      */
     void HandleCommandL( TInt aCommandId );
     
     /**
      * Add keystroke.
      * 
      * @since S60 v3.2.3.
      * @param aKey the input keystroke.
      * @return ETrue if that the key can add to the current keystroke, EFase others.
      */            
     TBool AddKeystrokeL( TInt aKey );
     
     /**
      * Revert the phrase to keystroke.
      * 
      * @since S60 v3.2.3.
      * @return None.
      */
     void RevertPhraseToKeystrokeL();
     
     /**
      * Add phrase to DB.
      * 
      * @since S60 v3.2.3.
      * @param aPhraseAdd The phrase to be added
      * @return None.
      */
     void AddPhraseToDB( const TDesC& aPhraseAdd );
     
     /**
      * Move the cursor to Left.
      * 
      * @since S60 v3.2.3.
      * @return ETrue if cursor can be moved, other EFalse.
      */         
     TBool MoveCursorLeft();
     
     /**
      * Move the cursor to Right.
      * 
      * @since S60 v3.2.3.
      * @return ETrue if cursor can be moved, other EFalse.
      */         
     TBool MoveCursorRight();

     /**
      * Handle Key Backspace
      * 
      * @since S60 v3.2.3.
      * @return None.
      */  
     void HandleKeyBackspaceL( TKeyPressLength aLength );
     
     /**
      * Commit content
      * 
      * @since S60 v3.2.3.
      * param aAddUDB ETrue if the content need to be added to UDB, otherwise EFalse.
      * @return None.
      */ 
     void CommitContentL( TBool aAddUDB );
     
     /**
      * Handle commit action
      * 
      * @since S60 v3.2.3.
      * @return None.
      */
     void HandleCommitL();
     
     /**
      * Refresh UI if necessary
      * 
      * @since S60 v3.2.3.
      * @return None.
      */
     void RefreshUI();
     
     /**
      * Refresh UI if necessary
      * 
      * @since S60 v3.2.3.
      * @return None.
      */
     void ImplicityShowInfo( TBool aGetScreenCoordinates );

     /**
      * Refresh UI if necessary
      * 
      * @since S60 v3.2.3.
      * @return None.
      */
     void ImplicityShowInfoL( TBool aGetScreenCoordinates );
     
     /**
      * Show EEP Info
      * 
      * @since S60 v3.2.3.
      * @return None.
      */
     void ShowEEPInfo();
     
     /**
       * Submit text
       * @param aText text
       */
     void SubmitTextL( const TDesC& aText );
     
protected:
     void CommitCandidateL();
     
     TInt GetIndexOfToneMark( const TDesC& aText);

    };

/**
 *  TAknFepInputStateSpellingMiniQwertyZhuyin class.
 *
 *  Implementation of Zhuyin phrase spelling selection state
 *
 *  @lib avkonfep.dll
 *  @since S60 v3.2
 */
class TAknFepInputStateSpellingMiniQwertyZhuyin: public TAknFepInputMiniQwertyZhuyinPhraseBase
    {
public:

    /**
     * C++ default constructor
     *
     * @since S60 V3.2
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepInputStateSpellingMiniQwertyZhuyin(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer );
    
    /**
     * Handling Command events.
     * This Method is called by CAknFepUIManagerJapanese.
     *
     * @param aCommandId Command ID value
     */
    void HandleCommandL( TInt aCommandId );
    
    /**
      * Submit text
      * @param aText text
      */
    void SubmitTextL( const TDesC& aText );
    
private:
    
    void CommitCandidateL();
    
    TInt GetIndexOfToneMark( const TDesC& aText);
    /**
     * From TAknFepInputStateCandidateBasePhrase
     * Handle system key press
     *
     * @since S60 v3.2
     * @param aKey System key event
     * @param aLength Key press length
     * @return ETrue if handle, otherwise this class do not handle this key
     */
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);

    /**
     * Refresh UI if necessary
     * 
     * @since S60 v3.2.3.
     * @return None.
     */
    void RefreshUI();
    
    /**
     * Refresh vaild key stroke length
     * 
     * @since S60 v3.2.3.
     * @return None.
     */
    void RefreshVaildKeyStroke();
    };

class TAknFepInputStateCanindateSelectingMiniQwertyZhuyin : public TAknFepInputMiniQwertyZhuyinPhraseBase
    {
public:
    /**
     * C++ default constructor
     *
     * @since S60 v3.2.3
     * @param aOwner Pointer to UI manager state interface
     * @param aUIContainer Pointer to Chinese UI container
     */
    TAknFepInputStateCanindateSelectingMiniQwertyZhuyin(
        MAknFepUIManagerStateInterface* aOwner,
        MAknFepUICtrlContainerChinese* aUIContainer
        );
   /**
    * From TAknFepInputStateMiniQwertyZhuyinPhraseCreationBase
    * Handle system key press
    *
    * @since S60 v3.2
    * @param aKey System key event
    * @param aLength Key press length
    * @return ETrue if handle, otherwise this class do not handle this key
    */
   TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
   
   /**
    * Handling Command events.
    * This Method is called by CAknFepUIManagerJapanese.
    *
    * @param aCommandId Command ID value
    */
   void HandleCommandL( TInt aCommandId );
   
   /**
     * commit the selected character into the editor pane 
     * @since S60 v3.2
     * @return none
     */
   void CommitCandidateL();
   
   /**
     * Check the select candidate numeric key is valid or not
     * @since S60 v3.2
     * @return ETure if the select number is valid ,others is EFalse
     */
   TBool SelectCandidate( TInt aKey );
   
   /**
     * the postion of the first tonemark appeared 
     * @since S60 v3.2
     */
    TInt GetIndexOfToneMark( const TDesC& aText);
    
    /**
      * Submit text
      * @param aText text
      */
    void SubmitTextL( const TDesC& aText );
    };

#endif /*AKNFEPUIINPUTSTATEMINIQWERTYZHUYINPHRASE_H_*/
