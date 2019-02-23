#include "Luna-mbx.h"
#include "Luna-mbx-private.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdint.h>
#include <ctype.h>

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


void LunaGraphicLoader::ConvertFormat( long w, long h, LAGPIXELFORMAT src_fmt, void *pSrc, LAGPIXELFORMAT dest_fmt, void **ppDest )
{
    long remaining, county, countx;

    if (h <= 0) return;

#define COPY_COLOR(color, fsrc, fdst, conv) ((fdst *)*ppDest)[county + countx].color = conv(((fsrc *)pSrc)[county + countx].color)
#define COPY_PIXEL(fsrc, fdst, conv) { COPY_COLOR(a, fsrc, fdst, conv); COPY_COLOR(r, fsrc, fdst, conv); COPY_COLOR(g, fsrc, fdst, conv); COPY_COLOR(b, fsrc, fdst, conv);}
#define CONV_NONE
#define CONV_16_FLOAT(value) ((value) * (double)(1.0/15.0))
#define CONV_32_FLOAT(value) ((value) * (double)(1.0/255.0))
#define CONV_64_FLOAT(value) ((value) * (double)(1.0/65535.0))

#define CONV_16_32(value) ((value) * (255/15))
#define CONV_16_64(value) ((value) * (65535/15))
#define CONV_32_64(value) ((value) * (65535/255))

#define CONV_32_16(value) (((value) * 15 + 127) / 255)
#define CONV_64_16(value) (((value) * 15 + 32767) / 65535)
#define CONV_64_32(value) (((value) * 255 + 32767) / 65535)

#define CONV_FLOAT_16(value) ((value) * 15.0)
#define CONV_FLOAT_32(value) ((value) * 255.0)
#define CONV_FLOAT_64(value) ((value) * 65535.0)

    county = 0;
    for (remaining = h; remaining != 0; remaining--)
    {
        for (countx = 0; countx < w; countx++)
        {
            switch (src_fmt)
            {
            case LAGFMT_A4R4G4B4:
                switch (dest_fmt)
                {
                case LAGFMT_A4R4G4B4:
                    COPY_PIXEL(PIXELDATA16, PIXELDATA16, CONV_NONE)
                    break;

                case LAGFMT_A8R8G8B8:
                    COPY_PIXEL(PIXELDATA16, PIXELDATA32, CONV_16_32)
                    break;

                case LAGFMT_A16R16G16B16:
                    COPY_PIXEL(PIXELDATA16, PIXELDATA64, CONV_16_64)
                    break;

                case LAGFMT_FLOAT:
                    COPY_PIXEL(PIXELDATA16, PIXELDATAFLOAT, CONV_16_FLOAT)
                    break;
                }

                break;

            case LAGFMT_A8R8G8B8:
                switch (dest_fmt)
                {
                case LAGFMT_A4R4G4B4:
                    COPY_PIXEL(PIXELDATA32, PIXELDATA16, CONV_32_16)
                    break;

                case LAGFMT_A8R8G8B8:
                    COPY_PIXEL(PIXELDATA32, PIXELDATA32, CONV_NONE)
                    break;

                case LAGFMT_A16R16G16B16:
                    COPY_PIXEL(PIXELDATA32, PIXELDATA64, CONV_32_64)
                    break;

                case LAGFMT_FLOAT:
                    COPY_PIXEL(PIXELDATA32, PIXELDATAFLOAT, CONV_32_FLOAT)
                    break;
                }

                break;

            case LAGFMT_A16R16G16B16:
                switch (dest_fmt)
                {
                case LAGFMT_A4R4G4B4:
                    COPY_PIXEL(PIXELDATA64, PIXELDATA16, CONV_64_16)
                    break;

                case LAGFMT_A8R8G8B8:
                    COPY_PIXEL(PIXELDATA64, PIXELDATA32, CONV_64_32)
                    break;

                case LAGFMT_A16R16G16B16:
                    COPY_PIXEL(PIXELDATA64, PIXELDATA64, CONV_NONE)
                    break;

                case LAGFMT_FLOAT:
                    COPY_PIXEL(PIXELDATA64, PIXELDATAFLOAT, CONV_64_FLOAT)
                    break;
                }

                break;

            case LAGFMT_FLOAT:
                switch (dest_fmt)
                {
                case LAGFMT_A4R4G4B4:
                    COPY_PIXEL(PIXELDATAFLOAT, PIXELDATA16, CONV_FLOAT_16)
                    break;

                case LAGFMT_A8R8G8B8:
                    COPY_PIXEL(PIXELDATAFLOAT, PIXELDATA32, CONV_FLOAT_32)
                    break;

                case LAGFMT_A16R16G16B16:
                    COPY_PIXEL(PIXELDATAFLOAT, PIXELDATA64, CONV_FLOAT_64)
                    break;

                case LAGFMT_FLOAT:
                    COPY_PIXEL(PIXELDATAFLOAT, PIXELDATAFLOAT, CONV_NONE)
                    break;
                }

                break;
            }
        }

        county += w;
    }

#undef COPY_COLOR
#undef COPY_PIXEL
#undef CONV_NONE
#undef CONV_16_FLOAT
#undef CONV_32_FLOAT
#undef CONV_64_FLOAT
#undef CONV_16_32
#undef CONV_16_64
#undef CONV_32_64
#undef CONV_32_16
#undef CONV_64_16
#undef CONV_64_32
#undef CONV_FLOAT_16
#undef CONV_FLOAT_32
#undef CONV_FLOAT_64
}

