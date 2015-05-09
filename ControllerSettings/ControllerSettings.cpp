//
//  ControllerSettings.cpp
//  360 Driver
//
//  Created by Scott Perry on 5/8/15.
//  Copyright (c) 2015 GitHub. All rights reserved.
//

#include <IOKit/IOLib.h>
#include <IOKit/hid/IOHIDDevice.h>
#include "ControllerSettings.h"

#define LOGGING_ENABLED 1

// constructor
ControllerSettings::ControllerSettings(void) {
    this->init();
}

// initialize settings to default values
void ControllerSettings::init(void) {
    
    // Axis Settings
    invertLeftX = false;           // invert the left X axis
    invertLeftY = false;           // invert the left Y axis
    invertRightX = false;          // invert the right X axis
    invertRightY = false;          // invert the right Y axis
    
    // Deadzone Settings
    deadzoneLeft = 0;              // size of the left stick deadzone
    deadzoneRight = 0;             // size of the right stick deadzone
    
    linkedLeft = false;            // left deadzone is X-Y linked
    linkedRight = false;           // right deadzone is X-Y linked
    
    normalizedLeft = false;        // left deadzone output is normalized
    normalizedRight = false;       // right deadzone output is normalized
    
    // Trigger Settings
    combinedTriggers = false;      // left and right triggers are combined into a single axis
    
    // Controller Button Mappings
    for (int i = 0; i < 11; i++)
    {
        mapping[i] = i;
    }
    for (int i = 12; i < 16; i++)
    {
        mapping[i-1] = i;
    }
    
    // Controller Specific Settings
    xoneRumbleType = 0;            // the type of rumble for Xbone controllers
}

// writes the current settings to log if loggin is enabled
inline void ControllerSettings::logSettings(const char *message) {
#if (LOGGING_ENABLED)
    IOLog("360Driver: %s - InvertLeft X[%s] Y[%s], InvertRight X[%s] Y[%s], CombinedTriggers[%s], Deadzone Left[%d%s%s] Right[%d%s%s]",
          message,
          invertLeftX ? "T" : "F", invertLeftY ? "T" : "F", invertRightX ? "T" : "F", invertRightY ? "T" : "F",
          combinedTriggers ? "T" : "F",
          deadzoneLeft, linkedLeft ? "-linked" : "", normalizedLeft ? "-normalized" : "",
          deadzoneRight, linkedRight ? "-linked" : "", normalizedRight ? "-normalized" : "");
#endif
}

// writes the contents of the report to the log if logging is enabled
static inline void logReport(const char *message, XBOX360_IN_REPORT *report) {
#if (LOGGING_ENABLED)
    IOLog("%s: Left X[%d] Y[%d] T[%d], Right X[%d] Y[%d] T[%d], Buttons [%d]",
          message,
          report->left.x, report->left.y, report->trigL,
          report->right.x, report->right.y, report->trigR,
          report->buttons);
#endif
}

// read current controller settings from the registry
void ControllerSettings::readSettings(OSDictionary *dataDictionary) {

    if (dataDictionary == NULL) {
        logSettings("Failed to load Controller Settings, using defaults");
        return;
    }

    OSBoolean *value = NULL;
    OSNumber *number = NULL;
    
    // inverted axis
    value = OSDynamicCast(OSBoolean, dataDictionary->getObject("InvertLeftX"));
    if (value != NULL) invertLeftX = value->getValue();
    value = OSDynamicCast(OSBoolean, dataDictionary->getObject("InvertLeftY"));
    if (value != NULL) invertLeftY = value->getValue();
    value = OSDynamicCast(OSBoolean, dataDictionary->getObject("InvertRightX"));
    if (value != NULL) invertRightX = value->getValue();
    value = OSDynamicCast(OSBoolean, dataDictionary->getObject("InvertRightY"));
    if (value != NULL) invertRightY = value->getValue();
    
    // deadzones
    number = OSDynamicCast(OSNumber, dataDictionary->getObject("DeadzoneLeft"));
    if (number != NULL) deadzoneLeft = number->unsigned32BitValue();
    number = OSDynamicCast(OSNumber, dataDictionary->getObject("DeadzoneRight"));
    if (number != NULL) deadzoneRight = number->unsigned32BitValue();
    value = OSDynamicCast(OSBoolean, dataDictionary->getObject("RelativeLeft"));
    if (value != NULL) linkedLeft = value->getValue();
    value = OSDynamicCast(OSBoolean, dataDictionary->getObject("RelativeRight"));
    if (value != NULL) linkedRight=value->getValue();
    value = OSDynamicCast(OSBoolean, dataDictionary->getObject("DeadOffLeft"));
    if (value != NULL) normalizedLeft = value->getValue();
    value = OSDynamicCast(OSBoolean, dataDictionary->getObject("DeadOffRight"));
    if (value != NULL) normalizedRight = value->getValue();
    
    // combined triggers
    value = OSDynamicCast(OSBoolean, dataDictionary->getObject("CombinedTriggers"));
    if (value != NULL) combinedTriggers = value->getValue();
    
    // controller type
    //number = OSDynamicCast(OSNumber, dataDictionary->getObject("ControllerType")); // No use currently.
    
    // controller specific settings
    number = OSDynamicCast(OSNumber, dataDictionary->getObject("XoneRumbleType"));
    if (number != NULL) xoneRumbleType = number->unsigned8BitValue();
    
    // control mappings
    number = OSDynamicCast(OSNumber, dataDictionary->getObject("BindingUp"));
    if (number != NULL) mapping[0] = number->unsigned32BitValue();
    number = OSDynamicCast(OSNumber, dataDictionary->getObject("BindingDown"));
    if (number != NULL) mapping[1] = number->unsigned32BitValue();
    number = OSDynamicCast(OSNumber, dataDictionary->getObject("BindingLeft"));
    if (number != NULL) mapping[2] = number->unsigned32BitValue();
    number = OSDynamicCast(OSNumber, dataDictionary->getObject("BindingRight"));
    if (number != NULL) mapping[3] = number->unsigned32BitValue();
    number = OSDynamicCast(OSNumber, dataDictionary->getObject("BindingStart"));
    if (number != NULL) mapping[4] = number->unsigned32BitValue();
    number = OSDynamicCast(OSNumber, dataDictionary->getObject("BindingBack"));
    if (number != NULL) mapping[5] = number->unsigned32BitValue();
    number = OSDynamicCast(OSNumber, dataDictionary->getObject("BindingLSC"));
    if (number != NULL) mapping[6] = number->unsigned32BitValue();
    number = OSDynamicCast(OSNumber, dataDictionary->getObject("BindingRSC"));
    if (number != NULL) mapping[7] = number->unsigned32BitValue();
    number = OSDynamicCast(OSNumber, dataDictionary->getObject("BindingLB"));
    if (number != NULL) mapping[8] = number->unsigned32BitValue();
    number = OSDynamicCast(OSNumber, dataDictionary->getObject("BindingRB"));
    if (number != NULL) mapping[9] = number->unsigned32BitValue();
    number = OSDynamicCast(OSNumber, dataDictionary->getObject("BindingGuide"));
    if (number != NULL) mapping[10] = number->unsigned32BitValue();
    number = OSDynamicCast(OSNumber, dataDictionary->getObject("BindingA"));
    if (number != NULL) mapping[11] = number->unsigned32BitValue();
    number = OSDynamicCast(OSNumber, dataDictionary->getObject("BindingB"));
    if (number != NULL) mapping[12] = number->unsigned32BitValue();
    number = OSDynamicCast(OSNumber, dataDictionary->getObject("BindingX"));
    if (number != NULL) mapping[13] = number->unsigned32BitValue();
    number = OSDynamicCast(OSNumber, dataDictionary->getObject("BindingY"));
    if (number != NULL) mapping[14] = number->unsigned32BitValue();
    
    logSettings("Controller Settings loaded");
}

