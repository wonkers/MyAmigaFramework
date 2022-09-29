#include "support/gcc8_c_support.h"
#include <proto/exec.h>
#include <hardware/intbits.h>
#include <graphics/collide.h>

#include "newdelete.h"
#include "graphics.h"
#include "CustomView.h"
#include "VerticalBlank.h"
#include "mouse.h"
#include "CustomViewPort.h"
#include "Hardware.h"

struct ExecBase *SysBase;
//volatile struct Custom *custom;
struct DosLibrary *DOSBase;
struct GfxBase *GfxBase;

//struct Interrupt *VBInterrupt;
#define CHAR(a) a-32


void interruptHandler() 
{
    Hardware::CustomChips->intreq=(1<<INTB_VERTB); 
    Hardware::CustomChips->intreq=(1<<INTB_VERTB); //reset vbl req. twice for a4000 bug.

	//sprData;
	Hardware::CustomChips->sprpt[0] = 0;//Make pointer disappear
	Hardware::CustomChips->sprpt[1] = 0;
	// custom->sprpt[2] = 0;
	// custom->sprpt[3] = 0;
	// custom->sprpt[4] = 0;
	// custom->sprpt[5] = 0;
	// custom->sprpt[6] = 0;
	// custom->sprpt[7] = 0;

}

WORD flameColors[3]={
	0xff0,0xf90,0xc20
};
UWORD vspFlame1[] __attribute__((section (".MEMF_CHIP"))) = {
	0x1000,0x0000,
	0x0004,0xa002,
	0x8003,0x5004,
	0x0000,0xe00a,
	0x800a,0xa00f,
	0x4004,0x4006
};
UWORD vspFlame2[] __attribute__((section (".MEMF_CHIP"))) = {
	0x4000,0x0000,
	0x0002,0x4000,
	0x9004,0x4002,
	0x4000,0x6002,
	0x2002,0x2003,
	0x4005,0x4007
};

UWORD Colours[16] = {0x000,0x682,0x462,0x8a2,0x442,0xa82,0x862,0x642,0xcc8,0xaa8,0x886,
	0x664,0xa40,0x468,0x046,0x0};//0xcc2

inline short MouseLeft(){return !((*(volatile UBYTE*)0xbfe001)&64);}	
inline short MouseRight(){return !((*(volatile UWORD*)0xdff016)&(1<<10));}
void DrawDB(CustomView *view, TexturedPolygon **);