long LunaGraphicLoader::LoadLAG( const char *file, const char *name, long *w, long *h, void **ppData, LAGPIXELFORMAT *fmt )
{
    LAGFILEHEADER lgh = { "" };
    LAGDATAHEADER ldh = { "", 0, 0, 0, 0 };
    long retval;

    retval = 0;

    FILE *f = fopen(file, "rb");
    if (f == NULL) return 0;

    fread( &lgh, sizeof(LAGFILEHEADER), 1, f );
    if (0 != strcmp(lgh.chunk, "LAG")) goto load_end;

    while (true)
    {
        memset(&ldh, 0, sizeof(LAGDATAHEADER));

        fread(&ldh, sizeof(LAGDATAHEADER), 1, f);

        if (feof(f)) break;

        if (0 != strcmp(ldh.name, name))
        {
            fseek(f, GetLAGDataSize(ldh.width, ldh.height, (LAGPIXELFORMAT)ldh.format), SEEK_CUR);
            continue;
        }

        if (w != NULL) *w = ldh.width;
        if (h != NULL) *h = ldh.height;
        if (fmt != NULL) *fmt = (LAGPIXELFORMAT)ldh.format;

        retval = GetLAGDataSize(ldh.width, ldh.height, (LAGPIXELFORMAT)ldh.format);

        if (ppData == NULL) break;

        fread(*ppData, retval, 1, f);

        break;
    }

load_end:
    fclose(f);

    return retval;
}

long LunaGraphicLoader::GetLAGDataSize( long w, long h, LAGPIXELFORMAT fmt )
{
    switch (fmt)
    {
        case LAGFMT_A4R4G4B4:
            return (w * h) * 2;
        case LAGFMT_A8R8G8B8:
            return (w * h) * 4;
        case LAGFMT_A16R16G16B16:
            return (w * h) * 8;
        case LAGFMT_FLOAT:
            return (w * h) * 16;
        default:
            return 0;
    }
}

LunaGraphicLoader::LunaGraphicLoader( void )
{
    List.InsertEnd(this);
    pBits = NULL;
    Width = 0;
    Height = 0;
    Pitch = 0;
}

LunaGraphicLoader::~LunaGraphicLoader()
{
    if (pBits != NULL)
    {
        free(pBits);
    }

    List.Erase(this);
}

BOOL LunaGraphicLoader::Init( void )
{
    List.Init();
    return TRUE;
}

void LunaGraphicLoader::UnInit( void )
{
    List.Release();
}

LGRAPHICS LunaGraphicLoader::Create( void )
{
    return new LunaGraphicLoader();
}

