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
*
*/












#include "AknFepCaseManager.h"
#include "AknFepPanic.h"
#include "aknfeppeninputenums.h"

#include <AknFep.rsg>
#include <aknedsts.h>
#include <barsread.h>
#include <jplangutil.h>
#include <AknPictographInterface.h>
#include <AknPictographDrawerInterface.h>
#include <featmgr.h>
#include <PUAcodes.hrh>

const TText KSpaceCharacter=' ';
const TText KInvertedExclamationMarkCharacter = 0x00A1;
const TText KInvertedQuestionMarkCharacter = 0x00BF;

/* Teleca change begin, 18.08.2009 ssal */
const TInt KSBase = 0xac00; // base address for hangul syllables
const TInt KCBase = 0x3131; // base address for hangul compatibility jamo

const TInt KLCount = 19; // count of L-jamos
const TInt KVCount = 21; // count of V-jamos
const TInt KTCount = 28; // count of T-jamos
const TInt KNCount = KVCount * KTCount; // count of V&T-jamo combinations
const TInt KSCount = KLCount * KNCount; // total count of hangul syllables

// Conversion table for hangul compatibility jamo
const TUint16 conversionTableFromCompatibilityJamoToJamo[] = {
0x1100,
0x1101,
0x11AA,
0x1102,
0x11AC,
0x11AD,
0x1103,
0x1104,
0x1105,
0x11B0,
0x11B1,
0x11B2,
0x11B3,
0x11B4,
0x11B5,
0x11B6,
0x1106,
0x1107,
0x1108,
0x11B9,
0x1109,
0x110A,
0x110B,
0x110C,
0x110D,
0x110E,
0x110F,
0x1110,
0x1111,
0x1112,
0x1161,
0x1162,
0x1163,
0x1164,
0x1165,
0x1166,
0x1167,
0x1168,
0x1169,
0x116A,
0x116B,
0x116C,
0x116D,
0x116E,
0x116F,
0x1170,
0x1171,
0x1172,
0x1173,
0x1174,
0x1175,
};

#define KCCount (sizeof(conversionTableFromCompatibilityJamoToJamo) / sizeof(TUint16))
/* Teleca change end, 18.08.2009 ssal */

// 0x2029 is the paragraph seperator. When the user inputs a
// new line character, 0x2029 is inserted.
const TText KParagraphSeperator = 0x2029;
// 0x000A is line feed character 
const TText KLineFeed = 0x000A;

//for chinese unicode ox4E00 to 0x9FA5
const TText KChineseLowerUnicode = 0x4E00;
const TText KChineseUpperUnicode = 0x9FA5;
CAknFepCaseManager* CAknFepCaseManager::NewL(CAknFepManager& aFep)
    {
    CAknFepCaseManager* self = new (ELeave) CAknFepCaseManager(aFep);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }


CAknFepCaseManager::~CAknFepCaseManager()
    {
    delete iPictoInterface;
    }




