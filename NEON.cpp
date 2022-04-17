#include <stdio.h>
#include <stdlib.h>
#include <bangtal.h>
#include <time.h>

SceneID scene, home, scene2, scene3;
ObjectID rail[29], ball, startbutton, instructions, endbutton, startbutton2, restartbutton, homebutton;
TimerID rail_timer, ball_timer, count_timer, score_timer, start_timer;

int h = 6, t = 3, bx = 500, by = 300, dbx = 10, dby = 10;
int move_type;
int count = 0;
bool rail_act[29] = { false };

void changeRailState(int i, bool state);
void reflect(void);
void gameover(void);
void start(void);
void start_delay();

void gameover(void) {
	stopTimer(rail_timer);
	stopTimer(ball_timer);
}

void start_delay(void) {
	enterScene(scene);
	setTimer(start_timer, 2.0f);
	startTimer(start_timer);
	srand((unsigned int)time(NULL));
	bx = (int)(rand() % 31) * 10 + 470;
	by = (int)(rand() % 16) * 10 + 230;
	dbx = 10;
	dby = 10;
	locateObject(ball, scene, bx, by);
	count = 0;
	setTimer(count_timer, count);
	//showTimer(count_timer);
}

void start(void) {

	startTimer(ball_timer);

}

void changeRailState(int i, bool state) {
	if (!state) {
		if (i <= 8 || (i <= 22) && (i >= 15)) {
			setObjectImage(rail[i], "Images/rail1.png");
			showObject(rail[i]);
		}
		else {
			setObjectImage(rail[i], "Images/rail2.png");
			showObject(rail[i]);
		}
		rail_act[i] = false;
	}
	else {
		if (i <= 8 || (i <= 22) && (i >= 15)) {
			setObjectImage(rail[i], "Images/rail_1.png");
			showObject(rail[i]);
		}
		else {
			setObjectImage(rail[i], "Images/rail_2.png");
			showObject(rail[i]);
		}
		rail_act[i] = true;
	}
}

void reflect(void) {
	if (bx == 320 && by == 540) {
		if (rail_act[1] && rail_act[28]) {
			dbx *= (-1);
			dby *= (-1);
			setTimer(count_timer, ++count);
		}
		else gameover();
	}
	else if (bx == 320 && by == 80) {
		if (rail_act[22] && rail_act[23]) {
			dbx *= (-1);
			dby *= (-1);
			setTimer(count_timer, ++count);
		}
		else gameover();
	}
	else if (bx == 930 && by == 80) {
		if (rail_act[14] && rail_act[15]) {
			dbx *= (-1);
			dby *= (-1);
			setTimer(count_timer, ++count);
		}
		else gameover();
	}
	else if (bx == 930 && by == 540) {
		if (rail_act[8] && rail_act[9]) {
			dbx *= (-1);
			dby *= (-1);
			setTimer(count_timer, ++count);
		}
		else gameover();
	}
	else if (by == 530) {
		for (int i = 1; i < 9; i++) {
			if (bx >= i * 80 + 250 - 25 && bx <= i * 80 + 250 + 60 - 5) {
				if (rail_act[i] == true) {
					dby *= (-1);
					setTimer(count_timer, ++count);
				}
			}
		}
	}
	else if (bx == 930) {
		for (int i = 9; i < 15; i++) {
			if (by >= (i - 8) * (-80) + 580 - 25 - 10 && by <= (i - 8) * (-80) + 580 + 60 - 5 - 10) {
				if (rail_act[i] == true) {
					dbx *= (-1);
					setTimer(count_timer, ++count);
				}
			}

		}
	}
	else if (by == 80) {
		for (int i = 15; i < 23; i++) {
			if (bx >= (i - 14) * (-80) + 970 - 25 && bx <= (i - 14) * (-80) + 970 + 60 - 5) {
				if (rail_act[i] == true) {
					dby *= (-1);
					setTimer(count_timer, ++count);
				}
			}

		}
	}
	else if (bx == 320) {
		for (int i = 23; i < 29; i++) {
			if (by >= (i - 22) * 80 + 20 - 25 - 10 && by <= (i - 22) * 80 + 20 + 60 - 5 - 10) {
				if (rail_act[i] == true) {
					dbx *= (-1);
					setTimer(count_timer, ++count);
				}
			}
		}
	}
	else if (bx > 940 || bx < 310 || by>550 || by < 80) {
		enterScene(scene3);
		stopTimer(rail_timer);
		stopTimer(ball_timer);
		setTimer(score_timer, count);
	}
	showTimer(count_timer);

}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == startbutton || object == startbutton2) {
		enterScene(scene);
		setTimer(count_timer, count);
		showTimer(count_timer);
		start_delay();
	}
	else if (object == instructions) {
		enterScene(scene2);
	}
	else if (object == endbutton) endGame();
	else if (object == restartbutton) {
		start_delay();
	}
	else if (object == homebutton) enterScene(home);
}

