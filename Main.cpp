// Demo-ClearScreen.cpp

#include <glad.h>                                         
#include <glfw3.h>                                        
#include "GLXtras.h" 
#include "Game.h"
#include "Config.h"

Game* game;

void Keyboard(int key, bool press, bool shift, bool control) {
	
}

void MouseButton(float x, float y, bool left, bool down) {
	if (down && game->menu.isClickStart(x, y)) {
		cout << "start" << endl;
	}

	if (down && game->menu.isClickExit(x, y)) {
		cout << "exit" << endl;
	}
}

int main() {
	GLFWwindow *w = InitGLFW(600, 200, windowWidth, windowHeight, "Doggy");
	RegisterKeyboard(Keyboard);
	RegisterMouseButton(MouseButton);

	game = new Game();
	while (!glfwWindowShouldClose(w)) {
		glfwPollEvents();
		// set background color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		                    
		game->draw();

		while (game->isPlay) {
			game->play();
			glfwSwapBuffers(w);
		}
		                                       
		glfwSwapBuffers(w);                          
	}

	glfwDestroyWindow(w);
	glfwTerminate();
	delete game;
	return 0;
}
