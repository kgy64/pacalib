/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     PaCaLib
 * Purpose:     Pnago Cairo C++ library on my OpenGL library, Glesly
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __SRC_PACALIB_H_INCLUDED__
#define __SRC_PACALIB_H_INCLUDED__

#include <pango/pangocairo.h>

#include <glesly/target2d.h>
#include <Debug/Debug.h>

SYS_DECLARE_MODULE(DM_PACALIB);

namespace PaCaLib
{
    /// A wrapper for cairo_surface_t
    class PacaSurface
    {
     public:
        inline PacaSurface(int width, int height)
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
            mySurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
        };

        inline virtual ~PacaSurface()
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
            cairo_surface_destroy(mySurface);
        }

        inline cairo_surface_t * get(void)
        {
            return mySurface;
        }

        inline const cairo_surface_t * get(void) const
        {
            return mySurface;
        }

        inline unsigned char * getData(void)
        {
            return cairo_image_surface_get_data(get());
        }

        inline const unsigned char * getData(void) const
        {
            return cairo_image_surface_get_data(const_cast<cairo_surface_t*>(get()));
        }

        inline int getWidth(void) const
        {
            return cairo_image_surface_get_width(const_cast<cairo_surface_t*>(get()));
        }

        inline int getPhysicalWidth(void) const
        {
            return cairo_image_surface_get_stride(const_cast<cairo_surface_t*>(get())) / 4; // Note: always using 32-bit pixels
        }

        inline int getHeight(void) const
        {
            return cairo_image_surface_get_height(const_cast<cairo_surface_t*>(get()));
        }

     private:
        SYS_DEFINE_CLASS_NAME("PaCaLib::PacaSurface");

        cairo_surface_t * mySurface;

    }; // class PacaSurface;

    /// A wrapper for cairo_t
    class PacaTarget: public Glesly::Target2D
    {
     public:
        PacaTarget(int width, int height, float aspect = 1.0f);
        virtual ~PacaTarget();

        virtual int GetWidth(void) const;
        virtual int GetHeight(void) const;
        virtual const void * GetPixelData(void) const;

        inline int GetLogicalWidth(void) const
        {
            return mySurface.getWidth();
        }

        inline void Stroke(void)
        {
            cairo_stroke(myCairo);
        }

        inline void Fill(void)
        {
            cairo_fill(myCairo);
        }

        inline void SetLineWidth(double width)
        {
            cairo_set_line_width(myCairo, width * myWidth);
        }

        inline void Move(double x, double y)
        {
            cairo_move_to(myCairo, x * myWidth, y * myHeight);
        }

        inline void Line(double x, double y)
        {
            cairo_line_to(myCairo, x * myWidth, y * myHeight);
        }

        inline void SetColour(double r, double g, double b)
        {
            cairo_set_source_rgb(myCairo, r, g, b);
        }

        inline void SetColour(double r, double g, double b, double a)
        {
            cairo_set_source_rgba(myCairo, r, g, b, a);
        }

        inline void DrawRectangle(double x, double y, double w, double h)
        {
            cairo_rectangle(myCairo, x * myWidth, y * myHeight, w * myWidth, h * myHeight);
        }

        void DrawText(double x, double y, const char * text, double size, double aspect = 1.0);

        inline void Paint(void)
        {
            cairo_paint(myCairo);
        }

        inline void Paint(double alpha)
        {
            cairo_paint_with_alpha(myCairo, alpha);
        }

     protected:
        double myWidth;

        double myHeight;

        PacaSurface mySurface;

        cairo_t * myCairo;

        PangoFontDescription *myFontDescription;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaLib::PacaTarget");

        float myScreenAspect;

    }; // class PacaTarget

} // namespace PaCaLib

#endif /* __SRC_PACALIB_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
