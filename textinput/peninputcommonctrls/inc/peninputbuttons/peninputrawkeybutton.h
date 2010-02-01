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
#ifndef C_CAKNFEPCTRLRAWKEYBUTTON_H
#define C_CAKNFEPCTRLRAWKEYBUTTON_H

// system includes
#include <AknsConstants.h>
#include <peninputlayoutbasecontrol.h>
#include <peninputlayoutbutton.h>
#include <peninputcommonbutton.h>

// forward declarations
class TResourceReader;

class CAknFepCtrlRawKeyButton : public CAknFepCtrlCommonButton
    {
public:

    IMPORT_C static CAknFepCtrlRawKeyButton* NewL(CFepUiLayout* aUiLayout, 
                                                  TInt aControlId,
                                                  TAknsItemID aNormalID,
    											  TAknsItemID aPressedID,
    											  TAknsItemID aInactiveID,
                                                  TInt aDownEvent = 0xffff,
                                                  TInt aUpEvent = 0xffff,
                                                  TInt aScanCode = 0);

    IMPORT_C static CAknFepCtrlRawKeyButton* NewLC(CFepUiLayout* aUiLayout, 
                                                   TInt aControlId,                                                    
                                                   TAknsItemID aNormalID,
    											   TAknsItemID aPressedID,
    											   TAknsItemID aInactiveID,
                                                   TInt aDownEvent = 0xffff,
                                                   TInt aUpEvent = 0xffff,
                                                   TInt aScanCode = 0);

    IMPORT_C virtual ~CAknFepCtrlRawKeyButton();

    IMPORT_C CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
    IMPORT_C CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);
    IMPORT_C void HandlePointerLeave(const TPoint& aPoint);
    IMPORT_C void ConstructFromResourceL(TResourceReader& aReader);
    IMPORT_C void ConstructFromResourceL();
    IMPORT_C void OnDeActivate();
    
    inline void SetDownEvent(TInt aEvent);
    inline void SetUpEvent(TInt aEvent);
    inline TInt DownEvent() const;
    inline TInt UpEvent() const;
    inline TInt ScanCode() const;
    inline void SetScanCode(TInt aScanCode);
    
protected:

    IMPORT_C CAknFepCtrlRawKeyButton(CFepUiLayout* aUiLayout, 
                                     TInt aControlId,                         
                                     TAknsItemID aNormalID,
									 TAknsItemID aPressedID,
									 TAknsItemID aInactiveID, 
                                     TInt aDownEvent,
                                     TInt aUpEvent,
                                     TInt aScanCode);

private:

    TInt iDownEvent;
    TInt iUpEvent;
    TInt iScanCode;
    // indicate whether last event is pointer down event
    TBool iLastPntDownEvent;
    };  

#include "peninputrawkeybutton.inl"
#endif // C_CAKNFEPCTRLRAWKEYBUTTON_H

// End Of File
