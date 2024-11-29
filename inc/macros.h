#ifndef MACROS
#define MACROS

#define RANDF() (float)rand()/(float)RAND_MAX
#define RANDINT(X) (int)(RANDF() * X)

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#endif