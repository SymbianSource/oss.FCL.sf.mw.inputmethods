/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  vkb conposition field implementation
*
*/


#include <peninputlayoutbasecontrol.h>

#include <AknsDrawUtils.h> 
#include <barsread.h>

#include "peninputvkbcompositionfield.h"

#include "peninputvkbevent.h"

#include "peninputvkb.hrh"

#include "peninputvkbdataprovider.h"

#include "AknFepVkbPinyinAnalyser.h"

#include "AknFepVkbPinyinAnalyserDb.h"
#include "peninputvkbdatamgr.h"

//Tone make
const TUint16 KCharToneMarkZhuyin1 = 0x0020;
const TUint16 KCharToneMarkPinyin1 = 0x02C9;
const TUint16 KCharToneMark2 = 0x02CA;
const TUint16 KCharToneMark3 = 0x02C7;
const TUint16 KCharToneMark4 = 0x02CB;
const TUint16 KCharToneMark5 = 0x02D9;

const TInt32 KInvalidResId = -1;

const TInt KInvalidImg = -1 ;

//Manual stroke separator char
const TUint16 KStrokeManuSeparator = 0x2022;
//Manul separator char       
const TUint KManulSeparator = 0x0027; // "'"
//Manul separator char       
const TUint KAutoSeparator = 0x002e;   //"."


_LIT(KWildChar, "\xFF1F");

_LIT(KInternalWildChar, "\x002B");

// ---------------------------------------------------------
// Helper function
// ---------------------------------------------------------
//
//Don't test aKey's length, treat as programming error!
inline TBool IsKeyToneMark(const TDesC& aKey)
    {
    return   (aKey[0] == KCharToneMarkZhuyin1 ||
              aKey[0] == KCharToneMarkPinyin1 ||
              aKey[0] == KCharToneMark2 ||
              aKey[0] == KCharToneMark3 ||
              aKey[0] == KCharToneMark4 ||
              aKey[0] == KCharToneMark5 );
    }
    
inline TBool IsKeyAutoSeparator(const TDesC& aKey)
    {
    return (aKey[0] == KAutoSeparator );
    }

inline TBool IsKeyManualSeparator(const TDesC& aKey)
    {
    return (aKey[0] == KManulSeparator || aKey[0] == KStrokeManuSeparator);
    }
    
inline TBool IsKeyInternalWildChar(const TDesC& aKey)
    {
    return (aKey[0] == KInternalWildChar()[0] );
    }


    
// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CCompositionElement* CCompositionElement::NewLC(const TDesC& aKey)
    {
    TInt keyType = EElementTypeCompositionChar;   
    if( IsKeyAutoSeparator(aKey) )
        {
        keyType = EElementTypeAutoSeparator;
        }
    else if( IsKeyManualSeparator(aKey) )
        {
        keyType = EElementTypeManualSeparator;
        }
    else if( IsKeyToneMark(aKey) )
        {
        keyType = EElementTypeTonemark;
        }

    CCompositionElement* self = new (ELeave) CCompositionElement(keyType);
    
    CleanupStack::PushL(self);

    if( IsKeyInternalWildChar(aKey) )
        {
        self->BaseConstructL(KWildChar);
        }
    else
        {
        self->BaseConstructL(aKey);
        }
    
    
    return self;
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CCompositionElement::CCompositionElement(TInt aKeyType)
:iKeyType(aKeyType)
    {
    }
    
// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
void CCompositionElement::BaseConstructL(const TDesC& aKey)
    {
    iKey = aKey.AllocL();
    }
    
// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CCompositionElement::~CCompositionElement()
    {
    delete iKey;
    }

// ---------------------------------------------------------
// Get original input length
// ---------------------------------------------------------
//
TInt CCompositionElement::OriginalCount() const
    {
    TInt result = (IsAutoSeparator() || !iKey)? 0 : iKey->Length();
    
    return result;
    }
    
// ---------------------------------------------------------
// UnConvert and insert the original elements back
// ---------------------------------------------------------
//
TInt CCompositionElement::UndoConvertL(CAknFepVkbCompositionField* /*aField*/, TInt /*aStartPos*/)
    {
    return 0;
    }

// ---------------------------------------------------------
// Get the content  of original input
// ---------------------------------------------------------
//
const CCompositionElement* CCompositionElement::OriginalContent( TInt /*aIndex*/ ) const
    {
    return NULL;
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CCompositionConvertedElement* CCompositionConvertedElement::NewLC(const TDesC& aKey)
    {
    CCompositionConvertedElement* self = new (ELeave) CCompositionConvertedElement();
    
    CleanupStack::PushL(self);
    self->ConstructL(aKey);
    
    return self;
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CCompositionConvertedElement::CCompositionConvertedElement()
:CCompositionElement(EElementTypeConvertedChar)
    {
    }
    
// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
void CCompositionConvertedElement::ConstructL(const TDesC& aKey)
    {
    BaseConstructL(aKey);
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CCompositionConvertedElement::~CCompositionConvertedElement()
    {
    iOriginalElements.ResetAndDestroy();
    iOriginalElements.Close();
    }

// ---------------------------------------------------------
// UnConvert and insert the original elements back
// ---------------------------------------------------------
//
TInt CCompositionConvertedElement::UndoConvertL(CAknFepVkbCompositionField* aField, 
                                               TInt aStartPos)
    {
    ASSERT(aStartPos >=0 && aStartPos < aField->Length());
    
    TInt length = iOriginalElements.Count();
    
    for (TInt i = length -1 ; i >= 0 ; i --)
        {
        aField->InsertL(aStartPos, iOriginalElements[i] );
        
        //ownership transfer to CAknFepVkbCompositionField
        iOriginalElements.Remove(i);
        }
        
    return length;
    }

// ---------------------------------------------------------
// Append an original element
// ---------------------------------------------------------
//
void CCompositionConvertedElement::AppendOriginal(const CCompositionElement* aElements)
    {
    iOriginalElements.Append(aElements);
    }

// ---------------------------------------------------------
// Get original input length
// ---------------------------------------------------------
//
TInt CCompositionConvertedElement::OriginalCount() const
    {
    return iOriginalElements.Count();
    }
    
// ---------------------------------------------------------
// Get the content  of original input
// ---------------------------------------------------------
//
const CCompositionElement* CCompositionConvertedElement::OriginalContent( TInt aIndex ) const
    {
    if( aIndex < 0 || aIndex > iOriginalElements.Count() - 1 )
        {
        return NULL;
        }
    return iOriginalElements[aIndex];
    }
    
// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CAknFepVkbCompositionField* CAknFepVkbCompositionField::NewL(TInt aMaxLength,
                                                            TRect aRect,
                                                            CFepUiLayout* aUiLayout,
                                                            TInt aControlId,
                                                            MAknFepVkbDataProvider* aDataProvider )
    {
    CAknFepVkbCompositionField* self = new (ELeave) CAknFepVkbCompositionField(aMaxLength,
                                                                              aRect,
                                                                              aUiLayout,
                                                                              aControlId,
                                                                              aDataProvider );
    
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    
    return self;
    }

// ---------------------------------------------------------
// C++ Constructor
// ---------------------------------------------------------
//
CAknFepVkbCompositionField::CAknFepVkbCompositionField(TInt aMaxLength,
                                                      TRect aRect,
                                                      CFepUiLayout* aUiLayout,
                                                      TInt aControlId,
                                                      MAknFepVkbDataProvider* aDataProvider)
: CFepLayoutEditAreaBase(aRect,aUiLayout,aControlId)
    {
    //iMaxLength <=0 means no max length limited
    iMaxLength = aMaxLength;
    iCurrentPos = 0;
    iFirstSegment = NULL;
    iDataProvider = aDataProvider;
    
    iIsValidSpells = EFalse;

    }

// ---------------------------------------------------------
// 2nd Constructor
// ---------------------------------------------------------
//
void CAknFepVkbCompositionField::ConstructL()
    {
    //No cursor in construction
    BaseConstructL();
    //iBufferForCommit = HBufC::NewL(KDefaultLen);
 
    
    CPinyinAnalyserDbFeed* pZhuyinDb = CPinyinAnalyserDbFeed::NewL( EPinyinAnalyserTypeZhuyin );
    CleanupStack::PushL( pZhuyinDb );
    iZhuyinAnalyser = CAknFepVkbPinyinAnalyser::NewL( pZhuyinDb );
    CleanupStack::Pop( pZhuyinDb );
    
    CPinyinAnalyserDbFeed* pPinyinDb = CPinyinAnalyserDbFeed::NewL( EPinyinAnalyserTypePinyin );
    CleanupStack::PushL( pPinyinDb );
    iPinyinAnalyser = CAknFepVkbPinyinAnalyser::NewL( pPinyinDb );
    CleanupStack::Pop( pPinyinDb );
    
    
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CAknFepVkbCompositionField::~CAknFepVkbCompositionField()
    {
    iElements.ResetAndDestroy();
    iElements.Close();
    //delete iBufferForCommit;
    delete iFirstSegment;
    delete iZhuyinAnalyser;
    delete iPinyinAnalyser;
    }

// ---------------------------------------------------------
// Handle events in
// ---------------------------------------------------------
//
void CAknFepVkbCompositionField::HandleControlEvent(TInt aEventType, 
                                                             CFepUiBaseCtrl* /*aCtrl*/, 
                                                             const TDesC& aEventData)
    {
    TBuf<4> buf;    
    switch (aEventType)
        {
        case EEventVirtualKeyUp:
            {
            TKeyEvent* event = (TKeyEvent*) aEventData.Ptr();
            buf.Append(TChar(event->iScanCode));	
            //assume keys are insertable
            TRAP_IGNORE(HandleInsertL(buf));
            break;
            }
        case EVkbEventKeySpace:
            {
            TInt imLayout = iDataProvider->RequestData(EAknFepDataTypeIMLayout);
            if (imLayout == EAknFepVkbImCnCangjie)
                {
                if(!IsValidSpell())
                    {
                    break;
                    }
                
                RBuf firstCandidate(static_cast<CAknFepVkbDataMgr*>(iDataProvider)->Candidates()[0]); 
                firstCandidate.AppendNum( 0 );
                TRAP_IGNORE(HandleConvertedResultL(firstCandidate));
                }
            else
                {
                TKeyEvent* event = (TKeyEvent*) aEventData.Ptr();
                buf.Append(TChar(event->iCode));	
                //assume keys are insertable
                TRAP_IGNORE(HandleInsertL(buf));
                }
            break;
            }        
        case EVkbEventKeyEnter:
            TRAP_IGNORE(HandleFlushL());        
            break;
        case EPeninputLayoutEventBack:
            TRAP_IGNORE(HandleBackL());
            break;
		case EVkbEventLeftArrow:
			{
		    TCursorSelection currentCursorSelection;
		    currentCursorSelection = Selection();
		    
		    if (ETrue)
		        {
	            if (currentCursorSelection.iCursorPos == 0)
	                {
	                currentCursorSelection.iCursorPos = TextLength();
	                }
	            else 
	                {
	                currentCursorSelection.iCursorPos--;
	                if(currentCursorSelection.iCursorPos > 0 &&
						iElements[currentCursorSelection.iCursorPos - 1]->IsAutoSeparator())
	                    {
	                    currentCursorSelection.iCursorPos--;		                
		                }
	                }
		        }
		        
		    TCursorSelection movedCursor(currentCursorSelection.iCursorPos, 
                                     currentCursorSelection.iCursorPos);
		    UpdateCursorSelection(movedCursor);
		    break;
			}
		case EVkbEventRightArrow:
			{
			TCursorSelection currentCursorSelection;
			currentCursorSelection = Selection();

		    if (currentCursorSelection.iCursorPos == TextLength())
		        {
		        currentCursorSelection.iCursorPos = 0;
		        }
		    else 
		        {
		        currentCursorSelection.iCursorPos++;
				if(currentCursorSelection.iCursorPos < TextLength() &&
					iElements[currentCursorSelection.iCursorPos - 1]->IsAutoSeparator())
                  {
                  currentCursorSelection.iCursorPos++;
                  }
		        
		        }	        

			TCursorSelection movedCursor(currentCursorSelection.iCursorPos, 
                                   currentCursorSelection.iCursorPos);
			UpdateCursorSelection(movedCursor);
			break;
			}
        case EEventControlFocusLost:
            SetFocus(EFalse);
            break;
        case EEventControlFocusGained:
            SetFocus();
            break;
        case EVkbEventCompFieldAnalysisResponse:
            TRAP_IGNORE(HandleAnalysisResponseL(aEventData));
            break;
        case EVkbEventCandidateSelected:
            TRAP_IGNORE(HandleConvertedResultL(aEventData));
            break;
        case EVkbEventWindowClose:
        case EVkbEventClearContent:
            TRAP_IGNORE(ClearTextL());
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------
// Count of elements
// ---------------------------------------------------------
//
TInt CAknFepVkbCompositionField::Length() const
    {
    return iElements.Count();
    }

// ---------------------------------------------------------
// Set warning color
// ---------------------------------------------------------
//
void CAknFepVkbCompositionField::SetWarningColor(const TRgb& aColor) 
    {
    iWarningColor = aColor;
    }
    
// ---------------------------------------------------------
// Set normal color
// ---------------------------------------------------------
//
void CAknFepVkbCompositionField::SetNormalColor(const TRgb& aColor) 
    {
    iNormalColor = aColor;
    }    

// ---------------------------------------------------------
// Insert a key
// ---------------------------------------------------------
//
void CAknFepVkbCompositionField::HandleInsertL(const TDesC& aKey)
    {
    CCompositionElement *element = CCompositionElement::NewLC(aKey);
    
    iCurrentPos = DisplayPosToInternaPos( iSelectedCompositionText.iCursorPos );              
    
    TBool updateText = EFalse;  
    //remove selection first
    if( iSelectedCompositionText.Length() > 0)
        {
        RemoveSelection();
        updateText = ETrue;
        }
        
    if (IsAllowedInsert(element))
        {
        InsertElementL(element);
        CleanupStack::Pop(element);
        SendAnalysisRequestL();
        updateText = EFalse;
        }
    else
        {
        // If select all the chars in composition and input an invalid char,
        // go to standby status.
        if(Length() == 0)
            {
            ReportEvent(EVkbEventCompFieldNoChars);
            ClearTextL();
            }
        else
            {
            SendAnalysisRequestL();
            }
            
        CleanupStack::PopAndDestroy(element);
        }
        
    if( updateText )
        {
        UpdateTextL();
        }
    }

// ---------------------------------------------------------
// Insert an element
// ---------------------------------------------------------
//
//insert is only insert 
void CAknFepVkbCompositionField::InsertElementL(const CCompositionElement* aElement)
    {
    if( IsReplaceFront(aElement) )
        {
        Remove(iCurrentPos - 1, 1);        
        }
    else if( IsReplaceBack(aElement) )
        {
        Remove(iCurrentPos, 1);        
        }      

    InsertL(iCurrentPos, aElement);  
    }
    
// ---------------------------------------------------------
// Clear text
// ---------------------------------------------------------
//
void CAknFepVkbCompositionField::ClearTextL()
    {
    Clear();
    
    UpdateTextL();
    
    iNeedCreateWord = EFalse;
    iConvertedElement = 0;
    iIsValidSpells = ETrue;
    }

// ---------------------------------------------------------
// Submit all text
// ---------------------------------------------------------
//
void CAknFepVkbCompositionField::HandleFlushL()
    {
    // Don't response when CangJieVkb state
    if ( iDataProvider->RequestData( EAknFepDataTypeIMLayout )  == EAknFepVkbImCnCangjie )
        {
        return;
        }
    
    HBufC* temp = HBufC::NewLC(iElements.Count());
    TPtr ptr = temp->Des();
    
    for (TInt i = 0; i < iElements.Count(); i++)
        {
        if (!iElements[i]->IsTypeOf(EElementTypeAutoSeparator))
            {
            ptr.Append(*iElements[i]->Key());
            }
        }
        
    if (ptr.Length() > 0)
        {
        ReportEvent(EVkbEventCompFieldDataFlush, *temp);
        ClearTextL();
        }
        
    CleanupStack::PopAndDestroy(temp);
    }
    
// ---------------------------------------------------------
// Handle back key
// ---------------------------------------------------------
//
void CAknFepVkbCompositionField::HandleBackL()
    {
    if (Length() == 0)
        {
        return;
        }
    
    iCurrentPos = DisplayPosToInternaPos( iSelectedCompositionText.iCursorPos );
    
    if(iSelectedCompositionText.Length() > 0)
        {
        // update cursor position
        RemoveSelection();
        }
    else
        {
        TInt pos = iCurrentPos - 1;
        if ( iCurrentPos == 0 )
            {//nothing to delete
            return;
            }
            
        //assure the element before cursor is not an auto separator
        //cursor can not be put right after an auto separator
        ASSERT(!iElements[pos]->IsAutoSeparator());

        if (!iElements[pos]->IsTypeOf(EElementTypeConvertedChar))
            {
            Remove(pos, 1, ETrue);
            ReFomatAfterDelete();
            }
        else
            {
            //current element is converted element, undo convert 
            TInt len = iElements[pos]->UndoConvertL(this, pos);
            iCurrentPos -= ( len + 1 );
            if ( iCurrentPos < 0 )
                {
                iCurrentPos = 0;
                }
            //remove the converted element
            Remove(pos + len, 1);
            iConvertedElement--;
            if( iConvertedElement == 0 && iNeedCreateWord)
                {
                iNeedCreateWord = EFalse;
                }
            }
        }

    if (Length() == 0)
        {
        ReportEvent(EVkbEventCompFieldNoChars);
        ClearTextL();
        }
    else
        {
        //no composition to analysis
        if (IsFinishComposition())
            {
            UpdateTextL();
            ReportEvent(EVkbEventCompFieldSubmit, iDisplayText);
            ClearTextL();
            }
        else
            {
            SendAnalysisRequestL();
            }
        }
    }
    
// ---------------------------------------------------------
// Handle analysis response
// ---------------------------------------------------------
//
void CAknFepVkbCompositionField::HandleAnalysisResponseL(const TDesC& aCompText)
    {
    
    if ( ( iDataProvider->RequestData( EAknFepDataTypeIMLayout )  == EAknFepVkbImCnZhuyin
        || iDataProvider->RequestData( EAknFepDataTypeIMLayout )  == EAknFepVkbImCnPinyin ) )
    {
    if( iIsValidSpells )
        {
        SetTextColor(iNormalColor);
        }
    else
        {
        SetTextColor(iWarningColor);    
        }
    }
            
    if ( iDataProvider->RequestData( EAknFepDataTypeIMLayout )  == EAknFepVkbImCnCangjie
        || iDataProvider->RequestData( EAknFepDataTypeIMLayout )  == EAknFepVkbImCnStroke )
    {
    if ( aCompText.Length() == 0 )
        {
        iIsValidSpells  = EFalse;
        SetTextColor(iWarningColor);
        }
    else
        {
        iIsValidSpells  = ETrue;
        SetTextColor(iNormalColor);
        }

    }
            
    if (aCompText.Length() > 0)
        {
        //merge the original elements with engine returned strings
        //remove auto separator in orignal elements and insert auto separator
        //add by engine 
        TInt origin = iFirstSegmentOffset;
        TInt engine = 0;
        while( engine < aCompText.Length() && origin < iElements.Count() )
            {
            if( *(iElements[origin]) == aCompText[engine] )
                {
                origin++;
                engine++;
                }
            else if( iElements[origin]->IsAutoSeparator() )
                {
                Remove(origin, 1);
                iFirstSegmentLength--;
                }
            else if( IsKeyAutoSeparator(aCompText.Mid(engine,1)) )
                {
                if( !iElements[origin]->IsManualSeparator() )
                    {
                	CCompositionElement* element = 
                                             CCompositionElement::NewLC(aCompText.Mid(engine, 1));
                    InsertL(origin, element);
                    CleanupStack::Pop(element);
                    iFirstSegmentLength++;
                    }
                origin++;
                engine++;
                }
            else
                {
                ASSERT(0);                
                }
            }
        //judge if invalid tone mark exist in iElements
        if(aCompText.Length() < iElements.Count() - iFirstSegmentOffset)
            {
            if(origin <= iElements.Count() - 1 && iElements[origin]->IsTonemark())
                {
                iIsValidSpells  = EFalse;
                SetTextColor(iWarningColor);                
                }
            }
        }

    UpdateTextL();
    Draw();
    UpdateArea(Rect(),EFalse);
    }
    
// ---------------------------------------------------------
// Handle converted result
// ---------------------------------------------------------
//
void CAknFepVkbCompositionField::HandleConvertedResultL(const TDesC& aConvertedText)
    {
    TInt index = iFirstSegmentOffset;
    
    //the last aConvertedText is the cell No of candidate list
    for (TInt i = 0; i < aConvertedText.Length() - 1; i ++)
        {
        CCompositionConvertedElement* element = 
                                     CCompositionConvertedElement::NewLC(aConvertedText.Mid(i, 1));
        
        TBool goOn = ETrue;
                
        while (index < Length() && !iElements[index]->IsTypeOf(EElementTypeConvertedChar) && goOn)
            {
            goOn = !iElements[index]->IsAuxilian();
            
            if( iElements[index]->IsAutoSeparator() )
                {
                Remove(index, 1); //remove automatic separator
                }
            else
                {
                element->AppendOriginal(iElements[index]); //transfer ownership to converted element                    
                Remove(index, 1, EFalse); //remove the original element
                }
            }

        //insert the converted element
        InsertL(index, element);
        iCurrentPos = index + 1;
        iConvertedElement++;
        
        CleanupStack::Pop(element);
        index ++;
        }

    if( IsFinishComposition() )
        {
        UpdateTextL();
        ReportEvent(EVkbEventCompFieldSubmit, *iBuffer);
        ClearTextL();
        }
    else
        {
        //user takes part in composition
        iNeedCreateWord = ETrue;
        SendAnalysisRequestL();
        }
    }
    
// ---------------------------------------------------------
// Is there need to create new word
// ---------------------------------------------------------
//
TBool CAknFepVkbCompositionField::NeedCreateWord()
	{
	return iNeedCreateWord && iElements.Count() <= KCpiMaxInputPhraseLen;
	}
    
// ---------------------------------------------------------
// Remove elements
// ---------------------------------------------------------
//
void CAknFepVkbCompositionField::Remove(TInt aStartPos, TInt aLength, TBool aDestroy)
    {
    ASSERT (aStartPos >= 0 && aStartPos < Length());
        
    TInt index = Min(aStartPos + aLength - 1, Length() -1);

    //adjust cursor position if necessary
    if( iCurrentPos > aStartPos)
        {
        iCurrentPos -= index - aStartPos + 1;    
        }
            
    for (TInt i = index; i >= aStartPos; i --)    
        {
        if (aDestroy)
            {
            delete iElements[i];
            }
            
        iElements.Remove(i);
        }          
    }

// ---------------------------------------------------------
// Insert an element
// ---------------------------------------------------------
//
void CAknFepVkbCompositionField::InsertL(TInt aPos, const CCompositionElement* aElement)
    {
    ASSERT (aPos >=0 && aPos <= Length());
    iElements.Insert(aElement, aPos);    
    if( iCurrentPos >= aPos )
        {
        iCurrentPos ++;
        }
    }

// ---------------------------------------------------------
// Clear element
// ---------------------------------------------------------
//
void CAknFepVkbCompositionField::Clear()
    {
    if (Length() == 0)
        {
        return;
        }
    
    iCurrentPos = 0;
    iElements.ResetAndDestroy();
    }
        
// ---------------------------------------------------------
// Generate first segment
// ---------------------------------------------------------
//
void CAknFepVkbCompositionField::GetFirstSegmentL()
    {    
    delete iFirstSegment;
    iFirstSegment = NULL;
    iFirstSegment = HBufC::NewL(Length());
    TPtr ptr = iFirstSegment->Des();

    iFirstSegmentOffset = 0;
    iFirstSegmentLength = 0;
    
    TInt i = 0;
    for (i = 0; i < Length(); i++)
        {
        //start from the first unconverted element
        if (!iElements[i]->IsTypeOf(EElementTypeConvertedChar) &&
            !iElements[i]->IsSeparator() )
            {
            iFirstSegmentOffset = i;
            ptr.Append((*iElements[i++]->Key()));
            iFirstSegmentLength++;
            while( i < Length() && !iElements[i]->IsTypeOf(EElementTypeConvertedChar)  )
                {
                if( !iElements[i]->IsAutoSeparator() )
                    {
                    ptr.Append(*(iElements[i]->Key()));
                    }
                    
                i++;
                }
            
            break;
            }
        }
    
    iFirstSegmentLength = i - iFirstSegmentOffset;
    if( ptr.Length() > 0 && 
        IsKeyManualSeparator(ptr.Right(1)) )
        {
        ptr.SetLength(ptr.Length() - 1);
        iFirstSegmentLength--;
        }
    }

// ---------------------------------------------------------
// Update editbase cursor and text
// ---------------------------------------------------------
// assume the current all elements are valid
void CAknFepVkbCompositionField::UpdateTextL()
    {
    HBufC* displayText = HBufC::NewLC(iElements.Count());
    //adjust cursor to avoid autosepartor
    if( iCurrentPos > 0 && iElements[iCurrentPos - 1]->IsAutoSeparator() )
        {
        iCurrentPos--;
        }
        
    TInt pos = iCurrentPos;
    for( TInt i = 0; i < iElements.Count(); ++i )
        {
        //deal with manual separator 
        if( iElements[i]->IsManualSeparator() )
            {
            //valid format ensure i > 0
            if( iElements[i - 1]->IsTypeOf(EElementTypeConvertedChar))
                {
                iElements[i]->Hide(ETrue);
                }
            else if( i + 1 < iElements.Count() && 
                     iElements[i + 1]->IsTypeOf(EElementTypeConvertedChar) )
                {
                iElements[i]->Hide(ETrue);
                }
            else
                {
                iElements[i]->Hide(EFalse);
                }
            }
        //maybe other things
        if(iElements[i]->IsVisible())
            {
                {
                displayText->Des().Append(iElements[i]->Key()->Left(1));
                }
            }
        else if( i < iCurrentPos)
            {
            pos--;
            }
        }
    
    TFepInputContextFieldData data;
    data.iCmd = EPeninputICFInitial;
    data.iStartPos = 0;
    data.iLength = displayText->Length();
    data.iMidPos = -1;
    data.iText.Set( *displayText );
    data.iCurSel = TCursorSelection(pos, pos);
    data.iCursorVisibility = ETrue;
    data.iCursorSelVisible = ETrue;
    SetTextL( data );
    
    CleanupStack::PopAndDestroy(displayText);
    }

// ---------------------------------------------------------
// Adjust cursor when pen down
// ---------------------------------------------------------
//
void CAknFepVkbCompositionField::AdjustSelectedCompositionText(
                                              TInt& aPositionOfInsertionPointInBuffer)
    {
    TInt pos = DisplayPosToInternaPos(aPositionOfInsertionPointInBuffer);
    pos--;
    if (pos >=0 && pos < Length() && iElements[pos]->IsAutoSeparator())
        {
        aPositionOfInsertionPointInBuffer --;
        }

    if (aPositionOfInsertionPointInBuffer < 0)
        {
        aPositionOfInsertionPointInBuffer = 0;
        }
    }
    
// ---------------------------------------------------------
// Update edit base content when scrolling
// ---------------------------------------------------------
//
void CAknFepVkbCompositionField::UpdateContent(const TCursorSelection& aCursorSel)
    {
    TInt cursorPos = aCursorSel.iCursorPos;
    if (cursorPos < 0 )
        {
        cursorPos = 0;
        }
    else if (cursorPos > iBuffer->Length())
        {
        cursorPos = iBuffer->Length();
        }
        
    UpdateCursorSelection(TCursorSelection(cursorPos, aCursorSel.iAnchorPos));
    }
    
// ---------------------------------------------------------
// Is allowed insert
// ---------------------------------------------------------
//
TBool CAknFepVkbCompositionField::IsAllowedInsert(const CCompositionElement* aElement)
    {
    if( (iCurrentPos == 0 ||
         iElements[iCurrentPos - 1]->IsTypeOf(EElementTypeConvertedChar)) &&
        aElement->IsAuxilian() )
        {
        return EFalse;
        }
    
    if (aElement->IsManualSeparator())
    	{
    	//cursor must be larger than zero
    	if (iElements[iCurrentPos - 1]->IsTonemark() ||
    	    iElements[iCurrentPos - 1]->IsManualSeparator())
    		{
    		return EFalse;
    		}    	
    	if ( (iElements.Count() >= iCurrentPos + 1) && 
    	     (iElements[iCurrentPos]->IsTonemark() ||
    	      iElements[iCurrentPos]->IsTypeOf(EElementTypeConvertedChar) ||
    	      iElements[iCurrentPos]->IsManualSeparator()) )
    		{
    		return EFalse;
    		}
    	}

    if ( IsReplaceFront(aElement) || 
         IsReplaceBack(aElement))
        {
        return ETrue;
        }
        
    TInt count = 0;    
    for (TInt i = 0; i <Length(); i++)
        {
        count += iElements[i]->OriginalCount();
        }
        
    return (count < iMaxLength);
    }

// ---------------------------------------------------------
// Send analysis request
// ---------------------------------------------------------
//
void CAknFepVkbCompositionField::SendAnalysisRequestL()
    {
    GetFirstSegmentL();
    CAknFepVkbPinyinAnalyser* pAnalyser = NULL;
    
    if( iDataProvider->RequestData( EAknFepDataTypeIMLayout )  == EAknFepVkbImCnZhuyin )
        {
        pAnalyser = iZhuyinAnalyser;
        }
    else if ( iDataProvider->RequestData( EAknFepDataTypeIMLayout ) == EAknFepVkbImCnPinyin )
        {
        pAnalyser = iPinyinAnalyser;
        }
    
    if ( pAnalyser )
        {
        iIsValidSpells = pAnalyser->AnalyzeL( *iFirstSegment );
    
        TPtrC compText( *pAnalyser->LastAnalysisResult() );
        TInt origin = iFirstSegmentOffset;
        TInt engine = 0;
        while( engine < compText.Length() && origin < iElements.Count() )
            {
            if( *(iElements[origin]) == compText[engine] )
                {
                origin++;
                engine++;
                }
            else if( iElements[origin]->IsAutoSeparator() )
                {
                Remove(origin, 1);
                iFirstSegmentLength--;
                }
            else if( IsKeyAutoSeparator(compText.Mid(engine,1)) )
                {
                if( !iElements[origin]->IsManualSeparator() )
                    {
                	CCompositionElement* element = CCompositionElement::NewLC(compText.Mid(engine, 1));
                    InsertL(origin, element);
                    CleanupStack::Pop(element);
                    iFirstSegmentLength++;
                    }
                origin++;
                engine++;
                }
            else
                {
                ASSERT(0);                
                }
        }
        
        ReportEvent(EVkbEventCompFieldAnalysisReq, *pAnalyser->LastAnalysisResult() );
        }
    else
        {
        ReportEvent(EVkbEventCompFieldAnalysisReq, *iFirstSegment);
        }
    }

// ---------------------------------------------------------
// To test whether the all input keys are converted
// ---------------------------------------------------------
//
TBool CAknFepVkbCompositionField::IsFinishComposition()
    {
    for (TInt i = 0; i < Length(); i++)
        {
        if (!iElements[i]->IsTypeOf(EElementTypeConvertedChar))
            {
            return EFalse;
            }
        }
        
    return ETrue;        
    }

// ---------------------------------------------------------
// To test whether current pos element should be replaced by input one
// ---------------------------------------------------------
//
TBool CAknFepVkbCompositionField::IsReplacePosition(const CCompositionElement* aElement,
                                                   TInt aPos)
    {
    if( aElement->IsTonemark() )
        {
        return iElements[aPos]->IsTonemark() || 
               iElements[aPos]->IsSeparator();
        }
    else if(aElement->IsManualSeparator() )
        {
        return iElements[aPos]->IsAutoSeparator();
        }
    else
        {
        return EFalse;
        }
    }

// ---------------------------------------------------------
// To test whether the one before current pos element should be replaced by input one
// ---------------------------------------------------------
//
TBool CAknFepVkbCompositionField::IsReplaceFront(const CCompositionElement* aElement)
    {
    if( iCurrentPos > 0 )
        {
        return IsReplacePosition(aElement, iCurrentPos - 1);
        }
        
    return EFalse;
    }
    
// ---------------------------------------------------------
// To test whether current pos element should be replaced by input one
// ---------------------------------------------------------
//
TBool CAknFepVkbCompositionField::IsReplaceBack(const CCompositionElement* aElement)
    {
    if( iCurrentPos <= iElements.Count() - 1 )
        {
        return IsReplacePosition(aElement, iCurrentPos);
        }
        
    return EFalse;
    }

// ---------------------------------------------------------
// Redo the format after deletion
// ---------------------------------------------------------
//
void CAknFepVkbCompositionField::ReFomatAfterDelete()
    {
//assumption
//1. delete only happen in cursor position
//2. before deletion, the format is OK
    if( iElements.Count() == 0 )
        {
        return;
        }
        
    if( iCurrentPos == 0 )
        {
        if( iElements[0]->IsSeparator() || 
            iElements[0]->IsTonemark())
            {
            Remove(0, 1, ETrue);
            }
        }
    else if( iCurrentPos <= iElements.Count() - 1)
        {
        if(iElements[iCurrentPos - 1]->IsAuxilian() &&
           iElements[iCurrentPos]->IsAuxilian() )
            { //one of two is need to be removed
            if(iElements[iCurrentPos]->IsTonemark() ||
               iElements[iCurrentPos]->IsManualSeparator() )
                {//remove the one before cursor
                Remove(iCurrentPos - 1, 1, ETrue);
                //to simulate remove + insert opertion
                iCurrentPos++;
                }
            else
                {//remove the one on the cursor        
                Remove(iCurrentPos, 1, ETrue);
                }
            }
        else if( iElements[iCurrentPos - 1]->IsAutoSeparator() &&
                 iElements[iCurrentPos]->IsTypeOf(EElementTypeConvertedChar) )
            {
            //remove auto separator before convert char
            Remove(iCurrentPos - 1, 1, ETrue);
            }
        else if( iElements[iCurrentPos]->IsAutoSeparator() &&
                 iElements[iCurrentPos - 1]->IsTypeOf(EElementTypeConvertedChar) )
            {
            //remove auto separator before convert char
            Remove(iCurrentPos, 1, ETrue);
            }
        }
    else //if( iCurrentPos == iElements.Count() )
        {
        if( iElements[iCurrentPos - 1]->IsAutoSeparator() )
            {
            Remove(iCurrentPos - 1, 1);
            }
        }
    }

// ---------------------------------------------------------
// Get display cursor positon by internal cursor position
// ---------------------------------------------------------
//
TInt CAknFepVkbCompositionField::InternalPosToDisplayPos(TInt aInternalPos)
    {
    for(TInt i = 0; i < aInternalPos; i++)
        {
        if( !iElements[i]->IsVisible() )
            {
            aInternalPos--;
            }
        }
    
    return aInternalPos;
    }

// ---------------------------------------------------------
// Get internal cursor positon by display cursor position
// ---------------------------------------------------------
//
TInt CAknFepVkbCompositionField::DisplayPosToInternaPos(TInt aDisplayPos)
    {
    TInt pos = 0;
    TInt i = 0;
    
    for( i = 0; i < iElements.Count() && pos < aDisplayPos; i++ )
        {
        if( iElements[i]->IsVisible() )
            {
            pos++;
            }        
        }
    for( i++; i < iElements.Count(); i++ )
        {
        if( iElements[i]->IsVisible() )
            {
            break;
            }
        }
    
    return --i;
    }

// ---------------------------------------------------------
// Remove the selected text
// ---------------------------------------------------------
//
void CAknFepVkbCompositionField::RemoveSelection()
    {
    TInt lower = DisplayPosToInternaPos( iSelectedCompositionText.LowerPos() );
    TInt upper = DisplayPosToInternaPos( iSelectedCompositionText.HigherPos() ); 
    Remove(lower, upper - lower);    
    ReFomatAfterDelete();    
    }

// ---------------------------------------------------------------------------
// CAknFepVkbCompositionField::GetCreatedWordSpell
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
const TInt KSpellSpace = 8; // private protocol for PtiCpiCore   
TPtrC CAknFepVkbCompositionField::GetCreatedWordSpell()
    {
    iCreateWordSpellBuffer.SetLength( 0 );
    //TUint16 bufText[10] = {0};
    //TUint16 bufSpell[100] = {0};    
    
    if( iNeedCreateWord )
        {
        //for()
        iCreateWordSpellBuffer.SetLength( KSpellSpace * iElements.Count() );
        iCreateWordSpellBuffer.FillZ();
        for ( int i = 0; i < iElements.Count(); i++ )
            {
            //bufText[i] = ( *( iElements[i]->Key() ) )[0];

            TUint16 temp  = ( *( iElements[i]->Key() ) )[0];
            iCreateWordSpellBuffer[KSpellSpace*i] = ( *( iElements[i]->Key() ) )[0];
            for( int j=0; j < iElements[i]->OriginalCount(); j++ )
                {
                const CCompositionElement* element = iElements[i]->OriginalContent( j );
                if( element && !element->IsSeparator() )
                    {
                    //bufSpell[ i*KSpellSpace + j] = ( *( element->Key() ) )[0];
                    iCreateWordSpellBuffer[KSpellSpace*i + j + 1] = ( *( element->Key() ) )[0];;
                    }
                }
            }
        }
    
    return iCreateWordSpellBuffer;
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbCompositionField::IsValidSpell
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TBool CAknFepVkbCompositionField::IsValidSpell()
    {
    // no just verify the Zhuyin & Pinyin spell, stoke&cangjie verify later
    if (  iDataProvider->RequestData( EAknFepDataTypeIMLayout )  == EAknFepVkbImCnZhuyin
        || iDataProvider->RequestData( EAknFepDataTypeIMLayout )  == EAknFepVkbImCnPinyin )
            {
            return iIsValidSpells;
            }
    if( ( iDataProvider->RequestData( EAknFepDataTypeIMLayout ) ==  EAknFepVkbImCnStroke
        || iDataProvider->RequestData( EAknFepDataTypeIMLayout ) ==  EAknFepVkbImCnCangjie ) )
        {
        return iIsValidSpells;
        }
    return ETrue;
    }
    
void CAknFepVkbCompositionField::ConstructFromResourceL()
    {    
    if (iResourceId == KInvalidResId)
    	{
        User::Leave(KErrArgument);
    	}

    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, iResourceId);


    TPtrC bmpFileName = reader.ReadTPtrC();  
    TInt32 imgMajorSkinId = reader.ReadInt32();

    TAknsItemID id;
    TInt skinitemid;
    
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();

    const TInt16 compositionbgId = reader.ReadInt16();
    const TInt16 compositionbgIdmaskId = reader.ReadInt16();
    skinitemid = reader.ReadInt16();

    id.Set( TInt( imgMajorSkinId ), skinitemid );
    
    if ( compositionbgId != KInvalidImg )
    	{
        CFbsBitmap* compositionImg = NULL;

        if (compositionbgIdmaskId != KInvalidImg)
            {
            CFbsBitmap* compositionmaskImg = NULL;
            
            AknsUtils::CreateIconL(skininstance,
                                   id,
                                   compositionImg,
                                   compositionmaskImg,
                                   bmpFileName,
                                   compositionbgId,
                                   compositionbgIdmaskId);
            
            AknIconUtils::SetSize(compositionmaskImg, TSize(1,1), EAspectRatioNotPreserved);
            SetBackgroundMaskBitmapL(compositionmaskImg);
            }
        else
            {
            AknsUtils::CreateIconL(skininstance,
                                   id,
                                   compositionImg,
                                   bmpFileName,
                                   compositionbgId);
            }
    	
    	AknIconUtils::SetSize(compositionImg, TSize(1,1), EAspectRatioNotPreserved);
    	SetBackgroundBitmapL(compositionImg);
    	}

    CleanupStack::PopAndDestroy(); // reader
    
    }
    
void CAknFepVkbCompositionField::SizeChanged(TRect aRect)
	{
	CFepLayoutEditAreaBase::SetRect(aRect);
    if( BackgroundBmp() )
        {
        CFbsBitmap* bmp = BackgroundBmp();
        if( aRect.Size() != bmp->SizeInPixels() )
            {
            AknIconUtils::SetSize(bmp, aRect.Size(), EAspectRatioNotPreserved);            
            }
        }
	}
// End Of File
