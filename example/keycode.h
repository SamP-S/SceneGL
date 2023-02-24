#ifndef _KEYCODE_
#define _KEYCODE_

/**
 * Keyboard Modifiers
 * - Specific modifiers for each key and combined bit representations for general modifiers.
 */

enum KeyMod : unsigned short {
    KM_NONE     = 0x0000,
    KM_LSHIFT   = 0x0001,
    KM_RSHIFT   = 0x0002,
    KM_LCTRL    = 0x0004,
    KM_RCTRL    = 0x0008,
    KM_LALT     = 0x0010,
    KM_RALT     = 0x0020,
    KM_LGUI     = 0x0040,
    KM_RGUI     = 0x0080,
    KM_NUM      = 0x0100,
    KM_CAPS     = 0x0200,
    KM_SHIFT    = KM_LSHIFT | KM_RSHIFT,
    KM_CTRL     = KM_LCTRL  | KM_RCTRL,
    KM_ALT      = KM_LALT   | KM_RALT,
    KM_GUI      = KM_LGUI   | KM_RGUI
};

/**
 * ScanCode Mask
 * For keycode values outside of ASCII values for keys without character representations.
 * 
 * ScanCode To KeyCode
 * Simple function to apply mask to X.
 */
#define SC_MASK (1 << 15)
#define SC_TO_KEYCODE(X)  (X | SC_MASK)

/**
 * Virtual KeyCodes
 */
enum KeyCode : unsigned short {
    /**
     * Unknown/Error key
     */
    K_UNKNOWN = 0,

    /**
     * ASCII Characters
     */
    K_RETURN = '\r',
    K_ESCAPE = '\033',
    K_BACKSPACE = '\b',
    K_TAB = '\t',
    K_SPACE = ' ',
    K_EXCLAIM = '!',
    K_QUOTEDBL = '"',
    K_HASH = '#',
    K_PERCENT = '%',
    K_DOLLAR = '$',
    K_AMPERSAND = '&',
    K_QUOTE = '\'',
    K_LEFTPAREN = '(',
    K_RIGHTPAREN = ')',
    K_ASTERISK = '*',
    K_PLUS = '+',
    K_COMMA = ',',
    K_MINUS = '-',
    K_PERIOD = '.',
    K_SLASH = '/',
    K_0 = '0',
    K_1 = '1',
    K_2 = '2',
    K_3 = '3',
    K_4 = '4',
    K_5 = '5',
    K_6 = '6',
    K_7 = '7',
    K_8 = '8',
    K_9 = '9',
    K_COLON = ':',
    K_SEMICOLON = ';',
    K_LESS = '<',
    K_EQUALS = '=',
    K_GREATER = '>',
    K_QUESTION = '?',
    K_AT = '@',

    /**
     * More ASCII Characters
     */
    K_LEFTBRACKET = '[',
    K_BACKSLASH = '\\',
    K_RIGHTBRACKET = ']',
    K_CARET = '^',
    K_UNDERSCORE = '_',
    K_BACKQUOTE = '`',
    K_a = 'a',
    K_b = 'b',
    K_c = 'c',
    K_d = 'd',
    K_e = 'e',
    K_f = 'f',
    K_g = 'g',
    K_h = 'h',
    K_i = 'i',
    K_j = 'j',
    K_k = 'k',
    K_l = 'l',
    K_m = 'm',
    K_n = 'n',
    K_o = 'o',
    K_p = 'p',
    K_q = 'q',
    K_r = 'r',
    K_s = 's',
    K_t = 't',
    K_u = 'u',
    K_v = 'v',
    K_w = 'w',
    K_x = 'x',
    K_y = 'y',
    K_z = 'z',

    K_CAPSLOCK = SC_TO_KEYCODE(SC_CAPSLOCK),

