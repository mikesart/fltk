//
// "$Id: filename_setext.cxx,v 1.4.2.3.2.1 2001/11/26 00:15:06 easysw Exp $"
//
// Filename extension routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2001 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

// Replace .ext with new extension
// If no . in name, append new extension
// If new extension is null, act like it is ""

#include <FL/filename.H>
#include <string.h>

char *filename_setext(char *buf, int buflen, const char *ext) {
  char *q = (char *)filename_ext(buf);
  if (ext) {
    strncpy(q,ext,buflen - (q - buf) - 1);
    buf[buflen - 1] = '\0';
  } else *q = 0;
  return(buf);
}


//
// End of "$Id: filename_setext.cxx,v 1.4.2.3.2.1 2001/11/26 00:15:06 easysw Exp $".
//
