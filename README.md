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


System requirements
-------------------

Mac OS X 10.6 or higher.

If you require KeyRemap4MacBook for Mac OS X 10.4.x or 10.5.x, use KeyRemap4MacBook 5.1.0.


How to build
------------

Requirements:

* OS X 10.8
* Xcode 4.4+ (Don't use Xcode 4.5. It makes invalid 32-bit binaries. http://openradar.appspot.com/12353507)
* Command Line Tools for Xcode
* Auxiliary Tools for Xcode
* Boost 1.52.0+ (header-only) http://www.boost.org/

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

Then, KeyRemap4MacBook-VERSION.dmg has been created in the current directory.
It's a distributable package.

<strong>
Build may be failed if you changed environment values or changed /usr/bin files.<br />
Use clean environment (new account) if build was failed.
</strong>
