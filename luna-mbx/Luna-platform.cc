#include "Luna-platform.h"

#include <cstdlib>

BOOL SetRect(
  RECT   *lprc,
  int    xLeft,
  int    yTop,
  int    xRight,
  int    yBottom
)
{
    if (lprc == NULL) return FALSE;

    lprc->left = xLeft;
    lprc->top = yTop;
    lprc->right = xRight;
    lprc->bottom = yBottom;

    return TRUE;
}

