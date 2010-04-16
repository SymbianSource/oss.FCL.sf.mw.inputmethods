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
 *      Provides the TAknFepInputStateEntryZhuyinPhraseCreation methods.
 *
*/












#include <PtiEngine.h>                          //CPtiEngine
#include <avkon.rsg>
#include <AknFep.rsg>

#include "aknfepuiinputstatezhuyinphrasecreationbase.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlPinyinPopup.h"    //Pinyin phrase
#include "aknfepuictrleditpane.h" 
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlInputPane.h"
#include "aknfepzhuyinkeyhandlerinterface.h" //MZhuyinKeyHandler
#include "AknFepZhuyinAnalyser.h"
#include "aknfepzhuyinconvertion.h" //TZhuyinSymbolsMethods

_LIT( KStrokeManualSeparator, "\x002E");
_LIT( KToneMark1, "\x0020" );
_LIT( KToneMark2, "\x02CA" );
_LIT( KToneMark3, "\x02C7" );
_LIT( KToneMark4, "\x02CB" );
_LIT( KToneMark5, "\x02D9" );

const TInt KMaxBufLen = 100;
const TInt KMaxKeystrokeCount = 100;
const TInt KMaxInputLength = 31;
const TInt KMaxPhraseCreationCount = 7;

// ---------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------
//
TAknFepInputStateZhuyinPhraseCreationBase::TAknFepInputStateZhuyinPhraseCreationBase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer ) :
    TAknFepInputStateCandidateBase( aOwner, aUIContainer )
    {    
    iState = EEntry;

    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();

    uiContainer->FocusCandidatePane(EFalse);
    
    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                            uiContainer->CandidatePane()->IsFirstPage());
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;

    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    uiContainer->FocusCandidatePane(ETrue);
    candidatePane->SetHighlighted(ETrue);
    candidatePane->ShowCandidateOrdinals(EFalse);
    candidatePane->SelectFirstPhrase();
    }
    
TBool TAknFepInputStateZhuyinPhraseCreationBase::AddKeystrokeL( TInt aKey )
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* zhuyinSymbol = popup->ZhuyinSymbols();
    TInt chineseCount = popup->ChineseCharsDisped()->Count();
    TInt currentIndex = editPane->GetCursorIndexOfKeystroke();
    TBuf<1> buf;
    TBuf<KMaxBufLen> showbuf;
    showbuf = uiContainer->PinyinPopupWindow()->GetEEPContent();
    
    TInt ret = iOwner->ZhuyinKeyHandler()->GetZhuyinSymbol( (TPtiKey)aKey, buf );  
    TInt showbufLen = showbuf.Length();
    
    if ( showbufLen >= KMaxInputLength )
        {
        if ( ( 0 == currentIndex ) || ( 1 == ret ) )
            {
            iOwner->ZhuyinKeyHandler()->Reset();
            return EFalse;
            }
        return ReplaceKeystrokeL( currentIndex,
                                 chineseCount,
                                 buf,
                                 zhuyinSymbol
                                 );
        }
    
    if ( showbufLen == KMaxInputLength - 1 )
        {
        if ( 0 == currentIndex )
            {
            iOwner->ZhuyinKeyHandler()->Reset();
            return EFalse;
            }
        else if ( 1 == ret )
            {
            zhuyinSymbol->InsertL( currentIndex, buf );
            if ( CheckKeystrokeCountL( chineseCount, zhuyinSymbol ) )
                {
                editPane->SetCursorIndexOfKeystroke( currentIndex + 1 );
                return ETrue;
                }
            else
                {
                iOwner->ZhuyinKeyHandler()->Reset();
                zhuyinSymbol->Delete( currentIndex );
                return EFalse;
                }
            }
        else
            {
            return ReplaceKeystrokeL( currentIndex,
                                     chineseCount,
                                     buf,
                                     zhuyinSymbol
                                     );
            }
        }
    

    else if ( ( 0 != currentIndex ) &&
        ( KMaxInputLength <= showbufLen ) )
        {
        if ( 0 == ret )
            {
            zhuyinSymbol->Delete( currentIndex - 1 );
            zhuyinSymbol->InsertL( currentIndex - 1, buf );
            return ETrue;
            }
        else
            {
            iOwner->ZhuyinKeyHandler()->Reset();
            return EFalse;
            }
        }
    else if ( ( 0 == currentIndex ) && 
        ( KMaxInputLength > showbufLen ) ) 
        {
        zhuyinSymbol->InsertL( currentIndex, buf );
        editPane->SetCursorIndexOfKeystroke( currentIndex + 1 );
        return ETrue;
        }
    else if ( ( 0 != currentIndex ) && 
        ( KMaxInputLength > showbufLen ) ) 
        {
        if ( 0 == ret )
            {
            zhuyinSymbol->Delete( currentIndex - 1 );
            zhuyinSymbol->InsertL( currentIndex - 1, buf );
            return ETrue;
            }
        else if ( 1 == ret )
            {
            zhuyinSymbol->InsertL( currentIndex, buf );
            editPane->SetCursorIndexOfKeystroke( currentIndex + 1 );
            return ETrue;
            }
        }
    return ETrue;
    }

