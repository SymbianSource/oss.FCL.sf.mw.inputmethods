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
*       Provides the CAknFepIndicator methods.
*
*/












#include <eikenv.h>         //CEikEnv
#include <eikspane.h>       //CEikStatusPane
#include <avkon.hrh>
#include <aknnavi.h>        //CAknNavigationControlContainer
#include <aknnavide.h>      //CAknNavigationDecorator
#include <AknIndicatorContainer.h>  //CAknIndicatorContainer
#include <aknEditStateIndicator.h>
#include <AknUtils.h>
#include "AknFepIndicatorAvkon.h"

#include "AknFepGlobalEnums.h"

/**
 *
 */ 
CAknFepIndicator* CAknFepIndicatorAvkon::NewL()
    {
    CAknFepIndicatorAvkon* self = new(ELeave)CAknFepIndicatorAvkon();
    return self;
    }

/**
 *
 */
CAknFepIndicatorAvkon::~CAknFepIndicatorAvkon()
    {
    RemoveContainer();
    }

 TBool CAknFepIndicatorAvkon::QueryPredictiveState(TAknEditingState aState)
 {
    return  (( aState == EIndicatorStatePinyin) ||
             ( aState == EIndicatorStatePinyinPhrase) ||
             ( aState == EIndicatorStateZhuyin) ||
             ( aState == EIndicatorStateZhuyinPhrase) ||
             ( aState == EIndicatorStateZhuyinFind) ||
             ( aState == EIndicatorStateStrokeFind) ||
             ( aState == EIndicatorStateStroke) || 
             ( aState == EIndicatorStateStrokePhrase) ||
			 ( aState == EIndicatorStateCangJie) ||
             ( aState == EIndicatorStateStrokeTrad) || 
             ( aState == EIndicatorStateStrokeTradPhrase) ||
             ( aState == ET9Upper) ||
             ( aState == ET9Shifted) ||
             ( aState == ET9Lower) ||
             ( aState == ET9Arabic) ||
             ( aState == ET9Hebrew) ||
             ( aState == ET9FarsiAndUrdu) ||
             ( aState == ET9Thai) ||
             ( aState == EIndicatorStatePredictiveHindi) ||
#ifdef RD_MARATHI
             ( aState == EIndicatorStatePredictiveMarathi) ||
#endif // RD_MARATHI
             ( aState == EIndicatorStateJapanesePredictive) ||
             ( aState == ET9UpperVietnamese) ||
             ( aState == ET9LowerVietnamese) ||
             ( aState == ET9ShiftedVietnamese) ||
             ( aState == ET9UpperGreek) ||
             ( aState == ET9LowerGreek) ||
             ( aState == ET9ShiftedGreek) ||
             ( aState == ET9UpperCyrillic) ||
             ( aState == ET9LowerCyrillic) ||
             ( aState == ET9ShiftedCyrillic) ); 
 }
 
  
#ifndef RD_INTELLIGENT_TEXT_INPUT
 TBool CAknFepIndicatorAvkon::QueryPredictiveAutoCompleteState(TAknEditingState aState)
 {
 	 	        
    return (( aState == EAutoCompleteUpper ) ||  
     		( aState == EAutoCompleteShifted )  ||
     		( aState == EAutoCompleteLower ) || 
     		( aState == EAutoCompleteHindi ) ||
     		( aState == EAutoCompleteArabic ) ||
     		( aState == EAutoCompleteHebrew ) ||
     		( aState == EAutoCompleteFarsiAndUrdu ) ||
     		( aState == EAutoCompleteThai ) ||
     		( aState == EAutoCompleteUpperVietnamese ) ||
     		( aState == EAutoCompleteLowerVietnamese ) ||
     		( aState == EAutoCompleteShiftedVietnamese ));
 }

