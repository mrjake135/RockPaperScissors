#include "fsm.h"
#include "port.h"
#include "message.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "app.h"
#include "app_dbg.h"

#include "task_list.h"
#include "task_game_single.h"
#include "Adafruit_ssd1306syp.h"
#include "timer.h"
Adafruit_ssd1306syp singleView;
int scoreBot;
int scoreHuman;
int humanInput;
int botInput;
int roundWinner;
static void drawArena(int humanInput, int roundWinner, int botInput);

void task_game_single(ak_msg_t* msg)
{
	switch (msg->sig)
	{
		case AC_GAME_SINGLE_INIT:
		{
			scoreBot = 0;
			scoreHuman = 0;
			roundWinner = -1;
			drawArena(humanInput,roundWinner,botInput);
		}
		break;
		case AC_GAME_ROUND_INIT:
		{
			if(scoreBot == 5 || scoreHuman ==5) task_post_pure_msg(AC_TASK_GAME_SINGLE_ID, END);
			roundWinner = -1;
			humanInput = 0;
			botInput = 0;
			drawArena(humanInput,roundWinner,botInput);
		}
		break;
		case AC_GAME_WAIT_INPUT_ROCK:
		{
			humanInput = 1;
			drawArena(humanInput,roundWinner,botInput);
			timer_set(AC_TASK_GAME_SINGLE_ID,WINNER,1000,TIMER_ONE_SHOT);
		}
		break;
		case AC_GAME_WAIT_INPUT_PAPER:
		{
			humanInput = 2;
			drawArena(humanInput,roundWinner,botInput);
			timer_set(AC_TASK_GAME_SINGLE_ID,WINNER,1000,TIMER_ONE_SHOT);
		}
		break;
		case AC_GAME_WAIT_INPUT_SCISSORS:
		{
			humanInput = 3;
			drawArena(humanInput,roundWinner,botInput);
			timer_set(AC_TASK_GAME_SINGLE_ID,WINNER,1000,TIMER_ONE_SHOT);
		}
		break;
		case WINNER:
		{
			botInput = rand()%3+1;
			if(botInput == humanInput) roundWinner = 0;
			else if((botInput == 1 && humanInput ==3) || (botInput == 2 && humanInput == 1) || (botInput == 3 && humanInput == 2))
			{
				roundWinner = 2;
				scoreBot++;
			}
			else
			{
				scoreHuman++;
				roundWinner = 1;
			}
			drawArena(humanInput,roundWinner,botInput);
			timer_set(AC_TASK_GAME_SINGLE_ID,AC_GAME_ROUND_INIT,1500,TIMER_ONE_SHOT);
		}
		break;
		case END:
		{
			task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_INITIAL);
			task_post_pure_msg(AC_TASK_GAME_ID, AC_GAME_INIT);
		}
		break;
	}
}

void drawArena(int humanInput, int roundWinner, int botInput){
	singleView.initialize();
	singleView.clear();
	singleView.drawRect(0,0,90,60,WHITE);
	singleView.drawLine(45,0,45,60,WHITE);
	singleView.drawRect(95,0,120,60,WHITE);
	singleView.setCursor(97,2);
	singleView.print("Bot: ");
	singleView.setCursor(97,12);
	singleView.print(scoreBot);
	singleView.setCursor(97,22);
	singleView.print("Man: ");
	singleView.setCursor(97,32);
	singleView.print(scoreHuman);
	if(botInput == 1) {
		singleView.setCursor(3,2);
		singleView.print("ROCK");
		singleView.drawCircle(20,30,10,WHITE);
	}
	if(botInput == 2) {
		singleView.setCursor(3,2);
		singleView.print("PAPER");
		singleView.drawRect(15,15,20,30,WHITE);
	}
	if(botInput == 3) {
		singleView.setCursor(3,2);
		singleView.print("SCISSOR");
		singleView.drawCircle(25,40,5,WHITE);
		singleView.drawCircle(10,40,5,WHITE);
		singleView.drawLine(20,40,13,25,WHITE);
		singleView.drawLine(15,40,22,25,WHITE);
	}
	if(humanInput == 1) {
		singleView.setCursor(48,2);
		singleView.print("ROCK");
		singleView.drawCircle(60,30,10,WHITE);
	}
	if(humanInput == 2) {
		singleView.setCursor(48,2);
		singleView.print("PAPER");
		singleView.drawRect(60,15,20,30,WHITE);
	}
	if(humanInput == 3) {
		singleView.setCursor(48,2);
		singleView.print("SCISSOR");
		singleView.drawCircle(70,40,5,WHITE);
		singleView.drawCircle(55,40,5,WHITE);
		singleView.drawLine(65,40,58,25,WHITE);
		singleView.drawLine(60,40,67,25,WHITE);
	}
	if(humanInput != 0 && roundWinner == -1)
	{
		singleView.setCursor(23,50);
		singleView.print("1 SECOND");
	}
	if(roundWinner == 0)
	{
		singleView.setCursor(37,50);
		singleView.print("TIE");
	}
	if(roundWinner == 1)
	{
		singleView.setCursor(15,50);
		singleView.print("HUMAN VICTORY");
	}
	if(roundWinner == 2)
	{
		singleView.setCursor(15,50);
		singleView.print("BOT VICTORY");
	}
	singleView.update();
}