TBool TAknFepInputStateZhuyinPhraseCreationBase::AddToneMarkL()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    MZhuyinKeyHandler* zhuyinKeyHandler = iOwner->ZhuyinKeyHandler();
    CDesCArrayFlat* zhuyinSymbols = uiContainer->PinyinPopupWindow()->ZhuyinSymbols();
    
    TBuf<1> baseBuf;
    TBuf<1> buf;
    baseBuf.Zero();
    TInt strokePos = editPane->GetCursorIndexOfKeystroke();
    
    TBuf<KMaxBufLen> showbuf;
    showbuf = popup->GetEEPContent();
    
    if ( strokePos > 0 )
        {
        baseBuf.Append( zhuyinSymbols->MdcaPoint( strokePos - 1 ) );
        }
    TInt insert = zhuyinKeyHandler->GetToneMark( baseBuf, buf );
    if ( insert == 0 )
        {
        zhuyinSymbols->Delete ( strokePos - 1, 1 );
        zhuyinSymbols->InsertL(strokePos - 1, buf);
        }
    // for insert
    else if ( insert == 1)
        {
        if ( showbuf.Length() >= KMaxInputLength )
            {
            return EFalse;
            }
        zhuyinSymbols->InsertL(strokePos, buf);
        editPane->SetCursorIndexOfKeystroke(strokePos + 1);
        }
    return ETrue;
    }

TBool TAknFepInputStateZhuyinPhraseCreationBase::DeleteZhuyinSymbolOrChinese()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    CDesCArrayFlat* zhuyinSymbols = popup->ZhuyinSymbols();
    CDesCArrayFlat* chineseDisped = popup->ChineseCharsDisped();
    CDesCArrayFlat* zhuyinSymbolsForChinese = popup->ZhuyinSymbolsForChineseCharsDisped();
    TInt keyStrokePos = editPane->GetCursorIndexOfKeystroke();
    
    if ( 1 == keyStrokePos )
        {
        editPane->Reset();
        }
    
    if ( keyStrokePos != 0 )
        {
        TZhuyinSymbolsMethods::DeleteZhuyinSymbol(
            zhuyinSymbols,
            keyStrokePos);
            editPane->SetCursorIndexOfKeystroke(keyStrokePos-1);
        }
    else if ( chineseDisped->Count() )
        {
        TInt oldLen =  zhuyinSymbols->Count();
        TZhuyinSymbolsMethods::DeleteChineseChar(
            zhuyinSymbols,
            chineseDisped,
            zhuyinSymbolsForChinese
            );
        TInt newLen = zhuyinSymbols->Count();
        editPane->SetCursorIndexOfKeystroke(newLen - oldLen);
        }
    return ETrue;
    }

