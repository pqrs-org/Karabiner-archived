KeyRemap4MacBook
================

KeyRemap4MacBook is a powerful utility for keyboard customization.

* Change the key. (For example, "Change Right Option to Enter")
* Accelerate speed of the key repeat.
* The revival of a lost NumPad key (Fn+jkluio789…)
* Features for more efficient operations. (Emacs Mode, Vi Mode, Mouse Keys Mode, ...)


Web pages
---------

* master: https://pqrs.org/macosx/keyremap4macbook/
* backup: http://tekezo.github.io/pqrs.org/
* Google Group:  https://groups.google.com/forum/#!forum/keyremap4macbook


System requirements
-------------------

Mac OS X 10.9 (Maveriks) or higher.  KeyRemap4MacBook works for all Mac products, including the MacBook series, the iMac, Mac mini, and Mac Pro.

* If you require KeyRemap4MacBook for OS X 10.6 - 10.8, use KeyRemap4MacBook 8.4.0.
* If you require KeyRemap4MacBook for OS X 10.4 - 10.5, use KeyRemap4MacBook 5.1.0.


How to build
------------

Requirements:

* OS X 10.9+
* Xcode 5.0.1+
* Command Line Tools for Xcode
* Boost 1.54.0+ (header-only) http://www.boost.org/

Please install Boost into /usr/local/include/boost.

### Step1: Getting source code

Execute a following command in Terminal.app.

<pre>
git clone --depth 10 https://github.com/tekezo/KeyRemap4MacBook.git
</pre>

### Step2: Building a package

Execute a following command in Terminal.app.

<pre>
cd KeyRemap4MacBook
make
</pre>

Then, KeyRemap4MacBook-VERSION.dmg has been created in the current directory.
It's a distributable package.


**Note:**<br />
Build may be failed if you changed environment values or changed /usr/bin files.<br />
Use clean environment (new account) if build was failed.


Customized Sparkle
------------------

We're using Sparkle to provide a software update feature.<br />
The Sparkle framework is located in "src/core/server/Frameworks/Sparkle.framework".

This built-in binary is built with some patches.

* Set MACOSX_DEPLOYMENT_TARGET 10.6: https://github.com/tekezo/Files/blob/master/patches/Sparkle/MACOSX_DEPLOYMENT_TARGET-10.6.diff
* Adding ".Sparkle" to appSupportPath: https://github.com/andymatuschak/Sparkle/pull/290
