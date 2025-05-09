static void killthis(Window w);
static void bulkill(const Arg *arg);

void
killthis(Window w) {
	if (!sendevent(w, wmatom[WMDelete], NoEventMask, wmatom[WMDelete], CurrentTime, 0, 0, 0)) {
		XGrabServer(dpy);
		XSetErrorHandler(xerrordummy);
		XSetCloseDownMode(dpy, DestroyAll);
		XKillClient(dpy, w);
		XSync(dpy, False);
		XSetErrorHandler(xerror);
		XUngrabServer(dpy);
	}
}

void
bulkill(const Arg *arg)
{
    Client *c;

	if (!selmon->sel)
		return;

    if (!arg->ui || arg->ui == 0) {
        killthis(selmon->sel->win);
        return;
    }

    for (c = selmon->clients; c; c = c->next) {
        if (!ISVISIBLE(c) || (arg->ui == 1 && c == selmon->sel))
            continue;
        killthis(c->win);
    }
}
