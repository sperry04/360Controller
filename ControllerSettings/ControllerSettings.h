//
//  ControllerSettings.h
//  360 Driver
//
//  Created by Scott Perry on 5/8/15.
//  Copyright (c) 2015 GitHub. All rights reserved.
//

#ifndef ___60_Driver__ControllerSettings__
#define ___60_Driver__ControllerSettings__

#include <IOKit/IOLib.h>
#include "../360Controller/ControlStruct.h"

// class to encapsulate the reading/fiddling of settings shared by all controllers
class ControllerSettings {
    
public:
    
    // constructor
    ControllerSettings(void);
    
    // Axis Settings
    bool invertLeftX;                   // invert the left X axis
    bool invertLeftY;                   // invert the left Y axis
    bool invertRightX;                  // invert the right X axis
    bool invertRightY;                  // invert the right Y axis
    
    // Deadzone Settings
    short deadzoneLeft;                 // size of the left stick deadzone
    short deadzoneRight;                // size of the right stick deadzone
    
    bool linkedLeft;                    // left deadzone is X-Y linked
    bool linkedRight;                   // right deadzone is X-Y linked

    bool normalizedLeft;                // left deadzone output is normalized
    bool normalizedRight;               // right deadzone output is normalized

    // Trigger Settings
    bool combinedTriggers;              // left and right triggers are combined into a single axis

    // Controller Button Mappings
    XBox360_Byte mapping[15];           // array of button mappings
    
    // Controller Specific Settings
    XBox360_Byte xoneRumbleType;        // the type of rumble for Xbone controllers
    
    // initialize settings to default values
    virtual void init(void);
    
    // read current controller settings from a dictionary
    virtual void readSettings(OSDictionary *dataDictionary);
    
    // modify an input report based on these settings
    virtual void fiddleReport(XBOX360_IN_REPORT *report);
    
};


#endif /* defined(___60_Driver__ControllerSettings__) */