    /**
     * Function Keys
     */
    K_F1 = SC_TO_KEYCODE(SC_F1),
    K_F2 = SC_TO_KEYCODE(SC_F2),
    K_F3 = SC_TO_KEYCODE(SC_F3),
    K_F4 = SC_TO_KEYCODE(SC_F4),
    K_F5 = SC_TO_KEYCODE(SC_F5),
    K_F6 = SC_TO_KEYCODE(SC_F6),
    K_F7 = SC_TO_KEYCODE(SC_F7),
    K_F8 = SC_TO_KEYCODE(SC_F8),
    K_F9 = SC_TO_KEYCODE(SC_F9),
    K_F10 = SC_TO_KEYCODE(SC_F10),
    K_F11 = SC_TO_KEYCODE(SC_F11),
    K_F12 = SC_TO_KEYCODE(SC_F12),

    /**
     * Special Modifiers
     */
    K_PRINTSCREEN = SC_TO_KEYCODE(SC_PRINTSCREEN),
    K_SCROLLLOCK = SC_TO_KEYCODE(SC_SCROLLLOCK),
    K_PAUSE = SC_TO_KEYCODE(SC_PAUSE),
    K_INSERT = SC_TO_KEYCODE(SC_INSERT),
    K_HOME = SC_TO_KEYCODE(SC_HOME),
    K_PAGEUP = SC_TO_KEYCODE(SC_PAGEUP),
    K_DELETE = '\177',
    K_END = SC_TO_KEYCODE(SC_END),
    K_PAGEDOWN = SC_TO_KEYCODE(SC_PAGEDOWN),

    /**
     * Arrow Keys
     */
    K_RIGHT = SC_TO_KEYCODE(SC_RIGHT),
    K_LEFT = SC_TO_KEYCODE(SC_LEFT),
    K_DOWN = SC_TO_KEYCODE(SC_DOWN),
    K_UP = SC_TO_KEYCODE(SC_UP),

    /**
     * KeyPad
     */
    K_NUMLOCKCLEAR = SC_TO_KEYCODE(SC_NUMLOCKCLEAR),
    K_KP_DIVIDE = SC_TO_KEYCODE(SC_KP_DIVIDE),
    K_KP_MULTIPLY = SC_TO_KEYCODE(SC_KP_MULTIPLY),
    K_KP_MINUS = SC_TO_KEYCODE(SC_KP_MINUS),
    K_KP_PLUS = SC_TO_KEYCODE(SC_KP_PLUS),
    K_KP_ENTER = SC_TO_KEYCODE(SC_KP_ENTER),
    K_KP_1 = SC_TO_KEYCODE(SC_KP_1),
    K_KP_2 = SC_TO_KEYCODE(SC_KP_2),
    K_KP_3 = SC_TO_KEYCODE(SC_KP_3),
    K_KP_4 = SC_TO_KEYCODE(SC_KP_4),
    K_KP_5 = SC_TO_KEYCODE(SC_KP_5),
    K_KP_6 = SC_TO_KEYCODE(SC_KP_6),
    K_KP_7 = SC_TO_KEYCODE(SC_KP_7),
    K_KP_8 = SC_TO_KEYCODE(SC_KP_8),
    K_KP_9 = SC_TO_KEYCODE(SC_KP_9),
    K_KP_0 = SC_TO_KEYCODE(SC_KP_0),
    K_KP_PERIOD = SC_TO_KEYCODE(SC_KP_PERIOD),

    /**
     * Super Special Keys
     */
    K_APPLICATION = SC_TO_KEYCODE(SC_APPLICATION),
    K_POWER = SC_TO_KEYCODE(SC_POWER),
    K_KP_EQUALS = SC_TO_KEYCODE(SC_KP_EQUALS),

    /**
     * Extended Function Keys
     */
    K_F13 = SC_TO_KEYCODE(SC_F13),
    K_F14 = SC_TO_KEYCODE(SC_F14),
    K_F15 = SC_TO_KEYCODE(SC_F15),
    K_F16 = SC_TO_KEYCODE(SC_F16),
    K_F17 = SC_TO_KEYCODE(SC_F17),
    K_F18 = SC_TO_KEYCODE(SC_F18),
    K_F19 = SC_TO_KEYCODE(SC_F19),
    K_F20 = SC_TO_KEYCODE(SC_F20),
    K_F21 = SC_TO_KEYCODE(SC_F21),
    K_F22 = SC_TO_KEYCODE(SC_F22),
    K_F23 = SC_TO_KEYCODE(SC_F23),
    K_F24 = SC_TO_KEYCODE(SC_F24),

