#include "Luna-mbx.h"

#include <cstdio>
#include <cstdlib>
#include <SDL.h>
#include <SDL_mixer.h>


//==========================================================================
// STRUCT
//==========================================================================

// ヘッダ
struct RWFFILEHEADER
{
    char guid[4];
};

#pragma pack(2)
typedef struct PACKED twaveformatex {
    uint16_t wFormatTag;
    uint16_t nChannels;
    uint32_t nSamplesPerSec;
    uint32_t nAvgBytesPerSec;
    uint16_t nBlockAlign;
    uint16_t wBitsPerSample;
    uint16_t cbSize;
} waveformatex,*pwaveforamtex,*lpwaveformatex;
#pragma pack()

typedef struct SoundData {
    Mix_Chunk chunk;
    int channel;
} SoundData;


class LunaSound_private : public LunaSound
{
public:
    SoundData *GetData( void ) { return (SoundData *) lpSoundBuffer; }
    void SetStopped( void ) { ((SoundData *) lpSoundBuffer)->channel = -1; bPlay = FALSE; bLoop = FALSE; }
};



static CList List;
static int AudioRate, AudioChannels;
static Uint16 AudioFormat;


void LogOut(const char *format, ...);


static void ChannelFinished(int channel)
{
    LunaSound_private *sound = (LunaSound_private *) List.Top();
    for (; sound != NULL; sound = (LunaSound_private *) sound->Next)
    {
        SoundData *data = sound->GetData();

        if (data != NULL)
        {
            if (data->channel == channel)
            {
                sound->SetStopped();
                break;
            }
        }
    }
}

LunaSound::LunaSound()
{
    List.InsertTop(this);
    lpSoundBuffer = NULL;
    bPause = FALSE;
    bLoop = FALSE;
    bPlay = FALSE;
    bLoad = FALSE;
    LogOut("○ CreateSound   / ID [0x%08X] / ", (long)this);
}

LunaSound::~LunaSound()
{
    Stop();

    if (lpSoundBuffer != NULL)
    {
        SoundData *data = (SoundData *) lpSoundBuffer;
        free(data->chunk.abuf);
        free(data);
    }

    LogOut("● ReleaseSound / ID [0x%08X]\n", (long)this);
    List.Erase(this);
}

void LunaSound::LoadWave( const char *pack, const char *data )
{
    struct RWFFILEHEADER rwh;
    uint32_t DataSize;
    FILE *f;
    const char *LogMsg;
    char DataName[32];
    waveformatex wf;
    Uint8 *DataBuffer;
    int frequency, channels;
    Uint16 format;
    SoundData *sdata;


    rwh.guid[0] = 0;
    rwh.guid[1] = 0;
    rwh.guid[2] = 0;
    rwh.guid[3] = 0;

    DataSize = 0;

    LogOut("%s in %s / ", data, pack);

    f = fopen(pack, "rb");
    if (f == NULL)
    {
        LogOut("  ERROR Can't find PackFile\n");
        return;
    }

    rwh.guid[0] = fgetc(f);
    rwh.guid[1] = fgetc(f);
    rwh.guid[2] = fgetc(f);
    rwh.guid[3] = fgetc(f);
    if ( strcmp(rwh.guid, "LSD") )
    {
        LogMsg = "  ERROR This file isn't LSD File\n";
        goto logout_and_close;
    }

    while ( 1 )
    {
        fread(DataName, 1, 32, f);
        if ( feof(f) ) break;

        if ( !strcmp(DataName, data) )
        {
            fread(&DataSize, sizeof(uint32_t), 1, f);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            DataSize = SDL_Swap32(DataSize);
#endif
            break;
        }

        uint32_t dsize;
        fread(&dsize, sizeof(uint32_t), 1, f);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        dsize = SDL_Swap32(dsize);
#endif
        fseek(f, dsize + sizeof(waveformatex), 1);
    }

    if ( DataSize == 0 )
    {
        LogMsg = "  ERROR Can't Find WaveData\n";
        goto logout_and_close;
    }

    fread(&wf, sizeof(waveformatex), 1, f);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    wf.wFormatTag      = SDL_Swap16(wf.wFormatTag     );
    wf.nChannels       = SDL_Swap16(wf.nChannels      );
    wf.nSamplesPerSec  = SDL_Swap32(wf.nSamplesPerSec );
    wf.nAvgBytesPerSec = SDL_Swap32(wf.nAvgBytesPerSec);
    wf.nBlockAlign     = SDL_Swap16(wf.nBlockAlign    );
    wf.wBitsPerSample  = SDL_Swap16(wf.wBitsPerSample );
    wf.cbSize          = SDL_Swap16(wf.cbSize         );
#endif

    DataBuffer = (Uint8 *) malloc(DataSize);

    fread(DataBuffer, 1, DataSize, f);
    fclose(f);
    f = NULL;

    frequency = wf.nSamplesPerSec;
    format = (wf.wBitsPerSample == 16)?AUDIO_S16LSB:AUDIO_U8;
    channels = wf.nChannels;

    if ( ( AudioRate != frequency ) ||
         ( AudioFormat != format ) ||
         ( AudioChannels != channels )
       )
    {
        SDL_AudioCVT cvt;

        memset(&cvt, 0, sizeof(SDL_AudioCVT));
        if ( SDL_BuildAudioCVT(&cvt, format, channels, frequency, AudioFormat, AudioChannels, AudioRate) < 0 )
        {
            free(DataBuffer);
            LogMsg = "  ERROR Failed SDL_BuildAudioCVT\n";
            goto logout_and_close;
        }

        DataBuffer = (Uint8 *) realloc(DataBuffer, DataSize * cvt.len_mult);

        cvt.buf = DataBuffer;
        cvt.len = DataSize;

        if ( SDL_ConvertAudio(&cvt) )
        {
            free(DataBuffer);
            LogMsg = "  ERROR Failed SDL_ConvertAudio\n";
            goto logout_and_close;
        }

        DataSize = cvt.len * cvt.len_ratio;
        DataBuffer = (Uint8 *) realloc(cvt.buf, DataSize);
    }

    sdata = (SoundData *) malloc(sizeof(SoundData));
    sdata->chunk.allocated = 0;
    sdata->chunk.volume = 128;

    sdata->chunk.abuf = DataBuffer;
    sdata->chunk.alen = DataSize;
    sdata->channel = -1;

    lpSoundBuffer = sdata;
    bLoad = TRUE;
    bPlay = FALSE;
    bLoop = FALSE;
    bPause = FALSE;

    LogMsg = "Complete\n";

logout_and_close:
    LogOut(LogMsg);
    if (f != NULL) fclose(f);
}

