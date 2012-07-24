//
// "$Id$"
//
// Font definitions for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2011 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

// Two internal fltk data structures:
//
// Fl_Fontdesc: an entry into the fltk3::font() table.  There is one of these
// for each fltk font number.
//
#ifndef FL_FONT_
#define FL_FONT_

#include <config.h>

#  if USE_XFT
typedef struct _XftFont XftFont;
#  elif !defined(WIN32) && !defined(__APPLE__)
#    include <fltk3/Xutf8.h>
#  endif // USE_XFT

/**
   This a structure for an actual system font, with junk to
   help choose it and info on character sizes.  Each Fl_Fontdesc has a
   linked list of these.  These are created the first time each system
   font/size combination is used.
*/
class Fl_Font_Descriptor {
public:
  /** linked list for this Fl_Fontdesc */
  Fl_Font_Descriptor *next;	
  fltk3::Fontsize size; /**< font size */
#ifndef FLTK3_DOXYGEN // don't bother with platorm dependant details in the doc. 
#  ifdef WIN32
  HFONT fid;
  int *width[64];
  TEXTMETRIC metr;
  int angle;
  FLTK3_EXPORT Fl_Font_Descriptor(const char* fontname, fltk3::Fontsize size);
#  elif defined(__APPLE_QUARTZ__)
  FLTK3_EXPORT Fl_Font_Descriptor(const char* fontname, fltk3::Fontsize size);
  ATSUTextLayout layout;
#     if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5
  CTFontRef fontref;
  // the unicode span is divided in 512 blocks of 128 characters
  float *width[512]; // array of arrays of character widths
#     endif
  ATSUStyle style;
  short ascent, descent, q_width;
  char *q_name;
#  elif USE_XFT
  XftFont* font;
  //const char* encoding;
  int angle;
  FLTK3_EXPORT Fl_Font_Descriptor(const char* xfontname, fltk3::Fontsize size, int angle);
#  else
  XUtf8FontStruct* font;	// X UTF-8 font information
  FLTK3_EXPORT Fl_Font_Descriptor(const char* xfontname);
#  endif
#  if HAVE_GL
  unsigned int listbase;// base of display list, 0 = none
#ifndef __APPLE_QUARTZ__
  char glok[64];
#endif // __APPLE_QUARTZ__
#  endif // HAVE_GL

  FLTK3_EXPORT ~Fl_Font_Descriptor();

#endif // FLTK3_DOXYGEN
};

//extern FLTK3_EXPORT Fl_Font_Descriptor *fltk3::fontsize; // the currently selected one

struct Fl_Fontdesc {
  const char *name;
  char fontname[128];	// "Pretty" font name
  Fl_Font_Descriptor *first;	// linked list of sizes of this style
#  ifndef WIN32
  char **xlist;		// matched X font names
  int n;		// size of xlist, negative = don't free xlist!
#  endif
};

namespace fltk3 {
  extern FLTK3_EXPORT Fl_Fontdesc *fonts; // the table
}

#  ifndef WIN32
// functions for parsing X font names:
namespace fltk3 {
  FLTK3_EXPORT const char* font_word(const char *p, int n);
}
FLTK3_EXPORT char *fl_find_fontsize(char *name);
#  endif

#endif

//
// End of "$Id$".
//