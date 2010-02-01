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












#ifndef AKNFEPUICTRLJAPANESEKUTENQUERYDIALOG_H
#define AKNFEPUICTRLJAPANESEKUTENQUERYDIALOG_H

// INCLUDES
#include <AknQueryDialog.h> // CAknTextQueryDialog

// FORWARD DECLARATIONS

// CONSTANTS

// CLASS DECLARATION

/**
*  CAknFepUICtrlJapaneseKutenQueryDialog class.
*
*  @lib AknFep.lib
*  @since 2.6
*/
NONSHARABLE_CLASS(CAknFepUICtrlJapaneseKutenQueryDialog) : public CAknTextQueryDialog
    {
    public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CAknFepUICtrlJapaneseKutenQueryDialog* NewL(
                        TInt& aCharCode, const TTone& aTone = ENoTone);

        /**
        * Destructor.
        */
        virtual ~CAknFepUICtrlJapaneseKutenQueryDialog();
        TInt ExecuteLD(TInt aResourceId);

        TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);
    protected:  // Functions from CEikDialog
        /**
        * From CEikdialog   This function is called by the EIKON framework
        *                   if the user activates a button in the button panel.
        *                   It is not called if the Cancel button is activated,
        *                   unless the EEikDialogFlagNotifyEsc flag is set.
        * @since 2.6
        * @param aButtonId  The ID of the button that was activated
        * @return           Should return ETrue if the dialog should exit,
        *                   and EFalse if it should not.
        */
        virtual TBool OkToExitL(TInt aButtonId);

    protected:  // Functions from CAknQueryDialog
        /**
        * If the query text is ok (retrieved form query control) then display the
        * left soft key, otherwise hide it.
        * @since 2.6
        */
        virtual void  UpdateLeftSoftKeyL();

    private:
        /**
        * constructor.
        */
        CAknFepUICtrlJapaneseKutenQueryDialog(
                        TInt& aCharCode, const TTone& aTone = ENoTone);

        /**
        * Set visibility of the left softkey.
        * Softkey is hidden when data in the query is not acceptable.
        * @since 2.6
        */
        void MakeLeftSoftkeyVisibleL();

        enum TJisElementsCharMap
            {
            EJisFirstMin = 0x21,
            EJisFirstMax = 0x74,
            EJisSecondMin = 0x21,
            EJisSecondMax = 0x7E,
            EJisLastSecondMax = 0x2F
            };

        enum TSjisElements
            {
            ESjisFirstMin1   = 0x81,
            ESjisFirstMax1   = 0x9F,
            ESjisFirstMin2   = 0xE0,
            ESjisFirstMax2   = 0xEA,
//          ESjisFirstMax2   = 0xEE,                // NEC extended kanji code
//          ESjisFirstMax2   = 0xFC,                // IBM extended kanji code
            ESjisFirstCount  = (ESjisFirstMax2 - ESjisFirstMin2 + 1
                                + ESjisFirstMax1 - ESjisFirstMin1 + 1),
            ESjisSecondMin1  = 0x40,
            ESjisSecondMax1  = 0x7E,
            ESjisSecondMin2  = 0x80,
            ESjisSecondMax2  = 0xFC,
            ESjisSecondCount = (ESjisSecondMax2 - ESjisSecondMin2 + 1
                                + ESjisSecondMax1 - ESjisSecondMin1 + 1)
            };

        /**
        * Convert from Kuten code to unicode, Kuten code is 4 characters which inputted in query dialog.
        * @since 2.6
        * @return unicode, it is one character.
        */
        TText   ConvertKutenCode2Unicode();

        /**
        * Convert to valid jis code. If inputting invalid code, this function converts nearly valid code.
        * @since 2.6
        */
        void    AdjustJisCode(TText& aJisCode);

        /**
        * Convert to valid s-jis code. If inputting invalid code, this function converts nearly valid code.
        * @since 2.6
        * @return ETrue: s-jis code EFalse: none s-jis code
        */
        TBool   AdjustSjisCode(TText& aSjisCode);

        /**
        * Convert from JIS to S-JIS
        * @since 2.6
        * @return s-jis code as inputting jis code
        */
        TText   JisToSjis(TText aJisCode);

        /**
        * Check a s-jis code inputted in dialog
        * @since 2.6
        * @return ETrue: s-jis code EFalse: none s-jis code
        */
        TBool   CheckSjisCode(TText aSjisCode);

        /**
        * Convert from s-jis to unicode
        * @since 2.6
        * @return unicode code as inputting s-jis code
        */
        TText   ConvSjisToUnicode(TText aSjisCode);

    private:
        TInt&       iCharCode;
        TBuf<5>     iBuffer;
        TUint       iCbaResId;
    };

#endif  // AKNFEPUICTRLJAPANESEKUTENQUERYDIALOG_H

// End of File
