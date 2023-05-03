// Sprite.cpp - 2D display and manipulate texture-mapped quad

#include <glad.h>
#include <GLFW/glfw3.h>
#include <time.h>
#include "Draw.h"
#include "GLXtras.h"
#include "IO.h"
#include "Sprite.h"

Sprite background, player, obstacle, explosion, menuBG, startBt, endBt, buttonreplay;

string backgroundImg_path = "Image/game-bg.png";
string playerImg_path = "Image/car-yellow.png";
string obstacleImg_path = "Image/car-purple.png";
string explosionImg_path = "Image/explosion2.png";
string menuBG_path = "Image/start-bg-w.png";
string startBt_path = "Image/button-play.png";
string endBt_path = "Image/button-exit.png";
string buttonreplay_path = "Image/button-replay.png";

constexpr float windowWidth = 600;
constexpr float windowHeight = 700;
constexpr float leftBoundary = 140.0;
constexpr float rightBoundary = 460.0;
constexpr float startX = 165.0;
constexpr float startY = 40.0;
float startBt_x = 150;
float startBt_y = 350;
float endBt_x = 450;
float endBt_y = 350;

bool isLeftKeyPressed = false;
bool isRightKeyPressed = false;
bool gameover = false;
bool gamerunning = false;

float loopDuration = 2;
time_t startTime;
float elapsedTime = 0;
float vScrollMod = 0;
float obstacleDelay = 2; // wait till display obstacle (in secs.)

vec2 getNormalizedPosition(float x, float y) {
	float x_ndc = 2.0f * x / windowWidth - 1.0f;
	float y_ndc = 2.0f * y / windowHeight - 1.0f;
	return vec2(x_ndc, y_ndc);
}

void Scroll() {
	// calculate u and v
	float v = elapsedTime / loopDuration, u = 0;
	vScrollMod = fmod(v, 1);
	// transform u and v
	background.uvTransform = Translate(u, v, 0);
	// scroll obstacle with background
	if (elapsedTime > obstacleDelay) {
		float obstacleTime = elapsedTime - obstacleDelay;
		vec2 obstaclePos = getNormalizedPosition(startX, windowHeight - vScrollMod * windowHeight);
		obstacle.SetPosition(obstaclePos);
	}
}

void MouseButton(float x, float y, bool left, bool down) { 
	if (left && down) {
		cout << x << ", " << y << endl;
		if (x >= 95.0f && x <= 200.0f && y >= 290.0f && y <= 410.0f) {
			gamerunning = true;
			startTime = clock();
		}
	}
}

void Keyboard(int key, int action, bool shift, bool control) {
	if (key == GLFW_KEY_LEFT) {
		if (action == GLFW_PRESS) {
			isLeftKeyPressed = true;
		}

		if (action == GLFW_RELEASE) {
			isLeftKeyPressed = false;
		}
	}

	if (key == GLFW_KEY_RIGHT) {
		if (action == GLFW_PRESS) {
			isRightKeyPressed = true;
		}

		if (action == GLFW_RELEASE) {
			isRightKeyPressed = false;
		}
	}
}

void Outline(Sprite& s, float width = 2, vec3 color = vec3(1, 1, 0)) {
	UseDrawShader(mat4());
	vec2 pts[] = { s.PtTransform({-1,-1}), s.PtTransform({-1,1}), s.PtTransform({1,1}), s.PtTransform({1,-1}) };
	for (int i = 0; i < 4; i++)
		Line(pts[i], pts[(i + 1) % 4], width, color);
}

void Display() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glClear(GL_DEPTH_BUFFER_BIT);
	// glEnable(GL_DEPTH_TEST);
	
	if (isLeftKeyPressed) {
		vec2 p = player.GetPosition();
		if (p.x > getNormalizedPosition(leftBoundary, startY).x) {
			p.x -= 0.005;
			player.SetPosition(p);
		}
	}

	if (isRightKeyPressed) {
		vec2 p = player.GetPosition();
		if (p.x < getNormalizedPosition(rightBoundary, startY).x) {
			p.x += 0.005;
			player.SetPosition(p);
		}
	}
	time_t now;
	if (gamerunning) {
		now = clock();
		elapsedTime = (float)(now - startTime) / CLOCKS_PER_SEC; // update elapsed time
	}
	
	background.Display();
	player.Display();
	if (elapsedTime > obstacleDelay) {
		obstacle.Display();
		if (obstacle.Intersect(player)){
			// display red outline of obstacle sprite object if collision occurs
			Outline(obstacle, 2, vec3(1, 0, 0));
			gameover = true;
		}	
	}
	glFlush();
}

// Application

void Resize(int width, int height) { glViewport(0, 0, width, height); }

int main(int ac, char** av) {
	GLFWwindow* w = InitGLFW(200, 200, windowWidth, windowHeight, "Doggy");
	// read background, foreground, and mat textures
	menuBG.Initialize(menuBG_path);
	startBt.Initialize(startBt_path);
	endBt.Initialize(endBt_path);
	startBt.SetPosition(getNormalizedPosition(startBt_x, startBt_y));
	endBt.SetPosition(getNormalizedPosition(endBt_x, endBt_y));
	startBt.SetScale(vec2(0.31f, 0.1f));
	endBt.SetScale(vec2(0.31f, 0.1f));
	background.Initialize(backgroundImg_path);
	player.Initialize(playerImg_path);
	player.SetScale(.1f);
	player.SetPosition(getNormalizedPosition(startX, startY));
	obstacle.Initialize(obstacleImg_path);
	obstacle.SetScale(.1f);
	obstacle.SetPosition(getNormalizedPosition(startX, windowHeight));
	explosion.Initialize(explosionImg_path);
	explosion.SetPosition(vec2(.0f, .45f));
	explosion.SetScale(.5f);
	buttonreplay.Initialize(buttonreplay_path);
	buttonreplay.SetPosition(vec2(.0f, .0f));
	buttonreplay.SetScale(vec2(0.31f, 0.1f));
	// callbacks
	RegisterResize(Resize);
	RegisterKeyboard(Keyboard);
	RegisterMouseButton(MouseButton);
	// event loop
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(w)) {
		// display menu
		if (!gamerunning) {
			menuBG.Display();
			startBt.Display();
			endBt.Display();
		}
		else {
			if (!gameover) {
				Scroll();
			}
			Display();

		}
		glfwSwapBuffers(w);
		glfwPollEvents();
	}
	glfwDestroyWindow(w);
	glfwTerminate();
}
