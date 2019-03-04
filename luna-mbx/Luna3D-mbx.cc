#include "Luna-mbx.h"
#include "Luna-mbx-private.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <SDL.h>
#ifdef USE_GLES
#include <GLES/gl.h>
#include <GLES/glext.h>
#include "EGLPort/eglport.h"
#else
#if defined(PANDORA)
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#include <SDL_opengl.h>
#endif


static int IsFullColor = 0;
static int IsFullColorTexture = 0;
static unsigned int NowTexture, FrameTexture, FrameBuffer;
static int TextureFormat, AlphaTextureFormat, BBFormat;

static int RenderParam[sizeof_RenderState];
static int screen_width, screen_height;

static struct {
  int x, y, width, height;
} frontbuffer_viewport, backbuffer_viewport;

static GLfloat backbuffer_vertex[2*4] = {
    -1.0f, -1.0f,
     1.0f, -1.0f,
     1.0f,  1.0f,
    -1.0f,  1.0f,
};
static GLfloat backbuffer_vtex[2*4] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
};



int featureFramebufferObject; // GL_ARB_framebuffer_object / GL_OES_framebuffer_object
int featureNpotTexture; // GL_ARB_texture_non_power_of_two / GL_OES_texture_npot / GL_APPLE_texture_2D_limited_npot / GL_IMG_texture_npot

#ifdef USE_GLES
int featureDrawTexture; // GL_OES_draw_texture
int featureRequiredInternalFormat; // GL_OES_required_internalformat
int featureRgb8Rgba8; // GL_OES_rgb8_rgba8

PFNGLBINDFRAMEBUFFEROESPROC glBindFramebuffer;
PFNGLDELETEFRAMEBUFFERSOESPROC glDeleteFramebuffers;
PFNGLGENFRAMEBUFFERSOESPROC glGenFramebuffers;
PFNGLFRAMEBUFFERTEXTURE2DOESPROC glFramebufferTexture2D;

PFNGLGENERATEMIPMAPOESPROC glGenerateMipmap;

PFNGLDRAWTEXIOESPROC glDrawTexiOES;

#if !defined(GL_FRAMEBUFFER)
#define GL_FRAMEBUFFER GL_FRAMEBUFFER_OES
#define GL_COLOR_ATTACHMENT0 GL_COLOR_ATTACHMENT0_OES
#endif

#else
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;

PFNGLGENERATEMIPMAPPROC glGenerateMipmap;

PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLBUFFERSUBDATAPROC glBufferSubData;
#endif


void LogOut(const char *format, ...);


BOOL Luna3D::GetAdapterInfo( void )
{
    LogOut("  ■ Get Adapter Information\n");

    if (IsFullColorTexture)
    {
        TextureFormat = FMT_RGB32;
        AlphaTextureFormat = FMT_RGBA32;
    }
    else
    {
        TextureFormat = FMT_RGB16;
        AlphaTextureFormat = FMT_RGBA16;
    }

    if (IsFullColor)
    {
        BBFormat = FMT_RGB32;
    }
    else
    {
        BBFormat = FMT_RGB16;
    }

    return TRUE;
}

BOOL Luna3D::Create3DDevice( void )
{
    long width, height;

    Luna::GetScreenSize(&width, &height);

    SDL_Surface *Screen = SDL_GetVideoSurface();

    int surface_width = Screen->w;
    int surface_height = Screen->h;

#if !defined(USE_GLES)
    bool window_mode = Luna::GetWindowMode();

    if (IsFullColor)
    {
        SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
        SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    }
    else
    {
        SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
        SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 6 );
        SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    }
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    Uint32 flags = SDL_OPENGL;
    if (!window_mode)
    {
        flags |= SDL_FULLSCREEN | SDL_NOFRAME;
    }

    Screen = SDL_SetVideoMode (surface_width, surface_height, (IsFullColor)?32:16, flags);
    if ((Screen == NULL) && IsFullColor)
    {
        IsFullColor = 0;

        SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
        SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 6 );
        SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );

        Screen = SDL_SetVideoMode (surface_width, surface_height, 16, flags);
    }
    if (Screen == NULL)
    {
        SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );

        Screen = SDL_SetVideoMode (surface_width, surface_height, 16, flags);
    }
    if (Screen == NULL)
    {
        LogOut("  ERROR Failed SDL_SetVideoMode: %s\n", SDL_GetError ());
        return FALSE;
    }
