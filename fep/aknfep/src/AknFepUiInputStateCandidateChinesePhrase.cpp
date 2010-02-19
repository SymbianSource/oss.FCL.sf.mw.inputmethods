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
* Description:            Implementation of candidate chinese phrase state
*
*/












#include "AknFepUiInputStateCandidateChinesePhrase.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUIManagerStateInterface.h"  //MAknFepUIManagerStateInterface
#include "AknFepUICtrlPinyinPopup.h"        //Pinyin phrase
#include "aknfepuictrleditpane.h"
#include "AknFepUICtrlCandidatePane.h"      //MAknFepUICtrlCandidatePane
#include <PtiEngine.h>                      //CPtiEngine
#include <aknnotewrappers.h>
#include "aknfepzhuyinconvertion.h"
#include "aknfepzhuyinkeyhandlerinterface.h"
#include <PtiUserDicEntry.h>

const TInt KMaxBufLen = 100;
const TInt KInputPhraseMaxLength = 7;
const TInt KMaxPinYinSpellLength = 7;//max pinyin length for one chinese Zi

_LIT( KMaxPhraseNote,"\x8BCD\x7EC4\x6700\x957F\x4E3A\x4E03\x5B57" );
///////////////////
// Candidate state
///////////////////

TAknFepInputStateCandidateChinesePhrase::TAknFepInputStateCandidateChinesePhrase(
                               MAknFepUIManagerStateInterface* aOwner,
                               MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateCandidateBaseChinesePhrase(aOwner, aUIContainer)
    {
    iState = ECandidate;
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    
    uiContainer->CandidatePane()->SelectFirstPhrase();

    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() &&
                            uiContainer->CandidatePane()->IsFirstPage());
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;

    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    uiContainer->FocusCandidatePane(ETrue);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(ETrue);    
    UpdateIndicator();
    UIContainer()->EditPaneWindow()->DisableCursor();
    }

