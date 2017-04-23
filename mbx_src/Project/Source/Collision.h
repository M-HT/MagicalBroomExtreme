/* encoding = CP932 */
//======================================================================================
//======================================================================================
//
//
//			ƒ^ƒCƒgƒ‹
//
//
//======================================================================================
//======================================================================================

#pragma once

//======================================================================================
// INCLUDE
//======================================================================================
#include "Base.h"

//======================================================================================
// STRUCT
//======================================================================================
typedef struct LINEDATA
{
	long x1, y1;
	long x2, y2;
}
LINEDATA, *LPLINEDATA;

typedef struct HITPOINT
{
	long x, y;
}
HITPOINT, *LPHITPOINT;

//======================================================================================
// CLASS
//======================================================================================
class Collision
{
private:
	static bool Intersect( LPHITPOINT pt11, LPHITPOINT pt12, LPHITPOINT pt21, LPHITPOINT pt22 );
	static bool Straddle( LPLINEDATA l1, LPLINEDATA l2 );
	static long Side( long x, long y, LPLINEDATA l );

public:
	static bool Check( LPHITPOINT pt1, long cnt1, LPHITPOINT pt2, long cnt2 );
	static bool CheckLine( LPHITPOINT pt1, long cnt1, LPHITPOINT pt2, long cnt2 );
};