#else
    if (EGL_Open(surface_width, surface_height) != 0)
    {
        LogOut("  ERROR Unable to open EGL context\n");
        return FALSE;
    }
#endif

    backbuffer_viewport.x = (surface_width - width) / 2;
    backbuffer_viewport.y = (surface_height - height) / 2;
    backbuffer_viewport.width = width;
    backbuffer_viewport.height = height;

    featureFramebufferObject = 0;
    featureNpotTexture = 0;
#ifdef USE_GLES
    featureDrawTexture = 0;
    featureRequiredInternalFormat = 0;
    featureRgb8Rgba8 = 0;
#endif

    const char *extensions = (const char *) glGetString(GL_EXTENSIONS);
    if (extensions != NULL)
    while (*extensions != 0)
    {
        int length;
        const char *delim = strchr(extensions, ' ');
        if (delim == NULL)
        {
            length = strlen(extensions);
        }
        else
        {
            length = (uintptr_t)delim - (uintptr_t)extensions;
        }

        if ((length == 31) && (0 == strncmp(extensions, "GL_ARB_texture_non_power_of_two", length)))
        {
            featureNpotTexture = 1;
        }
#ifdef USE_GLES
        else if ((length == 19) && (0 == strncmp(extensions, "GL_OES_texture_npot", length)))
        {
            featureNpotTexture = 1;
        }
        else if ((length == 19) && (0 == strncmp(extensions, "GL_IMG_texture_npot", length)))
        {
            featureNpotTexture = 1;
        }
        else if ((length == 32) && (0 == strncmp(extensions, "GL_APPLE_texture_2D_limited_npot", length)))
        {
            featureNpotTexture = 1;
        }
        else if ((length == 25) && (0 == strncmp(extensions, "GL_OES_framebuffer_object", length)))
        {
            featureFramebufferObject = 1;
        }
        else if ((length == 19) && (0 == strncmp(extensions, "GL_OES_draw_texture", length)))
        {
            featureDrawTexture = 1;
        }
        else if ((length == 30) && (0 == strncmp(extensions, "GL_OES_required_internalformat", length)))
        {
            featureRequiredInternalFormat = 1;
        }
        else if ((length == 17) && (0 == strncmp(extensions, "GL_OES_rgb8_rgba8", length)))
        {
            featureRgb8Rgba8 = 1;
        }
#else
        else if ((length == 25) && (0 == strncmp(extensions, "GL_ARB_framebuffer_object", length)))
        {
            featureFramebufferObject = 1;
        }
#endif

        if (delim == NULL)
        {
            break;
        }
        else
        {
            extensions += length + 1;
        }
    }

    if (featureFramebufferObject)
    {
#ifdef USE_GLES
        glBindFramebuffer = (PFNGLBINDFRAMEBUFFEROESPROC) eglGetProcAddress("glBindFramebufferOES");
        glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSOESPROC) eglGetProcAddress("glDeleteFramebuffersOES");
        glGenFramebuffers = (PFNGLGENFRAMEBUFFERSOESPROC) eglGetProcAddress("glGenFramebuffersOES");
        glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DOESPROC) eglGetProcAddress("glFramebufferTexture2DOES");
#else
        glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) SDL_GL_GetProcAddress("glBindFramebuffer");
        glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) SDL_GL_GetProcAddress("glDeleteFramebuffers");
        glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) SDL_GL_GetProcAddress("glGenFramebuffers");
        glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) SDL_GL_GetProcAddress("glFramebufferTexture2D");
#endif

        if ((glBindFramebuffer == NULL) ||
            (glDeleteFramebuffers == NULL) ||
            (glGenFramebuffers == NULL) ||
            (glGenFramebuffers == NULL)
           )
        {
            featureFramebufferObject = 0;
        }
    }

#ifdef USE_GLES
    //glGenerateMipmap = (PFNGLGENERATEMIPMAPOESPROC) eglGetProcAddress("glGenerateMipmapOES");
    glGenerateMipmap = NULL;

    if (featureDrawTexture)
    {
        glDrawTexiOES = (PFNGLDRAWTEXIOESPROC) eglGetProcAddress("glDrawTexiOES");

        if (glDrawTexiOES == NULL)
        {
            featureDrawTexture = false;
        }
    }

