#ifndef Arduino_h
#define Arduino_h

void randomSeed(unsigned long seed);
long random(long howbig);
long random(long howsmall, long howbig);
long map(long x, long in_min, long in_max, long out_min, long out_max);
unsigned int makeWord(unsigned int w);
unsigned int makeWord(unsigned char h, unsigned char l);

#endif // whole file