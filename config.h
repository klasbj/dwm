/* See LICENSE file for copyright and license details. */

/* appearance */
static const char font[]            = "-*-monospace-medium-r-*-*-9-*-*-*-*-*-*-*";
static const char normbordercolor[] = "#262626";
static const char normbgcolor[]     = "#1c1c1c";
static const char normfgcolor[]     = "#9e9e9e";
static const char emptyfgcolor[]    = "#2e3436";
static const char urgfgcolor[]      = "#f1281e";
static const char urgbordercolor[]  = "#ff7e3d";
static const char selbordercolor[]  = "#00009c";
static const char selbgcolor[]      = "#1c1c1c";
static const char selfgcolor[]      = "#3d3dff";
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */
#define DWMBAR  0
/*#define TWOMONS*/
int struts[StrutLast] = { 15, 0, 15, 0 };

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Opera",    NULL,       NULL,       1 << 0,            False,       -1 },
	{ "Steam",    NULL,       NULL,       1 << 8,            False,       -1 },
	{ NULL,       NULL,       "irc",      1 << 2,            False,       -1 },
};

/* layout(s) */
static const float mfact      = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = False; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#ifdef TWOMONS
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      viewall,        {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#else
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#endif

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *dmenucmd[] = { "dmenu_run", "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "xterm", NULL };
static const char *audiomute[] = { "amixer", "-q", "sset", "Master", "toggle", NULL };
static const char *audiolower[] = { "amixer", "-q", "sset", "Master", "3%-", NULL };
static const char *audioraise[] = { "amixer", "-q", "sset", "Master", "3%+", NULL };
static const char *backlightup[] = { "backlight", "inc", NULL };
static const char *backlightdown[] = { "backlight", "dec", NULL };
static const char *kbdlightup[] = { "asus-kbd-backlight", "up", NULL };
static const char *kbdlightdown[] = { "asus-kbd-backlight", "down", NULL };
static const char *keymapse[] = { "swkbd", "se", NULL };
static const char *keymapen[] = { "swkbd", "us", NULL };

/* custom command functions */
extern void movestack(const Arg *arg);
extern void self_restart(const Arg *arg);

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_s,      spawn,          {.v = keymapse } },
	{ MODKEY,                       XK_e,      spawn,          {.v = keymapen } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
#ifdef TWOMONS
	{ MODKEY|ShiftMask,             XK_h,      setsbs_mfact,   {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_l,      setsbs_mfact,   {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_m,      togglesbs_both, {0} },
#endif
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_r,      self_restart,   {0} },
  /* audio keys */
  { 0,                            0x1008ff12, spawn,         {.v = audiomute } },
  { 0,                            0x1008ff11, spawn,         {.v = audiolower } },
  { 0,                            0x1008ff13, spawn,         {.v = audioraise } },
  { 0,                            0x1008ff17, spawn,         {.v = audiolower } },
  { 0,                            0x1008ff16, spawn,         {.v = audioraise } },
  /* backlight keys */
  { 0,                            0x1008ff02, spawn,         {.v = backlightup } },
  { 0,                            0x1008ff03, spawn,         {.v = backlightdown } },
  /* kbd backlight keys */
  { 0,                            0x1008ff05, spawn,         {.v = kbdlightup } },
  { 0,                            0x1008ff06, spawn,         {.v = kbdlightdown } },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

