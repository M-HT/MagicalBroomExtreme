#include "Luna-mbx.h"
#include "Luna-mbx-private.h"

#include <cstdio>
#include <cstdlib>
#ifdef USE_GLES
#include <GLES/gl.h>
#include <GLES/glext.h>
#else
#if defined(PANDORA)
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#include <SDL_opengl.h>
#endif


static CList List;


void LogOut(const char *format, ...);

extern int featureFramebufferObject;
extern int featureNpotTexture;

#ifdef USE_GLES
extern int featureDrawTexture;

extern PFNGLBINDFRAMEBUFFEROESPROC glBindFramebuffer;
extern PFNGLDELETEFRAMEBUFFERSOESPROC glDeleteFramebuffers;
extern PFNGLGENFRAMEBUFFERSOESPROC glGenFramebuffers;
extern PFNGLFRAMEBUFFERTEXTURE2DOESPROC glFramebufferTexture2D;

extern PFNGLDRAWTEXIOESPROC glDrawTexiOES;

#if !defined(GL_FRAMEBUFFER)
#define GL_FRAMEBUFFER GL_FRAMEBUFFER_OES
#define GL_COLOR_ATTACHMENT0 GL_COLOR_ATTACHMENT0_OES
#endif

#else
extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
#endif


LunaSurface::LunaSurface( void )
{
    List.InsertTop(this);
    Format = 0;
    lpSurface = 0;
    Width = 0;
    Height = 0;
    LogOut("○ CreateSurface / ID [0x%08X] / ", (long)this);
}

LunaSurface::~LunaSurface()
{
    if (lpSurface != 0)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &lpSurface);
        lpSurface = 0;
    }
    LogOut("● ReleaseSurface / ID [0x%08X]\n", (long)this);
    List.Erase(this);
}

BOOL LunaSurface::Init( void )
{
    List.Init();
    return TRUE;
}

void LunaSurface::UnInit( void )
{
    LogOut("\n============================================== Destroy Surface\n");
    List.Release();
}

LSURFACE LunaSurface::Create( long w, long h, eFormat fmt )
{
    LSURFACE surface = new LunaSurface();

    surface->Width = w;
    surface->Height = h;

    switch (fmt)
    {
    default:
    case FORMAT_BACKBUFFER:
        surface->Format = Luna3D::GetBBFormat();
        break;
    case FORMAT_TEXTURE:
        surface->Format = Luna3D::GetTextureFormat();
        break;
    case FORMAT_ALPHATEXTURE:
        surface->Format = Luna3D::GetAlphaTextureFormat();
        break;
    }

    surface->lpSurface = 0;
    glGenTextures(1, &surface->lpSurface);
    if (surface->lpSurface == 0)
    {
        GLenum err = glGetError();
        LogOut("Error Can't CreateSurface: %i\n", err);
        return surface;
    }

    glBindTexture(GL_TEXTURE_2D, surface->lpSurface);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    if (featureNpotTexture)
    {
        define_texture_image(surface->Width, surface->Height, surface->Format, NULL);
    }
    else
    {
        define_texture_image(next_pow2(surface->Width), next_pow2(surface->Height), surface->Format, NULL);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    LogOut("Size [%dx%d] / [%s] / Complete\n", surface->Height, surface->Width, Luna3D::GetFormat(surface->Format));

    return surface;
}

void LunaSurface::CopyFromBackBuffer( long px, long py, long x1, long y1, long x2, long y2 )
{
    if (lpSurface == 0) return;

    if (!featureFramebufferObject) return;

    unsigned int BackBuffer, FrameBuffer = 0;
    glGenFramebuffers(1, &FrameBuffer);
    if (FrameBuffer == 0) return;

    BackBuffer = Luna3D::GetBackBuffer();

    long screen_width, screen_height;

    Luna::GetScreenSize(&screen_width, &screen_height);


    glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lpSurface, 0);

    glViewport(0, 0, Width, Height);


    long px2 = x2 + px - x1;
    long py2 = y2 + py - y1;

    if (px < 0)
    {
        x1 -= px;
        px = 0;
    }
    if (py < 0)
    {
        y1 -= py;
        py = 0;
    }
    if (px2 > Width)
    {
        x2 += Width - px2;
        px2 = Width;
    }
    if (py2 > Height)
    {
        y2 += Height - py2;
        py2 = Height;
    }


    GLfloat vertex[2*4];
    GLfloat vtex[2*4];

    vertex[0*2 + 0] = vertex[3*2 + 0] = ((2.0f * (float)px) / (float)Width) - 1.0f;
    vertex[0*2 + 1] = vertex[1*2 + 1] = ((-2.0f * (float)py) / (float)Height) + 1.0f;
    vertex[1*2 + 0] = vertex[2*2 + 0] = ((2.0f * (float)px2) / (float)Width) - 1.0f;
    vertex[2*2 + 1] = vertex[3*2 + 1] = ((-2.0f * (float)py2) / (float)Height) + 1.0f;

    float fScreenWidth, fScreenHeight;
    if (featureNpotTexture)
    {
        fScreenWidth = (float)screen_width;
        fScreenHeight = (float)screen_height;
    }
    else
    {
        fScreenWidth = (float)next_pow2(screen_width);
        fScreenHeight = (float)next_pow2(screen_height);
    }
    vtex[0*2 + 0] = vtex[3*2 + 0] = ((float)x1) / fScreenWidth;
    vtex[0*2 + 1] = vtex[1*2 + 1] = ((float)y1) / fScreenHeight;
    vtex[1*2 + 0] = vtex[2*2 + 0] = ((float)x2) / fScreenWidth;
    vtex[2*2 + 1] = vtex[3*2 + 1] = ((float)y2) / fScreenHeight;


    glDisable(GL_BLEND);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, BackBuffer);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, &(vertex[0]));
    glTexCoordPointer(2, GL_FLOAT, 0, &(vtex[0]));

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();


    Luna3D::ResumeScene();

    glDeleteFramebuffers(1, &FrameBuffer);
}

