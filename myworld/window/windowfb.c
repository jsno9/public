#include<linux/fb.h>		//	framebuffer
#include<unistd.h>
#include<fcntl.h>
#include <stdio.h>
#include <stdlib.h>

static unsigned long long fbaddr=0;
static int fbtotalbyte=0;
static int fboneline=0;
//static int width=512;
//static int height=512;
//virtual info
static int xmax=0;
static int ymax=0;
//static int bpp=0;

//自己的画板
static int fb=-1;
//static char* screenbuf;

static void init()
{
	fb=open("/dev/fb0",O_RDWR);
	if(fb<0)
	{
		    loge("can not open /dev/fb0\n");
		    exit(-1);
	}
	
	struct fb_fix_screeninfo finfo;
	if(ioctl(fb,FBIOGET_FSCREENINFO,&finfo))
	{
		loge("can not get finfo!\n");
		exit(-1);
	}
	
	fbaddr=finfo.smem_start;
	fbtotalbyte=finfo.smem_len;
	fboneline=finfo.line_length;
	loge("fbaddr=0x%llx,fbtotalbyte=0x%x,fboneline=0x%x\n"
		,fbaddr,fbtotalbyte,fboneline);
	
	struct fb_var_screeninfo vinfo;
	if(ioctl(fb,FBIOGET_VSCREENINFO,&vinfo))
	{
		loge("can not get vinfo\n");
		exit(-1);
	}

	xmax=vinfo.xres;
	ymax=vinfo.yres;
	bpp=vinfo.bits_per_pixel;
	printf("xmax=%d,ymax=%d,bpp=%d\n",xmax,ymax,bpp);
}

static void uninit()
{

}

static void windowwrite(struct window *win)
{
	int x,y,ret;

	//5,6,5
	if(bpp==16)
	{
		for(x=0;x<win->width*win->height;x++)
		{
			*(unsigned short*)(winfb->screenbuf+x*2)=
				    (win->screenbuf[x*4+0]>>3)
				+ ( (win->screenbuf[x*4+1]>>2) <<  5 )
				+ ( (win->screenbuf[x*4+2]>>3) << 11 );
		}
	}
	//
	for(y=0;y<win->height;y++)
	{
		ret=lseek(fb , y*fboneline , SEEK_SET);
		ret=write(fb , win->screenbuf + y*win->width*win->bpp/8 , win->width*win->bpp/8);
	}
}


void winfb_create(struct window *win)
{
	win->id=1;
	win->init=init;
	win->uninit=uninit;
	win->write=windowwrite;
}


