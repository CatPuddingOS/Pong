#include <SDL.h>
#include <iostream>
#include "classes.cpp"
#include "key_tracking.h"

#define WIDTH 1920
#define HEIGHT 1080

#define SIZE 16;

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Color color;
Keytracker keyDown;
bool running = false;
int frameCount, timerFPS, thisFrame, lastFrame, fps;
float deltaTime;

//Declare objects
//SDL_Rect leftPaddle, rightPaddle, ball;
//float velocityY, velocityX;

LeftPaddle leftPaddle;
RightPaddle rightPaddle;
Ball ball;
Court court;

void debugInfo()
{
	std::cout << "Left Paddle Values: ";
	leftPaddle.printMovementValues();
	std::cout << "Right Paddle Values: ";
	rightPaddle.printMovementValues();
	std::cout << std::endl;
}

void CollisionCheck()
{
	float paddleBottomLimit = leftPaddle.Yposition + leftPaddle.verticalHalfSize;
	float paddleTopLimit = leftPaddle.Yposition - leftPaddle.verticalHalfSize;
	if (paddleTopLimit < court.topBoundary)
	{
		leftPaddle.HandleContactingWall(court.topBoundary);
	}
	else if (paddleBottomLimit > court.bottomBoundary)
	{
		leftPaddle.HandleContactingWall(court.bottomBoundary);
	}
}

bool DidItMove(float previousPosition)
{
	if (leftPaddle.Yposition > previousPosition || leftPaddle.Yposition < previousPosition)
	{
		return true;
	}
	return false;
}

void Update()
{
	float previousPosition = leftPaddle.Yposition;
	//Constraint = Upper:90, Lower:810
	if (keyDown.moveToPosition)
	{
		keyDown.moveToPosition = false;
		previousPosition = leftPaddle.Yposition;
		SDL_GetMouseState(NULL, &leftPaddle.leftPaddle.y);
		leftPaddle.Yposition = leftPaddle.leftPaddle.y;
	}
	/*MOVEMENT PHYSICS ON BUTTON PRESS
	TODO: Get direction mouse was moved in to apply physics to mouse movement*/
	//if (keyDown.wPressed) { leftPaddle.incrimentAcceleration('+'); }
	//if (keyDown.sPressed) { leftPaddle.incrimentAcceleration('-'); }
	//leftPaddle.movePaddle(deltaTime, leftPaddle.acceleration);
	//leftPaddle.acceleration = 0.f;
	if (DidItMove(previousPosition)) { leftPaddle.incrimentAcceleration('+'); }
	if (DidItMove(previousPosition)) { leftPaddle.incrimentAcceleration('-'); }
	leftPaddle.movePaddle(deltaTime, leftPaddle.acceleration);
	leftPaddle.acceleration = 0.f;

	CollisionCheck();
	leftPaddle.leftPaddle.y = leftPaddle.Yposition;
	debugInfo();
}

void MouseInput()
{
	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			running = false;
			break;
		case SDL_MOUSEMOTION:
			break;
		default:
			break;
		}
	}
}

void Input()
{
	SDL_Event e;
	const Uint8* keyStates = SDL_GetKeyboardState(NULL);

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{
			case SDLK_w:	keyDown.wPressed = true; break;
			case SDLK_s:	keyDown.sPressed = true; break;
			default:
				break;
			}
			break;
		case SDL_KEYUP:
			switch (e.key.keysym.sym)
			{
			case SDLK_w:	keyDown.wPressed = false; break;
			case SDLK_s:	keyDown.sPressed = false; break;
			default:
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			keyDown.moveToPosition = true;
			break;
		default:
			break;
		}

		if (keyStates[SDL_SCANCODE_ESCAPE])
		{
			running = false;
		}
	}
}

void Render()
{
	//SetRenderer (Pre-Load)
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	//Recheck Frame Count (Delta)
	frameCount++;
	timerFPS = SDL_GetTicks64() - lastFrame;
	if (timerFPS < (1000 / 60))
	{
		SDL_Delay((1000 / 60) - timerFPS);
	}

	//Colorize Viewport (Queue)
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

	SDL_RenderFillRect(renderer, &leftPaddle.leftPaddle);
	SDL_RenderFillRect(renderer, &rightPaddle.rightPaddle);
	SDL_RenderFillRect(renderer, &ball.ball);

	//Present Renering (Draw)
	SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[])
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) { std::cout << "### SDL_Init() has failed ###" << std::endl; }
	//Create the window and prepare the renderer
	if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) < 0) { std::cout << "SDL_Create() has failed" << std::endl; }
	running = true;
	static int lastTime = 0;

	//Create color (white)
	color.r = color.g = color.b = 255;

	leftPaddle.leftPaddle.w = 15;
	leftPaddle.leftPaddle.h = HEIGHT / 6;
	leftPaddle.verticalHalfSize = HEIGHT / 12;
	leftPaddle.leftPaddle.x = 100;
	leftPaddle.leftPaddle.y = leftPaddle.Yposition = (HEIGHT / 2) - (leftPaddle.leftPaddle.h / 2);

	/*rightPaddle.rightPaddle.w = 15;
	rightPaddle.rightPaddle.h = HEIGHT / 6;
	ritghPaddle.verticalHalfSize = HEIGHT / 12;
	rightPaddle.rightPaddle.x = WIDTH - 100;
	rightPaddle.rightPaddle.y = rightPaddle.Yposition = (HEIGHT / 2) - (rightPaddle.rightPaddle.h / 2)*/;

	ball.ball.w = SIZE - 8;
	ball.ball.h = SIZE - 8;
	ball.ball.x = WIDTH / 2;
	ball.ball.y = HEIGHT / 2;

	//Court dimensions
	court.topBoundary = 0, court.bottomBoundary = 900;
	court.leftBoundary = -WIDTH, court.rightBoundary = WIDTH;

	while (running)
	{
		lastFrame = SDL_GetTicks64();
		if (lastFrame >= (lastTime + 1000))
		{
			lastTime = lastFrame;
		}

		Update();
		Input();
		//MouseInput();
		Render();

		deltaTime = (SDL_GetTicks() - lastFrame) / 1000.f;
		//deltaTime = (float)((SDL_GetTicks64() - lastFrame) * 1000.f / (float)SDL_GetPerformanceFrequency());
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"HelloWorld","SuccessfulBoot",NULL);
	return 0;
}