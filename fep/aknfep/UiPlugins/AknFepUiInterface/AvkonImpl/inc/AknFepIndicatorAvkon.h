/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CAknFepIndicatorAvkon definition.
*
*/












#ifndef AknFepAvkonIND_H
#define AknFepAvkonIND_H

#include <AknFepIndicator.h>
#include <aknEditStateIndicator.h>  //MAknEditingStateIndicator

class CAknNavigationDecorator;
class CAknNavigationControlContainer;
class CEikonEnv;

class CAknFepIndicatorAvkon : public CAknFepIndicator
    {
public:
    static CAknFepIndicator* NewL();
    ~CAknFepIndicatorAvkon();
    virtual void SetState(TAknEditingState aState); 
    virtual CAknIndicatorContainer* IndicatorContainer();

public: // new functions    
    /**
     * Sets flag that copy mode is ON
     */    
    void SetCopyMode(TBool aCopyMode); 

private:
    CAknFepIndicatorAvkon();
    void AddContainerL(CAknNavigationControlContainer* aNaviPane);
    void RemoveContainer();
    void SetIndicator(TInt aIndicator, TInt aMode, TBool aDrawNow = ETrue);
    CAknNavigationControlContainer* NaviPaneL();
    
     /** Since 3.2
 	  * 	 Returns True if Predictive Mode is On so that - the appropriate indicator is set
 	  */
    TBool QueryPredictiveState(TAknEditingState aState);
    
#ifndef RD_INTELLIGENT_TEXT_INPUT
     /** Since 5.0
 	  * 	 Returns True if AutoCompletion Mode is On so that - the appropriate indicator is set
 	  */
    
    TBool QueryPredictiveAutoCompleteState(TAknEditingState aState);
    
#endif     // RD_PRED_AW_COMPLETION
     /** Since 3.2
      * 	 Returns the indicator to be displayed based on the Editor state 
      */
    TInt EvaluateState(TAknEditingState aState);
private:
    CAknIndicatorContainer*     iIndicatorContainer;
    CAknNavigationDecorator*    iNaviDecorator;
    TAknEditingState            iPreviousState;
    CEikonEnv& iEikEnv;
    CAknNavigationControlContainer* iNaviPane;
    TBool iNaviPaneChanged;
    };

#endif // AknFepIND_H

// End of file
