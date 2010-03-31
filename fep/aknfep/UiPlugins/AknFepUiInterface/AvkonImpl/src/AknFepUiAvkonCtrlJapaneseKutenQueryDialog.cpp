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
* Description:            Kuten code query dialog
*
*/












// INCLUDE FILES
#include <barsread.h>

#include <numberconversion.h>
#include <AknQueryDialog.h>
#include <aknQueryControl.h>
#include <aknnotewrappers.h>                        // CAknInformationNote
#include <aknsoundsystem.h>
#include <charconv.h>
#include <PtiDefs.h>                //keys
#include <AknFep.rsg>
#include <avkon.hrh>

#include "AknFepUIAvkonCtrlJapaneseKutenQueryDialog.h"

// MACROS

// CONSTANTS
const TInt KKutenCodeLength = 4;
const TInt KDummyUnicode = 0xfffd;
const TInt KTextKutenCodeErrorMaxSize = 32;
const TText KHalfWidthNotSign = 0x00AC;
const TText KFullWidthNotSign = 0xFFE2;

// ================= MEMBER FUNCTIONS =========================================

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseKutenQueryDialog::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAknFepUICtrlJapaneseKutenQueryDialog*
CAknFepUICtrlJapaneseKutenQueryDialog::NewL(TInt& aCharCode, const TTone& aTone)
    {
    CAknFepUICtrlJapaneseKutenQueryDialog* self =
        new (ELeave) CAknFepUICtrlJapaneseKutenQueryDialog(aCharCode, aTone);

    return self;
    }

// ----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseKutenQueryDialog::~CAknFepUICtrlJapaneseKutenQueryDialog()
// Destructor.
// ----------------------------------------------------------------------------
//
CAknFepUICtrlJapaneseKutenQueryDialog::~CAknFepUICtrlJapaneseKutenQueryDialog()
    {
    }

