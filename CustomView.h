#pragma once
#include "support/gcc8_c_support.h"
#include <exec/types.h>
#include <proto/graphics.h>
#include <graphics/gfxbase.h>
#include <graphics/view.h>
#include <graphics/gfxmacros.h>
#include "gels.h"
#include "CustomRastPort.h"
#include "CustomViewPort.h"
#include "newdelete.h"

class CustomView : public View
{
	public:
		UCopList *m_UserList;
		Custom *m_UserCustom;
	private:
		int m_Depth, m_Width, m_Height, m_ViewPortCount;
		//CustomRastPort m_RastPort;
		//RasInfo m_RasInfo;
		//BitMap m_BitMaps[2];
		View *m_OldView;
		//CustomViewPort *m_ViewPort;
		cprlist *m_LOF[2];
		cprlist *m_SHF[2];
		BYTE m_Buffer = 0;
		//Gels m_Gels = Gels(&m_ViewPort, &m_RastPort);
		
	public:
		void AddViewPort(int index, int x, int y, int width, int height, int depth, UWORD modes)
		{
			struct ViewPort **Refvp = &((struct View *)this)->ViewPort;
			for(int i = 0; i < index; i++)
			{
				Refvp = &((struct ViewPort *)*Refvp)->Next;
			}
			
			*Refvp = new CustomViewPort(this, x, y, width, height, depth, modes);
			m_ViewPortCount++;

			MakeVPort(this, *Refvp);
			MrgCop(this);
			m_LOF[0] = this->LOFCprList;
			m_SHF[0] = this->SHFCprList;

			this->LOFCprList = NULL;
			this->SHFCprList = NULL;
			((CustomViewPort *)*Refvp)->SwapBuffers(1);
			MakeVPort(this, *Refvp);
			MrgCop(this);
			m_LOF[1] = this->LOFCprList;
			m_SHF[1] = this->SHFCprList;

			// this->SHFCprList = m_SHF[1];
			// this->LOFCprList = m_LOF[1];
			

			// ((CustomViewPort *)*Refvp)->SwapBuffers(1);
			// GetRastPort(index)->BitMap = &GetViewPort(index)->m_BitMaps[0];
			// GetViewPort(index)->RasInfo->BitMap = &GetViewPort(index)->m_BitMaps[0];
			

		}
		void RemoveViewPort(int index)
		{
			delete GetViewPort(index);
		}

		CustomViewPort *GetViewPort(int index)
		{
			struct ViewPort **Refvp = &((struct View *)this)->ViewPort;
			for(int i = 0; i < index; i++)
			{
				Refvp = &((struct ViewPort *)*Refvp)->Next;
			}
			return (CustomViewPort*)*Refvp;
		}
		CustomRastPort *GetRastPort(int index)
		{
			return &GetViewPort(index)->m_RastPort;
		}

