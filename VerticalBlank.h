#include <exec/types.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <hardware/intbits.h>
#include <exec/interrupts.h>
#include <hardware/custom.h>

struct Interrupt *VBInterrupt;

class VBlankInterrupt : public Interrupt
{
    private:
    //volatile struct Custom *custom;
    public:
    VBlankInterrupt()
    {
        this->is_Node.ln_Type = NT_INTERRUPT;
        this->is_Node.ln_Pri = -60;
        //SetCode((void (*)())&VBlankInterrupt::interruptHandler);
    }
    void SetCode( VOID (*routine)())
    {
        this->is_Code = routine;
        AddIntServer(INTB_VERTB, this);
    }
    void Start()
    {
        AddIntServer(INTB_VERTB, this);
    }
    ~VBlankInterrupt()
    {
        RemIntServer(INTB_VERTB, this);
    }

    // void interruptHandler() 
    // {
    //     custom->intreq=(1<<INTB_VERTB); 
    //     custom->intreq=(1<<INTB_VERTB); //reset vbl req. twice for a4000 bug.

    //     custom->sprpt[2] = 0;
    //     custom->sprpt[3] = 0;
    //     custom->sprpt[4] = 0;
    //     custom->sprpt[5] = 0;
    // }

	
};