    /**
     * Macro Keys
     */
    K_EXECUTE = SC_TO_KEYCODE(SC_EXECUTE),
    K_HELP = SC_TO_KEYCODE(SC_HELP),
    K_MENU = SC_TO_KEYCODE(SC_MENU),
    K_SELECT = SC_TO_KEYCODE(SC_SELECT),
    K_STOP = SC_TO_KEYCODE(SC_STOP),
    K_AGAIN = SC_TO_KEYCODE(SC_AGAIN),
    K_UNDO = SC_TO_KEYCODE(SC_UNDO),
    K_CUT = SC_TO_KEYCODE(SC_CUT),
    K_COPY = SC_TO_KEYCODE(SC_COPY),
    K_PASTE = SC_TO_KEYCODE(SC_PASTE),
    K_FIND = SC_TO_KEYCODE(SC_FIND),
    K_MUTE = SC_TO_KEYCODE(SC_MUTE),
    K_VOLUMEUP = SC_TO_KEYCODE(SC_VOLUMEUP),
    K_VOLUMEDOWN = SC_TO_KEYCODE(SC_VOLUMEDOWN),
    K_KP_COMMA = SC_TO_KEYCODE(SC_KP_COMMA),
    K_KP_EQUALSAS400 = SC_TO_KEYCODE(SC_KP_EQUALSAS400),

    /**
     * Removed Keys?
     */
    K_ALTERASE = SC_TO_KEYCODE(SC_ALTERASE),
    K_SYSREQ = SC_TO_KEYCODE(SC_SYSREQ),
    K_CANCEL = SC_TO_KEYCODE(SC_CANCEL),
    K_CLEAR = SC_TO_KEYCODE(SC_CLEAR),
    K_PRIOR = SC_TO_KEYCODE(SC_PRIOR),
    K_RETURN2 = SC_TO_KEYCODE(SC_RETURN2),
    K_SEPARATOR = SC_TO_KEYCODE(SC_SEPARATOR),
    K_OUT = SC_TO_KEYCODE(SC_OUT),
    K_OPER = SC_TO_KEYCODE(SC_OPER),
    K_CLEARAGAIN = SC_TO_KEYCODE(SC_CLEARAGAIN),
    K_CRSEL = SC_TO_KEYCODE(SC_CRSEL),
    K_EXSEL = SC_TO_KEYCODE(SC_EXSEL),

