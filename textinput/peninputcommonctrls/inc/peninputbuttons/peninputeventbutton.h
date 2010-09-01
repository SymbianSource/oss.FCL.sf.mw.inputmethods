/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface of normal button and multidisplay button
*
*/


#ifndef C_CAKNFEPCTRLEVENTBUTTON_H
#define C_CAKNFEPCTRLEVENTBUTTON_H

// system includes
#include <peninputlayoutbasecontrol.h>
#include <peninputlayoutbutton.h>
#include <peninputbutton.h>
#include <peninputcommonbutton.h>

// forward declarations
class TResourceReader;

// class declarations
/**
 *  Basic functionality for graphic buttons
 *  It provides creation from resource and can react according to SizeChanged Msg
 *
 *  @lib fepcommonctrls.lib
 *  @since S60 v3.2
 */
class CAknFepCtrlEventButton : public CAknFepCtrlCommonButton
    {
public:

    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout The layout
     * @param aControlId The control id
     * @param aEvent An event Id to be sent
     * @param aUnicode A Unicode value to be sent
     * @return Pointer to created CAknFepCtrlEventButton object
     */
    IMPORT_C static CAknFepCtrlEventButton* NewL(CFepUiLayout* aUiLayout, 
                                                 TInt aControlId, 
                                                 TInt aEvent = 0xFFFF,
                                                 TInt aUnicode = 0,
											      TAknsItemID aNormalID = KAknsIIDQsnFrFunctionButtonNormal,
			    								  TAknsItemID aPressedID = KAknsIIDQsnFrFunctionButtonPressed,
			    								  TAknsItemID aInactiveID = KAknsIIDQsnFrFunctionButtonInactive);

    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout The layout
     * @param aControlId The control id
     * @param aEvent An event Id to be sent
     * @param aUnicode A Unicode value to be sent
     * @return Pointer to created CAknFepCtrlEventButton object
     */                                           
    IMPORT_C static CAknFepCtrlEventButton* NewLC(CFepUiLayout* aUiLayout, 
                                                  TInt aControlId, 
                                                  TInt aEvent = 0xFFFF,
                                                  TInt aUnicode = 0,
											      TAknsItemID aNormalID = KAknsIIDQsnFrFunctionButtonNormal,
			    								  TAknsItemID aPressedID = KAknsIIDQsnFrFunctionButtonPressed,
			    								  TAknsItemID aInactiveID = KAknsIIDQsnFrFunctionButtonInactive);

    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual ~CAknFepCtrlEventButton();

    /**
     * From CButtonBase
     * Handle button up event. cancel all timers
     *
     * @since S60 v3.2
     * @param aPoint Point position
     * @return The control who handles the event
     */
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);
    
    /**
     * This function gets the current Event Id
     *
     * @since S60 v3.2
     * @return Current Event Id
     */      
    IMPORT_C TInt Event() const;
    
    /**
     * This function gets the current Unicode value
     *
     * @since S60 v3.2
     * @return Current Unicode value
     */    
    IMPORT_C TInt Unicode() const;
    
    /**
     * This function sets the Event Id sent by this button
     *
     * @since S60 v3.2
     * @param aEvent An event Id to be sent
     * @return None
     */
    IMPORT_C void SetEvent(TInt aEvent);
    
    /**
     * This function sets the Unicode value sent by this button
     *
     * @since S60 v3.2
     * @param aUnicode A Unicode value to be sent
     * @return None
     */
    IMPORT_C void SetUnicode(TInt aUnicode);           

    /**
     * This function sets the text written on the button
     *
     * @since S60 v3.2
     * @param aText the text will be shown
     * @return None
     */
    IMPORT_C void SetText(const TDesC& aText);

    /**
     * This function set the font used to draw the text
     *
     * @since S60 v3.2
     * @param aFont the font
     * @return None
     */
    IMPORT_C void SetFont(const CFont* aFont);

    /**
     * This function set the shadow font used to draw the text
     *
     * @since S60 v3.2
     * @param aFont the shadow font
     * @return None
     */
    IMPORT_C void SetShadowFont(const CFont* aFont);

    /**
     * This function set the font color used to draw the text
     *
     * @since S60 v3.2
     * @param aColor the font color
     * @return None
     */
    IMPORT_C void SetFontColor(const TRgb aColor);

    /**
     * This function set the shadow font color used to draw the text
     *
     * @since S60 v3.2
     * @param aColor the shadow font color
     * @return None
     */
    IMPORT_C void SetShadowFontColor(const TRgb aColor);

    /**
     * Draw function
     *
     * @since S60 v3.2
     * @return None
     */        
    IMPORT_C void Draw();
    
protected:

    /**
     * C++ constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout Ui Layout who contains this control
     * @param aControlId Control Id
     * @param aEvent An event Id to be sent
     * @param aUnicode A Unicode value to be sent
     * @return None 
     */
    IMPORT_C CAknFepCtrlEventButton(CFepUiLayout* aUiLayout, 
                                    TInt aControlId, 
                                    TInt aEvent,
                                    TInt aUnicode,
							        TAknsItemID aNormalID = KAknsIIDQsnFrFunctionButtonNormal,
								    TAknsItemID aPressedID = KAknsIIDQsnFrFunctionButtonPressed,
								    TAknsItemID aInactiveID = KAknsIIDQsnFrFunctionButtonInactive);
    /**
     * Display button caption.
     *
     * @since S60 v3.2
     * @return None 
     */
    IMPORT_C void DisplayText();
    
private:

    /**
     * Event Id
     */
    TInt iEvent;
    
    /**
     * Unicode value
     */    
    TInt iUnicode;    
    
    /**
     * Text shown on button
     */    
    HBufC* iText;
    
    /**
     * the font used to show the text
     */    
    const CFont* iFont;    

    /**
     * the shadow font used to show the text
     */    
    const CFont* iShadowFont;    
    
    /**
     * the font color used to show the text
     */    
    TRgb iFontColor;    

    /**
     * the shadow font color used to show the text
     */    
    TRgb iShadowFontColor;    
    };  

#endif // C_CAKNFEPCTRLEVENTBUTTON_H

// End Of File
