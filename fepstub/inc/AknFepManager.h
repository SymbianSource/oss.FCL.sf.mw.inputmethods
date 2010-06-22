/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CAknFepManager definition.
*
*/












#ifndef __AKN_FEP_MANAGER_H__
#define __AKN_FEP_MANAGER_H__

#include <gulicon.h>  
#include <fepbase.h>                    //CCoeFep
#include <aknedstsobs.h>                //MAknEdStateObserver
#include <uikon.hrh>                    //TAknEditorNumericKeymap
#include <aknEditStateIndicator.h>      //TAknEditingState

class CAknEdwinState;
/**
 * The CCoeFep derived class of AknFep
 * Controls mode in the Fep. Handles all interaction with the editor
 * Creates and owns the UI Manger, the Key Catcher and the edit state indicators
 */
class CAknFepManager :
    public CCoeFep,
    private MAknEdStateObserver
    {

public:

public:
    /**
     * C++ Constructor
     */
    CAknFepManager(CCoeEnv& aConeEnvironment);

    /**
     *  Second phase construction
     */
    void ConstructL(const CCoeFepParameters& aFepParameters);

    /**
     * destructor
     */
    virtual ~CAknFepManager();
public: // from MAknEdStateObserver
    /**
     * from MAknEdStateObserver - not used
     */
    virtual void HandleAknEdwinStateEventL(CAknEdwinState* aAknEdwinState, 
                                           EAknEdwinStateEvent aEventType);
//private:        //from MFepInlineTextFormatRetriever
//    /**
//     * from MFepInlineTextFormatRetriever - not needed, has to be implemented for MCoeFepAwareTextEditor interface
//     */
//    virtual void GetFormatOfFepInlineText(TCharFormat& aFormat, 
//                                          TInt& aNumberOfCharactersWithSameFormat, 
//                                          TInt aPositionOfCharacter) const;
private:        // from MCoeFocusObserver (via CCoeFep)

    virtual void HandleChangeInFocus();

    /**
     * in queries the editor is destroyed before the call to HandleChangeInFocus
     * so NULL the iInputCapabilities variables to prevent trying to save to an unreferenced object
     */
    virtual void HandleDestructionOfFocusedItem();

private:    // from MCoeForegroundObserver (via CCoeFep)
    /**
     * from MCoeForegroundObserver (via CCoeFep) - not used
     */
    virtual void HandleGainingForeground();

    /**
     * from MCoeForegroundObserver (via CCoeFep) - not used
     */
    virtual void HandleLosingForeground();

private:    // from MFepAttributeStorer (via CCoeFep)
    /**
     * from MFepAttributeStorer (via CCoeFep) - not used
     */
    virtual TInt NumberOfAttributes() const;

    /**
     * from MFepAttributeStorer (via CCoeFep) - not used
     */
    virtual TUid AttributeAtIndex(TInt aIndex) const;

    /**
     * from MFepAttributeStorer (via CCoeFep) - not used
     */
    virtual void WriteAttributeDataToStreamL(TUid aAttributeUid, RWriteStream& aStream) const;

    /**
     * from MFepAttributeStorer (via CCoeFep) - not used
     */
    virtual void ReadAttributeDataFromStreamL(TUid aAttributeUid, RReadStream& aStream);

private: // from CCoeFep
    /**
     * from CCoeFep - not used
     */
    virtual void CancelTransaction();

    /**
     * from CCoeFep - not used
     */
    virtual void IsOnHasChangedState();

    /**
     * from CCoeFep - not used
     */
    virtual void OfferKeyEventL(TEventResponse& aEventResponse, const TKeyEvent& aKeyEvent, 
                                TEventCode aEventCode);

    /**
     * from CCoeFep - not used
     */
    virtual void OfferPointerEventL(TEventResponse& aEventResponse, 
                                    const TPointerEvent& aPointerEvent, 
                                    const CCoeControl* aWindowOwningControl);

    /**
     * from CCoeFep - not used
     */
    virtual void OfferPointerBufferReadyEventL(TEventResponse& aEventResponse, 
                                               const CCoeControl* aWindowOwningControl);

    
private:
  
    };
#endif //__AKN_FEP_MANAGER_H__    
// End of file
