/* GAIL - The GNOME Accessibility Implementation Library
 * Copyright 2001 Sun Microsystems Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __GTK_TOPLEVEL_ACCESSIBLE_H__
#define __GTK_TOPLEVEL_ACCESSIBLE_H__

#include <atk/atk.h>

G_BEGIN_DECLS

#define GTK_TYPE_TOPLEVEL_ACCESSIBLE               (_gtk_toplevel_accessible_get_type ())
#define GTK_TOPLEVEL_ACCESSIBLE(obj)               (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_TOPLEVEL_ACCESSIBLE, GtkToplevelAccessible))
#define GTK_TOPLEVEL_ACCESSIBLE_CLASS(klass)       (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_TOPLEVEL_ACCESSIBLE, GtkToplevelAccessibleClass))
#define GTK_IS_TOPLEVEL_ACCESSIBLE(obj)            (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_TOPLEVEL_ACCESSIBLE))
#define GTK_IS_TOPLEVEL_ACCESSIBLE_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_TOPLEVEL_ACCESSIBLE))
#define GTK_TOPLEVEL_ACCESSIBLE_GET_CLASS(obj)     (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_TOPLEVEL_ACCESSIBLE, GtkToplevelAccessibleClass))

typedef struct _GtkToplevelAccessible      GtkToplevelAccessible;
typedef struct _GtkToplevelAccessibleClass GtkToplevelAccessibleClass;

struct _GtkToplevelAccessible
{
  AtkObject parent;
  GList *window_list;
};

struct _GtkToplevelAccessibleClass
{
  AtkObjectClass parent_class;
};

GType _gtk_toplevel_accessible_get_type (void);

G_END_DECLS

#endif /* __GTK_TOPLEVEL_ACCESSIBLE_H__ */
