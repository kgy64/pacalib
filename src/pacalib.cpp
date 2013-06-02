/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     PaCaLib
 * Purpose:     Pnago Cairo C++ library on my OpenGL library, Glesly
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "pacalib.h"

SYS_DEFINE_MODULE(DM_PACALIB);

using namespace PaCaLib;

PacaTarget::PacaTarget(int width, int height):
    mySurface(width, height),
    myCairo(cairo_create(mySurface.get()))
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

PacaTarget::~PacaTarget()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 cairo_destroy(myCairo);
}

int PacaTarget::GetWidth(void) const
{
 return mySurface.getPhysicalWidth();
}

int PacaTarget::GetHeight(void) const
{
 return mySurface.getHeight();
}

const void * PacaTarget::GetPixelData(void) const
{
 return mySurface.getData();
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
