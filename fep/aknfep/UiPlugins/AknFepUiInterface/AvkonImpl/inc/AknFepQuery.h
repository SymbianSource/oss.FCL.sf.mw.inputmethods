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












#ifndef __AKNFEPQUERY__
#define __AKNFEPQUERY__

#if !defined(__AKNQUERYDIALOG_H__)
#include <AknQueryDialog.h>
#endif

class CAknEdwinState;

class CAknFepTextQueryDialog : public CAknTextQueryDialog
    {
    public:
        CAknFepTextQueryDialog(TDes& aDataText, CAknEdwinState& aEditorState, 
            const TTone aTone = ENoTone, const TInt aEditorFlag = 0);
        virtual ~CAknFepTextQueryDialog();
    protected: //from CEikDialog
        void PreLayoutDynInitL();
    private:
        CAknEdwinState& iUnderlyingEditorState; // not owned
        TInt iEditorFlag;
    };

#endif

// End of File
