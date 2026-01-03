#!/bin/sh

prefix="/usr"
exec_prefix="${prefix}"
xrdb="xrdb"
xinitdir="/etc/X11/xinit"
xclock="xclock"
xterm="xterm"
twm="twm"
xmodmap="xmodmap"

userresources="$HOME/.Xresources"
usermodmap="$HOME/.Xmodmap"
sysresources="$xinitdir/.Xresources"
sysmodmap="$xinitdir/.Xmodmap"

# merge in defaults and keymaps

if [ -f "$sysresources" ]; then
    if [ -x /usr/bin/cpp ] ; then
        "$xrdb" -merge "$sysresources"
    else
        "$xrdb" -nocpp -merge "$sysresources"
    fi
fi

if [ -f "$sysmodmap" ]; then
    "$xmodmap" "$sysmodmap"
fi

if [ -f "$userresources" ]; then
    if [ -x /usr/bin/cpp ] ; then
        "$xrdb" -merge "$userresources"
    else
        "$xrdb" -nocpp -merge "$userresources"
    fi
fi

if [ -f "$usermodmap" ]; then
    "$xmodmap" "$usermodmap"
fi

[[ -f ~/.Xmodmap ]] && xmodmap ~/.Xmodmap

# start some nice programs

if [ -d "$xinitdir"/xinitrc.d ] ; then
	for f in "$xinitdir/xinitrc.d"/?*.sh ; do
		[ -x "$f" ] && . "$f"
	done
	unset f
fi

#"$twm" &
#"$xclock" -geometry 50x50-1+1 &
#"$xterm" -geometry 80x50+494+51 &
#"$xterm" -geometry 80x20+494-0 &
#exec "$xterm" -geometry 80x66+0+0 -name login


####### Lang ######
export LC_ALL=en_US.UTF-8
export MM_CHARSET=UTF-8
export LANG=en_US.UTF-8
export LANGUAGE=en_US.UTF-8
#export GTK2_RC_FILES="$HOME/.gtkrc-2.0"
#export BROWSER=firefox
#setxkbmap us
#export QT_QPA_PLATFORMTHEME="qt5ct"
MOZ_X11_EGL=1
#xset s off
#xset -dpms
#xset s noblank
#xautolock -time 10 -locker slock
export GVFS_DISABLE_FUSE=1
export XDG_SESSION_TYPE=x11
export env XDG_SESSION_TYPE=x11

# Hide Mouse
#unclutter -idle 5 &
#
xset +fp /home/DrRoot/.fonts
xset fp rehash

## Nvidia ##
#xrandr --setprovideroutputsource modesetting NVIDIA-0
#xrandr --auto
#autorandr --load highres2
#autorandr --load 1440x900

### Pywal ###
# Add this to your .xinitrc or whatever file starts programs on startup.
# -R restores the last colorscheme that was in use.
wal -R

dbus-update-activation-environment DBUS_SESSION_BUS_ADDRESS DISPLAY XAUTHORITY &

autowall &

# Fix Zoom BlackBox For Xorg
xcompmgr -c -l0 -t0 -r0 -o.00

#pywalfox start

while :; do
	ssh-agent dwm
done
