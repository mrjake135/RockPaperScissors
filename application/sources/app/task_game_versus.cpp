#include "task_game_versus.h"
#include "fsm.h"
#include "port.h"
#include "message.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "app.h"
#include "app_dbg.h"

#include "task_list.h"
#include "Adafruit_ssd1306syp.h"
#include "timer.h"

int humanInputVersus;
int human2Input;
int roundWinnerVersus;
int scoreLeft;
int scoreRight;
Adafruit_ssd1306syp doubleView;
int turn;
static void drawArena2(int humanInputVersus, int human2Input, int roundWinnerVersus,int turn);

void task_game_versus(ak_msg_t* msg)
{
	switch(msg->sig)
	{
		case AC_GAME_VERSUS_INIT:{
			scoreLeft = 0;
			scoreRight = 0;
			humanInputVersus = 0;
			human2Input = 0;
			roundWinnerVersus = -1;
			turn = 0;
			drawArena2(humanInputVersus, human2Input,roundWinnerVersus, turn);
		}
			break;

		case AC_GAME_VERSUS_ROUND_INIT:{
			if(scoreLeft ==5 || scoreRight ==5) task_post_pure_msg(AC_TASK_GAME_VERSUS_ID, END2);
			roundWinnerVersus = -1;
			turn = 0;
			drawArena2(humanInputVersus, human2Input,roundWinnerVersus, turn);
		}
			break;
		case AC_GAME_VERSUS_ROCK:{
			APP_DBG("ROCK\n");
			if(turn == 0){
				humanInputVersus = 1;
				turn = 1;
				drawArena2(humanInputVersus, human2Input,roundWinnerVersus, turn);
			}
			else if(turn == 1)
			{
				human2Input = 1;
				turn = 2;
				drawArena2(humanInputVersus, human2Input,roundWinnerVersus, turn);
				task_post_pure_msg(AC_TASK_GAME_VERSUS_ID, AC_GAME_VERSUS_WINNER);
			}
		}
			break;
		case AC_GAME_VERSUS_PAPER:{
			APP_DBG("PAPER\n");
			if(turn == 0){
				humanInputVersus = 2;
				turn = 1;
				drawArena2(humanInputVersus, human2Input,roundWinnerVersus, turn);

			}
			else if(turn == 1)
			{
				human2Input = 2;
				turn = 2;
				drawArena2(humanInputVersus, human2Input,roundWinnerVersus, turn);
				task_post_pure_msg(AC_TASK_GAME_VERSUS_ID, AC_GAME_VERSUS_WINNER);
			}

		}
			break;
		case AC_GAME_VERSUS_SCISSORS:{
			APP_DBG("SCISSORS\n");
			if(turn == 0){
				humanInputVersus = 3;
				turn = 1;
				drawArena2(humanInputVersus, human2Input,roundWinnerVersus, turn);
			}
			else if(turn == 1)
			{
				human2Input = 3;
				turn = 2;
				drawArena2(humanInputVersus, human2Input,roundWinnerVersus, turn);
				task_post_pure_msg(AC_TASK_GAME_VERSUS_ID, AC_GAME_VERSUS_WINNER);
			}
		}
			break;
		case AC_GAME_VERSUS_WINNER:{
			if(humanInputVersus==human2Input) roundWinnerVersus = 0;
			else if((humanInputVersus == 1 && human2Input == 3) ||(humanInputVersus == 2 && human2Input == 1)||(humanInputVersus == 3 && human2Input == 2))
			{
				roundWinnerVersus = 1;
				scoreLeft ++;
			}
			else
			{
				roundWinnerVersus = 2;
				scoreRight ++;
			}
			drawArena2(humanInputVersus, human2Input,roundWinnerVersus, turn);
			timer_set(AC_TASK_GAME_VERSUS_ID, AC_GAME_ROUND_INIT,1500,TIMER_ONE_SHOT);
		}
			break;
		case END2:{
			task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_INITIAL);
			task_post_pure_msg(AC_TASK_GAME_ID, AC_GAME_INIT);
		}
			break;
	}

}



void drawArena2(int humanInputVersus, int human2Input, int roundWinnerVersus, int turn){

	doubleView.initialize();
	doubleView.clear();
	doubleView.drawRect(0,0,90,60,WHITE);
	doubleView.drawLine(45,0,45,60,WHITE);
	doubleView.drawRect(95,0,120,60,WHITE);
	doubleView.setCursor(97,2);
	doubleView.print("Left ");
	doubleView.setCursor(97,12);
	doubleView.print(scoreLeft);
	doubleView.setCursor(97,22);
	doubleView.print("Right ");
	doubleView.setCursor(97,32);
	doubleView.print(scoreRight);

	if(humanInputVersus == 1 && turn == 2) {
		doubleView.setCursor(3,2);
		doubleView.print("ROCK");
		doubleView.drawCircle(20,30,10,WHITE);
	}
	if(humanInputVersus == 2 && turn == 2) {
		doubleView.setCursor(3,2);
		doubleView.print("PAPER");
		doubleView.drawRect(15,15,20,30,WHITE);
	}
	if(humanInputVersus == 3 && turn == 2) {
		doubleView.setCursor(3,2);
		doubleView.print("SCISSOR");
		doubleView.drawCircle(25,40,5,WHITE);
		doubleView.drawCircle(10,40,5,WHITE);
		doubleView.drawLine(20,40,13,25,WHITE);
		doubleView.drawLine(15,40,22,25,WHITE);
	}
	if(human2Input == 1 && turn == 2) {
		doubleView.setCursor(48,2);
		doubleView.print("ROCK");
		doubleView.drawCircle(60,30,10,WHITE);
	}
	if(human2Input == 2 && turn ==2) {
		doubleView.setCursor(48,2);
		doubleView.print("PAPER");
		doubleView.drawRect(60,15,20,30,WHITE);
	}
	if(human2Input == 3 && turn == 2) {
		doubleView.setCursor(48,2);
		doubleView.print("SCISSOR");
		doubleView.drawCircle(70,40,5,WHITE);
		doubleView.drawCircle(55,40,5,WHITE);
		doubleView.drawLine(65,40,58,25,WHITE);
		doubleView.drawLine(60,40,67,25,WHITE);
	}
	if(turn == 0)
	{
		doubleView.setCursor(21,50);
		doubleView.print("Left Turn");
	}
	if(turn == 1)
	{
		doubleView.setCursor(16,50);
		doubleView.print("Right Turn");
	}
	if(roundWinnerVersus == 0)
	{
		doubleView.setCursor(37,50);
		doubleView.print("Tie");
	}
	if(roundWinnerVersus == 1)
	{
		doubleView.setCursor(17,50);
		doubleView.print("Left Winner");
	}
	if(roundWinnerVersus == 2)
	{
		doubleView.setCursor(15,50);
		doubleView.print("Right Winner");
	}
	doubleView.update();
}
