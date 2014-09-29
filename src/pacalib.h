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

#include <glesly/target2d.h>
#include <Debug/Debug.h>

#include <Memory/Memory.h>
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

    enum Oper {
        OP_SOURCE,
        OP_OVER
    };

    enum LineCap {
        LINE_CAP_ROUND
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

        virtual void Move(float x, float y) =0;
        virtual void Line(float x, float y) =0;
        virtual void Arc(float xc, float yc, float r, float a1, float a2) =0;
        virtual void Close(void) =0;
        virtual void Clear(void) =0;
        virtual void SetLineWidth(float width) =0;
        virtual void SetLineCap(LineCap mode) =0;
        virtual void Fill(void) =0;
        virtual void Stroke(void) =0;
        virtual void SetColour(float r, float g, float b, float a) =0;

        inline void SetColour(float r, float g, float b)
        {
            SetColour(r, g, b, 1.0f);
        }

        inline void SetColour(const Colour & col)
        {
            SetColour(col.r, col.g, col.b, col.a);
        }

     private:
        SYS_DEFINE_CLASS_NAME("PaCaAndroid::Path");

    }; // class PaCaLib::Path

    typedef MEM::shared_ptr<Path> PathPtr;

    class Draw;
    typedef MEM::shared_ptr<Draw> DrawPtr;

    class Target: public Glesly::Target2D
    {
     public:
        virtual ~Target();
        static TargetPtr Create(int width, int height); // must be defined in the specialization

        virtual int GetLogicalWidth(void) const =0;
        virtual DrawPtr Draw(void) =0;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaLib::Target");

    }; // class Target

    class Draw
    {
     public:
        virtual void Scale(float w, float h) =0;
        virtual void SetLineWidth(float width) =0;
        virtual void Move(float x, float y) =0;
        virtual void Line(float x, float y) =0;
        virtual void SetLineCap(LineCap mode) =0;
        virtual void SetColour(float r, float g, float b, float a) =0;
        virtual void Rectangle(float x, float y, float w, float h) =0;
        virtual void Arc(float xc, float yc, float r, float a1, float a2) =0;
        virtual void SetTextOutlineColour(float r, float g, float b, float a = 1.0) =0;
        virtual void SetTextOutline(float outline) =0;
        virtual void Paint(void) =0;
        virtual void Paint(float alpha) =0;
        virtual void Operator(Oper op) =0;
        virtual PathPtr NewPath(void) =0;

        /// Draw an UTF8 text
        /*! \param  y       The Y coordinate of the origin
         *  \param  mode    Select the origin position
         *  \param  text    The UTF8 string to be drawn
         *  \param  size    Relative vertical size of the text. The value 1.0 will fit the whole target image
         *                  vertically.
         *  \param  aspect  The aspect ratio of the target image. It is necessary to keep the glyphs' aspect correctly.
         *  \note   The newlines of the string are handled. The given x/y position is the origin of the first line. */
        float DrawText(float x, float y, TextMode mode, const char * text, float size, float aspect = 1.0);

        inline void SetColour(float r, float g, float b)
        {
            SetColour(r, g, b, 1.0f);
        }

        inline void SetColour(const Colour & col)
        {
            SetColour(col.r, col.g, col.b, col.a);
        }

     private:
        SYS_DEFINE_CLASS_NAME("PaCaLib::Draw");

        virtual float DrawTextInternal(float x, float y, TextMode mode, const char * text, float size, float offset, float aspect = 1.0) =0;

    }; // class PaCaLib::Draw

} // namespace PaCaLib

inline std::ostream & operator<<(std::ostream & os, const PaCaLib::Colour & col)
{
 return col.toStream(os);
}

#endif /* __SRC_PACALIB_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
