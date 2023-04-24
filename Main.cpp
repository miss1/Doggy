// Sprite.cpp - 2D display and manipulate texture-mapped quad

#include <glad.h>
#include <GLFW/glfw3.h>
#include <time.h>
#include "GLXtras.h"
#include "IO.h"
#include "Sprite.h"

Sprite background, actor;

string backgroundImg_path = "Image/game-bg.png", playerImg_path = "Image/car-yellow.png";

constexpr float windowWidth = 600;
constexpr float windowHeight = 700;
constexpr float leftBoundary = 140.0;
constexpr float rightBoundary = 460.0;
constexpr float startX = 165.0;
constexpr float startY = 40.0;

bool isLeftKeyPressed = false;
bool isRightKeyPressed = false;

bool	scrolling = true, vertically = true;

float	loopDuration = 2, accumulatedVTime = 0, accumulatedHTime = 0;
time_t	scrollTime = clock();

void Scroll() {
	time_t now = clock();
	if (scrolling) {
		float dt = (float)(now - scrollTime) / CLOCKS_PER_SEC;
		(vertically ? accumulatedVTime : accumulatedHTime) += dt;
	}
	scrollTime = now;
	float v = accumulatedVTime / loopDuration, u = accumulatedHTime / loopDuration;
	background.uvTransform = Translate(u, v, 0);
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

vec2 getNormalizedPosition(float x, float y) {
	float x_ndc = 2.0f * x / windowWidth - 1.0f;
	float y_ndc = 2.0f * y / windowHeight - 1.0f;
	return vec2(x_ndc, y_ndc);
}

void Display() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glClear(GL_DEPTH_BUFFER_BIT);
	// glEnable(GL_DEPTH_TEST);
	
	if (isLeftKeyPressed) {
		vec2 p = actor.GetPosition();
		if (p.x > getNormalizedPosition(leftBoundary, startY).x) {
			p.x -= 0.005;
			actor.SetPosition(p);
		}
	}

	if (isRightKeyPressed) {
		vec2 p = actor.GetPosition();
		if (p.x < getNormalizedPosition(rightBoundary, startY).x) {
			p.x += 0.005;
			actor.SetPosition(p);
		}
	}

	background.Display();
	actor.Display();
	glFlush();
}

// Application

void Resize(int width, int height) { glViewport(0, 0, width, height); }

int main(int ac, char** av) {
	GLFWwindow* w = InitGLFW(200, 200, windowWidth, windowHeight, "Doggy");
	// read background, foreground, and mat textures
	background.Initialize(backgroundImg_path);
	actor.Initialize(playerImg_path);
	actor.SetScale(.1f);
	actor.SetPosition(getNormalizedPosition(startX, startY));
	// callbacks
	RegisterResize(Resize);
	RegisterKeyboard(Keyboard);
	// event loop
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(w)) {
		Scroll();
		Display();
		glfwSwapBuffers(w);
		glfwPollEvents();
	}
}