		CustomView(short depth, short width, short height, UWORD modes)
			: 	m_Depth(depth),
				m_Width(width),
				m_Height(height)
		{
			m_OldView = GfxBase->ActiView;
			m_UserCustom = (struct Custom*)0xdff000;	
			m_ViewPortCount = 0;	

			InitView((View *)this);

			if(width > 320)
				modes |= HIRES;
			if(height > 256)
				modes |= LACE;

			((View *)this)->Modes = modes;
			//((View *)this)->ViewPort = new CustomViewPort(0, 0, width, 64, depth, modes);//&m_ViewPort;

			// InitBitMap(&m_BitMaps[0], depth, width, height);
			// InitBitMap(&m_BitMaps[1], depth, width, height);
			// for(UBYTE plane = 0; plane < depth; plane++)
			// {
			// 	m_BitMaps[0].Planes[plane] = AllocRaster(width, height);
			// 	m_BitMaps[1].Planes[plane] = AllocRaster(width, height);
			// 	BltClear(m_BitMaps[0].Planes[plane], RASSIZE(width, height), 0);
			// 	BltClear(m_BitMaps[1].Planes[plane], RASSIZE(width, height), 0);
			// }

			//create RastPort and structures for area drawing/polygons
			// InitRastPort(&m_RastPort);
			// PLANEPTR l_RasBuffer = AllocRaster(width, height);
			// WORD l_AreaBuffer[200];
			// m_RastPort.TmpRas = new TmpRas();
			// m_RastPort.AreaInfo = new AreaInfo();

			// InitTmpRas(m_RastPort.TmpRas, l_RasBuffer, RASSIZE(width, height));
			// InitArea(m_RastPort.AreaInfo, l_AreaBuffer, 80);

			// m_RastPort.GelsInfo = &m_Gels;
			
			// m_UserList =(UCopList*) AllocMem(sizeof(UCopList), MEMF_PUBLIC|MEMF_CLEAR);
			
			// CWAIT(m_UserList, 0, 0);
			// CMOVE(m_UserList, m_UserCustom->color[0], 0x00f);
			// CWAIT(m_UserList, 128, 0);
			// CMOVE(m_UserList, m_UserCustom->color[0], 0xff0);
			// CWAIT(m_UserList,265,0);

			// CEND(m_UserList);
			// this->ViewPort->UCopIns = m_UserList;

			//make copper list 1 (buffer 0)
			// m_RastPort.BitMap = &((CustomViewPort*)this->ViewPort)->m_BitMaps[0];
			// ViewPort->RasInfo->BitMap = &((CustomViewPort*)this->ViewPort)->m_BitMaps[0];
			// MakeVPort(this, &m_ViewPort);
			//MrgCop(this);
			// m_LOF[0] = this->LOFCprList;
			// m_SHF[0] = this->SHFCprList;
			//LoadView(this);

			// this->LOFCprList = NULL;
			// this->SHFCprList = NULL;

			// //make copper list 2 (buffer 1)
			// m_RastPort.BitMap = &((CustomViewPort*)this->ViewPort)->m_BitMaps[1];
			// ViewPort->RasInfo->BitMap = &((CustomViewPort*)this->ViewPort)->m_BitMaps[1];
			// MakeVPort(this, &m_ViewPort);
			// MrgCop(this);
			// m_LOF[1] = this->LOFCprList;
			// m_SHF[1] = this->SHFCprList;
			
			// this->SHFCprList = m_SHF[0];
			// this->LOFCprList = m_LOF[0];

			//m_RastPort.BitMap =&((CustomViewPort*)this->ViewPort)->m_BitMaps[0];
			//m_ViewPort.RasInfo->BitMap = &((CustomViewPort*)this->ViewPort)->m_BitMaps[0];
			
			//LoadView(this);

		}
		~CustomView()
		{
			// FreeColorMap(m_ViewPort.ColorMap);
			
			// // for(int i = 0; i < m_RastPort.BitMap->Depth; i++)
			// // {
			// // 	FreeRaster(m_BitMaps[0].Planes[i], m_Width, m_Height);
			// // 	FreeRaster(m_BitMaps[1].Planes[i], m_Width, m_Height);
			// // }

			// FreeRaster((PLANEPTR)m_RastPort.TmpRas->RasPtr, m_Width, m_Height);

			// delete m_RastPort.TmpRas;
			// delete m_RastPort.AreaInfo;
	
			// //FreeMem(m_UserList, sizeof(UCopList));

			// FreeVPortCopLists(&m_ViewPort);

			// FreeCprList(m_LOF[0]);
			// FreeCprList(m_LOF[1]);
			// FreeCprList(m_SHF[0]);
			// FreeCprList(m_SHF[1]);

			delete (CustomViewPort *)this->ViewPort;
			LoadView(m_OldView);
			
		}

		// RastPort *GetRastPort()
		// {
		// 	return &((CustomViewPort* )this->ViewPort)->m_RastPort;
		// 	//return &m_RastPort;
		// }
		// Gels *GetGelsInfo()
		// {
		// 	return &m_Gels;
		// }
		// struct ViewPort *GetViewPort()
		// {
		// 	return this->ViewPort;
		// }