#endif
TInt CAknFepIndicatorAvkon::EvaluateState(TAknEditingState aState)
{
	TInt result = 0;
	switch(aState)
	{
		case EArabicIndicNumeric:
  			 result = EAknNaviPaneEditorIndicatorArabicIndicNumberCase;
  			 break;
  			 
		case ET9Thai:
		case EMultitapThai:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteThai:
#endif
			result = EAknNaviPaneEditorIndicatorThai;
			break;				
			
		case EIndicatorStatePredictiveHindi:
		case EIndicatorStateMultitapHindi:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteHindi:
#endif
#ifdef RD_MARATHI
		case EIndicatorStateMultitapMarathi:
		case EIndicatorStatePredictiveMarathi:
#endif // RD_MARATHI
			result = EAknNaviPaneEditorIndicatorDevanagariCase;
			break;	
			
		case ET9Arabic:
		case EMultitapArabic:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteArabic:
#endif
			result = EAknNaviPaneEditorIndicatorArabicCase;
			break;	

		case ET9Hebrew:
		case EMultitapHebrew:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteHebrew:
#endif
			result = EAknNaviPaneEditorIndicatorHebrewCase;
			break;	
			
		case ET9FarsiAndUrdu:
		case EMultitapFarsiAndUrdu:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteFarsiAndUrdu:
#endif
			result = EAknNaviPaneEditorIndicatorFarsiAndUrduCase;
			break;					


		case ET9Upper:
		case EMultitapUpper:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteUpper:
#endif
				if(iCopyMode)
					{
					result = EAknNaviPaneEditorIndicatorUpperCaseSelect;		
					}
				else
					{
					result = EAknNaviPaneEditorIndicatorUpperCase;	
					}
			
			break;

		case ET9Lower:
		case EMultitapLower:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteLower:
#endif
				if(iCopyMode)
					{
					result = EAknNaviPaneEditorIndicatorLowerCaseSelect;		
					}
				else
					{
					result = EAknNaviPaneEditorIndicatorLowerCase;	
					}		
			break;

		case ET9Shifted:
		case EMultitapShifted:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteShifted:
#endif
				if(iCopyMode)
					{
					result = EAknNaviPaneEditorIndicatorTextCaseSelect;		
					}
				else
					{
					result = EAknNaviPaneEditorIndicatorTextCase;	
					}		
			break;
			
		case ENumeric:
				if(iCopyMode)
					{
					result = EAknNaviPaneEditorIndicatorNumberCaseSelect;		
					}
				else
					{
					result = EAknNaviPaneEditorIndicatorNumberCase;	
					}		
			break;		
  			 
  			 	
		case EIndicatorStatePinyin:
			result = EAknNaviPaneEditorIndicatorPinyin;
			break;
		case EIndicatorStatePinyinPhrase:
			result = EAknNaviPaneEditorIndicatorPinyinPhrase;
			break;
			
		case EIndicatorStateZhuyin:
			 result = EAknNaviPaneEditorIndicatorZhuyin;
			break;
		case EIndicatorStateZhuyinPhrase:
			 result = EAknNaviPaneEditorIndicatorZhuyinPhrase;
			break;

		case EIndicatorStateZhuyinFind:
			result = EAknNaviPaneEditorIndicatorZhuyinFind;
			break;

		case EDevanagariIndicNumeric :
			result = EAknNaviPaneEditorIndicatorDevanagariIndicNumberCase;
			break;
		
		case EIndicatorStateHiraganaKanji:
		case EIndicatorStateJapanesePredictive:
			result = EAknNaviPaneEditorIndicatorJapaneseHiraganaKanji;
			break;

		case EIndicatorStateFullKatakana:
			result = EAknNaviPaneEditorIndicatorJapaneseFullKatakana;
			break;

		case EIndicatorStateHalfKatakana:
			result = EAknNaviPaneEditorIndicatorJapaneseHalfKatakana;
			break;

		case EIndicatorStateFullLatinText:
			result = EAknNaviPaneEditorIndicatorJapaneseFullTextAlphabet;
			break;

		case EIndicatorStateFullLatinUpper:
			result = EAknNaviPaneEditorIndicatorJapaneseFullUpperAlphabet;
			break; 

		case EIndicatorStateFullLatinLower:
			result = EAknNaviPaneEditorIndicatorJapaneseFullLowerAlphabet;
			break;

		case EIndicatorStateFullNumeric:
			result = EAknNaviPaneEditorIndicatorJapaneseFullNumeric;
			break;

		case EIndicatorStateHiragana:
			result = EAknNaviPaneEditorIndicatorJapaneseHiragana;
			break;

		case EIndicatorStateStroke:
			result = EAknNaviPaneEditorIndicatorStroke;
			break;
		case EIndicatorStateStrokePhrase:
			result = EAknNaviPaneEditorIndicatorStrokePhrase;
			break;

		case EIndicatorStateStrokeTrad:
			result = EAknNaviPaneEditorIndicatorStrokeTrad;
			break;
		case EIndicatorStateStrokeTradPhrase:
			result = EAknNaviPaneEditorIndicatorStrokeTradPhrase;
			break;

		case EQwertyShift:
			result = EAknNaviPaneEditorIndicatorQwertyShift;
			break;

		case EIndicatorStateStrokeFind:
			result = EAknNaviPaneEditorIndicatorStrokeFind;
			break;

		case EIndicatorStateCangJie:
			result = EAknNaviPaneEditorIndicatorCangJie;
			break;
			
		case EIndicatorStateHangul:
			if(iCopyMode)
					{
					result = EAknNaviPaneEditorIndicatorKoreanHangulSelect;		
					}
				else
					{
					result = EAknNaviPaneEditorIndicatorKoreanHangul;	
					}
			break;
		
		case ET9UpperVietnamese:
		case EMultitapUpperVietnamese:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteUpperVietnamese:
#endif
			result = EAknNaviPaneEditorIndicatorVietnameseUpperCase;
			break;			
			
		case ET9LowerVietnamese:
		case EMultitapLowerVietnamese:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteLowerVietnamese:
#endif
			result = EAknNaviPaneEditorIndicatorVietnameseLowerCase;
			break;	
			
		case ET9ShiftedVietnamese:
		case EMultitapShiftedVietnamese:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteShiftedVietnamese:
#endif
			result = EAknNaviPaneEditorIndicatorVietnameseTextCase;
			break;
#ifdef RD_HINDI_PHONETIC_INPUT	
		case EIndicatorStateHindiPhoneticUpper:
			result = EAknNaviPaneEditorIndicatorDevanagariPhoneticUpperCase;
			break;
		case EIndicatorStateHindiPhoneticLower:
			result = EAknNaviPaneEditorIndicatorDevanagariPhoneticLowerCase;
			break;
#endif
#ifdef RD_INTELLIGENT_TEXT_INPUT
              case EFnKeyPressed:
			result = EAknNaviPaneEditorIndicatorFnKeyPressed;
			break;
              case EFnKeyLocked:
			result = EAknNaviPaneEditorIndicatorFnKeyLocked;
			break;
#endif
        case ET9UpperGreek:
        case EMultitapUpperGreek:
            result = EAknNaviPaneEditorIndicatorGreekUpperCase;
            break;
            
        case ET9LowerGreek:
        case EMultitapLowerGreek:
            result = EAknNaviPaneEditorIndicatorGreekLowerCase;
            break;
        
        case ET9ShiftedGreek:
        case EMultitapShiftedGreek:
            result = EAknNaviPaneEditorIndicatorGreekTextCase;
            break;
            
        case ET9UpperCyrillic:
        case EMultitapUpperCyrillic:
            result = EAknNaviPaneEditorIndicatorCyrillicUpperCase;
            break;
            
        case ET9LowerCyrillic:
        case EMultitapLowerCyrillic:
            result = EAknNaviPaneEditorIndicatorCyrillicLowerCase;
            break;
        
        case ET9ShiftedCyrillic:
        case EMultitapShiftedCyrillic:
            result = EAknNaviPaneEditorIndicatorCyrillicTextCase;
            break;
	}
	return result;
}
/**
 *
 */
