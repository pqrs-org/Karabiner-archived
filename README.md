[![Build Status](https://travis-ci.org/tekezo/Karabiner.svg?branch=master)](https://travis-ci.org/tekezo/Karabiner)
[![License](https://img.shields.io/badge/license-Public%20Domain-blue.svg)](https://github.com/tekezo/Karabiner/blob/master/LICENSE.md)

OBSOLETED
=========

Karabiner is obsoleted on macOS Sierra (10.12) or later.<br />
Please use [Karabiner-Elements](https://github.com/tekezo/Karabiner-Elements).

Karabiner
=========

Karabiner is a powerful utility for keyboard customization.

* Change the key mappings (For example, "Change Right Option to Enter")
* Change the key repeat speed.
* The revival of a lost NumPad key (Fn+jkluio789…)
* Features for more efficient operations. (Emacs Mode, Vi Mode, Mouse Keys Mode, ...)

Prior to version 9.3.0, Karabiner was called *KeyRemap4MacBook*.

**Please use [Karabiner-Elements](https://github.com/tekezo/Karabiner-Elements) on macOS Sierra (10.12) or later.**

We made Karabiner-Elements from scratch due to kernel architecture changes in macOS Sierra.

Useful links
------------

* Latest build: https://pqrs.org/osx/karabiner/
* Mirror: http://tekezo.github.io/pqrs.org/
* Google Group: https://groups.google.com/forum/#!forum/osx-karabiner
* List of key names: https://github.com/tekezo/Karabiner/blob/master/src/bridge/generator/keycode/data/KeyCode.data

You can also get the latest stable release package via fixed URL.

```
$ curl -L -O https://pqrs.org/latest/karabiner-latest.dmg
```

System requirements
-------------------

Karabiner works for all Mac products, including the MacBook series, iMac, Mac mini, and Mac Pro, with the requirement that the product runs OS X 10.9 (Mavericks) or higher, up until MacOS 10.12 (Sierra), which is not supported.

* If you require Karabiner for macOS 10.12, please use [Karabiner Elements](https://github.com/tekezo/Karabiner-Elements).
* If you require Karabiner for OS X 10.6 - 10.8, please use KeyRemap4MacBook 8.4.0.
* If you require Karabiner for OS X 10.4 - 10.5, please use KeyRemap4MacBook 5.1.0.


How to build
------------

System requirements:

* OS X 10.11+
* Xcode 8+
* Command Line Tools for Xcode
* Boost 1.56.0+ (header-only) http://www.boost.org/

Please install Boost into `/opt/local/include/boost`. (eg. `/opt/local/include/boost/version.hpp`)

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
