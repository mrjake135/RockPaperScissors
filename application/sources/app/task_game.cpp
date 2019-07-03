#include "fsm.h"
#include "port.h"
#include "message.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "app.h"
#include "app_dbg.h"

#include "task_list.h"
#include "task_game.h"
#include "task_game_single.h"
#include "timer.h"
#include "task_game_versus.h"


int player = 0;
void task_game(ak_msg_t* msg) {
	switch (msg->sig) {
	case AC_GAME_INIT:{
		APP_DBG("INITIALIZE \n");
		player = 0;
		task_post_pure_msg(AC_TASK_GAME_ID,AC_GAME_DECIDE);
	}
		break;

	case AC_GAME_DECIDE:{
		APP_DBG("CHOOSING\n");
		if (player == 0) task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_ON_LOGO);
		if (player == 1) task_post_pure_msg(AC_TASK_DISPLAY_ID,AC_GAME_DISPLAY_GAME_1);
		if (player == 2) task_post_pure_msg(AC_TASK_DISPLAY_ID,AC_GAME_DISPLAY_GAME_2);
	}
		break;
	case AC_GAME_DECIDE_LEFT:{
		if (player == 0)
		{
			player = 1;
			task_post_pure_msg(AC_TASK_GAME_ID,AC_GAME_DECIDE);
		}
		else if(player == 1)
		{
			task_post_pure_msg(AC_TASK_GAME_SINGLE_ID,AC_GAME_WAIT_INPUT_ROCK);
		}
		else if(player == 2)
		{
			task_post_pure_msg(AC_TASK_GAME_VERSUS_ID,AC_GAME_VERSUS_ROCK);
		}
	}
		break;
	case AC_GAME_DECIDE_MIDDLE:{
		if (player == 0)
		{
			player = 2;
			task_post_pure_msg(AC_TASK_GAME_ID,AC_GAME_DECIDE);
		}
		else if(player == 1)
		{
			task_post_pure_msg(AC_TASK_GAME_SINGLE_ID,AC_GAME_WAIT_INPUT_PAPER);
		}
		else if(player == 2)
		{
			task_post_pure_msg(AC_TASK_GAME_VERSUS_ID,AC_GAME_VERSUS_PAPER);
		}
	}
		break;
	case AC_GAME_DECIDE_RIGHT:{
		if (player == 0)
		{
			player = 0;
			task_post_pure_msg(AC_TASK_GAME_ID,AC_GAME_INIT);
		}
		else if(player == 1)
		{
			task_post_pure_msg(AC_TASK_GAME_SINGLE_ID,AC_GAME_WAIT_INPUT_SCISSORS);
		}
		else if(player == 2)
		{
			task_post_pure_msg(AC_TASK_GAME_VERSUS_ID,AC_GAME_VERSUS_SCISSORS);
		}
	}
		break;
	default:
		break;
	}
}
