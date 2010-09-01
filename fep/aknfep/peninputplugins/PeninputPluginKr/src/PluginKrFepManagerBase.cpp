/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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

#include <s32mem.h>
#include <centralrepository.h>
#include <AknFepGlobalEnums.h>
#include <aknfeppeninputenums.h>
#include <aknedsts.h>

#include <peninputcommonlayoutglobalenum.h>

#include "PeninputPluginKrDbg.h"
#include "PeninputPluginKr.hrh"
#include "PluginKrFepManagerBase.h"

// ---------------------------------------------------------------------------
// Constructors & Destructors
// ---------------------------------------------------------------------------
//

CPluginKrFepManagerBase::CPluginKrFepManagerBase(
        CPeninputPluginKr& aOwner, 
        RPeninputServer& aPenInputServer)
    : iOwner(aOwner),
      iPenInputServer(aPenInputServer),
      iSuggestedRange(ERangeInvalid),
      iLastRange(ERangeInvalid)
    {
    }

void CPluginKrFepManagerBase::ConstructL()
    {

    }

CPluginKrFepManagerBase::~CPluginKrFepManagerBase()
    {
    delete iRepository;
    }

// ---------------------------------------------------------------------------
// from MAknFepManagerInterface
// ---------------------------------------------------------------------------
//

void CPluginKrFepManagerBase::SetMode(
        TInt aMode, 
        TBool aPredictive, 
        TBool aQwertyInputMode,
        TInt /*aKeyboardType*/ )
    {
    LOG3("FEP.IMEPLG.KR.Base.SetMode %d %d %d",aMode,aPredictive,aQwertyInputMode);
    iHkbUi->SetMode(aMode, aPredictive, aQwertyInputMode);
    }

void CPluginKrFepManagerBase::SetCase(TCase aCase)
    {
    LOG1("FEP.IMEPLG.KR.Base.SetCase %d",aCase);
    iHkbUi->SetCase(aCase);
    }

void CPluginKrFepManagerBase::SetNumberModeKeyMappingL(
        TAknEditorNumericKeymap aAknEditorNumericKeymap)
    {
    LOG1("FEP.IMEPLG.KR.Base.SetNumberModeKeyMappingL %d",aAknEditorNumericKeymap);
    SendCommandToServer(ECmdPenInputEditorNumericKeyMap, aAknEditorNumericKeymap);    
    }

TBool CPluginKrFepManagerBase::HandleKeyL(
        TInt aKey, 
        TKeyPressLength aLength,
        TEventCode /*aEventCode*/ )
    {
    LOG2("FEP.IMEPLG.KR.Base.HandleKeyL %d",aKey,aLength);
    return iHkbUi->HandleKeyL( aKey, aLength );
    }

void CPluginKrFepManagerBase::HandleCommandL(
        TInt aCommandId)
    {
    LOG1("FEP.IMEPLG.KR.Base.HandleCommandL %d",aCommandId);
    SendCommandToServer( aCommandId );
    }
    
