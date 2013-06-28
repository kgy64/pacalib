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
 // The font size is "2", because the output range is is -1 ... +1
 pango_font_description_set_absolute_size(myFontDescription, 2*PANGO_SCALE);

 // To be compatible with the OpenGL backend, set the coordinate ranges to -1.0 ... +1.0:
 cairo_translate(myCairo, myWidth/2, myHeight/2);
 Scale(myWidth/2, -myHeight/2);
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

void PacaTarget::DrawText(double x, double y, TextMode mode, const char * text, double size, double aspect)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 cairo_save(myCairo);

 double h = 0.70;
 double v = 0.70; // Heuristic values :-)

 h *= size * aspect;
 v *= size;

 Scale(h, v);

 PangoLayout *layout = pango_cairo_create_layout(myCairo);

 pango_layout_set_text(layout, text, -1);
 pango_layout_set_font_description(layout, myFontDescription);

 NewPath();

 pango_cairo_update_layout(myCairo, layout);
 int width, height;
 pango_layout_get_size(layout, &width, &height);
 double text_width_half = (double)width / (2*PANGO_SCALE);
 double text_height_half = (double)height / (2*PANGO_SCALE);

 double x_pos = x/h - text_width_half;
 double y_pos = y/v - text_height_half;

 switch (mode) {
    case LEFT:
        x_pos += text_width_half;
    break;
    case CENTER:
        // Nothing to do
    break;
    case RIGHT:
        x_pos -= text_width_half;
    break;
 }
 Move(x_pos, y_pos);
 pango_cairo_layout_path(myCairo, layout);

 Fill();
 Stroke();

 g_object_unref(layout);

 cairo_restore(myCairo);
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
