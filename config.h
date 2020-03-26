/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 0;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Hack Nerd Font:size=13" };
static const char dmenufont[]       = "Hack Nerd Font:size=13";
static const char col_gray1[]       = "#282828"; /* background color unfocused windows and tags*/
static const char col_gray2[]       = "#282828"; /* border color unfocused windows and tags*/
static const char col_gray3[]       = "#ebdbb2"; /* foreground color unfocused windows and tags*/
static const char col_gray4[]       = "#1d2021"; /* foreground color focused windows and tags*/
static const char col_cyan[]        = "#d79921"; /* border color focused windows and tags */
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     iscentered     isfloating   monitor */
	{ "st",       NULL,       "weechat",  1,            0,             0,           -1 },
	{ "st",       NULL,       "mutt",     1 << 1,       0,             0,           -1 },
	{ "st",       NULL,       "ranger",   1 << 2,       0,             0,           -1 },
	{ "Gimp",     NULL,       NULL,       0,            0,             1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,             0,           -1 },
	{ "Chromium", NULL,       NULL,       1 << 7,       0,             0,           -1 },
	{ "st",       "alsamixer",NULL,       0,            1,             1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define MEH ControlMask|ShiftMask|Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           { .ui = 1 << TAG } }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     { .ui = 1 << TAG } }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            { .ui = 1 << TAG } }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      { .ui = 1 << TAG } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]         = { "dmenu_run", "-i", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]          = { "st", NULL };
static const char *firefoxcmd[]       = { "firefox", NULL };
static const char *chromiumcmd[]      = { "chromium", NULL };
static const char *muttcmd[]          = { "st", "-n", "mutt", "-e", "mutt", NULL };
static const char *weechatcmd[]       = { "st", "-n", "weechat", "-e", "weechat", NULL };
static const char *rangercmd[]        = { "st", "-n", "ranger", "-e", "ranger", NULL };
static const char *mixercmd[]         = { "st", "-n", "alsamixer", "-e", "alsamixer", NULL };
static const char *upvol[]            = { "/usr/bin/pactl", "set-sink-volume", "0", "+5%", NULL };
static const char *downvol[]          = { "/usr/bin/pactl", "set-sink-volume", "0", "-5%", NULL };
static const char *mutevol[]          = { "/usr/bin/pactl", "set-sink-mute", "0", "toggle", NULL };
static const char *mutemic[]          = { "/usr/bin/pactl", "set-source-mute", "0", "toggle", NULL };
static const char *upbright[]         = { "/usr/bin/xbacklight", "-inc", "10", NULL };
static const char *downbright[]       = { "/usr/bin/xbacklight", "-dec", "10", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
    { MEH,                          XK_f,      spawn,          { .v = firefoxcmd } },
    { MEH,                          XK_c,      spawn,          { .v = chromiumcmd } },
    { MEH,                          XK_w,      spawn,          { .v = weechatcmd } },
    { MEH,                          XK_m,      spawn,          { .v = muttcmd } },
    { MEH,                          XK_r,      spawn,          { .v = rangercmd } },
    { MEH,                          XK_a,      spawn,          { .v = mixercmd } },

	{ 0,                       XF86XK_AudioLowerVolume,      spawn,          { .v = downvol } },
	{ 0,                       XF86XK_AudioRaiseVolume,      spawn,          { .v = upvol } },
	{ 0,                       XF86XK_AudioMute,             spawn,          { .v = mutevol } },
	{ 0,                       XF86XK_AudioMicMute,          spawn,          { .v = mutemic } },
	{ 0,                       XF86XK_MonBrightnessUp,       spawn,          { .v = upbright } },
	{ 0,                       XF86XK_MonBrightnessDown,     spawn,          { .v = downbright } },

	{ MODKEY,                       XK_p,      spawn,          { .v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          { .v = termcmd } },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           { 0 } },
	{ MODKEY,                       XK_b,      togglebar,      { 0 } },
	{ MODKEY,                       XK_j,      focusstack,     { .i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     { .i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     { .i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     { .i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       { .f = -0.05 } },
	{ MODKEY,                       XK_l,      setmfact,       { .f = +0.05 } },
	{ MODKEY,                       XK_Tab,    view,           { 0 } },
	{ MODKEY,                       XK_t,      setlayout,      { .v = &layouts[0] } },
	{ MODKEY,                       XK_f,      setlayout,      { .v = &layouts[1] } },
	{ MODKEY,                       XK_m,      setlayout,      { .v = &layouts[2] } },
	{ MODKEY,                       XK_space,  setlayout,      { 0 } },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, { 0 } },
	{ MODKEY,                       XK_0,      view,           { .ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            { .ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       { .i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       { .i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         { .i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         { .i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY,                       XK_q,      killclient,     { 0 } },
	{ MODKEY|ShiftMask,             XK_q,      quit,           { 0 } },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           { 1 } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      { 0 } },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      { .v = &layouts[2] } },
	{ ClkWinTitle,          0,              Button2,        zoom,           { 0 } },
	{ ClkStatusText,        0,              Button2,        spawn,          { .v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      { 0 } },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, { 0 } },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    { 0 } },
	{ ClkTagBar,            0,              Button1,        view,           { 0 } },
	{ ClkTagBar,            0,              Button3,        toggleview,     { 0 } },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            { 0 } },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      { 0 } },
};

