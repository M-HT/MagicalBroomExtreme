#include "Luna-mbx.h"

#include <cstdio>
#include <cstdlib>
#include <SDL.h>

#if defined(PANDORA)
#define PANDORA_BUTTON_UP              (SDLK_UP)
#define PANDORA_BUTTON_DOWN            (SDLK_DOWN)
#define PANDORA_BUTTON_LEFT            (SDLK_LEFT)
#define PANDORA_BUTTON_RIGHT           (SDLK_RIGHT)
#define PANDORA_BUTTON_Y               (SDLK_PAGEUP)
#define PANDORA_BUTTON_X               (SDLK_PAGEDOWN)
#define PANDORA_BUTTON_A               (SDLK_HOME)
#define PANDORA_BUTTON_B               (SDLK_END)
#define PANDORA_BUTTON_L               (SDLK_RSHIFT)
#define PANDORA_BUTTON_R               (SDLK_RCTRL)
#define PANDORA_BUTTON_START           (SDLK_LALT)
#define PANDORA_BUTTON_SELECT          (SDLK_LCTRL)
#endif


//==========================================================================
// STRUCT
//==========================================================================

//-----------------------------------------------------
// 軸データ
//-----------------------------------------------------
typedef struct AXISDATA
{
	long Ax, Ay, Az;
}
AXISDATA, *LPAXISDATA;

//-----------------------------------------------------
// ジョイスティック統括
//-----------------------------------------------------
typedef struct JOYSTICKDATA2
{
	SDL_Joystick *lpDevice;
	JOYSTICKDATA Data;
	short bPolling;
	short bForceFeedback;
	AXISDATA Center;
	AXISDATA Max;
}
JOYSTICKDATA2, *LPJOYSTICKDATA2;


static unsigned char KeyboardData[256];
static JOYSTICKDATA2 JoystickData[16];
static int JoystickCount;


BOOL LunaInput::Init( void )
{
    memset(KeyboardData, 0, 256);
    memset(JoystickData, 0, 16 * sizeof(JOYSTICKDATA2));
    JoystickCount = 0;

    if (!SDL_InitSubSystem(SDL_INIT_JOYSTICK))
    {
#if defined(PANDORA)
        int num = SDL_NumJoysticks();
        int stick;
        for (stick = 0; stick < num; stick++)
        {
            if (!strcmp(SDL_JoystickName(stick), "nub0"))
            {
                JoystickData[0].lpDevice = SDL_JoystickOpen(stick);
                break;
            }
        }
#else
        if (SDL_NumJoysticks() > 0)
        {
            JoystickData[0].lpDevice = SDL_JoystickOpen(0);
        }
#endif

        if (JoystickData[0].lpDevice != NULL)
        {
            JoystickCount = 1;
        }
    }

    return TRUE;
}

void LunaInput::UnInit( void )
{
    if (JoystickCount)
    {
        SDL_JoystickClose(JoystickData[0].lpDevice);
        JoystickData[0].lpDevice = NULL;
    }
    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
}

void LunaInput::RefreshKeyboard( void )
{
    int numkeys;
    Uint8 *keystate = SDL_GetKeyState(&numkeys);

#if defined(PANDORA)
    KeyboardData[DIK_UP] = (numkeys >= PANDORA_BUTTON_UP)?(keystate[PANDORA_BUTTON_UP]):0;
    KeyboardData[DIK_DOWN] = (numkeys >= PANDORA_BUTTON_DOWN)?(keystate[PANDORA_BUTTON_DOWN]):0;
    KeyboardData[DIK_LEFT] = (numkeys >= PANDORA_BUTTON_LEFT)?(keystate[PANDORA_BUTTON_LEFT]):0;
    KeyboardData[DIK_RIGHT] = (numkeys >= PANDORA_BUTTON_RIGHT)?(keystate[PANDORA_BUTTON_RIGHT]):0;

    KeyboardData[DIK_NUMPAD2] = 0;
    KeyboardData[DIK_NUMPAD4] = 0;
    KeyboardData[DIK_NUMPAD6] = 0;
    KeyboardData[DIK_NUMPAD8] = 0;

    KeyboardData[DIK_Z] = (numkeys >= PANDORA_BUTTON_B)?(keystate[PANDORA_BUTTON_B]):0;
    KeyboardData[DIK_X] = (numkeys >= PANDORA_BUTTON_Y)?(keystate[PANDORA_BUTTON_Y]):0;
    KeyboardData[DIK_C] = (numkeys >= PANDORA_BUTTON_X)?(keystate[PANDORA_BUTTON_X]):0;
    KeyboardData[DIK_V] = (numkeys >= PANDORA_BUTTON_A)?(keystate[PANDORA_BUTTON_A]):0;
#else
    KeyboardData[DIK_UP] = (numkeys >= SDLK_UP)?(keystate[SDLK_UP]):0;
    KeyboardData[DIK_DOWN] = (numkeys >= SDLK_DOWN)?(keystate[SDLK_DOWN]):0;
    KeyboardData[DIK_LEFT] = (numkeys >= SDLK_LEFT)?(keystate[SDLK_LEFT]):0;
    KeyboardData[DIK_RIGHT] = (numkeys >= SDLK_RIGHT)?(keystate[SDLK_RIGHT]):0;

    KeyboardData[DIK_NUMPAD2] = (numkeys >= SDLK_KP2)?(keystate[SDLK_KP2]):0;
    KeyboardData[DIK_NUMPAD4] = (numkeys >= SDLK_KP4)?(keystate[SDLK_KP4]):0;
    KeyboardData[DIK_NUMPAD6] = (numkeys >= SDLK_KP6)?(keystate[SDLK_KP6]):0;
    KeyboardData[DIK_NUMPAD8] = (numkeys >= SDLK_KP8)?(keystate[SDLK_KP8]):0;

    KeyboardData[DIK_Z] = (numkeys >= SDLK_z)?(keystate[SDLK_z]):0;
    KeyboardData[DIK_X] = (numkeys >= SDLK_x)?(keystate[SDLK_x]):0;
    KeyboardData[DIK_C] = (numkeys >= SDLK_c)?(keystate[SDLK_c]):0;
    KeyboardData[DIK_V] = (numkeys >= SDLK_v)?(keystate[SDLK_v]):0;
#endif
}

void LunaInput::RefreshJoystick( void )
{
    if (JoystickCount)
    {
        SDL_JoystickUpdate();

        JoystickData[0].Data.Px = 2 * SDL_JoystickGetAxis(JoystickData[0].lpDevice, 0);
        JoystickData[0].Data.Py = 2 * SDL_JoystickGetAxis(JoystickData[0].lpDevice, 1);

        JoystickData[0].Data.Button[0] = SDL_JoystickGetButton(JoystickData[0].lpDevice, 0);
        JoystickData[0].Data.Button[1] = SDL_JoystickGetButton(JoystickData[0].lpDevice, 1);
        JoystickData[0].Data.Button[2] = SDL_JoystickGetButton(JoystickData[0].lpDevice, 2);
        JoystickData[0].Data.Button[3] = SDL_JoystickGetButton(JoystickData[0].lpDevice, 3);
    }
}

BOOL LunaInput::GetKeyData( unsigned long key )
{
    return KeyboardData[key] == 1;
}

const LPJOYSTICKDATA LunaInput::GetJoystickData( unsigned long no )
{
    if ( JoystickData[no].lpDevice != NULL )
    {
        return &(JoystickData[no].Data);
    }
    else
    {
        return NULL;
    }
}

