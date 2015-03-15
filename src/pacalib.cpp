/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     PaCaLib
 * Purpose:     Interface for my rendering library
 * Author:      György Kövesdi (kgy@teledigit.eu)
 * Licence:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "pacalib.h"

#include <vector>
#include <string.h>
#include <alloca.h>

SYS_DEFINE_MODULE(DM_PACALIB);

using namespace PaCaLib;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class Target:                                                                     *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Target::~Target()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     class Draw:                                                                       *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

float Draw::DrawText(float x, float y, TextMode mode, const char * text, float size, float aspect, float rotation, float shear_x, float shear_y)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 SYS_DEBUG(DL_INFO1, "Drawing text: '" << text << "', mode=" << (int)mode << ", size=" << size << ", aspect=" << aspect);

 PaCaLib::Draw::TextParams par = { text, x, y, mode, size, 0.0f, aspect, rotation, shear_x, shear_y };

 const char * newline = strchr(text, '\n');
 if (!newline) {
    SYS_DEBUG(DL_INFO2, "Drawing text: one-line text.");
    // Single-line text, use offset=0:
    return DrawTextInternal(par);
 }

 uint8_t * myText = reinterpret_cast<uint8_t *>(alloca(strlen(text)+1));
 const uint8_t * originalText = reinterpret_cast<const uint8_t *>(text);
 std::vector<uint8_t *> lines;
 lines.push_back(myText);
 uint8_t * p;
 for (p = myText; *originalText; ++p, ++originalText) {
    if (*originalText >= 0x20) {
        *p = *originalText;
    } else {
        switch (*originalText) {
            case '\n':
                *p = 0;
                lines.push_back(p+1);
            break;
        }
    }
 }
 *p = 0;

 SYS_DEBUG(DL_INFO2, "Drawing text: lines=" << lines.size());

 par.offset = ((int)lines.size() - 1) / 2.0f;
 float result = 0.0f;
 for (std::vector<uint8_t *>::const_iterator i = lines.begin(); i < lines.end(); ++i) {
    par.text = reinterpret_cast<const char *>(*i);
    result += DrawTextInternal(par);
    par.offset -= 1.0f;
 }

 return result;
}

void Draw::TextParams::toStream(std::ostream & os) const
{
 os << "{text=\"" << text << "\", x=" << x << ", y=" << y << ", mode=" << (int)mode << ", size=" << size << ", offset=" << offset << ", aspect=" << aspect << ", rot=" << rotation << ", sx=" << shear_x << ", sy=" << shear_y << "}";
}

void Draw::Distortion::toStream(std::ostream & os) const
{
 os << "{shear_x" << ", sy=" << shear_y << "}";
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *     Generic functions:                                                                *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

std::ostream & operator<<(std::ostream & os, ColourCompose mode)
{
 switch (mode) {
    case COLOUR_COMPOSE_DEFAULT:
        os << "COLOUR_COMPOSE_DEFAULT";
    break;
    case COLOUR_COMPOSE_ADD:
        os << "COLOUR_COMPOSE_ADD";
    break;
    case COLOUR_COMPOSE_SUBTRACT:
        os << "COLOUR_COMPOSE_SUBTRACT";
    break;
    case COLOUR_COMPOSE_OVERWRITE:
        os << "COLOUR_COMPOSE_OVERWRITE";
    break;
    default:
        os << "unknown colour compose: " << (int)mode;
    break;
 }
 return os;
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
