/*
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef _IOKIT_HID_IOHIDCONSUMER_H
#define _IOKIT_HID_IOHIDCONSUMER_H

#include <IOKit/IOLib.h>
#include <IOKit/IOService.h>

// HID system includes.
#include <IOKit/hidsystem/IOHIDDescriptorParser.h>
#include <IOKit/hidsystem/IOHIDShared.h>
#include "IOHIKeyboard.h"
#include "IOHIDKeyboard.h"
#include "IOHIDEventService.h"

// extra includes.
#include <libkern/OSByteOrder.h>

//====================================================================================================
//	IOHIDConsumer
//	Generic driver for usb devices that contain special keys.
//====================================================================================================

class IOHIDConsumer : public IOHIKeyboard
{
    OSDeclareDefaultStructors(IOHIDConsumer)
    
    IOHIDEventService *     _provider;
    IOHIDKeyboard *         _keyboardNub;
    
    UInt32                  _otherEventFlags;
    UInt32                  _cachedEventFlags;
    bool                    _otherCapsLockOn;
	
	bool					_repeat;
    
    bool                    _isDispatcher;
    
    // Our implementation specific stuff.
    UInt32                  findKeyboardsAndGetModifiers();
    
public:
    // Allocator
    static IOHIDConsumer * 		Consumer(bool isDispatcher = false);
    
    // IOService methods
    virtual bool			init(OSDictionary *properties=0);
    virtual bool			start(IOService * provider);    
    
    virtual void            dispatchConsumerEvent(
                                IOHIDKeyboard *             sendingkeyboardNub,
                                AbsoluteTime                timeStamp,
                                UInt32                      usagePage,
                                UInt32                      usage,
                                UInt32						value,
                                IOOptionBits                options = 0);

    inline bool             isDispatcher() { return _isDispatcher;};
   
    // IOHIKeyboard methods
    virtual const unsigned char*	defaultKeymapOfLength( UInt32 * length );
    virtual bool                    doesKeyLock(unsigned key);
    virtual unsigned                eventFlags();
    virtual unsigned                deviceFlags();
    virtual void                    setDeviceFlags(unsigned flags);
    virtual void                    setNumLock(bool val);
    virtual bool                    numLock();
    virtual bool                    alphaLock();
};
#endif /* !_IOKIT_HID_IOHIDCONSUMER_H */
