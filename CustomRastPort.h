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

    public:
        CustomRastPort()
        {

        }
};