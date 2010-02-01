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
* Description:   Predective text input engine API header
*
*/



#ifndef C_TRUIPTIENGINE_H
#define C_TRUIPTIENGINE_H

// system includes
#include <e32base.h>
#include <badesca.h>
#include <f32file.h>

// user includes
#include "PtiHwrRecognizer.h"

// FORWARD DECLARATIONS
class CSymbolList;
class THwrUdmRange;
class RFs;


class CTruiPtiEngine : public CBase, public MTruiEngine
    {

public:
   /**
    * Two phase constructor.
    *
    * @param  aHwrEngine
    * @return 
    */
    static CTruiPtiEngine* NewL( CHwrRecognizer* aHwrEngine );

   /**
    * Two phase constructor.
    *
    * @param  aHwrEngine
    * @return 
    */
    static CTruiPtiEngine* NewLC( CHwrRecognizer* aHwrEngine );

   /**
    * Destructor.
    */
    virtual ~CTruiPtiEngine();

// from base class MTruiEngine

    /**
     * Set current trained character's Language Script
     *
     * @param aLanguageScript current trained character's Language Script
     */
    void SetLanguageScriptL( TInt aLanguageScript );

    /**
     * Check if character has model
     *
     * @param aChar The character which is checked
     * @return TBool True means this character has model
     *               False means this character doesn't has model yet
     */
    TBool CharacterModelExist( const TDesC& aChar );
    
    /**
     * Get character model
     *
     * @param aChar Get this character's model
     * @param aModel Array of points where the character's model is stored.
     * @return TInt Error value. If no error occurred, KErrNone is returned
     */
    void GetCharacterModelL( const TDesC& aChar, RArray<TPoint>& aModel );
    
    /**
     * Set character model
     *
     * @param aChar Set model to this character
     * @param aModel Array of points where the character's model is stored.
     * @return TInt Error value. If no error occurred, KErrNone is returned
     */
    void SetCharacterModelL( const TDesC& aChar, RArray<TPoint>& aModel,  TInt aSubRange, TDes& aSimilarChar );
        
    /**
     * Delete character model
     *
     * @param aChar Delete this character's model
     * @return TInt Error value. If no error occurred, KErrNone is returned
     */
    TInt DeleteCharacterModel( const TDesC& aChar );   
    
    /**
     * Get text list of all the shortcuts
     *
     * @param aShortcutTextList The text list.
     * @return TInt Error value. If no error occurred, KErrNone is returned
     */
    void GetShortcutTextListL( RPointerArray<HBufC>& aShortcutTextList );
    
    /**
     * Check if shortcut has model
     *
     * @param aText The shortcut which is checked
     * @return TBool True means this shortcut has model
     *               False means this shortcut doesn't has model yet
     */
    TBool CheckShortcutModel( const TDesC& aText );
    
    /**
     * Get Shortcut model
     *
     * @param aText Get this Shortcut's model
     * @param aModel Array of points where the Shortcut's model is stored.
     * @return TInt Error value. If no error occurred, KErrNone is returned
     */
    void GetShortcutModelL( const TDesC& aText, RArray<TPoint>& aModel, TUint& aUnicode );
    
    /**
     * Set Shortcut model
     *
     * @param aText Set model to this Shortcut
     * @param aModel Array of points where the Shortcut's model is stored.
     * @return TInt Error value. If no error occurred, KErrNone is returned
     */
    void SetShortcutModelL( const TDesC& aText, RArray<TPoint>& aModel, TDes& aSimilarText  );

    /**
     * Delete Shortcut model
     *
     * @param aText Delete this Shortcut's model
     * @return TInt Error value. If no error occurred, KErrNone is returned
     */
    TInt DeleteShortcutModel( const TDesC& aText );
    
    /**
     * Change the text of an existing shortcut
     *
     * @param aOldText The text of an existing shortcut
     * @param aNewText The new text of the shortcut
     * @return TInt Error value. If no error occurred, KErrNone is returned
     */
    void ChangeShortcutTextL( const TDesC& aOldText, const TDesC& aNewText );    

    /**
     * Get guiding line pos
     * 
     *
     * @param aTop The vertical coordinate of top guiding line
     * @param aBottom The vertical coordinate of bottom guiding line
     * @return TInt Error value. If no error occurred, KErrNone is returned
     */
    TInt GetGuidingLinePos( const TSize& aSize, TInt& aTop , TInt&  aBottom); 

    /**
     * Get the number of the maximum of shortcuts
     *
     * @return TInt The number of the maximum of shortcuts
     */
    TInt MaxShortCutLength(); 
    
    /**
     * Save preset shortcuts.
     * 
     * @param aUnicode The assigned preset text.
     * @param aShortcut The shortcut to be save.
     */
    void SavePresetShortcutL( TUint aUnicode, const TDesC& aShortcut ) ;
    
    /**
     * Return all unicodes of preset text stored in PTI engine
     * 
     * @param aPresets All preset text's unicodes are stored in this array.     
     */
    void GetAllPresetSymbolsL( RArray<TUint>& aPresets ) ;  
    
    /**
     * Get trails assigned to a preset text.
     * 
     * @param aUnicode The unicode of the preset text.
     * @param aModel Store trails of the preset text.
     * @param aShortcut Indicate whether the preset text has been assigned to a shortcut text.
     *        If KNullDesC, it is not in use.
     */
    void GetPresetSymbolByUnicodeL( TUint aUnicode, RArray<TPoint>& aModel, TDes& aShortcut ) ;  
    
    //======== own public functions for derived hwr core only =================
    /**
     * Load the aType models to memory.
     *
     * @param aType Shall set the value of TUdmType
     * @return none
     */
     void LoadUdmL( TUdmType aType );
    
    /**
     * UnLoad the aType models to memory.
     *
     * @param aType Shall set the value of TUdmType
     * @return none
     */
     void UnLoadUdm(  TUdmType aType );
     
    /**
     * Get the indexes of the symbol modles spcified by aRange.
     *
     * @param aType Shall set the value of TUdmType
     * @param aList Store the indexed of the found symbol models.
     * @param aRange Udm Range
     * @return none
     */    
     void GetModelIndexListL( TUdmType aType, RArray<TInt>& aList, const THwrUdmRange& aRange );
     
    /**
     * Get the model properties of the specified index
     *
     * @param aType Shall set the value of TUdmType
     * @param aList Store the indexed of the found symbol models.
     * @param aRange Udm Range
     * @return none
     */ 
     void GetSymbolModelL( TUdmType aType, TInt aIndex, TPtrC& aSymbolName,RArray<TPoint>& aModel, TInt& aHelpLine, TInt& aBaseLine );
    
private:
    
    CTruiPtiEngine();
    
    void ConstructL( CHwrRecognizer* aHwrEngine );
    
    /**
     * Change the query strategy.
     *
     * @param aRange The range to change
     * @return none
     */ 
     void ConvertUdmRange( THwrUdmRange& aRange );
private: // data
    
    TFixedArray<CSymbolList*,EUdmMax>  iSymboList;
    
    TInt iScript;
    
    TInt iHl;
    
    TInt iBl;
    
    CHwrRecognizer* iHwrEngine;
    
    CSymbolList* iPreSetList;
    
    RFs iFsSession;
    };


#endif // C_TRUIPTIENGINE_H
