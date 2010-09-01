/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Layout UI interface base class implementation
*
*/












#include <w32std.h>
#include <s32mem.h> 
#include <aknedsts.h>               //CAknEdwinState
#include <centralrepository.h>

#include "pluginfepmanagerbase.h"
#include "peninputimepluginjp.h"
#include <peninputclient.h>
#include <aknfeppeninputenums.h>
#include <AknFepInternalCRKeys.h>

// Implementation of Class CPluginFepManagerBase

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::CPluginFepManagerBase
// destructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPluginFepManagerBase::~CPluginFepManagerBase()
    {
    delete iRepository;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::OnInit
// Initalize UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::OnInit()
    {
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::RetrieveEditorMaxLength
// Explicitly get ICF max length from layout UI .
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::RetrieveEditorMaxLength()
    {
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::UpdateCaseMode
// Update local copy of case mode.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::UpdateCaseMode(TInt aCaseMode)
    {
    iLastCase = aCaseMode;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetNumberModeKeyMappingL
// Set layout UI current number mode.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetNumberModeKeyMappingL(
                                            TAknEditorNumericKeymap aAknEditorNumericKeymap)
    {
    SendCommandToServer(ECmdPenInputEditorNumericKeyMap, aAknEditorNumericKeymap);
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::HandleKeyL
// Handle key event.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPluginFepManagerBase::HandleKeyL(TInt aKey, TKeyPressLength aLength, TEventCode /*aEventCode*/)
    {
    return iHkbUi->HandleKeyL(aKey, aLength);
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::HandleCommandL
// Handle command come from FEP.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::HandleCommandL(TInt aCommandId)
    {
    SendCommandToServer(aCommandId);
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::HandleCommandL
// Handle command come from FEP.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::HandleCommandL(TInt aCommandId,TInt aParam)
    {
    switch(aCommandId)
        {
        case ECmdPenInputRange:
            iHkbMode = aParam;
            break;
        case ECmdPenInputPermittedRange:
            iLastCase = ECaseInvalide;
            SetLayoutPermitedRanges(aParam);
            break;
        case ECmdPenInputEditorState:
            SetLayoutRange(reinterpret_cast<CAknEdwinState*>(aParam));
            break;
        case ECmdPenInputSetWindowPos:
            SetLayoutPosition(*reinterpret_cast<TRect*>(aParam));
            break;
        case ECmdPenInputCaseMode:
            {
            SendCommandToServer(ECmdPenInputCase, aParam);
            iLastCase = aParam;
            }
            break;
        case ECmdPenInputEditorCustomNumericKeyMap:
            {
            TDesC* res = (TDesC*)aParam;
            TPtrC8 param;
            param.Set((TUint8*)res->Ptr(), res->Size());
            SendCommandToServer( aCommandId, param );
            SetNumberModeKeyMappingL((TAknEditorNumericKeymap)EKeymapFromResource);
            }
            break;
        case ECmdPenInputJapaneseSetting:
            {
            SaveCurrentInfo(aParam);
            SendCommandToServer(aCommandId, aParam);
            }
            break;
        case ECmdPenInputJapaneseSetSubRange:
            {
            iSubRange = aParam;
            }
            break;
        case ECmdPenInputJapaneseGetModeOfSubRange:
            {
            TInt newMode = *((TInt*)aParam);
            if(iSubRange == ERangeFullWidthEnglish)
                {
                newMode = ELatin;
                }
            else if( iSubRange == ERangeFullWidthNumeric )
                {
                newMode = ENumber;
                }
            else if( iSubRange == ERangeKatakana ||
                     iSubRange == ERangeFullWidthKatakana )
                {
                newMode = EKatakana;
                }

            else if( iSubRange == ERangeHiraganaKanji )
                {
                newMode = EHiraganaKanji;
                }
            else if( iSubRange == ERangeHiraganaOnly )
                {
                newMode = EHiragana;
                }
            *((TInt*)aParam) = newMode;
            }
            break;
        case ECmdPenInputSendEditorTextAndCurPos:
            {
            TFepInputContextFieldData* pIcfData = 
                reinterpret_cast<TFepInputContextFieldData*>( aParam );

            TInt dataSize = sizeof( TFepInputContextFieldData );
            TInt textSize = pIcfData->iText.Size();

            HBufC8* buf = HBufC8::NewLC(dataSize + textSize + 2*sizeof(TInt));
            TPtr8 bufPtr = buf->Des();

            RDesWriteStream writeStream;
            writeStream.Open(bufPtr);
            CleanupClosePushL(writeStream);

            writeStream.WriteInt32L(dataSize);
            writeStream.WriteInt32L(textSize);

            const TUint8* pData = reinterpret_cast<const TUint8*>( pIcfData );
            writeStream.WriteL( pData, dataSize );
                
            const TUint8* pText = reinterpret_cast<const TUint8*>( pIcfData->iText.Ptr() );
            writeStream.WriteL( pText, textSize );
            
            writeStream.CommitL();

            SendCommandToServer( aCommandId, bufPtr );

            CleanupStack::PopAndDestroy(&writeStream);
            CleanupStack::PopAndDestroy(buf);
            }
            break;
        default:
            SendCommandToServer(aCommandId, aParam);
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::CloseUI
// Close plugin layout UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::CloseUI()
    {
    if (iPenInputServer->IsVisible())
        {
        iPenInputServer->ActivateLayout(EFalse);
        }
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::ActivateUI
// Activate plugin layout UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::ActivateUI()
    {
    iPenInputServer->ActivateLayout(ETrue);
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::EditorMaxLength
// Get max text length of layout UI ICF control.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPluginFepManagerBase::EditorMaxLength()
    {
    return iMaxEditorLength;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::ResourceChanged
// Handle screen size changed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::ResourceChanged(TInt aType)
    {
    iPenInputServer->ResourceChanged(aType);
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetNextFepUI
// Set underlining UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetNextFepUI(MAknFepManagerInterface* aNextUi)
    {
    iHkbUi = aNextUi;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SupportLanguage
// Get support language in sepecfied mode.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPluginFepManagerBase::SupportLanguage(TInt /*aMode*/) const
    {
    return iLanguage;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetInputLanguageL
// Set current language.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetInputLanguageL(TLanguage aLanguage)
    {
    iLanguage = aLanguage;
    if( iLanguage == ELangJapanese )
        {
        SendCommandToServer(ECmdPenInputLanguage, (TInt)aLanguage);
        }
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetFepAwareEditorText
// Set editor text and cursor information to layout UI ICF control.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetFepAwareEditorText(const TFepInputContextFieldData& aIcfData)
    {
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetMode
// Set layout UI mode.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
#ifdef RD_INTELLIGENT_TEXT_INPUT          
void CPluginFepManagerBase::SetMode( TInt aMode, TBool aPredictive, 
                                     TBool aQwertyInputMode, TInt aKeyboardType)
#else
void CPluginFepManagerBase::SetMode( TInt aMode, TBool aPredictive, 
                                     TBool aQwertyInputMode )
#endif 
    {
    iHkbUi->SetMode(aMode, aPredictive, aQwertyInputMode);
    }
#ifdef RD_INTELLIGENT_TEXT_INPUT
    TBool CPluginFepManagerBase::IsValidFnKeyPress() const
    {
    return EFalse;
    }
    TBool CPluginFepManagerBase::IsValidLongChrKeyPress() const
    {
    return EFalse;
    }
#endif
// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetMode
// Set layout UI current case.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetCase(TCase aCase)
    {
    iHkbUi->SetCase(aCase);
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetMode
// Compatibile with HKB UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::ExpireMultitapTimer()
    {
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::IsValidNumericLongKeyPress
// Compatibile with HKB UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPluginFepManagerBase::IsValidNumericLongKeyPress(TInt /*aKey*/) const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::AddTextToUserDictionaryL
// Compatibile with HKB UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::AddTextToUserDictionaryL(const TDesC& /*aText*/)
    {
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::GetFormatOfFepInlineText
// Compatibile with HKB UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::GetFormatOfFepInlineText(
                                TCharFormat& /*aFormat*/,
                                TInt& /*aNumberOfCharactersWithSameFormat*/,
                                TInt /*aPositionOfCharacter*/) const
    {
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::IsValidShiftKeyPress
// Compatibile with HKB UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPluginFepManagerBase::IsValidShiftKeyPress() const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetEditorContext
// Compatibile with HKB UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetEditorContext(TInt /*aContext*/)
    {
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetStarKeyFlag
// Set the Star key flag.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetStarKeyFlag( TBool /*aSet*/ )
    {
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::CPluginFepManagerBase
// C++ default constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPluginFepManagerBase::CPluginFepManagerBase(CPenInputImePluginJp& aOwner,
                                             RPeninputServer* aPenInputServer)
    :iOwner(aOwner), iPenInputServer(aPenInputServer), iSubRange(ERangeInvalid)
    {
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::BaseConstructL
// Symbian 2nd phase constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::BaseConstructL()
    {
    iRepository = CRepository::NewL(KCRUidAknFep);
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetLayoutPermitedRanges
// Set layout permited ranges.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetLayoutPermitedRanges(TInt aPermittedMode)
    {
    //not derived from edwin
    if ( aPermittedMode == EAknEditorNumericInputMode )
        {
        iPermittedRange = ERangeNumber;
        }
    else if (aPermittedMode & EAknEditorTextInputMode)
        {
        //derived from edwin and EAknEditorTextInputMode
        iPermittedRange = ERangeHiraganaKanji |
                          ERangeKatakana |
                          ERangeFullWidthKatakana |
                          ERangeFullWidthEnglish |
                          ERangeFullWidthNumeric |
                          ERangeNumber |
                          ERangeEnglish |
                          ERangeSymbol;
        }
    else if (aPermittedMode & EAknEditorSecretAlphaInputMode )
        {
        //derived from EAknEditorSecretAlphaInputMode
        iPermittedRange = ERangeNumber |
                          ERangeEnglish |
                          ERangeSymbol;
        }
    else
        {
        iPermittedRange = ERangeInvalid;
        if (aPermittedMode & EAknEditorNumericInputMode )
            {
            iPermittedRange |= ERangeNumber | ERangeSymbol;
            }
        if (aPermittedMode & EAknEditorKatakanaInputMode )
            {
            iPermittedRange |= ERangeKatakana;
            }
        if (aPermittedMode & EAknEditorFullWidthTextInputMode )
            {
            iPermittedRange |= ERangeFullWidthEnglish;
            }
        if (aPermittedMode & EAknEditorFullWidthNumericInputMode )
            {
            iPermittedRange |= ERangeFullWidthNumeric;
            }
        if (aPermittedMode & EAknEditorFullWidthKatakanaInputMode )
            {
            iPermittedRange |= ERangeFullWidthKatakana;
            }
        if (aPermittedMode & EAknEditorHiraganaKanjiInputMode )
            {
            iPermittedRange |= ERangeHiraganaKanji;
            }
        if (aPermittedMode & EAknEditorHiraganaInputMode )
            {
            iPermittedRange |= ERangeHiraganaOnly;
            }
        if (aPermittedMode & EAknEditorHalfWidthTextInputMode )
            {
            iPermittedRange |= ERangeEnglish;
            }
        }
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetLayoutRange
// Set layout primary range.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetLayoutRange(const CAknEdwinState* aEditorState)
    {
    //calculate permitted ranges
    if(aEditorState)
        {
        if( aEditorState->Flags() & EAknEditorFlagLatinInputModesOnly )
            {
            iPermittedRange &= (ERangeNumber |
                                ERangeEnglish |
                                ERangeSymbol);
            }
        }

    SendCommandToServer(ECmdPenInputPermittedRange, iPermittedRange);

    TInt priRange;
    if( !aEditorState )
        {
        //if editor state is null,which indicates that
        //the editor is not derived from edwin
        priRange = ERangeNumber;
        }
    else
        {
        //if still not get, then set according to default input mode
        switch(iHkbMode)
            {
            case ELatin:
                priRange = (iCharWidth)? ERangeFullWidthEnglish: ERangeEnglish;
                break;
            case ENumber:
                priRange = (iCharWidth)? ERangeFullWidthNumeric : ERangeNumber;
                break;
            case EHiraganaKanji:
                priRange = ERangeHiraganaKanji;
                break;
            case EKatakana:
                priRange = (iCharWidth)? ERangeFullWidthKatakana : ERangeKatakana;
                break;
            case EHiragana:
                priRange = ERangeHiraganaOnly;
                break;
            default:
                {
                TInt defaultInputMode = aEditorState->DefaultInputMode();
                switch(defaultInputMode)
                    {
                    case EAknEditorNumericInputMode:
                        priRange = ERangeNumber ;
                        break;
                    case EAknEditorTextInputMode:
                        {
                        TLanguage editorLan = GetLocalLanguage(aEditorState);
                        priRange = ERangeEnglish;

                        if( editorLan == ELangTest)
                            {
                            editorLan = iLanguage;
                            }
                        if( editorLan == ELangJapanese )
                            {
                            //load last use range
                            TInt lastUsedRange = LayoutLastUsedRange();
                            if( lastUsedRange != ELangJapanese )
                                {
                                //invalid repository key value or first time use HWR,
                                priRange = ERangeHiraganaKanji;
                                }
                            else
                                {
                                priRange = lastUsedRange;
                                }
                            }
                        }
                        break;
                    default:
                        priRange = ERangeEnglish ;
                        break;
                    }
                }
            }
        }

    SendCommandToServer(ECmdPenInputRange, priRange);
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SendCommandToServer
// Send command to pen input server utility function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SendCommandToServer(TInt aCommandId)
    {
    iPenInputServer->HandleCommand(aCommandId);
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SendCommandToServer
// Send command to pen input server utility function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SendCommandToServer(TInt aCommandId, TInt aParam)
    {
    TBuf8<sizeof(TInt)> buf;
    buf.Append((TUint8*)&aParam,sizeof(TInt));
    SendCommandToServer(aCommandId, buf);
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SendCommandToServer
// Send command to pen input server utility function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SendCommandToServer(TInt aCommandId, const TDesC8& aParam)
    {
    iPenInputServer->HandleCommand(aCommandId, aParam);
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::LayoutLastUsedRange
// Get layout UI last used range.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPluginFepManagerBase::LayoutLastUsedRange()
    {
    TInt value = 0;
    if( iRepository )
        {
        iRepository->Get(KAknFepLastUsedRange, value);
        }

    return value;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetLayoutPosition
// Set layout UI position.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetLayoutPosition(const TRect& pParam)
    {
    TBuf8<sizeof(TRect)> buf;
    buf.Append((TUint8*)&pParam, sizeof(pParam));

    SendCommandToServer(ECmdPenInputSetWindowPos, buf);
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::GetLocalLanguage
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TLanguage CPluginFepManagerBase::GetLocalLanguage(
                const CAknEdwinState* aEditorState) const
    {
    TLanguage language = ELangTest;
    // Check for a local language override
    if (aEditorState)
        {
        TInt aknEditorFlags = aEditorState->Flags() ;
        if (aknEditorFlags & EAknEditorFlagLatinInputModesOnly)
            {
            language = ELangEnglish;
            }
        else
            {
            language = aEditorState->LocalLanguage();
            }
        }
    return language;
    }


// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SaveCurrentInfo
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SaveCurrentInfo(const TInt aParam)
    {
    iCharWidth = (aParam & EPenInputJapaneseSettingCharacterWidth)?
                        ETrue : EFalse;
    iPredictive = (aParam & EPenInputJapaneseSettingPredictive)?
                        ETrue : EFalse;
    iConversion = (aParam & EPenInputJapaneseSettingConversion)?
                        ETrue : EFalse;

    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::IsCharWidth
// Check the character width.
// -----------------------------------------------------------------------------
//
TBool CPluginFepManagerBase::IsCharWidth() const
    {
    return iCharWidth;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::IsJapanesePredictive
// Check the Japanese predictive.
// -----------------------------------------------------------------------------
//
TBool CPluginFepManagerBase::IsJapanesePredictive() const
    {
    return iPredictive;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::IsJapaneseHwrConversion
// Check the Japanese HWR conversion.
// -----------------------------------------------------------------------------
//
TBool CPluginFepManagerBase::IsJapaneseHwrConversion() const
    {
    return iConversion;
    }

 // End Of File
