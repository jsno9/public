#ifndef _DRAW_H
#define _DRAW_H

double squareroot(double );
void backgroundcolor(struct window *win, unsigned int color);
void line(struct window *win,int x1, int y1, int x2, int y2, unsigned int color);
void rectframe(struct window *win,int x1, int y1, int x2, int y2, unsigned int color);
void rectbody(struct window *win,int x1, int y1, int x2, int y2, unsigned int color);
void rect(struct window *win,int x1, int y1, int x2, int y2, unsigned int bodycolor, unsigned int framecolor);
void circleframe(struct window *win,int cx, int cy, int radius, unsigned int color);
void circlebody(struct window *win,int cx, int cy, int radius, unsigned int color);

#endif
