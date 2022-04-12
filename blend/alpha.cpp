#include <SFML/Graphics.hpp>
#include <immintrin.h>
#include <alpha.hpp>


#define PRINT_LINE printf("[%s:%d]\n", __func__, __LINE__);


const char I = 255u,
           Z = 128u;

const __m128i   _0 =                    _mm_set_epi8 (0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0);
const __m128i _255 = _mm_cvtepu8_epi16 (_mm_set_epi8 (I,I,I,I, I,I,I,I, I,I,I,I, I,I,I,I));


int useSSE(sf::Uint32 *front, sf::Uint32 *back, sf::Uint32 *src)
{
    if (!front || !back || !src)
    {
        printf("Wrong array ptr\n");
        return -1;
    }
    for (int i = 0; i < 5000; ++i)
    {
        for (int y = 0; y < heigth; y++, src += width, front += width, back += width)
        {
            for (int x = 0; x < width; x += 4)
            {
                //-----------------------------------------------------------------------
                //       15 14 13 12   11 10  9  8    7  6  5  4    3  2  1  0
                // fr = [r3 g3 b3 a3 | r2 g2 b2 a2 | r1 g1 b1 a1 | r0 g0 b0 a0]
                //-----------------------------------------------------------------------

                __m128i fr = _mm_loadu_si128 ((__m128i*) (front + x));                   // fr = front[y][x]
                __m128i bk = _mm_loadu_si128 ((__m128i*) (back  + x));

                //-----------------------------------------------------------------------
                //       15 14 13 12   11 10  9  8    7  6  5  4    3  2  1  0
                // fr = [a3 r3 g3 b3 | a2 r2 g2 b2 | a1 r1 g1 b1 | a0 r0 g0 b0]
                //        \  \  \  \    \  \  \  \   xx xx xx xx   xx xx xx xx            
                //         \  \  \  \    \  \  \  \.
                //          \  \  \  \    '--+--+--+-------------+--+--+--.
                //           '--+--+--+------------+--+--+--.     \  \  \  \.
                //                                  \  \  \  \     \  \  \  \.
                // FR = [-- -- -- -- | -- -- -- -- | a3 r3 g3 b3 | a2 r2 g2 b2]
                //-----------------------------------------------------------------------

                __m128i FR = (__m128i) _mm_movehl_ps ((__m128) _0, (__m128) fr);       // FR = (fr >> 8*8)
                __m128i BK = (__m128i) _mm_movehl_ps ((__m128) _0, (__m128) bk);

                //-----------------------------------------------------------------------
                //       15 14 13 12   11 10  9  8    7  6  5  4    3  2  1  0
                // fr = [a3 r3 g3 b3 | a2 r2 g2 b2 | a1 r1 g1 b1 | a0 r0 g0 b0]
                //       xx xx xx xx   xx xx xx xx                 /  /   |  |
                //                                         _______/  /   /   |
                //            ...   ...     ...           /     ____/   /    |
                //           /     /       /             /     /       /     |
                // fr = [-- a1 -- r1 | -- g1 -- b1 | -- a0 -- r0 | -- g0 -- b0]
                //-----------------------------------------------------------------------

                fr = _mm_cvtepu8_epi16 (fr);                                               // fr[i] = (WORD) fr[i]
                FR = _mm_cvtepu8_epi16 (FR);

                bk = _mm_cvtepu8_epi16 (bk);
                BK = _mm_cvtepu8_epi16 (BK);

                //-----------------------------------------------------------------------
                //       15 14 13 12   11 10  9  8    7  6  5  4    3  2  1  0
                // fr = [-- a1 -- r1 | -- g1 -- b1 | -- a0 -- r0 | -- g0 -- b0]
                //          |___________________        |___________________
                //          |     \      \      \       |     \      \      \.
                // a  = [-- a1 -- a1 | -- a1 -- a1 | -- a0 -- a0 | -- a0 -- a0]
                //-----------------------------------------------------------------------

                static const __m128i moveA = _mm_set_epi8 (Z, 14, Z, 14, Z, 14, Z, 14,
                                                        Z,  6, Z,  6, Z,  6, Z, 6);

                __m128i a = _mm_shuffle_epi8 (fr, moveA);                                // a [for r0/b0/b0...] = a0...
                __m128i A = _mm_shuffle_epi8 (FR, moveA);

                //-----------------------------------------------------------------------

                fr = _mm_mullo_epi16 (fr, a);                                          // fr *= a
                FR = _mm_mullo_epi16 (FR, A);

                bk = _mm_mullo_epi16 (bk, _mm_sub_epi16 (_255, a));                     // bk *= (255-a)
                BK = _mm_mullo_epi16 (BK, _mm_sub_epi16 (_255, A));

                __m128i sum = _mm_add_epi16 (fr, bk);                                  // sum = fr*a + bk*(255-a)
                __m128i SUM = _mm_add_epi16 (FR, BK);
                //-----------------------------------------------------------------------
                //        15 14 13 12   11 10  9  8    7  6  5  4    3  2  1  0
                // sum = [A1 a1 R1 r1 | G1 g1 B1 b1 | A0 a0 R0 r0 | G0 g0 B0 b0]
                //         \     \       \     \       \_____\_______\_____\.
                //          \_____\_______\_____\______________    \  \  \  \.
                //                                    \  \  \  \    \  \  \  \.
                // sum = [-- -- -- -- | -- -- -- -- | A1 R1 G1 B1 | A0 R0 G0 B0]
                //-----------------------------------------------------------------------
                static const __m128i moveSum = _mm_set_epi8 (Z, Z, Z, Z, Z, Z, Z, Z, 
                                                            15, 13, 11, 9, 7, 5, 3, 1);

                sum = _mm_shuffle_epi8 (sum, moveSum);                                   // sum[i] = (sium[i] >> 8) = (sum[i] / 256)
                SUM = _mm_shuffle_epi8 (SUM, moveSum);

                //-----------------------------------------------------------------------
                //          15 14 13 12   11 10  9  8    7  6  5  4    3  2  1  0
                // sum   = [-- -- -- -- | -- -- -- -- | a1 r1 g1 b1 | a0 r0 g0 b0] ->-.
                // sumHi = [-- -- -- -- | -- -- -- -- | a3 r3 g3 b3 | a2 r2 g2 b2]    |
                //                                      /  /  /  /    /  /  /  /      V
                //             .--+--+--+----+--+--+--++--+--+--+----+--+--+--'       |
                //            /  /  /  /    /  /  /  /    ____________________________/
                //           /  /  /  /    /  /  /  /    /  /  /  /    /  /  /  /
                // color = [a3 r3 g3 b3 | a2 r2 g2 b2 | a1 r1 g1 b1 | a0 r0 g0 b0]
                //-----------------------------------------------------------------------

                __m128i color = (__m128i) _mm_movelh_ps ((__m128) sum, (__m128) SUM);  // color = (sumHi << 8*8) | sum

                _mm_storeu_si128 ((__m128i*) (src + x), color);
            }
        }

        src   -= width * heigth;
        front -= width * heigth;
        back  -= width * heigth;
    }

    return 0;
}


