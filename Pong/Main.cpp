#include <SDL.h>
#include <iostream>
#include "classes.cpp"
#include "input_tracker.h"

#define WIDTH 1920
#define HEIGHT 1080

#define SIZE 16;

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Color color;
DeltaFrames deltaFrame;
InputTracker hasInput;
bool running = false;

/*previous and modified potsition need to be in a class as: pervious and new*/
float previousYPosition = 0.f;
int modifiedYPosition = 0;

/*Objects must stay for now*/
Paddle leftPaddle;
Paddle rightPaddle;
Ball ball;
Court court;

/*---DEBUG---*/
void debugInfo()
{
	std::cout << "Left Paddle Values: ";
	leftPaddle.printMovementValues();
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

//This corrects the mouses' relative position if it strays from the play area. Keeps paddle motion stable
void CorrectRelativeBoundary()
{
	if (modifiedYPosition > 815)
	{
		modifiedYPosition = 815;
	}
	if (modifiedYPosition < 90)
	{
		modifiedYPosition = 90;
	}
}

void Update()
{
	if (hasInput.mouseMoving && previousYPosition == modifiedYPosition)
	{
		hasInput.mouseMoving = false;
	}
	
	/*MOVEMENT PHYSICS ON BUTTON PRESS
	//if (keyDown.wPressed) { leftPaddle.incrimentAcceleration('+'); }
	//if (keyDown.sPressed) { leftPaddle.incrimentAcceleration('-'); }
	//leftPaddle.movePaddle(deltaTime, leftPaddle.acceleration);
	//leftPaddle.acceleration = 0.f;*/

	//Position with no values applied
	leftPaddle.Yposition = (float)modifiedYPosition;

	/*MOVEMENT PHYSICS ON MOUSEMOVE*/
	leftPaddle.CalculateAcceleration(modifiedYPosition, previousYPosition, deltaFrame.deltaTime);
	previousYPosition = modifiedYPosition;

	//Move the paddle to its next Y position
	leftPaddle.MovePaddle(deltaFrame.deltaTime, leftPaddle.acceleration);
	CollisionCheck();
	leftPaddle.acceleration = 0.f;

	//Apply edited Yposition to rendered paddle
	leftPaddle.paddle.y = leftPaddle.Yposition;
	CorrectRelativeBoundary();

	//std::cout << "Relative Info: " << modifiedYPosition << " " << previousYPosition << " ";
	//debugInfo();
}

/*Probably gonna use this later*/
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
		case SDL_QUIT:	running = false; break;
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{
			case SDLK_w:	hasInput.wPressed = true; break;
			case SDLK_s:	hasInput.sPressed = true; break;
			default:	break;
			}
			break;
		case SDL_KEYUP:
			switch (e.key.keysym.sym)
			{
			case SDLK_w:	hasInput.wPressed = false; break;
			case SDLK_s:	hasInput.sPressed = false; break;
			default:	break;
			}
			break;
		case SDL_MOUSEMOTION:
			hasInput.mouseMoving = true;
			modifiedYPosition += e.motion.yrel;
			e.motion.yrel -= e.motion.yrel;
			break;
		default:	break;
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
	deltaFrame.SetNextTimerFPS();
	deltaFrame.FPSThrottle();

	//Colorize Viewport (Queue)
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

	SDL_RenderFillRect(renderer, &leftPaddle.paddle);
	SDL_RenderFillRect(renderer, &rightPaddle.paddle);
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

	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_Event event;
	event.type = SDL_MOUSEMOTION;
	leftPaddle.Yposition = event.motion.yrel = HEIGHT / 3; //Helping to create an illusion of falling from the top on spawn
	SDL_PushEvent(&event);

	//Create color (white)
	color.r = color.g = color.b = 255;

	leftPaddle.paddle.w = 15;
	leftPaddle.paddle.h = HEIGHT / 6;
	leftPaddle.verticalHalfSize = HEIGHT / 12;
	leftPaddle.paddle.x = 100;
	leftPaddle.paddle.y = leftPaddle.Yposition;
	// leftPaddle.paddle.y = leftPaddle.Yposition = (HEIGHT / 2) + leftPaddle.paddle.h;

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
		deltaFrame.CheckNewFrame(lastTime);

		Update();
		Input();
		//MouseInput();
		Render();

		deltaFrame.CalculateDelta();
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"HelloWorld","SuccessfulBoot",NULL);
	return 0;
}