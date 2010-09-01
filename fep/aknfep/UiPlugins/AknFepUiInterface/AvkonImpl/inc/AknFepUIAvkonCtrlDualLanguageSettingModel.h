/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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










#ifndef __AKNFEPDUALLANGUAGESETTINGMODEL_H_
#define __AKNFEPDUALLANGUAGESETTINGMODEL_H_

#include    <e32base.h>
#include <BADESCA.H> //CDesCArray
#include <coemain.h> //For CCoeEnv
#include <centralrepository.h>

class CAknFepDualLanguageSettingModel : public CBase
    {
public:
    static CAknFepDualLanguageSettingModel* NewL();
    enum TLanguageType
                {                
                // Writing language
                EAknFepDualLanguageSettingInputLanguage,
                // Secondary writing language
                EAknFepDualLanguageSettingInputLanguageSecondary
                };
    void GetSecondaryInputLanguagesL(
                  CArrayFix<TInt>* aSeedLanguages,
                  CDesCArray* aLanguageNames,
                  CCoeEnv* aCoeEnv);
    void ShowDualInputLanguageSettingListL(CCoeEnv* aCoeEnv);
    
    void GetLanguagesFromPtiL(
        CArrayFix<TInt>* aInstalledLangCodes,
        CArrayFix<TInt>* aSeedLanguages,
        CDesCArray* aLanguageNames );
    TInt LanguageL( TLanguageType aType );
    TBool SetLanguageL( TLanguageType aType, TInt aLanguage );
    
    /*
    * Returns Bool value according to predictive dictionary availability.
    * PTI Engine used for lang avaibility retrieving
    * @return ETrue: no errors
    *         EFalse: an error has occurred
    * @since 2.6
    */
    TBool CheckDictionaryFromPtiL(TInt aLangCode );
    ~CAknFepDualLanguageSettingModel();
private:
    CAknFepDualLanguageSettingModel(){};
    void ConstructL();
private:
    CRepository* iAknFepRepository;
    };
#endif //__AKNFEPDUALLANGUAGESETTINGMODEL_H_
