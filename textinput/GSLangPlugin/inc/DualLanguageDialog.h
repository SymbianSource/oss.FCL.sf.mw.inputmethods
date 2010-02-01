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
#ifndef __DUALLANGUAGE_DIALOG_H
#define __DUALLANGUAGE_DIALOG_H

#include <AknDialog.h>
#include "DualLanguageList.h"
#include "DualLanguageData.h"

class CGSLangModel;

class CGSDualLanguageDialog : public CAknDialog
{
    public:
        static CGSDualLanguageDialog* NewL(TInt aMenuResId, CGSLangModel* aModel);
        static CGSDualLanguageDialog* NewLC(TInt aMenuResId, CGSLangModel* aModel);
        virtual ~CGSDualLanguageDialog();
        
        virtual SEikControlInfo CreateCustomControlL(TInt aControlType);
        virtual CGSDualLanguageDialog::TFormControlTypes ConvertCustomControlTypeToBaseControlType(TInt aControlType) const;

        virtual TBool OkToExitL(TInt aButtonId);
        virtual void ProcessCommandL(TInt aCommandId);
        void EditItemL(TBool aCalledFromMenu);
        
    private:
        CGSDualLanguageDialog(CGSLangModel* aModel);
        void ReadSettingsDataL();
        void SaveSettingsDataL() const;
        
        TInt  iPredictiveTextFlag;
        
    protected:
    CGSDualLanguageList*   iSettingsList;
    CGSDualLanguageData*   iLangData;
    CGSLangModel*          iModel;
    CArrayFixFlat<TInt>*   iLanguages;
    CArrayFixFlat<TInt>*   iSecondaryLanguages;
};
#endif //__DUALLANGUAGE_DIALOG_H
