/* GTK - The GIMP Toolkit
 * Copyright (C) 2012 Benjamin Otte <otte@gnome.org>
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

#include "config.h"

#include "gtkcssmatcherprivate.h"

#include "gtkwidgetpath.h"

/* GTK_CSS_MATCHER_WIDGET_PATH */

static gboolean
gtk_css_matcher_widget_path_get_parent (GtkCssMatcher       *matcher,
                                        const GtkCssMatcher *child)
{
  if (child->path.index == 0)
    return FALSE;

  matcher->path.klass = child->path.klass;
  matcher->path.path = child->path.path;
  matcher->path.state_flags = 0;
  matcher->path.index = child->path.index - 1;
  matcher->path.sibling_index = gtk_widget_path_iter_get_sibling_index (matcher->path.path, matcher->path.index);

  return TRUE;
}

static gboolean
gtk_css_matcher_widget_path_get_previous (GtkCssMatcher       *matcher,
                                          const GtkCssMatcher *next)
{
  if (next->path.sibling_index == 0)
    return FALSE;

  matcher->path.klass = next->path.klass;
  matcher->path.path = next->path.path;
  matcher->path.state_flags = 0;
  matcher->path.index = next->path.index;
  matcher->path.sibling_index = next->path.sibling_index - 1;

  return TRUE;
}

static GtkStateFlags
gtk_css_matcher_widget_path_get_state (const GtkCssMatcher *matcher)
{
  return matcher->path.state_flags;
}

static gboolean
gtk_css_matcher_widget_path_has_name (const GtkCssMatcher *matcher,
                                      const char          *name)
{
  const GtkWidgetPath *siblings;
  GType type;
  
  type = g_type_from_name (name);
  siblings = gtk_widget_path_iter_get_siblings (matcher->path.path, matcher->path.index);
  if (siblings && matcher->path.sibling_index != gtk_widget_path_iter_get_sibling_index (matcher->path.path, matcher->path.index))
    return g_type_is_a (gtk_widget_path_iter_get_object_type (siblings, matcher->path.sibling_index), type);
  else
    return g_type_is_a (gtk_widget_path_iter_get_object_type (matcher->path.path, matcher->path.index), type);
}

static gboolean
gtk_css_matcher_widget_path_has_class (const GtkCssMatcher *matcher,
                                       const char          *class_name)
{
  const GtkWidgetPath *siblings;
  
  siblings = gtk_widget_path_iter_get_siblings (matcher->path.path, matcher->path.index);
  if (siblings && matcher->path.sibling_index != gtk_widget_path_iter_get_sibling_index (matcher->path.path, matcher->path.index))
    return gtk_widget_path_iter_has_class (siblings, matcher->path.sibling_index, class_name);
  else
    return gtk_widget_path_iter_has_class (matcher->path.path, matcher->path.index, class_name);
}

static gboolean
gtk_css_matcher_widget_path_has_id (const GtkCssMatcher *matcher,
                                    const char          *id)
{
  const GtkWidgetPath *siblings;
  
  siblings = gtk_widget_path_iter_get_siblings (matcher->path.path, matcher->path.index);
  if (siblings && matcher->path.sibling_index != gtk_widget_path_iter_get_sibling_index (matcher->path.path, matcher->path.index))
    return gtk_widget_path_iter_has_name (siblings, matcher->path.sibling_index, id);
  else
    return gtk_widget_path_iter_has_name (matcher->path.path, matcher->path.index, id);
}

static gboolean
gtk_css_matcher_widget_path_has_regions (const GtkCssMatcher *matcher)
{
  const GtkWidgetPath *siblings;
  GSList *regions;
  gboolean result;
  
  siblings = gtk_widget_path_iter_get_siblings (matcher->path.path, matcher->path.index);
  if (siblings && matcher->path.sibling_index != gtk_widget_path_iter_get_sibling_index (matcher->path.path, matcher->path.index))
    regions = gtk_widget_path_iter_list_regions (siblings, matcher->path.sibling_index);
  else
    regions = gtk_widget_path_iter_list_regions (matcher->path.path, matcher->path.index);
  result = regions != NULL;
  g_slist_free (regions);

  return result;
}

static gboolean
gtk_css_matcher_widget_path_has_region (const GtkCssMatcher *matcher,
                                        const char          *region,
                                        GtkRegionFlags       flags)
{
  const GtkWidgetPath *siblings;
  GtkRegionFlags region_flags;
  
  siblings = gtk_widget_path_iter_get_siblings (matcher->path.path, matcher->path.index);
  if (siblings && matcher->path.sibling_index != gtk_widget_path_iter_get_sibling_index (matcher->path.path, matcher->path.index))
    {
      if (!gtk_widget_path_iter_has_region (siblings, matcher->path.sibling_index, region, &region_flags))
        return FALSE;
    }
  else
    {
      if (!gtk_widget_path_iter_has_region (matcher->path.path, matcher->path.index, region, &region_flags))
        return FALSE;
    }

  if ((flags & region_flags) != flags)
    return FALSE;

  return TRUE;
}

static guint
gtk_css_matcher_widget_path_get_sibling_index (const GtkCssMatcher *matcher)
{
  return matcher->path.sibling_index;
}

static guint
gtk_css_matcher_widget_path_get_n_siblings (const GtkCssMatcher *matcher)
{
  const GtkWidgetPath *siblings;

  siblings = gtk_widget_path_iter_get_siblings (matcher->path.path, matcher->path.index);
  if (!siblings)
    return 0;

  return gtk_widget_path_length (siblings);
}

static const GtkCssMatcherClass GTK_CSS_MATCHER_WIDGET_PATH = {
  gtk_css_matcher_widget_path_get_parent,
  gtk_css_matcher_widget_path_get_previous,
  gtk_css_matcher_widget_path_get_state,
  gtk_css_matcher_widget_path_has_name,
  gtk_css_matcher_widget_path_has_class,
  gtk_css_matcher_widget_path_has_id,
  gtk_css_matcher_widget_path_has_regions,
  gtk_css_matcher_widget_path_has_region,
  gtk_css_matcher_widget_path_get_sibling_index,
  gtk_css_matcher_widget_path_get_n_siblings
};

void
_gtk_css_matcher_init (GtkCssMatcher       *matcher,
                       const GtkWidgetPath *path,
                       GtkStateFlags        state)
{
  matcher->path.klass = &GTK_CSS_MATCHER_WIDGET_PATH;
  matcher->path.path = path;
  matcher->path.state_flags = state;
  matcher->path.index = gtk_widget_path_length (path) - 1;
  matcher->path.sibling_index = gtk_widget_path_iter_get_sibling_index (path, matcher->path.index);
}
