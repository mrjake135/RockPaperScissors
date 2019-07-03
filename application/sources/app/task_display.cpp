#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "view_render.h"

#include "qrcode.h"

#include "app.h"
#include "app_dbg.h"

#include "task_list.h"
#include "task_display.h"
#include "task_game.h"
#include "Adafruit_ssd1306syp.h"

Adafruit_ssd1306syp graphicsObject;

/* list of view dynamic function */
static void view_scr_startup();
static void view_game_display_1();
static void view_game_display_2();
void task_display(ak_msg_t* msg) {
	switch (msg->sig) {
	case AC_DISPLAY_INITIAL: {
		APP_DBG_SIG("AC_DISPLAY_INITIAL\n");
		graphicsObject.clear();
		graphicsObject.initialize();
	}
		break;

	case AC_DISPLAY_SHOW_ON_LOGO: {
		view_scr_startup();
	}
		break;

	case AC_DISPLAY_SHOW_OFF: {
		SCREEN_NONE_UPDATE_MASK();
	}
		break;

	case AC_GAME_DISPLAY_GAME_1: {
		view_game_display_1();
		timer_set(AC_TASK_GAME_SINGLE_ID,AC_GAME_SINGLE_INIT,1000,TIMER_ONE_SHOT);
	}
		break;

	case AC_GAME_DISPLAY_GAME_2: {
		view_game_display_2();
		timer_set(AC_TASK_GAME_VERSUS_ID,AC_GAME_VERSUS_INIT,1000,TIMER_ONE_SHOT);
	}
		break;

	default:
		break;
	}
}

void view_scr_startup() {
	graphicsObject.clear();
	graphicsObject.drawRect(0,0,120,60,WHITE);
	graphicsObject.setCursor(15,5);
	graphicsObject.println("SELECT A MODE");
	graphicsObject.setCursor(1,20);
	graphicsObject.println("1. SINGLE PLAYER");
	graphicsObject.setCursor(1,30);
	graphicsObject.println("2. VERSUS");
	graphicsObject.update();
}
void view_game_display_1()
{
	graphicsObject.clear();
	graphicsObject.setCursor(0,0);
	graphicsObject.print("YOU HAVE SELECTED ONE PLAYER");
	graphicsObject.update();
}

void view_game_display_2()
{
	graphicsObject.clear();
	graphicsObject.setCursor(0,0);
	graphicsObject.print("YOU HAVE SELECTED VERSUS");
	graphicsObject.update();
}
