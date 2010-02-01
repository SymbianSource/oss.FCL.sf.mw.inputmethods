/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef PLUGINKRFEPMANAGERBASE_H
#define PLUGINKRFEPMANAGERBASE_H

#include <e32std.h>
#include <e32base.h>
#include <AknFepManagerInterface.h>
#include <PeninputClient.h>

class CPeninputPluginKr;
class RPeninputServer;
class CRepository;
class CAknEdwinState;

class CPluginKrFepManagerBase : public CBase, public MAknFepManagerInterface
    {
public:
    
    virtual ~CPluginKrFepManagerBase();

protected:

    CPluginKrFepManagerBase(
        CPeninputPluginKr& aOwner, 
        RPeninputServer& aPenInputServer);

    void ConstructL();
    
private:

    virtual void SetMode(
        TInt aMode, 
        TBool aPredictive, 
        TBool aQwertyInputMode,
        TInt aKeyboardType = EPtiKeyboardNone);

    virtual void SetCase(TCase aCase);

    virtual void SetNumberModeKeyMappingL(
        TAknEditorNumericKeymap aAknEditorNumericKeymap);

    virtual TBool HandleKeyL(TInt aKey, TKeyPressLength aLength, TEventCode aEventCode = EEventKey);

    virtual void HandleCommandL(TInt aCommandId);
    
    virtual void HandleCommandL(TInt aCommandId, TInt aParam);

    virtual void CloseUI();

    virtual void ExpireMultitapTimer();

    virtual TBool IsValidNumericLongKeyPress(TInt aKey) const;

    virtual void AddTextToUserDictionaryL(const TDesC& aText);

    virtual void GetFormatOfFepInlineText(
        TCharFormat& aFormat, 
        TInt& aNumberOfCharactersWithSameFormat, 
        TInt aPositionOfCharacter) const;

    virtual TInt SupportLanguage(TInt aMode) const;

    virtual void SetInputLanguageL(TLanguage aLanguage);

    virtual TBool IsValidShiftKeyPress() const;

    virtual void SetEditorContext(TInt aContext);

    virtual void ActivateUI();

    virtual void SetFepAwareEditorText(const TFepInputContextFieldData& /*aIcfData*/);

    virtual TInt EditorMaxLength();
    
    virtual void ResourceChanged(TInt aType);
        
    virtual void SetNextFepUI(MAknFepManagerInterface* /*aNextUi*/);

    virtual void SetStarKeyFlag( TBool aSet );
    
    virtual TBool IsValidFnKeyPress() const
		{
		return EFalse;
		};
    virtual TBool IsValidLongChrKeyPress() const
		{
        return EFalse;
		};
public:
    
    virtual void OnInit();
    
    virtual void RetrieveEditorMaxLength();
    
    virtual void UpdateRange( TInt aRange );
    
    virtual void UpdateCaseMode( TInt aCaseMode );
    
protected:
    
    void SendCommandToServer( TInt aCommandId );
    
    void SendCommandToServer( TInt aCommandId, TInt aParam );
    
    void SendCommandToServer(TInt aCommandId, const TDesC8& aParam);

    void SetLayoutRange(const CAknEdwinState* aEditorState);
    
    void SetLayoutPermitedRanges( TInt aPermittedMode );
    
    TBool SupportNativeNumberRange();
    
protected:

    CPeninputPluginKr& iOwner;

    RPeninputServer& iPenInputServer;

    TInt iPermittedRange;
    
    TInt iHkbMode;

    TInt iSuggestedRange;
    
    TLanguage iLanguage;   
    
private:

    CRepository* iRepository;

    TInt iMaxEditorLength;

    TInt iLastCase;
    
    TInt iLastRange;

    MAknFepManagerInterface* iHkbUi;
    
    TInt iPermittedMode;
    };

#endif // PLUGINKRFEPMANAGERBASE_H