TBool TAknFepInputStateZhuyinPhraseCreationBase::UpdateEEPContent()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    CDesCArrayFlat* zhuyinSymbols = UIContainer()->PinyinPopupWindow()->ZhuyinSymbols();
    CDesCArrayFlat* chineseDisped = UIContainer()->PinyinPopupWindow()->ChineseCharsDisped();
    CAknFepZhuyinAnalyser* analyser = iOwner->ZhuyinAnalyser();
    CDesCArrayFlat* candidateList = UIContainer()->CandidatePane()->CandidateArray();
    TBuf<KMaxBufLen> buf;
    TBool isPhraseCreation = EFalse;
    TInt extLen = chineseDisped->Count() + zhuyinSymbols->Count() - KMaxInputLength;
    
    if ( extLen > 0 )
        {
        TInt keyStrokePos = UIContainer()->EditPaneWindow()->GetCursorIndexOfKeystroke();
        zhuyinSymbols->Delete ( zhuyinSymbols->Count() - extLen, extLen );
        if ( keyStrokePos > zhuyinSymbols->Count() )
            {
            UIContainer()->EditPaneWindow()->SetCursorIndexOfKeystroke( zhuyinSymbols->Count() );
            }
        }

    TBool valid = TZhuyinSymbolsMethods::UpdateEEPContent(
        ptiengine,
        analyser,
        zhuyinSymbols,
        chineseDisped,
        candidateList,
        buf,
        isPhraseCreation
        );

    if ( ( chineseDisped->Count() != 0 || zhuyinSymbols->Count() > 1 ) &&
        ( candidateList->Count() != 0 ) )
        {
        for ( TInt i = 0; i < candidateList->Count(); ++i )
            {
            if ( TZhuyinSymbolsMethods::IsZhuyinSymbol( candidateList->MdcaPoint( i ) ) )
                {
                candidateList->Delete( i, 1 );
                break;
                }
            }
        }
    UIContainer()->PinyinPopupWindow()->SetEEPContentValid(valid);
    UIContainer()->CandidatePane()->SplitPhraseCandidatesIntoPages();
    if ( !valid || candidateList->Count() == 0 )
        {
        DeleteSeparatorWhenInvaild( buf );
        valid = EFalse;
        }
    CheckShowInfo( buf, zhuyinSymbols );
    UIContainer()->PinyinPopupWindow()->SetEEPContent(buf);
    return valid;
    }
   
void TAknFepInputStateZhuyinPhraseCreationBase::UpdateIndicator()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    TInt candidateCount = candidatePane->CandidateArray()->Count();

    if ( candidatePane->IsFirstPage() || candidateCount== 0 )
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows( EFalse );
        }
    else
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows( ETrue );
        }

    if ( candidatePane->IsLastPage() || candidateCount== 0 )
        {
        uiContainer->CandidatePane()->ShowDownScrollArrows( EFalse );
        }
    else
        {
        uiContainer->CandidatePane()->ShowDownScrollArrows( ETrue );
        }
    }

// ---------------------------------------------------------
// Get high light begine position.
// ---------------------------------------------------------
//
TInt TAknFepInputStateZhuyinPhraseCreationBase::BegPosOfMoreThanSeven()
    {
    TInt i = 0;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    TBuf<KMaxBufLen> buf;
    buf = uiContainer->PinyinPopupWindow()->GetEEPContent();
    TInt characterCount = uiContainer->PinyinPopupWindow()->ChineseCharsDisped()->Count();
    TInt allTextCount = characterCount;

    // calculate the total number of character according to stroke
    for (i = 0; i < buf.Length() && allTextCount < KMaxPhraseCreationCount; i++ )
        {
        if ( ( buf.Mid( i, 1 ) == KStrokeManualSeparator )|| 
            ( buf.Mid( i, 1 ) == KToneMark1 ) ||
            ( buf.Mid( i, 1 ) == KToneMark2 ) ||
            ( buf.Mid( i, 1 ) == KToneMark3 ) ||
            ( buf.Mid( i, 1 ) == KToneMark4 ) ||
            ( buf.Mid( i, 1 ) == KToneMark5 ) )
            {
            allTextCount++;
            }
        }

    allTextCount++;
    
    if ( allTextCount > KMaxPhraseCreationCount )
        {
        return i;
        }
    else
        {
        return 0;
        }
    }

// ---------------------------------------------------------
// Set high light.
// ---------------------------------------------------------
//
void TAknFepInputStateZhuyinPhraseCreationBase::HighlightMoreThanSeven( TInt aBegPos )
    {
    MAknFepUICtrlEditPane* editPane = UIContainer()->EditPaneWindow();
    TBuf<KMaxBufLen> buf;
    buf = UIContainer()->PinyinPopupWindow()->GetEEPContent();
    TInt EndPos = buf.Length();
    editPane->SetHighlight( aBegPos, EndPos );
    }

// ---------------------------------------------------------
// replace the keystroke.
// ---------------------------------------------------------
//
TBool TAknFepInputStateZhuyinPhraseCreationBase::ReplaceKeystrokeL(
                                                TInt aIndex, 
                                                TInt aChineseCount,
                                                const TDesC& aSymbol, 
                                                CDesCArrayFlat* aZhuyinSymbol )
    {
    TBuf<1> bufTmp;
    bufTmp.Append( aZhuyinSymbol->MdcaPoint( aIndex - 1 ) );
    aZhuyinSymbol->Delete( aIndex - 1 );
    aZhuyinSymbol->InsertL( aIndex - 1, aSymbol );
    if ( CheckKeystrokeCountL( aChineseCount, aZhuyinSymbol ) )
        {
        return ETrue;
        }
    else
        {
        aZhuyinSymbol->Delete( aIndex - 1 );
        aZhuyinSymbol->InsertL( aIndex - 1, bufTmp );
        return EFalse;
        }
    }

