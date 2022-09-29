#pragma once
#include "support/gcc8_c_support.h"
#include <exec/types.h>
#include <proto/graphics.h>
#include <graphics/gfxbase.h>
#include <graphics/view.h>
#include <graphics/gfxmacros.h>
#include "newdelete.h"

namespace Hardware
{
    volatile struct Custom * CustomChips = (struct Custom*)0xdff000;
    
    LONG GetClock()
    {
        LONG Clock = (GfxBase->DisplayFlags & PAL) ? 3546895 : 3579545;
        return Clock;
    }

    BOOL IsPAL()
    {
        return (GfxBase->DisplayFlags & PAL) >> 2;
    }
}