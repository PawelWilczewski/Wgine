#pragma once

// from glfw3.h
// mouse buttons
#define WGINE_MOUSE_BUTTON_1         0
#define WGINE_MOUSE_BUTTON_2         1
#define WGINE_MOUSE_BUTTON_3         2
#define WGINE_MOUSE_BUTTON_4         3
#define WGINE_MOUSE_BUTTON_5         4
#define WGINE_MOUSE_BUTTON_6         5
#define WGINE_MOUSE_BUTTON_7         6
#define WGINE_MOUSE_BUTTON_8         7
#define WGINE_MOUSE_BUTTON_LAST      WGINE_MOUSE_BUTTON_8
#define WGINE_MOUSE_BUTTON_LEFT      WGINE_MOUSE_BUTTON_1
#define WGINE_MOUSE_BUTTON_RIGHT     WGINE_MOUSE_BUTTON_2
#define WGINE_MOUSE_BUTTON_MIDDLE    WGINE_MOUSE_BUTTON_3

/* Printable keys */
#define WGINE_KEY_SPACE              32
#define WGINE_KEY_APOSTROPHE         39  /* ' */
#define WGINE_KEY_COMMA              44  /* , */
#define WGINE_KEY_MINUS              45  /* - */
#define WGINE_KEY_PERIOD             46  /* . */
#define WGINE_KEY_SLASH              47  /* / */
#define WGINE_KEY_0                  48
#define WGINE_KEY_1                  49
#define WGINE_KEY_2                  50
#define WGINE_KEY_3                  51
#define WGINE_KEY_4                  52
#define WGINE_KEY_5                  53
#define WGINE_KEY_6                  54
#define WGINE_KEY_7                  55
#define WGINE_KEY_8                  56
#define WGINE_KEY_9                  57
#define WGINE_KEY_SEMICOLON          59  /* ; */
#define WGINE_KEY_EQUAL              61  /* = */
#define WGINE_KEY_A                  65
#define WGINE_KEY_B                  66
#define WGINE_KEY_C                  67
#define WGINE_KEY_D                  68
#define WGINE_KEY_E                  69
#define WGINE_KEY_F                  70
#define WGINE_KEY_G                  71
#define WGINE_KEY_H                  72
#define WGINE_KEY_I                  73
#define WGINE_KEY_J                  74
#define WGINE_KEY_K                  75
#define WGINE_KEY_L                  76
#define WGINE_KEY_M                  77
#define WGINE_KEY_N                  78
#define WGINE_KEY_O                  79
#define WGINE_KEY_P                  80
#define WGINE_KEY_Q                  81
#define WGINE_KEY_R                  82
#define WGINE_KEY_S                  83
#define WGINE_KEY_T                  84
#define WGINE_KEY_U                  85
#define WGINE_KEY_V                  86
#define WGINE_KEY_W                  87
#define WGINE_KEY_X                  88
#define WGINE_KEY_Y                  89
#define WGINE_KEY_Z                  90
#define WGINE_KEY_LEFT_BRACKET       91  /* [ */
#define WGINE_KEY_BACKSLASH          92  /* \ */
#define WGINE_KEY_RIGHT_BRACKET      93  /* ] */
#define WGINE_KEY_GRAVE_ACCENT       96  /* ` */
#define WGINE_KEY_WORLD_1            161 /* non-US #1 */
#define WGINE_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define WGINE_KEY_ESCAPE             256
#define WGINE_KEY_ENTER              257
#define WGINE_KEY_TAB                258
#define WGINE_KEY_BACKSPACE          259
#define WGINE_KEY_INSERT             260
#define WGINE_KEY_DELETE             261
#define WGINE_KEY_RIGHT              262
#define WGINE_KEY_LEFT               263
#define WGINE_KEY_DOWN               264
#define WGINE_KEY_UP                 265
#define WGINE_KEY_PAGE_UP            266
#define WGINE_KEY_PAGE_DOWN          267
#define WGINE_KEY_HOME               268
#define WGINE_KEY_END                269
#define WGINE_KEY_CAPS_LOCK          280
#define WGINE_KEY_SCROLL_LOCK        281
#define WGINE_KEY_NUM_LOCK           282
#define WGINE_KEY_PRINT_SCREEN       283
#define WGINE_KEY_PAUSE              284
#define WGINE_KEY_F1                 290
#define WGINE_KEY_F2                 291
#define WGINE_KEY_F3                 292
#define WGINE_KEY_F4                 293
#define WGINE_KEY_F5                 294
#define WGINE_KEY_F6                 295
#define WGINE_KEY_F7                 296
#define WGINE_KEY_F8                 297
#define WGINE_KEY_F9                 298
#define WGINE_KEY_F10                299
#define WGINE_KEY_F11                300
#define WGINE_KEY_F12                301
#define WGINE_KEY_F13                302
#define WGINE_KEY_F14                303
#define WGINE_KEY_F15                304
#define WGINE_KEY_F16                305
#define WGINE_KEY_F17                306
#define WGINE_KEY_F18                307
#define WGINE_KEY_F19                308
#define WGINE_KEY_F20                309
#define WGINE_KEY_F21                310
#define WGINE_KEY_F22                311
#define WGINE_KEY_F23                312
#define WGINE_KEY_F24                313
#define WGINE_KEY_F25                314
#define WGINE_KEY_KP_0               320
#define WGINE_KEY_KP_1               321
#define WGINE_KEY_KP_2               322
#define WGINE_KEY_KP_3               323
#define WGINE_KEY_KP_4               324
#define WGINE_KEY_KP_5               325
#define WGINE_KEY_KP_6               326
#define WGINE_KEY_KP_7               327
#define WGINE_KEY_KP_8               328
#define WGINE_KEY_KP_9               329
#define WGINE_KEY_KP_DECIMAL         330
#define WGINE_KEY_KP_DIVIDE          331
#define WGINE_KEY_KP_MULTIPLY        332
#define WGINE_KEY_KP_SUBTRACT        333
#define WGINE_KEY_KP_ADD             334
#define WGINE_KEY_KP_ENTER           335
#define WGINE_KEY_KP_EQUAL           336
#define WGINE_KEY_LEFT_SHIFT         340
#define WGINE_KEY_LEFT_CONTROL       341
#define WGINE_KEY_LEFT_ALT           342
#define WGINE_KEY_LEFT_SUPER         343
#define WGINE_KEY_RIGHT_SHIFT        344
#define WGINE_KEY_RIGHT_CONTROL      345
#define WGINE_KEY_RIGHT_ALT          346
#define WGINE_KEY_RIGHT_SUPER        347
#define WGINE_KEY_MENU               348