void TAknFepInputStateZhuyinPhraseCreationBase::RefreshUI()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* zhuyinSymbols = uiContainer->PinyinPopupWindow()->ZhuyinSymbols();
    CDesCArrayFlat* chineseDisped = uiContainer->PinyinPopupWindow()->ChineseCharsDisped();
    
    CDesCArrayFlat* candidateList = uiContainer->CandidatePane()->CandidateArray();
    
    // get cursor position
    TPoint baseLine;
    TInt height(0);
    TInt ascent(0);
    TRAPD(ret,iOwner->FepMan()->
        GetScreenCoordinatesL(baseLine,height,ascent));
    if ( ret == KErrNone )
        {
        uiContainer->SetContainerPosition( baseLine, height );
        }
    TBuf<KMaxBufLen> seed;
    TBool valid = ETrue;
    TBuf<KMaxBufLen> buf;
    buf = uiContainer->PinyinPopupWindow()->GetEEPContent();
    valid = uiContainer->PinyinPopupWindow()->GetEEPContentValid();

    TBool isHighlight = EFalse;
    TInt zhuyinBegPos = chineseDisped->Count();

   TRAP_IGNORE( isHighlight = !iOwner->ZhuyinAnalyser()->AnalyzeL( buf.Mid( zhuyinBegPos ) ) );
   CheckShowInfo( buf, zhuyinSymbols );
   
    TInt keyStrokePos = editPane->GetCursorIndexOfKeystroke();
    if ( keyStrokePos > zhuyinSymbols->Count() + 1 )
    	{
    	keyStrokePos = zhuyinSymbols->Count();
    	editPane->SetCursorIndexOfKeystroke( keyStrokePos );
    	}
    TInt newCursorPos = TZhuyinSymbolsMethods::RefreshCursorPos(
        chineseDisped->Count(),buf,keyStrokePos);
    
    TZhuyinSymbolsMethods::Convert2Des(zhuyinSymbols,seed);
    editPane->SetTextForZhuyin(buf,newCursorPos,chineseDisped->Count(),seed,keyStrokePos,!valid);
    if ( !valid || isHighlight
        || candidateList->Count()== 0 || zhuyinSymbols->Count() == 0 )
        {
        uiContainer->CandidatePane()->SetInVisiable();
        editPane->SetHighlight(chineseDisped->Count(),buf.Length()-1);
        uiContainer->SetControlInVisible( EFalse );
        uiContainer->Enable( ETrue );
        uiContainer->ShowHorizontalScrollArrows( EFalse );
        uiContainer->ShowVerticalScrollArrows( EFalse );   
        }
    else
        {
        uiContainer->SetControlInVisible( ETrue );
        uiContainer->CandidatePane()->SetCandidateBuffer();
        uiContainer->CandidatePane()->SelectFirstPhrase();
        uiContainer->Enable( ETrue );
        uiContainer->ShowVerticalScrollArrows( ETrue );
        }
    TInt begPos = BegPosOfMoreThanSeven();

    if ( begPos )
        {
        HighlightMoreThanSeven( begPos );
        }

    UpdateIndicator();
    }

// ---------------------------------------------------------
// Check the keystroke count.
// ---------------------------------------------------------
//
TBool TAknFepInputStateZhuyinPhraseCreationBase::CheckKeystrokeCountL(
                                              TInt aChineseCount,
                                              CDesCArrayFlat* aZhuyinSymbol )
    {
    TBuf<KMaxKeystrokeCount> inputBuf;
    
    for (TInt i = 0; i < aZhuyinSymbol->Count(); ++i )
        {
        inputBuf.Append( aZhuyinSymbol->MdcaPoint( i ) );
        }
    
    if ( !iOwner->ZhuyinAnalyser()->AnalyzeL( inputBuf ) )
        {
        return ETrue;
        }
    else
        {
        HBufC* showBuf = iOwner->ZhuyinAnalyser()->LastAnalysisResult();
        if ( showBuf->Length() > KMaxInputLength - aChineseCount )
            {
            return EFalse;
            }        
        }  
    return ETrue;
    }