void keyboardCallback(KeyCode keycode, KeyState keystate) {
	if (keycode == KeyCode::KEY_RIGHT_ARROW && keystate == KeyState::KEY_PRESSED) {
		move_type = 1;
		setTimer(rail_timer, 0.05f);
		startTimer(rail_timer);
	}
	else if (keycode == KeyCode::KEY_LEFT_ARROW && keystate == KeyState::KEY_PRESSED) {
		move_type = 2;
		setTimer(rail_timer, 0.05f);
		startTimer(rail_timer);
	}
	else if ((keystate == KeyState::KEY_RELEASED) && (keycode == KeyCode::KEY_RIGHT_ARROW || keycode == KeyCode::KEY_LEFT_ARROW)) stopTimer(rail_timer);
}

void timerCallback(TimerID timer) {
	if (timer == rail_timer) {
		if (move_type == 1) {
			h++;
			t++;
			if (h == 29) {
				h = 1;
				changeRailState(25, false);
			}
			else if (t == 29) {
				t = 1;
				changeRailState(28, false);
			}
			else changeRailState(t - 1, false);
			changeRailState(h, true);
		}
		else if (move_type == 2) {
			h--;
			t--;
			if (h == 0) {
				h = 28;
				changeRailState(1, false);
			}
			else if (t == 0) {
				t = 28;
				changeRailState(4, false);
			}
			else changeRailState(h + 1, false);
			changeRailState(t, true);
		}
		setTimer(rail_timer, 0.05f);
		startTimer(rail_timer);
	}
	else if (timer == ball_timer) {
		bx += dbx;
		by += dby;
		locateObject(ball, scene, bx, by);
		showObject(ball);

		if (count < 3) {
			setTimer(ball_timer, 0.1f);
			startTimer(ball_timer);
		}
		else if (count < 8) {
			setTimer(ball_timer, 0.07f);
			startTimer(ball_timer);
		}
		else if (count < 15) {
			setTimer(ball_timer, 0.05f);
			startTimer(ball_timer);
		}
		else if (count < 24) {
			setTimer(ball_timer, 0.03f);
			startTimer(ball_timer);
		}
		else {
			setTimer(ball_timer, 0.02f);
			startTimer(ball_timer);
		}
		reflect();
	}
	else if (timer == start_timer) {
		start();
	}
}

int main() {

	setKeyboardCallback(keyboardCallback);
	setTimerCallback(timerCallback);
	setMouseCallback(mouseCallback);

	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);

	scene = createScene("배경", "Images/scene.png");
	home = createScene("홈", "Images/home.png");
	scene2 = createScene("설명", "Images/scene2.png");
	scene3 = createScene("끝", "Images/scene3.png");

	ball = createObject("Images/ball.png");
	locateObject(ball, scene, bx, by);
	showObject(ball);

	restartbutton = createObject("Images/restart.png");
	locateObject(restartbutton, scene3, 800, 200);
	scaleObject(restartbutton, 0.6f);
	showObject(restartbutton);

	homebutton = createObject("Images/homebutton.png");
	locateObject(homebutton, scene3, 800, 100);
	scaleObject(homebutton, 0.6f);
	showObject(homebutton);

	startbutton2 = createObject("Images/start.png");
	locateObject(startbutton2, scene2, 1000, 100);
	showObject(startbutton2);

	startbutton = createObject("Images/start.png");
	locateObject(startbutton, home, 800, 450);
	showObject(startbutton);

	instructions = createObject("Images/instructions.png");
	locateObject(instructions, home, 800, 350);
	showObject(instructions);

	endbutton = createObject("Images/end.png");
	locateObject(endbutton, home, 800, 250);
	showObject(endbutton);

	ball_timer = createTimer(0.05f);
	rail_timer = createTimer(0.05f);
	count_timer = createTimer(1.f);
	score_timer = createTimer(0.f);
	start_timer = createTimer(0.f);

	for (int i = 1; i < 29; i++) {
		if (i <= 8) {
			rail[i] = createObject("Images/rail1.png");
			locateObject(rail[i], scene, i * 80 + 250, 560);
		}
		else if (i <= 14) {
			rail[i] = createObject("Images/rail2.png");
			locateObject(rail[i], scene, 960, (i - 8) * (-80) + 570);
		}
		else if (i <= 22) {
			rail[i] = createObject("Images/rail1.png");
			locateObject(rail[i], scene, (i - 14) * (-80) + 970, 60);
		}
		else {
			rail[i] = createObject("Images/rail2.png");
			locateObject(rail[i], scene, 300, (i - 22) * 80 + 10);
		}
		showObject(rail[i]);
	}

	for (int i = 3; i < 7; i++) {
		rail_act[i] = true;
		setObjectImage(rail[i], "Images/rail_1.png");
		showObject(rail[i]);
	}

	startGame(home);
}