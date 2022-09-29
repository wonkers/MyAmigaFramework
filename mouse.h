#pragma once
#include <proto/exec.h>
#include <exec/exec.h>
#include <devices/gameport.h>
#include <devices/inputevent.h>
#include "newdelete.h"
#include "amiga.lib.h"

class GamePort
{
public:
	IOStdReq *game_io_msg = NULL;
	MsgPort *msgPort;
	InputEvent *inputEvent;
	GamePortTrigger gamePortTrigger;
	BYTE controllerType;
	GamePort()
	{		
		inputEvent = new InputEvent();
		gamePortTrigger.gpt_Keys = GPTF_DOWNKEYS + GPTF_UPKEYS;
		gamePortTrigger.gpt_Timeout = 0;
		gamePortTrigger.gpt_XDelta = 16;
		gamePortTrigger.gpt_YDelta = 16;
		controllerType = 0;

		msgPort = CreatePort((STRPTR)"RDM_game_port", 0);
		game_io_msg = CreateStdIO(msgPort);
		OpenDevice("gameport.device", 0, (IORequest*)game_io_msg, 0);
		
		SetControllerType(GPCT_MOUSE);
		SetTriggerConditions();
		
	}
	~GamePort()
	{
		delete inputEvent;
		FreeController();
		CloseDevice((IORequest*)game_io_msg);
		DeleteStdIO(game_io_msg);
		
	}
	void ReadMouseInput()
	{
		game_io_msg->io_Command = GPD_READEVENT;
		game_io_msg->io_Length = (LONG)sizeof(InputEvent);
		game_io_msg->io_Data = inputEvent;
		SendIO((IORequest*)game_io_msg);
	}
	
	void FlushBuffer()
	{
		game_io_msg->io_Command = CMD_CLEAR;
		game_io_msg->io_Flags = IOF_QUICK;
		DoIO((IORequest*)game_io_msg);
	}

private:
	void GetControllerType()
	{
		Forbid();
		game_io_msg->io_Command = GPD_ASKCTYPE;
		game_io_msg->io_Length = 1;
		game_io_msg->io_Flags = IOF_QUICK;
		game_io_msg->io_Data = (APTR)&controllerType;
		DoIO((IORequest*)game_io_msg);
		Permit();

	}
	void SetControllerType(BYTE t)
	{
		GetControllerType();
		switch(controllerType)
		{
			case GPCT_MOUSE:
			case GPCT_ABSJOYSTICK:
			case GPCT_RELJOYSTICK:
			FreeController();
			break;
		}
		BYTE type = GPCT_MOUSE;
		Forbid();
		game_io_msg->io_Command = GPD_SETCTYPE;
		game_io_msg->io_Length = 1;
		game_io_msg->io_Flags = IOF_QUICK;
		game_io_msg->io_Data = (APTR)&type;
		DoIO((IORequest*)game_io_msg);
		Permit();

	}
	void FreeController()
	{
		Forbid();
		game_io_msg->io_Command = GPD_SETCTYPE;
		game_io_msg->io_Length = 1;
		game_io_msg->io_Flags = IOF_QUICK;
		game_io_msg->io_Data = GPCT_NOCONTROLLER;
		DoIO((IORequest*)game_io_msg);
		Permit();
	}
	void SetTriggerConditions()
	{
		Forbid();
		game_io_msg->io_Command = GPD_SETTRIGGER;
		game_io_msg->io_Length = (LONG)sizeof(GamePortTrigger);
		game_io_msg->io_Data = (APTR)&gamePortTrigger;
		DoIO((IORequest*)game_io_msg);
		Permit();
	}
	void CheckMove()
	{

	}
};