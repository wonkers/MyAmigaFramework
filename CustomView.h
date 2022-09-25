#pragma once
#include "support/gcc8_c_support.h"
#include <exec/types.h>
#include <proto/graphics.h>
#include <graphics/gfxbase.h>
#include <graphics/view.h>
#include <graphics/gfxmacros.h>
#include "gels.h"
#include "CustomRastPort.h"
#include "newdelete.h"

class CustomView : public View
{
	public:
		UCopList *m_UserList;
		Custom *m_UserCustom;
	private:
		int m_Depth, m_Width, m_Height;
		RastPort m_RastPort;
		RasInfo m_RasInfo;
		BitMap m_BitMaps[2];
		View *m_OldView;
		struct ViewPort m_ViewPort;
		cprlist *m_LOF[2];
		cprlist *m_SHF[2];
		BYTE m_Buffer = 0;
		Gels m_Gels = Gels(&m_ViewPort, &m_RastPort);
		
	public:
		CustomView(short depth, short width, short height, UWORD modes)
			: 	m_Depth(depth),
				m_Width(width),
				m_Height(height)
		{
			m_OldView = GfxBase->ActiView;
			m_UserCustom = (struct Custom*)0xdff000;		

			InitView((View *)this);
			InitVPort(&m_ViewPort);			

			if(width > 320)
				modes |= HIRES;
			if(height > 256)
				modes |= LACE;

			m_RasInfo.RxOffset = 0;
			m_RasInfo.RyOffset = 0;

			this->Modes = modes;
			this->ViewPort = &m_ViewPort;
			m_ViewPort.RasInfo = &m_RasInfo;
			m_ViewPort.DxOffset = 0;
			m_ViewPort.DyOffset = 0;
			m_ViewPort.Modes = modes;
			m_ViewPort.DHeight = height;
			m_ViewPort.DWidth = width;
			m_ViewPort.ColorMap = GetColorMap(32);

			InitBitMap(&m_BitMaps[0], depth, width, height);
			InitBitMap(&m_BitMaps[1], depth, width, height);
			for(UBYTE plane = 0; plane < depth; plane++)
			{
				m_BitMaps[0].Planes[plane] = AllocRaster(width, height);
				m_BitMaps[1].Planes[plane] = AllocRaster(width, height);
				BltClear(m_BitMaps[0].Planes[plane], RASSIZE(width, height), 0);
				BltClear(m_BitMaps[1].Planes[plane], RASSIZE(width, height), 0);
			}

			//create RastPort and structures for area drawing/polygons
			InitRastPort(&m_RastPort);
			PLANEPTR l_RasBuffer = AllocRaster(width, height);
			WORD l_AreaBuffer[200];
			m_RastPort.TmpRas = new TmpRas();
			m_RastPort.AreaInfo = new AreaInfo();

			InitTmpRas(m_RastPort.TmpRas, l_RasBuffer, RASSIZE(width, height));
			InitArea(m_RastPort.AreaInfo, l_AreaBuffer, 80);

			m_RastPort.GelsInfo = &m_Gels;
			
			// m_UserList =(UCopList*) AllocMem(sizeof(UCopList), MEMF_PUBLIC|MEMF_CLEAR);
			
			// CWAIT(m_UserList, 0, 0);
			// CMOVE(m_UserList, m_UserCustom->color[0], 0x00f);
			// CWAIT(m_UserList, 128, 0);
			// CMOVE(m_UserList, m_UserCustom->color[0], 0xff0);
			// CWAIT(m_UserList,265,0);

			// CEND(m_UserList);
			// this->ViewPort->UCopIns = m_UserList;

			//make copper list 1 (buffer 0)
			m_RastPort.BitMap = &m_BitMaps[0];
			m_RasInfo.BitMap = &m_BitMaps[0];
			MakeVPort(this, &m_ViewPort);
			MrgCop(this);
			m_LOF[0] = this->LOFCprList;
			m_SHF[0] = this->SHFCprList;
			LoadView(this);

			this->LOFCprList = NULL;
			this->SHFCprList = NULL;

			//make copper list 2 (buffer 1)
			m_RastPort.BitMap = &m_BitMaps[1];
			m_RasInfo.BitMap = &m_BitMaps[1];
			MakeVPort(this, &m_ViewPort);
			MrgCop(this);
			m_LOF[1] = this->LOFCprList;
			m_SHF[1] = this->SHFCprList;
			
			this->SHFCprList = m_SHF[0];
			this->LOFCprList = m_LOF[0];

			m_RastPort.BitMap = &m_BitMaps[0];
			m_ViewPort.RasInfo->BitMap = &m_BitMaps[0];
			
			LoadView(this);

		}
		~CustomView()
		{
			FreeColorMap(m_ViewPort.ColorMap);
			
			for(int i = 0; i < m_RastPort.BitMap->Depth; i++)
			{
				FreeRaster(m_BitMaps[0].Planes[i], m_Width, m_Height);
				FreeRaster(m_BitMaps[1].Planes[i], m_Width, m_Height);
			}

			FreeRaster((PLANEPTR)m_RastPort.TmpRas->RasPtr, m_Width, m_Height);

			delete m_RastPort.TmpRas;
			delete m_RastPort.AreaInfo;
	
			//FreeMem(m_UserList, sizeof(UCopList));

			FreeVPortCopLists(&m_ViewPort);

			FreeCprList(m_LOF[0]);
			FreeCprList(m_LOF[1]);
			FreeCprList(m_SHF[0]);
			FreeCprList(m_SHF[1]);

			LoadView(m_OldView);
			
		}

