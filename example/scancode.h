#ifndef _SCANCODE_
#define _SCANCODE_

/**
 *  Physical keyboard scancode values taken from:
 *  https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf
 */
enum ScanCode : unsigned short  {
    /** 
     * Undefined
     */
    SC_UNKNOWN = 0,

    /** 
     * Error Codes
     */
    SC_ERROR_ROLL_OVER = 1,
    SC_ERROR_POST_FAIL = 2,
    SC_ERROR_UNDEFINED = 3,

    /** 
     * Alphabet
     * - No uppercase as keyboard modifiers used.
     */
    SC_A = 4,
    SC_B = 5,
    SC_C = 6,
    SC_D = 7,
    SC_E = 8,
    SC_F = 9,
    SC_G = 10,
    SC_H = 11,
    SC_I = 12,
    SC_J = 13,
    SC_K = 14,
    SC_L = 15,
    SC_M = 16,
    SC_N = 17,
    SC_O = 18,
    SC_P = 19,
    SC_Q = 20,
    SC_R = 21,
    SC_S = 22,
    SC_T = 23,
    SC_U = 24,
    SC_V = 25,
    SC_W = 26,
    SC_X = 27,
    SC_Y = 28,
    SC_Z = 29,

    /** 
     * Numbers
     */
    SC_1 = 30,
    SC_2 = 31,
    SC_3 = 32,
    SC_4 = 33,
    SC_5 = 34,
    SC_6 = 35,
    SC_7 = 36,
    SC_8 = 37,
    SC_9 = 38,
    SC_0 = 39,

    /** 
     * Special Chars
     */
    SC_RETURN = 40,
    SC_ESCAPE = 41,
    SC_BACKSPACE = 42,
    SC_TAB = 43,
    SC_SPACE = 44,

    /** 
     * Operators
     */
    SC_MINUS = 45,
    SC_EQUALS = 46,
    SC_LEFTBRACKET = 47,
    SC_RIGHTBRACKET = 48,
    SC_BACKSLASH = 49, 
    SC_NONUSHASH = 50,
    SC_SEMICOLON = 51,
    SC_APOSTROPHE = 52,
    SC_GRAVE = 53,
    SC_COMMA = 54,
    SC_PERIOD = 55,
    SC_SLASH = 56,
    SC_CAPSLOCK = 57,

    /** 
     * Function Keys
     */
    SC_F1 = 58,
    SC_F2 = 59,
    SC_F3 = 60,
    SC_F4 = 61,
    SC_F5 = 62,
    SC_F6 = 63,
    SC_F7 = 64,
    SC_F8 = 65,
    SC_F9 = 66,
    SC_F10 = 67,
    SC_F11 = 68,
    SC_F12 = 69,

    /** 
     * Special Modifiers
     */
    SC_PRINTSCREEN = 70,
    SC_SCROLLLOCK = 71,
    SC_PAUSE = 72,
    SC_INSERT = 73,
    SC_HOME = 74,
    SC_PAGEUP = 75,
    SC_DELETE = 76,
    SC_END = 77,
    SC_PAGEDOWN = 78,
    
    /** 
     * Arrow Keys
     */
    SC_RIGHT = 79,
    SC_LEFT = 80,
    SC_DOWN = 81,
    SC_UP = 82,

    SC_NUMLOCKCLEAR = 83, 

    /** 
     * KeyPad
     */
    SC_KP_DIVIDE = 84,
    SC_KP_MULTIPLY = 85,
    SC_KP_MINUS = 86,
    SC_KP_PLUS = 87,
    SC_KP_ENTER = 88,
    SC_KP_1 = 89,
    SC_KP_2 = 90,
    SC_KP_3 = 91,
    SC_KP_4 = 92,
    SC_KP_5 = 93,
    SC_KP_6 = 94,
    SC_KP_7 = 95,
    SC_KP_8 = 96,
    SC_KP_9 = 97,
    SC_KP_0 = 98,
    SC_KP_PERIOD = 99,

    /** 
     * Super Special Keys
     */
    SC_NONUSBACKSLASH = 100,
    SC_APPLICATION = 101,
    SC_POWER = 102,

    SC_KP_EQUALS = 103,

    /** 
     * Extended Function Keys
     */
    SC_F13 = 104,
    SC_F14 = 105,
    SC_F15 = 106,
    SC_F16 = 107,
    SC_F17 = 108,
    SC_F18 = 109,
    SC_F19 = 110,
    SC_F20 = 111,
    SC_F21 = 112,
    SC_F22 = 113,
    SC_F23 = 114,
    SC_F24 = 115,