#ifdef PANDORA
    if (featureDrawTexture)
    {
        FILE *f;
        int v0, v1, v2, v3, v4;

        featureDrawTexture = false;

        f = fopen("/proc/pvr/version", "rt");
        if (f != NULL)
        {
            v0 = fscanf(f, "Version %i.%i.%i.%i", &v1, &v2, &v3, &v4);
            fclose(f);
        }
        else v0 = 0;

        if ((v0 == 4) && (v1 == 1) && (v2 == 4) && (v3 == 14) && ((v4 == 2514) || (v4 == 2616)))
        {
            featureDrawTexture = true;
        }
    }
#endif

#else
    glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) SDL_GL_GetProcAddress("glGenerateMipmap");

    glBindBuffer = (PFNGLBINDBUFFERPROC) SDL_GL_GetProcAddress("glBindBuffer");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) SDL_GL_GetProcAddress("glDeleteBuffers");
    glGenBuffers = (PFNGLGENBUFFERSPROC) SDL_GL_GetProcAddress("glGenBuffers");
    glBufferData = (PFNGLBUFFERDATAPROC) SDL_GL_GetProcAddress("glBufferData");
    glBufferSubData = (PFNGLBUFFERSUBDATAPROC) SDL_GL_GetProcAddress("glBufferSubData");
#endif


    if (featureFramebufferObject)
    {
        LogOut("○ GetBackBuffer\n");

        // texture for framebuffer
        glGenTextures(1, &FrameTexture);
        glBindTexture(GL_TEXTURE_2D, FrameTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        if (featureNpotTexture)
        {
            define_texture_image(width, height, BBFormat, NULL);
        }
        else
        {
            backbuffer_vtex[1*2 + 0] = backbuffer_vtex[2*2 + 0] = (float)width / (float)next_pow2(width);
            backbuffer_vtex[2*2 + 1] = backbuffer_vtex[3*2 + 1] = (float)height / (float)next_pow2(height);

            define_texture_image(next_pow2(width), next_pow2(height), BBFormat, NULL);
        }

        // framebuffer
        glGenFramebuffers(1, &FrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FrameTexture, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        LogOut("GL error: %i\n", err);
        return FALSE;
    }

    screen_width = width;
    screen_height = height;

    return TRUE;
}

BOOL Luna3D::InitD3DSetting( void )
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glTranslatef(-1.0f, 1.0f, 0.0f);
    glScalef(2.0f / screen_width, -2.0f / screen_height, -1.0f);

//    GLdouble fovy = 60.0;
//    GLdouble zNear = 10.0;
//    GLdouble zFar = 1000.0;
//    GLdouble aspect = zNear /*(double)screen_width/(double)screen_height*/;
//    //gluPerspective(fovy, aspect, zNear, zFar);
//    {
//        GLdouble radians = (fovy * M_PI) / 360.0; // fovy /2
//        GLdouble sine = std::sin(radians);
//        GLdouble cosine = std::cos(radians);
//        GLdouble cotangent = cosine / sine;
//
//        GLdouble top = zNear / cotangent;
//        GLdouble right = top * aspect;
//
//        glFrustum(-right, right, -top,  top, zNear, zFar);
//    }

    frontbuffer_viewport.x = 0;
    frontbuffer_viewport.y = 0;
    frontbuffer_viewport.width = screen_width;
    frontbuffer_viewport.height = screen_height;

    glViewport(frontbuffer_viewport.x, frontbuffer_viewport.y, frontbuffer_viewport.width, frontbuffer_viewport.height);

    NowTexture = 0;
    //glBindTexture(GL_TEXTURE_2D, 0);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_ALPHA_TEST);
    glAlphaFunc(GL_ALWAYS, 1.0f);
    glDisable(GL_DITHER);

    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
#if !defined(USE_GLES)
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
#endif
    glDisable(GL_COLOR_MATERIAL);
    glShadeModel(GL_FLAT);
#if !defined(USE_GLES)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
    glDisable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ZERO);


    //glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
    //glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
    //glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PRIMARY_COLOR);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        LogOut("GL error: %i\n", err);
        return FALSE;
    }

    RenderParam[STATE_CULLING] = CULL_NONE;
    RenderParam[STATE_ZBUFFER] = FALSE;
    RenderParam[STATE_ZWRITE] = FALSE;
    RenderParam[STATE_ZFUNC] = 0;
    RenderParam[STATE_ALPHATEST] = FALSE;
    RenderParam[STATE_ALPHAREF] = 255;
    RenderParam[STATE_ALPHAFUNC] = FUNC_ALWAYS;
    RenderParam[STATE_DITHER] = FALSE;

    RenderParam[STATE_LIGHTING] = FALSE;
    RenderParam[STATE_FOG] = FALSE;
    RenderParam[STATE_SPECULAR] = FALSE;
    RenderParam[STATE_VERTEXCOLOR] = FALSE;
    RenderParam[STATE_SHADING] = SHADE_FLAT;
    RenderParam[STATE_BLENDING] = BLEND_NONE;

    return TRUE;
}

