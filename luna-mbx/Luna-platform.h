typedef bool BOOL;
typedef unsigned int D3DCOLOR;

typedef struct {
  long left;
  long top;
  long right;
  long bottom;
} RECT;

typedef void *HWND;

#if defined (__GNUC__)

#define PACKED __attribute__ ((__packed__))
#define NOINLINE __attribute__ ((noinline))
#define UNLIKELY(x) __builtin_expect((x), 0)

#if defined (__i386__)
#define __cdecl __attribute__((__cdecl__))
#else
#define __cdecl
#endif

#else

#define PACKED
#define NOINLINE
#define UNLIKELY(x)
#undef __cdecl

#endif

#define FALSE false
#define TRUE true


#if defined (__GNUC__)

#define GLCOLOR_RGBA(_r,_g,_b,_a) ({ \
    union { \
        struct { \
            unsigned char r, g, b, a; \
        } c; \
        D3DCOLOR v; \
    } color; \
    color.c.r = (_r); \
    color.c.g = (_g); \
    color.c.b = (_b); \
    color.c.a = (_a); \
    color.v; \
})

#else

inline D3DCOLOR GLCOLOR_RGBA(unsigned int _r, unsigned int _g, unsigned int _b, unsigned int _a)
{
    union {
        struct {
            unsigned char r, g, b, a;
        } c;
        D3DCOLOR v;
    } color;
    color.c.r = _r;
    color.c.g = _g;
    color.c.b = _b;
    color.c.a = _a;
    return color.v;
}

#endif

//#define D3DCOLOR_ARGB(a,r,g,b)        ((D3DCOLOR)((((a)&0xffu)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define D3DCOLOR_ARGB(a,r,g,b)        GLCOLOR_RGBA(r,g,b,a)
#define D3DCOLOR_XRGB(r,g,b)          D3DCOLOR_ARGB(0xff,r,g,b)

BOOL SetRect(
  RECT   *lprc,
  int    xLeft,
  int    yTop,
  int    xRight,
  int    yBottom
);

enum {
    DIK_UP,
    DIK_DOWN,
    DIK_LEFT,
    DIK_RIGHT,
    DIK_NUMPAD2,
    DIK_NUMPAD4,
    DIK_NUMPAD6,
    DIK_NUMPAD8,
    DIK_Z,
    DIK_X,
    DIK_C,
    DIK_V,
};

