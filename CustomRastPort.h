#pragma once
#include "support/gcc8_c_support.h"
#include <exec/types.h>
#include <proto/graphics.h>
#include <graphics/gfxbase.h>
#include <graphics/view.h>
#include <graphics/gfxmacros.h>
#include "gels.h"
#include "newdelete.h"

class CustomRastPort : public RastPort
{
    private:
    LONG MaxVectors = 80;
    UWORD buffer = 200;
    public:
        CustomRastPort()
        {
            
        }
        CustomRastPort(ULONG width, ULONG height)
        {
            InitRastPort(this);
			PLANEPTR l_RasBuffer = AllocRaster(width, height);
			WORD l_AreaBuffer[buffer];
			this->TmpRas = new struct TmpRas();
			this->AreaInfo = new struct AreaInfo();

			InitTmpRas(this->TmpRas, l_RasBuffer, RASSIZE(width, height));
			InitArea(this->AreaInfo, l_AreaBuffer, 80);

        }

        void SetBitMap(struct BitMap *bitmap)
        {
            this->BitMap = bitmap;
        }

        void SetGels(Gels *gels)
        {
            this->GelsInfo = gels;
        }
};