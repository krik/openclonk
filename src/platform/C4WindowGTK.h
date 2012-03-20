/*
 * OpenClonk, http://www.openclonk.org
 *
 * Copyright (c) 2006  Armin Burgmeier
 * Copyright (c) 2006-2009, RedWolf Design GmbH, http://www.clonk.de
 *
 * Portions might be copyrighted by other authors who have contributed
 * to OpenClonk.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * See isc_license.txt for full license and disclaimer.
 *
 * "Clonk" is a registered trademark of Matthes Bender.
 * See clonk_trademark_license.txt for full license.
 */

/* GTK+ version of StdWindow */

#ifndef INC_STDGTKWINDOW
#define INC_STDGTKWINDOW

#include <C4Window.h>

#include <gtk/gtk.h>

class C4GtkWindow: public C4Window
{
public:
	C4GtkWindow();
	virtual ~C4GtkWindow();

	virtual void Clear();

	using C4Window::Init;
	virtual C4Window * Init(WindowKind windowKind, C4AbstractApp * pApp, const char * Title, C4Window * pParent = 0, bool HideCursor = true);
	virtual bool ReInit(C4AbstractApp* pApp);

	GtkWidget* window;
protected:
	// InitGUI should either return a widget which is used as a
	// render target or return what the base class returns, in which
	// case the whole window is used as render target.
	virtual GtkWidget* InitGUI();

private:
	static void OnDestroyStatic(GtkWidget* widget, gpointer data);
	static GdkFilterReturn OnFilter(GdkXEvent* xevent, GdkEvent* event, gpointer user_data);
	static gboolean OnUpdateKeyMask(GtkWidget* widget, GdkEventKey* event, gpointer user_data);

	gulong handlerDestroy;
};

#endif // INC_STDGTKWINDOW