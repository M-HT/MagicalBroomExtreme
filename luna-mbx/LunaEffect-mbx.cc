#include "Luna-mbx.h"
#include "Luna-mbx-private.h"

#include <cstdio>
#include <cstdlib>
#ifdef USE_GLES
#include <GLES/gl.h>
#else
#include <SDL_opengl.h>
#endif


static RECT ScreenRect;
static eBlending NowBlend;


void LogOut(const char *format, ...);


BOOL LunaEffect::Init( void )
{
    long width, height;

    Luna::GetScreenSize(&width, &height);

    ScreenRect.top = 0;
    ScreenRect.left = 0;
    ScreenRect.right = width;
    ScreenRect.bottom = height;

    return TRUE;
}

void LunaEffect::UnInit( void )
{
    LogOut("\n============================================== Destroy Effect\n");
}

void LunaEffect::Gradation( RECT *dest, D3DCOLOR *color )
{
    GLuint vcolor[4];
    GLfloat vertex[4*4];

    vcolor[0] = color[0];
    vcolor[1] = color[1];
    vcolor[2] = color[2];
    vcolor[3] = color[3];

    vertex[0*4 + 0] = vertex[3*4 + 0] = dest->left;
    vertex[0*4 + 1] = vertex[1*4 + 1] = dest->top;
    vertex[1*4 + 0] = vertex[2*4 + 0] = dest->right;
    vertex[2*4 + 1] = vertex[3*4 + 1] = dest->bottom;

    vertex[0*4 + 2] = vertex[1*4 + 2] = vertex[2*4 + 2] = vertex[3*4 + 2] = 0.0f;
    vertex[0*4 + 3] = vertex[1*4 + 3] = vertex[2*4 + 3] = vertex[3*4 + 3] = 1.0f;


    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(4, GL_FLOAT, 0, &(vertex[0]));
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, &(vcolor[0]));
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    Luna::AddPolygon(2);
}

void LunaEffect::FillSquare( RECT *dest, D3DCOLOR color )
{
    GLuint vcolor[4];
    GLfloat vertex[4*4];

    vcolor[0] = vcolor[1] = vcolor[2] = vcolor[3] = color;

    vertex[0*4 + 0] = vertex[3*4 + 0] = dest->left;
    vertex[0*4 + 1] = vertex[1*4 + 1] = dest->top;
    vertex[1*4 + 0] = vertex[2*4 + 0] = dest->right;
    vertex[2*4 + 1] = vertex[3*4 + 1] = dest->bottom;

    vertex[0*4 + 2] = vertex[1*4 + 2] = vertex[2*4 + 2] = vertex[3*4 + 2] = 0.0f;
    vertex[0*4 + 3] = vertex[1*4 + 3] = vertex[2*4 + 3] = vertex[3*4 + 3] = 1.0f;


    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(4, GL_FLOAT, 0, &(vertex[0]));
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, &(vcolor[0]));
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    Luna::AddPolygon(2);
}