		void DrawFilledPolygon(int ViewPortIndex, ULONG colour, LONG x1, LONG y1, LONG x2, LONG y2, LONG x3, LONG y3)
		{
			SetAPen(GetRastPort(ViewPortIndex), colour);
			AreaMove(GetRastPort(ViewPortIndex), x1, y1);
			AreaDraw(GetRastPort(ViewPortIndex), x2, y2);
			AreaDraw(GetRastPort(ViewPortIndex), x3, y3);
			AreaEnd(GetRastPort(ViewPortIndex));
		}
        void DrawFilledPolygon(int ViewPortIndex, ULONG colour, Polygon *poly)
        {
			SetAPen(GetRastPort(ViewPortIndex), colour);
            AreaMove(GetRastPort(ViewPortIndex), poly->p1.x, poly->p1.y);
            AreaDraw(GetRastPort(ViewPortIndex), poly->p2.x, poly->p2.y);
            AreaDraw(GetRastPort(ViewPortIndex), poly->p3.x, poly->p3.y);
            AreaEnd(GetRastPort(ViewPortIndex));
        }
        void DrawTexturedPolygon(int ViewPortIndex, LONG x1, LONG y1, LONG x2, LONG y2, LONG x3, LONG y3, UWORD*texture, WORD height)
        {
            SetAfPt(GetRastPort(ViewPortIndex),  texture, -height);
            DrawFilledPolygon(ViewPortIndex, 0, x1, y1, x2, y2, x3, y3);
        }
        void SetTextureDraw(int ViewPortIndex )
        {
            SetAPen(GetRastPort(ViewPortIndex), 255);
            SetBPen(GetRastPort(ViewPortIndex), 0);
            SetDrMd(GetRastPort(ViewPortIndex), JAM2);
        }
        void SetWireFrame(int ViewPortIndex)
        {
            SetOPen(GetRastPort(ViewPortIndex), 15);
        }
        void DrawTexturedPolygon(int ViewPortIndex, TexturedPolygon *tx_poly)
        {
            SetAfPt(GetRastPort(ViewPortIndex), tx_poly->texture, -tx_poly->height);
            DrawFilledPolygon(ViewPortIndex, 0, (Polygon*)tx_poly);
        }
		
		void DrawRectangle(int ViewPortIndex, ULONG Colour, LONG x1, LONG y1, LONG x2, LONG y2)
		{
			SetAPen(GetRastPort(ViewPortIndex), Colour);
			Move(GetRastPort(ViewPortIndex), x1, y1);
			Draw(GetRastPort(ViewPortIndex), x2, y1);
			Draw(GetRastPort(ViewPortIndex), x2, y2);
			Draw(GetRastPort(ViewPortIndex), x1, y2);
			Draw(GetRastPort(ViewPortIndex), x1, x1);
		}


		void WriteText(int ViewPortIndex, ULONG colour, LONG x, LONG y, STRPTR text)
		{
			SetAPen(GetRastPort(ViewPortIndex), colour);
			Move(GetRastPort(ViewPortIndex), x, y);
			Text(GetRastPort(ViewPortIndex), text, strlen(text));
		}
		void Clear(int ViewPortIndex, ULONG colour)
		{
			SetRast(GetRastPort(ViewPortIndex), colour);
		}
	
		void SetColor(int ViewPortIndex, WORD i, UWORD colour)
		{
			SetRGB4(GetViewPort(ViewPortIndex), i, colour>>8, (colour&0xf0)>>4, colour&0x0f);
		}

	public:
		void DrawUpdate()
		{
			this->SHFCprList = m_SHF[m_Buffer];
		    this->LOFCprList = m_LOF[m_Buffer];
			
			for(int vp = 0; vp < this->m_ViewPortCount; vp++)
			{
				GetViewPort(vp)->SwapBuffers(m_Buffer);
				MakeVPort(this, GetViewPort(vp));
			}
			//ViewPort->RasInfo->BitMap = &((CustomViewPort*)this->ViewPort)->m_BitMaps[m_Buffer];
			//m_RastPort.BitMap = &((CustomViewPort*)this->ViewPort)->m_BitMaps[m_Buffer];

			//SortGList(&m_RastPort);
			//DoCollision(&m_RastPort);
			//DrawGList(&m_RastPort, this->ViewPort);
			
				//Only need for VSprites;
			MrgCop(this);						//Only need for VSprites;
			LoadView(this);
			WaitTOF();

			m_Buffer ^= 1;
		}
		void SwitchBuffer()
		{
			m_Buffer ^= 1;
			for(int vp = 0; vp < this->m_ViewPortCount; vp++)
			{
				GetViewPort(vp)->SwapBuffers(m_Buffer);
				//GetViewPort(vp)->RasInfo->BitMap = &GetViewPort(vp)->m_BitMaps[m_Buffer];
				//GetRastPort(vp)->BitMap = &GetViewPort(vp)->m_BitMaps[m_Buffer];
			}
			//ViewPort->RasInfo->BitMap = &((CustomViewPort*)this->ViewPort)->m_BitMaps[m_Buffer];
			//m_RastPort.BitMap = &((CustomViewPort*)this->ViewPort)->m_BitMaps[m_Buffer];
			
		}
};