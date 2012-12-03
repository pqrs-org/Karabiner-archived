Document for Developers
=======================

The functions of KeyRemap4MacBook by Developer's perspective.
-------------------------------------------------------------

* Change the key event. (For example, "Change Right Option to Enter")
* Change the consumer key event. (The consumer key is the functional key such as "Volume Down")
* Change the pointing event. (For example, "MiddleClick to Expose")
* The key repeat function.
* Add private setting dynamically by private.xml.
* Filtering by application or input source or specific device, etc.
* Send Growl notification. (The status of Modifier Lock and extra messages.)
* Support to "Fast User Switching".
* PreferencePane in System Preferences.
* Check software updates automatically


How to build
------------
See README.md


Main components
---------------
* KeyRemap4MacBook.kext (Kernel Extension)
* KeyRemap4MacBook.app (Userspace Application)


Source code dependencies
------------------------
<pre>
+- src/bridge/generator/keycode
  +- src/bridge/include/bridge.h
    +- src/lib/xml_compiler
      +- src/bin/dump_xml_compiler_result
        +- src/bridge/generator/config
          +- src/core/kext
          +- src/core/server

("src/core/server" depends on "src/bridge/generator/config", ...)

</pre>


About event handling (kext)
---------------------------
To prevent leakage of the input event, the modification of the input event is perfomed inside kernel entirely.

We set a hook function in the middle of hardware driver.
And we modify the input event in the hook function.

...


The sequence of event handling
------------------------------
1. Device sends the input event.
1. Modify the input event by PCKeyboardHack.
1. Modify the input event by "System Preferences > Keyboard > Modifier Keys..."
1. Modify the input event by KeyRemap4MacBook.
1. Send the input event to Applications.


Threads and Lock (kext)
-----------------------
We use a global lock to guard the status of modifier flags, the status of buttons, and other global variables.
All threads use this global lock. The only one thread runs at the same time.

Threads:

* Timer callback (*_timer_callback)
* hooked function (EventInputQueue::push_*)
* gIOMatchedNotification and gIOTerminatedNotification callback of input devices (Core::*Notification_callback)
* IOUserClient callback (UserClient_kext::callback_*)
