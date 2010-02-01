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
* Description:            Implementation of candidate base chinese phrase state
*
*/












#include "AknFepUiInputStateCandidateBaseChinesePhrase.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"
#include "AknFepManager.h"                  //FepMan flag

#include "AknFepUICtrlInputPane.h"
#include "AknFepUICtrlPinyinPopup.h"
#include <PtiEngine.h>                         //CPtiEngine

#include <aknnotewrappers.h>                //CAknInformationNote 

#include "AknFepUICtrlPinyinPopup.h"        //Pinyin phrase
#include "aknfepuictrleditpane.h"           //pinyin phrase creation
#include "aknfepzhuyinconvertion.h"
#include "aknfepzhuyinkeyhandlerinterface.h"
#include "AknFepZhuyinAnalyserDb.h"
#include "AknFepZhuyinAnalyser.h"

_LIT( KPinyinListSeparator, "'" );
_LIT( KPinyinStar, "*" );
_LIT( KPinyinTone0ValidStr, "\x02C9");
_LIT( KPinyinTone1ValidStr, "\x02CA");
_LIT( KPinyinTone2ValidStr, "\x02C7");
_LIT( KPinyinTone3ValidStr, "\x02CB");
_LIT( KPinyinTone4ValidStr, "\x02D9");

const TText KPinyinTone0Valid = 0x02C9;
const TText KPinyinTone1Valid = 0x02CA;
const TText KPinyinTone2Valid = 0x02C7;
const TText KPinyinTone3Valid = 0x02CB;
const TText KPinyinTone4Valid = 0x02D9;
const TInt KKey0Code = 48;
const TInt KKey2Code = 50;
const TInt KKey3Code = 51;
const TInt KKey4Code = 52;
const TInt KKey5Code = 53;
const TInt KKey6Code = 54;
const TInt KKey7Code = 55;
const TInt KKey8Code = 56;
const TInt KKey9Code = 57;

const TInt KMaxKeystrokeCount = 100;
const TInt KMaxPinyinLength = 60;

const TInt KStarCount1 = 1;
const TInt KStarCount2 = 2;
const TInt KStarCount3 = 3;
const TInt KStarCount4 = 4;
const TInt KStarCount5 = 5;

const TUint16 KSegment = 0x0020;

_LIT( KKey0Num, "0");
_LIT( KKey1Num, "1");
_LIT( KKey2Num, "2");
_LIT( KKey3Num, "3");
_LIT( KKey4Num, "4");
_LIT( KKey5Num, "5");
_LIT( KKey6Num, "6");
_LIT( KKey7Num, "7");
_LIT( KKey8Num, "8");
_LIT( KKey9Num, "9");
_LIT( KPinyinSysSeparator, "\x002E");

TAknFepInputStateCandidateBaseChinesePhrase::TAknFepInputStateCandidateBaseChinesePhrase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateBase(aOwner, aUIContainer)
    {
    }

void TAknFepInputStateCandidateBaseChinesePhrase::HandleCommandL(
    TInt aCommandId )
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    // it may be one of the 'valid' numbers..
    MAknFepUICtrlCandidatePane* candidatePane =
        UIContainer()->CandidatePane();
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    TPtrC text = candidatePane->CurrentPhraseCandidate();
    switch ( aCommandId )
        {
        // Handle the event frome command.
        case EAknSoftkeyCancel:
            //  case (TUint16)EAknSoftkeyCancel: //the Cancle in soft CBA
            TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
            break;
        case EAknSoftkeySelect:
            //case (TUint16)EAknSoftkeySelect: //the Selected in soft CBA
            HandleCommitL();
            break;
        default:
            TAknFepInputStateCandidateBase::HandleCommandL( aCommandId );
            break;
        }
    }

TBool TAknFepInputStateCandidateBaseChinesePhrase::HandleKeyL( TInt aKey,
    TKeyPressLength aLength )
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    if ( aKey == EPtiKeyStar )
        {
        if ( fepMan->IsEnableKeyStar() )
            {
            iOwner->FepMan()->TryCloseUiL();
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            return EFalse;
            }
        else
            {
            iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
            return ETrue;
            }
        }
    if ( !(HandleVerticalNavigation( aKey, aLength )
        || HandleHorizontalNavigation( aKey )) )
        {
        // it may be one of the 'valid' numbers..
        TInt index = MapKeyToIndex( aKey );
        MAknFepUICtrlCandidatePane* candidatePane =
            UIContainer()->CandidatePane();
        MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();

        TBool state = ETrue;
        if ( popup->IsChangeStatet() && aLength == ELongKeyPress )
            {
            state = EFalse;
            }
        else
            {
            popup->SetChangeState( EFalse );
            }
        if ( state )
            {
            if ( candidatePane->SelectIndex( index, EFalse ) || aKey
                == EKeyOK || aKey == EKeyCBA1 )
                {
                HandleCommitL();
                }
            }
        }
    return ETrue;
    }

