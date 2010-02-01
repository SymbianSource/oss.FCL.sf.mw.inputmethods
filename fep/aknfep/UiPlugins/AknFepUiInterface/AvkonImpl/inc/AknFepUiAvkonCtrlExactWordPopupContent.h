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
* Description:             Contents of the exact word tooltip shown above inline editor
*                when the exact word differs from the inline word in Western
*                QWERTY predictive mode.
*
*/












#ifndef __AKN_FEP_EXACT_WORD_POPUP_CONTENT_H 
#define __AKN_FEP_EXACT_WORD_POPUP_CONTENT_H
#include <coecntrl.h>
#include "AknFepUiWordPopupContent.h"

// FORWARD DECLARATIONS
class CAknPreviewPopUpController;


// CLASS DECLARATION
class CAknFepExactWordPopupContent : public CCoeControl, public MAknFepUiWordPopupContent
    {
    public: // construction and destruction
    
        /**
        * Two phased constructor
        */
        static CAknFepExactWordPopupContent* NewL();
        
        /**
        * Destructor
        */
        ~CAknFepExactWordPopupContent();
        
    private: // construction
        
        /**
        * C++ constructor
        */
        CAknFepExactWordPopupContent();
        
        /**
        * Second phase constructor
        */
        void ConstructL();
        
    public: // new interface functions
    
        /**
        * Sets the text to be shown in the popup
        */
        void SetTextL( const TDesC& aText );
        
        /**
        * Sets the popup controller pointer. This must be called before drawing the popup contents.
        * @param    aController Pointer to the popup for which this object provides the content.
        
        void SetPopupController( CAknPreviewPopUpController* aController );
		*/
		
        /**
        * Sets the direction of the arrow included in the popup. This should be used as a hint
        * about the direction to tap to select the word shown in the popup.
        */
        void SetArrowDirection( TArrowDirection aDirection );

    public: // functions from base classes
    	/**
     	* Returns the minimuz size of this preview popup content.
     	*/    
        TSize MinimumSize();        
    
    public: //new interface functions
    	/**
        * Sets the delay used before showing the preview popup. The default 
        * delay is one second.
        * @param aDelay Delay in microseconds.
        */
    	void SetPopUpShowDelay( const TTimeIntervalMicroSeconds32& aDelay );
    	/**
        * Sets the preview popup visible after specified delay. If the popup
        * is already visible it is hidden immediately and shown again after the
        * showing delay. Popup is automatically hidden after its hiding delay
        * unless the delay is zero in which case the popup is shown infinitely.
        */
        virtual void ShowPopUp();
        
        /**
        * Hides the popup immediately.
        */
        virtual void HidePopUp();
        /**
        * Sets the position of preview popup. Popup's size is determined by the
        * size of its content. The popup is placed left and down from the given 
        * point. If fixed mode is used then this function has no effect.
        * @param aPoint Popup's position.
        */
        virtual void SetPosition( const TPoint& aPoint );

        /**
        * Updates popup's size to reflect a change in content's size.
        * Should be called if the size of the content is changed dynamically.
        */
        virtual void UpdateContentSize();
        
        /**
        * Returns the popup's size.
        */
        virtual TSize Size() const; 
		
		/**
        * Returns true if the popup is visible.
        */
        virtual TBool IsPopUpVisible();
        virtual void HandlePointerEventL( const TPointerEvent& aPointerEvent )	;        
     
    private: // functions from base classes
    
        /**
        * Draws the contents of the popup
        * @aRect    The area which should be redrawn.
        */
        void Draw( const TRect& aRect ) const;
        
        /**
        * Draws the upwards or downwards pointing arrow, depending on the value
        * set by SetArrowDirection(). The upward arrow is placed in the bottom of the popup
        * while the downward arrow is drawn on the top part of the popup.
        * @aTextRect    The area of the poup which is reserved for the text.
        */ 
        void DrawArrowL( const TRect& aTextRect ) const;
        
    private: // data
    	TBool iIsPopUpVisible;
        //! The text label to be shown in the popup
        CEikLabel* iLabel;
        
        //! The popup where this object provides the content.
        CAknPreviewPopUpController* iPopupController; // not owned
        
        //! Tells the direction of the arrow included in the popup
        MAknFepUiWordPopupContent::TArrowDirection iArrowDirection;
    };

#endif // AKN_FEP_EXACT_WORD_POPUP_CONTENT_H