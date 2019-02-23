enum {
    FMT_RGB16 = 0,
    FMT_RGBA16 = 1,
    FMT_RGB32 = 2,
    FMT_RGBA32 = 3,
};


typedef struct _D3DMATRIX {
    union {
        struct {
            float        _11, _12, _13, _14;
            float        _21, _22, _23, _24;
            float        _31, _32, _33, _34;
            float        _41, _42, _43, _44;

        };
        float m[4][4];
    };
} D3DMATRIX;

typedef struct D3DXVECTOR3 {
  float x;
  float y;
  float z;
} D3DXVECTOR3, *LPD3DXVECTOR3;

D3DMATRIX *D3DXMatrixRotationYawPitchRoll(D3DMATRIX *pOut, float Yaw, float Pitch, float Roll);
D3DXVECTOR3 *D3DXVec3TransformCoord(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV, const D3DMATRIX *pM);
D3DXVECTOR3 *D3DXVec3Normalize(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV);

unsigned int next_pow2(unsigned int v);

void define_texture_image(int width, int height, int format, const void *data);