// This returns the abs() value of a short, swapping it if necessary
static inline XBox360_SShort getAbsolute(XBox360_SShort value)
{
    XBox360_SShort reverse;
    
#ifdef __LITTLE_ENDIAN__
    reverse = value;
#elif __BIG_ENDIAN__
    reverse = ((value & 0xFF00) >> 8) | ((value & 0x00FF) << 8);
#else
#error Unknown CPU byte order
#endif
    
    return (reverse < 0 ? ~reverse : reverse);
}

// check a single axis against the deadzone settings and return the fiddled value
static inline UInt16 processDeadzone(UInt16 inputValue, short deadzone, bool normalized, bool linked, UInt16 linkedValue) {
    UInt16 rval = inputValue;
    
    // if there is a deadzone defined
    if (deadzone > 0) {
        
        // if the horz and vert deadzones are linked
        if (linked) {
            // both the input value and the linked value from the other axis must be outside the zone
            if ((getAbsolute(inputValue) < deadzone) && (getAbsolute(linkedValue) < deadzone)) {
                return 0; // both axis values were not outside the deadzone
            }
        } else { // the horz and vert deadzones are not linked
            // just the input value must be outside the deadzone
            if (getAbsolute(inputValue) < deadzone) {
                // the input value was not outside the deadzone
                return 0;
            }
        }
        
        // at this point we know the input value is not clipped by the deadzone,
        // now to normalize the output value if necessary
        if (normalized) {
            const UInt16 max16 = 32767;
            float maxVal = (float)(max16 - deadzone);
            
            // linearly scale the rval between the deadzone and max value
            rval = max16 * (getAbsolute(inputValue) - deadzone) / maxVal;
            
            // fix the sign lost by the getAbsolute() call
            if (inputValue < 0) {
                rval = -rval;
            }
        }
    }
    
    return rval;
}

// modify an input report based on these settings
void ControllerSettings::fiddleReport(XBOX360_IN_REPORT *report) {
    
    logReport("Pre-Fiddled Report", report);
    
    // invert axis
    if (invertLeftX)    report->left.x  = ~report->left.x;
    if (!invertLeftY)   report->left.y  = ~report->left.y;
    if (invertRightX)   report->right.x = ~report->right.x;
    if (!invertRightY)  report->right.y = ~report->right.y;
    
    // deadzone limits for left stick
    UInt16 x = report->left.x;
    UInt16 y = report->left.y;
    report->left.x = processDeadzone(x, deadzoneLeft, normalizedLeft, linkedLeft, y);
    report->left.y = processDeadzone(y, deadzoneLeft, normalizedLeft, linkedLeft, x);

    // deadzone limits for right stick
    x = report->right.x;
    y = report->right.y;
    report->right.x = processDeadzone(x, deadzoneRight, normalizedRight, linkedRight, y);
    report->right.y = processDeadzone(y, deadzoneRight, normalizedRight, linkedRight, x);

    // combined trigger axis
    if (combinedTriggers) {
        // set only the left trigger axis as a combined value, zero out the right trigger axis
        const UInt8 trigCenter = UINT8_MAX / 2U;
//        report->trigL = trigCenter + ((report->trigR / 2U + 1) - report->trigL / 2U);
        report->trigL = trigCenter + ((report->trigR / 2U) - (report->trigL / 2U));
        report->trigR = 0U;
    }
    
    logReport("Post-Fiddled Report", report);
}


