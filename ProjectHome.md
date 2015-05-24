# Gmail Notifier, a KDE 4 plasmoid #

<br />

---

**TOC:** [Features](http://code.google.com/p/gmailnotifier/#Features) · [Download](http://code.google.com/p/gmailnotifier/#Download) · [Screenshots](http://code.google.com/p/gmailnotifier/#Screenshots) · [Feedback](http://code.google.com/p/gmailnotifier/#Feedback) ·
[Trademark notice](http://code.google.com/p/gmailnotifier/#Trademark_notice)

---


<br />
## Features ##
Gmail Notifier:
  * Supports multiple Gmail accounts and labels
  * Shows notifications whether new e-mails arrive.
  * Supports Gmail hosted domains


<br /><br />
## Download ##
### Build and installation ###
<font color='red'><u>WARNING:</u> Please make sure you installed all the required development packages and/or libraries suitable for your distribution! If it don't build, chances are the problem is on your side or, although I always try to commit code that compiles, you're very unlucky and you've just fetched the source code while it's broken. If so, try a previous revision until the problem is fixed in HEAD. So please, don't file an issue unless you're really sure it is a problem with Gmail Notifier!**</font>**

The latest code (in _trunk/_) **will only work with KDE >= 4.2.0**. If you wish to give it a try, just follow these steps:
```
$ svn co http://gmailnotifier.googlecode.com/svn/trunk/ gmailnotifier
$ cd gmailnotifier
$ mkdir Build; cd Build
$ cmake .. -DCMAKE_INSTALL_PREFIX=`kde4-config --prefix` -DCMAKE_BUILD_TYPE=Debugfull
$ make
$ sudo make install
$ kbuildsycoca4 # Shouldn't be necessary but won't do any harm
```

If you use **KDE < 4.2.0** you'll have to **stick to rev. 59** (which is quite buggy but should work on most setups), do:
```
$ svn co -r59 http://gmailnotifier.googlecode.com/svn/trunk/ gmailnotifier
$ cd gmailnotifier
$ mkdir Build; cd Build
$ cmake .. -DCMAKE_INSTALL_PREFIX=`kde4-config --prefix` -DCMAKE_BUILD_TYPE=Debugfull
$ make
$ sudo make install
$ kbuildsycoca4 # Shouldn't be necessary but won't do any harm
```

<u><b>Note:</b></u> The above commands will fetch, build **AND** install Gmail Notifier on your system.

### Update ###
To update the code to the latest revision do:
```
$ cd /path/to/gmailnotifier
$ svn update
$ cd Build
$ cmake .. -DCMAKE_INSTALL_PREFIX=`kde4-config --prefix` -DCMAKE_BUILD_TYPE=Debugfull
$ make
$ sudo make install
```

<u><b>Note:</b></u> The above commands will build Gmail Notifier in [full debug](http://techbase.kde.org/Development/CMake/Addons_for_KDE#Debugging_Features_in_Detail) mode. Replace "-DCMAKE\_BUILD\_TYPE=Debugfull" with "-DCMAKE\_BUILD\_TYPE=Release" if you don't need this (but note that if you find a bug or something it is usually helpful to have some debug output to submit with your bug report).


### Uninstallation ###
```
$ cd Build/
$ sudo make uninstall
$ kbuildsycoca4 # Shouldn't be necessary but won't do any harm
```


<u><b>Note #1:</b></u> There are no tarballs available! Code is only available through the SVN repo.<br />
<u><b>Note #2:</b></u> Please keep in mind that this is **development code**. Don't be surprised if you hit some nasty bugs or your computer orders some pizzas without your knowledge :). In other words: **use it at your own risk!**

<br /><br />
## Screenshots ##
<u>Note:</u> The following screenshots may not reflect the current state!

### Gmail Notifier placed on the desktop + configuration dialog ###
![![](http://gmailnotifier.googlecode.com/svn/wiki/images/screenshots/desktop_sm.png)](http://gmailnotifier.googlecode.com/svn/wiki/images/screenshots/desktop.png)
<br /><br />
### Gmail Notifier placed in a panel ###
![![](http://gmailnotifier.googlecode.com/svn/wiki/images/screenshots/panel_sm.png)](http://gmailnotifier.googlecode.com/svn/wiki/images/screenshots/panel.png)


<br /><br />
## Feedback ##
Have an idea? Feature request? Improvement Request? Patch? [File a feature request](http://code.google.com/p/gmailnotifier/issues/entry?template=Feature%20request)!

A problem? Bug? Any kind of weird behavior? [File a defect report](http://code.google.com/p/gmailnotifier/issues/entry?template=Defect%20report%20from%20user)!

You should [check the already opened issues](http://code.google.com/p/gmailnotifier/issues/list) before. Perhaps your problem was already reported by someone else.

<u>Note #1:</u> Please, please, please! Before filing a defect report (especially reports saying you can't build Gmail Notifier), make sure you installed all the required development packages/libraries suitable for your system/distro**. Since I'm only using one single distro, I'll be glad to hear what are the necessary packages for the distro you're using (drop me an e-mail), I'll then add this to that page.**<br />
<u><b>Note #2:</b></u> If you made a patch that you would like to see included in Gmail Notifier's codebase, please don't send it to me by e-mail. Instead, use the issue tracker so that other people can comment, star, etc... your work. Thanks!


<br /><br />
## Trademark notice ##
This project is neither endorsed, nor sponsored by, nor affiliated with Gmail®. Gmail® is a registered trademark of Google Inc.<br />
Google™ is a trademark of Google Inc.
