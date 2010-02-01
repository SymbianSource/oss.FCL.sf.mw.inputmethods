/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Define query dialog for user DB management
*
*/












#ifndef C_AKNFEPUSERDBQUERYDLG_H
#define C_AKNFEPUSERDBQUERYDLG_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <AknQueryDialog.h> 

// CLASS DECLARATION
class CDBManagement;


/**
 *  Query dialog for user DB management
 *  In this query dialog, it will check phrase and show note
 *
 *  @code
 *   ?good_class_usage_example(s)
 *  @endcode
 *
 *  @lib ?library
 *  @since S60 v3.1
 */
class CAknFepUserDBQueryDlg : public CAknTextQueryDialog 
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CAknFepUserDBQueryDlg();

    /**
     * Two-phased constructor.
     * @param aDBEngine Pointer of DBEngine
     * @param aText Text which for initialization of query dialog
     * @param aPrompt Prompt for query
     * @param aTone The tone played before the dialog is shown.
     *              Default No tone is played. 
     */
    static CAknFepUserDBQueryDlg* NewL( CDBManagement* aDBEngine, 
                                        TDes& aText, const TDesC& aPrompt, 
                                        const TTone& aTone = ENoTone );

    /**
     * Two-phased constructor.
     * @param aDBEngine Pointer of DBEngine
     * @param aText Text which for initialization of query dialog
     * @param aPrompt Prompt for query
     * @param aTone The tone played before the dialog is shown.
     *              Default No tone is played. 
     */
    static CAknFepUserDBQueryDlg* NewLC( CDBManagement* aDBEngine, 
                                         TDes& aText, const TDesC& aPrompt, 
                                         const TTone& aTone = ENoTone );

private:

    /**
     * C++ constructor.
     * @param aDBEngine Pointer of DBEngine
     * @param aText Text which for initialization of query dialog
     * @param aTone The tone played before the dialog is shown.
     *              Default No tone is played. 
     */
    CAknFepUserDBQueryDlg( CDBManagement* aDBEngine, 
                           TDes& aText,
                           const TTone& aTone = ENoTone );

    /**
     * Symbian second-phase constructor.
     * @param aPrompt Prompt for query
     */
    void ConstructL( const TDesC& aPrompt );
       
    // From base class CAknQueryDialog
    /**
     * From base class CAknQueryDialog
     * Deal the softkey event
     * @param aButtonId softkey Id
     * @return ETrue to validate and exit the dialog, EFalse to keep the dialog active.
     */
    TBool OkToExitL( TInt aButtonId );
    
    /**
     * From base class CAknQueryDialog
     * Initializes the dialog's controls after the 
     * dialog has been sized but before it has been activated
     * @param None
     * @return None
     */
    void PostLayoutDynInitL();
    
private: // data
    
    /**
     * Engine
     * Not own
     */
    CDBManagement* iDBEngine;  
    
    /**
     * Text pointer in query
     * Not own
     */
    TDes* iptrbuf;
    
    /**
     * Text in query
     * Own
     */
    HBufC* iItemText;
    
    };

#endif // C_AKNFEPUSERDBQUERYDLG_H