void CAknFepCaseManager::ConfigureCaseStateFromEditorState()
    {
    CAknEdwinState* editorState = iFep.EditorState();
    if (!editorState)
        {
        return;
        }
        
 	iCurrentCase = editorState->CurrentCase();   
    iPreviousCase = editorState->CaseState().PreviousCase();    
        
    SetFixedCase(editorState->Flags() & EAknEditorFlagFixedCase);

    iPreviousCase = iCurrentCase;

  	if (CurrentEditorIsFixedCase())
    	{    
    	SetCurrentCase(editorState->DefaultCase()); 
    	iPreviousCase = iCurrentCase; 
    	__ASSERT_DEBUG(IsCasePermitted(iCurrentCase), 
                     AknFepPanic(EAknFepPanicNonPermittedEditorMode));		   	
    	} 
    	
    //determine it based on position 
    UpdateCase(ENullNaviEvent);

    // if still there is no current case set - use the default
    if (!iCurrentCase)
        {
        SetCurrentCase(editorState->DefaultCase());
        if (!IsCasePermitted(iCurrentCase))
            {
            SetCurrentCase(editorState->CurrentCase());
            }
        
        iPreviousCase = iCurrentCase;
        __ASSERT_DEBUG(IsCasePermitted(iCurrentCase), 
                       AknFepPanic(EAknFepPanicNonPermittedEditorMode));
        }

    // if the current is not allowed stay where we are
    else if (!IsCasePermitted(iCurrentCase))
        {
        if (!iPreviousCase)
            {
            SetCurrentCase(editorState->DefaultCase());
            iPreviousCase = iCurrentCase;
            __ASSERT_DEBUG(IsCasePermitted(iCurrentCase), 
                           AknFepPanic(EAknFepPanicNonPermittedEditorMode));
            }
        else
            {
            if (IsCasePermitted(iPreviousCase))
                {
                SetCurrentCase(iPreviousCase);
                }
            else
                {
                SetCurrentCase(editorState->DefaultCase());
                iPreviousCase = iCurrentCase;
                __ASSERT_DEBUG(IsCasePermitted(iCurrentCase), 
                               AknFepPanic(EAknFepPanicNonPermittedEditorMode));
                }    
            }
        }
    }


void CAknFepCaseManager::UpdateCase(TWesternNaviEvent aNaviEvent)
    {
    TBool applyCaseNeeded = ETrue;
    if ( iFep.IsFlagSet(CAknFepManager::EFlagSupressAutoUpdate) )
        {
        return;
        }
    CAknEdwinState* editorState = iFep.EditorState();
    if (!CurrentEditorIsFixedCase() && editorState 
      && editorState->PermittedCases() & EAknEditorTextCase)
        {
        // lets figure out where the cursor will be when the case needs to be checked
        TInt cursorPos = FindCursorPositionAtCaseCheck(aNaviEvent);
        
        if (!(cursorPos == 0 
           && iFep.IsFlagSet(CAknFepManager::EFlagSupressAutoUpdateAtEditorStart) ))
            {
            TInt defaultCase = editorState->DefaultCase();
            if ( cursorPos == 0 && 
               (defaultCase == EAknEditorLowerCase || defaultCase == EAknEditorUpperCase ))
                {
                SetCurrentCase(defaultCase);
                }
            // UPPER CASE : If in upper case & inside sentence, stay in upper.
            else if (!(iCurrentCase == EAknEditorUpperCase && 
                       CapsLockRequired(cursorPos, iCurrentCase) && 
                       aNaviEvent != EBackspaceEvent))
                {
                const TBool textCaseRequired = TextCaseRequired(cursorPos);
                // UPPER CASE : If upper case & inside sentence & backspace event, stay in upper.
                if (!(iCurrentCase == EAknEditorUpperCase && aNaviEvent == EBackspaceEvent &&
                    !textCaseRequired && cursorPos != 0))
                    {
                    // TEXT CASE : set if at start of editor or following a full stop / text case character
                    if (cursorPos == 0 || textCaseRequired)
                        {
                        TInt inputMode = iFep.InputMode();
                        if (iFep.IsChineseInputLanguage() && 
                        	inputMode != ELatin &&
                        	iCurrentCase != EAknEditorLowerCase)
                        {
                        SetCurrentCase(iCurrentCase);
                        }
#ifdef RD_HINDI_PHONETIC_INPUT	
                        
	                    else if(iFep.IsIndicPhoneticInputLanguage())
	                    	{
	                    	if(CurrentCase() != EAknEditorUpperCase)
	                    	    SetCurrentCase(EAknEditorLowerCase);	
	                    	}
#endif
                        else
                        {
                        /* Teleca change begin, 27.08.2008 ssal 
                        	original code:
                        SetCurrentCase(EAknEditorTextCase);	*/
                        
                        if(iFep.IsKoreanInputLanguage() && inputMode == EHangul )
                        	{
                        	SetCurrentCase(EAknEditorLowerCase);	
                        	}
                        else
                        	{
                            SetCurrentCase(EAknEditorTextCase);
                            }
                        /* Teleca change end, 27.08.2008 ssal */
                        }
                        applyCaseNeeded = EFalse;
                        }
                    else // OTHERWISE LOWER
                        {
                        SetCurrentCase(EAknEditorLowerCase);
                        applyCaseNeeded = EFalse;
                        }
                    }
                }
            }
        else
            {
            iFep.ClearFlag(CAknFepManager::EFlagSupressAutoUpdateAtEditorStart);
            }
        }
    if (applyCaseNeeded)
        {
        ApplyCase();
        }
    }


