CSRCS += lv_my_test.c WRJZY_20.c img_bubble_pattern_2.c
CSRCS += imgbtn_img_1.c imgbtn_img_2.c imgbtn_img_3.c imgbtn_img_4.c

DEPPATH += --dep-path $(LVGL_DIR)/lv_my_test/
VPATH += :$(LVGL_DIR)/lv_my_test/

CFLAGS += -I$(LVGL_DIR)/lv_my_test/