void LunaEffect::SonicRotate( long cx, long cy, long r1, float r2, float yaw, float pitch, float roll, D3DCOLOR color )
{
    GLuint vcolor[SONIC_VERNUM + 2];
    GLfloat vertex[4*(SONIC_VERNUM + 2)];

    long scale = r1 * r2;
    long angle = 0;

    union {
        struct {
            unsigned char r, g, b, a;
        } c;
        D3DCOLOR v;
    } color2;

    color2.v = color;
    color2.c.a = 0;

    int index;
    for (index = 0; index < SONIC_VERNUM; index+=2)
    {
        vertex[index*4 + 0] = LunaMath::Cos(angle / SONIC_VERNUM, r1);
        vertex[index*4 + 1] = LunaMath::Sin(angle / SONIC_VERNUM, r1);
        vertex[index*4 + 2] = 0.0f;
        vertex[index*4 + 3] = 1.0f;

        vcolor[index] = color;

        vertex[(index+1)*4 + 0] = LunaMath::Cos(angle / SONIC_VERNUM, scale);
        vertex[(index+1)*4 + 1] = LunaMath::Sin(angle / SONIC_VERNUM, scale);
        vertex[(index+1)*4 + 2] = 0.0f;
        vertex[(index+1)*4 + 3] = 1.0f;

        vcolor[index+1] = color2.v;

        angle += 2 * SINTABLE_SIZE;
    }

    D3DMATRIX mat;

    D3DXMatrixRotationYawPitchRoll(&mat, yaw, pitch, roll);

    float scale3 = ( r1 <= scale ) ? ( 3 * scale ) : ( 3 * r1 );

    for (index = 0; index < SONIC_VERNUM; index++)
    {
        D3DXVECTOR3 vec1, vec2;

        vec1.x = vertex[index*4 + 0];
        vec1.y = vertex[index*4 + 1];
        vec1.z = vertex[index*4 + 2];

        D3DXVec3TransformCoord(&vec2, &vec1, &mat);

        vertex[index*4 + 0] = ( (vec2.x * scale3) / (vec2.z + scale3) ) + cx;
        vertex[index*4 + 1] = ( (vec2.y * scale3) / (vec2.z + scale3) ) + cy;
        vertex[index*4 + 2] = 0.0f;
        vertex[index*4 + 3] = 1.0f;
    }

    vertex[SONIC_VERNUM*4 + 0] = vertex[0*4 + 0];
    vertex[SONIC_VERNUM*4 + 1] = vertex[0*4 + 1];
    vertex[SONIC_VERNUM*4 + 2] = vertex[0*4 + 2];
    vertex[SONIC_VERNUM*4 + 3] = vertex[0*4 + 3];
    vertex[(SONIC_VERNUM + 1)*4 + 0] = vertex[1*4 + 0];
    vertex[(SONIC_VERNUM + 1)*4 + 1] = vertex[1*4 + 1];
    vertex[(SONIC_VERNUM + 1)*4 + 2] = vertex[1*4 + 2];
    vertex[(SONIC_VERNUM + 1)*4 + 3] = vertex[1*4 + 3];

    vcolor[SONIC_VERNUM] = vcolor[0];
    vcolor[SONIC_VERNUM + 1] = vcolor[1];

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(4, GL_FLOAT, 0, &(vertex[0]));
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, &(vcolor[0]));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, SONIC_VERNUM + 2);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    Luna::AddPolygon(64);
}

