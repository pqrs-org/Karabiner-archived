Karabiner
=========

Karabiner is a powerful utility for keyboard customization.

* Change the key mappings (For example, "Change Right Option to Enter")
* Change the key repeat speed.
* The revival of a lost NumPad key (Fn+jkluio789…)
* Features for more efficient operations. (Emacs Mode, Vi Mode, Mouse Keys Mode, ...)

Prior to version 9.3.0, Karabiner was called *KeyRemap4MacBook*.

Useful links
------------

* Latest build: https://pqrs.org/osx/karabiner/
* Mirror: http://tekezo.github.io/pqrs.org/
* Google Group: https://groups.google.com/forum/#!forum/osx-karabiner


System requirements
-------------------

Karabiner works for all Mac products, including the MacBook series, iMac, Mac mini, and Mac Pro, with the requirement that the product runs Mac OS X 10.9 (Mavericks) or higher.

* If you require Karabiner for OS X 10.6 - 10.8, please use KeyRemap4MacBook 8.4.0.
* If you require Karabiner for OS X 10.4 - 10.5, please use KeyRemap4MacBook 5.1.0.


How to build
------------

System Requirements:

* OS X 10.9+
* Xcode 5.0.1+
* Command Line Tools for Xcode
* Boost 1.54.0+ (header-only) http://www.boost.org/

Please install Boost into `/usr/local/include/boost`.

### Step 1: Getting source code

Download the source to master.tar.gz in the current directory, this can be re-executed to restart a cancelled download.

    curl -OLC - https://github.com/tekezo/Karabiner/archive/master.tar.gz

Extract the master.tar.gz file to "Karabiner-master" and delete the tar.gz file

    tar -xvzf master.tar.gz && rm master.tar.gz

### Step 2: Building a package

    cd Karabiner-master
    make

The `make` script will create a redistributable **Karabiner-VERSION.dmg** in the current directory.


**Note:**
The build may fail if you have changed any environment variables or if you have modified scripts in the `/usr/bin` locations. Use a clean environment (new account) if this is the case.


Customized Sparkle
------------------

We're using Sparkle to provide a software update feature.<br />
The Sparkle framework is located in "src/core/server/Frameworks/Sparkle.framework".

This built-in binary is built with some patches.

* Set MACOSX_DEPLOYMENT_TARGET 10.6: https://github.com/tekezo/Files/blob/master/patches/Sparkle/MACOSX_DEPLOYMENT_TARGET-10.6.diff
* Adding ".Sparkle" to appSupportPath: https://github.com/andymatuschak/Sparkle/pull/290
