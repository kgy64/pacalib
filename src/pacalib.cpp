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

Target::~Target()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

float Target::DrawText(float x, float y, TextMode mode, const char * text, float size, float aspect)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 SYS_DEBUG(DL_INFO1, "Drawing text: '" << text << "', mode=" << (int)mode << ", size=" << size << ", aspect=" << aspect);

 const char * newline = strchr(text, '\n');
 if (!newline) {
    SYS_DEBUG(DL_INFO2, "Drawing text: one-line text.");
    // Single-line text, use offset=0:
    return DrawTextInternal(x, y, mode, text, size, 0.0f, aspect);
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

 float offset = ((int)lines.size() - 1) / 2.0f;
 float result = 0.0f;
 for (std::vector<uint8_t *>::const_iterator i = lines.begin(); i < lines.end(); ++i) {
    const char * line = reinterpret_cast<const char *>(*i);
    result += DrawTextInternal(x, y, mode, line, size, offset, aspect);
    offset -= 1.0f;
 }

 return result;
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