void CAknFepCaseManager::ManualCaseUpdate()
    {
    if (!CurrentEditorIsFixedCase())
        {
        if (IsCasePermitted(iCurrentCase)) 
            {
            DoManualCaseUpdate();
            iFep.ClearFlag(CAknFepManager::EFlagSupressAutoUpdate);
            }
        }
    }


void CAknFepCaseManager::RevertCaseChange()
    {
    if (!CurrentEditorIsFixedCase())
        {
        if (IsCasePermitted(iPreviousCase)) 
            {
            SetCurrentCase(iPreviousCase); 
            }
        }
    }


void CAknFepCaseManager::SetCurrentCase(TInt aCase, TBool aOverrideNumericTest)
    {
    if ( ( iFep.InputMode() == ENumber || iFep.InputMode() == ENativeNumber ) && 
        !aOverrideNumericTest)
        {
        if ( iFep.EditorState() && 
            (iFep.EditorState()->NumericKeymap() == EAknEditorConverterNumberModeKeymap) )
            {
            // Converter number mode keymap for '*' key requires upper case 'E'.
            iCurrentCase = EAknEditorUpperCase;
            }
        else
            {
            // Number mode keymappings for '*' -key uses lower case letters (pw).
            iCurrentCase = EAknEditorLowerCase;
            }
        }
    else
        {
        if ( aCase == ECaseInverseText )
            {
            iCurrentCase = ECaseUpper;
            iInverseCase = ETrue;
            }
        else
            {
            iCurrentCase = aCase;
            iInverseCase = EFalse;
            }
        }
    ApplyCase();
    }


TInt CAknFepCaseManager::CurrentCase() const
    {
    return iCurrentCase;
    }

TBool CAknFepCaseManager::IsAbleChangeCase() const
    {
    TBool isable = ETrue;
    
    CAknEdwinState* editorState = iFep.EditorState();
    if (editorState)
    	{
	    const TInt permittedCases = editorState->PermittedCases();
	    
	    if (permittedCases == EAknEditorUpperCase ||
	        permittedCases == EAknEditorLowerCase )
	        {
	        isable = EFalse;
	        }
	    else if ((editorState->Flags() & EAknEditorFlagForceTransparentFepModes)
	          && iFep.IsChineseInputLanguage())
	        {
	        isable = EFalse;
	        }
	    else if ( editorState->Flags() & EAknEditorFlagFixedCase )
	        {
	        isable = EFalse;
	        }
	    else if (!iFep.InputLanguageSupportsCaseChanges())
	        {
	        isable = EFalse;
	        }
    	}
    else
    	{
    	isable = EFalse;
    	}
    return isable;
    }

CAknFepCaseManager::CAknFepCaseManager(CAknFepManager& aFep) :
    iFep(aFep)
    {
    }


void CAknFepCaseManager::ConstructL()
    {
    // Set text case character set from resource
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, R_AKNFEP_TEXT_CASE_CHARACTER_SET);
    iTextCaseCharacters=reader.ReadTPtrC();

    // Create PictographInterface
    CCoeControl* control = NULL;
    MAknPictographAnimatorCallBack* callback = NULL;
    iPictoInterface = CAknPictographInterface::NewL(*control,*callback);

    CleanupStack::PopAndDestroy(); // reader*/
    }


