// This is oxl/vgui/impl/glut/vgui_glut_popup_impl.cxx
#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma implementation
#endif
//:
// \file
// \author fsm@robots.ox.ac.uk

#include "vgui_glut_popup_impl.h"
#include "vgui_glut_adaptor.h"
#include <vcl_iostream.h>
#include <vgui/vgui_macro.h>
#include <vgui/vgui_command.h>
#include <vgui/vgui_glut.h>

vgui_glut_popup_impl::vgui_glut_popup_impl() {
  //vcl_cerr << "popup_impl ctor\n";
  int old_id = glutGetMenu();
  menu_id = glutCreateMenu(command_func);
  //vcl_cerr << "created menu_id = " << menu_id << vcl_endl;

  //vgui_macro_warning << "before\n";
  if (old_id)
    glutSetMenu(old_id);
  //vgui_macro_warning << "after\n";
}


void vgui_glut_popup_impl::clear() {
  //vcl_cerr << "popup_impl::clear()\n";
  int old_id = glutGetMenu();
  glutSetMenu(menu_id);

  //
  unsigned n = glutGet(GLenum(GLUT_MENU_NUM_ITEMS));
  //vcl_cerr << "removing " << n << " items in menu\n";
  for (int i=n; i>=1; --i)
    glutRemoveMenuItem(i);

  //
  //vgui_macro_warning << "before\n";
  if (old_id)
    glutSetMenu(old_id);
  //vgui_macro_warning << "after\n";

  //
  for (unsigned i=0; i<subs.size(); ++i)
    delete static_cast<vgui_glut_popup_impl*>( subs[i] );
  subs.clear();
}


vgui_glut_popup_impl::~vgui_glut_popup_impl() {
  //vcl_cerr << "popup_impl dtor\n";
  clear();
  glutDestroyMenu( menu_id );
  menu_id = 0;
}

void vgui_glut_popup_impl::build(vgui_menu const &m) {
  //clear();

  // FIXME - this line here is to make sure the commands in the
  // given menu stay alive while the popup menu is active :
  tmp_menu = m;

  //vcl_cerr << "popup_impl::build : m = \n" << m << vcl_endl;
  this->build_internal(m);
}

void vgui_glut_popup_impl::build_internal(vgui_menu const &m) {
  int old_id = glutGetMenu();
  //vgui_macro_warning << "before\n";
  glutSetMenu(menu_id);
  //vgui_macro_warning << "after\n";

  for (unsigned i=0; i<m.size(); ++i) {
    if (m[i].is_command())
      glutAddMenuEntry( m[i].name.c_str(),
                        reinterpret_cast<long>(m[i].cmnd.as_pointer()));

    else if (m[i].is_submenu()) {
      vgui_glut_popup_impl *sub = new vgui_glut_popup_impl;
      sub->build_internal( * m[i].menu );
      glutAddSubMenu  (m[i].name.c_str(), sub->menu_id);
      subs.push_back(sub);
    }

    else if (m[i].is_toggle_button()) {
      vgui_command_toggle *c = static_cast<vgui_command_toggle*>( m[i].cmnd.as_pointer() );
      glutAddMenuEntry( (m[i].name + (c->state ? " (on)":" (off)")).c_str(),
                        reinterpret_cast<long>(m[i].cmnd.as_pointer()));
    }

    else if (m[i].is_separator()) {
      // do glut menus have real separators?
      if (0<i && i<m.size()-1) // ignore separators at start and end.
        glutAddMenuEntry( "----------------", 1);
    }

    else {
      vgui_macro_warning << "unknown menu item\n";
      vgui_macro_warning << "menu = \n" << m << vcl_endl;
    }
  }

  //vgui_macro_warning << "before\n";
  if (old_id)
    glutSetMenu(old_id); // restore
  //vgui_macro_warning << "after\n";
}

// When a menu item is selected, glut may have upset its internal
// state considerably and so it is advisable to wait for the next
// idle event before invoking the vgui_command. E.g. if the command
// were to start run_one_event()ing, heap corruption often results.
// To accomplish that, the action of selecting a menu item just
// queues the command for later execution.

#include "vgui_glut_impl.h"

void vgui_glut_popup_impl::command_func(int value)
{
  if (value == 0)
    vgui_macro_warning << "null command\n";
  else if (value == 1)
    vcl_cerr << "[that's a separator]\n";
  else {
    int win = glutGetWindow();
    vgui_glut_adaptor *a = vgui_glut_adaptor::get_adaptor(win);
    vgui_command      *c = reinterpret_cast<vgui_command *>(value);
    vgui_glut_impl_queue_command(a, c);
  }
}
