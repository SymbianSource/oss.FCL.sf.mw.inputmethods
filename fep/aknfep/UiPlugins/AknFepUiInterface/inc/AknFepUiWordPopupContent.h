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
* Description:           
*       Interface through which the FEP can use pop up bubble components
*
*/












#ifndef __AKN_FEP_UI_INTERFACE_WORD_POPUP_CONTENT_H
#define __AKN_FEP_UI_INTERFACE_WORD_POPUP_CONTENT_H

#include <e32base.h>
#include <eikcmobs.h>
#include <avkon.hrh>



class MAknFepUiWordPopupContent 
	{
public:
    // Enum copied from CAknFepExactWordPopupContent
    enum TArrowDirection
            {
            EUpwards,       //!< The arrow included in the popup points upwards
            EDownwards      //!< The arrow included in the popup points downwards
			};
public: 
	virtual ~MAknFepUiWordPopupContent() = 0;
public:
		/**
        * Sets the text to be shown in the popup
        */
        virtual void SetTextL( const TDesC& aText ) = 0;
        
        /**
        * Sets the direction of the arrow included in the popup. This should be used as a hint
        * about the direction to tap to select the word shown in the popup.
        */
        virtual void SetArrowDirection( TArrowDirection aDirection ) = 0;
		/**
        * Sets the delay used before showing the preview popup. The default 
        * delay is one second.
        * @param aDelay Delay in microseconds.
        */
		virtual void SetPopUpShowDelay( const TTimeIntervalMicroSeconds32& aDelay ) = 0;
       
        /**
        * Sets the preview popup visible after specified delay. If the popup
        * is already visible it is hidden immediately and shown again after the
        * showing delay. Popup is automatically hidden after its hiding delay
        * unless the delay is zero in which case the popup is shown infinitely.
        */
        virtual void ShowPopUp() = 0;
        
        /**
        * Hides the popup immediately.
        */
        virtual void HidePopUp() = 0;
        
        /**
        * Sets the position of preview popup. Popup's size is determined by the
        * size of its content. The popup is placed left and down from the given 
        * point. If fixed mode is used then this function has no effect.
        * @param aPoint Popup's position.
        */
        virtual void SetPosition( const TPoint& aPoint ) = 0;

        /**
        * Updates popup's size to reflect a change in content's size.
        * Should be called if the size of the content is changed dynamically.
        */
        virtual void UpdateContentSize() = 0;
        
        /**
        * Returns the popup's size.
        */
        virtual TSize Size() const = 0;   
        
        /**
        * Returns true if the popup is visible.
        */
        virtual TBool IsPopUpVisible() = 0;   
    };

#endif
