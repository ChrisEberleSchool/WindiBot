#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <U8g2lib.h>

// Enum for all fonts you want to use
enum class FontStyle {
    MenuSmall,
    MenuMedium,
    MenuBold,
    NumbersSmall,
    NumbersLarge,
    RetroTiny,
};

// Helper function to get font pointer from enum
inline const uint8_t* getFont(FontStyle style) {
    switch (style) {
        case FontStyle::MenuSmall:
            return u8g2_font_6x10_tf;       // clean, small for menus
        case FontStyle::MenuMedium:
            return u8g2_font_7x13_tf;       // medium
        case FontStyle::MenuBold:
            return u8g2_font_8x13B_tf;      // bold for selected items
        case FontStyle::NumbersSmall:
            return u8g2_font_profont10_mf;  // pixel, numbers
        case FontStyle::NumbersLarge:
            return u8g2_font_inb16_mf;      // big numbers, bold
        case FontStyle::RetroTiny:
            return u8g2_font_04b_03_tr;     // tiny retro pixel
        default:
            return u8g2_font_6x10_tf;       // fallback
    }
}

#endif
