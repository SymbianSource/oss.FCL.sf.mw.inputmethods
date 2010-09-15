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
 *      Provides the TAknFepInputMiniQwertySogouPinyinPhrasePlugin definition.
 *
*/

// System includes
#include <e32cmn.h>
#include <PtiEngine.h>
#include <PtiDefs.h>
#include <avkon.rsg>
#include <aknfep.rsg>
#include <PtiUserDicEntry.h>
#include <aknnotewrappers.h> //CAknWarningNote 
// User includes
#include "AknFepUIManagerStateInterface.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepUICtrlPinyinPopup.h"        //Pinyin phrase
#include "aknfepuictrleditpane.h"           //pinyin phrase creation
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepManager.h"

#include "aknfepuiinputminiqwertysogoupinyinphraseplugin.h"
#include "AknFepUiInputStateCandidateQwertyBaseChinesePhrase.h"

// Constant definition
const TInt KMaxPhraseCreationCount = 10;
const TInt KMaxKeystrokeCount = 31;
const TInt KMaxShowKeystrokeCount = 255;
const TInt KInvalidToneMark = -1;
const TUint16 KAutoDLT = 0x002E;
const TUint16 KManualDLT = 0x0027;
const TUint16 KPYa = 0x0061;
const TUint16 KPYb = 0x0062;
const TUint16 KPYc = 0x0063;
const TUint16 KPYd = 0x0064;
const TUint16 KPYe = 0x0065;
const TUint16 KPYf = 0x0066;
const TUint16 KPYg = 0x0067;
const TUint16 KPYh = 0x0068;
const TUint16 KPYi = 0x0069;
const TUint16 KPYj = 0x006A;
const TUint16 KPYk = 0x006B;
const TUint16 KPYl = 0x006C;
const TUint16 KPYm = 0x006D;
const TUint16 KPYn = 0x006E;
const TUint16 KPYo = 0x006F;
const TUint16 KPYp = 0x0070;
const TUint16 KPYq = 0x0071;
const TUint16 KPYr = 0x0072;
const TUint16 KPYs = 0x0073;
const TUint16 KPYt = 0x0074;
const TUint16 KPYu = 0x0075;
const TUint16 KPYv = 0x0076;
const TUint16 KPYw = 0x0077;
const TUint16 KPYx = 0x0078;
const TUint16 KPYy = 0x0079;
const TUint16 KPYz = 0x007A;

const TMiniQwertyKeyMap StrokeMap[] =
    {
    {KPYa, EPtiKeyQwertyA},
    {KPYb, EPtiKeyQwertyB},
    {KPYc, EPtiKeyQwertyC},
    {KPYd, EPtiKeyQwertyD},
    {KPYe, EPtiKeyQwertyE},
    {KPYf, EPtiKeyQwertyF},
    {KPYg, EPtiKeyQwertyG},
    {KPYh, EPtiKeyQwertyH},
    {KPYi, EPtiKeyQwertyI},
    {KPYj, EPtiKeyQwertyJ},
    {KPYk, EPtiKeyQwertyK},
    {KPYl, EPtiKeyQwertyL},
    {KPYm, EPtiKeyQwertyM},
    {KPYn, EPtiKeyQwertyN},
    {KPYo, EPtiKeyQwertyO},
    {KPYp, EPtiKeyQwertyP},
    {KPYq, EPtiKeyQwertyQ},
    {KPYr, EPtiKeyQwertyR},
    {KPYs, EPtiKeyQwertyS},
    {KPYt, EPtiKeyQwertyT},
    {KPYu, EPtiKeyQwertyU},
    {KPYv, EPtiKeyQwertyV},
    {KPYw, EPtiKeyQwertyW},
    {KPYx, EPtiKeyQwertyX},
    {KPYy, EPtiKeyQwertyY},
    {KPYz, EPtiKeyQwertyZ},
    {KManualDLT, EPtiKeyQwertySpace}
    };

