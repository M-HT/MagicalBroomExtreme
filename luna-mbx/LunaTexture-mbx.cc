#include "Luna-mbx.h"
#include "Luna-mbx-private.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdint.h>
#include <SDL.h>
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


//==========================================================================
// STRUCT
//==========================================================================

// LAGファイルヘッダ
typedef struct LAGFILEHEADER
{
	char chunk[4];
	uint32_t reserved;
}
LAGFILEHEADER;

// LAGデータヘッダ
typedef struct LAGDATAHEADER
{
	char name[16];
	int32_t width;
	int32_t height;
	uint32_t format;
	uint32_t reserved;
}
LAGDATAHEADER;


static CList List;
static float Offset;

#ifdef USE_GLES
extern PFNGLGENERATEMIPMAPOESPROC glGenerateMipmap;
#else
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
#endif

void LogOut(const char *format, ...);


LunaTexture::LunaTexture( void )
{
    List.InsertTop(this);
    lpTexture = 0;
    lpBackUp = NULL;
    Format = 0;
    //Width = 0;
    //Height = 0;
    ImageWidth = 0;
    ImageHeight = 0;
    AspectWidth = 0.0f;
    AspectHeight = 0.0f;
    bActive = FALSE;
}

LunaTexture::~LunaTexture()
{
    if (lpTexture != 0)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &lpTexture);
        lpTexture = 0;
    }

    if (lpBackUp != NULL)
    {
        SDL_FreeSurface((SDL_Surface *)lpBackUp);
        lpBackUp = NULL;
    }

    LogOut("● ReleaseTexture / ID [0x%08X]\n", (long)this);
    List.Erase(this);
}

BOOL LunaTexture::Init( void )
{
    List.Init();
    return TRUE;
}

void LunaTexture::UnInit( void )
{
    LogOut("\n============================================== Destroy Texture\n");

    List.Release();
}

LTEXTURE LunaTexture::LoadLAG( const char *pack, const char *data, BOOL alpha )
{
    const char *errMsg;
    LTEXTURE texture = new LunaTexture();

    LogOut("○ CreateTexture / ID [0x%08X] / ", texture);
    LogOut("\"%s\" ( \"%s\" ) / ", data, pack);

    LGRAPHICS loader = LunaGraphicLoader::Create();

    const char *err = loader->LoadLAG(pack, data);
    if (err != NULL)
    {
        LogOut("Error %s\n", err);
        goto load_end;
    }

    long width, height;
    loader->GetGraphicSize(&width, &height);

    if (alpha)
    {
        texture->Format = Luna3D::GetAlphaTextureFormat();
    }
    else
    {
        texture->Format = Luna3D::GetTextureFormat();
    }
    texture->ImageWidth = width;
    texture->ImageHeight = height;
    //texture->Width = width;
    //texture->Height = height;
    texture->AspectWidth = 1.0 / (double)texture->ImageWidth;
    texture->AspectHeight = 1.0 / (double)texture->ImageHeight;

    SDL_Surface *surface;
    Uint32 rmask, gmask, bmask, amask;
    int depth;

    switch (texture->Format)
    {
    default:
    case FMT_RGB16:
        rmask = 0xf800;
        gmask = 0x07e0;
        bmask = 0x001f;
        amask = 0;
        depth = 16;
        break;
    case FMT_RGBA16:
        rmask = 0xf000;
        gmask = 0x0f00;
        bmask = 0x00f0;
        amask = 0x000f;
        depth = 16;
        break;
    case FMT_RGB32:
    case FMT_RGBA32:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = (texture->Format == FMT_RGBA32)?0x000000ff:0;
#else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = (texture->Format == FMT_RGBA32)?0xff000000:0;
#endif
        depth = (texture->Format == FMT_RGBA32)?32:24;
        break;
    }

    texture->lpBackUp = surface = SDL_CreateRGBSurface(SDL_SWSURFACE, texture->ImageWidth, texture->ImageHeight, depth, rmask, gmask, bmask, amask);
    if (surface == NULL)
    {
        LogOut("Error Can't Create Surface\n");
        goto load_end;
    }

    if (SDL_LockSurface(surface))
    {
        LogOut("Can't lock surface\n");
        goto load_end;
    }

    if (!loader->CopyToSurface(surface->pixels, surface->pitch, texture->Format))
    {
        SDL_UnlockSurface(surface);
        LogOut("Error サーフェイスへのデータ展開に失敗\n");
        goto load_end;
    }

    SDL_UnlockSurface(surface);

    texture->lpTexture = 0;
    glGenTextures(1, &texture->lpTexture);
    if (texture->lpTexture == 0)
    {
        LogOut("Error テクスチャ生成に失敗 [%i]\n", glGetError());
        goto load_end;
    }

    errMsg = texture->Refresh();
    if (errMsg != NULL)
    {
        LogOut("%s\n", errMsg);
        goto load_end;
    }

    texture->bActive = TRUE;

    //LogOut("[%s] / [%dx%d] / Complete\n", Luna3D::GetFormat(texture->Format), texture->Width, texture->Height);
    LogOut("[%s] / [%dx%d] / Complete\n", Luna3D::GetFormat(texture->Format), texture->ImageWidth, texture->ImageHeight);

load_end:
    if (loader != NULL) loader->Release();

    return texture;
}

