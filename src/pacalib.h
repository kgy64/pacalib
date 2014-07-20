/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     PaCaLib
 * Purpose:     Interface for my rendering library
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    Can be implemented for different backends.
 *              Currently available implementations are:
 *              - pacalib-linux:
 *                Implementation for a generic Linux, using Pango+Cairo.
 *                Repository path: git.teledigit.eu/devel/pacalib-linux.git
 *              - pacalib-android:
 *                Implementation for Android, using the built-in Java-based
 *                drawing functions.
 *                Repository path: git.teledigit.eu/devel/pacalib-android.git
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __SRC_PACALIB_H_INCLUDED__
#define __SRC_PACALIB_H_INCLUDED__

#include <glesly/target2d.h>
#include <Debug/Debug.h>

#include <boost/shared_ptr.hpp>
#include <ostream>

SYS_DECLARE_MODULE(DM_PACALIB);

namespace PaCaLib
{
    class Target;
    typedef boost::shared_ptr<Target> TargetPtr;

    void Initialize(void);

    struct Colour
    {
        Colour(double r, double g, double b, double a):
            r(r),
            g(g),
            b(b),
            a(a)
        {
        }

        double r;
        double g;
        double b;
        double a;

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

    class Target: public Glesly::Target2D
    {
     public:
        virtual ~Target();
        static TargetPtr Create(int width, int height);

        virtual int GetLogicalWidth(void) const =0;
        virtual void Scale(double w, double h) =0;
        virtual void Stroke(void) =0;
        virtual void Fill(void) =0;
        virtual void FillPreserve(void) =0;
        virtual void SetLineWidth(double width) =0;
        virtual void Move(double x, double y) =0;
        virtual void Line(double x, double y) =0;
        virtual void SetLineCap(LineCap mode) =0;
        virtual void SetColour(double r, double g, double b) =0;
        virtual void SetColour(double r, double g, double b, double a) =0;
        virtual void SetColour(const Colour & col) =0;
        virtual void Rectangle(double x, double y, double w, double h) =0;
        virtual void Arc(double xc, double yc, double r, double a1, double a2) =0;
        virtual void NewPath(void) =0;
        virtual void NewSubPath(void) =0;
        virtual void ClosePath(void) =0;
        virtual double DrawText(double x, double y, TextMode mode, const char * text, double size, double aspect = 1.0) =0;
        virtual void SetTextOutlineColour(double r, double g, double b, double a = 1.0) =0;
        virtual void SetTextOutline(double outline) =0;
        virtual void Paint(void) =0;
        virtual void Paint(double alpha) =0;
        virtual void Operator(Oper op) =0;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaLib::Target");

    }; // class Target

} // namespace PaCaLib

inline std::ostream & operator<<(std::ostream & os, const PaCaLib::Colour & col)
{
 return col.toStream(os);
}

#endif /* __SRC_PACALIB_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
