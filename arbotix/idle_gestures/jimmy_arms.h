#ifndef JIMMY_ARMS_POSES
#define JIMMY_ARMS_POSES

#include <avr/pgmspace.h>

PROGMEM prog_uint16_t l_elbow_up[] = {8, 1963, 1827, 2844, 2818, 1725, 2175, 1037, 2416};
PROGMEM prog_uint16_t r_elbow_up[] = {8, 2063, 512, 2596, 512, 1018, 512, 704, 512};
PROGMEM prog_uint16_t l_wave5[] = {8, 2103, 2080, 2813, 3269, 1792, 2929, 1610, 350};
PROGMEM prog_uint16_t l_wave4[] = {8, 2100, 2123, 2811, 3316, 1792, 2821, 1610, 467};
PROGMEM prog_uint16_t l_wave3[] = {8, 2100, 2109, 2812, 3381, 1792, 2151, 1610, 533};
PROGMEM prog_uint16_t l_wave2[] = {8, 2099, 2089, 2812, 2819, 1794, 1877, 1609, 3692};
PROGMEM prog_uint16_t l_wave1[] = {8, 2096, 2109, 2811, 3234, 1791, 2699, 1610, 520};
PROGMEM prog_uint16_t rest[] = {8, 2089, 2750, 2824, 2813, 1871, 1849, 1435, 2368};
PROGMEM prog_uint16_t r_wave1[] = {8, 2062, 512, 2580, 512, 589, 512, 533, 512};
PROGMEM prog_uint16_t r_wave2[] = {8, 2050, 512, 2597, 512, 1076, 512, 515, 512};

PROGMEM transition_t r_wave[] = {{0,7} ,{rest,1000} ,{r_elbow_up,2000} ,{r_wave1,2000} ,{r_wave2,2000} ,{r_wave1,2000} ,{r_elbow_up,2000} ,{rest,2000} };
PROGMEM transition_t l_wave[] = {{0,7} ,{rest,500} ,{l_elbow_up,500} ,{l_wave1,500} ,{l_wave2,500} ,{l_wave3,500} ,{l_wave4,500} ,{l_wave5,500} };

#endif
