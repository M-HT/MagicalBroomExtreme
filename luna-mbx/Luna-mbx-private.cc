#include "Luna-mbx-private.h"

#include <cmath>
#ifdef USE_GLES
#include <GLES/gl.h>
#include <GLES/glext.h>
#else
#include <SDL_opengl.h>
#endif


#ifdef USE_GLES
extern int featureRequiredInternalFormat;
extern int featureRgb8Rgba8;
#endif

D3DMATRIX *D3DXMatrixRotationYawPitchRoll(D3DMATRIX *pOut, float Yaw, float Pitch, float Roll)
{
    float sRoll, cRoll, sPitch, cPitch, sYaw, cYaw;

    sRoll = sinf(Roll);
    cRoll = cosf(Roll);
    sPitch = sinf(Pitch);
    cPitch = cosf(Pitch);
    sYaw = sinf(Yaw);
    cYaw = cosf(Yaw);

    pOut->m[0][0] = sRoll * sPitch * sYaw + cRoll * cYaw;
    pOut->m[0][1] = sRoll * cPitch;
    pOut->m[0][2] = sRoll * sPitch * cYaw - cRoll * sYaw;
    pOut->m[0][3] = 0.0f;
    pOut->m[1][0] = cRoll * sPitch * sYaw - sRoll * cYaw;
    pOut->m[1][1] = cRoll * cPitch;
    pOut->m[1][2] = cRoll * sPitch * cYaw + sRoll * sYaw;
    pOut->m[1][3] = 0.0f;
    pOut->m[2][0] = cPitch * sYaw;
    pOut->m[2][1] = -sPitch;
    pOut->m[2][2] = cPitch * cYaw;
    pOut->m[2][3] = 0.0f;
    pOut->m[3][0] = 0.0f;
    pOut->m[3][1] = 0.0f;
    pOut->m[3][2] = 0.0f;
    pOut->m[3][3] = 1.0f;

    return pOut;
}

D3DXVECTOR3 *D3DXVec3TransformCoord(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const D3DMATRIX *pM)
{
    float norm;

    norm = pM->m[0][3] * pV->x + pM->m[1][3] * pV->y + pM->m[2][3] * pV->z + pM->m[3][3];

    pOut->x = (pM->m[0][0] * pV->x + pM->m[1][0] * pV->y + pM->m[2][0] * pV->z + pM->m[3][0]) / norm;
    pOut->y = (pM->m[0][1] * pV->x + pM->m[1][1] * pV->y + pM->m[2][1] * pV->z + pM->m[3][1]) / norm;
    pOut->z = (pM->m[0][2] * pV->x + pM->m[1][2] * pV->y + pM->m[2][2] * pV->z + pM->m[3][2]) / norm;

    return pOut;
}

D3DXVECTOR3 *D3DXVec3Normalize(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV)
{
    float norm;

    norm = (pV)?sqrtf(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z):0.0f;
    if ( !norm )
    {
        pOut->x = 0.0f;
        pOut->y = 0.0f;
        pOut->z = 0.0f;
    }
    else
    {
        pOut->x = pV->x / norm;
        pOut->y = pV->y / norm;
        pOut->z = pV->z / norm;
    }

    return pOut;
}

unsigned int next_pow2(unsigned int v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

void define_texture_image(int width, int height, int format, const void *data)
{
#ifdef USE_GLES
    switch (format)
    {
    case FMT_RGB16:
        glTexImage2D(GL_TEXTURE_2D, 0, (featureRequiredInternalFormat)?GL_RGB565_OES:GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
        break;
    case FMT_RGBA16:
        glTexImage2D(GL_TEXTURE_2D, 0, (featureRequiredInternalFormat)?GL_RGBA4_OES:GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, data);
        break;
    case FMT_RGB32:
        glTexImage2D(GL_TEXTURE_2D, 0, (featureRequiredInternalFormat && featureRgb8Rgba8)?GL_RGB8_OES:GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        break;
    case FMT_RGBA32:
        glTexImage2D(GL_TEXTURE_2D, 0, (featureRequiredInternalFormat && featureRgb8Rgba8)?GL_RGBA8_OES:GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        break;
    }
#else
    switch (format)
    {
    case FMT_RGB16:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB5, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
        break;
    case FMT_RGBA16:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA4, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, data);
        break;
    case FMT_RGB32:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        break;
    case FMT_RGBA32:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        break;
    }
#endif
}

