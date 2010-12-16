/*
* Copyright (c) 2003-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CAknFepUIManagerChinese definition.
*
*/












#ifndef __AKN_FEP_UI_MANAGER_CHINESE_H__
#define __AKN_FEP_UI_MANAGER_CHINESE_H__

// INCLUDES
#include <uikon.hrh>                        //TAknEditorNumericKeymap
#include "AknFepUIManagerStateInterface.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepUiInputStateChineseBase.h"
#include <PtiDefs.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
class CAknFepUIManagerBase;
class MAknFepUICtrlContainerChinese;

/**
 *  CAknFepUIManagerChinese class.
 *
 *  @lib AknFep.lib
 *  @since 2.6
 *
 */
class CAknFepUIManagerChinese : public CAknFepUIManagerBase
    {
private:
    /**
     * private constants
     */
    enum
        {
        EBaseLanguageNumericKeymapIndex = 0x00FF,
        EMaskShiftOffset                = 8 ,
        ESupportPinyinPhrase            = 0x0010,
        ESupportZhuyinPhrase            = 0x0020,
        ESupportStrokePhrase            = 0x0040,
        EQueryPtiEngineAlready          = 0x0080
        };

public: // Constructors and destructor
    /**
     * 
     * 
     * @since 2.6
     */
    static CAknFepUIManagerChinese* NewL(MAknFepManagerUIInterface* aFepMan, 
                                         CAknFepCaseManager* aCaseMan, TLanguage aLanguage);

    /**
     * 
     * 
     * @since 2.6
     */
    virtual ~CAknFepUIManagerChinese();

public: // Functions from base classes
    /**
     * From CAknFepUIManagerBase
     * 
     * @since 2.6
     */
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength,TEventCode aEventCode = EEventKey);

    /**
     * From CAknFepUIManagerBase
     * 
     * @since 2.6
     */
    void CloseUI();

    /**
     * From CAknFepUIManagerBase
     * 
     * @since 2.6
     */
    void GetFormatOfFepInlineText(TCharFormat& aFormat, TInt& aNumberOfCharactersWithSameFormat, 
                                  TInt aPositionOfCharacter) const;

    /**
     * From CAknFepUIManagerBase
     * 
     * @since 2.6
     */
    TInt SupportLanguage(TInt aMode) const;

    /**
     * From CAknFepUIManagerBase
     * 
     * @since 2.6
     */
    void SetInputLanguageL(TLanguage aLanguage);

    /**
     * From CAknFepUIManagerBase
     * 
     * @since 2.6
     */
    void SetEditorContext(TInt aContext);

    /**
     * From CAknFepUIManagerBase
     * 
     * @since 2.6
     */
    TUIState ChangeState(TUIState aState);

    /**
     * From CAknFepUIManagerBase
     * 
     * @since 3.0
     */
    void ChangeStatetItut(TUIState aState);
    
    /**
     * From CAknFepUIManagerBase
     * 
     * @since 3.0
     */
    void ChangeStateQwerty(TUIState aState);

    /**
     * From CAknFepUIManagerBase
     * 
     * @since 2.6
     */
    TBool IsValidChineseInputKey(TInt aKey) const;
    /**
     * From CAknFepUIManagerBase
     * 
     * @since 3.0
     */
    TBool IsValidChineseInputKeyQwerty(TInt aKey) const;

    /**
     * From CAknFepUIManagerBase
     * 
     * @since 3.0
     */
    TBool IsQwertyZhuyinToneMarkKey(TInt aKey) const;

    /**
     * From CAknFepUIManagerBase
     * 
     * @since 2.6
     */
    TPtr GetLatestDeliberateSelection() const;

    /**
     * From CAknFepUIManagerBase
     * 
     * @since 3.0
     */
    void UpdateCangJieState();
    
    /**
     * From CAknFepUIManagerBase
     * 
     * @since 3.0
     */ 
    TBool IsValidChineseSymbol(TInt aKey) const;

private: // Functions from base classes
    /**
     * From CAknFepUIManagerBase
     * 
     * @since 2.6
     */
    TAknFepInputStateBase* FepUIState();

    /**
    * Clears all flags down
    */
    inline void ResetFlags();

    /**
    *  Clears aFlag bitfield in iFlags
    */
    inline void ClearFlag(TInt aFlag);

    /**
    *  Sets aFlag bitfield in iFlags
    */
    inline void SetFlag(TInt aFlag);
    
    /**
    * Returns ETrue if the aFlag bitfield in iFlags is set, EFalse if it
    * is clear
    */    
    inline TBool IsFlagSet(TInt aFlag) const;
    /**
    * Query PtiEngine for phrase input
    */ 
    void QueryPtiEngine();
#ifdef RD_INTELLIGENT_TEXT_INPUT    
    /**
     * Decide if the key mapped stroke
     * 
     * since 3.2.3
     */   
    TBool IsKeyMappedNumber(TInt aKey) const;
    TBool IsKeyMappedStroke(TInt aKey) const;
#endif
public: // New Fanctions
    /**
     * 
     * 
     * @since 2.6
     */
    MAknFepUICtrlContainerChinese* UIContainer() const;

    /**
    * Check valid shift key press
    */ 
    TBool IsValidShiftKeyPress() const;
#ifdef RD_INTELLIGENT_TEXT_INPUT     
    TBool IsValidFnKeyPress() const;  

    TBool IsValidLongChrKeyPress() const;

#endif    
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT
protected: // from CAknFepUIManagerBase
    TBool RollbackPreviousCharL();
#endif // RD_INTELLIGENT_TEXT_INPUT	
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
private: // New Fanctions
    /**
     * 
     * 
     * @since 2.6
     */
    CAknFepUIManagerChinese(MAknFepManagerUIInterface* aFepMan, CAknFepCaseManager* aCaseMan);

    /**
     * 
     * 
     * @since 2.6
     */
    void ConstructL(TLanguage aLanguage);
    
   /**
	* Tell whether this key mapped valid zhuyin, pinyin, or cangjie symbol
	*/
    TBool IsValidChineseInputSymbol(TInt aKey, TPtiEngineInputMode aMode) const;
	
	/**
	* Tell whether this key mapped valid chinese tone mark
	*/
    TBool IsValidChineseToneMarkKey(TInt aKey, TPtiEngineInputMode aMode) const;


private: // Data
    TAknFepInputStateChineseBase iFepUiState;
    MAknFepUICtrlContainerChinese* iContainerPane;

    // note that this must be stored in the manager, as the input states cannot allocate any memory
    TBufC<MAknFepUICtrlInputPane::EMaxInputCharsPinyinPopupNotIncludingToneMark> iLatestDeliberateSelection;
	TUint iFlags;
    TInt iPrevMode;
    TBool iQwertyMode;
    };
    
inline void CAknFepUIManagerChinese::SetFlag(TInt aFlag)
    {
    iFlags |= aFlag;
    }

inline void CAknFepUIManagerChinese::ClearFlag(TInt aFlag)
    {
    iFlags &= ~aFlag;
    }

inline void CAknFepUIManagerChinese::ResetFlags()
    {
    iFlags = 0;
    }

inline TBool CAknFepUIManagerChinese::IsFlagSet(TInt aFlag) const
    {
    return iFlags & aFlag;
    }


#endif //__AKN_FEP_UI_MANAGER_CHINESE_H__

// End of file