TBool TAknFepInputStateCandidateBaseChinesePhrase::HandleHorizontalNavigation(TInt aKey)
    {
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    TBool response = EFalse;

    // Do navigation...
    if (aKey == EKeyLeftArrow)
        {
//        if (candidatePane->SelectedIndex() == 0 && 
//            candidatePane->IsFirstPage() )
//            {
//            return ETrue;
//            }
        
        if(!candidatePane->SelectPrev())
            {
//            candidatePane->PreviousCandidatePage();
            candidatePane->SetCandidateBuffer();
            candidatePane->SelectLastPhrase();
            }
        response = ETrue;
        }
    else if (aKey == EKeyRightArrow)
        {
//        if (candidatePane->IsLastPage() && 
//            (candidatePane->SelectedIndex() == candidatePane->VisibleCandidateCount() - 1))
//            {
//            return ETrue;
//            }  
                    
        if(!candidatePane->SelectNext())
            {
//            candidatePane->NextCandidatePage();
            candidatePane->SetCandidateBuffer();
            candidatePane->SelectFirstPhrase();
            }
        response = ETrue;
        }
        
    if (response)
        {
        UpdateIndicator();
        }   
             
    return response;
    }
TBool TAknFepInputStateCandidateBaseChinesePhrase::HandleVerticalNavigation(TInt aKey)
    {
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    MZhuyinKeyHandler* zhuyinKeyHandler = iOwner->ZhuyinKeyHandler();
    
    if (aKey == EKeyDownArrow)
        {
        if (candidatePane->IsLastPage())
            {
            return ETrue;
            }
        if ( iState != EPredictiveCandidate &&
        		iState != EPredictiveInput )
			{
			if (candidatePane->NeedGetNewCandidatePage() )
				{
				if ( !iOwner->PtiEngine()->NextCandidatePage() )
                {
                if ( candidatePane->IsLastPage() )
                    {
                    iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                    return ETrue;
                    }
                }
				else
					{
					CDesCArrayFlat* phraseCandidates =
							candidatePane->CandidateArray();
					RPointerArray<HBufC> list;
					TRAP_IGNORE(
					GetNewCandidatesL(iOwner->PtiEngine()->CandidatePage(), list);
					for (TInt i = 0; i < list.Count(); ++i)
                        {
						phraseCandidates->AppendL( *(list[i]));
						}
					list.ResetAndDestroy(););
					}
				}          
        	}       
        candidatePane->NextCandidatePage();
        }
    else if (aKey == EKeyUpArrow)
        {
        if ( candidatePane->IsFirstPage() && iState != EPredictiveCandidate )
            {
            zhuyinKeyHandler->SetState(0);
            iOwner->ChangeState(EZhuyinSequenceEdit);
            return EFalse;
            }
        else if ( candidatePane->IsFirstPage() )
        	{
        	return ETrue;
        	}
        candidatePane->PreviousCandidatePage();
        }
    else
        {
        return EFalse;
        }
    candidatePane->SetCandidateBuffer();
    candidatePane->SelectFirstPhrase();
    UpdateIndicator();    
    return ETrue;
    }

TBool TAknFepInputStateCandidateBaseChinesePhrase::HandleVerticalNavigation(TInt aKey, TKeyPressLength aLength)
    {
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    
    if (aKey == EKeyDownArrow)
        {
        TBool state = ETrue;
        if ( popup->IsChangeStatet() && aLength == ELongKeyPress )
            {
            state = EFalse;
            }
        else
            {
            popup->SetChangeState( EFalse );
            }
        if ( state )
            {
            if (candidatePane->IsLastPage())
                {
                return ETrue;
                }       
            if ( iState != EPredictiveCandidate &&
            		iState != EPredictiveInput )
            	{
            	if (candidatePane->NeedGetNewCandidatePage() )
					{
                if ( !iOwner->PtiEngine()->NextCandidatePage( ) )
                    {
                    if ( candidatePane->IsLastPage() )
                        {
                        iOwner->FepMan()->PlaySound( EAvkonSIDErrorTone );
                        return ETrue;
                        }
                    }
                else
                    {
                    CDesCArrayFlat* phraseCandidates =
                        candidatePane->CandidateArray( );
                    RPointerArray<HBufC> list;
                    TRAP_IGNORE(
                    GetNewCandidatesL( iOwner->PtiEngine()->CandidatePage( ), list );
                    for (TInt i = 0; i < list.Count( ); ++i )
                        {
                        phraseCandidates->AppendL( *(list[i]) );
                        }
						list.ResetAndDestroy(););
						}
					}
				}     
            candidatePane->NextCandidatePage();
            }
        }
    else if (aKey == EKeyUpArrow)
        {
        if (candidatePane->IsFirstPage() )
            {
            return ETrue;   
            }       
        candidatePane->PreviousCandidatePage();
        }
    else
        {
        return EFalse;
        }
    candidatePane->SetCandidateBuffer();
    candidatePane->SelectFirstPhrase();
    UpdateIndicator();    
    return ETrue;
    }

