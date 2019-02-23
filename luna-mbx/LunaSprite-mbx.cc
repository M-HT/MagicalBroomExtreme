#include "Luna-mbx.h"
#include "Luna-mbx-private.h"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#ifdef USE_GLES
#include <GLES/gl.h>
#else
#include <SDL_opengl.h>
#endif


static CList List;
static unsigned int lpIB;

#define SPRITE_MAX 10000

#if !defined(USE_GLES)
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
#endif


void LogOut(const char *format, ...);


BOOL LunaSprite::AddPolygon( LPTLVERTEX pVx, BOOL clip )
{
    if (clip)
    {
        if (!ClipVertex(pVx)) return TRUE;
    }

    lpVertex[0][2 * NumPolygon] = pVx[0];
    lpVertex[0][2 * NumPolygon + 1] = pVx[1];
    lpVertex[0][2 * NumPolygon + 2] = pVx[2];
    lpVertex[0][2 * NumPolygon + 3] = pVx[3];

    NumPolygon += 2;

    return TRUE;
}

BOOL LunaSprite::ClipVertex( LPTLVERTEX pVx )
{
    RECT viewport;

    Luna3D::GetViewport(&viewport);

    int index, count = 0;
    for (index = 0; index < 4; index++)
    {
        if (pVx[index].x < (float)viewport.left) count++;
    }
    if (count == 4) return FALSE;

    count = 0;
    for (index = 0; index < 4; index++)
    {
        if (pVx[index].x > (float)viewport.right) count++;
    }
    if (count == 4) return FALSE;

    count = 0;
    for (index = 0; index < 4; index++)
    {
        if (pVx[index].y < (float)viewport.top) count++;
    }
    if (count == 4) return FALSE;

    count = 0;
    for (index = 0; index < 4; index++)
    {
        if (pVx[index].y > (float)viewport.bottom) count++;
    }
    if (count == 4) return FALSE;

    return TRUE;
}

