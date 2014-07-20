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

AUTON_INTERFACE(PaCaLib::Target);

SYS_DEFINE_MODULE(DM_PACALIB);

using namespace PaCaLib;

Target::~Target()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
