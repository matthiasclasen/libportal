/*
 * Copyright (C) 2018, Matthias Clasen
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <portal.h>
#include <gtk/gtk.h>

#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#endif
#ifdef GDK_WINDOWING_WAYLAND
#include <gdk/gdkwayland.h>
#endif

G_BEGIN_DECLS

static inline void _xdp_parent_exported_wayland (GdkWindow *window,
                                                 const char *handle,
                                                 gpointer data)

#ifndef __GI_SCANNER__
{
  XdpParent *parent = data;
  g_autofree char *handle_str = g_strdup_printf ("wayland:%s", handle);
  parent->callback (parent, handle_str, parent->data);
}
#else
;
#endif

static inline gboolean _xdp_parent_export_gtk (XdpParent *parent,
                                               XdpParentExported callback,
                                               gpointer data)
#ifndef __GI_SCANNER__
{
  
#ifdef GDK_WINDOWING_X11
  if (GDK_IS_X11_DISPLAY (gtk_widget_get_display (GTK_WIDGET (parent->object))))
    {
      GdkWindow *w = gtk_widget_get_window (GTK_WIDGET (parent->object));
      guint32 xid = (guint32) gdk_x11_window_get_xid (w);
      g_autofree char *handle = g_strdup_printf ("x11:%x", xid);
      callback (parent, handle, data);
      return TRUE;
    }
#endif
#ifdef GDK_WINDOWING_WAYLAND
  if (GDK_IS_WAYLAND_DISPLAY (gtk_widget_get_display (GTK_WIDGET (parent->object))))
    {
      GdkWindow *w = gtk_widget_get_window (GTK_WIDGET (parent->object));
      parent->callback = callback;
      parent->data = data;
      return gdk_wayland_window_export_handle (w, _xdp_parent_exported_wayland, parent, NULL);
    }
#endif
  g_warning ("Couldn't export handle, unsupported windowing system");
  return FALSE;
}
#else
;
#endif

static inline void _xdp_parent_unexport_gtk (XdpParent *parent)
{
#ifdef GDK_WINDOWING_WAYLAND
  if (GDK_IS_WAYLAND_DISPLAY (gtk_widget_get_display (GTK_WIDGET (parent->object))))
    {
      GdkWindow *w = gtk_widget_get_window (GTK_WIDGET (parent->object));
      gdk_wayland_window_unexport_handle (w);
    }
#endif
}

#if 0
void       xdp_parent_free    (XdpParent *parent);
XdpParent *xdp_parent_new_gtk (GtkWindow *window);
#endif

XDP_PUBLIC
XdpParent               *xdp_parent_new_from_gtk (GtkWindow *window);

static inline XdpParent *xdp_parent_new_gtk      (GtkWindow *window);

static inline XdpParent *xdp_parent_new_gtk      (GtkWindow *window)
{
  XdpParent *parent = g_new0 (XdpParent, 1);
  parent->export = _xdp_parent_export_gtk;
  parent->unexport = _xdp_parent_unexport_gtk;
  parent->object = (GObject *) g_object_ref (window);
  return parent;
}

G_END_DECLS
