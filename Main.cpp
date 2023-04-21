// Sprite.cpp - 2D display and manipulate texture-mapped quad

#include <glad.h>
#include <GLFW/glfw3.h>
#include "GLXtras.h"
#include "Sprite.h"

Sprite background, actor;

constexpr auto windowWidth = 600;
constexpr auto windowHeight = 700;
constexpr auto leftBoundary = 140.0;
constexpr auto rightBoundary = 460.0;
constexpr auto startX = 165.0;
constexpr auto startY = 40.0;

bool isLeftKeyPressed = false;
bool isRightKeyPressed = false;

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
	return vec2 (x_ndc, y_ndc);
}

// Application

void Resize(int width, int height) { glViewport(0, 0, width, height); }

int main(int ac, char** av) {
	GLFWwindow* w = InitGLFW(200, 200, windowWidth, windowHeight, "Doggy");
	// read background, foreground, and mat textures
	// sprite1.Initialize(pix1, .5f); sprite1.SetScale(.3f);
	background.Initialize("Image/game-bg.png");
	actor.Initialize("Image/car-yellow.png");
	actor.SetScale(.1f);
	actor.SetPosition(getNormalizedPosition(startX, startY));
	// callbacks
	RegisterResize(Resize);
	RegisterKeyboard(Keyboard);
	// event loop
	while (!glfwWindowShouldClose(w)) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
		glfwSwapBuffers(w);
		glfwPollEvents();
	}
}
