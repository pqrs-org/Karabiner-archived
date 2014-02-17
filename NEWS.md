Since 9.3.0
-----------

* v9.3.1: Updated prepared settings.

Version 9.3.0
-------------

* Fixed an issue:
    * "Kernel extension is not loaded" alert might be shown on some machines when automatic login is enabled.
* Updated prepared settings.
* Updates for people who add new settings by oneself:
    * These manipulators have been integrated into \_\_KeyToKey\_\_. <br />
      You can change KeyCode, ConsumerKeyCode and PointingButton by \_\_KeyToKey\_\_.
        * \_\_KeyToConsumer\_\_
        * \_\_ConsumerToKey\_\_
        * \_\_ConsumerToConsumer\_\_
        * \_\_KeyToPointingButton\_\_
        * \_\_PointingButtonToKey\_\_
        * \_\_PointingButtonToPointingButton\_\_
    * You can use KeyCode, ConsumerKeyCode and PointingButton in these manipulators:
        * \_\_KeyOverlaidModifier\_\_
        * \_\_HoldingKeyToKey\_\_
        * \_\_SimultaneousKeyPresses\_\_
        * \_\_DoublePressModifier\_\_
        * \_\_ScrollWheelToKey\_\_
        * \_\_PointingRelativeToScroll\_\_
    * Improved KeyCode::VK_CONFIG_* behavior in the edge case.
    * You can change the delay and speed of keyboard repeat per autogen. <br />
      For example, this autogen changes control-p to up arrow with:
        * delay until repeat: 100ms
        * key repeat: 10ms

        <pre>
        &lt;autogen&gt;
          __KeyToKey__
          KeyCode::P, MODIFIERFLAG_EITHER_LEFT_OR_RIGHT_CONTROL,
          KeyCode::CURSOR_UP,
          DelayUntilRepeat::RawValue::100,
          KeyRepeat::RawValue::10,
        &lt;/autogen&gt;
        </pre>

Version 9.2.0
-------------

* KeyRemap4MacBook no longer requires system restart at installing or upgrading. <br />
  If you need to restart system for some reason, KeyRemap4MacBook will show an alert which urges you to restart.
* Fixed issues:
    * System might crash when you disconnected a keyboard.

Version 9.0.0
-------------

* Optimized for OS X 10.9. <br />
  KeyRemap4MacBook 9.0.0 requires OS X 10.9+.
* Added delay configurations into multi-touch extension.
* Drop power button support due to limitations of OS X 10.9. <br />
  You can no longer change the power button on MacBook.
* Fixed issues:
    * ENV\_HOME does not work properly in included xml.
    * New plugged devices will not be recognized on OS X 10.9.
    * KeyRemap4MacBook disables the shut down dialog.
    * Fn key on Leopold FC660M cancels mouse dragging.
* Updated prepared settings:
    * "VIM Emulation" has been updated.
    * Fixed "MarkSet" in Emacs Mode.
* Updated installer.
* Some minor improvements.
* Signed with Developer ID.


Version 8.4.0
-------------

* Icons have been refined. (Thanks to Kouji TAMURA.)
* Introduced @begin and @end in private.xml. <br />
  You can use more than one key at \_\_KeyOverlaidModifier\_\_, \_\_DoublePressModifier\_\_.
* Supported shell commands execution with KeyCode::VK\_OPEN\_URL\_*.


Version 8.3.0
-------------

* Fixed a bug that "Look up" feature of OS X (tap trackpad with three fingers) does not work properly.
* Some minor improvements.


Version 8.2.0
-------------

* Fixed a bug that object selection is disabled on Adobe Fireworks.
* Fixed a bug that "General > Don't remap XXX" does not take effect immediately.
* Fixed a bug that control-eject shortcut does not work properly on OS X 10.7 and 10.6.


Version 8.1.0
-------------

* Power button remapping has been supported. (Thanks to Peter Kamb who is the author of PowerKey.)
* Added "Quit KeyRemap4MacBook" into menu bar.
* Added some styles into status message indicator.
* Improved compatibility with SmoothMouse.
* Supported more than three keys at \_\_SimultaneousKeyPresses\_\_.
* Added KeyCode::VK\_OPEN\_URL\_*.
* Added KeyCode::VK\_PARTIAL\_KEYDOWN and KeyCode::VK\_PARTIAL\_KEYUP.
* Added Option::POINTINGRELATIVETOSCROLL\_TOKEYS.
* Added \_\_FlipPointingRelative\_\_.
* Added \_\_FlipScrollWheel\_\_.
* Added filters:
    * &lt;lastpressedphysicalkey\_not&gt;
    * &lt;lastpressedphysicalkey\_only&gt;
    * &lt;elapsedtimesincelastpressed\_greaterthan&gt;
    * &lt;elapsedtimesincelastpressed\_lessthan&gt;
* Some minor improvements.


Version 8.0.0
-------------

* Changed Preferences location. <br />
  https://pqrs.org/macosx/keyremap4macbook/document.html