void LunaSurface::Blit( long px, long py, long x1, long y1, long x2, long y2 )
{
    long screen_width, screen_height;

    Luna::GetScreenSize(&screen_width, &screen_height);


    long px2 = x2 + px - x1;
    long py2 = y2 + py - y1;
    if (px < 0)
    {
        x1 -= px;
        px = 0;
        if (px2 <= 0) return;
    }
    if (py < 0)
    {
        y1 -= py;
        py = 0;
        if (py2 <= 0) return;
    }
    if (px2 > screen_width)
    {
        x2 += screen_width - px2;
        px2 = screen_width;
        if (px >= screen_width) return;
    }
    if (py2 > screen_height)
    {
        y2 += screen_height - py2;
        py2 = screen_height;
        if (py >= screen_height) return;
    }

    glDisable(GL_BLEND);

#ifdef USE_GLES
    if (featureDrawTexture)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, lpSurface);

        //GLint coords [] = {x1, y1, x2 - x1, y2 - y1};
        GLint coords [] = {x1, y2 - 1, x2 - x1, y1 - y2};
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, coords);
        //glDrawTexiOES(px, py, 0, px2 - px, py2 - py);
        glDrawTexiOES(px, screen_height - py2, 0, px2 - px, py2 - py);

        glDisable(GL_TEXTURE_2D);
    }
    else
#endif
    {
        GLfloat vertex[2*4];
        GLfloat vtex[2*4];

        vertex[0*2 + 0] = vertex[3*2 + 0] = px;
        vertex[0*2 + 1] = vertex[1*2 + 1] = py;
        vertex[1*2 + 0] = vertex[2*2 + 0] = px2;
        vertex[2*2 + 1] = vertex[3*2 + 1] = py2;

        float fWidth, fHeight;
        if (featureNpotTexture)
        {
            fWidth = (float)Width;
            fHeight = (float)Height;
        }
        else
        {
            fWidth = (float)next_pow2(Width);
            fHeight = (float)next_pow2(Height);
        }
        vtex[0*2 + 0] = vtex[3*2 + 0] = ((float)x1) / fWidth;
        vtex[0*2 + 1] = vtex[1*2 + 1] = ((float)y1) / fHeight;
        vtex[1*2 + 0] = vtex[2*2 + 0] = ((float)x2) / fWidth;
        vtex[2*2 + 1] = vtex[3*2 + 1] = ((float)y2) / fHeight;


        glBindTexture(GL_TEXTURE_2D, lpSurface);

        glEnable(GL_TEXTURE_2D);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(2, GL_FLOAT, 0, &(vertex[0]));
        glTexCoordPointer(2, GL_FLOAT, 0, &(vtex[0]));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        glDisable(GL_TEXTURE_2D);
    }
}

