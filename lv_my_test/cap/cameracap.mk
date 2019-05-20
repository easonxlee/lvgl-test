CFLAGS += -pthread -I./
CSRCS += cameracap.c rgb2bmp.c jyuv.c cameraopt.c 111111.c
#CSRCS += framebufferopt.c

DEPPATH += --dep-path $(LVGL_DIR)/lv_my_test/cap/
VPATH += :$(LVGL_DIR)/lv_my_test/cap/