TInt CAknFepCaseManager::FindCursorPositionAtCaseCheck(TWesternNaviEvent aNaviEvent)
    {
    TCursorSelection curCursSel;
    iFep.FepAwareTextEditor()->GetCursorSelectionForFep(curCursSel);
    
    TInt docLength = iFep.FepAwareTextEditor()->DocumentLengthForFep();
    TInt returnedPosition = -1;

    TCursorSelection uncommittedText = iFep.UncommittedText();

    switch (aNaviEvent)
        {
        case ENullNaviEvent:
            if (iFep.IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
                {
                returnedPosition = uncommittedText.HigherPos();
                }
            else
                {
                // In case of non-predictive mode, following condition
                // check text is selceted mode or not?
                // In selected mode, character always being inserted
                // anchor position
                if( curCursSel.Length() >0 )
                    {
                    returnedPosition = curCursSel.LowerPos();
                    }
                else
                    {
                    returnedPosition = curCursSel.HigherPos();
                    }
                }
            break;
        case ERightNaviEvent:
            if (iFep.IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
                {
                returnedPosition = uncommittedText.HigherPos();
                }
            else
                {
                if (curCursSel.Length())
                    {
                    returnedPosition = curCursSel.HigherPos();
                    }
                else
                    {
                    if (curCursSel.iCursorPos == docLength)
                        {
                        returnedPosition = 0;
                        }
                    else
                        {
                        returnedPosition = curCursSel.HigherPos() + 1;
                        }
                    }
                }
            break;
        case ELeftNaviEvent:
            if (iFep.IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
                {
                returnedPosition = uncommittedText.LowerPos();
                }
            else
                {
                if (curCursSel.Length())
                    {
                    returnedPosition = curCursSel.LowerPos();
                    }
                else
                    {
                    if (curCursSel.iCursorPos == 0)
                        {
                        returnedPosition = docLength;
                        }
                    else
                        {
                        returnedPosition = curCursSel.LowerPos() - 1;
                        }
                    }
                }
            break;
        case EBackspaceEvent:
            if (iFep.IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
                {
                returnedPosition = uncommittedText.HigherPos();
                }
            else
                {
                if (curCursSel.Length())
                    {
                    returnedPosition = curCursSel.LowerPos();
                    }
                else
                    {
                    returnedPosition = curCursSel.LowerPos() - 1;
                    }
                }
            break;
        default:
            break;
        }

    if (returnedPosition < 0)
        {
        returnedPosition = 0;
        }
    else if (returnedPosition > docLength)
        {
        returnedPosition = docLength;
        }
    return returnedPosition;
    }


TBool CAknFepCaseManager::TextCaseRequired(TInt aCursorPos)
    {
    TBool textCaseRequired = EFalse;
    TInt charPosToCheck = aCursorPos - 1;   
    TChar ch = KSpaceCharacter;
    TBuf<CAknFepManager::ESingleCharacter> editorText;
    TInt nonEnterSpaceCharCount = 0;
    
    if (charPosToCheck >= 0) 
        {
        // Get the character just before the cursor.
        iFep.FepAwareTextEditor()->GetEditorContentForFep(editorText,
            charPosToCheck, CAknFepManager::ESingleCharacter);
        ch = editorText[0];        
        if ( ch == KInvertedQuestionMarkCharacter || ch == KInvertedExclamationMarkCharacter )
            {
            // Text case is required imediatelly after 'spanich' question and exclamation mark that begins a sentence.
            textCaseRequired = ETrue;        
            }
        // for chinese uncode limit
        if ( ch >= KChineseLowerUnicode && ch<= KChineseUpperUnicode)
        	{
        	textCaseRequired = ETrue; 
        	}
        
        if( ( TUint )ch == KParagraphSeperator || ( TUint )ch == KLineFeed )
            {
            // When we have enter key, we need to decide the case after: 
            // we reach some non space characters (OR)
            // we reach the beginning of the editor
            while (charPosToCheck > 0 && !textCaseRequired)
                {
                charPosToCheck--;
                iFep.FepAwareTextEditor()->GetEditorContentForFep(editorText,
                    charPosToCheck, CAknFepManager::ESingleCharacter);
                ch = editorText[0];
                
                // IsSpace returns true for ParagraphSeperator.
                if (!(ch.IsSpace() || ch == EKeyEnter) && nonEnterSpaceCharCount <= 1)
                    {
                    if (IsTextCaseCharacter(ch))
                        {
        	            textCaseRequired = ETrue;
                        }
                    break;
                    }
                else 
                	{
                	if(!( ( TUint )ch == KParagraphSeperator || ( TUint )ch == EKeyEnter || ( TUint )ch == KLineFeed ) )
                		{
                		// Incrementing the count if we have
                		// IsSpace() returns True AND
                		// the character is not a ParagraphSeparator or Enter or Line Feed
                		nonEnterSpaceCharCount++;
	                	}
                	if (charPosToCheck <= 0 && !nonEnterSpaceCharCount)
                    	{
                    	//Reached the beginning of the Editor. Text case should be ON
                    	textCaseRequired = ETrue;
                    	}                    	
                	}    	
                }
            charPosToCheck = aCursorPos - 1;
            
           	if (charPosToCheck <= 0 && !nonEnterSpaceCharCount)
           	    {
           	    // If Enter is pressed as the first character. Text case is ON
                textCaseRequired = ETrue;
                }                    	
            
        	}

        /* Teleca change begin, 27.08.2008 ssal */	
        if (IsKoreanCharacter(ch))
        	{
        	textCaseRequired = ETrue;       
        	}
        /* Teleca change end, 27.08.2008 ssal */
        }


    if (charPosToCheck > 0 && !textCaseRequired) 
        {
        if (iFep.IsFeatureSupportedJapanese())
            {
            // The character case is changed to text case when the left side
            // character is Japanese character.
            if (IsJapaneseCharacter(ch))
                {
                textCaseRequired = ETrue;
                }
            else if (ch.IsSpace() || ch == EKeyEnter ||
                     ch == (TUint)KPuaCodeSpaceSymbol)
                {
                while (charPosToCheck > 0)
                    {
                    charPosToCheck--;
                    // Get the character before the previous character.
                    iFep.FepAwareTextEditor()->GetEditorContentForFep(editorText,
                        charPosToCheck, CAknFepManager::ESingleCharacter);
                    ch = editorText[0];
                    if (!(ch.IsSpace() || ch == EKeyEnter || ch == (TUint)KPuaCodeSpaceSymbol))
                        {
                        if (IsTextCaseCharacter(ch))
                            {
                            // One space between the cursor and the punctuation character => text case needed
                            textCaseRequired = ETrue;
                            }
                        break;
                        }
                    else if (charPosToCheck <= 0)
                        {
                        // reached the start of the editor => text case needed
                        textCaseRequired = ETrue;
                        }
                    }
                }
            }
        else
            {
            if ( charPosToCheck > 0 && !textCaseRequired && !nonEnterSpaceCharCount) 
			    {        
			    if ( ch.IsSpace() || ch == EKeyEnter || 
			    	ch == ( TUint )KPuaCodeSpaceSymbol )
			        {
			        while ( charPosToCheck > 0 )
			            {
			            charPosToCheck--;
			            
			            // Get the character before the previous character.
			            iFep.FepAwareTextEditor()->GetEditorContentForFep(editorText,
			                charPosToCheck, CAknFepManager::ESingleCharacter);
			                
			            ch = editorText[0];
			            
			            // This condition is added to keep Text case ON when space is
			            // entered after a "New line" character.
			            if( ( TUint )ch == KParagraphSeperator )
			            	{			            	
			            	textCaseRequired = ETrue;
			            	break;
			            	}
			            	
			            if ( !( ch == EKeyEnter || 
			            		ch == ( TUint )KPuaCodeSpaceSymbol ) )
			                {
			                if ( IsTextCaseCharacter( ch ) )
			                    {			                    
			                    textCaseRequired = ETrue;
			                    }
			                break;
			                }
			            else if ( charPosToCheck <= 0 )
			                {
			                // reached the start of the editor => text case needed
			                textCaseRequired = ETrue;
			                }
			            }
                    }
                }
            }
        }
    else if (charPosToCheck < 0)
        {
        // reached the start of the editor => text case needed
        textCaseRequired = ETrue;
        }
    else    // if (charPosToCheck == 0)
        {
        if (!textCaseRequired && iFep.IsFeatureSupportedJapanese())
            {
            if (IsJapaneseCharacter(ch)
             || ch.IsSpace()
             || ch == EKeyEnter
             || ch == (TUint)KPuaCodeSpaceSymbol)
                {
                // reached the start of the editor => text case needed
                textCaseRequired = ETrue;
                }
            }
        }

    return textCaseRequired;
    }


TBool CAknFepCaseManager::CapsLockRequired(TInt aCursorPos, TInt aCurCase)
    {
    TBool capsLocked = iForceUpperCase;
    TInt charPosToCheck = aCursorPos - 1;   
    TChar ch = KSpaceCharacter;
    TBuf<CAknFepManager::ESingleCharacter> editorText;
    
    // is the previous character alphabetical?
    if (charPosToCheck >= 0) 
        {
        iFep.FepAwareTextEditor()->GetEditorContentForFep(editorText,
            charPosToCheck, CAknFepManager::ESingleCharacter);
        if( editorText.Length()>0 )
            {
            ch = editorText[0];
            }
            
        if (IsAlpha(ch) ||
            ((aCurCase == EAknEditorUpperCase || aCurCase == ECaseInverseText) && 
             !TextCaseRequired(aCursorPos)))
            {
            // Caps lock (upper case mode) is kept up if we are currently in upper case mode
            // and previous character is alpha character or for example full stop. 
            // If the cursor and full stop is separated with space, text case is restored. 
            capsLocked = ETrue;
            }
        }
    
    return capsLocked;
    }


void CAknFepCaseManager::DoManualCaseUpdate()
    {
    TBool applyCaseNeeded = ETrue;
    iPreviousCase = iCurrentCase;
    TCursorSelection currentSelection;
    iFep.FepAwareTextEditor()->GetCursorSelectionForFep(currentSelection);

    switch (iCurrentCase)
        {
        case EAknEditorLowerCase:
            if (CapsLockRequired(currentSelection.iCursorPos, iCurrentCase) 
             && IsCasePermitted(EAknEditorUpperCase))
                {
                SetCurrentCase(EAknEditorUpperCase);
                }
            else if (!IsCasePermitted(EAknEditorTextCase) 
#ifdef RD_HINDI_PHONETIC_INPUT	
            
            || iFep.IsIndicPhoneticInputLanguage()
#endif            
            )
                {
                if( IsCasePermitted(EAknEditorUpperCase) )
                    SetCurrentCase(EAknEditorUpperCase);
                }
            else
                {
                SetCurrentCase(EAknEditorTextCase);
                }
            applyCaseNeeded = EFalse;
            break;
        case EAknEditorTextCase:
             if (iForceUpperCase && IsCasePermitted(EAknEditorUpperCase))
                {
                SetCurrentCase(EAknEditorUpperCase);
                applyCaseNeeded = EFalse;     
                break;          
                }
        case EAknEditorUpperCase:
            SetCurrentCase(EAknEditorLowerCase);
            applyCaseNeeded = EFalse;
            break;
        default:
            break;
        }

    if (!(IsCasePermitted(iCurrentCase)))
        {
        DoManualCaseUpdate();
        }
    else
        {
        if (applyCaseNeeded)
            {
            ApplyCase();
            }
        }
    }


void CAknFepCaseManager::ApplyCase()
    {
    // update engine...
    switch (iCurrentCase)
        {
        case EAknEditorUpperCase: // fall through
        case EAknEditorTextCase:
            iFep.SetCase(EUpperCase);
            break;
        case EAknEditorLowerCase: // fall through
        default:
            iFep.SetCase(ELowerCase);
            break;
        }
    if(iFep.EditorState())
        {
        iFep.EditorState()->SetCurrentCase(iCurrentCase);
        }
    iFep.UpdateIndicators();

#ifdef RD_SCALABLE_UI_V2    
    iFep.UpdateTouchCaseMode();
#endif
    }


void CAknFepCaseManager::SetFixedCase(TBool aFixedCase)
    {
    iFixedCase = aFixedCase;
    }


TBool CAknFepCaseManager::CurrentEditorIsFixedCase() const
    {
    return iFixedCase;
    }


TBool CAknFepCaseManager::IsCasePermitted(TInt aCase) const
    {
    // if current is permitted or current is lower and text is permitted
    CAknEdwinState* editorState = iFep.EditorState();
    return ( editorState && ( !editorState->PermittedCases() || 
           (aCase & editorState->PermittedCases() || 
           (aCase == EAknEditorLowerCase && 
           (EAknEditorTextCase & editorState->PermittedCases())))) );
    }

TBool CAknFepCaseManager::IsTextCaseCharacter(TChar aChar) const
    {
    TBool ret = EFalse;
    if (iTextCaseCharacters.Locate(aChar)!=KErrNotFound)
        {
        ret = ETrue;
        }
    else if (JPLangUtil::IsKanji((TText)aChar)
          || JPLangUtil::IsHiragana((TText)aChar)
          || JPLangUtil::IsKatakana((TText) aChar))
        {
        ret = ETrue;
        }
    return ret;
    }

TBool CAknFepCaseManager::IsAlpha(TChar aChar) const
    {
    TBool ret = EFalse;
    TBool isPicto = EFalse;
    
    if (iPictoInterface && iPictoInterface->Interface())
        {
        isPicto = iPictoInterface->Interface()->IsPictograph((TText)aChar);
        }
    
    if (!( JPLangUtil::IsHiragana((TText)aChar)
        || JPLangUtil::IsKanji((TText)aChar)
        || JPLangUtil::IsKatakana((TText)aChar)
        /* Teleca change begin, 18.08.2009 ssal */
        || IsKoreanCharacter(aChar)
        /* Teleca change end, 18.08.2009 ssal */
        || isPicto ))
        {
        TChar ch = aChar;
        if (iFep.IsFeatureSupportedJapanese() && 
            JPLangUtil::IsFullWidth((TText) ch))
            {
            TBuf<CAknFepManager::ESingleCharacter> fullChar;
            TBuf<CAknFepManager::ESingleCharacter> halfChar;
            fullChar.Append(ch);
            if (JPLangUtil::ConvertFullToHalfWidth(fullChar, halfChar))
                {
                ch = halfChar[0];
                ret = ch.IsAlpha();
                }
            }
        else
            {
            ret = ch.IsAlpha();
            }
        }
    return ret;
    }

TInt CAknFepCaseManager::PluginPermittedCases()
    {
	TInt allowedCases = ECaseInvalide;    
    if ( !(iFep.EditorState()) )
        {
        return allowedCases;
        }
    
    TInt editorCases = iFep.EditorState()->PermittedCases();
    
    if( editorCases == 0 )
        {
        allowedCases = ECaseUpper | ECaseLower | ECaseText | ECaseInverseText;    
        }
    else
        {
        allowedCases = editorCases;
        
        if( (editorCases & EAknEditorUpperCase) && 
            (editorCases & EAknEditorTextCase) )
            {
            allowedCases |= ECaseInverseText;
            }
        }
    
    return allowedCases;
    }


// -----------------------------------------------------------------------------
// CAknFepCaseManager::CapsLockRequired
// 
// 
// -----------------------------------------------------------------------------
//
TBool CAknFepCaseManager::CapsLockRequired(TInt aCurrentCase)
    {
  	TCursorSelection currentSelection;
  	
  	if (iFep.FepAwareTextEditor())
  		{  		  	
    	iFep.FepAwareTextEditor()->GetCursorSelectionForFep(currentSelection);    
    	return CapsLockRequired(currentSelection.iCursorPos, aCurrentCase);
  		}
  		
	return EFalse;  		
    }

// -----------------------------------------------------------------------------
// CAknFepCaseManager::IsJapaneseCharacter
// Determines whether a character is one of Japanese characters
// -----------------------------------------------------------------------------
//
TBool CAknFepCaseManager::IsJapaneseCharacter(TChar aChar) const
    {
    TBool ret = EFalse;
    // The character case is changed to text case when the left side
    // character is Japanese character (Hiragana, Katakana, Kanji,
    // Full-width Symbol).
    if (JPLangUtil::IsKanji((TText)aChar)
     || JPLangUtil::IsHiragana((TText)aChar)
     || JPLangUtil::IsKatakana((TText)aChar))
        {
        // Hiragana, Katakana, Kanji
        ret = ETrue;
        }
    else if (JPLangUtil::IsFullWidth((TText)aChar)
     && aChar != KPuaCodeSpaceSymbol
     && aChar != KPuaCodeLineFeedSymbol)
        {
        if (iFep.InputMode() == ELatin
          && iFep.CharacterWidth() == EFullWidthChar)
            {
            TBuf<CAknFepManager::ESingleCharacter> fullChar;
            TBuf<CAknFepManager::ESingleCharacter> halfChar;
            fullChar.Append(aChar);
            if (JPLangUtil::ConvertFullToHalfWidth(fullChar, halfChar))
                {
                TChar ch2 = halfChar[0];
                if (!ch2.IsAlpha())
                    {
                    // Full width Symbol or Number
                    ret = ETrue;
                    }
                }
            else
                {
                // The character which isn't converted to half is
                // special Japanese Symbols.
                ret = ETrue;
                }
            }
        else
            {
            ret = ETrue;
            }
        }
    return ret;
    }

/* Teleca change begin, 18.08.2009 ssal */
// -----------------------------------------------------------------------------
// CAknFepCaseManager::IsKoreanCharacter
// Determines whether a character is one of Korean characters
// -----------------------------------------------------------------------------
//
TBool CAknFepCaseManager::IsKoreanCharacter(TChar aChar) const
    {  
    if ( IsHangulSyllable( aChar) || IsHangulCompatibilityJamo( aChar ) )
    	{
    	return ETrue;
    	}
    return EFalse;
    }

// ---------------------------------------------------------------------------
// Checks that is given character hangul syllable or not.
// ---------------------------------------------------------------------------
//
TBool CAknFepCaseManager::IsHangulSyllable( TChar aChar ) const
    {
    // Character is 'hangul syllable' 
    // if it's numeric value is between KSBase and KSBase + KSCount.
    TInt index = static_cast<TInt> ( aChar ) - KSBase;
    if ( index < 0 || index >= KSCount )
        {
        return EFalse;
        }
    return ETrue;
    }

// ---------------------------------------------------------------------------
// Checks that is given character hangul syllable or not.
// ---------------------------------------------------------------------------
//
TBool CAknFepCaseManager::IsHangulCompatibilityJamo( TChar aChar ) const
    {
    // Character is 'hangul compatibility jamo' 
    // if it's numeric value is between KCBase and KCBase + KCCount.
    TInt index = static_cast<TInt> ( aChar ) - KCBase;
    if ( index < 0 || index >= KCCount )
        {
        return EFalse;
        }
    return ETrue;
    }

/* Teleca change end, 18.08.2009 ssal */    
// End of file
