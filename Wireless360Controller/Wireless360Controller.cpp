/*
    MICE Xbox 360 Controller driver for Mac OS X
    Copyright (C) 2006-2013 Colin Munro
    
    Wireless360Controller.cpp - main source of the standard wireless controller driver
    
    This file is part of Xbox360Controller.

    Xbox360Controller is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Xbox360Controller is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <IOKit/IOLib.h>
#include "Wireless360Controller.h"
#include "../WirelessGamingReceiver/WirelessDevice.h"
#include "../360Controller/ControlStruct.h"
#include "../360Controller/xbox360hid.h"

#define kDriverSettingKey "DeviceData"

OSDefineMetaClassAndStructors(Wireless360Controller, WirelessHIDDevice)
#define super WirelessHIDDevice

bool Wireless360Controller::init(OSDictionary *propTable)
{
    bool res = super::init(propTable);    
    settings = new ControllerSettings();
    return res;
}

void Wireless360Controller::free()
{
    delete settings;
    super::free();
}

void Wireless360Controller::receivedHIDupdate(unsigned char *data, int length)
{
    settings->fiddleReport((XBOX360_IN_REPORT*)data);
    super::receivedHIDupdate(data, length);
}

void Wireless360Controller::SetRumbleMotors(unsigned char large, unsigned char small)
{
    unsigned char buf[] = {0x00, 0x01, 0x0f, 0xc0, 0x00, large, small, 0x00, 0x00, 0x00, 0x00, 0x00};
    WirelessDevice *device = OSDynamicCast(WirelessDevice, getProvider());
    
    if (device != NULL)
        device->SendPacket(buf, sizeof(buf));
}

IOReturn Wireless360Controller::setReport(IOMemoryDescriptor *report, IOHIDReportType reportType, IOOptionBits options)
{
    char data[2];
    
    // IOLog("setReport(%p, %d, %d)\n", report, reportType, options);
    if (report->readBytes(0, data, 2) < 2)
        return kIOReturnUnsupported;
        
    // Rumble
    if (data[0] == 0x00)
    {
        if ((data[1] != report->getLength()) || (data[1] != 0x04))
            return kIOReturnUnsupported;
        report->readBytes(2, data, 2);
        SetRumbleMotors(data[0], data[1]);
        return kIOReturnSuccess;
    }
    
    return super::setReport(report, reportType, options);
}

IOReturn Wireless360Controller::newReportDescriptor(IOMemoryDescriptor ** descriptor ) const
{
    IOBufferMemoryDescriptor *buffer = IOBufferMemoryDescriptor::inTaskWithOptions(kernel_task, 0, sizeof(ReportDescriptor));
    
    if (buffer == NULL)
        return kIOReturnNoResources;
    buffer->writeBytes(0, ReportDescriptor, sizeof(ReportDescriptor));
    *descriptor = buffer;
    return kIOReturnSuccess;
}

// Called by the userspace IORegistryEntrySetCFProperties function
IOReturn Wireless360Controller::setProperties(OSObject *properties)
{
    OSDictionary *dictionary = OSDynamicCast(OSDictionary, properties);
    
    if (dictionary != NULL) {
        setProperty(kDriverSettingKey, dictionary);
        settings->readSettings(dictionary);
        return kIOReturnSuccess;
    } else return kIOReturnBadArgument;
}

// Get info

OSString* Wireless360Controller::newManufacturerString() const
{
    return OSString::withCString("Microsoft");
}

OSNumber* Wireless360Controller::newPrimaryUsageNumber() const
{
    // Gamepad
    return OSNumber::withNumber(0x05, 8);
}

OSNumber* Wireless360Controller::newPrimaryUsagePageNumber() const
{
    // Generic Desktop
    return OSNumber::withNumber(0x01, 8);
}

OSNumber* Wireless360Controller::newProductIDNumber() const
{
    return OSNumber::withNumber((unsigned long long)0x28e, 16);
}

OSString* Wireless360Controller::newProductString() const
{
    return OSString::withCString("Wireless 360 Controller");
}

OSString* Wireless360Controller::newTransportString() const
{
    return OSString::withCString("Wireless");
}

OSNumber* Wireless360Controller::newVendorIDNumber() const
{
    return OSNumber::withNumber((unsigned long long)0x45e, 16);
}