BOOL LunaSound::Init( void )
{
    LogOut("\n============================================== Initialize DirectSound\n");

    if ( !SDL_InitSubSystem(SDL_INIT_AUDIO) )
    {
        int frequency, channels, buffersize;
        Uint16 format;

        frequency = 44100;
        format = AUDIO_S16LSB;
        channels = 2;
        buffersize = 4096;

        if ( !Mix_OpenAudio(frequency, format, channels, buffersize))
        {
            Mix_QuerySpec(&frequency, &format, &channels);
            AudioRate = frequency;
            AudioFormat = format;
            AudioChannels = channels;

            Mix_AllocateChannels(32);

            // set function to call when any channel finishes playback
            Mix_ChannelFinished(ChannelFinished);
        }
        else
        {
            SDL_QuitSubSystem(SDL_INIT_AUDIO);
        }
    }

    List.Init();
    return TRUE;
}

void LunaSound::UnInit( void )
{
    LogOut("\n============================================== Destroy DirectSound\n");

    Mix_HaltChannel(-1);

    {
        LunaSound_private *sound = (LunaSound_private *) List.Top();
        for (; sound != NULL; sound = (LunaSound_private *) sound->Next)
        {
            SoundData *data = sound->GetData();

            if (data != NULL)
            {
                if (data->channel >= 0)
                {
                    sound->SetStopped();
                }
            }
        }
    }

    List.Release();

    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

LSOUND LunaSound::Load( const char *pack, const char *data )
{
    LSOUND result = new LunaSound();

    result->LoadWave(pack, data);

    return result;
}

void LunaSound::Play( BOOL loop )
{
    if ( bLoad )
    {
        Stop();

        SoundData *data = (SoundData *) lpSoundBuffer;

        data->channel = Mix_PlayChannel(-1, &(data->chunk), (loop)?-1:0);

        bLoop = loop;
        bPlay = TRUE;
    }
}

void LunaSound::Stop( void )
{
    if ( bLoad && bPlay )
    {
        int channel = -1;

        if (lpSoundBuffer != NULL)
        {
            channel = ((SoundData *) lpSoundBuffer)->channel;
        }

        if (channel >= 0)
        {
            Mix_HaltChannel(channel);

            ((SoundData *) lpSoundBuffer)->channel = -1;
        }

        bPlay = FALSE;
        bLoop = FALSE;
    }
}

