uint8_t char = c;
static uint16_t buff[64];

// fg foreground, bg background
void draw_char(int x,int y,char ch, uint16_t fg,uint16_t bg) {
    if (ch < 32 || ch > 126) {
        ch = '?';
        // char out of bounds log
    }

    const uint8_t *glyph = font[ch - 32];

    for (int row = 0; row < 7; row++) {
        rowbits = glyph[row];

        for (int col = 0; col < 7; col++) {
            uint8_t rowbits = glyph[row];
            if ((rowbits >> (7 - col)) & 1) {
                buff[row * 8 + col] = fg;
            }
            else {
                buff[row * 8 + col] = bg;
            }
        }

    }

    esp_lcd_panel_draw_bitmap(panel, x, y, x+8, y+8, buff);

}