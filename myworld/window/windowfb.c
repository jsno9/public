#include<linux/fb.h>		//	framebuffer
#include<unistd.h>
#include<fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "window.h"

static unsigned long *fbaddr;
static int fbtotalbyte=0;
static int fboneline=0;

//virtual info
static int *xmax;
static int *ymax;
static int *bpp;
static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;

//自己的画板
static int fb=-1;
__attribute__((aligned(0x1000))) static unsigned char pixbuf[2048*1024*32];
//static char* screenbuf;

static void init(struct window *win)
{
	fb=open("/dev/fb0",O_RDWR);
	if(fb<0)
	{
		    loge("can not open /dev/fb0\n");
		    exit(-1);
	}
	
	
	if(ioctl(fb,FBIOGET_FSCREENINFO,&finfo))
	{
		loge("can not get finfo!\n");
		exit(-1);
	}
	//loge("window init!\n");
	fbaddr=&finfo.smem_start;
	fbtotalbyte=finfo.smem_len;
	fboneline=finfo.line_length;

	//loge("fbaddr=0x%llx,fbtotalbyte=0x%x,fboneline=0x%x\n"
	//	,*fbaddr,fbtotalbyte,fboneline);
	
	
	if(ioctl(fb,FBIOGET_VSCREENINFO,&vinfo))
	{
		loge("can not get vinfo\n");
		exit(-1);
	}

	xmax=&vinfo.xres;
	ymax=&vinfo.yres;
	bpp=&vinfo.bits_per_pixel;
//	loge("xmax=%d,ymax=%d,bpp=%d\n",*xmax,*ymax,*bpp);
//	loge("xmax=%llx,ymax=%llx,bpp=%llx\n",xmax,ymax,bpp);

	win->width=xmax;
	win->height=ymax;
	win->bpp=bpp;
//loge("win->width=%d\n",win->width);
}

static void uninit()
{
	close(fb);
}

static void windowwrite(struct window *win)
{
	int x,y,ret;
	unsigned int width=*(win->width);
	unsigned int height=*(win->height);
	unsigned int bpp=*(win->bpp);
	//5,6,5
	if(bpp==16)
	{
		for(x=0;x<width*height;x++)
		{
			*(unsigned short*)(win->screenbuf+x*2)=
				    (win->screenbuf[x*4+0]>>3)
				+ ( (win->screenbuf[x*4+1]>>2) <<  5 )
				+ ( (win->screenbuf[x*4+2]>>3) << 11 );
		}
	}
	//
	for(y=0;y<height;y++)
	{
		ret=lseek(fb , y*fboneline , SEEK_SET);
		ret=write(fb , win->screenbuf + y*width*bpp/8 , width*bpp/8);
	}
}


void winfb_create(struct window *win)
{
	loge("winfb_create\n");
	win->id=1;
	win->init=init;
	win->uninit=uninit;
	win->write=windowwrite;
	win->screenbuf=pixbuf;

}