void LunaTexture::SetOffset( float offset )
{
    Offset = offset;
}

float LunaTexture::GetOffset( void )
{
    return Offset;
}

const char *LunaTexture::Refresh( void )
{
    static char ErrMsg[256];
    const char *retval;

    retval = ErrMsg;

    if (lpBackUp == NULL)
    {
        strcpy(ErrMsg, "サーフェイスが不正です");
        return retval;
    }

    //if ((Width == ImageWidth) && (Height == ImageHeight))
    {
        glBindTexture(GL_TEXTURE_2D, lpTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#if defined(PANDORA)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        if (glGenerateMipmap == NULL) glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
#endif

        SDL_LockSurface((SDL_Surface *)lpBackUp);

        define_texture_image(ImageWidth, ImageHeight, Format, ((SDL_Surface *)lpBackUp)->pixels);

#if !defined(PANDORA)
        if (glGenerateMipmap != NULL)
        {
            glEnable(GL_TEXTURE_2D);
            glGenerateMipmap(GL_TEXTURE_2D);
            glDisable(GL_TEXTURE_2D);
        }
#endif

        SDL_UnlockSurface((SDL_Surface *)lpBackUp);

        glBindTexture(GL_TEXTURE_2D, 0);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            sprintf(ErrMsg, "GL error: %i", err);
            return retval;
        }

        return NULL;
    }
}

float LunaTexture::GetAspectWidth( void )
{
    return AspectWidth;
}

float LunaTexture::GetAspectHeight( void )
{
    return AspectHeight;
}

unsigned int LunaTexture::GetTexture( void )
{
    return lpTexture;
}

BOOL LunaTexture::BlitToTarget( RECT *dest, RECT *src, D3DCOLOR color, eBlending blend, float z )
{
    GLuint vcolor[4];
    GLfloat vertex[4*4];
    GLfloat vtex[2*4];

    if (!bActive) return FALSE;

    vcolor[0] = vcolor[1] = vcolor[2] = vcolor[3] = color;

    vertex[0*4 + 0] = vertex[3*4 + 0] = dest->left;
    vertex[0*4 + 1] = vertex[1*4 + 1] = dest->top;
    vertex[1*4 + 0] = vertex[2*4 + 0] = dest->right;
    vertex[2*4 + 1] = vertex[3*4 + 1] = dest->bottom;

    vertex[0*4 + 2] = vertex[1*4 + 2] = vertex[2*4 + 2] = vertex[3*4 + 2] = z;
    vertex[0*4 + 3] = vertex[1*4 + 3] = vertex[2*4 + 3] = vertex[3*4 + 3] = 1.0f;

    vtex[0*2 + 0] = vtex[3*2 + 0] = ((float)src->left + 0.25f /*+ Offset*/) * AspectWidth;
    vtex[0*2 + 1] = vtex[1*2 + 1] = ((float)src->top + 0.25f /*+ Offset*/) * AspectHeight;
    vtex[1*2 + 0] = vtex[2*2 + 0] = ((float)src->right - 1 /*- Offset*/) * AspectWidth;
    vtex[2*2 + 1] = vtex[3*2 + 1] = ((float)src->bottom - 1 /*- Offset*/) * AspectHeight;


    Luna3D::SetRenderState(STATE_ALPHATEST, FALSE);
    Luna3D::SetRenderState(STATE_LIGHTING, FALSE);
    Luna3D::SetRenderState(STATE_ZBUFFER, FALSE);
    Luna3D::SetRenderState(STATE_ZWRITE, FALSE);
    Luna3D::SetRenderState(STATE_FOG, FALSE);
    Luna3D::SetRenderState(STATE_CULLING, CULL_NONE);
    Luna3D::SetRenderState(STATE_SHADING, SHADE_GOURAUD);

    if ( blend == BLEND_SUB )
    {
        //long width = 0;
        //long height = 0;
        //Luna::GetScreenSize(&width, &height);

        LunaEffect::Begin(BLEND_SUB);

        Luna3D::SetTexture(this);

        glEnable(GL_TEXTURE_2D);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        Luna3D::SetBlending(BLEND_ADD);

        glVertexPointer(4, GL_FLOAT, 0, &(vertex[0]));
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, &(vcolor[0]));
        glTexCoordPointer(2, GL_FLOAT, 0, &(vtex[0]));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        glDisable(GL_TEXTURE_2D);

        LunaEffect::Begin(BLEND_SUB);
    }
    else
    {
        Luna3D::SetTexture(this);

        glEnable(GL_TEXTURE_2D);


        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        Luna3D::SetBlending(blend);

        glVertexPointer(4, GL_FLOAT, 0, &(vertex[0]));
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, &(vcolor[0]));
        glTexCoordPointer(2, GL_FLOAT, 0, &(vtex[0]));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        glDisable(GL_TEXTURE_2D);
    }

    Luna::AddPolygon(2);

    return TRUE;
}