void TAknFepInputStateZhuyinPhraseCreationBase::HandleCommandL(TInt /*aCommandId*/)
    {   
    RefreshUI();    
    }

void TAknFepInputStateZhuyinPhraseCreationBase::CheckShowInfo( TDes& aOutDes, 
                                            CDesCArrayFlat* aZhuyinSymbol )
    {
    if( aOutDes.Length() <= KMaxInputLength )
        {
        return;
        }
    TInt extLen = aOutDes.Length() - KMaxInputLength;
    TInt delCount = 0;
    TBuf<KMaxInputLength> delBuf;
    delBuf.Copy( aOutDes.Mid( KMaxInputLength ) );
    aOutDes.Delete( KMaxInputLength, extLen );
    if ( aOutDes.Mid( KMaxInputLength - 1, 1 ).
        Compare( KStrokeManualSeparator ) == 0 )
        {
        aOutDes.Delete( KMaxInputLength - 1, 1 );
        }
    
    for ( TInt i = 0; i < delBuf.Length(); i++ )
        {
        if ( delBuf.Mid( i, 1 ).Compare( KStrokeManualSeparator ) != 0 )
            {
            delCount++;
            }
        }
    
    for ( TInt i = 0; i < delCount; i++ )
        {
        aZhuyinSymbol->Delete( aZhuyinSymbol->Count() - 1 );
        }
    }

// ---------------------------------------------------------
// Change CBA
// ---------------------------------------------------------
//
void TAknFepInputStateZhuyinPhraseCreationBase::ChangeCbaL()
    {
    MAknFepUICtrlPinyinPopup* popup = UIContainer()->PinyinPopupWindow();
    CDesCArrayFlat* zhuyinSymbol = popup->ZhuyinSymbols();
    CDesCArrayFlat* chineseArray = popup->ChineseCharsDisped( );
    CDesCArrayFlat* candidateArray = UIContainer()->CandidatePane()->CandidateArray( );
    TBool vaild = popup->GetEEPContentValid( );
    TInt lastRes = popup->GetLastResouce();

    if ( chineseArray->Count( ) == KMaxPhraseCreationCount )
        {
        if ( lastRes == R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE )
            {
            return;
            }
        //User cba done-done-cancel
        iOwner->FepMan()->UpdateCbaL( R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE );
        popup->SetLastResouce( R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE );
        }
    else if ( ( 0 != zhuyinSymbol->Count( ) ) && (candidateArray->Count( )
        != 0 ) && vaild )
        {
        if ( lastRes == R_AKNFEP_SOFTKEYS_PHRASE_CREATION_SELECT_CANCEL_SELECT )
            {
            return;
            }
        //User cba select-select-cancel
        iOwner->FepMan()->UpdateCbaL( R_AKNFEP_SOFTKEYS_PHRASE_CREATION_SELECT_CANCEL_SELECT );
        popup->SetLastResouce( R_AKNFEP_SOFTKEYS_PHRASE_CREATION_SELECT_CANCEL_SELECT );
        }
    else if ( (chineseArray->Count( ) > 1 )
        && ( 0 == zhuyinSymbol->Count( ) ) )
        {
        if ( lastRes == R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE )
            {
            return;
            }
        //User cba Done-Done-Cancel
        iOwner->FepMan()->UpdateCbaL( R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE );
        popup->SetLastResouce( R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE );
        }
    else
        {
        if ( lastRes == R_AKNFEP_SOFTKEYS__PHRASE_CREATION_EMPTY_CANCEL_EMPTY )
            {
            return;
            }
        iOwner->FepMan()->UpdateCbaL( R_AKNFEP_SOFTKEYS__PHRASE_CREATION_EMPTY_CANCEL_EMPTY );
        popup->SetLastResouce( R_AKNFEP_SOFTKEYS__PHRASE_CREATION_EMPTY_CANCEL_EMPTY );
        }
    }

void TAknFepInputStateZhuyinPhraseCreationBase::DeleteSeparatorWhenInvaild( TDes& aDes )
    {
    TInt count = aDes.Length();
    for ( TInt i = count - 1; i > 0; i-- )
        {
        if ( aDes.Mid( i, 1 ).Compare( KStrokeManualSeparator ) == 0 )
            {
            aDes.Delete( i, 1 );
            }
        }
    }

// End of file