BOOL Luna3D::Init( void )
{
    LogOut("\n============================================== Initialize DirectX Graphics\n");

    if (!GetAdapterInfo()) return FALSE;
    if (!Create3DDevice()) return FALSE;
    if (!InitD3DSetting()) return FALSE;

    return TRUE;
}

void Luna3D::UnInit( void )
{
    LogOut("\n============================================== Destroy DirectX Graphics\n");

    if (FrameBuffer != 0)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &FrameBuffer);
        FrameBuffer = 0;
    }

    if (FrameTexture != 0)
    {
        LogOut("● Release BackBuffer\n");

        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &FrameTexture);
        FrameTexture = 0;
    }

#ifdef USE_GLES
    EGL_Close();
#endif
}

void Luna3D::EnableFullColorMode( void )
{
    IsFullColor = 1;
}

void Luna3D::EnableFullColorTexture( void )
{
    IsFullColorTexture = 1;
}

const char *Luna3D::GetFormat( int fmt )
{
    switch (fmt)
    {
    case FMT_RGB16:
        return "RGB16";
    case FMT_RGBA16:
        return "RGBA16";
    case FMT_RGB32:
        return "RGB32";
    case FMT_RGBA32:
        return "RGBA32";
    default:
        return "Unknown";
    }
}

int Luna3D::GetTextureFormat( void )
{
    return TextureFormat;
}

int Luna3D::GetAlphaTextureFormat( void )
{
    return AlphaTextureFormat;
}

BOOL Luna3D::BeginScene( void )
{
    if (featureFramebufferObject)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);
    }
    glViewport(frontbuffer_viewport.x, frontbuffer_viewport.y, frontbuffer_viewport.width, frontbuffer_viewport.height);

    return TRUE;
}

void Luna3D::ResumeScene( void )
{
    if (featureFramebufferObject)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);
    }
    glViewport(frontbuffer_viewport.x, frontbuffer_viewport.y, frontbuffer_viewport.width, frontbuffer_viewport.height);

    if (NowTexture != 0) glBindTexture(GL_TEXTURE_2D, NowTexture);
}

void Luna3D::EndScene( void )
{
    if (featureFramebufferObject)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    glViewport(backbuffer_viewport.x, backbuffer_viewport.y, backbuffer_viewport.width, backbuffer_viewport.height);
    glDisable(GL_BLEND);
}

void Luna3D::GetViewport( RECT *pRect )
{
    pRect->left = frontbuffer_viewport.x;
    pRect->top = frontbuffer_viewport.y;
    pRect->right = frontbuffer_viewport.width;
    pRect->bottom = frontbuffer_viewport.height;
}

void Luna3D::SetBlending( eBlending blend )
{
    switch (blend)
    {
    default:
    case BLEND_NONE:
        glDisable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ZERO);
        break;
    case BLEND_NORMAL:
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        break;
    case BLEND_ADD:
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        break;
    case BLEND_SUB:
        break;
    case BLEND_MUL:
        glEnable(GL_BLEND);
        glBlendFunc(GL_ZERO, GL_SRC_COLOR);
        break;
    case BLEND_ONEONE:
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        break;
    case BLEND_BRIGHT:
        glEnable(GL_BLEND);
        glBlendFunc(GL_DST_COLOR, GL_ONE);
        break;
    case BLEND_REVERSE:
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
        break;
    }
}

void Luna3D::SetSmoothFilter( BOOL flag )
{
    glBindTexture(GL_TEXTURE_2D, 0);
    if (flag)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#if defined(PANDORA)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
#endif

        LunaTexture::SetOffset(0.5f);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#if defined(PANDORA)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
#endif

        LunaTexture::SetOffset(0.0f);
    }
}

void Luna3D::SetTexture( LTEXTURE tex )
{
    if (tex != NULL)
    {
        NowTexture = tex->GetTexture();
        glBindTexture(GL_TEXTURE_2D, NowTexture);
    }
    else
    {
        NowTexture = 0;
    }
}

