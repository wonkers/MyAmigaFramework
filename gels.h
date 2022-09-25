
#pragma once
#include <exec/types.h>
#include <proto/graphics.h>
#include <graphics/gfxbase.h>
#include <graphics/view.h>
#include "newdelete.h"
#include "customTypes.h"

class HitBox : public Rectangle{
	public:
		HitBox(){};
		HitBox(int x1, int y1, int x2, int y2)
		{
			this->MinX = x1;
			this->MinY = y1;
			this->MaxX = x2;
			this->MaxY = y2;
		}
		BOOL Intersects(Vector2d *point)
		{
			if(point->x >= this->MinX && point->x <= this->MaxX)
				if(point->y >= this->MinY && point->y <= this->MaxY)
					return TRUE;
			return FALSE;
		}
		BOOL Intersects(Rectangle *rectangle)
		{
			if(rectangle->MinX >= this->MinX && rectangle->MinX <= this->MaxX)
			{
				if(rectangle->MinY >= this->MinY && rectangle->MinY <= this->MaxY)
					return TRUE;
				if(rectangle->MaxY >= this->MinY && rectangle->MaxY <= this->MaxY)
					return TRUE;
			}
			if(rectangle->MaxX >= this->MinX && rectangle->MaxX <= this->MaxX)
			{
				if(rectangle->MinY >= this->MinY && rectangle->MinY <= this->MaxY)
					return TRUE;
				if(rectangle->MaxY >= this->MinY && rectangle->MaxY <= this->MaxY)
					return TRUE;
			}
			return FALSE;
		}
};

class Gels : public GelsInfo
{
	public:
		Gels(struct ViewPort *vp, struct RastPort *rp)
		{
			m_Start = new VSprite();
			m_End = new VSprite();
			m_collisionTable = new collTable();

			WORD *l_LastColors[8] = {0,0,0,0,0,0,0,0};
			WORD l_NextLines[8] = {0,0,0,0,0,0,0,0};

			this->sprRsrvd = 0xfa; //1111 1100
			this->nextLine = l_NextLines;
			this->lastColor = l_LastColors;
			this->collHandler = m_collisionTable;
			this->leftmost = 0;
			this->rightmost = vp->DWidth-1;
			this->topmost = 0;
			this->bottommost = vp->DHeight-1;
			
			InitGels(m_Start, m_End, this);
			rp->GelsInfo = this;
		}
		~Gels()
		{
			delete m_Start;
			delete m_End;
			delete m_collisionTable;
		}

	private:
		VSprite *m_Start;
		VSprite *m_End;
		collTable *m_collisionTable;
};




class HardwareSprite : public SimpleSprite
{
	public:
		HardwareSprite(ViewPort *vp, WORD num)
		{
			m_ViewPort = vp;
			this->num = GetSprite(this, num);
		}
		~HardwareSprite()
		{
			FreeSprite(this->num);
		}
		void SetPosition(LONG x, LONG y)
		{
			MoveSprite(m_ViewPort, this, x, y);
		}
		void Change(UWORD *newData)
		{
			ChangeSprite(m_ViewPort, this, newData);
		}

	private:
		ViewPort *m_ViewPort;
};

class CustomSprite : public VSprite{
	public:
	Vector2d m_Velocity;
	Vector2d m_Target;
	HitBox m_HitBox;
	char *m_Nname;

};


class Sprite : public CustomSprite
{
	public:

	Sprite(WORD x, WORD y, WORD *ImageData, WORD height, WORD *colours)
	{
		m_Velocity.x = 0; 
		m_Velocity.y = 0;

		this->Width = 1;
		this->Height = height;
		this->Flags = VSPRITE;
		this->Depth = 2;
		this->ImageData = ImageData;
		this->SprColors = colours;
		this->X = x;
		this->Y = y;

		m_HitBox.MinX = 0;
		m_HitBox.MinY = 0;
		m_HitBox.MaxX = 16;
		m_HitBox.MaxY = height;
	}
	~Sprite()
	{
		if(this->BorderLine != NULL)
			FreeMem(BorderLine, 2);
		if(this->CollMask != NULL)
			FreeMem(this->CollMask, RASSIZE(16, this->Height));
	}

	VOID SetCollisionOn(UWORD Me, UWORD Hit)
	{
		WORD *l_Borderline = (WORD*)AllocMem(2, MEMF_CLEAR|MEMF_CHIP);
		WORD *l_CollisionMask = (WORD*)AllocMem(RASSIZE(16, this->Height), MEMF_CLEAR|MEMF_CHIP);

		this->CollMask = l_CollisionMask;
		this->BorderLine = l_Borderline;
		this->MeMask = Me;
		this->HitMask = Hit;
		
		InitMasks(this);
	}

