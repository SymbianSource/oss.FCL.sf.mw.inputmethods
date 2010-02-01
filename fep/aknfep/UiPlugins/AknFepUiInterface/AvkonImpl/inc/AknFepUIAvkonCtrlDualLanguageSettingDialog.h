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










#ifndef __AKNFEPDUALLANGUAGESETTINGDIALOG_H
#define __AKNFEPDUALLANGUAGESETTINGDIALOG_H

#include <AknDialog.h>
#include "AknFepUIAvkonCtrlDualLanguageSettingList.h"
#include "AknFepUIAvkonCtrlDualLanguageSettingData.h"

class CAknFepDualLanguageSettingModel;

class CAknFepDualLanguageSettingDialog : public CAknDialog
{
    public:
        static CAknFepDualLanguageSettingDialog* NewL(TInt aMenuResId);
        static CAknFepDualLanguageSettingDialog* NewLC(TInt aMenuResId);
        virtual ~CAknFepDualLanguageSettingDialog();
        
        virtual SEikControlInfo CreateCustomControlL(TInt aControlType);
        virtual CAknFepDualLanguageSettingDialog::TFormControlTypes ConvertCustomControlTypeToBaseControlType(TInt aControlType) const;

        virtual TBool OkToExitL(TInt aButtonId);
        virtual void ProcessCommandL(TInt aCommandId);
        void EditItemL(TBool aCalledFromMenu);
        
    private:
        CAknFepDualLanguageSettingDialog();
        void ReadSettingsDataL();
        void SaveSettingsDataL() const;
        
        TInt  iPredictiveTextFlag;
        
    protected:
    CAknFepDualLanguageSettingList*   iSettingsList;
    CAknFepDualLanguageSettingData*   iLangData;
    CAknFepDualLanguageSettingModel*          iModel;
    CArrayFixFlat<TInt>*   iLanguages;
    CArrayFixFlat<TInt>*   iSecondaryLanguages;
};
#endif //__AKNFEPDUALLANGUAGESETTINGDIALOG_H