const TMiniQwertyToneMap PinyinToneMap[] =
    {
    {0x02C9, 1},
    {0x02CA, 2},
    {0x02C7, 3},
    {0x02CB, 4},
    {0x02D9, 5}
    };

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertySogouPinyinPhrasePlugin::TAknFepInputMiniQwertySogouPinyinPhrasePlugin
// C++ default constructor
// ---------------------------------------------------------------------------
//
TAknFepInputMiniQwertySogouPinyinPhrasePlugin::TAknFepInputMiniQwertySogouPinyinPhrasePlugin(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer,
    TUIState aState )
    : iOwner( aOwner ),
      iUIContainer( aUIContainer ),
      iState( aState ),
      iValid( EFalse ),
      iNeedAddDLT( EFalse ),
      iCanConvertAll( ETrue ),
      iConvertCount( -1 )
    {
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertySogouPinyinPhrasePlugin::operator=
// 
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertySogouPinyinPhrasePlugin::operator=( const 
        TAknFepInputMiniQwertySogouPinyinPhrasePlugin& aPlugin )
    {
    Mem::Copy( this, &aPlugin, sizeof(TAknFepInputMiniQwertySogouPinyinPhrasePlugin));
    }


// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertySogouPinyinPhrasePlugin::AnalyseL
// Analyse key storke.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertySogouPinyinPhrasePlugin::AnalyseL()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlEditPane* editPane = iUIContainer->EditPaneWindow();
    
    //user pressed keystroke sequence
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    
    //stroke to be shown
    CDesCArrayFlat* showKeystroke = editPane->ShowKeystrokeArray();
    TInt start = 0;
    TInt toneMarkCount = KInvalidToneMark;
//    TInt delimiterPosition = 0;
    showKeystroke->Reset();
    TBuf<1> atuoDLT;
    TBuf<1> manualDLT;
    atuoDLT.Append( KAutoDLT );
    manualDLT.Append( KManualDLT );
    TBuf<KMaxKeystrokeCount> keyTmp;

    if ( keystroke->Count() == 0 )
        {
        return;
        }
    
    ClearPtiEngineKeystroke();
    
    TInt phraseCount = 0;
    CDesCArrayFlat* phrase = editPane->PhraseArray();
    for ( TInt j = 0;  j < phrase->Count(); j++ )
        {
        phraseCount += phrase->MdcaPoint(j).Length();;
        }
    
    // Analysis every keystroke
    TInt firstIsTonemark = EFalse;
    TBool lastOneIsTonemark = EFalse;
    for ( TInt i = start; i < keystroke->Count(); ++i )
        {
        // Detect which tonemark the keystroke is.
        toneMarkCount = ToneMark( keystroke->MdcaPoint( i ));

        // current keystroke is tonemark
        if ( KInvalidToneMark != toneMarkCount )
            {
            if ( i == 0 )
                {
                firstIsTonemark = ETrue;
                }
            // Analysis and add the tonemark
            TBool checkTonemark = CheckSpellingAddToneMarkToPTIL( toneMarkCount );
            if ( !checkTonemark || lastOneIsTonemark )
                {
                if ( lastOneIsTonemark && checkTonemark )
                    {
                    ptiengine->IncrementToneMark( ETrue );
                    }
                TPtrC ptr = getCurrentLeastDLTSpell();
                TBuf<1> tonemark;
                if ( ptr.Length() > 0 )
                    {
                    tonemark.Append( ptr[ ptr.Length() - 1 ] );
                    if ( lastOneIsTonemark )
                        {
                        keystroke->Delete( i - 1, 2 );
                        keystroke->InsertL( i - 1, tonemark );
                        i = i - 1;
                        }
                    else
                        {
                        keystroke->Delete( i );
                        keystroke->InsertL( i, tonemark );
                        }
                    }
                else if ( lastOneIsTonemark )
                    {
                    toneMarkCount = ToneMark( keystroke->MdcaPoint( i )) + 
                            ToneMark( keystroke->MdcaPoint( i - 1 ));
                    toneMarkCount = toneMarkCount % 
                            ( sizeof(PinyinToneMap ) / ( sizeof(PinyinToneMap[0])));
                    if ( toneMarkCount == 0 )
                        {
                        toneMarkCount = 5;
                        }
                    tonemark.Append( PinyinToneMap[toneMarkCount - 1].iValue );
                    keystroke->Delete( i - 1, 2 );
                    keystroke->InsertL( i - 1, tonemark );
                    i = i - 1;
                    }
                    
                }
            lastOneIsTonemark = ETrue;
            ptiengine->HandleCommandL( EPtiCommandResetToneMark );
            }
        else
            {
            if ( lastOneIsTonemark )
                {
                lastOneIsTonemark = EFalse;
                }
            TInt keyCode = 0;
            GetKeystrokeCode( keyCode, keystroke->MdcaPoint( i ));
            ptiengine->AppendKeyPress(( TPtiKey )keyCode );
            }
        
        // If the last keystroke is appended, get the spell from core and
        // add it to show keystroke.
        if ( i == keystroke->Count() - 1 )
            {
            TPtrC spell = getCurrentLeastDLTSpell();
            TInt dltCount = 0;
            for ( TInt j = 0; j < spell.Length(); j++ )
                {
                showKeystroke->AppendL( spell.Mid( j, 1 ));
                if ( KAutoDLT == spell[j] )
                    {
                    dltCount++;
                    }
                }
            TInt tmp = 0;
            if ( firstIsTonemark )
                {
                tmp = 1;
                }
            if ( spell.Length() - dltCount + start + tmp < keystroke->Count())
                {
                if ( spell.Length() != 0 )
                    {
                    showKeystroke->AppendL( atuoDLT );
                    }
                for ( TInt k = spell.Length() - dltCount; k < keystroke->Count(); k++ )
                    {
                    showKeystroke->AppendL( keystroke->MdcaPoint( k ));
                    }
                }
            else if ( spell.Length() - dltCount + start < keystroke->Count())
                {
                showKeystroke->InsertL( 0, keystroke->MdcaPoint( 0 ));
                }
            
            ClearPtiEngineKeystroke();
            }

        if ( phraseCount + showKeystroke->Count() > KMaxKeystrokeCount )
            {
            showKeystroke->Delete( KMaxKeystrokeCount - phraseCount, 
                  phraseCount + showKeystroke->Count() - KMaxKeystrokeCount );
            if ( CheckSpellingDLT( showKeystroke->MdcaPoint( 
                                      KMaxKeystrokeCount - phraseCount - 1 )))
                {
                showKeystroke->Delete( KMaxKeystrokeCount - phraseCount - 1 );
                }
            break;
            }
        }
    if ( iNeedAddDLT )
        {
        iNeedAddDLT = EFalse;
        }
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertySogouPinyinPhrasePlugin::ClearPtiEngineKeystroke
// clear the ptiengine keystroke;
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertySogouPinyinPhrasePlugin::ClearPtiEngineKeystroke()
    {
    iOwner->PtiEngine()->ClearCurrentWord();
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertySogouPinyinPhrasePlugin::GetKeystrokeCode
// get Code of the key stroke. 
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertySogouPinyinPhrasePlugin::GetKeystrokeCode( TInt& aKey,
    const TDesC& aKeystroke )
    {
    TInt count = sizeof(StrokeMap) / sizeof(StrokeMap[0]);
    for ( TInt i = 0; i < count; i++ )
        {
        TBuf<1> buf;
        buf.Append( StrokeMap[i].iValue );
        if ( 0 == aKeystroke.Compare( buf ))
            {
            aKey = StrokeMap[i].iKeyCode;
            break;
            }
        }
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertySogouPinyinPhrasePlugin::getCurrentLeastDLTSpell
// get current spell which contains the least DLT.
// ---------------------------------------------------------
//
TPtrC TAknFepInputMiniQwertySogouPinyinPhrasePlugin::getCurrentLeastDLTSpell()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    CDesC16ArrayFlat* allSpell = new CDesC16ArrayFlat( 1 );
    TPtrC  currentSpell;
    TBuf<1> delimiter;
    TBuf<KMaxKeystrokeCount> preDltPosizeArr;
    TBuf<KMaxKeystrokeCount> curDltPosizeArr;
    preDltPosizeArr.FillZ( KMaxKeystrokeCount );
    preDltPosizeArr.SetLength( KMaxKeystrokeCount );
    curDltPosizeArr.FillZ( KMaxKeystrokeCount );
    curDltPosizeArr.SetLength( KMaxKeystrokeCount );
    
    TInt index = 1;
    delimiter.Append( KManualDLT );
    TRAP_IGNORE( ptiengine->GetPhoneticSpellingsL( *allSpell ));
    TInt allCount = ptiengine->PhoneticSpellingCount();
    
    
    for( TInt i = 0; i < allCount; i++ )
        {
        currentSpell.Set(allSpell->MdcaPoint( i ));
        TInt dltIndex = 0;
        curDltPosizeArr.FillZ(KMaxKeystrokeCount);
        curDltPosizeArr.SetLength(KMaxKeystrokeCount);
        TInt preIndex = 0;
        for ( TInt j = 0; j < currentSpell.Length(); j++ )
            {
            if ( currentSpell.Mid( j, 1 ).Compare( delimiter ) == 0 )
                {   
                curDltPosizeArr[dltIndex] = j - preIndex - 1;
                preIndex = j;
                dltIndex++;
                }
            else if (( j + 1 ) == currentSpell.Length())
                {
                curDltPosizeArr[dltIndex] = j - preIndex;
                }
            }
        
        for ( TInt k = 0; k < KMaxKeystrokeCount; k++ )
            {
            
            if ( preDltPosizeArr[k] < curDltPosizeArr[k] )
                {
                preDltPosizeArr.Copy( curDltPosizeArr );
                index = i + 1;
                break;
                }
            else if ( preDltPosizeArr[k] > curDltPosizeArr[k] )
                {
                break;
                }
            }
        }
    
    // Pop and destroy allSpell
    if ( allSpell )
        {
        delete allSpell;
        }
    
    ptiengine->SelectPhoneticSpelling( index );
    return ptiengine->GetPhoneticSpelling( index ); 
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertySogouPinyinPhrasePlugin::CheckSpellingDLT
// Check the delimiter of spelling.
// ---------------------------------------------------------
//
TBool TAknFepInputMiniQwertySogouPinyinPhrasePlugin::CheckSpellingDLT(
    const TDesC& aKeystroke )
    {
    if ( aKeystroke.Length() == 0 )
        {
        return EFalse;
        }
    
    if (( KAutoDLT == aKeystroke[0] ) || ( KManualDLT == aKeystroke[0] ))
        {
        return ETrue;
        }
    
    return EFalse;
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertySogouPinyinPhrasePlugin::ToneMark
// Get pinyin tone mark.
// ---------------------------------------------------------
//
TInt TAknFepInputMiniQwertySogouPinyinPhrasePlugin::ToneMark( 
        const TDesC& aKeystroke )
    {
    if ( aKeystroke.Length() == 0 )
        {
        return KInvalidToneMark;
        }
    TInt numOfToneMark = sizeof(PinyinToneMap) / sizeof(PinyinToneMap[0]);
    for ( TInt i = 0; i < numOfToneMark; i++ )
        {
        if ( PinyinToneMap[i].iValue == aKeystroke[0] )
            {
            return PinyinToneMap[i].iInputTone;
            }
        }
    return KInvalidToneMark;
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertySogouPinyinPhrasePlugin::CheckSpellingAddToneMarkToPTIL
// Check spell after add the tonemark to ptiEngine.
// ---------------------------------------------------------
//
TBool TAknFepInputMiniQwertySogouPinyinPhrasePlugin::CheckSpellingAddToneMarkToPTIL(
                                                        const TInt aToneMark )
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlEditPane* editPane = iUIContainer->EditPaneWindow();
    TBuf<1> toneMark;
    TBuf<1> delimiter;
    
    toneMark.Append( PinyinToneMap[aToneMark - 1].iValue );
    delimiter.Append( KManualDLT );
    
    for ( TInt i = 0; i < aToneMark; ++i )
        {
        ptiengine->IncrementToneMark( ETrue );
        TPtrC ptr = getCurrentLeastDLTSpell();
        
        // Some tonemarks are invalid for some pinyin stroke, so we need check
        // the tonemark.
        if ( ptr.Length() == 0 )
            {
            break;
            }
        TBuf<1> tonemarkResult;
        tonemarkResult.Append( ptr.Mid( ptr.Length() - 1, 1 ));
        if ( i != aToneMark - 1 )
            {
            CheckResultOfToneMark( tonemarkResult , i );
            }

        if (( 0 == ptr.Mid( ptr.Length() - 1, 1 ).Compare( toneMark )) &&
            ( KErrNotFound == ptr.Find( delimiter ))
            )
            {
            return ETrue;
            }
        }
    
    return EFalse;
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertySogouPinyinPhrasePlugin::CheckResultOfToneMark
// Check the tonemark result.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertySogouPinyinPhrasePlugin::CheckResultOfToneMark( 
                                            const TDesC16& aDes, TInt& aIndex )
    {
    TInt tonemarkIndex = 0;
    TInt num = sizeof(PinyinToneMap )/( sizeof(PinyinToneMap[0]));
    for ( tonemarkIndex = 0; tonemarkIndex < num; ++tonemarkIndex )
        {
        TBuf<1> toneMark;
        toneMark.Append( PinyinToneMap[tonemarkIndex].iValue );
        if ( aDes.Compare( toneMark ) == 0 )
            {
            break;
            }
        }
    aIndex = tonemarkIndex;
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertySogouPinyinPhrasePlugin::Enable
// Enable or disable the plugin.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertySogouPinyinPhrasePlugin::Enable( TBool aValid )
    {
    iValid = aValid;
    }
        
// ---------------------------------------------------------
// TAknFepInputMiniQwertySogouPinyinPhrasePlugin::IsEnable
// Get the state of the plugin.
// ---------------------------------------------------------
//
TBool TAknFepInputMiniQwertySogouPinyinPhrasePlugin::IsEnable()
    {
    return iValid;
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertySogouPinyinPhrasePlugin::CommitInlineEEPL
// Commint text to EEP or editor
// ---------------------------------------------------------------------------
//
TBool TAknFepInputMiniQwertySogouPinyinPhrasePlugin::CommitInlineEEPL(
    const TDesC& aDes )
    {
    TInt charCount = aDes.Length();
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    MAknFepUICtrlEditPane* editpane = iUIContainer->EditPaneWindow();
    CDesCArrayFlat* showkeyStoke = editpane->ShowKeystrokeArray();
    CDesCArrayFlat* keyStoke = editpane->KeystrokeArray();
    CDesCArrayFlat* phraseArray = editpane->PhraseArray();
    CDesCArrayFlat* phraseShowKeyStroke = editpane->PhraseShowKeyStrokeArray();
    TBuf<KMaxKeystrokeCount> buf;
    TBuf<1> autoDLT;
    autoDLT.Append( KAutoDLT );

    if (phraseArray->Count() == 0)
        {
        editpane->SetPhraseCreationFlag(EFalse);
        }
    phraseArray->AppendL( aDes );

    // Calculate the count of show keystroke which need to be deleted.   
    TInt delCount = 0;
    for ( TInt i = 0; i < showkeyStoke->Count() && charCount
            && keyStoke->Count() != 0; i++ )
        {
        TPtrC showptr = showkeyStoke->MdcaPoint(i);
        TPtrC ptr = keyStoke->MdcaPoint( 0);
        if ( CheckSpellingDLT( showptr ) || ( KInvalidToneMark
                != ToneMark( showptr )))
            {
            // If the previous show key stroke is DLT or tonemark too, break.
            if ( i > 0 && 
                 ( CheckSpellingDLT( showkeyStoke->MdcaPoint( i - 1 )) || 
                 ( KInvalidToneMark != ToneMark( showkeyStoke->MdcaPoint( i - 1 )))))
                {
                break;
                }

            charCount--;
            }
        if ( showptr.Compare( autoDLT ) != 0 )
            {
            buf.Append( ptr );
            keyStoke->Delete( 0 );
            }
        delCount++;
        }
    
    // Delete the show keystroke
    for (TInt j = 0; j < delCount; j++)
        {
        showkeyStoke->Delete( 0 );
        }
    
    // Record the keystrokes relative with the commit word
    phraseShowKeyStroke->AppendL( buf );
    
    // For Sogou core, maybe one element of PhraseArray() contains two or
    // more characters.
    TInt phraseCount = 0;
    for ( TInt l = 0; l < phraseArray->Count(); l++ )
        {
        phraseCount += phraseArray->MdcaPoint(l).Length();
        }

    // Tell sogou core which phrase is selected. It is used for phrase creation.
    TBuf<KMaxPhraseCreationCount> phraseSelected;
    phraseSelected.FillZ();
    if ( aDes.Length() <= KMaxPhraseCreationCount )
        {
        phraseSelected.Append( aDes );
        iOwner->PtiEngine()->HandleCommandL( EPtiCommandSelectElement, 
            ( TAny* )( &phraseSelected ));
        }
    
    // If there is no keystroke left or character count reach the limitation
    if ( keyStoke->Count() == 0 /*|| phraseCount == KMaxPhraseCreationCount */)
        {
        // Though maybe one element of PhraseArray() contains two or more characters,
        // here we do not need to get the characters count. Because we will handle
        // the phrase here, not character.
        HBufC* phraseCreated = HBufC::NewLC( phraseCount );
        TPtr keyPtr = phraseCreated->Des();
        phraseCount = phraseArray->Count();
        
        for ( TInt ii = 0; ii < phraseCount; ++ii )
            {
            TPtrC ptrZi = phraseArray->MdcaPoint( ii );
            keyPtr.Append( ptrZi );
            }
        fepMan->NewTextL( phraseCreated->Des());
        fepMan->CommitInlineEditL();
        
        // Tell sogou core the phrase pinyin
        AddPhrasePinyinToPti();
        iOwner->PtiEngine()->SetPredictiveChineseChar( phraseCreated->Des());
        CleanupStack::PopAndDestroy();
        return EFalse;
        }

    editpane->SetCursorIndexOfKeystroke( 0 );
    editpane->DisableCursor();
    editpane->SetPhraseCreationFlag( ETrue );
    return ETrue;
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::AddPhrasePinyinToPti
// Add phrase to DB.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertySogouPinyinPhrasePlugin::AddPhrasePinyinToPti()
    {
    MAknFepUICtrlEditPane* editpane = iUIContainer->EditPaneWindow();
    CDesCArrayFlat* phraseShowKeyStroke = editpane->PhraseShowKeyStrokeArray();
    TBuf<KMaxShowKeystrokeCount> phraseCreatedPinYin;
    phraseCreatedPinYin.FillZ();
    for ( TInt i = 0; i < phraseShowKeyStroke->Count(); i++ )
        {
        TPtrC ptrPinYin = phraseShowKeyStroke->MdcaPoint(i);
        phraseCreatedPinYin.Append(ptrPinYin);
        }
    //Add the phrase pinyin to the DB by PTI Engine
    TRAP_IGNORE( iOwner->PtiEngine()->HandleCommandL( 
            EPtiCommandSetPinyinForLearnWord, (TAny*)( &phraseCreatedPinYin )));
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertySogouPinyinPhrasePlugin::GetCandidateL
// Get the candidate info.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertySogouPinyinPhrasePlugin::GetCandidateL()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlEditPane* editPane = iUIContainer->EditPaneWindow();
    CDesCArrayFlat* phraseCandidates = 
            iUIContainer->CandidatePane()->CandidateArray();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    CDesCArrayFlat* showKeystroke = editPane->ShowKeystrokeArray();
    TInt phraseCount = editPane->PhraseArray()->Count();
    phraseCandidates->Reset();
    TBuf<KMaxKeystrokeCount> buf;
    TBuf<1> delimiter;
    delimiter.Append( KManualDLT );
    TInt keyCode;
    TInt count = KInvalidToneMark;
    ClearPtiEngineKeystroke();
    
    if ( showKeystroke->Count() == 0 )
        {
        return;
        }
    
    // If all strokes are valid
    if ( iCanConvertAll )
        {
        AnalyseL();
        ptiengine->GetChinesePhraseCandidatesL( *phraseCandidates ); 
        return;
        }

    // If some characters are invalid, we only need to get the candidates of 
    // the strokes before the invalid character.
    ClearPtiEngineKeystroke();
    TInt autoDLT = 0;
    for ( TInt j = 0; j < iConvertCount; j++ )
        {
        if ( showKeystroke->MdcaPoint( j )[0] == KAutoDLT )
            {
            autoDLT++;
            }
        }
    for ( TInt i = 0; i < iConvertCount - autoDLT/*showKeystroke->Count()*/; ++i )
        {
        count = ToneMark( keystroke->MdcaPoint( i ) );
        if ( KInvalidToneMark != count )
            {
            for ( TInt j = 0; j < count; ++j )
                {
                ptiengine->IncrementToneMark( ETrue );
                }
            }
        else
            {
            GetKeystrokeCode( keyCode, keystroke->MdcaPoint( i ) );
            ptiengine->AppendKeyPress((TPtiKey)keyCode);
            }
        }

    getCurrentLeastDLTSpell();
    ptiengine->GetChinesePhraseCandidatesL( *phraseCandidates );
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertySogouPinyinPhrasePlugin::CanConvertAll
// Set flag for convert all stroke.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertySogouPinyinPhrasePlugin::CanConvertAll( TBool aEnable )
    {
    iCanConvertAll = aEnable;
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertySogouPinyinPhrasePlugin::ConvertCount
// Set convert count.
// ---------------------------------------------------------
//           
void TAknFepInputMiniQwertySogouPinyinPhrasePlugin::ConvertCount( TInt aCount )
    {
    iConvertCount = aCount;
    }

// End of file
