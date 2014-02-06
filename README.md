KSleep
======

A sleep function just like your TV!

A few years ago I used to watch TV in my computer with `tvtime`. My tuner card came with a remote control that worked great with `LIRC`. The only thing that was missing was the "sleep" function so I wrote this very simple KDE/QT4 program to shutdown your computer from the RC at predefied intervals (just like TVs). 

It's been a while since I wrote this program and looking at it today there lots of things I would change (maybe even use PyQt instead of C++ or plain shell scripting). 

Anyway I hope this helps you avoid those confussing times in the middle of the night when you are woken up by the loud sounds of the TV.

Installation
------------

Run the following command as non-root user:
```shell
make
make install
```

Note that the above command will copy the following files to your home
directory:
```
    <~/>
     |
     +-<.ksleep/>
     | |
     | +-<ksleep-halt.sh>      (default halt command)
     | |
     | +-<ksleep-next.sh>      (default next command)
     | |
     | +-<ksleep.conf>         (config file)
     | |
     | \-<ksleep.sh>           (main client program)
     |
     \-<.kde/>
       |
       +-<bin/>
       | |
       | \-<ksleepd>           (ksleep deamon)
       |
       \-<Autostart>
         |
         \-<ksleep.desktop>    (autostart descriptor)
```

Configuration
-------------
After installing (see above), you may tweak KSleep editing the following
files:

- <tt>~/.ksleep/ksleep-halt.sh</tt>: by default, this file will be executed when
  KSleep finishes it's countdown. You can edit this file to change the
  halt behavior, or you can edit ksleep.conf (see below) to specify
  another program to be executed.

- <tt>~/.ksleep/ksleep-next.sh</tt>: by default, this file will make KSleep jump to
  the next timeout interval. You can edit this file to change the
  intervals. Note that this file should be called to loop through the
  intervals (e.g. from irexec).

- <tt>~/.ksleep/ksleep.conf</tt>: when KSleep stats, it reads this config file. You
  can edit this file to change the halt command or the visual settings.

Lirc Configuration
------------------
To use KSleep with Lirc, add the following lines to your <tt>~/.lircrc</tt> file:

    begin
        prog   = irexec

        button = EPG   # change this to the button you like!

        config = ~/.ksleep/ksleep-next.sh
    end
