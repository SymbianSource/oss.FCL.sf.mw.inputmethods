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




#include <aknnotewrappers.h> //CAknWarningNote
#include <eikedwin.h> //CEikEdwin 

#include <AknFep.rsg>

#include "aknfepuserdbquerydlg.h"
#include "dbmanagement.h"

const TInt KErrMsgMaxLength = 60;
const TInt KTextMaxLength = 10;

CAknFepUserDBQueryDlg::CAknFepUserDBQueryDlg( CDBManagement* aDBEngine, 
    TDes& aText, const TTone& aTone ) : 
    CAknTextQueryDialog ( aText, aTone ), iDBEngine(aDBEngine)
    {
    iptrbuf = & aText;
    iItemText = NULL;
    }

CAknFepUserDBQueryDlg::~CAknFepUserDBQueryDlg()
    {
    if ( iItemText )
        {
        delete iItemText;
        iItemText = NULL;
        }
    }

CAknFepUserDBQueryDlg* CAknFepUserDBQueryDlg::NewLC( CDBManagement* aDBEngine, 
    TDes& aText, const TDesC& aPrompt, const TTone& aTone )
    {
    CAknFepUserDBQueryDlg* self = new (ELeave)CAknFepUserDBQueryDlg( aDBEngine, aText, aTone );
    CleanupStack::PushL(self);
    self->ConstructL( aPrompt );
    return self;
    }

CAknFepUserDBQueryDlg* CAknFepUserDBQueryDlg::NewL( CDBManagement* aDBEngine, 
    TDes& aText, const TDesC& aPrompt, const TTone& aTone )
    {
    CAknFepUserDBQueryDlg* self=CAknFepUserDBQueryDlg::NewLC( aDBEngine, aText, aPrompt, aTone );
    CleanupStack::Pop( self ); // self;
    return self;
    }

void CAknFepUserDBQueryDlg::ConstructL( const TDesC& aPrompt )
    {
    SetPromptL( aPrompt );
    }

TBool CAknFepUserDBQueryDlg::OkToExitL( TInt aButtonId )
    {
    TBuf<KErrMsgMaxLength> errMsg;
    if (aButtonId == EAknSoftkeyOk)
        {
        CAknQueryControl* control = 
            static_cast<CAknQueryControl*>(ControlOrNull(EGeneralQuery));
        CEikEdwin* editor = static_cast<CEikEdwin*>
            ( control->ControlByLayoutOrNull( EDataLayout ) );

        iItemText = editor->GetTextInHBufL();
        TBuf<KTextMaxLength> textbuf;
        textbuf.Copy( iItemText->Des() );
        
        if( !iDBEngine->InputPhraseCheck( textbuf ) )
            {
            iptrbuf->Zero();
            iptrbuf->Copy( textbuf );
            
            iDBEngine->GetErrMsg( errMsg );
            errMsg.Copy( errMsg.Mid( 1 ) );
            
            
            if ( iItemText )
                {
                delete iItemText;
                iItemText = NULL;
                }
            editor->SetTextL( &textbuf ); 
            //note fot err message
            CAknWarningNote* note = new( ELeave ) CAknWarningNote();
            note->SetTimeout( CAknNoteDialog::EShortTimeout );
            note->ExecuteLD( errMsg ); 

            UpdateLeftSoftKeyL();
            return EFalse;
            }
        iptrbuf->Zero();
        iptrbuf->Copy( textbuf );
        if ( iItemText )
            {
            delete iItemText;
            iItemText = NULL;
            }
        return ETrue;
        }
    else
        {
        if (aButtonId == EAknSoftkeyCancel)
            {
            return ETrue;
            }
        }
    return ETrue;
    }


void CAknFepUserDBQueryDlg::PostLayoutDynInitL()
    {
    CAknQueryControl* control = 
        static_cast<CAknQueryControl*>(ControlOrNull(EGeneralQuery));
    CEikEdwin* editor = static_cast<CEikEdwin*>
        ( control->ControlByLayoutOrNull( EDataLayout ) );
    editor->SetTextL( iptrbuf );
    SetFocus(ETrue);
    SetFocusing(ETrue);
    }

// End of file
