/*
* Copyright (c) 2003-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CAknFepUIManagerWestern methods.
*
*/












// INCLUDE FILES
#include <e32svr.h> // RDebug stuff...
#include <e32keys.h>
#include <AknsUtils.h>

#include "AknFepGlobalEnums.h"
#include "AknFepManagerUIInterface.h"
#include "AknFepManagerInterface.h"
#include "AknFepUIManagerStateInterface.h"
#include "AknFepUiManagerBase.h"
#include "AknFepUiManagerWestern.h"

// Western input
#include "AknFepUiInputStateInitialLatinMultitap.h"
#include "AknFepUiInputStateInitialNumber.h"
#include "AknFepUiInputStateInitialWesternPredictive.h"
#include "AknFepUiInputStateEntryWesternPredictive.h"
#include "AknFepUiInputStateInitialHalfQwertyWesternPredictive.h"
#include "AknFepUiInputStateEntryHalfQwertyWesternPredictive.h"
#ifdef RD_INTELLIGENT_TEXT_INPUT
// Predictive QWERTY changes ---->
#include "AknFepUiInputStateInitialQwertyWesternPredictive.h"
#include "AknFepUiInputStateEntryQwertyWesternPredictive.h"
// Predictive QWERTY changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT
#include "AknFepPluginManager.h"

// Arabic & Hebrew input
#include "AknFepUiInputStateInitialArabicHebrewMultitap.h"
#include "AknFepUiInputStateInitialArabicHebrewPredictive.h"
#include "AknFepUiInputStateEntryArabicHebrewPredictive.h"

// Thai input
#include "AknFepUIInputStateInitialThaiMultitap.h"
#include "AknFepUiInputStateInitialThaiPredictive.h"
#include "AknFepUiInputStateEntryThaiPredictive.h"

#include "AknFepThaiSCTSelector.h"

//Hindi Input
#include "AknFepUIInputStateInitialIndicMultitap.h"
#include "AknFepUiInputStateInitialIndicPredictive.h"
#include "AknFepUiInputStateEntryIndicPredictive.h"
#include "AknFepUiIndicInputManager.h"

// Qwerty input
#include "AknFepUiInputStateQwerty.h"
#ifdef RD_INTELLIGENT_TEXT_INPUT
#include "AknFepUiInputStateHalfQwerty.h"
#endif

#ifdef RD_HINDI_PHONETIC_INPUT	
#include "aknfepuiinputstateinitialindicphoneticmultitap.h"
#include "aknfepuiinputstateindicphoneticqwerty.h"
#endif
// Vietnamese input
#include "AknFepUIInputStateInitialVietnameseMultitap.h"
#include "AknFepVietnameseToneManager.h"
#include "AknFepUiInputStateEntryVietnamesePredictive.h"
#include "AknFepCaseManager.h"
#include "AknFepPanic.h"

// Japanese input
#include "AknFepUiInputStateEntryWesternPredictiveJp.h"

#include <AknIndicatorContainer.h>  // CAknIndicatorContainer::TIndicatorContext
#include <PtiEngine.h> 
#include <PtiDefs.h>
#include <PtiUserDicEntry.h>
#include <AknFep.rsg>
// CONSTANTS

/**
 *  CAknFepUIManagerWestern class.
 * 
 */

//============================ MEMBER FUNCTIONS ==============================


