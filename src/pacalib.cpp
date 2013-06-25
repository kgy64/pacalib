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

PacaTarget::PacaTarget(int width, int height, float aspect):
    myWidth(width),
    myHeight(height),
    mySurface(width, height),
    myCairo(cairo_create(mySurface.get())),
    myScreenAspect(aspect)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 myFontDescription = pango_font_description_new();
 pango_font_description_set_family(myFontDescription, "serif");
 pango_font_description_set_weight(myFontDescription, PANGO_WEIGHT_BOLD);
 pango_font_description_set_absolute_size(myFontDescription, myHeight * PANGO_SCALE);
}

PacaTarget::~PacaTarget()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 pango_font_description_free(myFontDescription);
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

void PacaTarget::DrawText(double x, double y, const char * text, double size, double aspect)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 cairo_save(myCairo);

 double h = size * aspect;
 double v = size;

 h *= 0.6;
 v *= 0.8; // Heuristic values :-)

 cairo_scale(myCairo, h/myScreenAspect, -v);

 PangoLayout *layout = pango_cairo_create_layout(myCairo);

 pango_layout_set_text(layout, text, -1);
 pango_layout_set_font_description(layout, myFontDescription);
 cairo_new_path(myCairo);
 Move(x/h, -y/v);
 pango_cairo_update_layout(myCairo, layout);
 pango_cairo_layout_path(myCairo, layout);
 cairo_fill(myCairo);
 cairo_stroke(myCairo);

 g_object_unref(layout);

 cairo_restore(myCairo);
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
