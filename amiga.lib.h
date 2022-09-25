#include <proto/exec.h>

struct IOStdReq *CreateStdIO(struct MsgPort *msgPort);
struct IORequest *CreateExtIO(struct MsgPort *ioReplayPort, ULONG size);
void DeleteStdIO(struct IOStdReq *);
void DeleteExtIO(struct IORequest *);
struct MsgPort *CreatePort(STRPTR name, LONG pri);
void DeletePort(struct MsgPort *mp);
BOOL SendMsgToPort(struct Message *message, STRPTR portname);
struct Task *CreateTask(char* name, ULONG pri, APTR initPC, ULONG stackSize);
void DeleteTask(Task *);
//void BeginIO();
//void NewList();
//struct IOStdReq *CreateStdIORequest(struct MsgPort *msgPort);