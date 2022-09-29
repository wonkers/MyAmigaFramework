#pragma once
#include "support/gcc8_c_support.h"
#include <exec/types.h>
#include <proto/graphics.h>
#include <graphics/gfxbase.h>
#include <graphics/view.h>
#include <graphics/gfxmacros.h>
#include "gels.h"
#include "newdelete.h"

class CustomViewPort : public ViewPort{

    public:
    UWORD m_Depth;
    struct RasInfo m_RasInfos[2];
    struct BitMap m_BitMaps[2];
    CustomRastPort m_RastPort;
    public:
    CustomViewPort()
    {
        InitVPort(this);
        this->RasInfo = &m_RasInfos[0];
        this->DxOffset = 0;
        this->DyOffset = 0;
        this->Modes = 0x0;
        this->DHeight = 256;
        this->DWidth = 320;
        this->ColorMap = GetColorMap(32);

        InitBitMap(&m_BitMaps[0], 4, 320, 256);
        InitBitMap(&m_BitMaps[1], 4, 320, 256);
        for(UBYTE plane = 0; plane < 4; plane++)
        {
            m_BitMaps[0].Planes[plane] = AllocRaster(320, 256);
            m_BitMaps[1].Planes[plane] = AllocRaster(320, 256);
            BltClear(m_BitMaps[0].Planes[plane], RASSIZE(320, 256), 0);
            BltClear(m_BitMaps[1].Planes[plane], RASSIZE(320, 256), 0);
        }

        InitRastPort(&m_RastPort);
        PLANEPTR l_RasBuffer = AllocRaster(320, 256);
        WORD l_AreaBuffer[200];
        m_RastPort.TmpRas = new TmpRas();
        m_RastPort.AreaInfo = new AreaInfo();

        InitTmpRas(m_RastPort.TmpRas, l_RasBuffer, RASSIZE(320, 256));
        InitArea(m_RastPort.AreaInfo, l_AreaBuffer, 80);
        m_RastPort.BitMap = &m_BitMaps[0];
        m_RasInfos[0].BitMap = &m_BitMaps[0];
        

    }
    CustomViewPort(struct View *view, WORD x, WORD y, UWORD width, UWORD height, UWORD depth, UWORD modes)
    {        
        m_Depth = depth;        
        
        if(GfxBase->DisplayFlags & PAL)
        {
            if(height > 256)
                modes |= LACE;
        }
        else
        {
            if(height > 200)
                modes |= LACE;
        }

        if(width > 320)
			modes |= HIRES;  

        InitVPort(this);
        this->RasInfo = &m_RasInfos[0];
        this->DxOffset = x;
        this->DyOffset = y;
        this->Modes = modes;
        this->DHeight = height;
        this->DWidth = width;
        this->ColorMap = GetColorMap(32);

        InitBitMap(&m_BitMaps[0], depth, width, height);
        InitBitMap(&m_BitMaps[1], depth, width, height);
        for(UBYTE plane = 0; plane < 4; plane++)
        {
            m_BitMaps[0].Planes[plane] = AllocRaster(width, height);
            m_BitMaps[1].Planes[plane] = AllocRaster(width, height);
            BltClear(m_BitMaps[0].Planes[plane], RASSIZE(width, height), 0);
            BltClear(m_BitMaps[1].Planes[plane], RASSIZE(width, height), 0);
        }

        InitRastPort(&m_RastPort);
        PLANEPTR l_RasBuffer = AllocRaster(width, height);
        WORD l_AreaBuffer[200];
        m_RastPort.TmpRas = new TmpRas();
        m_RastPort.AreaInfo = new AreaInfo();

        InitTmpRas(m_RastPort.TmpRas, l_RasBuffer, RASSIZE(width, height));
        InitArea(m_RastPort.AreaInfo, l_AreaBuffer, 80);
        m_RastPort.BitMap = &m_BitMaps[0];

        m_RasInfos[0].RxOffset = 0;
        m_RasInfos[0].RyOffset = 0;
        m_RasInfos[0].Next = &m_RasInfos[1];
        m_RasInfos[0].BitMap = &m_BitMaps[0];

        m_RasInfos[1].RxOffset = 0;
        m_RasInfos[1].RyOffset = 0;
        m_RasInfos[1].Next = &m_RasInfos[0];
        m_RasInfos[1].BitMap = &m_BitMaps[0];


        //MakeVPort(view, this);
        //MrgCop(view);
    }
    void SwapBuffers(int buf)
    {
        m_RastPort.BitMap = &m_BitMaps[buf];
        m_RasInfos[0].BitMap = &m_BitMaps[buf];
    }
    UWORD GetDepth()
    {
        return m_Depth;
    }
    WORD GetWidth()
    {
        return this->DWidth;
    }
    WORD GetHeight()
    {
        return this->DHeight;
    }
    struct BitMap*  GetBitMap()
    {
        return &m_BitMaps[0];
    }
};