void CPluginKrFepManagerBase::HandleCommandL(
        TInt aCommandId, 
        TInt aParam)
    {
    switch ( aCommandId )
        {
        case ECmdPenInputRange:
            {
            LOG1("FEP.IMEPLG.KR.Base.HandleCommandL ECmdPenInputRange %d",iHkbMode);
            if (aParam&EPRCFind) LOG("FEP.IMEPLG.KR.Base  EPRCFind");
            if (aParam&ECangJie) LOG("FEP.IMEPLG.KR.Base  ECangJie");
            if (aParam&EZhuyinFind) LOG("FEP.IMEPLG.KR.Base  EZhuyinFind");
            if (aParam&EStrokeFind) LOG("FEP.IMEPLG.KR.Base  EStrokeFind");
            if (aParam&EPinyin) LOG("FEP.IMEPLG.KR.Base  EPinyin");
            if (aParam&EZhuyin) LOG("FEP.IMEPLG.KR.Base  EZhuyin");
            if (aParam&EStroke) LOG("FEP.IMEPLG.KR.Base  EStroke");
            if (aParam&ELatin) LOG("FEP.IMEPLG.KR.Base  ELatin");
            if (aParam&ENumber) LOG("FEP.IMEPLG.KR.Base  ENumber");
            if (aParam&EHiraganaKanji) LOG("FEP.IMEPLG.KR.Base  EHiraganaKanji");
            if (aParam&EKatakana) LOG("FEP.IMEPLG.KR.Base  EKatakana");
            if (aParam&EHiragana) LOG("FEP.IMEPLG.KR.Base  EHiragana");
            if (aParam&EHindi) LOG("FEP.IMEPLG.KR.Base  EHindi");
            if (aParam&ENativeNumber) LOG("FEP.IMEPLG.KR.Base  ENativeNumber");
            if (aParam&EHangul) LOG("FEP.IMEPLG.KR.Base  EHangul");
            if (aParam&ELatinText) LOG("FEP.IMEPLG.KR.Base  ELatinText");
            if (aParam&ELatinUpper) LOG("FEP.IMEPLG.KR.Base  ELatinUpper");
            if (aParam&ELatinLower) LOG("FEP.IMEPLG.KR.Base  ELatinLower");
            if (aParam&ECangJieNormal) LOG("FEP.IMEPLG.KR.Base  ECangJieNormal");
            if (aParam&ECangJieEasy) LOG("FEP.IMEPLG.KR.Base  ECangJieEasy");
            if (aParam&ECangJieAdvanced) LOG("FEP.IMEPLG.KR.Base  ECangJieAdvanced");
            iHkbMode = aParam;
            }
            break;
        case ECmdPenInputPermittedRange:
            {
            LOG("FEP.IMEPLG.KR.Base.HandleCommandL ECmdPenInputPermittedRange");
            if (aParam&EAknEditorTextInputMode) LOG("FEP.IMEPLG.KR.Base  EAknEditorTextInputMode");
            if (aParam&EAknEditorNumericInputMode) LOG("FEP.IMEPLG.KR.Base  EAknEditorNumericInputMode");
            if (aParam&EAknEditorSecretAlphaInputMode) LOG("FEP.IMEPLG.KR.Base  EAknEditorSecretAlphaInputMode");
            if (aParam&EAknEditorKatakanaInputMode) LOG("FEP.IMEPLG.KR.Base  EAknEditorKatakanaInputMode");
            if (aParam&EAknEditorFullWidthTextInputMode) LOG("FEP.IMEPLG.KR.Base  EAknEditorFullWidthTextInputMode");
            if (aParam&EAknEditorFullWidthNumericInputMode) LOG("FEP.IMEPLG.KR.Base  EAknEditorFullWidthNumericInputMode");
            if (aParam&EAknEditorFullWidthKatakanaInputMode) LOG("FEP.IMEPLG.KR.Base  EAknEditorFullWidthKatakanaInputMode");
            if (aParam&EAknEditorHiraganaKanjiInputMode) LOG("FEP.IMEPLG.KR.Base  EAknEditorHiraganaKanjiInputMode");
            if (aParam&EAknEditorHiraganaInputMode) LOG("FEP.IMEPLG.KR.Base  EAknEditorHiraganaInputMode");
            if (aParam&EAknEditorHalfWidthTextInputMode) LOG("FEP.IMEPLG.KR.Base  EAknEditorHalfWidthTextInputMode");
            if (aParam&EAknEditorHangulInputMode) LOG("FEP.IMEPLG.KR.Base  EAknEditorHangulInputMode");
            iLastCase = ECaseInvalide;
            SetLayoutPermitedRanges( aParam );
            }
            break;
        case ECmdPenInputEditorState:
            {
            LOG("FEP.IMEPLG.KR.Base.HandleCommandL ECmdPenInputEditorState");
            SetLayoutRange(reinterpret_cast<CAknEdwinState*>(aParam));
            }
            break;
        case ECmdPenInputSetWindowPos:
            {
            LOG("FEP.IMEPLG.KR.Base.HandleCommandL ECmdPenInputSetWindowPos");
            //SetLayoutPosition(*reinterpret_cast<TRect*>(aParam));
            }
            break;
        case ECmdPenInputCaseMode:
            LOG1("FEP.IMEPLG.KR.Base.HandleCommandL ECmdPenInputCaseMode %d",aParam);
            //if( aParam != iLastCase )
                {
                SendCommandToServer( ECmdPenInputCase, aParam );
                iLastCase = aParam;
                }
            break;
        case ECmdPenInputEditorCustomNumericKeyMap:
            {
            LOG("FEP.IMEPLG.KR.Base.HandleCommandL ECmdPenInputEditorCustomNumericKeyMap");
            TDesC* res = (TDesC*)aParam;
            TPtrC8 param;
            param.Set((TUint8*)res->Ptr(), res->Size());
            SendCommandToServer( aCommandId, param );                    
            SetNumberModeKeyMappingL((TAknEditorNumericKeymap)EKeymapFromResource);
            }
            break;            
        case ECmdPenInputSuggestRange:
            LOG("FEP.IMEPLG.KR.Base.HandleCommandL ECmdPenInputSuggestRange");
            iSuggestedRange = aParam;
            if( iSuggestedRange == ERangeAccent )
                {
                iSuggestedRange = ERangeEnglish;
                }
            break;
        case ECmdPenInputSendEditorTextAndCurPos:
            {
            LOG("FEP.IMEPLG.KR.Base.HandleCommandL ECmdPenInputSendEditorTextAndCurPos");
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
        case ECmdPenInputSetPromptText:
            {
            LOG("FEP.IMEPLG.KR.Base.HandleCommandL ECmdPenInputSetPromptText");
            TFepPromptText* pPromptData = 
                reinterpret_cast<TFepPromptText*>( aParam );
                
            TInt dataSize = sizeof( TFepPromptText );
            TInt textSize = 0;
            
            if (pPromptData->iText.Length())
                {
                textSize = pPromptData->iText.Size();
                }
            
            HBufC8* buf = HBufC8::NewLC(dataSize + textSize + 2*sizeof(TInt));
            TPtr8 bufPtr = buf->Des();
            
            RDesWriteStream writeStream;
            writeStream.Open(bufPtr);
            CleanupClosePushL(writeStream);
            
            writeStream.WriteInt32L(dataSize);
            writeStream.WriteInt32L(textSize);
            
            const TUint8* pData = reinterpret_cast<const TUint8*>( pPromptData );
            writeStream.WriteL( pData, dataSize );
            
            if (textSize != 0)
                {
                const TUint16* pText = pPromptData->iText.Ptr();
                writeStream.WriteL( pText, textSize/2 );
                }
            
            writeStream.CommitL();
        
            SendCommandToServer( aCommandId, bufPtr );
    
            CleanupStack::PopAndDestroy(&writeStream);
            CleanupStack::PopAndDestroy(buf);
            }
            break;
        case ECmdPenInputFingerMatchIndicator:
            {
            if (aParam)  // ITUT
                {
                TFepIndicatorInfo* pIndicatorData = 
                    reinterpret_cast<TFepIndicatorInfo*>( aParam );

                HBufC8* buf = HBufC8::NewLC(4 * sizeof(TInt));
                TPtr8 bufPtr = buf->Des();

                RDesWriteStream writeStream;
                writeStream.Open(bufPtr);
                CleanupClosePushL(writeStream);

                writeStream.WriteInt32L(pIndicatorData->iIndicatorImgID);
                writeStream.WriteInt32L(pIndicatorData->iIndicatorMaskID);
                writeStream.WriteInt32L(pIndicatorData->iIndicatorTextImgID);
                writeStream.WriteInt32L(pIndicatorData->iIndicatorTextMaskID);

                writeStream.CommitL();

                SendCommandToServer( aCommandId, bufPtr );

                CleanupStack::PopAndDestroy(&writeStream);
                CleanupStack::PopAndDestroy(buf);
                }
            else
                {
                SendCommandToServer( aCommandId, 0 );
                }
            }
            break;
        default:
            LOG2("FEP.IMEPLG.KR.Base.HandleCommandL %d 0x%08x",aCommandId,aParam);
            SendCommandToServer( aCommandId, aParam );
            break;
        }
    }

void CPluginKrFepManagerBase::CloseUI()
    {
    LOG("FEP.IMEPLG.KR.Base.CloseUI");
    if ( iPenInputServer.IsVisible() )
        {
        iPenInputServer.ActivateLayout( EFalse ); 
        }
    }

void CPluginKrFepManagerBase::ExpireMultitapTimer()
    {
    LOG("FEP.IMEPLG.KR.Base.ExpireMultitapTimer");
    }

TBool CPluginKrFepManagerBase::IsValidNumericLongKeyPress(TInt aKey) const
    {
    LOG1("FEP.IMEPLG.KR.Base.IsValidNumericLongKeyPress %d",aKey);
    return EFalse;
    }

void CPluginKrFepManagerBase::AddTextToUserDictionaryL(const TDesC& aText)
    {
    TPtrC ptr(aText);
    LOG1("FEP.IMEPLG.KR.Base.AddTextToUserDictionaryL %S",&ptr);
    }

void CPluginKrFepManagerBase::GetFormatOfFepInlineText(
        TCharFormat& /*aFormat*/, 
        TInt& /*aNumberOfCharactersWithSameFormat*/, 
        TInt aPositionOfCharacter) const
    {
    LOG1("FEP.IMEPLG.KR.Base.GetFormatOfFepInlineText %S",aPositionOfCharacter);
    }

TInt CPluginKrFepManagerBase::SupportLanguage(TInt aMode) const
    {
    LOG2("FEP.IMEPLG.KR.Base.SupportLanguage %d - %d",aMode,iLanguage);
    return iLanguage;
    }

void CPluginKrFepManagerBase::SetInputLanguageL(TLanguage aLanguage)
    {
    LOG1("FEP.IMEPLG.KR.Base.SetInputLanguageL %d",aLanguage);
    iLanguage = aLanguage;
    SendCommandToServer(ECmdPenInputLanguage, (TInt)aLanguage);
    }

TBool CPluginKrFepManagerBase::IsValidShiftKeyPress() const
    {
    LOG("FEP.IMEPLG.KR.Base.IsValidShiftKeyPress");
    return EFalse;
    }

void CPluginKrFepManagerBase::SetEditorContext(TInt aContext)
    {
    LOG1("FEP.IMEPLG.KR.Base.SetEditorContext %d",aContext);
    }

void CPluginKrFepManagerBase::ActivateUI()
    {
    LOG("FEP.IMEPLG.KR.Base.ActivateUI");
    iPenInputServer.ActivateLayout( ETrue );
    }

void CPluginKrFepManagerBase::SetFepAwareEditorText(
        const TFepInputContextFieldData& /*aIcfData*/)
    {
    LOG("FEP.IMEPLG.KR.Base.SetFepAwareEditorText");
    }

TInt CPluginKrFepManagerBase::EditorMaxLength()
    {
    LOG1("FEP.IMEPLG.KR.Base.EditorMaxLength - %d",iMaxEditorLength);
    return iMaxEditorLength;
    }
    
void CPluginKrFepManagerBase::ResourceChanged(TInt aType)
    {
    LOG1("FEP.IMEPLG.KR.Base.ResourceChanged - %d",aType);
    iPenInputServer.ResourceChanged(aType);
    }
        
void CPluginKrFepManagerBase::SetNextFepUI(
        MAknFepManagerInterface* aNextUi)
    {
    LOG("FEP.IMEPLG.KR.Base.SetNextFepUI");
    iHkbUi = aNextUi;
    }

void CPluginKrFepManagerBase::SetStarKeyFlag( TBool aSet )
    {
    LOG1("FEP.IMEPLG.KR.Base.SetStarKeyFlag %d",aSet);
    }

// ---------------------------------------------------------------------------
// External Functions
// ---------------------------------------------------------------------------
//

void CPluginKrFepManagerBase::OnInit()
    {
    
    }

void CPluginKrFepManagerBase::RetrieveEditorMaxLength()
    {
    
    }

void CPluginKrFepManagerBase::UpdateRange( TInt aRange )
    {
    iLastRange=aRange;
    }

void CPluginKrFepManagerBase::UpdateCaseMode( TInt aCaseMode )
    {
    iLastCase = aCaseMode; 
    }

// ---------------------------------------------------------------------------
// Internal Functions
// ---------------------------------------------------------------------------
//

void CPluginKrFepManagerBase::SendCommandToServer( TInt aCommandId )
    {
    iPenInputServer.HandleCommand( aCommandId );
    }

void CPluginKrFepManagerBase::SendCommandToServer( TInt aCommandId, TInt aParam )
    {
    TBuf8<sizeof(TInt)> buf;
    buf.Append( (TUint8*)&aParam, sizeof(TInt) );    
    SendCommandToServer( aCommandId, buf );
    }

void CPluginKrFepManagerBase::SendCommandToServer(TInt aCommandId, const TDesC8& aParam)
    {
    iPenInputServer.HandleCommand( aCommandId, aParam );        
    }

void CPluginKrFepManagerBase::SetLayoutRange(const CAknEdwinState* aEditorState)
    {
    //calculate permitted ranges
    // TODO: Korean specific permitted range can be added
    if (aEditorState)
        {
        if( aEditorState->Flags() & EAknEditorFlagLatinInputModesOnly )
            {
            iPermittedRange &= ~ERangeNative;
            iPermittedRange &= ~ERangeNativeNumber;
            iPermittedRange &= ~ERangeSymbol;
            }
        /*
        if ( iPermittedMode != EAknEditorNumericInputMode 
            && !( aEditorState->Flags() & EAknEditorFlagLatinInputModesOnly ) )
            {
            iPermittedRange &= ~ERangeEnglish;
            }
        */
        }
    else // for MFNE editor
        {
        if ( iPermittedMode == EAknEditorNumericInputMode )
            {
            iPermittedRange &= ~ERangeSymbol;
            if( iHkbMode == ENumber )
                {
                iPermittedRange = ERangeNumber;
                }
            else if ( iHkbMode == ENativeNumber )
                {
                iPermittedRange = ERangeNativeNumber;
                } 
            }
        }
    

    SendCommandToServer( ECmdPenInputPermittedRange, iPermittedRange );
    
    TInt priRange;
    
    switch( iHkbMode )
        {
        case ECangJie:
        case EStrokeFind:
        case EStroke:
        case EPinyin:
        case EZhuyin:
        case EZhuyinFind:
        case EHindi:
        case EHangul:
            {
            priRange = ERangeNative;
            }
            break;                 
        case ENumber:
            {
            priRange = ERangeNumber;
            }
            break;  
        case ENativeNumber:
            {
            priRange = ERangeNativeNumber;
            }
            break;  
        case EPRCFind:
        case ELatin:
        case ELatinText:
        case ELatinUpper:
        case ELatinLower:
        	{
        	priRange = ERangeEnglish;
        	}
        	break;
        default:
            priRange = iPermittedRange & ERangeNative ? ERangeNative : ERangeEnglish;
            break;               
        }
     
    if( iSuggestedRange != ERangeInvalid && (iSuggestedRange & iPermittedRange) )
        {
        priRange = iSuggestedRange;
        iSuggestedRange = ERangeInvalid;
        }
    
    if (iLastRange)
        {
        priRange=iLastRange;
        }
    
    SendCommandToServer( ECmdPenInputRange, priRange );
    }

void CPluginKrFepManagerBase::SetLayoutPermitedRanges( TInt aPermittedMode )
    {
    iPermittedMode = aPermittedMode;
    
    // Not derived from edwin
    if ( aPermittedMode == EAknEditorNumericInputMode ) 
        {
        // Only number range is supported in Korean Variant
        iPermittedRange = ERangeNumber;
        }
    else if ( aPermittedMode & EAknEditorTextInputMode )
        {
        // Derived from edwin and EAknEditorTextInputMode
        iPermittedRange = ERangeNative | 
                          ERangeNumber | 
                          ERangeEnglish | 
                          ERangeSymbol |
                          ERangeNativeNumber;
        }
    else if ( aPermittedMode & EAknEditorSecretAlphaInputMode ) 
        {
        // Derived from EAknEditorSecretAlphaInputMode 
        iPermittedRange = ERangeNumber |
                          ERangeEnglish | 
                          ERangeSymbol;        
        }
    else
        {
        iPermittedRange = ERangeNative | 
                          ERangeNumber | 
                          ERangeEnglish | 
                          ERangeSymbol |
                          ERangeNativeNumber;
        }
    }

TBool CPluginKrFepManagerBase::SupportNativeNumberRange()
    {
    if ( iLanguage == ELangArabic || iLanguage == ELangFarsi ||
         iLanguage == ELangUrdu || iLanguage == ELangHindi ||
         iLanguage == ELangThai )
         {
         return ETrue;
         }
        
    return EFalse;                   
    }   


