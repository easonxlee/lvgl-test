#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "lvgl/lvgl.h"
#include "mydemo.h"

LV_FONT_DECLARE(WRJZY_20);  


static void demo_create(void);
static void demo_create2(void);
void lv_my_test(void)
{
		//demo_create();
        demo_create2();
}

static lv_obj_t *ta;
static lv_obj_t *kb;
static lv_obj_t *chart;
static lv_chart_series_t * ser1;

static lv_res_t keyboard_open_close(lv_obj_t * text_area);
static lv_res_t keyboard_hide_action(lv_obj_t * keyboard);


static void write_create(lv_obj_t* parent);
static void list_create(lv_obj_t* parent);
static void chart_create(lv_obj_t* parent);
static void mbtn_create(lv_obj_t* parent);
static void cam_create(lv_obj_t* parent);
static void imgbtn_create(lv_obj_t* parent);

void task_print(void *para)
{
	char *p;
	p = lv_ta_get_text(ta);
	if(strlen(p) == 0)
		return;
	printf("%s\n",p);
}

static void demo_create2(void)
{
    static lv_obj_t *tv,*tab1,*tab2,*tab3,*tab4,*tab5,*tab6;

	lv_font_add(&WRJZY_20, &lv_font_dejavu_20);

    tv = lv_tabview_create(lv_scr_act(),NULL);
    tab1 = lv_tabview_add_tab(tv,"Write/闹钟");
    tab2 = lv_tabview_add_tab(tv,"List");
    tab3 = lv_tabview_add_tab(tv,"Chart&Mbtn");
    tab4 = lv_tabview_add_tab(tv,"Video");
	tab5 = lv_tabview_add_tab(tv,"Imgbtn");
    write_create(tab1);
	//lv_task_create(task_print,1000,LV_TASK_PRIO_MID, NULL);
	list_create(tab2);
    chart_create(tab3);
    mbtn_create(tab3);
    cam_create(tab4);
	imgbtn_create(tab5);

	lv_ta_set_text(ta,"123");
	lv_ta_add_text(ta,"闹钟理发");

}

