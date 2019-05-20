#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include "jyuv.h"
#include "cameraopt.h"
#include "framebufferopt.h"

#define    WIDTH   320
#define    HIGHT   240


static char yuyv[WIDTH*HIGHT*2];
static char cam_bmp[WIDTH*HIGHT*3];
char g_cam_bmp[WIDTH*HIGHT*4];

void* camera_cap(void* args)
{
	
	//初始化摄像头
	Init_Cameral(WIDTH , HIGHT );
	//初始化framebuffer
	//Init_FrameBuffer(WIDTH , HIGHT ); 
	//开启摄像头
	Start_Cameral();
	//采集一张图片
	while(1)
	{
		Get_Picture(yuyv);
		yuyv2rgb24(yuyv, cam_bmp, WIDTH, HIGHT);
		int i,j = 0,k = 0,l = 0;
#if 0
		for(i = 0; i < WIDTH*HIGHT; i++)
		{
			g_cam_bmp[j++] = cam_bmp[k++];
			g_cam_bmp[j++] = cam_bmp[k++];
			g_cam_bmp[j++] = cam_bmp[k++];
			g_cam_bmp[j++] = 0xFF;
		}
#endif
		for(i = 0; i < HIGHT; i++)
		{
			for(j = 0; j < WIDTH; j++)
			{
				l = (HIGHT - i)*WIDTH*3+j*3;
				g_cam_bmp[k++] = cam_bmp[l];
				g_cam_bmp[k++] = cam_bmp[l+1];
				g_cam_bmp[k++] = cam_bmp[l+2];
				g_cam_bmp[k++] = 0xFF;
			}
		}
		//Write_FrameBuffer(g_cam_bmp);
		usleep(50000);
		//break;
	}
	camera_exit();
}

void  camera_exit(void)
{
	//关闭摄像头
	Stop_Cameral();
	//关闭Framebuffer
	//Exit_Framebuffer();
	//退出
	Exit_Cameral();
}



