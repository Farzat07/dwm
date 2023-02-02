/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10", "JoyPixels:pixelsize=10" };
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
	[SchemeTabActive]  = { col_gray2, col_gray3,  col_gray2 },
	[SchemeTabInactive]  = { col_gray1, col_gray3,  col_gray1 }
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class          instance    title       tags mask     isfloating   monitor */
	{ "stalonetray",  NULL,       NULL,       ~0,           1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */

/* Bartabgroups properties */
#define BARTAB_BORDERS 1       // 0 = off, 1 = on
#define BARTAB_BOTTOMBORDER 1  // 0 = off, 1 = on
#define BARTAB_TAGSINDICATOR 1 // 0 = off, 1 = on if >1 client/view tag, 2 = always on
#define BARTAB_TAGSPX 5        // # pixels for tag grid boxes
#define BARTAB_TAGSROWS 3      // # rows in tag grid (9 tags, e.g. 3x3)
static void (*bartabmonfns[])(Monitor *) = { monocle /* , customlayoutfn */ };
static void (*bartabfloatfns[])(Monitor *) = { NULL /* , customlayoutfn */ };

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(CHAIN,KEY,TAG) \
	{ MODKEY,                       CHAIN, KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           CHAIN, KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             CHAIN, KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, CHAIN, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd1[]  = { "terminal_tmux", NULL };
static const char *termcmd2[]  = { "st", NULL };

#include <X11/XF86keysym.h>  // For XF86keysym keys.

static const Key keys[] = {
	/* modifier                     chain key  key        function        argument */
	{ MODKEY,                       -1,        XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       -1,        XK_Return, spawn,          {.v = termcmd1 } },
	{ MODKEY|ControlMask,           -1,        XK_Return, spawn,          {.v = termcmd2 } },
	{ MODKEY,                       -1,        XK_b,      togglebar,      {0} },
	{ MODKEY|ShiftMask,             -1,        XK_b,      spawn,          SHCMD("killall stalonetray || stalonetray") },
	{ MODKEY,                       -1,        XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       -1,        XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       -1,        XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       -1,        XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       -1,        XK_l,      spawn,          SHCMD("slock") },
	{ MODKEY|ShiftMask,             -1,        XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             -1,        XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             -1,        XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             -1,        XK_k,      movestack,      {.i = -1 } },
	{ MODKEY|ShiftMask,             -1,        XK_Return, zoom,           {0} },
	{ MODKEY,                       -1,        XK_Tab,    view,           {0} },
	{ MODKEY,                       -1,        XK_c,      killclient,     {0} },
	{ MODKEY,                       -1,        XK_v,      spawn,          SHCMD("clipmenu") },
	{ MODKEY|ShiftMask,             -1,        XK_v,      spawn,          SHCMD("xdotool type \"$(CM_OUTPUT_CLIP=1 clipmenu)\"") },
	{ MODKEY,                       -1,        XK_o,      spawn,          SHCMD("xclip -o -sel clip | xargs -d '\\n' linkhandler") },
	{ MODKEY,                       -1,        XK_t,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       -1,        XK_f,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       -1,        XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       -1,        XK_space,  spawn,          SHCMD("dmenu_type") },
	{ MODKEY|ShiftMask,             -1,        XK_space,  spawn,          SHCMD("xkb-switch -n") },
	{ MODKEY|ControlMask,           -1,        XK_space,  spawn,          SHCMD("dmenu_type set") },
	{ MODKEY|ShiftMask,             -1,        XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       -1,        XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             -1,        XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       -1,        XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       -1,        XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             -1,        XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             -1,        XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        -1,        XK_1,                      0)
	TAGKEYS(                        -1,        XK_2,                      1)
	TAGKEYS(                        -1,        XK_3,                      2)
	TAGKEYS(                        -1,        XK_4,                      3)
	TAGKEYS(                        -1,        XK_5,                      4)
	TAGKEYS(                        -1,        XK_6,                      5)
	TAGKEYS(                        -1,        XK_7,                      6)
	TAGKEYS(                        -1,        XK_8,                      7)
	TAGKEYS(                        -1,        XK_9,                      8)
	{ MODKEY,                       -1,        XK_Left,   togglehorizontalmax, {.i = -1 } },
	{ MODKEY,                       -1,        XK_Right,  togglehorizontalmax, {.i = +1 } },
	{ MODKEY,                       -1,        XK_Down,   toggleverticalmax,   {.i = -1 } },
	{ MODKEY,                       -1,        XK_Up,     toggleverticalmax,   {.i = +1 } },
	{ MODKEY|ShiftMask,             -1,        XK_Left,   togglehorizontalmax, {.i = -2 } },
	{ MODKEY|ShiftMask,             -1,        XK_Right,  togglehorizontalmax, {.i = +2 } },
	{ MODKEY|ShiftMask,             -1,        XK_Down,   toggleverticalmax,   {.i = -2 } },
	{ MODKEY|ShiftMask,             -1,        XK_Up,     toggleverticalmax,   {.i = +2 } },
	{ MODKEY|ShiftMask,             -1,        XK_m,      togglemaximize, {0} },
	{ MODKEY|ShiftMask,             -1,        XK_q,      quit,           {0} },
	{ 0,           -1, XF86XK_AudioMute,           spawn,     SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle; kill -44 $(pidof dwmblocks)") },
	{ 0,           -1, XF86XK_AudioLowerVolume,    spawn,     SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%; kill -44 $(pidof dwmblocks)") },
	{ 0,           -1, XF86XK_AudioRaiseVolume,    spawn,     SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%; kill -44 $(pidof dwmblocks)") },
	{ ShiftMask,   -1, XF86XK_AudioLowerVolume,    spawn,     SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -1%; kill -44 $(pidof dwmblocks)") },
	{ ShiftMask,   -1, XF86XK_AudioRaiseVolume,    spawn,     SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +1%; kill -44 $(pidof dwmblocks)") },
	{ 0,           -1, XF86XK_AudioMicMute,        spawn,     SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle; kill -45 $(pidof dwmblocks)") },
	{ 0,           -1, XF86XK_MonBrightnessDown,   spawn,     SHCMD("xbacklight -dec 5; kill -36 $(pidof dwmblocks)") },
	{ 0,           -1, XF86XK_MonBrightnessUp,     spawn,     SHCMD("xbacklight -inc 5; kill -36 $(pidof dwmblocks)") },
	{ ShiftMask,   -1, XF86XK_MonBrightnessDown,   spawn,     SHCMD("xbacklight -dec 1; kill -36 $(pidof dwmblocks)") },
	{ ShiftMask,   -1, XF86XK_MonBrightnessUp,     spawn,     SHCMD("xbacklight -inc 1; kill -36 $(pidof dwmblocks)") },
	{ 0,           -1, XF86XK_AudioPlay,           spawn,     SHCMD("playerctl play-pause") },
	{ 0,           -1, XF86XK_AudioPause,          spawn,     SHCMD("playerctl play-pause") },
	{ 0,           -1, XF86XK_AudioPrev,           spawn,     SHCMD("playerctl position 10-") },
	{ ShiftMask,   -1, XF86XK_AudioPrev,           spawn,     SHCMD("playerctl previous") },
	{ 0,           -1, XF86XK_AudioNext,           spawn,     SHCMD("playerctl position 10+") },
	{ ShiftMask,   -1, XF86XK_AudioNext,           spawn,     SHCMD("playerctl next") },
	{ 0,           -1, XF86XK_AudioStop,           spawn,     SHCMD("playerctl --all-players pause") },
	{ ShiftMask,   -1, XF86XK_AudioStop,           spawn,     SHCMD("playerctl stop") },
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
	{ ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigstatusbar,   {.i = 6} },
	{ ClkStatusText,        ShiftMask,      Button2,        sigstatusbar,   {.i = 7} },
	{ ClkStatusText,        ShiftMask,      Button3,        sigstatusbar,   {.i = 8} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