void CAknFepIndicatorAvkon::SetState(TAknEditingState aState)
    {
    CAknNavigationControlContainer* naviPane = 0;
    TRAPD(err, naviPane = NaviPaneL());

    // If the navi pane has been changed, clean up and start again.
    if (iNaviPane != naviPane)
        {
        RemoveContainer();
        iNaviPane = naviPane;
        iNaviPaneChanged = ETrue;
        }
    
    if ( (iPreviousState == aState) && !iNaviPaneChanged  && !iCopyModeChanged)
        {
        // The required state already exists and navi pane has not been changed 
        // by launching embedded application. Leave the contents alone.
        return;
        }
    iCopyModeChanged = EFalse;    
        
    if (aState == EStateNone) 
        {
        if ( iNaviDecorator && naviPane )
            {
            CCoeControl* decoratedControl = iNaviDecorator->DecoratedControl();
            if ( decoratedControl )
                {
                decoratedControl->MakeVisible(EFalse);
                }
            naviPane->Pop(iNaviDecorator);
            }
        if(iPreviousState && (iPreviousState != EStateNone))
    	    {
	   		SetIndicator(EvaluateState(iPreviousState), EAknIndicatorStateOff, EFalse); 	   	
	   		// Also set off predictive / query (pen) indicator.
	   		SetIndicator(EAknNaviPaneEditorIndicatorQuery, EAknIndicatorStateOff, EFalse); 	   			   		
	   		SetIndicator(EAknNaviPaneEditorIndicatorT9, EAknIndicatorStateOff, EFalse); 	   			   		
        	}
        iPreviousState = aState;
        iNaviPaneChanged = EFalse;
        return;
        }
    
    if ( err == KErrNone )
        {
        iNaviPaneChanged = EFalse;
        TRAP(err,AddContainerL(naviPane));
        

        // selection indicator
        
        SetIndicator(EvaluateState(iPreviousState), EAknIndicatorStateOff, EFalse); 
        
        // selection indicator
        if (iCopyMode)
            {
            // clear normal indicators first
            SetIndicator(EAknNaviPaneEditorIndicatorTextCase, EAknIndicatorStateOff,  ETrue);
            SetIndicator(EAknNaviPaneEditorIndicatorLowerCase, EAknIndicatorStateOff, ETrue);
            SetIndicator(EAknNaviPaneEditorIndicatorUpperCase, EAknIndicatorStateOff, ETrue);
            SetIndicator(EAknNaviPaneEditorIndicatorNumberCase,EAknIndicatorStateOff, ETrue);       
            SetIndicator(EAknNaviPaneEditorIndicatorKoreanHangul,EAknIndicatorStateOff, ETrue); 
            }
	    else
            {
            // clear selection indicators first
            SetIndicator(EAknNaviPaneEditorIndicatorTextCaseSelect, EAknIndicatorStateOff,  ETrue);
            SetIndicator(EAknNaviPaneEditorIndicatorLowerCaseSelect, EAknIndicatorStateOff, ETrue);
            SetIndicator(EAknNaviPaneEditorIndicatorUpperCaseSelect, EAknIndicatorStateOff, ETrue);
            SetIndicator(EAknNaviPaneEditorIndicatorNumberCaseSelect,EAknIndicatorStateOff, ETrue);       
            SetIndicator(EAknNaviPaneEditorIndicatorKoreanHangulSelect,EAknIndicatorStateOff, ETrue);  
            }
             
  		TBool predictiveMode = QueryPredictiveState(aState);
#ifndef RD_INTELLIGENT_TEXT_INPUT
  		TBool predictiveAutoCompleteMode = EFalse;
  		if(!predictiveMode)
  			{
  			predictiveAutoCompleteMode = QueryPredictiveAutoCompleteState(aState);		
  			}
#endif
        
        SetIndicator(EAknNaviPaneEditorIndicatorQuery, 
            ( predictiveMode || aState == EStateNone  
#ifndef RD_INTELLIGENT_TEXT_INPUT
			|| predictiveAutoCompleteMode
#endif
            )? EAknIndicatorStateOff : EAknIndicatorStateOn, EFalse);
        SetIndicator(EAknNaviPaneEditorIndicatorT9, predictiveMode 
            ? EAknIndicatorStateOn : EAknIndicatorStateOff, EFalse);
#ifndef RD_INTELLIGENT_TEXT_INPUT
        // Indicator for Auto Word Completion Mode
     	SetIndicator(EAknNaviPaneEditorIndicatorAutoComplete, predictiveAutoCompleteMode 
            ? EAknIndicatorStateOn : EAknIndicatorStateOff, EFalse);
#endif
       
      SetIndicator(EvaluateState(aState), EAknIndicatorStateOn, EFalse);

        // Push the state to the navi-pane
        if (naviPane && iNaviDecorator)
            {
            CCoeControl* decoratedControl = iNaviDecorator->DecoratedControl();
            if ( decoratedControl )
                {
                decoratedControl->MakeVisible(ETrue);
                }
            TRAP_IGNORE(naviPane->PushL(*iNaviDecorator));
            }        
        }
    iPreviousState = aState;    
    }