    /**
     * Magic Keypad
     */
    K_KP_00 = SC_TO_KEYCODE(SC_KP_00),
    K_KP_000 = SC_TO_KEYCODE(SC_KP_000),
    K_THOUSANDSSEPARATOR = SC_TO_KEYCODE(SC_THOUSANDSSEPARATOR),
    K_DECIMALSEPARATOR = SC_TO_KEYCODE(SC_DECIMALSEPARATOR),
    K_CURRENCYUNIT = SC_TO_KEYCODE(SC_CURRENCYUNIT),
    K_CURRENCYSUBUNIT = SC_TO_KEYCODE(SC_CURRENCYSUBUNIT),
    K_KP_LEFTPAREN = SC_TO_KEYCODE(SC_KP_LEFTPAREN),
    K_KP_RIGHTPAREN = SC_TO_KEYCODE(SC_KP_RIGHTPAREN),
    K_KP_LEFTBRACE = SC_TO_KEYCODE(SC_KP_LEFTBRACE),
    K_KP_RIGHTBRACE = SC_TO_KEYCODE(SC_KP_RIGHTBRACE),
    K_KP_TAB = SC_TO_KEYCODE(SC_KP_TAB),
    K_KP_BACKSPACE = SC_TO_KEYCODE(SC_KP_BACKSPACE),
    K_KP_A = SC_TO_KEYCODE(SC_KP_A),
    K_KP_B = SC_TO_KEYCODE(SC_KP_B),
    K_KP_C = SC_TO_KEYCODE(SC_KP_C),
    K_KP_D = SC_TO_KEYCODE(SC_KP_D),
    K_KP_E = SC_TO_KEYCODE(SC_KP_E),
    K_KP_F = SC_TO_KEYCODE(SC_KP_F),
    K_KP_XOR = SC_TO_KEYCODE(SC_KP_XOR),
    K_KP_POWER = SC_TO_KEYCODE(SC_KP_POWER),
    K_KP_PERCENT = SC_TO_KEYCODE(SC_KP_PERCENT),
    K_KP_LESS = SC_TO_KEYCODE(SC_KP_LESS),
    K_KP_GREATER = SC_TO_KEYCODE(SC_KP_GREATER),
    K_KP_AMPERSAND = SC_TO_KEYCODE(SC_KP_AMPERSAND),
    K_KP_DBLAMPERSAND = SC_TO_KEYCODE(SC_KP_DBLAMPERSAND),
    K_KP_VERTICALBAR = SC_TO_KEYCODE(SC_KP_VERTICALBAR),
    K_KP_DBLVERTICALBAR = SC_TO_KEYCODE(SC_KP_DBLVERTICALBAR),
    K_KP_COLON = SC_TO_KEYCODE(SC_KP_COLON),
    K_KP_HASH = SC_TO_KEYCODE(SC_KP_HASH),
    K_KP_SPACE = SC_TO_KEYCODE(SC_KP_SPACE),
    K_KP_AT = SC_TO_KEYCODE(SC_KP_AT),
    K_KP_EXCLAM = SC_TO_KEYCODE(SC_KP_EXCLAM),
    K_KP_MEMSTORE = SC_TO_KEYCODE(SC_KP_MEMSTORE),
    K_KP_MEMRECALL = SC_TO_KEYCODE(SC_KP_MEMRECALL),
    K_KP_MEMCLEAR = SC_TO_KEYCODE(SC_KP_MEMCLEAR),
    K_KP_MEMADD = SC_TO_KEYCODE(SC_KP_MEMADD),
    K_KP_MEMSUBTRACT = SC_TO_KEYCODE(SC_KP_MEMSUBTRACT),
    K_KP_MEMMULTIPLY = SC_TO_KEYCODE(SC_KP_MEMMULTIPLY),
    K_KP_MEMDIVIDE = SC_TO_KEYCODE(SC_KP_MEMDIVIDE),
    K_KP_PLUSMINUS = SC_TO_KEYCODE(SC_KP_PLUSMINUS),
    K_KP_CLEAR = SC_TO_KEYCODE(SC_KP_CLEAR),
    K_KP_CLEARENTRY = SC_TO_KEYCODE(SC_KP_CLEARENTRY),
    K_KP_BINARY = SC_TO_KEYCODE(SC_KP_BINARY),
    K_KP_OCTAL = SC_TO_KEYCODE(SC_KP_OCTAL),
    K_KP_DECIMAL = SC_TO_KEYCODE(SC_KP_DECIMAL),
    K_KP_HEXADECIMAL = SC_TO_KEYCODE(SC_KP_HEXADECIMAL),

    /**
     * Modifier Keys
     */
    K_LCTRL = SC_TO_KEYCODE(SC_LCTRL),
    K_LSHIFT = SC_TO_KEYCODE(SC_LSHIFT),
    K_LALT = SC_TO_KEYCODE(SC_LALT),
    K_LGUI = SC_TO_KEYCODE(SC_LGUI),
    K_RCTRL = SC_TO_KEYCODE(SC_RCTRL),
    K_RSHIFT = SC_TO_KEYCODE(SC_RSHIFT),
    K_RALT = SC_TO_KEYCODE(SC_RALT),
    K_RGUI = SC_TO_KEYCODE(SC_RGUI),

    /** 
     * Size
     */
    K_SIZE = 210
};

#endif