void TAknFepInputStateCandidateBaseChinesePhrase::UpdateIndicator()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();        
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
        
    if (candidatePane->IsFirstPage())
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows(EFalse);    
        }
    else
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows(ETrue);    
        }
   
   if (candidatePane->IsLastPage())
       {
       uiContainer->CandidatePane()->ShowDownScrollArrows(EFalse);     
       }
   else
   	   {
   	   uiContainer->CandidatePane()->ShowDownScrollArrows(ETrue);      
   	   }
    }    
        
// ---------------------------------------------------------
// get the valid keystroke
// ---------------------------------------------------------
//
void TAknFepInputStateCandidateBaseChinesePhrase::GetValidkeystrokeL( )
    {
    TInt keyCode;
    TInt stringBeforeLength = 0;
    TInt stringAfterLength = 0;
    TBuf<1> keystroke;
    TBuf<1> keystrokeTmp;

    CPtiEngine* ptiengine = iOwner->PtiEngine( );
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer( );
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow( );

    CDesCArrayFlat* keystrokeArray = popup->KeystrokeArray( );
    CDesCArrayFlat* inEffectKeystrokeArray = popup->InEffectKeystrokeArray( );

    inEffectKeystrokeArray->Reset( );
    inEffectKeystrokeArray->Compress( );

    for (TInt i = 0; i < keystrokeArray->Count( ); ++i )
        {
        stringBeforeLength = 0;
        stringAfterLength = 0;
        keystroke.Copy( keystrokeArray->MdcaPoint( i ) );
        if ( 0 == i )
            {
            if ( ( 0 == keystroke.Compare( KPinyinStar ) ) || ( 0
                == keystroke.Compare( KPinyinListSeparator ) ) )
                {
                return;
                }
            }
        else
            {
            keystrokeTmp.Copy( keystrokeArray->MdcaPoint( i - 1 ) );
            if ( 0 == keystroke.Compare( KPinyinListSeparator ) )
                {
                if ( ( 0 != keystrokeTmp.Compare( KPinyinListSeparator ) )
                    && ( 0 != keystrokeTmp.Compare( KPinyinStar ) ) )
                    {
                    inEffectKeystrokeArray->AppendL( keystroke );
                    }
                return;
                }
            }

        if ( i >= 1 )
            {
            keystrokeTmp.Copy( keystrokeArray->MdcaPoint( i - 1 ) );
            if ( ( 0 == keystroke.Compare( KPinyinStar ) ) && ( 0
                == keystrokeTmp.Compare( KPinyinStar ) ) )
                {
                inEffectKeystrokeArray->AppendL( keystroke );
                ptiengine->IncrementToneMark( ETrue );
                }
            else if ( ( 0 != keystrokeTmp.Compare( KPinyinStar ) ) && ( 0
                == keystroke.Compare( KPinyinStar ) ) )
                {
                inEffectKeystrokeArray->AppendL( keystroke );
                ptiengine->IncrementToneMark( ETrue );
                }
            else if ( ( 0 != keystroke.Compare( KPinyinStar ) ) && ( 0
                == keystrokeTmp.Compare( KPinyinStar ) ) )
                {
                if ( !CheckSpellingOfPtiEngine( ) )
                    {
                    ptiengine->DeleteKeyPress( );
                    DeleteTonMarkFromInEffectKeytrokeArray( );
                    }
                return;
                }
            }

        if ( 0 != keystroke.Compare( KPinyinStar ) )
            {

            inEffectKeystrokeArray->AppendL( keystrokeArray->MdcaPoint( i ) );
            GetKeystrokeCode( keyCode, keystroke );
            stringBeforeLength = ptiengine->GetPhoneticSpelling(1).Length( );
            stringAfterLength = ptiengine->AppendKeyPress((TPtiKey)keyCode).Length( );
            if ( stringBeforeLength == stringAfterLength )
                {
                inEffectKeystrokeArray->Delete( inEffectKeystrokeArray->Count( )
                    - 1 );
                inEffectKeystrokeArray->Compress( );
                return;
                }
            if ( 0 == keystroke.Compare( KPinyinListSeparator ) )
                {
                return;
                }

            if ( !CheckSpellingOfPtiEngine( ) )
                {
                ptiengine->DeleteKeyPress( );
                inEffectKeystrokeArray->Delete( i );
                return;
                }
            }
        else
            {
            if ( i == keystrokeArray->Count( ) - 1 )
                {
                if ( !CheckSpellingOfPtiEngine( ) )
                    {
                    ptiengine->DeleteKeyPress( );
                    DeleteTonMarkFromInEffectKeytrokeArray( );
                    return;
                    }
                }
            }
        }
    }

