/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Container for GSLangPlugin.
*
*/

#ifndef GSLANGCONTAINER_H
#define GSLANGCONTAINER_H

// INCLUDES
#include    <gsbasecontainer.h>
#include    <gscommon.hrh>
#include    <bldvariant.hrh>

_LIT(HLP_GS_LANGUAGE_PLUGIN, "CP_HLP_LANGUAGE"); // temp fix for helps integration build break;replace KCP_HLP_LANGUAGE in cp.hlp.hrh

// FORWARD DECLARATION
class CGSRadioButtonSettingPageItemTextArray;
class CGSListBoxItemTextArray;
class CGSLangModel;

// CLASS DECLARATION

/**
*  CGSLangContainer container class
*  @lib GSLangPlugin.dll
*  @since Series60 3.1
*/
class CGSLangContainer : public CGSBaseContainer
    {
    public: // Constructors and destructor

        CGSLangContainer();

        /**
        * Symbian OS constructor.
        *
        * @param aRect Listbox's rect.
        */
        void ConstructL( const TRect& aRect );

        /**
        * Destructor.
        */
        ~CGSLangContainer();

    public: // From CCoeControl

        /**
        * See base class.
        */
        TInt CountComponentControls() const;

        /**
        * See base class.
        */
        CCoeControl* ComponentControl( TInt /*aIndex*/ ) const;

        //void HandleResourceChange(TInt aType);
        //void SizeChanged();

        /**
        * See base class.
        */
        void GetHelpContext( TCoeHelpContext& aContext ) const;

    public: // New

        /**
        * Updates listbox's item's value.
        * @param aItemId An item which is updated.
        */
        void UpdateListBoxL( TInt aItemId );

        /**
        * Retrieves the currently selected listbox feature id
        * @return feature id.
        */
        TInt CurrentFeatureId() const;

        /**
        *   Getter for CGSModel. Does not transfer ownership.
        */
        CGSLangModel* Model();

        /**
        * @return pointer to number mode items.
        */
        const MDesCArray* NumberModeItems(  TInt aNbrModeType  );
        void MakeListItemVisibleL( TInt aFeatureId, 
                                   TInt aCurrentlySelectedFeature ); 
        
        /*
        * Set T9 item visibility
        */
        void SetT9ItemVisibilityL( TBool aVisibility );
        void HandleResourceChange( TInt aType );
        
    protected: // From CGSBaseContainer
        /**
        * Constructs listbox and initializes its sub-folders
        * @param aResLbxId: resource ID of the listbox item array
        */
        void ConstructListBoxL( TInt aResLbxId );

    private: // New

        // Functions for making the list items:
        void CreateListBoxItemsL();

        void MakeDTLItemL();
        void MakeITLItemL();
        void MakeT9LItemL();
        /*
        *  @ From 4.0 Req: Autoword completion
        */
        void MakePredictiveModeItemL();
        
        // Available in Japanese variant
        void MakeEngPredTxtItemL();
        void MakeJnpPredTxtItemL();
        void MakeKeypressTimeoutItemL();
        void MakeJapQwertyItemL( TInt aFeatureId );        
        void MakeResetATOKDictionaryItemL();
        
        void MakeAHNumberModeItemL();
        void MakeHindiNumberModeItemL();
        void MakeDIMItemL();
        void MakeCangJieItemL();
#ifdef RD_INTELLIGENT_TEXT_INPUT
        void MakePredictiveOptionsItemL();
#endif

    private: // New
        // Language setting page items
        CDesCArrayFlat* iDtLanguageItems;
        // Obtained from SysLangUtil
        CArrayFixFlat<TInt>* iDtLanguagesCodes;
        // T9 language codes
        CArrayFixFlat<TInt>* iWtLanguagesCodes;
        // T9 language menu items
        CDesCArrayFlat* iWtLanguageItems;
        // T9 setting page items
        CDesCArrayFlat* iOnOffItems;
        // T9 related
        HBufC* iOnOffNotAvailable;
        // A&H Number mode items.
        CDesCArrayFlat* iAHNumberModeItems;
        // Hindi Number mode items.
        CDesCArrayFlat* iHindiNumberModeItems;
        // Default Input Method items
        CDesCArrayFlat* iInputMethodItems;
        // T9 setting page items
        CDesCArrayFlat* iJapPredTxtOnOffItems;
        //Keypress Timeout setting page items
        CDesCArrayFlat* iKeypressTimeoutItems;
        //Japanese Qwerty Comma setting page items
        CDesCArrayFlat* iJapQwertyCommaItems;
        //Japanese Qwerty Period setting page items
        CDesCArrayFlat* iJapQwertyPeriodItems;
        //Japanese Qwerty Width Of Space setting page items
        CDesCArrayFlat* iJapQwertyWidthOfSpaceItems;
        //Chinese CangJie input mode
        CDesCArrayFlat* iChiCangJieItems;
        //Predictive mode :  Autoword completion Req
        CDesCArrayFlat* iPredictiveModeItems;
        
        CGSListBoxItemTextArray* iListboxItemArray;
        TBool iUpdateITL;
        CGSLangModel* iModel;
    };

#endif // GSLANGCONTAINER_H
// End of File