int main() {

	SysBase = *((struct ExecBase**)4UL);
	
	GfxBase = (struct GfxBase *)OpenLibrary((CONST_STRPTR)"graphics.library",0);
	if (!GfxBase)
		Exit(0);

	DOSBase = (struct DosLibrary*)OpenLibrary((CONST_STRPTR)"dos.library", 0);
	if (!DOSBase)
		Exit(0);

	VBlankInterrupt *vblankInterrupt = new VBlankInterrupt();
	vblankInterrupt->SetCode(interruptHandler);
	
	CustomView *View = new CustomView(4, 320, 256, 0x0);
	View->AddViewPort(0, 0, 0, 320, 64, 4, 0);
	View->AddViewPort(1, 0, 66, 320, 64, 4, 0x0);
	View->AddViewPort(2, 32, 132, 256, 64, 4, 0x0);	
	LoadView(View);
	

	// for(int i = 0; i < 16; i++)
	// {
	// 	View->SetColor(0, i, Colours[i]);
	// 	View->SetColor(1, i, Colours[i]);
	// 	View->SetColor(2, i, Colours[i]);
	// }

	// GamePort *controller = new GamePort();	

	// controller->FlushBuffer();
	// controller->ReadMouseInput();

	// View->SetColor(1, 0xfff);
	// SetAPen(View->GetRastPort(), 1);
	// int length = TextLength(View->GetRastPort(), "Loading", strlen("Loading"));
	// View->WriteText(320/2-length/2, 256/2-View->GetRastPort()->TxBaseline, (STRPTR)"Loading");
	
	// Graphics::BitplaneImage *longbowman1;
	// WORD *longbowImage;
	// Graphics::BitplaneImage *tank;
	// WORD *tankImage;
	// Graphics::BitplaneImage *priest1;
	// WORD *priestImage;
	// Graphics::BitplaneImage *sorceror1;
	// WORD *sorcerorImage;

	// longbowman1 = new Graphics::BitplaneImage((UBYTE*)"dh0:longbowman1.iff");
	// longbowImage = (WORD*)AllocMem(sizeof(WORD)*17*4, MEMF_CHIP|MEMF_CLEAR);
	// tank = new Graphics::BitplaneImage((UBYTE*)"dh0:tank1.iff");
	// tankImage = (WORD*)AllocMem(sizeof(WORD)*17*4, MEMF_CHIP|MEMF_CLEAR);
	// priest1 = new Graphics::BitplaneImage((UBYTE*)"dh0:priest1.iff");
	// priestImage = (WORD*)AllocMem(sizeof(WORD)*17*4, MEMF_CHIP|MEMF_CLEAR);
	// sorceror1 = new Graphics::BitplaneImage((UBYTE*)"dh0:sorceror2.iff");
	// sorcerorImage = (WORD*)AllocMem(sizeof(WORD)*17*4, MEMF_CHIP|MEMF_CLEAR);

	// SetRast(View->GetRastPort(), 0);
	
	// for(int plane = 0; plane < DEPTH; plane++)
	// {
	// 	CopyMem(longbowman1->GetBitMap()->Planes[plane], &longbowImage[plane*17], 34);
	// 	CopyMem(tank->GetBitMap()->Planes[plane], &tankImage[plane*17], 34);
	// 	CopyMem(priest1->GetBitMap()->Planes[plane], &priestImage[plane*17], 34);
	// 	CopyMem(sorceror1->GetBitMap()->Planes[plane], &sorcerorImage[plane*17], 34);
	// }

	
	
	// BlitterObject *tankman = new BlitterObject(158, 130, (WORD*)tankImage, 4, 17, 16);
	// AddBob(tankman->VSBob, View->GetRastPort());

	// BlitterObject *priestman = new BlitterObject(140, 122, (WORD*)priestImage, 4, 17, 16);
	// AddBob(priestman->VSBob, View->GetRastPort());
	
	// BlitterObject *sorcerorman = new BlitterObject(124, 140, (WORD*)sorcerorImage, 4, 17, 16);
	// AddBob(sorcerorman->VSBob, View->GetRastPort());

	// BlitterObject *lbman = new BlitterObject(150, 150, (WORD*)longbowImage, 4, 17, 16);
	// AddBob(lbman->VSBob, View->GetRastPort());

	// Sprite *flames = new Sprite(124, 140, (WORD*)vspFlame1, 6, flameColors);
	// AddVSprite(flames, View->GetRastPort());

	// UWORD *images[2]{vspFlame1, vspFlame2};
	// UWORD frame = 0;

	// View->SetColor(0, 0x342);
	//View->SwitchBuffer();
	while(!MouseLeft())
	{
		View->Clear(0, 10);
		View->WriteText(0, 3, 0, View->GetRastPort(0)->TxBaseline, (STRPTR)"Text");
	
		View->Clear(1, 6);
		View->WriteText(1, 1, 9, 9, (STRPTR)"More Text");
		

		View->Clear(2, 12);	
		View->WriteText(2, 1, 	
			View->GetViewPort(2)->DWidth/2 - TextLength(View->GetRastPort(2), (STRPTR)"Even More Text", strlen("Even More Text"))/2, 
			View->GetViewPort(2)->DHeight/2, 
			(STRPTR)"Even More Text");
		View->DrawFilledPolygon(2, 5, 100, 10, 100, 20, 200, 20);
		View->DrawRectangle(2, 5, 30, 30, 60, 60);
		//Wait(1<<controller->msgPort->mp_SigBit);
		//GetMsg(controller->msgPort);
		

		// controller->ReadMouseInput();
		// lbman->X += (controller->inputEvent->ie_position.ie_xy.ie_x>>3);
		// lbman->Y += (controller->inputEvent->ie_position.ie_xy.ie_y>>3);
		// //controller->inputEvent->ie_Code GPCT_MOUSE


		// controller->inputEvent->ie_position.ie_xy.ie_x = 0;
		// controller->inputEvent->ie_position.ie_xy.ie_y = 0;

		View->DrawUpdate();
		
		// flames->ImageData = (WORD*)images[frame];
		// frame ^= 1;
	}
	delete vblankInterrupt;
	delete View;
	//delete customView;

	CloseLibrary((struct Library*)DOSBase);
	CloseLibrary((struct Library*)GfxBase);
}