// ---------------------------------------------------------
// Check the Spelling of PtiEngine.
// ---------------------------------------------------------
//
TBool TAknFepInputStateCandidateBaseChinesePhrase::CheckSpellingOfPtiEngine( )
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine( );
    TInt count = ptiengine->PhoneticSpellingCount( );

    for (TInt i = 1; i <= count; ++i )
        {
        TPtrC spelling = ptiengine->GetPhoneticSpelling( i );
        if ( KErrNotFound == spelling.Find( KPinyinListSeparator ) )
            {
            return ETrue;
            }
        }

    return EFalse;
    }

void TAknFepInputStateCandidateBaseChinesePhrase::GetKeystrokeCode(
    TInt& aKey, const TDesC& aKeystroke )
    {
    if ( 0 == aKeystroke.Compare( KKey0Num ) )
        {
        aKey = KKey0Code;
        }
    else if ( 0 == aKeystroke.Compare( KKey1Num ) )
        {
        aKey = KKey0Code;
        }
    else if ( 0 == aKeystroke.Compare( KKey2Num ) )
        {
        aKey = KKey2Code;
        }
    else if ( 0 == aKeystroke.Compare( KKey3Num ) )
        {
        aKey = KKey3Code;
        }
    else if ( 0 == aKeystroke.Compare( KKey4Num ) )
        {
        aKey = KKey4Code;
        }
    else if ( 0 == aKeystroke.Compare( KKey5Num ) )
        {
        aKey = KKey5Code;
        }
    else if ( 0 == aKeystroke.Compare( KKey6Num ) )
        {
        aKey = KKey6Code;
        }
    else if ( 0 == aKeystroke.Compare( KKey7Num ) )
        {
        aKey = KKey7Code;
        }
    else if ( 0 == aKeystroke.Compare( KKey8Num ) )
        {
        aKey = KKey8Code;
        }
    else if ( 0 == aKeystroke.Compare( KKey9Num ) )
        {
        aKey = KKey9Code;
        }
    else if ( 0 == aKeystroke.Compare( KPinyinListSeparator ) )
        {
        aKey = KKey0Code;
        }

    }
// ---------------------------------------------------------
// clear the ptiengine keystroke;
// ---------------------------------------------------------
//
void TAknFepInputStateCandidateBaseChinesePhrase::ClearPtiEngineKeystroke( )
    {
    CDesCArrayFlat* inEffectKeystrokeArray = UIContainer()->PinyinPopupWindow()->InEffectKeystrokeArray( );

    for (TInt i = 0; i < KMaxKeystrokeCount; ++i )
        {
        iOwner->PtiEngine()->DeleteKeyPress( );
        }
    inEffectKeystrokeArray->Reset( );
    inEffectKeystrokeArray->Compress( );
    }

// ---------------------------------------------------------
// delete tonmark from in effect keystroke array
// ---------------------------------------------------------
//
void TAknFepInputStateCandidateBaseChinesePhrase::DeleteTonMarkFromInEffectKeytrokeArray( )
    {
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    CDesCArrayFlat* inEffectKeystrokeArray = popup->InEffectKeystrokeArray();
    TInt count = inEffectKeystrokeArray->Count() - 1;
    TInt checkCount = 0;
    for ( TInt i = count; i > 0 ; i-- )
      {
      if ( 0 == inEffectKeystrokeArray->MdcaPoint( i ).Compare( KPinyinStar ) )
        {
        inEffectKeystrokeArray->Delete( i );
        inEffectKeystrokeArray->Compress();
        checkCount = checkCount + 1;
        }
      else
        {
        break;
        }
      }
    
    if ( checkCount > 0 )
        {
        if ( inEffectKeystrokeArray->Count() > 0 )
            {
            inEffectKeystrokeArray->Delete( inEffectKeystrokeArray->Count() -1 );
            }
        iOwner->PtiEngine()->DeleteKeyPress();
        }
    }