static void write_create(lv_obj_t* parent)
{
    lv_page_set_style(parent, LV_PAGE_STYLE_BG, &lv_style_transp_fit);
    lv_page_set_style(parent, LV_PAGE_STYLE_SCRL, &lv_style_transp_fit);
    lv_page_set_sb_mode(parent, LV_SB_MODE_OFF);

    ta = lv_ta_create(parent,NULL);
    lv_obj_set_size(ta,lv_page_get_scrl_width(parent), lv_obj_get_height(parent)/2);
	
	lv_ta_set_text(ta,"");

    kb = lv_kb_create(parent,NULL);
    lv_obj_set_size(kb,lv_page_get_scrl_width(parent), lv_obj_get_height(parent)/2);
    lv_kb_set_ta(kb, ta);
    lv_page_set_rel_action(ta, keyboard_open_close);
    lv_obj_align(kb, ta, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
}


static lv_res_t keyboard_open_close(lv_obj_t * text_area)
{
    (void) text_area;    /*Unused*/

    lv_obj_t * parent = lv_obj_get_parent(lv_obj_get_parent(ta));   /*Test area is on the scrollable part of the page but we need the page itself*/

    if(kb) {
        return keyboard_hide_action(kb);
    } else {
        kb = lv_kb_create(parent, NULL);
        lv_obj_set_size(kb, lv_page_get_scrl_width(parent), lv_obj_get_height(parent) / 2);
        lv_obj_align(kb, ta, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_kb_set_ta(kb, ta);
        lv_kb_set_hide_action(kb, keyboard_hide_action);
        lv_kb_set_ok_action(kb, keyboard_hide_action);

        return LV_RES_OK;
    }
}

static lv_res_t keyboard_hide_action(lv_obj_t * keyboard)
{
    lv_obj_del(kb);
    kb = NULL;
    return LV_RES_INV;

}

static lv_res_t list_btn_action(lv_obj_t * btn)
{
    lv_ta_add_text(ta,"\n");
    lv_ta_add_text(ta,lv_list_get_btn_text(btn));
}
static void list_create(lv_obj_t* parent)
{
    static const char *txt[] = {"ok",""};
    lv_obj_t* mbox;
	mbox = lv_mbox_create(parent,NULL);
    lv_mbox_set_text(mbox,"hello");
    lv_mbox_add_btns(mbox,txt,NULL);

    lv_obj_t * list = lv_list_create(parent, NULL);
    //lv_obj_set_height(list, 2 * lv_obj_get_height(parent) / 3);

    lv_list_add(list, SYMBOL_FILE, "New卡", list_btn_action);
    lv_list_add(list, SYMBOL_DIRECTORY, "Open", list_btn_action);
    lv_list_add(list, SYMBOL_TRASH, "Delete", list_btn_action);
    lv_list_add(list, SYMBOL_EDIT, "Edit", list_btn_action);
    lv_list_add(list, SYMBOL_SAVE, "Save", list_btn_action);
    lv_list_add(list, SYMBOL_WIFI, "WiFi", list_btn_action);
    lv_list_add(list, SYMBOL_GPS, "GPS", list_btn_action);

}
static lv_res_t slider_action(lv_obj_t * slider)
{
    int16_t v = lv_slider_get_value(slider);
    v = 1000 * 100 / v; /*Convert to range modify values linearly*/
    //lv_chart_set_range(chart, 0, v);

    lv_chart_ext_t * ext = lv_obj_get_ext_attr(chart);
    ser1->points[3] = lv_slider_get_value(slider)/10; 
    lv_chart_refresh(chart);

    return LV_RES_OK;
}
static void chart_create(lv_obj_t * parent)
{
    chart = lv_chart_create(parent,NULL);
    lv_chart_set_series_opa(chart, LV_OPA_70);
    
    ser1 = lv_chart_add_series(chart, LV_COLOR_RED);
    lv_chart_set_next(chart, ser1, 40);
    lv_chart_set_next(chart, ser1, 30);
    lv_chart_set_next(chart, ser1, 59);
    lv_chart_set_next(chart, ser1, 31);
    lv_chart_set_next(chart, ser1, 55);
    lv_chart_set_next(chart, ser1, 70);
    lv_chart_set_next(chart, ser1, 47);
    lv_chart_set_next(chart, ser1, 59);
    lv_chart_set_next(chart, ser1, 82);
    lv_chart_set_next(chart, ser1, 91);

    /*Create a second slider*/
    lv_obj_t * slider = lv_slider_create(parent, NULL);
    lv_obj_set_size(slider, lv_obj_get_width(chart), LV_DPI / 3);
    //lv_obj_align(slider, chart, LV_ALIGN_OUT_BOTTOM_MID, 0, (LV_VER_RES - chart->coords.y2 - lv_obj_get_height(slider)) / 2); /*Align to below the chart*/
	lv_obj_align(slider, chart, LV_ALIGN_OUT_BOTTOM_MID, 5, 5); /*Align to below the chart*/
    lv_slider_set_action(slider, slider_action);
    lv_slider_set_range(slider, 10, 1000);
    lv_slider_set_value(slider, 700);
    slider_action(slider);          /*Simulate a user value set the refresh the chart*/
}
 static const char * map[] = {"1","2","3","\n","4","5","6","\n","7","8","9","\n","*","0","#",""};
static lv_res_t btnm_action(lv_obj_t* btnm)
{
    lv_ta_add_char(ta,' ');
    lv_ta_add_text(ta,map[lv_btnm_get_pressed(btnm)]);
}
static void mbtn_create(lv_obj_t * parent)
{
    lv_obj_t * btnm;
   
    btnm = lv_btnm_create(parent,NULL);
    lv_btnm_set_map(btnm,map);
    lv_obj_set_size(btnm, LV_HOR_RES/3, LV_VER_RES / 3);
	lv_obj_align(btnm, chart, LV_ALIGN_OUT_RIGHT_TOP, 5, 5);
    lv_btnm_set_action(btnm,btnm_action);
}

static lv_obj_t* video;
#define    WIDTH   320
#define    HIGHT   240
extern char g_cam_bmp[WIDTH*HIGHT*4];
static char s_cam_bmp[WIDTH*HIGHT*4];
void* camera_cap(void* args);
void  camera_exit(void);

lv_img_dsc_t img_vedio = {
   .header.always_zero = 0,
  .header.w = 320,
  .header.h = 240,
  .data_size = 76800 * LV_COLOR_SIZE / 8,
  .header.cf = LV_IMG_CF_TRUE_COLOR,
  .data = s_cam_bmp,
};

void task_cam_fresh(void* para)
{
	int i;
	for(i = 0; i < WIDTH*HIGHT*4; i++)
	{
		s_cam_bmp[i] = g_cam_bmp[i];
	}
	lv_img_set_src(video,&img_vedio);
}

static void cam_create(lv_obj_t* parent)
{
	int i = 0,j;
	pthread_t pid_cam;
	pid_cam = pthread_create(&pid_cam,NULL,camera_cap,NULL);

	sleep(1);

	video = lv_img_create(parent,NULL);
	lv_img_set_src(video,&img_vedio);
	lv_task_create(task_cam_fresh,30,LV_TASK_PRIO_MID, NULL);	
	//pthread_join(pid_cam,NULL);
	//camera_exit();

}

LV_IMG_DECLARE(imgbtn_img_1);
LV_IMG_DECLARE(imgbtn_img_2);
LV_IMG_DECLARE(imgbtn_img_3);
LV_IMG_DECLARE(imgbtn_img_4);
lv_res_t imgbtn_clicked(lv_obj_t * imgbtn);

static void imgbtn_create(lv_obj_t *parent)
{
	lv_obj_t* imgbtn1,* imgbtn2,* imgbtn3;
	
	imgbtn1 = lv_imgbtn_create(parent,NULL);
	//imgbtn2 = lv_imgbtn_create(parent,NULL);
	//imgbtn3 = lv_imgbtn_create(parent,NULL);
	
	lv_obj_set_pos(imgbtn1, 10, 10);
    lv_imgbtn_set_toggle(imgbtn1, true);
	lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_REL, &imgbtn_img_1);
    lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_PR, &imgbtn_img_2);
    lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_REL, &imgbtn_img_3);
    lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_TGL_PR, &imgbtn_img_4);

    lv_imgbtn_set_action(imgbtn1, LV_BTN_ACTION_CLICK, imgbtn_clicked);
	lv_obj_t * label = lv_label_create(imgbtn1, NULL);
    lv_label_set_text(label, "Button 1");
	
}

lv_res_t imgbtn_clicked(lv_obj_t * imgbtn)
{
    (void) imgbtn; /*Unused*/

    printf("Imgbtn1 Clicked\n");
    return LV_RES_OK;
}

