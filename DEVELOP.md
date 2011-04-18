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
See <http://pqrs.org/macosx/keyremap4macbook/source.html>


Main components
---------------
* KeyRemap4MacBook.kext (Kernel Extension)
* KeyRemap4MacBook_server.app (Userspace Application)


Sub components
--------------
* KeyRemap4MacBook.app (Updater)
* KeyRemap4MacBook.prefPane (PreferencePane)


About event handling
--------------------
To prevent leakage of the input event, the conversion of the input event is done in the kernel entirely.

We set a hook function in the middle of hardware driver.
And we modify the input event in the hook function.

...
