// This is oxl/vgui/vgui_active_visible.h
//:
// \file
// \author Philip C. Pritchett, Robotics Research Group, University of Oxford
// \date   25 Jan 00
// \brief Tableau which can toggle between visable/invisable and active/inactive
//        Contains classes: vgui_active_visable, vgui_active_visable_new.
//
// \verbatim
//  Modifications:
//    25-JAN-2000 P.Pritchett - Initial version.
//    07-AUG-2002 K.Y.McGaul - Changed to and added Doxygen style comments.
// \endverbatim

#ifndef vgui_active_visible_h_
#define vgui_active_visible_h_
#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma interface
#endif

#include "vgui_active_visible_sptr.h"
#include "vgui_wrapper_tableau.h"

//: Tableau which can toggle between visable/invisable and active/inactive.
//
//  By calling toggle_active and toggle_visable this tableau
//  (or rather a tableau derived from it) can appear visable or
//  invisable, and active or inactive.
//
//  Functions to toggle these can appear on the popup menu for the tableau.
//
//  When the tableau is invisable, all vgui_DRAW, vgui_CLEAR and
//  vgui_DRAW_OVERLAY events are passed to the child tableau.
//
//  When the tableau is inactive, all events apart from draw events
//  are passed directly to the child tableau.
class vgui_active_visible : public vgui_wrapper_tableau
{
 public:
  //: Constructor - don't use this, use vgui_active_visable_new.
  vgui_active_visible(vgui_tableau_sptr const &, bool name_in_menu = false);

  //: Return the name of this tableau ('vgui_active_visable').
  vcl_string type_name() const;

  //: Handle all events sent to this tableau.
  //  If visable, then use drawing events, else pass them to the child tableau.
  //  If active, use non-drawing events, else pass them to the child tableau.
  bool handle(const vgui_event&);

  //: Add option to the popup menu to toggle active and visable.
  void add_popup(vgui_menu&);

  //: Toggle between active (using events) and inactive (passing events on).
  void toggle_active();

  //: Toggle between visable (using drawing events) and invisable.
  void toggle_visible();

  //: Return true if the tableau is active.
  bool get_active() const { return active_; }

  //: Return true if the tableau is visable.
  bool get_visible() const { return visible_; }

  //: True makes the tableau active, false makes it inactive.
  void set_active(bool v) { active_ = v; }

  //: True makes the tableau visable, false makes it invisable.
  void set_visible(bool v) { visible_ = v; }

 protected:
 ~vgui_active_visible();
  bool active_;
  bool visible_;
  bool name_in_menu_;
};

//: Create a smart-pointer to a vgui_active_visable tableau.
struct vgui_active_visible_new : public vgui_active_visible_sptr
{
  typedef vgui_active_visible_sptr base;
  vgui_active_visible_new(vgui_tableau_sptr const &a, bool name=false)
    : base(new vgui_active_visible(a,name)) { }
};

#endif // vgui_active_visible_h_