void Luna3D::Clear( void )
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Luna3D::ColorFill( D3DCOLOR color )
{
    union {
        struct {
            unsigned char r, g, b, a;
        } c;
        D3DCOLOR v;
    } color2;

    color2.v = color;

    glClearColor(color2.c.r/255.0f, color2.c.g/255.0f, color2.c.b/255.0f, color2.c.a/255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Luna3D::Refresh( RECT *dest, RECT *src, HWND hWnd )
{
    GLenum err = glGetError();
    while (err != GL_NO_ERROR)
    {
        fprintf(stderr, "GL error: %i\n", err);
        err = glGetError();
    }

#ifdef USE_GLES
    if (featureFramebufferObject && featureDrawTexture)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, FrameTexture);

        GLint coords [] = {0, 0, screen_width, screen_height};
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, coords);
        glDrawTexiOES(backbuffer_viewport.x, backbuffer_viewport.y, 0, backbuffer_viewport.width, backbuffer_viewport.height);

        glDisable(GL_TEXTURE_2D);
    }
    else
#endif
    if (featureFramebufferObject)
    {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, FrameTexture);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(2, GL_FLOAT, 0, &(backbuffer_vertex[0]));
        glTexCoordPointer(2, GL_FLOAT, 0, &(backbuffer_vtex[0]));

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        glDisable(GL_TEXTURE_2D);

        glPopMatrix();
    }

#ifdef USE_GLES
    EGL_SwapBuffers();
#else
    SDL_GL_SwapBuffers();
#endif

    Luna::AddFPS(1);
}

int Luna3D::GetBBFormat( void )
{
    return BBFormat;
}

unsigned int Luna3D::GetBackBuffer( void )
{
    return FrameTexture;
}

void Luna3D::SetRenderState( eRenderState state, long param )
{
    if (RenderParam[state] == param) return;

    RenderParam[state] = param;

    switch (state)
    {
    case STATE_CULLING:
        switch (param)
        {
        default:
        case CULL_NONE:
            glDisable(GL_CULL_FACE);
            break;
        case CULL_RIGHT:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CW);
            break;
        case CULL_LEFT:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);
            break;
        }
        break;

    case STATE_ZBUFFER:
    case STATE_ZWRITE:
        if (RenderParam[STATE_ZBUFFER] || RenderParam[STATE_ZWRITE]) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
        break;

    case STATE_ZFUNC:
        break;

    case STATE_ALPHATEST:
        if (param) glEnable(GL_ALPHA_TEST); else glDisable(GL_ALPHA_TEST);
        break;

    case STATE_ALPHAREF:
    case STATE_ALPHAFUNC:
        glAlphaFunc((RenderParam[STATE_ALPHAFUNC] - 1) + GL_NEVER, RenderParam[STATE_ZBUFFER] / 255.0f);
        break;

    case STATE_DITHER:
        if (param) glEnable(GL_DITHER); else glDisable(GL_DITHER);
        break;

    case STATE_LIGHTING:
        if (param) glEnable(GL_LIGHTING); else glDisable(GL_LIGHTING);
        break;

    case STATE_FOG:
        if (param) glEnable(GL_FOG); else glDisable(GL_FOG);
        break;

    case STATE_SPECULAR:
#if !defined(USE_GLES)
        glColorMaterial(GL_FRONT_AND_BACK, param?GL_SPECULAR:GL_AMBIENT_AND_DIFFUSE);
#endif
        break;

    case STATE_VERTEXCOLOR:
        if (param) glEnable(GL_COLOR_MATERIAL); else glDisable(GL_COLOR_MATERIAL);
        break;

    case STATE_SHADING:
        switch (param)
        {
        case SHADE_POINT:
#if !defined(USE_GLES)
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
#endif
        case SHADE_WIRE:
#if !defined(USE_GLES)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#else
            LogOut("  ERROR Shade mode not supported: %i\n", (int)param);
            Luna::Exit();
#endif
            break;
        default:
        case SHADE_FLAT:
            glShadeModel(GL_FLAT);
#if !defined(USE_GLES)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
            break;
        case SHADE_GOURAUD:
            glShadeModel(GL_SMOOTH);
#if !defined(USE_GLES)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
        }
        break;

    case STATE_BLENDING:
        SetBlending((eBlending) param);
        break;

    default:
        break;
    }
}