// ---------------------------------------------------------
// get the valid keystroke
// ---------------------------------------------------------
//
void TAknFepInputStateCandidateBaseChinesePhrase::CheckTonemarkL( )
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine( );
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow( );
    CDesCArrayFlat* inEffectKeystrokeArray = popup->InEffectKeystrokeArray( );
    CDesCArrayFlat* spelling = popup->OptimizeSpelling( );

    TInt keystrokeStarCount = 0;
    TInt validSpellCount = 0;
    TInt i = 0;
    TBuf<1> buf;

    TBuf<1> lastSpellTonemark;
    TBuf<1> tonmarkBuf0;
    TBuf<1> tonmarkBuf1;
    TBuf<1> tonmarkBuf2;
    TBuf<1> tonmarkBuf3;
    TBuf<1> tonmarkBuf4;
    TBuf<KMaxPinyinLength> spellBuf;
    tonmarkBuf0.Append( KPinyinTone0Valid );
    tonmarkBuf1.Append( KPinyinTone1Valid );
    tonmarkBuf2.Append( KPinyinTone2Valid );
    tonmarkBuf3.Append( KPinyinTone3Valid );
    tonmarkBuf4.Append( KPinyinTone4Valid );

    if ( 0 == inEffectKeystrokeArray->Count( ) )
        {
        return;
        }

    for (i = inEffectKeystrokeArray->Count( ) - 1; i >= 0; i-- )
        {
        if ( 0 == inEffectKeystrokeArray->MdcaPoint( i ).Compare( KPinyinStar ) )
            {
            keystrokeStarCount = keystrokeStarCount + 1;
            }
        else
            {
            break;
            }
        }

    if ( 0 == keystrokeStarCount )
        {
        return;
        }

    switch ( keystrokeStarCount )
        {
        case KStarCount1:
            buf.Copy( tonmarkBuf0 );
            break;
        case KStarCount2:
            buf.Copy( tonmarkBuf1 );
            break;
        case KStarCount3:
            buf.Copy( tonmarkBuf2 );
            break;
        case KStarCount4:
            buf.Copy( tonmarkBuf3 );
            break;
        case KStarCount5:
            buf.Copy( tonmarkBuf4 );
            break;
        default:
            break;
        }

    spelling->Reset( );
    spelling->Compress( );

    ptiengine->GetPhoneticSpellingsL( *spelling );

    for (i = 0; i < spelling->Count( ); ++i )
        {
        spellBuf.Copy( spelling->MdcaPoint( i ) );
        TInt position = spellBuf.Find( KPinyinListSeparator );
        if ( KErrNotFound == position )
            {
            lastSpellTonemark.Copy( spellBuf.Mid( spellBuf.Length( ) - 1, 1 ) );
            if ( 0 == lastSpellTonemark.Compare( buf ) )
                {
                validSpellCount = 1;
                break;
                }
            }
        }

    spelling->Reset( );
    spelling->Compress( );

    if ( validSpellCount > 0 )
        {
        return;
        }

    for (i = 0; i < keystrokeStarCount; ++i )
        {
        inEffectKeystrokeArray->Delete( inEffectKeystrokeArray->Count( ) -1 );
        }

    ptiengine->DeleteKeyPress( );

    if ( inEffectKeystrokeArray->Count( ) > 0 )
        {
        inEffectKeystrokeArray->Delete( inEffectKeystrokeArray->Count( ) - 1 );
        }
    inEffectKeystrokeArray->Compress( );

    ptiengine->DeleteKeyPress( );

    while (inEffectKeystrokeArray->Count( ) > 0 )
        {
        spelling->Reset( );
        spelling->Compress( );
        ptiengine->GetPhoneticSpellingsL( *spelling );

        for (i = 0; i < spelling->Count( ); ++i )
            {
            if ( KErrNotFound == spelling->MdcaPoint( i ).Find( KPinyinListSeparator ) )
                {
                spelling->Reset( );
                spelling->Compress( );
                return;
                }
            }
        ptiengine->DeleteKeyPress( );
        inEffectKeystrokeArray->Delete( inEffectKeystrokeArray->Count( ) - 1 );
        }

    spelling->Reset( );
    spelling->Compress( );
    }