    /** 
     * Macro Keys
     */
    SC_EXECUTE = 116,
    SC_HELP = 117,
    SC_MENU = 118,
    SC_SELECT = 119,
    SC_STOP = 120,
    SC_AGAIN = 121,
    SC_UNDO = 122,
    SC_CUT = 123,
    SC_COPY = 124,
    SC_PASTE = 125,
    SC_FIND = 126,
    SC_MUTE = 127,
    SC_VOLUMEUP = 128,
    SC_VOLUMEDOWN = 129,

    /** 
     * Locking
     */
    SC_LOCKINGCAPSLOCK = 130,
    SC_LOCKINGNUMLOCK = 131,
    SC_LOCKINGSCROLLLOCK = 132,

    SC_KP_COMMA = 133,
    SC_KP_EQUALSAS400 = 134,

    /** 
     * International
     * - Check doc.
     */
    SC_INTERNATIONAL1 = 135,
    SC_INTERNATIONAL2 = 136,
    SC_INTERNATIONAL3 = 137,
    SC_INTERNATIONAL4 = 138,
    SC_INTERNATIONAL5 = 139,
    SC_INTERNATIONAL6 = 140,
    SC_INTERNATIONAL7 = 141,
    SC_INTERNATIONAL8 = 142,
    SC_INTERNATIONAL9 = 143,
    SC_LANG1 = 144,
    SC_LANG2 = 145,
    SC_LANG3 = 146,
    SC_LANG4 = 147,
    SC_LANG5 = 148,
    SC_LANG6 = 149,
    SC_LANG7 = 150,
    SC_LANG8 = 151,
    SC_LANG9 = 152,

    /** 
     * Removed Keys?
     */
    SC_ALTERASE = 153,
    SC_SYSREQ = 154,
    SC_CANCEL = 155,
    SC_CLEAR = 156,
    SC_PRIOR = 157,
    SC_RETURN2 = 158,
    SC_SEPARATOR = 159,
    SC_OUT = 160,
    SC_OPER = 161,
    SC_CLEARAGAIN = 162,
    SC_CRSEL = 163,
    SC_EXSEL = 164,

    /** 
     * Magic Keypad
     */
    SC_KP_00 = 176,
    SC_KP_000 = 177,
    SC_THOUSANDSSEPARATOR = 178,
    SC_DECIMALSEPARATOR = 179,
    SC_CURRENCYUNIT = 180,
    SC_CURRENCYSUBUNIT = 181,
    SC_KP_LEFTPAREN = 182,
    SC_KP_RIGHTPAREN = 183,
    SC_KP_LEFTBRACE = 184,
    SC_KP_RIGHTBRACE = 185,
    SC_KP_TAB = 186,
    SC_KP_BACKSPACE = 187,
    SC_KP_A = 188,
    SC_KP_B = 189,
    SC_KP_C = 190,
    SC_KP_D = 191,
    SC_KP_E = 192,
    SC_KP_F = 193,
    SC_KP_XOR = 194,
    SC_KP_POWER = 195,
    SC_KP_PERCENT = 196,
    SC_KP_LESS = 197,
    SC_KP_GREATER = 198,
    SC_KP_AMPERSAND = 199,
    SC_KP_DBLAMPERSAND = 200,
    SC_KP_VERTICALBAR = 201,
    SC_KP_DBLVERTICALBAR = 202,
    SC_KP_COLON = 203,
    SC_KP_HASH = 204,
    SC_KP_SPACE = 205,
    SC_KP_AT = 206,
    SC_KP_EXCLAM = 207,
    SC_KP_MEMSTORE = 208,
    SC_KP_MEMRECALL = 209,
    SC_KP_MEMCLEAR = 210,
    SC_KP_MEMADD = 211,
    SC_KP_MEMSUBTRACT = 212,
    SC_KP_MEMMULTIPLY = 213,
    SC_KP_MEMDIVIDE = 214,
    SC_KP_PLUSMINUS = 215,
    SC_KP_CLEAR = 216,
    SC_KP_CLEARENTRY = 217,
    SC_KP_BINARY = 218,
    SC_KP_OCTAL = 219,
    SC_KP_DECIMAL = 220,
    SC_KP_HEXADECIMAL = 221,

    /** 
     * Modifier Keys
     */
    SC_LCTRL = 224,
    SC_LSHIFT = 225,
    SC_LALT = 226,
    SC_LGUI = 227,
    SC_RCTRL = 228,
    SC_RSHIFT = 229,
    SC_RALT = 230,
    SC_RGUI = 231,

    /** 
     * Size Key
     */
    SC_SIZE = 232
};

#endif