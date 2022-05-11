typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;
#define null 0

struct sigaction {
    void (*sa_handler)(int);
    uint sigmask;
};
