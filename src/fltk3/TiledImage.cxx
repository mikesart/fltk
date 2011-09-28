//
// "$Id$"
//
// Tiled image code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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


#include <fltk3/run.h>
#include <fltk3/TiledImage.h>
#include <fltk3/draw.h>

/**
  The constructors create a new tiled image containing the specified image.
  Use a width and height of 0 to tile the whole window/widget.
*/
fltk3::TiledImage::TiledImage(fltk3::Image *i,	// I - Image to tile
                               int      W,	// I - Width of tiled area
			       int      H) :	// I - Height of tiled area
  fltk3::Image(W,H,0) {
  image_       = i;
  alloc_image_ = 0;

  if (W == 0) w(fltk3::w());
  if (H == 0) h(fltk3::h());
}
/**
  The destructor frees all memory and server resources that are used by
  the tiled image.
*/
  fltk3::TiledImage::~TiledImage() {
  if (alloc_image_) delete image_;
}


//
// 'fltk3::TiledImage::copy()' - Copy and resize a tiled image...
//

fltk3::Image *			// O - New image
fltk3::TiledImage::copy(int W,	// I - New width
                     int H) {	// I - New height
  if (W == w() && H == h()) return this;
  else return new fltk3::TiledImage(image_, W, H);
}


//
// 'fltk3::TiledImage::color_average()' - Blend colors...
//

void
fltk3::TiledImage::color_average(fltk3::Color c,	// I - Color to blend with
                              float    i) {	// I - Blend fraction
  if (!alloc_image_) {
    image_       = image_->copy();
    alloc_image_ = 1;
  }

  image_->color_average(c, i);
}


//
// 'fltk3::TiledImage::desaturate()' - Convert the image to grayscale...
//

void
fltk3::TiledImage::desaturate() {
  if (!alloc_image_) {
    image_       = image_->copy();
    alloc_image_ = 1;
  }

  image_->desaturate();
}


//
// 'fltk3::TiledImage::draw()' - Draw a shared image...
//

void
fltk3::TiledImage::draw(int X,	// I - Starting X position
                     int Y,	// I - Starting Y position
		     int W,	// I - Width of area to be filled
		     int H,	// I - Height of area to be filled
		     int cx,	// I - "Source" X position
		     int cy) {	// I - "Source" Y position
  if (!image_->w() || !image_->h()) return;
  if (W == 0) W = fltk3::w();
  if (H == 0) H = fltk3::h();

  fltk3::push_clip(X, Y, W, H);

  X += cx;
  Y += cy;

  X = X - (X % image_->w());
  Y = Y - (Y % image_->h());

  W += X;
  H += Y;

  for (int yy = Y; yy < H; yy += image_->h())
    for (int xx = X; xx < W; xx += image_->w())
      image_->draw(xx, yy);

  fltk3::pop_clip();
}


//
// End of "$Id$".
//