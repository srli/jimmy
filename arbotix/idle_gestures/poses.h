#ifndef JIMMY_ARMS_POSES
#define JIMMY_ARMS_POSES

#include <avr/pgmspace.h>

PROGMEM prog_uint16_t con1[] = {8, 2241, 1927, 2868, 2881, 818, 2795, 1513, 884};
PROGMEM prog_uint16_t yay3[] = {8, 2140, 2130, 2204, 3471, 1483, 2007, 736, 1768};
PROGMEM prog_uint16_t yay2[] = {8, 2143, 2089, 2180, 3466, 768, 2734, 747, 1760};
PROGMEM prog_uint16_t r_elbow_up[] = {8, 2007, 2016, 2680, 2829, 791, 1781, 1191, 790};
PROGMEM prog_uint16_t l_wave2[] = {8, 2035, 2049, 2766, 3472, 1778, 2977, 1530, 1661};
PROGMEM prog_uint16_t l_wave1[] = {8, 2034, 2048, 2766, 3445, 1778, 2172, 1530, 1774};
PROGMEM prog_uint16_t con2[] = {8, 2187, 1850, 2867, 2772, 622, 2922, 1855, 524};
PROGMEM prog_uint16_t ran1[] = {8, 2062, 2012, 2637, 3093, 1834, 1786, 1333, 985};
PROGMEM prog_uint16_t rest[] = {8, 2037, 2024, 2769, 2824, 1775, 1760, 1527, 805};
PROGMEM prog_uint16_t ran3[] = {8, 1873, 1773, 2696, 3005, 1744, 1860, 1420, 739};
PROGMEM prog_uint16_t ran2[] = {8, 2311, 2315, 2732, 2910, 1842, 1963, 1424, 991};
PROGMEM prog_uint16_t r_wave1[] = {8, 2073, 2016, 2761, 2829, 596, 1781, 487, 790};
PROGMEM prog_uint16_t l_elbow_up2[] = {8, 2032, 2048, 2765, 3507, 1778, 2126, 1530, 1682};
PROGMEM prog_uint16_t l_elbow_up1[] = {8, 2032, 2054, 2765, 3160, 1778, 1790, 1530, 1300};
PROGMEM prog_uint16_t r_wave2[] = {8, 2067, 2016, 2749, 2829, 1014, 1781, 742, 790};

PROGMEM transition_t yay[] = {{0,5} ,{rest,1000} ,{yay2,2000} ,{yay3,1500} ,{yay2,2000} ,{rest,1500} };
PROGMEM transition_t l_wave[] = {{0,9} ,{rest,1000} ,{l_elbow_up1,1500} ,{l_wave1,1500} ,{l_wave2,1500} ,{l_wave1,1000} ,{l_wave2,1000} ,{l_wave1,1000} ,{l_elbow_up1,2000} ,{rest,1500} };
PROGMEM transition_t penguin[] = {{0,5} ,{rest,500} ,{ran1,1000} ,{rest,500} ,{ran2,1000} ,{rest,1500} };

#endif
