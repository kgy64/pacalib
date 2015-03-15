/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     PaCaLib
 * Purpose:     Interface for my generic Software Rendering Library
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    Can be implemented for different backends.
 *              Currently available implementations are:
 *              - pacalib-linux:
 *                Implementation for a generic Linux, using Pango+Cairo.
 *                Repository path: git.teledigit.eu/devel/Linux/pacalib-pango.git
 *              - pacalib-android:
 *                Implementation for Android, using the built-in Java-based
 *                drawing functions.
 *                Repository path: git.teledigit.eu/devel/Android/pacalib-android.git
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __SRC_PACALIB_H_INCLUDED__
#define __SRC_PACALIB_H_INCLUDED__

#include <glesly/format.h>
#include <glesly/target2d.h>
#include <Memory/Memory.h>
#include <Debug/Debug.h>

#include <ostream>

SYS_DECLARE_MODULE(DM_PACALIB);

namespace PaCaLib
{
    class Target;
    typedef MEM::shared_ptr<Target> TargetPtr;

    void Initialize(void);

    struct Colour
    {
        Colour(float r, float g, float b, float a):
            r(r),
            g(g),
            b(b),
            a(a)
        {
        }

        float r;
        float g;
        float b;
        float a;

        inline std::ostream & toStream(std::ostream & os) const
        {
            return os << "{" << r << ", " << g << ", " << b << ", " << a << "}";
        }

    }; // struct Colour

    enum TextMode {
        LEFT    = 0,
        CENTER,
        RIGHT
    };

    enum LineCap {
        LINE_CAP_NONE,
        LINE_CAP_ROUND,
        LINE_CAP_SQUARE
    };

    enum ColourCompose {
        COLOUR_COMPOSE_DEFAULT,
        COLOUR_COMPOSE_ADD,
        COLOUR_COMPOSE_SUBTRACT,
        COLOUR_COMPOSE_OVERWRITE
    };

    class Path
    {
     protected:
        inline Path(void)
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
        }

     public:
        virtual ~Path()
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
        }

        enum DrawMode
        {
            DRAW_NONE               = 0,
            DRAW_STROKE             = 1,
            DRAW_FILL               = 2,
            DRAW_STROKE_AND_FILL    = 3
        };

        virtual void Move(float x, float y) =0;
        virtual void Line(float x, float y) =0;
        virtual void Arc(float xc, float yc, float r, float a1, float a2) =0;
        virtual void Bezier(float x, float y, float dx, float dy) =0;
        virtual void Close(void) =0;
        virtual void Clear(void) =0;
        virtual void Draw(DrawMode mode = DRAW_STROKE) =0;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaLib::Path");

    }; // class PaCaLib::Path

    typedef MEM::shared_ptr<PaCaLib::Path> PathPtr;

    class Draw;
    typedef MEM::shared_ptr<PaCaLib::Draw> DrawPtr;

    class Target: public Glesly::Target2D
    {
     public:
        virtual ~Target();
        static TargetPtr Create(int width, int height, Glesly::PixelFormat format = Glesly::FORMAT_DEFAULT); // must be defined in the specialization

        virtual int GetLogicalWidth(void) const
        {
            return GetWidth();
        }

        virtual DrawPtr Draw(void) =0;

        virtual Glesly::Target2D & operator=(const Glesly::Target2D & other)    // from Glesly::Target2D
        {
            return Glesly::Target2D::operator=(other);
        }

     private:
        SYS_DEFINE_CLASS_NAME("PaCaLib::Target");

    }; // class PaCaLib::Target

    class Draw
    {
     public:
        inline Draw(void)
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
        }

        virtual ~Draw()
        {
            SYS_DEBUG_MEMBER(DM_PACALIB);
        }

        virtual void Scale(float w, float h) =0;
        virtual void SetColourCompose(PaCaLib::ColourCompose mode = PaCaLib::COLOUR_COMPOSE_DEFAULT) =0;
        virtual void SetColour(float r, float g, float b, float a = 1.0f) =0;
        virtual void SetOutlineColour(float r, float g, float b, float a = 1.0f) =0;
        virtual void SetOutlineWidth(float outline) =0;
        virtual void SetLineWidth(float width) =0;
        virtual void SetLineCap(PaCaLib::LineCap mode) =0;
        virtual void Paint(void) =0;
        virtual PaCaLib::PathPtr NewPath(void) =0;

        /// Draw an UTF8 text
        /*! \param  x           The X coordinate of the origin (-1.0 ... +1.0)
         *  \param  y           The Y coordinate of the origin (-1.0 ... +1.0)
         *  \param  mode        Select the origin position
         *  \param  text        The UTF8 string to be drawn
         *  \param  size        Relative vertical size of the text. The value 1.0 will fit the whole target image vertically.
         *  \param  aspect      The aspect ratio of the target image. It is necessary to keep the glyphs' aspect correctly.
         *  \param  rotation    The rotation angle of the text, in radians.
         *  \param  shear_x     Horizontal shear (tangent alpha)
         *  \param  shear_y     Vertical shear (tangent alpha)
         *  \note   The newlines of the string are handled. The given x/y position is the origin of the first line. */
        float DrawText(float x, float y, PaCaLib::TextMode mode, const char * text, float size, float aspect = 1.0, float rotation = 0.0f, float shear_x = 0.0f, float shear_y = 0.0f);

        inline void SetColour(const Colour & col)
        {
            SetColour(col.r, col.g, col.b, col.a);
        }

        struct TextParams {

            const char * text;
            float x;
            float y;
            PaCaLib::TextMode mode;
            float size;
            float offset;
            float aspect;
            float rotation;
            float shear_x;
            float shear_y;

            void toStream(std::ostream & os) const;

        }; // struct PaCaLib::Draw::TextParams

        struct Distortion {

            inline Distortion(void):
                obj_size(1.0f),
                scene_height(1.0f),
                rotation(0.0f),
                obj_height(1.0f),
                shear_x(0.0f),
                shear_y(0.0f)
            {
            }

            /// Size correction of the object
            float obj_size;

            /// Size correction after rotation
            float scene_height;

            /// Addidional rotation of the object
            float rotation;

            /// Height correction of the object
            float obj_height;

            /// Horizontal shear of the object
            float shear_x;

            /// Verical; shear of the object
            float shear_y;

            void toStream(std::ostream & os) const;

        }; // struct PaCaLib::Draw::Distortion

        virtual float DrawTextInternal(const TextParams & params, const Distortion * distortion = nullptr) =0;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaLib::Draw");

    }; // class PaCaLib::Draw

} // namespace PaCaLib

static inline std::ostream & operator<<(std::ostream & os, const PaCaLib::Colour & col)
{
 return col.toStream(os);
}

std::ostream & operator<<(std::ostream & os, PaCaLib::ColourCompose mode);

static inline std::ostream & operator<<(std::ostream & os, const PaCaLib::Draw::TextParams & par)
{
 par.toStream(os);
 return os;
}

static inline std::ostream & operator<<(std::ostream & os, const PaCaLib::Draw::Distortion & dist)
{
 dist.toStream(os);
 return os;
}

#endif /* __SRC_PACALIB_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
