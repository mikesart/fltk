//
// "$Id: Fl_Tabs.cxx,v 1.19 1999/10/13 16:04:03 vincent Exp $"
//
// Tab widget for the Fast Light Tool Kit (FLTK).
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

// This is the "file card tabs" interface to allow you to put lots and lots
// of buttons and switches in a panel, as popularized by many toolkits.

// Each child widget is a card, and it's label() is printed on the card tab.
// Clicking the tab makes that card visible.

#include <FL/Fl.H>
#include <FL/Fl_Tabs.H>
#include <FL/fl_draw.H>

#define BORDER 10
#define TABSLOPE 8

// return the left edges of each tab (plus a fake left edge for a tab
// past the right-hand one).  These position are actually of the left
// edge of the slope.  They are either seperated by the correct distance
// or by TABSLOPE or by zero.
// Return value is the index of the selected item.

int Fl_Tabs::tab_positions(int* p, int* w) {
  int selected = 0;
  Fl_Widget*const* a = array();
  int i;
  p[0] = 0;
  for (i=0; i<children(); i++) {
    Fl_Widget* o = *a++;
    if (o == value_) selected = i;
    if (o->label()) {
      int wt = 0; int ht = 0; o->measure_label(wt,ht);
      w[i] = wt+TABSLOPE;
    } else 
      w[i] = 2*TABSLOPE;
    p[i+1] = p[i]+w[i];
  }
  int r = this->w()-TABSLOPE-1;
  if (p[i] <= r) return selected;
  // uh oh, they are too big:
  // pack them against right edge:
  p[i] = r;
  for (i = children(); i--;) {
    int l = r-w[i];
    if (p[i+1]-TABSLOPE < l) l = p[i+1]-TABSLOPE;
    if (p[i] <= l) break;
    p[i] = l;
    r -= TABSLOPE;
  }
  // pack them against left edge and truncate width if they still don't fit:
  for (i = 0; i<children(); i++) {
    if (p[i] >= i*TABSLOPE) break;
    p[i] = i*TABSLOPE;
    int W = this->w()-1-TABSLOPE*(children()-i) - p[i];
    if (w[i] > W) w[i] = W;
  }
  // adjust edges according to visiblity:
  for (i = children(); i > selected; i--) {
    p[i] = p[i-1]+w[i-1];
  }
  return selected;
}

// return space needed for tabs.  Negative to put them on the bottom:
int Fl_Tabs::tab_height() {
  int H = h();
  int H2 = y();
  Fl_Widget*const* a = array();
  for (int i=children(); i--;) {
    Fl_Widget* o = *a++;
    if (o->y() < y()+H) H = o->y()-y();
    if (o->y()+o->h() > H2) H2 = o->y()+o->h();
  }
  H2 = y()+h()-H2;
  if (H2 > H) {
    H = H2-box()->dy();
    return (H <= 0) ? 0 : -H-1;
  } else {
    H = H-box()->dy();
    return (H <= 0) ? 0 : H+1;
  }
}

// this is used by fluid to pick tabs:
Fl_Widget *Fl_Tabs::which(int event_x, int event_y) {
  int H = tab_height();
  if (H < 0) {
    if (event_y > y()+h() || event_y < y()+h()+H) return 0;
  } else {
    if (event_y > y()+H || event_y < y()) return 0;
  }
  if (event_x < x()) return 0;
  int p[128], w[128];
  int selected = tab_positions(p, w);
  int d = (event_y-(H>=0?y():y()+h()))*TABSLOPE/H;
  for (int i=0; i<children(); i++) {
    if (event_x < x()+p[i+1]+(i<selected ? TABSLOPE-d : d)) return child(i);
  }
  return 0;
}

int Fl_Tabs::handle(int event) {

  Fl_Widget *o;

  switch (event) {

  case FL_PUSH: {
    int H = tab_height();
    if (H >= 0) {
      if (Fl::event_y() > y()+H) goto DEFAULT;
    } else {
      if (Fl::event_y() < y()+h()+H) goto DEFAULT;
    }}
  case FL_DRAG:
  case FL_RELEASE:
    o = which(Fl::event_x(), Fl::event_y());
    if (event == FL_RELEASE && !Fl::pushed()) {push(0); if (o) value(o);}
    else push(o);
    return 1;

  default:
  DEFAULT:
    value(); // initialize value & visibility if value_ == 0
    return Fl_Group::handle(event);

  }
}

int Fl_Tabs::push(Fl_Widget *o) {
  if (push_ == o) return 0;
  if (push_ && push_ != value_ || o && o != value_) damage(FL_DAMAGE_EXPOSE);
  push_ = o;
  return 1;
}

Fl_Widget* Fl_Tabs::value() {
  Fl_Widget *v = value_;
  if (!v) {
    // If value() has not been called, find first visible() child:
    Fl_Widget*const* a = array();
    for (int i=children(); i--;) {
      Fl_Widget* o = *a++;
      if (v) o->hide();
      else if (o->visible()) v = o;
    }
    if (!v) return 0; // no children...
    value_ = v;
  }
  return v;
}

int Fl_Tabs::value(Fl_Widget *o) {
  if (value_ == o) return 0;
  Fl_Widget* oldvalue = value_;
  value_ = o;
  if (o) o->show();
  if (oldvalue) oldvalue->hide();
  value_ = o;
  redraw();
  do_callback();
  return 1;
}

