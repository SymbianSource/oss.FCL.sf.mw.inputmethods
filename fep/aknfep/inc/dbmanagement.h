/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            DB Engine
*
*/












#ifndef C_CDBMANAGEMENT_H
#define C_CDBMANAGEMENT_H

// CLASS DECLARATION
class CPtiEngine;
class CAknFepSharedDataInterface;
class CAknFepManager;

// CONSTANTS
const TInt KErrorMsgMaxLength = 60;
/**
 *  CDBManagement
 *  User DB management view's DB Engine
 * 
 * 
 *  @lib peninputhwrboxcn.lib
 *  @since S60 v5.0
 */
class CDBManagement : public CBase
    {
    
public:
    
    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * @return Pointer to created CDBManagement object
     */
    static CDBManagement* NewL( CAknFepManager* aFepManager );

    /**
     * Destructor.
     *
     * @since S60 v5.0
     * @return None
     */    
    virtual ~CDBManagement();

    /**
     * Add phrase to DB.
     *
     * @since S60 v5.0
     * @param aPhraseAdd the phrase to be added to DB  
     * @return ETrue if the phrase is added to DB successfully, otherwise EFalse.
     */    
    TBool AddPhraseToDB( const TDesC& aPhraseAdd );
    
    /**
     * Edit phrase from DB.
     *
     * @since S60 v5.0
     * @param aPhraseOld the phrase to be edited before
     * @param aPhraseNew the phrase to be edited after
     * @return ETrue if the phrase is edited from DB successfully, otherwise EFalse.
     */    
    TBool EditPhraseFromDB(  const TDesC& aPhraseOld,
                             const TDesC& aPhraseNew );
    
    /**
     * Remove phrase from DB.
     *
     * @since S60 v5.0
     * @param aPhraseRemove the phrase to be removed
     * @return ETrue if the phrase is remove from DB successfully, otherwise EFalse.
     */    
    TBool RemovePhraseFromDB( const TDesC& aPhraseRemove );
    
    /**
     * Get phrases from DB.
     *
     * @since S60 v5.0
     * @param aPhraseGet the phrases are getde from DB
     * @return ETrue if the phrases are geted from DB successfully, otherwise EFalse.
     */    
    TBool GetPhraseFromDBL( CDesCArray& aPhraseGet);     
    
    /**
     * Get Error Message.
     *
     * @since S60 v5.0
     * @param aErrMsg the Error Message
     * @return None.
     */    
    void GetErrMsg( TDes& aErrMsg );
 
    /**
     * Check the input pharese.
     *
     * @since S60 v5.0
     * @param aPhraseInput  the phrase to be checked
     * @return ETrue if the input phrase are checked successfully, otherwise EFalse.
     */
    TBool InputPhraseCheck( TDes& aPhraseInput );

private:
    
    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @return None
     */
    CDBManagement();
    
    /**
     * Second phase constructor.
     *
     * @since S60 v5.0
     * @return Pointer to created CDBManagement object
     */    
    void ConstructL( CAknFepManager* aFepManager );

    /**
     * set language
     *
     * @since S60 v5.0
     * @return none.
     */    
    void ActivateLanguageL();
    
    /**
     * set language
     *
     * @since S60 v5.0
     * @return none.
     */    
    void ActivateLanguageOfFepL();
    
    /**
     * set language
     *
     * @since S60 v5.0
     * @return none.
     */    
    void ActivateLanguage();
    
    /**
     * set input mode
     *
     * @since S60 v5.0
     * @return ETrue if set input mode successfully, otherwise EFalse.
     */
    TBool SetInputMode( CPtiEngine* aPtiEngine );
    
    /**
     * get db error message
     *
     * @since S60 v5.0
     * @return none.
     */
    void GetDBErrMsg( TInt aErrCode );
    
private: //private data
    
    /**
     * CAknFepSharedDataInterface objcet
     * Own
     */
    CAknFepSharedDataInterface* iAknFepSharedDataInterface;
    
    /**
     * CAknFepSharedDataInterface objcet
     * Own
     */
    CPtiEngine* iPtiEngine;
    
    /**
     * CAknFepSharedDataInterface objcet
     * not Own
     */
    CPtiEngine* iPtiEngineOfFep;
    
    /**
     * CAknFepSharedDataInterface objcet
     * not Own
     */
    CAknFepManager* iFepManager;
        
    /**
     * Error message info
     */
    TBuf<KErrorMsgMaxLength> iErrMsg;
    };

#endif // C_CDBMANAGEMENT_H
