#include <iostream>
#include <fstream>
#include <string>

namespace {
    #include "font5x8.h"

    constexpr int dimx = 384u, dimy = 1'000u;
    char line[dimx / 8] = { 0 };

    void set_pixel(int x)
    {
        int i = x / 8;
        int j = 7 - (x % 8);
        line[i] |= (1 << j);
    }

    void draw_horizontal_line(int x1, int x2)
    {
        if (x1 > x2)
            std::swap(x1, x2);

        for (int i = x1; i < x2; ++i)
            set_pixel(i);
    }

}

int main()
{
    std::ofstream ofs("C:/Temp/sample9.pbm", std::ios_base::out | std::ios_base::binary);
    ofs << "P4" << std::endl << dimx << ' ' << dimy << std::endl;

    for (int j = 0u; j < dimy; ++j) {
        // Diagonal line
        set_pixel(j % dimx);

        // Horizontal line at the end of the Diagonal line (Every 384px)
        if (j % dimx == 0) {
            draw_horizontal_line(0, dimx - 1);
        }

        // ----- Dashes every 5px, 10px, 50px, 100px -----
        if (j % 5 == 0) {
            draw_horizontal_line(dimx - 2, dimx - 1);
        }

        if (j % 10 == 0) {
            draw_horizontal_line(dimx - 11, dimx - 1);
        }

        if (j % 50 == 0) {
            draw_horizontal_line(dimx - 21, dimx - 1);
        }

        if (j % 100 == 0) {
            draw_horizontal_line(dimx - 41, dimx - 1);
        }
        // -----------------------------------------------

        // Add label every 100px
        // 
        //    px |          | 
        //       |          |
        //    97 | glyph[0] |             ^                ^
        //    98 | glyph[1] |             | v_offset = 3   |
        //    99 | glyph[2] |   11        v                |
        //   100 | glyph[3] |  1  1   <----- glyph virtual |
        //   101 | glyph[4] |  1  1          center        | font_heght = 8
        //   102 | glyph[5] |  1  1                        |
        //   103 | glyph[6] |   11                         |
        //   104 | glyph[7] |                              v

        int v_offset = 4;
        int k = (j + v_offset) % 100;
        int font_height = 8;
        if (k < font_height) {
            int val = ((j + v_offset) / 100) * 100;
            std::string s = std::to_string(val);
            for (int n = 0; n < s.length(); ++n)
                line[((384 / 8) - 5) - s.length() + n] |= console_font_5x8[s[n]][k];
        }

        // Save pixel line to output stream
        for (int i = 0u; i < sizeof(line); ++i) {
            ofs << line[i];
            line[i] = 0;
        }
    }

    ofs.close();

    return EXIT_SUCCESS;
}
