//
// "$Id: image.cxx,v 1.11 2001/07/29 22:17:02 spitzak Exp $"
//
// Fl_Image test program for the Fast Light Tool Kit (FLTK).
//
// Notice that Fl_Image is for a static, multiple-reuse image, such
// as an icon or postage stamp.  Use fl_draw_image to go directly
// from an buffered image that changes often.
//
// Copyright 1998-1999 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#include <fltk/Fl.h>
#include <fltk/Fl_Window.h>
#include <fltk/Fl_Button.h>
#include <fltk/Fl_RGB_Image.h>
#include <stdio.h>
#include <stdlib.h>

int width = 75;
int height = 75;
uchar *image;

void make_image() {
  image = new uchar[3*width*height];
  uchar *p = image;
  for (int y = 0; y < height; y++) {
    double Y = double(y)/(height-1);
    for (int x = 0; x < width; x++) {
      double X = double(x)/(width-1);
      *p++ = uchar(255*((1-X)*(1-Y))); // red in upper-left
      *p++ = uchar(255*((1-X)*Y));	// green in lower-left
      *p++ = uchar(255*(X*Y));	// blue in lower-right
    }
  }
}

#include <fltk/Fl_Toggle_Button.h>

Fl_Toggle_Button *leftb,*rightb,*topb,*bottomb,*insideb;
Fl_Toggle_Button *tleftb,*trightb,*ttopb,*tbottomb;
Fl_Button *b;
Fl_Window *w;

void button_cb(Fl_Widget *,void *) {
  int i = 0;
  if (leftb->value()) i |= FL_ALIGN_LEFT;
  if (rightb->value()) i |= FL_ALIGN_RIGHT;
  if (topb->value()) i |= FL_ALIGN_TOP;
  if (bottomb->value()) i |= FL_ALIGN_BOTTOM;
  if (insideb->value()) i |= FL_ALIGN_INSIDE;
//   if (tleftb->value()) i |= FL_TEXT_ALIGN_LEFT;
//   if (trightb->value()) i |= FL_TEXT_ALIGN_RIGHT;
//   if (ttopb->value()) i |= FL_TEXT_ALIGN_TOP;
//   if (tbottomb->value()) i |= FL_TEXT_ALIGN_BOTTOM;

  b->clear_flag(FL_ALIGN_MASK);
//   b->clear_flag(FL_TEXT_ALIGN_MASK);
  b->set_flag(i);
  w->redraw();
}

#include <fltk/x.h>
#ifndef _WIN32
#include "list_visuals.cxx"
#endif

int visid = -1;
int arg(int argc, char **argv, int &i) {
  if (argv[i][1] == 'v') {
    if (i+1 >= argc) return 0;
    visid = atoi(argv[i+1]);
    i += 2;
    return 2;
  }
  return 0;
}

int main(int argc, char **argv) {

#ifndef _WIN32
  int i = 1;
  if (Fl::args(argc,argv,i,arg) < argc) {
    fprintf(stderr," -v # : use visual\n%s\n",Fl::help);
    exit(1);
  }

  if (visid >= 0) {
    fl_open_display();
    XVisualInfo templt; int num;
    templt.visualid = visid;
    fl_visual = XGetVisualInfo(fl_display, VisualIDMask, &templt, &num);
    if (!fl_visual) {
      fprintf(stderr, "No visual with id %d, use one of:\n",visid);
      list_visuals();
      exit(1);
    }
    fl_colormap = XCreateColormap(fl_display, RootWindow(fl_display,fl_screen),
				fl_visual->visual, AllocNone);
    fl_xpixel(FL_BLACK); // make sure black is allocated in overlay visuals
  } else {
    Fl::visual(FL_RGB);
  }
#endif

  Fl_Window window(400,400); ::w = &window;
  Fl_Button b(140,160,120,120,"Fl_RGB_Image"); ::b = &b;
  make_image();
  b.image(new Fl_RGB_Image(image, width, height));
  leftb = new Fl_Toggle_Button(50,75,50,25,"left");
  leftb->callback(button_cb);
  rightb = new Fl_Toggle_Button(100,75,50,25,"right");
  rightb->callback(button_cb);
  topb = new Fl_Toggle_Button(150,75,50,25,"top");
  topb->callback(button_cb);
  bottomb = new Fl_Toggle_Button(200,75,50,25,"bottom");
  bottomb->callback(button_cb);
  insideb = new Fl_Toggle_Button(250,75,50,25,"inside");
  insideb->callback(button_cb);
//   tleftb = new Fl_Toggle_Button(50,100,50,25,"text left");
//   tleftb->callback(button_cb);
//   trightb = new Fl_Toggle_Button(100,100,50,25,"text right");
//   trightb->callback(button_cb);
//   ttopb = new Fl_Toggle_Button(150,100,50,25,"text top");
//   ttopb->callback(button_cb);
//   tbottomb = new Fl_Toggle_Button(200,100,50,25,"text bottom");
//   tbottomb->callback(button_cb);

  window.resizable(window);
  window.end();
  window.show(argc, argv);
  return Fl::run();
}

//
// End of "$Id: image.cxx,v 1.11 2001/07/29 22:17:02 spitzak Exp $".
//