// ---------------------------------------------------------------------------
// CAknFepUIManagerWestern::NewL
// 
// 
// ---------------------------------------------------------------------------
//
CAknFepUIManagerWestern* CAknFepUIManagerWestern::NewL(MAknFepManagerUIInterface* aFepMan, 
                                                       CAknFepCaseManager* aCaseMan, 
                                                       TLanguage aLanguage)
    {   
    CAknFepUIManagerWestern* self = new(ELeave)CAknFepUIManagerWestern(aFepMan, aCaseMan);
    CleanupStack::PushL(self);
    self->ConstructL(aLanguage);
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerWestern::~CAknFepUIManagerWestern
// 
// 
// ---------------------------------------------------------------------------
//
CAknFepUIManagerWestern::~CAknFepUIManagerWestern()
    {
    delete iAknFepThaiSCTSelector;
    delete iVietToneMarkMgr;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerWestern::HandleKeyL
// 
// 
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerWestern::HandleKeyL( TInt aKey, TKeyPressLength aLength, 
		                                   TEventCode /*aEventCode*/ )
    {
#ifdef RD_INTELLIGENT_TEXT_INPUT
    // The implementation is anyway identical with the base class, so we can
    // as well call the base implementation.
    return CAknFepUIManagerBase::HandleKeyL(aKey, aLength);
#else    
    TUIState currentState = iFepUiState.State();
    if (aLength == EShortKeyPress)
        {
        iInitialFepUIState = currentState;
        }

    if(!iStatePtr->HandleKeyL(aKey, aLength))
        {
        if(currentState != iStatePtr->State()) //Check to see if we've changed state.
            {
            return iStatePtr->HandleKeyL(aKey, aLength);
            }
        return EFalse; // we've failed to handle it..
        }
    return ETrue; //we're ok.
#endif // RD_INTELLIGENT_TEXT_INPUT  
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerWestern::CloseUI
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerWestern::CloseUI()
    {
    ChangeState(EInitial);
    }


// ---------------------------------------------------------------------------
// CAknFepUIManagerWestern::AddTextToUserDictionaryL
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerWestern::AddTextToUserDictionaryL(const TDesC& aText)
    {
    HBufC* udbTextBuf = HBufC::NewLC(CAknFepManager::EMaximumFepWordLength); // buffer for text
    TPtr udbText=udbTextBuf->Des();
    TChar ch;

    TInt textLengthToAdd = Min((TInt)CAknFepManager::EMaximumFepWordLength, aText.Length());

    for (TInt ii=0; ii < textLengthToAdd; ii++)
        {
        ch = STATIC_CAST(TChar, aText[ii]);
        udbText.Append(ch);
        if (ch.IsSpace() || ii == textLengthToAdd-1)
            {
            if (ch.IsSpace())
                {
                udbText.Delete(udbText.Length()-1, CAknFepManager::ESingleCharacter);
                }
            if (udbText.Length())
                {
                // add the contents of udbText to the user dictionary.
                CPtiEngine* ptiengine = PtiEngine();
                TPtiUserDictionaryEntry newUdbEntry(udbText);
                TPtiEngineInputMode currentInputMode = ptiengine->InputMode();
                // PtiEngine needs to be in predictive mode when the word is added to UDB.
#ifdef RD_INTELLIGENT_TEXT_INPUT
                if(iQwertyInputMode)
	                {
#ifdef __HALF_QWERTY_KEYPAD	              
                	if( EPtiKeyboardHalfQwerty == iFepMan->KeyboardLayout())
                		{
                		ptiengine->SetInputMode( EPtiEngineHalfQwertyPredictive );
                		}
                	else	
#endif //__HALF_QWERTY_KEYPAD                	
						{
						ptiengine->SetInputMode(EPtiEngineQwertyPredictive );	
						}
	                } 	                
                else
#endif //RD_INTELLIGENT_TEXT_INPUT
	                {
					ptiengine->SetInputMode(EPtiEnginePredictive);	                	
	                }
                ptiengine->AddUserDictionaryEntry(newUdbEntry);
                ptiengine->SetInputMode(currentInputMode);
                }
            udbText.Zero();
            }
        }
    CleanupStack::PopAndDestroy();  // udbTextBuf
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerWestern::GetFormatOfFepInlineText
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerWestern::GetFormatOfFepInlineText(TCharFormat& aFormat, 
                                                       TInt& aNumberOfCharactersWithSameFormat, 
                                                       TInt aPositionOfCharacter) const
    {
    if ( (iPredictive && iFepUiState.State() == EEntry ) 
#ifdef RD_HINDI_PHONETIC_INPUT    
//adding condition to underline uncommitted text for hindi phonetic
    	  || TAknFepUiIndicInputManager :: IsIndicPhoneticLanguage( ( TLanguage )iLanguage )
#endif
       )   // EInitial???
        {
        aFormat.iFontPresentation.iUnderline = EUnderlineOn;
        if (iFepMan->IsFlagSet(CAknFepManager::EFlagNoMatches))
            {
            aFormat.iFontPresentation.iHighlightStyle = 
                                             TFontPresentation::EFontHighlightNoMatchesIndicator;
            }
        TCursorSelection uncommit = iFepMan->UncommittedText();
        TInt lengthOfRemainderOfInlineText = uncommit.Length()-aPositionOfCharacter;
#ifndef RD_INTELLIGENT_TEXT_INPUT
        if(iFepMan->IsAutoCompleteOn())
        	{
        	// For AutoWord Completion - the suggested completion has to be gray slated
        	CPtiEngine* ptiengine = PtiEngine();
        	TInt actualLength = (ptiengine->CurrentInputSequence()).Length();
        
	        if(lengthOfRemainderOfInlineText>0)
		        {   
      		    if(actualLength < lengthOfRemainderOfInlineText)
			        {
			        if(iLanguage == ELangHindi)
			        	{
			        	TBuf<CAknFepManager::EMaximumFepWordLength> currentWord(ptiengine->CurrentWord());	//
			        // AutoComplete - Fix - Begin
			        	if(actualLength < currentWord.Length())
			        		{
			        		TUint currentchar = TUint(currentWord[actualLength]);
			    			while((TAknFepUiIndicInputManager::IsIndicDependantCharacter(currentchar, iLanguage)))
				    			{
				    			actualLength++;
				    			if(!(actualLength < currentWord.Length()))
				    			break;
				    			currentchar = TUint(currentWord[actualLength]);
				    			}
	        		// AutoComplete - Fix - Begin			        	
				    		}
			        	}
			        }
        
		        if (aPositionOfCharacter<actualLength && (lengthOfRemainderOfInlineText>0))
			        {
			        aFormat.iFontPresentation.iUnderline = EUnderlineOn;
			        aNumberOfCharactersWithSameFormat = actualLength;
			        }
				else
			        {	
			        // Display the autocompleted text in gray colour
			        TRgb autoTextColor; 
			        MAknsSkinInstance *skin = AknsUtils::SkinInstance();
			        TInt err = AknsUtils::GetCachedColor( skin, autoTextColor, KAknsIIDQsnTextColors, 
                                                    EAknsCIQsnTextColorsCG64 );
			        if (!err)
			            {
			            aFormat.iFontPresentation.iTextColor = autoTextColor;
			            }
			        else
			            {
			            // In case of error occurs, display text in hardcoded slate gray color
			            aFormat.iFontPresentation.iTextColor = KSLATEGRAY; 			            
			            }
			            			        
			        aNumberOfCharactersWithSameFormat=lengthOfRemainderOfInlineText;
			       	}
	            }
	        }
        else
#endif
#ifdef RD_INTELLIGENT_TEXT_INPUT
// Predictive QWERTY (XT9) changes ---->
        TInt wordLength = uncommit.Length();
        if ( lengthOfRemainderOfInlineText > 0 )
           {
            TInt tailLength = 0;
            //TInt err = PtiEngine()->HandleCommandL( EPtiCommandGetAutoCompletionTailLength, &tailLength );
            TRAP_IGNORE( PtiEngine()->HandleCommandL( 
            									EPtiCommandGetAutoCompletionTailLength,
            									&tailLength ));

            if ( aPositionOfCharacter < wordLength-tailLength )
    	        {
    			aFormat.iFontPresentation.iUnderline = EUnderlineOn;
    			aNumberOfCharactersWithSameFormat = lengthOfRemainderOfInlineText-tailLength;
                }
            else
                {
                aFormat.iFontPresentation.iUnderline = EUnderlineOff;
                aFormat.iFontPresentation.iTextColor = KSLATEGRAY;
	            aNumberOfCharactersWithSameFormat=lengthOfRemainderOfInlineText;
	            }
            }
#else
#ifdef RD_HINDI_PHONETIC_INPUT
			//for phonetic, space character needs to be checked explicitly so that it is not underlined
			//for predictive when space is entered it gets committed and changes to initial state, hence check is not present
             if(TAknFepUiIndicInputManager :: IsIndicPhoneticLanguage( ( TLanguage )iLanguage )	)
             	{
             		TChar tempChar(0);
             		iFepUiState.GetPhoneticLatinChar(tempChar);
             		if(tempChar.IsSpace())
             		    aFormat.iFontPresentation.iUnderline = EUnderlineOff;
	            }	
	#else
        	if (lengthOfRemainderOfInlineText>0)                   
	            {
	            aFormat.iFontPresentation.iUnderline = EUnderlineOn;
	            aNumberOfCharactersWithSameFormat=lengthOfRemainderOfInlineText;
		            }	
	#endif	            
// Predictive QWERTY (XT9) changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT
            }
        }


// ---------------------------------------------------------------------------
// CAknFepUIManagerWestern::SupportLanguage
// 
// 
// ---------------------------------------------------------------------------
//
TInt CAknFepUIManagerWestern::SupportLanguage(TInt aMode) const
    {
    TInt lang = iLanguage;
    switch (aMode)
        {
        case ELatin:
            {
            if ( iLanguage == ELangTaiwanChinese ||
                 iLanguage == ELangHongKongChinese ||
                 iLanguage == ELangPrcChinese ||
                 iLanguage == ELangJapanese || 
                 iLanguage == ELangHindi )
                {
                // Chinese and Japanese languages uses always English as a latin language.
                lang = ELangEnglish;
                }
            else
                {
                lang = iLanguage;
                }
            }
            break;
        case ENumber:
            {
            if ( iLanguage == ELangJapanese )
                {
                // Japanese languages uses always English as a number language for qwerty.
                lang = ELangEnglish;
                }
            else
                {
                lang = iLanguage;
                }
            }
            break;
        case EHindi:
        case ENativeNumber:
            lang = iLanguage;
            break;
        
            
        default:
            break;
        }
    return lang;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerWestern::ActivateUI
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerWestern::ActivateUI()
    {
    ChangeState(EEntry);
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerWestern::ChangeState
// 
// 
// ---------------------------------------------------------------------------
// MAknFepUIManagerStateInterface
TUIState CAknFepUIManagerWestern::ChangeState(TUIState aState)
    {
#ifdef RD_SCALABLE_UI_V2    
    if (iFepMan->PluginUIManager() &&
        iFepMan->PluginUIManager()->IsMatchState() && 
        !iFepMan->PluginUIManager()->IsSpellVisible())
        {
        //TRAP_IGNORE(iFepMan->UpdateCbaL(R_AKNFEP_SOFTKEYS_FINGER));
        iFepMan->PluginUIManager()->SetFingerMatchSelectionState(EFalse);
        }
        
    // For addition of ITI features on FSQ
    // When candidate list on FSQ is opened, some HW keys may cause state change,
    // e.g. left/right keys, need to hide candidate list
    if ( iFepMan->PluginUIManager() 
         && iFepMan->PluginUIManager()->EnableITIOnFSQ() )
        {
        TRAP_IGNORE( iFepMan->PluginUIManager()->HideCandidateListL() );
        }
    
#endif //RD_SCALABLE_UI_V2
    switch(aState)
        {
        case EInitial:
            {
            switch(iMode)
                {
                case ELatin:
                    {
                    if (iPredictive)
                        {
#ifdef RD_INTELLIGENT_TEXT_INPUT
                        // Predictive QWERTY (XT9) changes ---->
                        if (iQwertyInputMode)
                            {
#ifdef __HALF_QWERTY_KEYPAD
                            if( EPtiKeyboardHalfQwerty == iFepMan->KeyboardLayout())
                                {
                                iFepUiState = TAknFepUiInputStateInitialHalfQwertyWesternPredictive( this );
                                }
                            else
#endif // Half Qwerty
                            iFepUiState = TAknFepInputStateInitialQwertyWesternPredictive(this);
                            } // Predictive QWERTY (XT9) changes <----
                        else 
#endif // RD_INTELLIGENT_TEXT_INPUT
                        if (  iLanguage == ELangArabic || iLanguage == ELangHebrew 
                           || iLanguage == ELangFarsi  || iLanguage == ELangUrdu )
                            {
                            iFepUiState = TAknFepInputStateInitialArabicHebrewPredictive(this);
                            }
                        else if (iLanguage == ELangThai)
                            {
                            iFepUiState = TAknFepInputStateInitialThaiPredictive(this);
                            }
                        // Indic Script
                        else
                            {
                            iFepUiState = TAknFepInputStateInitialWesternPredictive(this);
                            }
                        }
                    else
                        {
                        if (  iLanguage == ELangArabic || iLanguage == ELangHebrew 
                           || iLanguage == ELangFarsi  || iLanguage == ELangUrdu )
                            {
                            iFepUiState = TAknFepInputStateInitialArabicHebrewMultitap(this);
                            }
                        else if (iLanguage == ELangThai)
                            {
                            iFepUiState = TAknFepInputStateInitialThaiMultitap(this);
                            }
                        /* Indic Script */

#ifdef RD_HINDI_PHONETIC_INPUT	
        
                        else if(TAknFepUiIndicInputManager :: IsIndicPhoneticLanguage( 
                                                            ( TLanguage )iLanguage ) )
                        {
                        iFepUiState = TAknFepInputStateInitialIndicPhoneticMultitap(
                                                            this, KLangHindiPhonetic);	
                        }
#endif    
                        else if(iLanguage == ELangVietnamese)
                            {
                            iFepUiState = TAknFepInputStateInitialVietnameseMultitap(this);
                            }
                        else
                            {
                            iFepUiState = TAknFepInputStateInitialLatinMultitap(this);
                            }
                        }
                    }
                    break;
                case EHindi:
                    iFepUiState = TAknFepInputStateInitialIndicMultitap(this, iLanguage);
                    break;
                case ENumber:
                case ENativeNumber:
                    {
                    // Map directly to the resources...
                    iFepUiState = TAknFepInputStateInitialNumber(this);
                    break;
                    }
                default:
                    AknFepPanic(EAknFepPanicBadInputState);
                }
            break;
            }
        case EEntry:
            {
            switch(iMode)
                {
                case ELatin:
#ifdef RD_INTELLIGENT_TEXT_INPUT
                        // Predictive QWERTY (XT9) changes ---->
                        if (iQwertyInputMode)
                            {
#ifdef __HALF_QWERTY_KEYPAD
                            if( EPtiKeyboardHalfQwerty == iFepMan->KeyboardLayout())
                                {
                                iFepUiState = TAknFepUiInputStateEntryHalfQwertyWesternPredictive( this );
                                }
                            else
#endif // Half Qwerty
	                        iFepUiState = TAknFepInputStateEntryQwertyWesternPredictive(this);
                            } // Predictive QWERTY (XT9) changes <----
                        else 
#endif // RD_INTELLIGENT_TEXT_INPUT
                    if (  iLanguage == ELangArabic || iLanguage == ELangHebrew 
                       || iLanguage == ELangFarsi  || iLanguage == ELangUrdu)
                        {
                        iFepUiState = TAknFepInputStateEntryArabicHebrewPredictive(this);
                        }
                    else if (iLanguage == ELangThai)
                        {
                        iFepUiState = TAknFepInputStateEntryThaiPredictive(this);
                        }
					/* Indic Script */
                    else if (iLanguage == ELangVietnamese)
                        {
                        iFepUiState = TAknFepInputStateEntryVietnamesePredictive(this);
                        }
                    else if (iLanguage == ELangJapanese)
                        {
                        iFepUiState = TAknFepInputStateEntryWesternPredictiveJp(this);
                        }
                    else
                        {
                        iFepUiState = TAknFepInputStateEntryWesternPredictive(this);
                        }
                    break;
                case EHindi:
                    iFepUiState = TAknFepInputStateEntryIndicPredictive(this, iLanguage);
                    break;
                default:
                    AknFepPanic(EAknFepPanicBadInputState);
                    break;
                }
            }
            break;
        case EQwerty:
#ifdef RD_HINDI_PHONETIC_INPUT	
        
        	if(iLanguage == KLangHindiPhonetic)
        		{
        		iFepUiState = TAknFepInputStateIndicPhoneticQwerty(this);	
        		}
        	else
        		{
#endif
            iFepUiState = TAknFepInputStateQwerty(this);
#ifdef RD_HINDI_PHONETIC_INPUT	
      
        		}
#endif
            break;
#ifdef RD_INTELLIGENT_TEXT_INPUT            
        case EHalfQwerty:
             iFepUiState = TAknFepInputStateHalfQwerty(this);        
             break;    
#endif             
        case ECandidate:
            AknFepPanic(EAknFepPanicBadInputState);
            break;
        case EPredictiveCandidate:
            {
            AknFepPanic(EAknFepPanicBadInputState);
            }
            break;
        case EPredictiveInput:
            AknFepPanic(EAknFepPanicBadInputState);
            break;
        default:
            AknFepPanic(EAknFepPanicBadInputState);
        }
    
    iPtiEngine->SetObserver(&iFepUiState);
#ifdef _DEBUG
    RDebug::Print(_L("UI Manager: New State: %d"), iFepUiState.State());
#endif
    return iStatePtr->State();
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerWestern::ThaiSCTResourceId
// 
// 
// ---------------------------------------------------------------------------
//
TInt CAknFepUIManagerWestern::ThaiSCTResourceId(TUint aPrewChar,TInt aKey)
    {
    TInt resourceId = 0;
    if (iAknFepThaiSCTSelector)
        {
        resourceId = iAknFepThaiSCTSelector->ThaiSCRResourceId(aPrewChar,aKey);
        }
    return resourceId;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerWestern::VietnameseToneManager
// 
// 
// ---------------------------------------------------------------------------
//
CAknFepVietnameseToneManager* CAknFepUIManagerWestern::VietnameseToneManager() const
    {
    return iVietToneMarkMgr;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerWestern::CAknFepUIManagerWestern
// 
// 
// ---------------------------------------------------------------------------
//
CAknFepUIManagerWestern::CAknFepUIManagerWestern(MAknFepManagerUIInterface* aFepMan, 
                                                 CAknFepCaseManager* aCaseMan)
    : CAknFepUIManagerBase(aFepMan, aCaseMan),
      iFepUiState(NULL)
    {
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerWestern::ConstructL
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerWestern::ConstructL(TLanguage aLanguage)
    {   
    CAknFepUIManagerBase::ConstructL(aLanguage);
    iAknFepThaiSCTSelector = CAknFepThaiSCTSelector::NewL();
    iVietToneMarkMgr = CAknFepVietnameseToneManager::NewL();
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerWestern::FepUIState
// 
// 
// ---------------------------------------------------------------------------
//
TAknFepInputStateBase* CAknFepUIManagerWestern::FepUIState()
    {
    return &iFepUiState;
    }

// End of file