const char *LunaGraphicLoader::LoadLAG( const char *file, const char *name )
{
    static char ErrMsg[256];
    char tempname[256] = "";
    long width, height, size;
    void *data;
    LAGPIXELFORMAT format;
    const char *retval;

    retval = ErrMsg;

    data = NULL;

    for (char *tempptr = tempname; *name != 0; name++, tempptr++)
    {
        *tempptr = tolower(*name);
    }

    size = LoadLAG(file, tempname, &width, &height, NULL, &format);
    if (size == 0)
    {
        strcpy(ErrMsg, "Can't Load LAG File");
        goto load_end;
    }

    data = malloc(size);
    if (data == NULL)
    {
        strcpy(ErrMsg, "メモリ確保に失敗 空きメモリが不足している可能性があります");
        goto load_end;
    }

    LoadLAG(file, tempname, &width, &height, &data, &format);

    if (format == LAGFMT_A8R8G8B8)
    {
        pBits = (PIXELDATA32 *)data;
        data = NULL;
    }
    else
    {
        pBits = (PIXELDATA32 *) malloc(GetLAGDataSize(width, height, LAGFMT_A8R8G8B8));
        if (pBits == NULL)
        {
            strcpy(ErrMsg, "メモリ確保に失敗 空きメモリが不足している可能性があります");
            goto load_end;
        }

        ConvertFormat(width, height, format, data, LAGFMT_A8R8G8B8, (void **)&pBits);
    }

    Width = width;
    Height = height;
    Pitch = width * 4;

    retval = NULL;

load_end:
    if (data != NULL) free(data);

    return retval;
}

void LunaGraphicLoader::GetGraphicSize( long *pw, long *ph )
{
    if (pw != NULL) *pw = Width;
    if (ph != NULL) *ph = Height;
}

BOOL LunaGraphicLoader::CopyToSurface( void *dest, long pitch, int fmt )
{
    PIXELDATA32 *src;
    uint8_t *dst8;
    uint16_t *dst16;
    long x, y;

    switch (fmt)
    {
    case FMT_RGB16:
        src = pBits;
        dst16 = (uint16_t *) dest;

        for (y = 0; y < Height; y++)
        {
            for (x = 0; x < Width; x++)
            {
                dst16[x] = ((src[x].r >> 3) << 11) | ((src[x].g >> 2) << 5) | ((src[x].b >> 3) << 0);
            }

            src = (PIXELDATA32 *) (Pitch + (uintptr_t)src);
            dst16 = (uint16_t *) (pitch + (uintptr_t)dst16);
        }

        return TRUE;
    case FMT_RGBA16:
        src = pBits;
        dst16 = (uint16_t *) dest;

        for (y = 0; y < Height; y++)
        {
            for (x = 0; x < Width; x++)
            {
                dst16[x] = ((src[x].r >> 4) << 12) | ((src[x].g >> 4) << 8) | ((src[x].b >> 4) << 4) | ((src[x].a >> 4) << 0);
            }

            src = (PIXELDATA32 *) (Pitch + (uintptr_t)src);
            dst16 = (uint16_t *) (pitch + (uintptr_t)dst16);
        }

        return TRUE;
    case FMT_RGB32:
        src = pBits;
        dst8 = (uint8_t *) dest;

        for (y = 0; y < Height; y++)
        {
            for (x = 0; x < Width; x++)
            {
                dst8[3 * x + 0] = src[x].r;
                dst8[3 * x + 1] = src[x].g;
                dst8[3 * x + 2] = src[x].b;
            }

            src = (PIXELDATA32 *) (Pitch + (uintptr_t)src);
            dst8 = (uint8_t *) (pitch + (uintptr_t)dst8);
        }

        return TRUE;
    case FMT_RGBA32:
        src = pBits;
        dst8 = (uint8_t *) dest;

        for (y = 0; y < Height; y++)
        {
            for (x = 0; x < Width; x++)
            {
                dst8[4 * x + 0] = src[x].r;
                dst8[4 * x + 1] = src[x].g;
                dst8[4 * x + 2] = src[x].b;
                dst8[4 * x + 3] = src[x].a;
            }

            src = (PIXELDATA32 *) (Pitch + (uintptr_t)src);
            dst8 = (uint8_t *) (pitch + (uintptr_t)dst8);
        }

        return TRUE;
    default:
        return FALSE;
    }
}