* Changed KeyRemap4MacBook\_cli location. <br />
  https://pqrs.org/macosx/keyremap4macbook/document-misc.html#commandlineinterface
* Integrated a status message indicator. (Removed Growl support.)
* Dynamic VK\_CHANGE\_INPUTSOURCE definition feature has been added.
* Dynamic &lt;inputsource\_only&gt; definition feature has been added.
* Changed --KeyToKey-- to \_\_KeyToKey\_\_ at &lt;autogen&gt;.
* Added a preference of ignored area into "multi-touch extension".
* Added Option::SIMULTANEOUSKEYPRESSES\_STRICT\_KEY\_ORDER.
* Added DeviceLocation into &lt;device\_only&gt;, &lt;device\_not&gt; filters.
* Some minor improvements.


Version 7.8.0
-------------

* Fixed a bug that private.xml does not work properly if &lt;identifier&gt; contains white space.
* Fixed a bug that "General &gt; Don't restore modifiers in the mouse event" does not work properly.
* Some minor improvements.


Version 7.7.0
-------------

* Fixed a bug introduced in version 7.6.0 that detecting of Input Sources does not work properly in Japanese and some other languages.


Version 7.6.0
-------------

* Added &lt;replacementdef&gt;. You can replace preset settings behavior by this.
* Added &lt;include&gt;. You can load xml files in private.xml.
* Added --ScrollWheelToKey--.
* Added --ScrollWheelToScrollWheel--.
* Added KeyCode::VK\_WAIT\_*.
* Improved KeyCode::VK\_CHANGE\_INPUTMODE\_* behavior.
* Improved XML processing engine. (XML reloading is 2x faster.)
* Some minor improvements.


Version 7.5.0
-------------

* Dynamic ApplicationType,DeviceVendor,DeviceProduct definition feature has been added.
* Support Growl-1.3 on notifications. <br />
  https://pqrs.org/macosx/keyremap4macbook/document.html#t4
* Added --StripModifierFromScrollWheel--.
* Added Option::KEYTOKEY\_BEFORE\_KEYDOWN, Option::KEYTOKEY\_AFTER\_KEYUP.
* Added KeyCode::VK\_CONSUMERKEY\_*.
* Some minor improvements.
* Added about 40 kinds settings to preset.


Version 7.4.0
-------------

* Some improvements on Mac OS X 10.7 (Lion).
* Removed "General &gt; Enable CapsLock LED Hack".<br />
  If you're using this setting for PCKeyboardHack, use "No Action" configuration instead. <br />
  https://pqrs.org/macosx/keyremap4macbook/extra.html#t1p1
* Clean up "Simultaneous Vi Mode". <br />
  (and split some options into "Home Row Arrow and Modifier Mode".)
* Added "[Key Overlaid Modifier] Initial Modifier Wait" preference into "Key Repeat" tab. <br />
  KeyOverlaidModifier changes key to modifier after this wait. <br />
  This preference is useful if your typing speed is too fast.
* Added KeyCode::VK\_STICKY\_*\_FORCE\_ON, KeyCode::VK\_STICKY\_*\_FORCE\_OFF.
* Added --DropScrollWheel--.
* Some minor improvements.
* Added some settings to preset.


Version 7.3.0
-------------

* Improved stability when using Sticky Keys in Universal Access.
* Improved stability when using multi-touch extension together.
* Improved "Lazy-Modifier (KeyCode::VK\_LAZY\_*)" behavior.
* Some minor improvements.
* Clean up the internal source code. The connection between kernel and userspace became more easier to understand.
* Added KeyCode::VK\_MOUSEKEY\_BUTTON\_*.
* Added Option::NOREPEAT at &lt;autogen&gt;.
* Added DeviceProduct::ANY at &lt;device\_only&gt;, &lt;device\_not&gt;.
* Added about 80 kinds settings.


version 7.2.0
-------------

* Fixed a bug that Growl notification did not work in specific environment.
* Fixed a bug that the default setting of checkForUpdate was "Nothing".
* Improved multi-touch extension around sleep/wakeup.
* Added about 20 kinds settings.

Version 7.1.0
-------------

* Added setting to be able to use "Logitech Number Pad" which we were not able not use in Mac OS X. <br />
  Activate "Change KeyPad Key &gt; Logitech Number Pad Hack".
* Replaced StatusWindow with Growl. <br />
  StatusWindow was displaying the lock state of modifiers and extra message. <br />
  Now, these messages are shown by Growl. <br />
  https://pqrs.org/macosx/keyremap4macbook/img/growl.png <br />
  http://growl.info/
* Attached an application named "multi-touch extension". <br />
  This application activates specific setting while fingers touch the multi-touch device. <br />
  ThumbSense is one of the function which this application supplies. <br />
* Improve UI of checkForUpdate.
* Fixed some bugs.
* Added about 140 kinds settings.
* Added &lt;modifier\_only&gt;, &lt;modifier\_not&gt; filters.
* Added --ForceNumLockOn-- to &lt;autogen&gt;.
