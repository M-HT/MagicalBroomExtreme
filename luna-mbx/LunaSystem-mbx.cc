#include "Luna-mbx.h"

#if (defined(_WIN32) || defined(__WIN32__) || (__WINDOWS__))
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <unistd.h>
#endif
#include <stdint.h>
#include <cstdio>
#include <cstring>
#include <SDL_endian.h>


BOOL LunaSystem::SaveData( const char *file, void *pData, unsigned long size )
{
    FILE *f = fopen(file, "wb");
    if (f == NULL) return FALSE;

    fwrite("LSD", 1, 4, f);
    uint32_t size2 = (uint32_t)size;
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    fwrite(&size2, 4, 1, f);
#else
    uint32_t size2LE = SDL_Swap32(size2);
    fwrite(&size2LE, 4, 1, f);
#endif

    uint8_t xorvalue = (uint8_t)size2;
    uint8_t *bData = (uint8_t *)pData;
    for (; size2 != 0; size2--)
    {
        uint8_t value = xorvalue ^ *bData;
        bData++;
        xorvalue++;
        fputc((int)(int8_t)(((value >> 4) & 0x0f) | (value << 4)), f);
    }

    fclose(f);

    return TRUE;
}

BOOL LunaSystem::LoadData( const char *file, void *pData, unsigned long size )
{
    FILE *f = fopen(file, "rb");
    if (f == NULL) return false;

    BOOL result = FALSE;

    char header[4];
    fread(&(header[0]), 1, 4, f);
    if (0 == strcmp(&(header[0]), "LSD"))
    {
        uint32_t size2 = 0;
        fread(&size2, 4, 1, f);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        size2 = SDL_Swap32(size2);
#endif
        if (size2 == size)
        {
            uint8_t xorvalue = (uint8_t)size2;
            uint8_t *bData = (uint8_t *)pData;
            for (; size2 != 0; size2--)
            {
              uint8_t value = fgetc(f);
              *bData = xorvalue ^ (((value >> 4) & 0x0f) | (value << 4));
              bData++;
              xorvalue++;
            }
            result = TRUE;
        }
    }
    fclose(f);

    return result;
}

void LunaSystem::SetShowIME( BOOL flag )
{
#if (defined(_WIN32) || defined(__WIN32__) || (__WINDOWS__))
    WINNLSEnableIME(0, flag);
#else
    // do nothing
#endif
}

BOOL LunaSystem::FileExist( const char *file )
{
#if (defined(_WIN32) || defined(__WIN32__) || (__WINDOWS__))
    struct _WIN32_FIND_DATAA FindFileData;

    HANDLE FindFileHandle = FindFirstFileA(file, &FindFileData);
    FindClose(FindFileHandle);
    return (FindFileHandle != INVALID_HANDLE_VALUE);
#else
    return (!access(file, F_OK));
#endif
}

