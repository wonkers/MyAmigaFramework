#include "amiga.lib.h"

struct IOStdReq *CreateStdIO(struct MsgPort *msgPort){
	struct IOStdReq *ioReq;
	if(!msgPort)
		return NULL;
	ioReq = (struct IOStdReq*)AllocMem(sizeof(struct IOStdReq), MEMF_PUBLIC|MEMF_CLEAR);
	if(!ioReq)
		return NULL;
	
	ioReq->io_Message.mn_Node.ln_Type = NT_MESSAGE;
	ioReq->io_Message.mn_Length = sizeof(struct IOStdReq);
	ioReq->io_Message.mn_ReplyPort = msgPort;

	return ioReq;
}
void DeleteStdIO(struct IOStdReq *ioReq)
{
	ioReq->io_Message.mn_Node.ln_Type = -1;
	ioReq->io_Message.mn_ReplyPort = (struct MsgPort *)-1;
	ioReq->io_Device = (struct Device *)-1;

	FreeMem(ioReq, sizeof(struct IOStdReq));
}

struct MsgPort *CreatePort(STRPTR name, LONG pri)
{
	int sigBit;
	struct MsgPort *mp;

	if((sigBit = AllocSignal(-1)) == -1)
		return NULL;
	mp = (struct MsgPort *)
		AllocMem((ULONG)sizeof(struct MsgPort), (ULONG)MEMF_CLEAR|MEMF_PUBLIC);

	if(!mp)
	{
		FreeSignal(sigBit);
		return NULL;
	}

	mp->mp_Node.ln_Name 	= (char *)name;
	mp->mp_Node.ln_Pri 		= pri;
	mp->mp_Node.ln_Type 	= NT_MSGPORT;
	mp->mp_Flags			= PA_SIGNAL;
	mp->mp_SigBit			= sigBit;
	mp->mp_SigTask			= (struct Task *)FindTask(0); //this task

	if(name)
		AddPort(mp);
	//else	
	//	NewList
	return mp;
}

void DeletePort(struct MsgPort *mp)
{
	if(mp->mp_Node.ln_Name)
		RemPort(mp);
	
	mp->mp_SigTask			= (struct Task*)-1;
	mp->mp_MsgList.lh_Head	= (struct Node *)-1;

	FreeSignal(mp->mp_SigBit);
	FreeMem(mp, (ULONG)sizeof(struct MsgPort));
}

BOOL SendMsgToPort(struct Message *message, STRPTR portname)
{
	struct MsgPort *port;
	Forbid();
		port = FindPort(portname);
		if(port)
			PutMsg(port, message);
	Permit();

	if(port)
		return TRUE;

	return FALSE;
}