enum {LEFT, RIGHT, SELECTED};

bool fl_tabs_fake = 0;
void Fl_Tabs::draw() {
  Fl_Widget *v = value();

  // Make sure the tab's label isn't drawn normally
  const char* l=0;
  if (v) {l=v->label();v->label(0);}

  int H = tab_height();
  if (damage() & FL_DAMAGE_ALL) { // redraw the entire thing:
    if (v) draw_child(*v);
  } else { // redraw the child
    if (v) update_child(*v);
  }

  // restore the tab's label
  if (v) v->label(l);

  // draw the tabs if needed:
  if (v && (damage() & (FL_DAMAGE_EXPOSE|FL_DAMAGE_ALL))) {
    int p[128]; int w[128];
    int selected = tab_positions(p,w);
    int i;

    // draw the parent's box under the tabs
    if (damage()&FL_DAMAGE_ALL) {
      fl_clip(x(), y()+(H>=0?0:h()+H), p[children()-1]+w[children()-1]+TABSLOPE, (H>=0?H:-H));
      parent()->draw_group_box();
      fl_pop_clip();
    }

    Fl_Widget*const* a = array();
    for (i=0; i<selected; i++)
      draw_tab(x()+p[i], x()+p[i+1], w[i], H, a[i], LEFT);
    for (i=children()-1; i > selected; i--)
      draw_tab(x()+p[i], x()+p[i+1], w[i], H, a[i], RIGHT);
    i = selected;
    draw_tab(x()+p[i], x()+p[i+1], w[i], H, a[i], SELECTED);

    // The tabs widget build itself the clip_out region with a special shape
    if (damage()&FL_DAMAGE_ALL) {
      fl_clip_out(x(), y()+(H>=0?0:h()+H), p[children()-1]+w[children()-1]+TABSLOPE, (H>=0?H:-H));
      fl_clip_out(x(), y()+(H>0?H:0), this->w(), h()-(H>=0?H:-H-1));
    }
  }
}

// Call the draw method, handle the clip out
void Fl_Tabs::draw_n_clip()
{
  clear_damage(FL_DAMAGE_ALL);
  draw();
  clear_damage();
  // clip out is done in the draw() method ...
}

void Fl_Tabs::draw_tab(int x1, int x2, int W, int H, Fl_Widget* o, int what) {
  if (x2 < x1+W) {
    if (what == LEFT) {
      if (x1+W < x2+TABSLOPE) x2 = x1+W;
      else x2 += TABSLOPE;
    } else {
      if (x1+W < x2+TABSLOPE) x1 = x2-W;
      else x1 -= TABSLOPE;
    }
  }
  int sel = (what == SELECTED);
  fl_color(o->color());
  if (H >= 0) {
    fl_polygon(x1, y()+H+sel, x1+TABSLOPE, y(), x2, y(),
	       x2+TABSLOPE, y()+H+sel);
    fl_color(!sel && o==push_ ? FL_DARK3 : FL_LIGHT3);
    fl_line(x1, y()+H, x1+TABSLOPE, y(), x2, y());
    if (sel) {
      if (x1>x()) fl_xyline(x(), y()+H, x1);
      if (x2+TABSLOPE < x()+w()-1) fl_xyline(x2+TABSLOPE, y()+H, x()+w()-1);
    }
    fl_color(!sel && o==push_ ? FL_LIGHT3 : FL_DARK3);
    fl_line(x2, y(), x2+TABSLOPE, y()+H);
  } else {
    fl_polygon(x1, y()+h()+H-sel, x1+TABSLOPE, y()+h(), x2, y()+h(),
	       x2+TABSLOPE, y()+h()+H-sel);
    fl_color(!sel && o==push_ ? FL_LIGHT3 : FL_DARK3);
    fl_line(x1+TABSLOPE, y()+h()-1, x2, y()+h()-1, x2+TABSLOPE, y()+h()+H);
    if (sel) {
      if (x1>x()) fl_xyline(x(), y()+h()+H, x1);
      if (x2+TABSLOPE < x()+w()-1) fl_xyline(x2+TABSLOPE, y()+h()+H,x()+w()-1);
    }
    fl_color(!sel && o==push_ ? FL_DARK3 : FL_LIGHT3);
    fl_line(x1, y()+h()+H, x1+TABSLOPE, y()+h()-1);
  }
  if (W > TABSLOPE)
    o->draw_label(what==LEFT ? x1+TABSLOPE : x2-W+TABSLOPE,
		  y()+(H<0?h()+H-3:0), W-TABSLOPE,
		  (H<0?-H:H)+3, FL_ALIGN_CENTER);
}

// this is private as there is no need for themes to alter this:
static Fl_Style tabs_style = {
  FL_THIN_UP_BOX // box
  // rest is zero and inherited from parent's style
};

Fl_Style_Definer fl_tabs_style_definer("tabs", tabs_style);

Fl_Tabs::Fl_Tabs(int X,int Y,int W, int H, const char *l)
  : Fl_Group(X,Y,W,H,l)
{
  style(tabs_style);
  type(FL_TABS);
  value_ = push_ = 0;
}

//
// End of "$Id: Fl_Tabs.cxx,v 1.19 1999/10/13 16:04:03 vincent Exp $".
//