/**
 *
 */
CAknIndicatorContainer* CAknFepIndicatorAvkon::IndicatorContainer()
    {
    CAknNavigationControlContainer* naviPane = 0;
    TRAPD(err, naviPane = NaviPaneL());

    // If the navi pane have changed, clean up and start again
    if ( naviPane != iNaviPane )
        {
        RemoveContainer();
        iNaviPane = naviPane;
        iNaviPaneChanged = ETrue;
        }
    if ( !iIndicatorContainer )
        {
        TRAP(err, AddContainerL(naviPane));        
        }
    return iIndicatorContainer;
    }

/**
 *
 */
CAknFepIndicatorAvkon::CAknFepIndicatorAvkon()
    :iPreviousState(EStateNone), iEikEnv(*CEikonEnv::Static())
    {
    }

/**
 *  Create a new indicator container and push it to the navi-pane
 */
void CAknFepIndicatorAvkon::AddContainerL(CAknNavigationControlContainer* aNaviPane)
    {
    // Create a new indicator container and push it to the navi-pane
    if (!iIndicatorContainer && aNaviPane)
        {
        iNaviDecorator = aNaviPane->CreateEditorIndicatorContainerL();
        iIndicatorContainer = STATIC_CAST(CAknIndicatorContainer*, iNaviDecorator->DecoratedControl());
        }
    }

/**
 * Pop the indicator container from the navi-pane
 */
void CAknFepIndicatorAvkon::RemoveContainer()
    {
    // Pop the indicator container from the navi-pane
    if (iIndicatorContainer)
        {
        delete iNaviDecorator;
        iNaviDecorator = NULL;
        iIndicatorContainer = NULL;
        }
    }

/**
 *
 */
void CAknFepIndicatorAvkon::SetIndicator(TInt aIndicator, TInt aMode, TBool aDrawNow)
    {
    if ( iIndicatorContainer )
        {
        iIndicatorContainer->SetIndicatorState(TUid::Uid(aIndicator), aMode, aDrawNow);
        }
    }

/**
 *  get a handle to the Navi Pane
 */
CAknNavigationControlContainer* CAknFepIndicatorAvkon::NaviPaneL()
    {
    CEikStatusPane *sp = iEikEnv.AppUiFactory()->StatusPane();
    User::LeaveIfNull(sp);
    // Fetch pointer to the default navi pane control
    return (CAknNavigationControlContainer *)sp->ControlL(TUid::Uid(EEikStatusPaneUidNavi));
    }

// End of file