int noSSE(sf::Uint32 *front, sf::Uint32 *back, sf::Uint32 *src)
{
    if (!front || !back || !src)
    {
        printf("Wrong array ptr\n");
        return -1;
    }

    sf::Uint32 red   = 0x000000FF;
    sf::Uint32 green = 0x0000FF00;
    sf::Uint32 blue  = 0x00FF0000;
    sf::Uint32 trans = 0xFF000000; 
    
    for (int i = 0; i < 5000; ++i)
    {
        for (int y = 0; y < 600; y++, src += width, front += width, back += width)
        { 
            for (int x = 0; x < 800; x++)
            {
                sf::Uint32 fr = front[x];
                sf::Uint32 bk = back[x];

                uint32_t tf = (fr & trans) >> 24;

                uint8_t no_red   = ((fr & red)   * tf + (bk & red)   * (255 - tf)) >> 8;
                uint8_t no_green = ((fr & green) * tf + (bk & green) * (255 - tf)) >> 16;
                uint8_t no_blue  = ((fr & blue)  * tf + (bk & blue)  * (255 - tf)) >> 24;

                src[x]   = 0xFF000000 + no_red + (no_green << 8) + (no_blue << 16); 
            }
        }

        src   -= width * heigth;
        front -= width * heigth;
        back  -= width * heigth;
    }

    return 0;
}