// ----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseKutenQueryDialog::OfferKeyEventL
//
// ----------------------------------------------------------------------------
//
TKeyResponse CAknFepUICtrlJapaneseKutenQueryDialog::OfferKeyEventL(const TKeyEvent& aKeyEvent,
                                                                   TEventCode aType)
{
    if (aType == EEventKey)
    {
        TInt keyCode = aKeyEvent.iCode;
        switch(keyCode)
        {
            case EPtiKey0:
            case EPtiKey1:
            case EPtiKey2:
            case EPtiKey3:
            case EPtiKey4:
            case EPtiKey5:
            case EPtiKey6:
            case EPtiKey7:
            case EPtiKey8:
            case EPtiKey9:
                {
                    if (iDataText.Length() == KKutenCodeLength)
                    {
                        if (SoundSystem())
                        {
                        SoundSystem()->PlaySound(EErrorTone);
                        }
                    }
                }
                break;
            default:
                break;
        }
    }
    return CAknTextQueryDialog::OfferKeyEventL(aKeyEvent, aType);
}

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseKutenQueryDialog::OkToExitL
// This function is called in case pressing OK or Cancel
// -----------------------------------------------------------------------------
//
TBool CAknFepUICtrlJapaneseKutenQueryDialog::OkToExitL(TInt aButtonId)
    {
    TBool checkResult = EFalse;
    iCharCode = ConvertKutenCode2Unicode();
    if (iCharCode > 0)
        {
        checkResult = CAknTextQueryDialog::OkToExitL(aButtonId);
        }
    else
        {
        TResourceReader reader;
        TBufC<KTextKutenCodeErrorMaxSize> string;
        CCoeEnv::Static()->CreateResourceReaderLC(reader, R_AKNFEP_TEXT_KUTEN_INVALID_ERROR);
        string = reader.ReadTPtrC();
        CleanupStack::PopAndDestroy();      // reader

        CAknInformationNote* dialog = new(ELeave) CAknInformationNote();
        dialog->ExecuteLD(string);

        CAknQueryControl* control = QueryControl();
        if (control)
            {
            control->SetTextL(KNullDesC);
            control->DrawNow();
            }

        UpdateLeftSoftKeyL();
        }

    return checkResult;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseKutenQueryDialog::UpdateLeftSoftKeyL
// Updating left softkey
//
// If control text is ok then display left soft key
// otherwise don't
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseKutenQueryDialog::UpdateLeftSoftKeyL()
    {
    CAknQueryControl* control = QueryControl();
    if ( control )
        {
        MakeLeftSoftkeyVisibleL();
        }
    }

// ----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseKutenQueryDialog::CAknFepUICtrlJapaneseKutenQueryDialog()
// Overload constructor.
//-----------------------------------------------------------------------------
//
CAknFepUICtrlJapaneseKutenQueryDialog::CAknFepUICtrlJapaneseKutenQueryDialog(
        TInt& aCharCode, const TTone& aTone)
        : CAknTextQueryDialog(iBuffer, aTone),
          iCharCode(aCharCode),
          iCbaResId(R_AVKON_SOFTKEYS_CANCEL)
    {
    iCharCode = 0;
    }

TInt CAknFepUICtrlJapaneseKutenQueryDialog::ExecuteLD(TInt aResourceId)
    {
    PrepareLC(aResourceId);
    CEikEdwin* edwin = STATIC_CAST(CEikEdwin*, (QueryControl()->ControlByLayoutOrNull(EDataLayout)));
    edwin->SetAknEditorAllowedInputModes(EAknEditorNumericInputMode);
    edwin->SetAknEditorNumericKeymap(EAknEditorPlainNumberModeKeymap);
    return(RunLD());
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseKutenQueryDialog::MakeLeftSoftkeyVisibleL
// Set visibility of the left softkey.
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseKutenQueryDialog::MakeLeftSoftkeyVisibleL()
    {
    TBool visible = EFalse;
    CAknQueryControl* control = QueryControl();
    if (control)
        {
        control->GetText(iDataText);
        if (iDataText.Length() == KKutenCodeLength)
            {
            visible = ETrue;
            }
        }
    else
        {
        visible = EFalse;
        }

    TUint resid = R_AKNFEP_SOFTKEYS_EMPTY_CANCEL_EMPTY;
    if (visible)
        {
        resid = R_AVKON_SOFTKEYS_OK_CANCEL__OK;
        }
    if (iCbaResId != resid)
        {
        ButtonGroupContainer().SetCommandSetL(resid);
        ButtonGroupContainer().DrawDeferred();
        iCbaResId = resid;
        }

    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseKutenQueryDialog::ConvertKutenCode2Unicode
// Convert from Kuten code to unicode, Kuten code is 4 characters which inputted in query dialog.
// -----------------------------------------------------------------------------
TText CAknFepUICtrlJapaneseKutenQueryDialog::ConvertKutenCode2Unicode()
    {
    TText code = 0;
    if (iBuffer.Length() == KKutenCodeLength)
        {
        TBuf<4 + 1> buf;
        TBool valid = ETrue;
        buf.FillZ(4 + 1);

        for (TInt i = 0; i < KKutenCodeLength; ++i)
            {
            if ((iBuffer[i] >= '0') && (iBuffer[i] <= '9'))
                {
                buf[i] = (TText)(iBuffer[i] - '0');
                }
            else
                {
                valid = EFalse;
                break;
                }
            }
        if (valid)
            {
            TInt hi_code = buf[0] * 10 + buf[1] + 0x20;
            TInt low_code = buf[2] * 10 + buf[3] + 0x20;
            code = (TText)((hi_code << 8) + low_code);
            AdjustJisCode(code);
            code = JisToSjis((TText)code);
            if (AdjustSjisCode(code))
                {
                code = ConvSjisToUnicode(code);
                }
            else
                {
                code = 0;
                }
            }
        }

    // '0244" Kuten codes is converted to '0x00AC' unicode of half-width
    // character. In Japanese variant, Kuten code input must be always
    // Full-width character, so it converts from '0x00AC' to '0xFFE2'
    if (code == KHalfWidthNotSign)
        {
        code = KFullWidthNotSign;
        }

    return code;
    }

void CAknFepUICtrlJapaneseKutenQueryDialog::AdjustJisCode(TText& aJisCode)
    {
    TUint JisHi = (aJisCode >> 8) & 0x000000ff;
    TUint JisLow = aJisCode & 0x000000ff;

    if ((JisHi < EJisFirstMin) || (JisHi > EJisFirstMax))
        {
        JisHi = 0;
        JisLow = 0;
        }
    else
        {
        if ((JisLow < EJisSecondMin) || (JisLow > EJisSecondMax))
            {
            JisHi = 0;
            JisLow = 0;
            }
        else
            {
            if (JisHi == EJisFirstMax)
                {
                if (JisLow > EJisLastSecondMax)
                    {
                    JisHi = 0;
                    JisLow = 0;
                    }
                }
            }
        }

    aJisCode = (TText)((JisHi << 8) + JisLow);
    return;
    }

TBool CAknFepUICtrlJapaneseKutenQueryDialog::AdjustSjisCode(TText& aSjisCode)
{
    TUint SjisHi = (aSjisCode >> 8) & 0x000000ff;
    TUint SjisLow = aSjisCode & 0x000000ff;

    if ((SjisHi < ESjisFirstMin1) || (SjisHi > ESjisFirstMax2)
        || ((SjisHi > ESjisFirstMax1) && (SjisHi < ESjisFirstMin2)))
        {
        SjisHi = 0;
        SjisLow = 0;
        }
    else
        {
        if ((SjisLow < ESjisSecondMin1) || (SjisLow > ESjisSecondMax2)
            || ((SjisLow > ESjisSecondMax1) && (SjisLow < ESjisSecondMin2)))
            {
            SjisHi = 0;
            SjisLow = 0;
            }
        
        /*
        else
            {
            if (SjisHi == ESjisFirstMax2)
                {
                if (SjisLow > ESjisSecondMax2)
                    {
                    SjisHi = 0;
                    SjisLow = 0;
                    }
                }
            }
        */
        }

    if (CheckSjisCode((TText)((SjisHi << 8) + SjisLow)))
        {
        aSjisCode = (TText)((SjisHi << 8) + SjisLow);
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

TText CAknFepUICtrlJapaneseKutenQueryDialog::JisToSjis(TText aJisCode)
    {
    if (aJisCode == 0)
        {
        return 0;
        }

    TUint jis_hi = (aJisCode >> 8) & 0x000000ff;    /* hi byte */
    TUint jis_lo = aJisCode & 0x000000ff;           /* low byte */

    jis_lo += 0x1F;
    if  ((jis_hi & 1) == 0)
        {
        jis_hi >>= 1;
        jis_hi--;
        jis_lo += 0x5E;
        }
    else
        {
        jis_hi >>= 1;
        }
    if  (jis_lo >= 0x7F)
        {
        jis_lo++;
        }
    jis_hi += (jis_hi > 0x2E) ? 0xB1 : 0x71;

    return (TText)((jis_hi << 8) + jis_lo);
    }

TBool CAknFepUICtrlJapaneseKutenQueryDialog::CheckSjisCode(TText aSjisCode)
    {
    const TText8 first = (TText8)((aSjisCode >> 8) & 0x000000ff);
    const TText8 second = (TText8)(aSjisCode & 0x000000ff);
    if ((((first >= ESjisFirstMin1) && (first <= ESjisFirstMax1))
         || ((first >= ESjisFirstMin2) && (first <= ESjisFirstMax2)))
        && (((second >= ESjisSecondMin1) && (second <= ESjisSecondMax1))
            || ((second >= ESjisSecondMin2) && (second <= ESjisSecondMax2))))
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

TText CAknFepUICtrlJapaneseKutenQueryDialog::ConvSjisToUnicode(TText aSjisCode)
    {
    TText8  sjis[2];
    TText   unicode = 0;
    if (aSjisCode & 0xff00)
        {
        sjis[0] = (TText8)((aSjisCode & 0xff00) >> 8);
        sjis[1] = (TText8)(aSjisCode & 0x00ff);
        }
    else
        {
        sjis[0] = (TText8)aSjisCode;
        sjis[1] = '\0';
        }

    TPtrC8 foreign((TUint8*)&sjis[0], sizeof sjis);
    TPtr16 unicode_des((TUint16*)&unicode, 1);
    RFs fileServerSession;

    if (fileServerSession.Connect() == 0)
        {
        CCnvCharacterSetConverter*  charconv = NULL;
        TRAPD(error, charconv = CCnvCharacterSetConverter::NewL());
        if (error == KErrNone)
            {
            TRAP(error, charconv->PrepareToConvertToOrFromL(KCharacterSetIdentifierShiftJis, fileServerSession));
            if (error == KErrNone)
                {
                TInt state = CCnvCharacterSetConverter::KStateDefault;
                charconv->ConvertToUnicode(unicode_des, foreign, state);
                }
            delete charconv;
            }
        fileServerSession.Close();
        }
    if (unicode == (TText)KDummyUnicode)
        {
        unicode = 0;
        }
    return unicode;
    }

// End of File