TBool TAknFepInputStateCandidateChinesePhrase::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();
    MAknFepUICtrlPinyinPopup* popup = uiContainer->PinyinPopupWindow();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    
    CDesCArrayFlat* zhuyinSymbols = uiContainer->PinyinPopupWindow()->ZhuyinSymbols();
    CDesCArrayFlat* chineseDisped = uiContainer->PinyinPopupWindow()->ChineseCharsDisped();
    CDesCArrayFlat* zhuyinSymbolsForChinese = uiContainer->PinyinPopupWindow()->ZhuyinSymbolsForChineseCharsDisped();
    CDesCArrayFlat* candidateList = uiContainer->CandidatePane()->CandidateArray();
    
    MZhuyinKeyHandler* zhuyinKeyHandler = iOwner->ZhuyinKeyHandler();
    
    TInt index = MapKeyToIndex(aKey);
    
    if(aKey == EKeyBackspace)
        {
        zhuyinKeyHandler->SetStatusChgFlag(MZhuyinKeyHandler::EFromCandidateToEntry);
        iOwner->ChangeState(EEntry);
        return ETrue;
        }
    
    // for fix bug when press left or right arrow play err-tone
    if ( HandleVerticalNavigation( aKey ) || HandleHorizontalNavigation( aKey ) )
        {
        return ETrue;
        }
    
    if ( aLength == EShortKeyPress && ( candidatePane->SelectIndex(index,EFalse) 
        || aKey == EKeyOK || aKey ==  EKeyCBA1 ))
        {
        TPtrC text = candidatePane->CurrentPhraseCandidate();//current selected in candidates
        if ( text.Length() == 0 )
            {
            return ETrue;
            }
        TBuf<KMaxBufLen> pinyinshown = UIContainer()->PinyinPopupWindow()->GetEEPContent();//current selected spelling
        TInt count = TZhuyinSymbolsMethods::GetChineseCount(pinyinshown);//zhuyin group num
        if ( count > text.Length() )
            {
            fepMan->PhraseCreation(ETrue);
            }
        if ( fepMan->IsPhraseCreation( ) )
            {
            TBuf<KMaxBufLen> buf;
            buf.Copy( text );
            TBool commit = TZhuyinSymbolsMethods::AppendChineseChar( buf,
                fepMan->ZhuyinAnalyser( ), zhuyinSymbols, chineseDisped,
                zhuyinSymbolsForChinese );
            TBool changeState = UpdateEEPContent();
            if ( commit )
                {
                TBuf<KMaxBufLen> commitbuf;
                TZhuyinSymbolsMethods::Convert2Des(chineseDisped, commitbuf);
                fepMan->NewTextL(commitbuf);
                fepMan->CommitInlineEditL();

				//we pass zhuyin-spelling to db together with phrase.
                //chineseDisped is created phrase
                //zhuyinSymbolsForChinese is spelling
				TBuf<(1+KMaxPinYinSpellLength)*KInputPhraseMaxLength> phraseCreatedWithZhuYin;//(Zi+zhuyin)* max_Zi
				phraseCreatedWithZhuYin.FillZ();
				for (TInt i = 0; i < chineseDisped->Count(); i++)
					{
					TPtrC ptrZi = chineseDisped->MdcaPoint(i);
					phraseCreatedWithZhuYin.Append(ptrZi);
					
					TPtrC ptrZhuYin = zhuyinSymbolsForChinese->MdcaPoint(i);
					phraseCreatedWithZhuYin.Append(ptrZhuYin);
					
					TInt zeroTail = (1+KMaxPinYinSpellLength)-(ptrZi.Length()+ptrZhuYin.Length());
					phraseCreatedWithZhuYin.AppendFill(0,zeroTail);
					
					}
				
                AddPhraseToDB(phraseCreatedWithZhuYin);
                if ( chineseDisped->Count() < KInputPhraseMaxLength ||
                    zhuyinSymbols->Count() == 0 )
                    {
                    iOwner->FepMan()->TryCloseUiL();
                    }
                else
                    {
                    CAknWarningNote* errnote = new( ELeave ) CAknWarningNote();
                    errnote->SetTone( CAknNoteDialog::EWarningTone );
                    errnote->ExecuteLD( KMaxPhraseNote );
                    fepMan->TryCloseUiL();
                    }
                popup->ResetStrokeArray();
                }
            else
                {
                editPane->SetCursorIndexOfKeystroke( 0 );
                if ( changeState )
                    {
                    for ( TInt i = 0; i < candidateList->Count(); i++ )
                        {
                        if ( TZhuyinSymbolsMethods::IsZhuyinSymbol( //0x3105~0x3129 is zhuyin symbol,not include tone
                            candidateList->MdcaPoint( i ) ) )
                            {
                            candidateList->Delete( i, 1 );
                            break;
                            }
                        }
                    if ( candidateList->Count() > 0 )
                        {
                        popup->SetChangeState( ETrue );
                        iOwner->ChangeState( EEntry );
                        }
                    else
                        {
                        popup->SetChangeState( ETrue );
                        iOwner->ChangeState( EZhuyinSequenceEdit );
                        }
                    }
                else
                    {
                    popup->SetChangeState( ETrue );
                    iOwner->ChangeState( EZhuyinSequenceEdit );
                    }
                }
            }
        else
            {
            TBuf<KMaxBufLen> textbuf;
            for ( TInt i = 0; i < chineseDisped->Count(); i++ )
                {
                textbuf.Append(chineseDisped->MdcaPoint(i));
                }
            textbuf.Append(text);
            fepMan->NewTextL(textbuf);
            fepMan->CommitInlineEditL();
            AddPhraseToDB(textbuf);
            popup->ResetStrokeArray();
            if ( text.Length() <= KInputPhraseMaxLength && !fepMan->IsPhraseCreation() )
                {
                if (fepMan->IsFlagSet(CAknFepManager::EFlagEditorFull))
                    {
                    fepMan->ClearFlag(CAknFepManager::EFlagEditorFull);
                    iOwner->FepMan()->TryCloseUiL();
                    }
                else
                    {
                    iOwner->ChangeState( EPredictiveCandidate );
                    }
                }
            else
                {
                fepMan->TryCloseUiL();
                }
            }
        }
    else if ( aKey == EPtiKeyStar )
        {
        return ETrue;
        }    
    else if ( aKey == EKeyCBA2 )
        {
        popup->ResetStrokeArray();
        iOwner->FepMan()->EntryPhrase( EFalse );
        iOwner->FepMan()->TryCloseUiL();
        }
    else if ( !iOwner->IsValidChineseInputKey(aKey) )
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        }
    return ETrue;
    }

TBool TAknFepInputStateCandidateChinesePhrase::UpdateEEPContent( )
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    CDesCArrayFlat* zhuyinSymbols = UIContainer()->PinyinPopupWindow()->ZhuyinSymbols();
    CDesCArrayFlat* chineseDisped = UIContainer()->PinyinPopupWindow()->ChineseCharsDisped();
    CAknFepZhuyinAnalyser* analyser = iOwner->ZhuyinAnalyser();
    CDesCArrayFlat* candidateList = UIContainer()->CandidatePane()->CandidateArray();
    TBuf<KMaxBufLen> buf;
    TBool isPhraseCreation = EFalse;
    TBool valid = TZhuyinSymbolsMethods::UpdateEEPContent(
        ptiengine,
        analyser,
        zhuyinSymbols,
        chineseDisped,
        candidateList,
        buf,
        isPhraseCreation
        );
    
    if ( chineseDisped->Count() != 0  || zhuyinSymbols->Count() > 1 )
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
    iOwner->FepMan()->PhraseCreation(isPhraseCreation);
    UIContainer()->PinyinPopupWindow()->SetEEPContent(buf);
    UIContainer()->PinyinPopupWindow()->SetEEPContentValid(valid);
    return valid;
    }
void TAknFepInputStateCandidateChinesePhrase::AddPhraseToDB( TDes& aPhrase )
    {
    TPtiUserDictionaryEntry addUdbEntry( aPhrase );
    //Add the phrase to the DB by PTI Engine
    iOwner->PtiEngine()->AddUserDictionaryEntry( addUdbEntry );
    }

// End of file