TBool TAknFepInputStateCandidateBaseChinesePhrase::IsValid( const TDesC& aDesC, TInt aCount )
    {
    TBool lastIsToneMark = EFalse;
    TBool ret = ETrue;
    TInt charCount = aCount;
    for ( TInt i = 0; i < aDesC.Length(); i++ )
        {
        if ( ( aDesC.Mid( i, 1 ) == KPinyinSysSeparator ) ||
             ( aDesC.Mid( i, 1 ) == KPinyinListSeparator ) ||
             ( *aDesC.Mid( i , 1 ).Ptr() == KPinyinTone0Valid ) ||
             ( *aDesC.Mid( i , 1 ).Ptr() == KPinyinTone1Valid ) ||
             ( *aDesC.Mid( i , 1 ).Ptr() == KPinyinTone2Valid ) ||
             ( *aDesC.Mid( i , 1 ).Ptr() == KPinyinTone3Valid ) ||
             ( *aDesC.Mid( i , 1 ).Ptr() == KPinyinTone4Valid ) )
            {
            if ( lastIsToneMark && charCount <= 0 )
                {
                ret = EFalse;
                }
            lastIsToneMark = ETrue;
            charCount--;
            }
        else
            {
            lastIsToneMark = EFalse;
            }
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateCandidateBaseChinesePhrase::HandleCommitL
// Handle CBA1 or select event.
// ---------------------------------------------------------------------------
//
void TAknFepInputStateCandidateBaseChinesePhrase::HandleCommitL()
    {
    // it may be one of the 'valid' numbers..
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlCandidatePane* candidatePane =
        UIContainer()->CandidatePane();
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    TPtrC text = candidatePane->CurrentPhraseCandidate();
    if ( text.Length() )
        {
        //commit and show char in editor
        if ( fepMan->IsFlagSet( CAknFepManager::EFlagEditorFull ) )
            {
            fepMan->ClearFlag( CAknFepManager::EFlagEditorFull );
            iOwner->FepMan()->TryCloseUiL();
            }
        else
            {
            TInt i = 0;
            CDesCArrayFlat* keystroke =
                UIContainer()->PinyinPopupWindow()->KeystrokeArray();
            if ( keystroke->Count() != 0 && keystroke->Count()
                > text.Length() )
                {
                TBuf<KMaxKeystrokeCount> seq;
                for (i = 0; i < keystroke->Count(); ++i )
                    {
                    seq.Append( keystroke->MdcaPoint( i ) );
                    }
                TInt count = 0;

                TBufC<KMaxKeystrokeCount> buf1;
                TPtr ptr1 = buf1.Des();
                TBufC<KMaxKeystrokeCount> keyseqBuf;
                TPtr ptr2 = keyseqBuf.Des();
                TBuf<KMaxKeystrokeCount> ptr2Left;
                ptr2.Copy( seq );
                if ( ptiengine->SelectedPhoneticSpelling() == 1 )
                	{
                    ptr1 = ptiengine->GetPhoneticSpelling(1);
                	}
                else
                	{
                    UIContainer()->InputPane()->GetText( ptr1 );
                	}
                TInt pos = 0;
                TInt len = 0;

                CDesCArray
                    * chineseCharacterArrayKeystroke =
                        UIContainer()->PinyinPopupWindow()->ChooseChineseCharacterArrayKeystroke();
                chineseCharacterArrayKeystroke->Reset();
                for (i = 0; i < ptr1.Length() && count < text.Length()
                    && ptr2.Length() != 0; i++ )
                    {
                    if ( ptr1.Mid( i, 1 ) == KPinyinSysSeparator || ptr1.Mid(
                        i, 1 ) == KPinyinListSeparator )
                        {
                        pos = i - len - count;
                        if ( pos < 0 )
                            {
                            pos = 0;
                            }

                        if ( pos == ptr2.Length() )
                            {
                            pos = ptr2.Length() - 1;
                            }

                        if ( ptr2.Mid( pos, 1 ) == KPinyinListSeparator )
                            {
                            i++;
                            pos += 1;
                            chineseCharacterArrayKeystroke->AppendL(
                                ptr2.Left( pos ) );
                            ptr2 = ptr2.Mid( pos );
                            }
                        else
                            {
                            chineseCharacterArrayKeystroke->AppendL(
                                ptr2.Left( pos ) );
                            ptr2 = ptr2.Mid( pos );
                            }
                        len += pos;
                        count++;
                        }
                    else if ( *ptr1.Mid( i, 1 ).Ptr() == KPinyinTone0Valid )
                        {
                        pos = i + 1 - len - count;
                        len += pos;
                        ptr2Left.Zero();
                        ptr2Left.Copy( ptr2.Left( pos - 1 ) );
                        ptr2Left.Append( KPinyinTone0ValidStr );
                        chineseCharacterArrayKeystroke->AppendL( ptr2Left );
                        ptr2 = ptr2.Mid( pos );
                        count++;
                        }
                    else if ( *ptr1.Mid( i, 1 ).Ptr() == KPinyinTone1Valid )
                        {
                        pos = i + 2 - len - count;
                        len += pos - 1;
                        ptr2Left.Zero();
                        ptr2Left.Copy( ptr2.Left( pos - 2 ) );
                        ptr2Left.Append( KPinyinTone1ValidStr );
                        chineseCharacterArrayKeystroke->AppendL( ptr2Left );
                        ptr2 = ptr2.Mid( pos );
                        count++;
                        }
                    else if ( *ptr1.Mid( i, 1 ).Ptr() == KPinyinTone2Valid )
                        {
                        pos = i + 3 - len - count;
                        len += pos - 2;
                        ptr2Left.Zero();
                        ptr2Left.Copy( ptr2.Left( pos - 3 ) );
                        ptr2Left.Append( KPinyinTone2ValidStr );
                        chineseCharacterArrayKeystroke->AppendL( ptr2Left );
                        ptr2 = ptr2.Mid( pos );
                        count++;
                        }
                    else if ( *ptr1.Mid( i, 1 ).Ptr() == KPinyinTone3Valid )
                        {
                        pos = i + 4 - len - count;
                        len += pos - 3;
                        ptr2Left.Zero();
                        ptr2Left.Copy( ptr2.Left( pos - 4 ) );
                        ptr2Left.Append( KPinyinTone3ValidStr );
                        chineseCharacterArrayKeystroke->AppendL( ptr2Left );
                        ptr2 = ptr2.Mid( pos );
                        count++;
                        }
                    else if ( *ptr1.Mid( i, 1 ).Ptr() == KPinyinTone4Valid )
                        {
                        pos = i + 5 - len - count;
                        len += pos - 4;
                        ptr2Left.Zero();
                        ptr2Left.Copy( ptr2.Left( pos - 5 ) );
                        ptr2Left.Append( KPinyinTone4ValidStr );
                        chineseCharacterArrayKeystroke->AppendL( ptr2Left );
                        ptr2 = ptr2.Mid( pos );
                        count++;
                        }
                    }

                TInt totalCount = 0;
                for (i = 0; i < ptr1.Length(); i++ )
                    {
                    if ( (ptr1.Mid( i, 1 ) == KPinyinSysSeparator)
                        || (ptr1.Mid( i, 1 ) == KPinyinListSeparator)
                        || (*ptr1.Mid( i, 1 ).Ptr() == KPinyinTone0Valid)
                        || (*ptr1.Mid( i, 1 ).Ptr() == KPinyinTone1Valid)
                        || (*ptr1.Mid( i, 1 ).Ptr() == KPinyinTone2Valid)
                        || (*ptr1.Mid( i, 1 ).Ptr() == KPinyinTone3Valid)
                        || (*ptr1.Mid( i, 1 ).Ptr() == KPinyinTone4Valid) )
                        {
                        totalCount++;
                        }
                    }

                if ( !((ptr1.Right( 1 ) == KPinyinListSeparator)
                    || (*ptr1.Right( 1 ).Ptr() == KPinyinTone0Valid)
                    || (*ptr1.Right( 1 ).Ptr() == KPinyinTone1Valid)
                    || (*ptr1.Right( 1 ).Ptr() == KPinyinTone2Valid)
                    || (*ptr1.Right( 1 ).Ptr() == KPinyinTone3Valid)
                    || (*ptr1.Right( 1 ).Ptr() == KPinyinTone4Valid)) )
                    {
                    totalCount++;
                    }

                if ( totalCount > text.Length() )
                    {
                    CDesCArray
                        * chineseCharacterArray =
                            UIContainer()->PinyinPopupWindow()->ChooseChineseCharacterArray();
                    chineseCharacterArray->Reset();
                    for (i = 0; i < text.Length(); i++ )
                        {
                        chineseCharacterArray->AppendL( text.Mid( i, 1 ) );
                        }
                    //set key stroke
                    CDesCArray* keyStroke =
                        UIContainer()->PinyinPopupWindow()->KeystrokeArray();
                    keyStroke->Reset();
                    for (i = 0; i < ptr2.Length(); i++ )
                        {
                        keyStroke->AppendL( ptr2.Mid( i, 1 ) );
                        }
                    //set show stroke                               
                    CDesCArray
                        * showStroke =
                            UIContainer()->PinyinPopupWindow()->ShowKeystrokeArray();
                    showStroke->Reset();
                    TInt pinyinStarCount = 0;
                    for (i = 0; i < ptr2.Length(); )
                        {
                        pinyinStarCount = 0;
                        if ( ptr2.Mid( i, 1 ) == KPinyinStar )
                            {
                            while (i < ptr2.Length() && ptr2.Mid( i, 1 )
                                == KPinyinStar )
                                {
                                pinyinStarCount++;
                                i++;
                                }
                            switch ( pinyinStarCount )
                                {
                                case 1:
                                    showStroke->AppendL( KPinyinTone0ValidStr );
                                    break;
                                case 2:
                                    showStroke->AppendL( KPinyinTone1ValidStr );
                                    break;
                                case 3:
                                    showStroke->AppendL( KPinyinTone2ValidStr );
                                    break;
                                case 4:
                                    showStroke->AppendL( KPinyinTone3ValidStr );
                                    break;
                                case 5:
                                    showStroke->AppendL( KPinyinTone4ValidStr );
                                    break;
                                default:
                                    break;
                                }
                            }
                        else
                            {
                            showStroke->AppendL( ptr2.Mid( i, 1 ) );
                            i++;
                            }
                        }
                    fepMan->PinyinPhraseCreation( ETrue );
                    ClearPtiEngineKeystroke();
                    GetValidkeystrokeL();
                    CheckTonemarkL();

                    CDesCArrayFlat* inEffectKeystrokeArray =
                        popup->InEffectKeystrokeArray();

                    if ( inEffectKeystrokeArray->Count() == 0 || !IsValid(
                        ptr1, text.Length() ) )
                        {
                        UIContainer()->SetControlInVisible( EFalse );
                        popup->SetPhraseCreationState( ETrue );
                        iOwner->ChangeState( EKeySequenceEdit );
                        }
                    else
                        {
                        popup->SetPhraseCreationState( ETrue );
                        iOwner->ChangeState( ESpellingSelection );
                        }
                    }
                else
                    {
                    CDesCArray* keyStroke =
                        UIContainer()->PinyinPopupWindow()->KeystrokeArray();
                    keyStroke->Reset();
                    //commit and show char in editor
                    fepMan->NewTextL( text );
                    fepMan->CommitInlineEditL();
                    fepMan->PinyinPhraseCreation( EFalse );
                    if ( fepMan->IsFlagSet( CAknFepManager::EFlagEditorFull ) )
                        {
                        fepMan->ClearFlag( CAknFepManager::EFlagEditorFull );
                        iOwner->FepMan()->TryCloseUiL();
                        }
                    else
                        {
                        iOwner->ChangeState( EPredictiveCandidate );
                        }
                    }
                }
            else
                {
                CDesCArray* keyStroke =
                    UIContainer()->PinyinPopupWindow()->KeystrokeArray();
                keyStroke->Reset();
                //commit and show char in editor
                fepMan->NewTextL( text );
                fepMan->CommitInlineEditL();
                fepMan->PinyinPhraseCreation( EFalse );
                if ( fepMan->IsFlagSet( CAknFepManager::EFlagEditorFull ) )
                    {
                    fepMan->ClearFlag( CAknFepManager::EFlagEditorFull );
                    iOwner->FepMan()->TryCloseUiL();
                    }
                else
                    {
                    iOwner->ChangeState( EPredictiveCandidate );
                    }
                }
            }
        }
    else
        {
        // No candidates available. Back to Entry state.
        iOwner->ChangeState( EEntry );
        }
    }

void TAknFepInputStateCandidateBaseChinesePhrase::GetNewCandidatesL(const TDesC& aSource, 
                                                    RPointerArray<HBufC>& aList)
    {
    TInt start = 0;
    TInt length = 0;

    for (TInt ii = 0; ii < aSource.Length(); ii++)
        {
        if (aSource[ii] == KSegment)
            {
            TPtrC segment(aSource.Ptr() + start, length);
            aList.AppendL(segment.AllocL());
            start += (length + 1);
            length = 0;
            }
        else
            {
            length++;
            }
        }

    if (length)
        {
        TPtrC segm(aSource.Ptr() + start, length);
        aList.AppendL(segm.AllocL());
        }
    }
// End of file