		RastPort *GetRastPort()
		{
			return &m_RastPort;
		}
		Gels *GetGelsInfo()
		{
			return &m_Gels;
		}
		struct ViewPort *GetViewPort()
		{
			return this->ViewPort;
		}

		void DrawFilledPolygon(LONG x1, LONG y1, LONG x2, LONG y2, LONG x3, LONG y3)
		{
			AreaMove(&m_RastPort, x1, y1);
			AreaDraw(&m_RastPort, x2, y2);
			AreaDraw(&m_RastPort, x3, y3);
			AreaEnd(&m_RastPort);
		}
        void DrawFilledPolygon(Polygon *poly)
        {
            AreaMove(&m_RastPort, poly->p1.x, poly->p1.y);
            AreaDraw(&m_RastPort, poly->p2.x, poly->p2.y);
            AreaDraw(&m_RastPort, poly->p3.x, poly->p3.y);
            AreaEnd(&m_RastPort);
        }
        void DrawTexturedPolygon(LONG x1, LONG y1, LONG x2, LONG y2, LONG x3, LONG y3, UWORD*texture, WORD height)
        {
            SetAfPt(&m_RastPort,  texture, -height);
            DrawFilledPolygon(x1, y1, x2, y2, x3, y3);
        }
        void SetTextureDraw()
        {
            SetAPen(&m_RastPort, 255);
            SetBPen(&m_RastPort, 0);
            SetDrMd(&m_RastPort, JAM2);
        }
        void SetWireFrame()
        {
            SetOPen(&m_RastPort, 15);
        }
        void DrawTexturedPolygon(TexturedPolygon *tx_poly)
        {
            SetAfPt(&m_RastPort, tx_poly->texture, -tx_poly->height);
            DrawFilledPolygon((Polygon*)tx_poly);
        }
		
		void DrawRectangle(LONG x1, LONG y1, LONG x2, LONG y2)
		{
			Move(&m_RastPort, x1, y1);
			Draw(&m_RastPort, x2, y1);
			Draw(&m_RastPort, x2, y2);
			Draw(&m_RastPort, x1, y2);
			Draw(&m_RastPort, x1, x1);
		}
		void WriteText(LONG x, LONG y, STRPTR text)
		{
			Move(&m_RastPort, x, y);
			Text(&m_RastPort, text, strlen(text));
		}
		void SetColor(WORD i, UWORD colour)
		{
			SetRGB4(this->ViewPort, i, colour>>8, (colour&0xf0)>>4, colour&0x0f);
		}

	

	public:
		void SetPen(int colour)
		{
			SetAPen(&m_RastPort, colour);
		}
		void DrawUpdate()
		{
			this->SHFCprList = m_SHF[m_Buffer];
		    this->LOFCprList = m_LOF[m_Buffer];
			m_RasInfo.BitMap = &m_BitMaps[m_Buffer];
			m_RastPort.BitMap = &m_BitMaps[m_Buffer];

			SortGList(&m_RastPort);
			DoCollision(&m_RastPort);
			DrawGList(&m_RastPort, this->ViewPort);
			
			MakeVPort(this, this->ViewPort);	//Only need for VSprites;
			MrgCop(this);						//Only need for VSprites;
			LoadView(this);
			WaitTOF();

			m_Buffer ^= 1;
		}
		void SwitchBuffer()
		{
			m_Buffer ^= 1;
			m_RasInfo.BitMap = &m_BitMaps[m_Buffer];
			m_RastPort.BitMap = &m_BitMaps[m_Buffer];
			
		}
};