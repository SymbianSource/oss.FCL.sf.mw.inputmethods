/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  CTruiContainerBase class of TrainingUI
*
*/


#ifndef C_TRUICONTAINERBASE_H
#define C_TRUICONTAINERBASE_H


#include <coecntrl.h>
#include <eiklabel.h>

class CTruiAppUi;
class CTruiEngine;
class MCoeControlObserver;
class CAknButton;
class CAknToolbar;
class TAknLayoutText;
class CAknView;

// Varity of landscape
const TInt KLandscapeVarity = 1;

// Varity of portrait
const TInt KPortraitVarity = 0;

const TInt KLabelMaxLines = 20;

/* Key lead to the setting of always displaying wizard view*/
const TUint32 KAlwaysDisplayWizardKey              = 0x00000001;

_LIT( KLineBreak, "\n" );

const TUid KAknfepUID = { 0x100056de };

const TInt KExtraSize = 10;

/**
 *  Base class for contrainers
 *  All contrainer need to be inherited from this class
 *
 */
class CTruiContainerBase : public CCoeControl
    {
public:

    /**
    * Destructor.
    */
    virtual ~CTruiContainerBase();
    
    /**     
     * Get the origin size in which input the model for preset symbols. 
     *
     * @return The size in which to input preset symbols.
     */
    static TSize OriginSymbolSize();
    inline void SetView( CAknView* aView )
        {
        iView = aView;
        };
protected:
    CTruiContainerBase();
    
    /**     
     * Wrap the text to fit the minisize of Label
     *
     * @param aLabel The pointer to the label.
     * @param aText The origin text
     * @param aMaxLines The max line of the wrapped text.     
     * @param aWrappedText Store the wrapped text.
     * @return The number of lines.
     */    
    TInt WrapLabelText( const CEikLabel* aLabel, const TDesC& aText, 
                        TInt aMaxLines, HBufC* aWrappedText );
                        
    /**     
     * Wrap the text to fit the minisize of Label
     *
     * @param aLayoutText Layout of text.     
     * @param aText The origin text
     * @param aMaxLines The max line of the wrapped text.     
     * @param aWrappedText Store the wrapped text.
     * @return The number of lines.
     */
    TInt WrapLabelText(  const TAknLayoutText& aLayoutText, const TDesC& aText,
                         TInt aMaxLines, HBufC* aWrappedText );
                         
    /**     
     * Wrap the text to fit  Label
     *
     * @param aFont Font of text.     
     * @param aText The origin text need to be wrapped
     * @param aLineWidthArray Line width array.     
     * @param aWrappedText Store the wrapped text.     
     */
    void WrapLabelTextL(  const CFont* aFont, const TDesC& aText,
                          const RArray<TInt>& aLineWidthArray, 
                          HBufC* aWrappedText );


    /**     
     * Create skin specified button from resource
     *
     * @param aResourceId The resource Id.
     * @param aParent The parent of control
     * @param aObserver The observer of control
     * @return Instance of aknbutton.
     */
    CAknButton* CreateButtonL( TInt aResourceId, 
                               const CCoeControl* aParent, 
                               MCoeControlObserver* aObserver );
                               
    /**     
     * Add skin specified buttons into toolbar
     *
     * @param aToolbar The toolbar will contain skin-specified buttons.
     * @param aResourceId Button groups resource id.
     */
    void AddButtonsToToolbarL( CAknToolbar* aToolbar, TInt aResourceId );
    
     /**
     * Return current keyboard mode.
     *
     * @return 0: ITU-T; 1: QWERTY.
     */    
    TInt CurrentKeyBoardModeL(); 
    
    /**
     * Read setting from ini file to decide if display wizard view.
     *
     * @param aKey Key value in ini file.
     * @param aValue Value of the key.
     */
    void GetSettingFromIniFileL( TUint32 aKey, TInt& aValue );
    
    /**
     * Save settings into file
     * 
     * @param aKey Key value to specify one setting
     * @param aValue Value to be stored
     */
    void SaveSettingIntoIniFileL( TUint32 aKey, TInt aValue );
    
    /**
     * Get cached color of skin for label control
     * 
     * @param aColor The color of Label will be set    
     */
    TInt GetCachedLabelTextColor( TRgb& aColor );
    
    /**
     * Get cached color of skin for icon
     * 
     * @param aColorId The color id of icon color group
     * @param aColorIndex The color index in the color group
     * @param aColor On return, the catched color.
     */
    TInt GetCachedIcontColor( TRgb& aColor, const TAknsItemID& aColorId, TInt aColorIndex );
    
    /**
     * Construct icons from resource.
     *
     * @param aResourceId Resource Id.
     * @return The constructed icon
     */
    CGulIcon* ConstructIconFromResourceL( TInt aResourceId );

    /**
     * From CoeControl.
     * Handles a change to the control's resources.
     *
     * @param aType A message UID value.
     */
    void HandleResourceChange( TInt aType );    
    
private:
    /**     
     * Create skin specified button from resource
     *
     * @param aReader The resource reader.
     * @return Instance of aknbutton.
     */
    CAknButton* ConstructButtonFromResourceL( TResourceReader& aReader );
    
    /**     
     * Wrap the text to fit the minisize of Label
     *
     * @param aFont Font of text.
     * @param aWidth Width of text.
     * @param aText The origin text
     * @param aMaxLines The max line of the wrapped text.     
     * @param aWrappedText Store the wrapped text.
     * @return The number of lines.
     */
    TInt DoWrapLabelText(  const CFont* aFont, TInt aWidth, 
                           const TDesC& aText, 
                           TInt aMaxLines, 
                           HBufC* aWrappedText );

        
protected: // data

    /**
     * Point to the controlor class of the application.
     * Not Own.
     */
    CTruiAppUi* iAppUi;
    
    /**  
     * hand writing engine
     * Not Own.
     */
    CTruiEngine* iEngine;
    CAknView* iView;
    };

#endif // C_TRUICONTAINERBASE_H

