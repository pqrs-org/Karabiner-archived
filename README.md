KeyRemap4MacBook
================

KeyRemap4MacBook is a powerful utility for keyboard customization.

* Change the key. (For example, "Change Right Option to Enter")
* Accelerate speed of the key repeat.
* The revival of a lost NumPad key (Fn+jkluio789…)
* Features for more efficient operations. (Emacs Mode, Vi Mode, Mouse Keys Mode, ...)


System requirements
-------------------

Mac OS X 10.6 or higher.

If you require KeyRemap4MacBook for Mac OS X 10.4.x or 10.5.x, use KeyRemap4MacBook 5.1.0.


How to build
------------

Requirements:

* OS X 10.7
* Xcode 4.3+
* Command Line Tools for Xcode
* Auxiliary Tools for Xcode
* Boost 1.49.0+ (header-only) http://www.boost.org/

Please install PackageMaker.app into /Applications/Utilities.
(PackageMaker.app is included in Auxiliary Tools for Xcode.)

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

Then, KeyRemap4MacBook-VERSION.pkg.zip has been created in the current directory.
It's a distributable package.