void LunaSprite::RefreshVertex( void )
{
    if (bLaser)
    {
        for (long index = 0; index < NumPolygon; index++)
        {
            glBindBuffer(GL_ARRAY_BUFFER, lpVB[index]);
            glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * Division * sizeof(TLVERTEX), lpVertex[index]);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, lpVB[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * NumPolygon * sizeof(TLVERTEX), lpVertex[0]);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

LunaSprite::LunaSprite( long num, LTEXTURE ptex, eBlending blend )
{
    LogOut("○ CreateSprite  / ID [0x%08X] / ", this);

    List.InsertTop(this);

    for (int vindex = 0; vindex < LASER_MAX; vindex++)
    {
        lpVertex[vindex] = NULL;
        lpVB[vindex] = 0;
    }

    Blend = BLEND_NONE;
    lpTex = NULL;
    NumPolygon = 0;
    MaxPolygon = 0;
    bLaser = FALSE;
    Division = 0;

    if (num >= SPRITE_MAX)
    {
        LogOut(" ERROR The sprite more than %d cannot be created\n", SPRITE_MAX);
        return;
    }

    if (ptex == NULL)
    {
        LogOut(" ERROR Invalid Texture\n");
        return;
    }

    lpVertex[0] = (LPTLVERTEX) malloc(4 * num * sizeof(TLVERTEX)); // allocate memory for vertices
    glGenBuffers(1, &(lpVB[0])); // create vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, lpVB[0]);
    glBufferData(GL_ARRAY_BUFFER, 4 * num * sizeof(TLVERTEX), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    switch(blend)
    {
        default:
        case BLEND_NONE:
            LogOut("None Blend / ");
            break;
        case BLEND_NORMAL:
            LogOut("Normal Blend / ");
            break;
        case BLEND_ADD:
            LogOut("Add Blend / ");
            break;
        case BLEND_SUB:
            LogOut("Sub Blend / ");
            break;
        case BLEND_MUL:
            LogOut("Mul Blend / ");
            break;
        case BLEND_ONEONE:
            LogOut("OneOne Blend / ");
            break;
        case BLEND_BRIGHT:
            LogOut("Bright Blend / ");
            break;
        case BLEND_REVERSE:
            LogOut("Revease Blend / ");
            break;
    }

    Blend = blend;
    NumPolygon = 0;
    MaxPolygon = 2 * num;
    lpTex = ptex;
    ptex->AddRef();

    LogOut("MaxPolygon %d / Complete\n", MaxPolygon);
}

LunaSprite::LunaSprite( long div, RECT *uv, LTEXTURE ptex, eBlending blend )
{
    LogOut("○ CreateLaserSprite  / ID [0x%08X] / ", this);

    List.InsertTop(this);

    for (int vindex = 0; vindex < LASER_MAX; vindex++)
    {
        lpVertex[vindex] = NULL;
        lpVB[vindex] = 0;
    }

    Blend = BLEND_NONE;
    lpTex = NULL;
    NumPolygon = 0;
    MaxPolygon = 0;
    bLaser = FALSE;
    Division = 0;
    RectUV = *uv;
    bLaser = TRUE;

    if (ptex == NULL)
    {
        LogOut(" ERROR Invalid Texture\n");
        return;
    }

    switch(blend)
    {
        default:
        case BLEND_NONE:
            LogOut("None Blend / ");
            break;
        case BLEND_NORMAL:
            LogOut("Normal Blend / ");
            break;
        case BLEND_ADD:
            LogOut("Add Blend / ");
            break;
        case BLEND_SUB:
            LogOut("Sub Blend / ");
            break;
        case BLEND_MUL:
            LogOut("Mul Blend / ");
            break;
        case BLEND_ONEONE:
            LogOut("OneOne Blend / ");
            break;
        case BLEND_BRIGHT:
            LogOut("Bright Blend / ");
            break;
        case BLEND_REVERSE:
            LogOut("Revease Blend / ");
            break;
    }

    lpTex = ptex;
    ptex->AddRef();

    Division = div;
    Blend = blend;
    MaxPolygon = 2 * div - 2;

    LogOut(" Division %d / MaxPolygon % d / ", Division, MaxPolygon << 8);

    //float offset = LunaTexture::GetOffset();
    float left = lpTex->GetAspectWidth() * ((float)uv->left + 0.25f /*+ offset*/);
    float top = lpTex->GetAspectHeight() * ((float)uv->top + 0.25f /*+ offset*/);
    float right = lpTex->GetAspectWidth() * ((float)uv->right - 1 /*- offset*/);
    float bottom = lpTex->GetAspectHeight() * ((float)uv->bottom - 1 /*- offset*/);

    for (int vindex = 0; vindex < LASER_MAX; vindex++)
    {
        lpVertex[vindex] = (LPTLVERTEX) malloc(2 * Division * sizeof(TLVERTEX)); // allocate memory for vertices
        glGenBuffers(1, &(lpVB[vindex])); // create vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, lpVB[vindex]);
        glBufferData(GL_ARRAY_BUFFER, 2 * Division * sizeof(TLVERTEX), NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        if ((lpVertex[vindex] != NULL) && (Division > 0))
        {
            for (long dindex = 0; dindex < Division; dindex++)
            {
                float y = top + ( (dindex * (bottom - top)) / (Division - 1) );

                lpVertex[vindex][2 * dindex].u = left;
                lpVertex[vindex][2 * dindex].v = y;
                lpVertex[vindex][2 * dindex].z = 0.0f;
                lpVertex[vindex][2 * dindex].w = 1.0f;

                lpVertex[vindex][2 * dindex + 1].u = right;
                lpVertex[vindex][2 * dindex + 1].v = y;
                lpVertex[vindex][2 * dindex + 1].z = 0.0f;
                lpVertex[vindex][2 * dindex + 1].w = 1.0f;
            }
        }
    }

    LogOut(" Complete\n");
}

LunaSprite::~LunaSprite()
{
    LogOut("● ReleaseSprite  / ID [0x%08X]\n", (long)this);
    if (lpTex != NULL)
    {
        lpTex->Release();
        lpTex = NULL;
    }

    for (int vindex = 0; vindex < LASER_MAX; vindex++)
    {
        if (lpVB[vindex] != 0)
        {
            glDeleteBuffers(1, &(lpVB[vindex])); // delete vertex buffer
            lpVB[vindex] = 0;
        }

        // release memory for vertices
        if (lpVertex[vindex] != NULL)
        {
            free(lpVertex[vindex]);
            lpVertex[vindex] = NULL;
        }
    }

    List.Erase(this);
}

LSPRITE LunaSprite::Create( long num, LTEXTURE ptex, eBlending blend )
{
    return new LunaSprite(num, ptex, blend);
}

LSPRITE LunaSprite::CreateLaser( long div, RECT *uv, LTEXTURE ptex, eBlending blend )
{
    return new LunaSprite(div, uv, ptex, blend);
}

BOOL LunaSprite::Init( void )
{
    List.Init();

    GLushort *IndexArray = (GLushort *) malloc(3 * 2 * SPRITE_MAX * sizeof(GLushort));
    for (int index = 0; index < SPRITE_MAX; index++)
    {
        IndexArray[6 * index + 0] = 4 * index;
        IndexArray[6 * index + 1] = 4 * index + 1;
        IndexArray[6 * index + 2] = 4 * index + 2;
        IndexArray[6 * index + 3] = 4 * index;
        IndexArray[6 * index + 4] = 4 * index + 2;
        IndexArray[6 * index + 5] = 4 * index + 3;
    }

    glGenBuffers(1, &lpIB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lpIB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 2 * SPRITE_MAX * sizeof(GLushort), IndexArray, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    free(IndexArray);

    return TRUE;
}

void LunaSprite::UnInit( void )
{
    LogOut("\n============================================== Destroy Sprite\n");

    if (lpIB != 0)
    {
        glDeleteBuffers(1, &lpIB);
        lpIB = 0;
    }

    List.Release();
}

void LunaSprite::DrawLaser( float *x, float *y, float *w, long *rot, D3DCOLOR *c, RECT *uv, float z )
{
    if (!bLaser) return;
    if (NumPolygon == LASER_MAX) return;

    TLVERTEX *vertex = lpVertex[NumPolygon];
    D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255);

    //float offset = LunaTexture::GetOffset();
    float u1 = lpTex->GetAspectWidth() * ((float)uv->left + 0.25f /*+ offset*/);
    float v1 = lpTex->GetAspectHeight() * ((float)uv->top + 0.25f /*+ offset*/);
    float u2 = lpTex->GetAspectWidth() * ((float)uv->right - 1 /*- offset*/);
    float v2 = lpTex->GetAspectHeight() * ((float)uv->bottom - 1 /*- offset*/);

    float v = v1;
    float add_v = (v2 - v1) / (Division - 1);

    for (long dindex = 0; dindex < Division; dindex++)
    {
        if (c != NULL) color = c[dindex];

        float px = 0.0f;
        float py = w[dindex];
        float angle = (rot[dindex] & 4095) * (float)(M_PI / 2048);
        float sin = LunaMath::SinF(angle);
        float cos = LunaMath::CosF(angle);
        float tx = cos * px - sin * py;
        float ty = cos * py + sin * px;

        vertex[2 * dindex].x = x[dindex] - tx - 0.5f;
        vertex[2 * dindex].y = y[dindex] - ty - 0.5f;
        vertex[2 * dindex].z = z;
        vertex[2 * dindex].w = 1.0f;
        vertex[2 * dindex].color = color;
        vertex[2 * dindex].u = u1;
        vertex[2 * dindex].v = v;

        vertex[2 * dindex + 1].x = x[dindex] + tx - 0.5f;
        vertex[2 * dindex + 1].y = y[dindex] + ty - 0.5f;
        vertex[2 * dindex + 1].z = z;
        vertex[2 * dindex + 1].w = 1.0f;
        vertex[2 * dindex + 1].color = color;
        vertex[2 * dindex + 1].u = u2;
        vertex[2 * dindex + 1].v = v;

        v += add_v;
    }

    NumPolygon++;
}

BOOL LunaSprite::Draw( RECT *dest, D3DCOLOR color, RECT *src, BOOL clip, float z )
{
    if (MaxPolygon == NumPolygon) return FALSE;

    //float offset = LunaTexture::GetOffset();
    float u1 = lpTex->GetAspectWidth() * ((float)src->left + 0.25f /*+ offset*/);
    float v1 = lpTex->GetAspectHeight() * ((float)src->top + 0.25f /*+ offset*/);
    float u2 = lpTex->GetAspectWidth() * ((float)src->right - 1 /*- offset*/);
    float v2 = lpTex->GetAspectHeight() * ((float)src->bottom - 1 /*- offset*/);

    TLVERTEX vertex[4];

    vertex[0].x = dest->left;
    vertex[0].y = dest->top;
    vertex[0].z = z;
    vertex[0].w = 1.0f;
    vertex[0].color = color;
    vertex[0].u = u1;
    vertex[0].v = v1;

    vertex[1].x = dest->right;
    vertex[1].y = dest->top;
    vertex[1].z = z;
    vertex[1].w = 1.0f;
    vertex[1].color = color;
    vertex[1].u = u2;
    vertex[1].v = v1;

    vertex[2].x = dest->right;
    vertex[2].y = dest->bottom;
    vertex[2].z = z;
    vertex[2].w = 1.0f;
    vertex[2].color = color;
    vertex[2].u = u2;
    vertex[2].v = v2;

    vertex[3].x = dest->left;
    vertex[3].y = dest->bottom;
    vertex[3].z = z;
    vertex[3].w = 1.0f;
    vertex[3].color = color;
    vertex[3].u = u1;
    vertex[3].v = v2;

    AddPolygon(vertex, clip);

    return TRUE;
}

BOOL LunaSprite::DrawRotate( RECT *dest, D3DCOLOR color, float angle, RECT *src, BOOL clip, eRotateAxis axis, float z )
{
    if (MaxPolygon == NumPolygon) return FALSE;

    //float offset = LunaTexture::GetOffset();
    float u1 = lpTex->GetAspectWidth() * ((float)src->left + 0.25f /*+ offset*/);
    float v1 = lpTex->GetAspectHeight() * ((float)src->top + 0.25f /*+ offset*/);
    float u2 = lpTex->GetAspectWidth() * ((float)src->right - 1 /*- offset*/);
    float v2 = lpTex->GetAspectHeight() * ((float)src->bottom - 1 /*- offset*/);

    float cx, cy;

    switch (axis)
    {
    default:
    case ROTATE_CENTER:
        cx = (dest->left + dest->right) / 2;
        cy = (dest->top + dest->bottom) / 2;
        break;
    case ROTATE_LT:
        cx = dest->left;
        cy = dest->top;
        break;
    case ROTATE_LB:
        cx = dest->left;
        cy = dest->bottom;
        break;
    case ROTATE_RT:
        cx = dest->right;
        cy = dest->top;
        break;
    case ROTATE_RB:
        cx = dest->right;
        cy = dest->bottom;
        break;
    }

    float x1 = (float)dest->left - cx;
    float y1 = (float)dest->top - cy;
    float x2 = (float)dest->right - cx;
    float y2 = (float)dest->top - cy;
    float x3 = (float)dest->right - cx;
    float y3 = (float)dest->bottom - cy;
    float x4 = (float)dest->left - cx;
    float y4 = (float)dest->bottom - cy;
    float s = LunaMath::SinF(angle);
    float c = LunaMath::CosF(angle);

    float tmpf;

    tmpf = c * y1 + s * x1;
    x1 = c * x1 - s * y1;
    y1 =  tmpf;

    tmpf = c * y2 + s * x2;
    x2 = c * x2 - s * y2;
    y2 =  tmpf;

    tmpf = c * y3 + s * x3;
    x3 = c * x3 - s * y3;
    y3 =  tmpf;

    tmpf = c * y4 + s * x4;
    x4 = c * x4 - s * y4;
    y4 =  tmpf;

    TLVERTEX vertex[4];

    vertex[0].x = x1 + cx;
    vertex[0].y = y1 + cy;
    vertex[0].z = z;
    vertex[0].w = 1.0f;
    vertex[0].color = color;
    vertex[0].u = u1;
    vertex[0].v = v1;

    vertex[1].x = x2 + cx;
    vertex[1].y = y2 + cy;
    vertex[1].z = z;
    vertex[1].w = 1.0f;
    vertex[1].color = color;
    vertex[1].u = u2;
    vertex[1].v = v1;

    vertex[2].x = x3 + cx;
    vertex[2].y = y3 + cy;
    vertex[2].z = z;
    vertex[2].w = 1.0f;
    vertex[2].color = color;
    vertex[2].u = u2;
    vertex[2].v = v2;

    vertex[3].x = x4 + cx;
    vertex[3].y = y4 + cy;
    vertex[3].z = z;
    vertex[3].w = 1.0f;
    vertex[3].color = color;
    vertex[3].u = u1;
    vertex[3].v = v2;

    AddPolygon(vertex, clip);

    return TRUE;
}

BOOL LunaSprite::DrawRotate( RECT *dest, D3DCOLOR color, float yaw, float pitch, float roll, RECT *src, BOOL clip )
{
    if (MaxPolygon == NumPolygon) return FALSE;

    //float offset = LunaTexture::GetOffset();
    float u1 = lpTex->GetAspectWidth() * ((float)src->left + 0.25f /*+ offset*/);
    float v1 = lpTex->GetAspectHeight() * ((float)src->top + 0.25f /*+ offset*/);
    float u2 = lpTex->GetAspectWidth() * ((float)src->right - 1 /*- offset*/);
    float v2 = lpTex->GetAspectHeight() * ((float)src->bottom - 1 /*- offset*/);

    float w = (dest->right - dest->left) / 2;
    float h = (dest->bottom - dest->top) / 2;
    float d = ((w <= h)?h:w) * 4.0f;

    float cx = dest->left + w;
    float cy = dest->top + h;

    D3DXVECTOR3 vec[4];

    vec[0].x = -w;
    vec[0].y = -h;
    vec[0].z = 0.0f;

    vec[1].x = w;
    vec[1].y = -h;
    vec[1].z = 0.0f;

    vec[2].x = w;
    vec[2].y = h;
    vec[2].z = 0.0f;

    vec[3].x = -w;
    vec[3].y = h;
    vec[3].z = 0.0f;

    D3DMATRIX mat;

    D3DXMatrixRotationYawPitchRoll(&mat, yaw, pitch, roll);

    TLVERTEX vx[4];

    for (int index = 0; index < 4; index++)
    {
        D3DXVECTOR3 temp;
        D3DXVec3TransformCoord(&temp, &(vec[index]), &mat);

        vec[index].x = temp.x;
        vec[index].y = temp.y;
        vec[index].z = temp.z + d;

        vx[index].x = (d * vec[index].x / vec[index].z) + cx - 0.5f;
        vx[index].y = (d * vec[index].y / vec[index].z) + cy - 0.5f;
        vx[index].z = vec[index].z / d;
        vx[index].w = 1.0f / vx[index].z;
        vx[index].color = color;
    }

    vx[0].u = u1;
    vx[0].v = v1;
    vx[1].u = u2;
    vx[1].v = v1;
    vx[2].u = u2;
    vx[2].v = v2;
    vx[3].u = u1;
    vx[3].v = v2;

    AddPolygon(vx, clip);

    return TRUE;
}

void LunaSprite::Rendering( BOOL state )
{
    if (NumPolygon > 0)
    {
        RefreshVertex();
        if (state)
        {
            Luna3D::SetRenderState(STATE_ALPHATEST, FALSE);
            Luna3D::SetRenderState(STATE_LIGHTING, FALSE);
            Luna3D::SetRenderState(STATE_ZBUFFER, FALSE);
            Luna3D::SetRenderState(STATE_ZWRITE, FALSE);
            Luna3D::SetRenderState(STATE_FOG, FALSE);
            Luna3D::SetRenderState(STATE_CULLING, CULL_NONE);
            Luna3D::SetRenderState(STATE_SHADING, SHADE_GOURAUD);
        }

        Luna3D::SetTexture(lpTex);

        if ( Blend == BLEND_SUB )
        {
            LunaEffect::Begin(BLEND_SUB);
            Luna3D::SetBlending(BLEND_ADD);
        }
        else
        {
            Luna3D::SetBlending(Blend);
        }


        glEnable(GL_TEXTURE_2D);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        if (bLaser)
        {
            for (long index = 0; index < NumPolygon; index++)
            {
                glBindBuffer(GL_ARRAY_BUFFER, lpVB[index]);

                glVertexPointer(4, GL_FLOAT, sizeof(TLVERTEX), (const void *)offsetof(TLVERTEX, x));
                glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(TLVERTEX), (const void *)offsetof(TLVERTEX, color));
                glTexCoordPointer(2, GL_FLOAT, sizeof(TLVERTEX), (const void *)offsetof(TLVERTEX, u));
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * Division);

                Luna::AddPolygon(MaxPolygon);
            }

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        else
        {
            glBindBuffer(GL_ARRAY_BUFFER, lpVB[0]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lpIB);

            glVertexPointer(4, GL_FLOAT, sizeof(TLVERTEX), (const void *)offsetof(TLVERTEX, x));
            glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(TLVERTEX), (const void *)offsetof(TLVERTEX, color));
            glTexCoordPointer(2, GL_FLOAT, sizeof(TLVERTEX), (const void *)offsetof(TLVERTEX, u));
            glDrawElements(GL_TRIANGLES, 3 * NumPolygon, GL_UNSIGNED_SHORT, NULL);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            Luna::AddPolygon(NumPolygon);
        }

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        glDisable(GL_TEXTURE_2D);


        if ( Blend == BLEND_SUB )
        {
            LunaEffect::End();
        }
    }

    NumPolygon = 0;

}

void LunaSprite::InitDraw( void )
{
    NumPolygon = 0;
}


