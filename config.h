/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int vertpad            = 10;       /* vertical padding of bar */
static const int sidepad            = 10;       /* horizontal padding of bar */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showtitle          = 1;        /* 0 means no title */
static const int showtags           = 1;        /* 0 means no tags */
static const int showlayout         = 1;        /* 0 means no layout indicator */
static const int showstatus         = 1;        /* 0 means no status bar */
static const int showfloating       = 1;        /* 0 means no floating indicator */
static const int topbar             = 1;        /* 0 means bottom bar */
#define ICONSIZE 16   /* icon size */
#define ICONSPACING 5 /* space between icon and title */
static const char *fonts[]          = { "GoMono Nerd Font:size=10", "Hack:size=9:antialias=true:autohint=true", "NotoColorEmoji:pixelsize=9:antialias=true:autohint=true" , "terminus:style=Bold:size=9" ,"monospace:size=9" };
static const char dmenufont[]       = "GoMono Nerd Font:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       // [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
	[SchemeSel]  = { selbgcolor,  selfgcolor,  selbordercolor  },
	/* for bar --> {text, background, null} */
	[SchemeStatus]  = { normfgcolor, normbgcolor,  normbgcolor  }, /* status R */
	[SchemeTagsSel]  = { normfgcolor, normbgcolor,  normbgcolor  }, /* tag L selected */
	[SchemeTagsNorm]  = { selbordercolor, normbgcolor,  normbgcolor  }, /* tag L unselected */
	[SchemeInfoSel]  = { normfgcolor, normbgcolor,  normbgcolor  }, /* info M selected */
	[SchemeInfoNorm]  = { normfgcolor, normbgcolor,  normbgcolor  }, /* info M unselected */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

/* launcher commands (They must be NULL terminated) */
static const char* surf[]      = { "surf", "duckduckgo.com", NULL };
static const char* komikku[]      = { "komikku",  NULL };
static const char* zen[]      = { "zen-browser", "duckduckgo.com", NULL };
static const char* telegram[]      = { "telegram-desktop",  NULL };
static const char* qutebrowser[]      = { "qutebrowser", "duckduckgo.com", NULL };
static const char* pavucontrol[]      = { "pavucontrol", NULL };
static const char* zoomet[]      = { "zoom", NULL };
static const char* ai[]      = { "/home/DrRoot/.local/bin/ai", NULL };

/* Helper macros for spawning commands */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define CMD(...)   { .v = (const char*[]){ __VA_ARGS__, NULL } }

static const Launcher launchers[] = {
       /* command       name to display */
	{ surf,         "surf" },
	{ komikku,         	"" },
	{ zen,         	"" },
	{ telegram, 		""},
	{ qutebrowser, 	"" },
	{ pavucontrol, 	"" },
	{ zoomet,	 	"" },
	{ ai,	 		"" },
	
	
};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \
	{ MOD, XK_q,     ACTION##stack, {.i = 0 } }, \
	{ MOD, XK_a,     ACTION##stack, {.i = 1 } }, \
	{ MOD, XK_z,     ACTION##stack, {.i = 2 } }, \
	{ MOD, XK_x,     ACTION##stack, {.i = -1 } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"
#define BROWSER "qutebrowser"

/* helper for launching gtk application */
#define GTKCMD(cmd) { .v = (const char*[]){ "/usr/bin/gtk-launch", cmd, NULL } }


/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *clipcmd[] 	 = { "clipmenu", "-m", dmenumon, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL }; 

/* Custom */
// static const char *kbd_cmd  = "/home/DrRoot/.local/bin/lang";
static const char *downbri[]  = { "/home/DrRoot/.local/bin/backlight", " --dec", NULL };
static const char *upbri[]  = { "/home/DrRoot/.local/bin/backlight", " --inc", NULL };

//static const char *micmute[]  = {"pactl set-source-mute @DEFAULT_SOURCE@ toggle", NULL};
static const char *micmute[]  = { "/home/DrRoot/.local/bin/volume", "--toggle-mic", NULL };
static const char *downvol[]  = { "/home/DrRoot/.local/bin/volume", "--dec", NULL };
static const char *upvol[]  = { "/home/DrRoot/.local/bin/volume", "--inc", NULL };
static const char *mute[]  = { "/home/DrRoot/.local/bin/volume", "--toggle", NULL };

// static const char *layoutmenu_cmd = "/home/DrRoot/.local/bin/layoutmenu.sh";
static const char *setabg[]  = { "/home/DrRoot/.local/bin/setbg", NULL };
static const char *autowall[]  = { "/home/DrRoot/.local/bin/autowall", NULL };

#include "selfrestart.c"
#include "bulkill.c"
 
static const Arg tagexec[] = {
	{ .v = termcmd },
	{ .v = termcmd },
	{ .v = termcmd },
	{ .v = termcmd },
	{ .v = termcmd },
	{ .v = termcmd },
	{ .v = termcmd },
	{ .v = termcmd },
	{ .v = termcmd }
};

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY, 	                  XK_v,	 spawn, 	   {.v = clipcmd} },
	/*XF86 Keys control keys */
	/* Light */
 	{ 0, XF86XK_MonBrightnessDown, spawn, {.v = downbri}},
	{ 0, XF86XK_MonBrightnessUp,   spawn, {.v = upbri}},
 	/* Audio */
 	{ 0, XF86XK_AudioMicMute,	   spawn, {.v = micmute}},
 	{ 0, XF86XK_AudioLowerVolume, spawn, {.v = downvol}},
 	{ 0, XF86XK_AudioRaiseVolume, spawn, {.v = upvol}},
 	{ 0, XF86XK_AudioMute, spawn, {.v = mute}},
 	/* Other */
  	// { 0, XF86XK_Explorer,          spawn, {.v = autowall}},
	// { 0, XF86XK_LaunchA,           spawn, {.v = }},
  	{ 0, XF86XK_Tools,             spawn, {.v = autowall}},
  	{ 0, XF86XK_Search,            spawn, {.v = setabg}},
  	// { 0, XF86XK_Display,           spawn, {.v = }},
  	/* Custom Key */
	// {Mod1Mask,              	ShiftMask,	 spawn, 	   {.v = kbd_cmd} },
	/* */
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY|Mod1Mask,              XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_i,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_o,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_6,      incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_7,      incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_8,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_9,      incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY|ControlMask,           XK_c,      bulkill,        {.ui = 1} },  // kill unselect
	{ MODKEY|ShiftMask|ControlMask, XK_c,      bulkill,        {.ui = 2} },  // killall
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	// { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,			    XK_f,	   togglefullscreen, {0} },
	{ MODKEY,			    XK_t,      togglebartitle, {0} },
	{ MODKEY,                       XK_s,      togglebarstatus,{0} },
	{ MODKEY|ShiftMask,             XK_t,      togglebartags,  {0} },
	{ MODKEY|ShiftMask,		    XK_s,      togglebarlt,    {0} },
	{ MODKEY|ShiftMask,		    XK_f,      togglebarfloat, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_F5,     xrdb,           {.v = NULL } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
    	{ MODKEY|ShiftMask,             XK_r,      self_restart,   {0} },
	// { MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_BackSpace, quit,        {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} }, 
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button2,        spawntag,       {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
