/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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












#include <aknedsts.h>
#include "AknFepQuery.h"
#include <featmgr.h>                //FeatureManager

CAknFepTextQueryDialog::CAknFepTextQueryDialog(TDes& aDataText, CAknEdwinState& aEditorState, 
    const TTone aTone, const TInt aEditorFlag) :
    CAknTextQueryDialog(aDataText, aTone),
    iUnderlyingEditorState(aEditorState),
    iEditorFlag(aEditorFlag)
    {
    }

CAknFepTextQueryDialog::~CAknFepTextQueryDialog()
    {
    }

void CAknFepTextQueryDialog::PreLayoutDynInitL()
    {
    CAknTextQueryDialog::PreLayoutDynInitL();

    // need to set query editor's case and sct resource based on editor present when this is launched
    CAknQueryControl* ctrl=STATIC_CAST(CAknQueryControl*, ControlOrNull(EGeneralQuery));
    CEikEdwin* editor=STATIC_CAST(CEikEdwin*, ctrl->ControlByLayoutOrNull(EDataLayout));
    if (editor)
        {
        if (iEditorFlag)
	       { 
	       editor->SetAknEditorFlags(editor->AknEdwinFlags() | iEditorFlag );	     
	       }
        TInt currentCase = iUnderlyingEditorState.CurrentCase();
        if (currentCase != 0)
            {
            editor->SetAknEditorCase(iUnderlyingEditorState.CurrentCase());
            }
        else
            {
            editor->SetAknEditorCase(EAknEditorUpperCase);
            }
        if( iUnderlyingEditorState.Flags() & EAknEditorFlagLatinInputModesOnly )
            {
            editor->SetAknEditorFlags(editor->AknEdwinFlags() | EAknEditorFlagLatinInputModesOnly);
            }
        if (FeatureManager::FeatureSupported(KFeatureIdJapanese))
            {
            editor->SetAknEditorFlags(editor->AknEdwinFlags() | EAknEditorFlagLatinInputModesOnly);
            }
        TInt underlyingEditorSCTResId=iUnderlyingEditorState.SpecialCharacterTableResourceId();
        if (FeatureManager::FeatureSupported(KFeatureIdChinese))
            {
            if (!underlyingEditorSCTResId || underlyingEditorSCTResId == R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG)
                {
                underlyingEditorSCTResId = R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG_CHINESE;
                }
            }
        else if (!underlyingEditorSCTResId)
            {
            underlyingEditorSCTResId = R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG;
            }
        editor->SetAknEditorSpecialCharacterTable(underlyingEditorSCTResId);
        }
    }

// End of File