void LunaEffect::TriangleRotate( long x, long y, long range, float yaw, float pitch, float roll, D3DCOLOR color, BOOL light )
{
    GLuint vcolor[3];
    GLfloat vertex[4*3];

    vcolor[0] = vcolor[1] = vcolor[2] = color;

    vertex[0*4 + 0] = LunaMath::Cos((SINTABLE_SIZE / 4), range);    // 90 degree
    vertex[0*4 + 1] = LunaMath::Sin((SINTABLE_SIZE / 4), range);
    vertex[1*4 + 0] = LunaMath::Cos((SINTABLE_SIZE / 4) + (long)((120 * SINTABLE_SIZE) / 360), range);  // 210 degree (90 + 120)
    vertex[1*4 + 1] = LunaMath::Sin((SINTABLE_SIZE / 4) + (long)((120 * SINTABLE_SIZE) / 360), range);
    vertex[2*4 + 0] = LunaMath::Cos((SINTABLE_SIZE / 4) + 2 * (long)((120 * SINTABLE_SIZE) / 360), range);  // 330 degree (90 + 2*120)
    vertex[2*4 + 1] = LunaMath::Sin((SINTABLE_SIZE / 4) + 2 * (long)((120 * SINTABLE_SIZE) / 360), range);

    vertex[0*4 + 2] = vertex[1*4 + 2] = vertex[2*4 + 2] = 0.0f;
    vertex[0*4 + 3] = vertex[1*4 + 3] = vertex[2*4 + 3] = 1.0f;

    D3DMATRIX mat;

    D3DXMatrixRotationYawPitchRoll(&mat, yaw, pitch, roll);

    D3DXVECTOR3 vec1, vec2, vec3;

    for (int index = 0; index < 3; index++)
    {
        vec1.x = vertex[index*4 + 0];
        vec1.y = vertex[index*4 + 1];
        vec1.z = vertex[index*4 + 2];

        D3DXVec3TransformCoord(&vec2, &vec1, &mat);

        vertex[index*4 + 0] = ( (vec2.x * (3.0f * range)) / (vec2.z + (3.0f * range)) ) + x;
        vertex[index*4 + 1] = ( (vec2.y * (3.0f * range)) / (vec2.z + (3.0f * range)) ) + y;
        vertex[index*4 + 2] = (range + vec2.z) * 0.5f;
        vertex[index*4 + 3] = 1.0f;
    }

    if (light)
    {
        vec2.x = vertex[1*4 + 0] - vertex[0*4 + 0];
        vec2.y = vertex[1*4 + 1] - vertex[0*4 + 1];
        vec2.z = vertex[1*4 + 2] - vertex[0*4 + 2];

        vec3.x = vertex[2*4 + 0] - vertex[0*4 + 0];
        vec3.y = vertex[2*4 + 1] - vertex[0*4 + 1];
        vec3.z = vertex[2*4 + 2] - vertex[0*4 + 2];

        vec1.x = (vec3.z * vec2.y) - (vec3.y * vec2.z);
        vec1.y = (vec3.x * vec2.z) - (vec3.z * vec2.x);
        vec1.z = (vec3.y * vec2.x) - (vec3.x * vec2.y);

        D3DXVec3Normalize(&vec2, &vec1);

        float cshade = vec2.z;
        if ( cshade < 0.0f ) cshade = -cshade;
        cshade = (cshade + 1.0f) * 0.5f;

        union {
            struct {
                unsigned char r, g, b, a;
            } c;
            D3DCOLOR v;
        } color2;

        color2.v = color;

        color2.c.r = cshade * (float)color2.c.r;
        color2.c.g = cshade * (float)color2.c.g;
        color2.c.b = cshade * (float)color2.c.b;

        vcolor[0] = vcolor[1] = vcolor[2] = color2.v;
    }

    vertex[0*4 + 2] = vertex[1*4 + 2] = vertex[2*4 + 2] = 0.0f;


    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(4, GL_FLOAT, 0, &(vertex[0]));
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, &(vcolor[0]));
    glDrawArrays(GL_TRIANGLE_FAN, 0, 3);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    Luna::AddPolygon(1);
}

void LunaEffect::Begin( eBlending blend )
{
    NowBlend = blend;

    Luna3D::SetRenderState(STATE_ALPHATEST, FALSE);
    Luna3D::SetRenderState(STATE_LIGHTING, FALSE);
    Luna3D::SetRenderState(STATE_ZBUFFER, FALSE);
    Luna3D::SetRenderState(STATE_ZWRITE, FALSE);
    Luna3D::SetRenderState(STATE_FOG, FALSE);
    Luna3D::SetRenderState(STATE_CULLING, CULL_NONE);
    Luna3D::SetRenderState(STATE_SHADING, SHADE_GOURAUD);

    Luna3D::SetTexture(NULL);

    if ( blend == BLEND_SUB )
    {
        Luna3D::SetBlending(BLEND_REVERSE);
        FillSquare(&ScreenRect, 0xffffffff);
        Luna3D::SetBlending(BLEND_ADD);
    }
    else
    {
        Luna3D::SetBlending(blend);
    }
}

void LunaEffect::End( void )
{
    if ( NowBlend == BLEND_SUB )
    {
        Luna3D::SetBlending(BLEND_REVERSE);
        FillSquare(&ScreenRect, 0xffffffff);
    }
}