	private:
	
};

class BobSprite : public Bob
{
	public:
	BobSprite()
	{
		
	}
};

class BlitterObject : public CustomSprite
{
	public:

	BlitterObject(WORD x, WORD y, WORD *ImageData, WORD depth, WORD height, WORD width)
	{
		m_Velocity.x = 0; 
		m_Velocity.y = 0;
		m_Bob = new Bob();
		this->CollMask = 0;
		this->BorderLine = 0;
		
		this->Width = width>>4;
		this->Height = height;
		this->Depth = depth;
		this->ImageData = ImageData;
		this->X = x;
		this->Y = y;
		this->PlaneOnOff = 0x0;
		this->PlanePick = 0xff;
		this->Flags = OVERLAY;
		this->MeMask = 0x0;
		this->HitMask = 0x0;

		m_HitBox.MinX = 0;
		m_HitBox.MinY = 0;
		m_HitBox.MaxX = width;
		m_HitBox.MaxY = height;
		
		m_Bob->SaveBuffer = 0;
		m_Bob->DBuffer = 0;
		m_Bob->Flags = 0;
		m_Bob->BobVSprite = this;
		this->VSBob = m_Bob;

		WORD *l_Borderline = (WORD*)AllocMem(sizeof(WORD) * (width>>4), MEMF_CLEAR|MEMF_CHIP);
		WORD *l_CollisionMask = (WORD*)AllocMem(RASSIZE(width, height), MEMF_CLEAR|MEMF_CHIP);

		this->CollMask = l_CollisionMask;
		this->BorderLine = l_Borderline; 
		
		m_Bob->ImageShadow = l_CollisionMask;

		//non brush
		WORD *l_saveBuffer = (WORD *)AllocMem(RASSIZE(width, height)*(depth), MEMF_CLEAR|MEMF_CHIP);
		m_Bob->SaveBuffer = l_saveBuffer;
		this->Flags |= SAVEBACK;

		//double buffer
		WORD *l_dBufBuffer = (WORD *)AllocMem(RASSIZE(width, height)*(depth), MEMF_CLEAR|MEMF_CHIP);
		DBufPacket *l_dBufPackets  = (DBufPacket *)AllocMem(sizeof(DBufPacket), MEMF_CLEAR|MEMF_CHIP);
		m_Bob->DBuffer = l_dBufPackets;
		l_dBufPackets->BufBuffer = l_dBufBuffer;
		
		InitMasks(this);
	}
	~BlitterObject()
	{
		delete m_Bob;
		if(this->CollMask != 0)
		{
			FreeMem(CollMask, sizeof(WORD) * this->Width);
		}
		if(this->BorderLine != 0)
		{
			FreeMem(BorderLine, RASSIZE(this->Width<<4, this->Height));
		}
		if(m_Bob->SaveBuffer != 0)
		{
			FreeMem(m_Bob->SaveBuffer, RASSIZE(this->Width<<4, this->Height)*this->Depth);
			FreeMem(m_Bob->DBuffer->BufBuffer, RASSIZE(Width, Height)*(Depth));
			FreeMem(m_Bob->DBuffer, sizeof(DBufPacket));
		}
	}
	private:
	struct Bob *m_Bob;
	
};

class Entity
{
	public:
	CustomSprite *m_Sprite;

	Entity()
	{

	}
};
class PlayerEntity : public Entity{
	public:
	UBYTE m_Aggro;
	PlayerEntity()
	{
		m_Aggro = 0;
	}
};

class Trash : public Entity{
	public:
	PlayerEntity m_Targets[4];
	PlayerEntity *m_CurrentTarget;
	Trash()
	{

	}
	void PickTarget()
	{
		m_CurrentTarget = &m_Targets[0];//default tank
		UBYTE countOfZeros = 0;
		for(UBYTE target = 0; target < 4; target++)
		{
			if(m_CurrentTarget->m_Aggro < m_Targets[target].m_Aggro)
			{
				m_CurrentTarget = &m_Targets[target];
			}
			if(m_Targets[target].m_Aggro == 0)
			{
				countOfZeros++;
			}
		}
		//check 
		if(countOfZeros == 4)
		{
			//use distance
			int distance = (m_Targets[0].m_Sprite->X - m_Sprite->X);
			if(distance < 0)
				distance*=-1;
			for(UBYTE target = 0; target < 4; target++)
			{

			}
		}
	}
};

class Boss : public Trash{

};