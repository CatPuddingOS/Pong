#include <SDL.h>
#include <iostream>
#include "classes.cpp"
#include "input_tracker.h"

#define WIDTH 1920
#define HEIGHT 1080

#define SIZE 16;

enum ScreenSide
{
	LEFT = 100,
	RIGHT = WIDTH - 100
};

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Color color;
DeltaFrames deltaFrame;
InputTracker hasInput;
bool running = false;

Paddle leftPaddle(HEIGHT, LEFT);
Paddle rightPaddle(HEIGHT, RIGHT);
//Array of all paddle objects. For loops hate him, see how this one simple trick ruined the movement algorithm.
Paddle paddles[] = {leftPaddle, rightPaddle};
Ball ball;
Court court;

/*---DEBUG---*/
void debugInfo()
{
	std::cout << "Left Paddle Values: ";
	leftPaddle.printMovementValues();
	std::cout << std::endl;
}

void CollisionCheck(Paddle &p)
{
	float paddleBottomLimit = p.Yposition + p.verticalHalfSize;
	float paddleTopLimit = p.Yposition - p.verticalHalfSize;
	if (paddleTopLimit < court.topBoundary)
	{
		p.HandleContactingWall(court.topBoundary);
	}
	else if (paddleBottomLimit > court.bottomBoundary)
	{
		p.HandleContactingWall(court.bottomBoundary);
	}
}

//This corrects the mouses' relative position if it strays from the play area. Keeps paddle motion stable when colliding with a border
void CorrectRelativeBoundary(Paddle &paddle)
{
	if (paddle.modifiedYPosition > 815)
	{
		paddle.modifiedYPosition = 815;
	}
	if (paddle.modifiedYPosition < 90)
	{
		paddle.modifiedYPosition = 90;
	}
}

void Update()
{
	if (hasInput.mouseMoving && leftPaddle.previousYPosition == leftPaddle.modifiedYPosition)
	{
		hasInput.mouseMoving = false;
	}

	/*****Wanting to throw a foreach in here but the paddle array doesn't play nice with those for some reason.
	It's as if none of the functions in the loop are firing for either object on call.*****/
	/*FIX: Objects need to be passed by ref not as stale date*/

	//for (Paddle p : paddles)
	//{
	//	//Everything under this works when not in a for loop
	//	/*LEFT PADDLE MOUSE*/
	//	p.MovePaddle(deltaFrame.deltaTime);
	//	CollisionCheck();

	//	//Apply edited Yposition to rendered paddle
	//	p.paddle.y = p.Yposition;
	//	CorrectRelativeBoundary(p);
	//}

	/*LEFT PADDLE MOUSE*/
	leftPaddle.MovePaddle(deltaFrame.deltaTime);
	CollisionCheck(leftPaddle);

	//Apply edited Yposition to rendered paddle
	leftPaddle.paddle.y = leftPaddle.Yposition;
	CorrectRelativeBoundary(leftPaddle);

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
			leftPaddle.modifiedYPosition += e.motion.yrel;
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

	//Helping to create an illusion of falling from the top on spawn
	leftPaddle.Yposition = event.motion.yrel = HEIGHT / 3;
	//rightPaddle.Yposition = event.motion.yrel = HEIGHT / 3;
	SDL_PushEvent(&event);
	//Create color (white)
	color.r = color.g = color.b = 255;

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