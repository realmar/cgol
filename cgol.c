#include <SDL2/SDL.h>
#include <stdlib.h>

#define WINDOW_SIZE 1000
#define CELLS_IN_AXIS 100
#define MAX_DELAY 500

typedef struct {
	SDL_Rect rect;
	char alive;
} Cell;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

Cell cells [CELLS_IN_AXIS][CELLS_IN_AXIS];
Cell oldCells[CELLS_IN_AXIS][CELLS_IN_AXIS];

int paused = 0;
int delayTime = 100;
unsigned int lastTime = 0, currentTime;

int currentMouseGridX = 0;
int currentMouseGridY = 0;

void addDelayTime(int addedTime) {
	int newTime = delayTime + addedTime;
	delayTime = newTime < 0 ? 0 : newTime;
	delayTime = newTime > MAX_DELAY ? MAX_DELAY : delayTime;
}

void initCells(int blank)
{
	srand(SDL_GetTicks());
	for (int i = 0; i < CELLS_IN_AXIS; i++)
	{
		for (int j = 0; j < CELLS_IN_AXIS; j++)
		{
			cells[i][j].alive = blank ? 0 : rand() % 2;

			int cellSize = WINDOW_SIZE / CELLS_IN_AXIS;

			cells[i][j].rect.x = i * cellSize;
			cells[i][j].rect.y = j * cellSize;
			cells[i][j].rect.h = cellSize;
			cells[i][j].rect.w = cellSize;
		}
	}
}

void renderCells()
{
	for (int i = 0; i < CELLS_IN_AXIS; i++)
	{
		for (int j = 0; j < CELLS_IN_AXIS; j++)
		{
			int color = cells[i][j].alive ? 255 : 0;
			SDL_SetRenderDrawColor(renderer, color, color, color, 255);
			SDL_RenderFillRect(renderer, &cells[i][j].rect);
		}
	}
}

void renderFeintCell(int x, int y) {
	int cellSize = WINDOW_SIZE / CELLS_IN_AXIS;
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	SDL_RenderFillRect(renderer, &cells[x][y].rect);
}

int countNeighbours(int x, int y)
{
	int coordTop = y - 1 >= 0 ? (y - 1) : CELLS_IN_AXIS - 1;
	int coordBottom = y + 1 < CELLS_IN_AXIS ? (y + 1) : 0;
	int coordRight = x + 1 < CELLS_IN_AXIS ? (x + 1) : 0;
	int coordLeft = x - 1 >= 0 ? (x - 1) : CELLS_IN_AXIS - 1;

	int top = oldCells[x][coordTop].alive;
	int bottom = oldCells[x][coordBottom].alive;
	int right = oldCells[coordRight][y].alive;
	int left = oldCells[coordLeft][y].alive;
	int topRight = oldCells[coordRight][coordTop].alive;
	int topLeft = oldCells[coordLeft][coordTop].alive;
	int bottomRight = oldCells[coordRight][coordBottom].alive;
	int bottomLeft = oldCells[coordLeft][coordBottom].alive;

	return top + bottom + right + left + topRight + topLeft + bottomRight + bottomLeft;
}

void updateCells()
{
	memcpy(&oldCells, &cells, sizeof(cells));

	for (int i = 0; i < CELLS_IN_AXIS; i++)
	{
		for (int j = 0; j < CELLS_IN_AXIS; j++)
		{
			int neighbours = countNeighbours(i, j);

			Cell* cell = &cells[i][j];

			if (cell->alive && neighbours < 2) {
				cell->alive = 0;
			}
			else if (cell->alive && (neighbours == 2 || neighbours == 3)) {
				cell->alive = 1;
			}
			else if (cell->alive && neighbours > 3){
				cell->alive = 0;
			}
			else if (cell->alive == 0 && neighbours == 3) {
				cell->alive = 1;
			}
		}
	}
}

void handleKeyInput(SDL_KeyboardEvent* keyEvent) {
	switch (keyEvent->keysym.sym) {
		case SDLK_r:
			initCells(0);
			break;
		case SDLK_c:
			initCells(1);
			break;
		case SDLK_t:
			paused = !paused;
			break;
		case SDLK_a:
			addDelayTime(-50);
			break;
		case SDLK_s:
			addDelayTime(50);
			break;
	}
}

void handleMouseMotion(SDL_MouseMotionEvent* mouseEvent) {
	int cellSize = WINDOW_SIZE / CELLS_IN_AXIS;
	currentMouseGridX =  mouseEvent->x / cellSize;
	currentMouseGridY = mouseEvent->y / cellSize;

	if (mouseEvent->state & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		cells[currentMouseGridX][currentMouseGridY].alive = 1;
	}
	else if (mouseEvent->state & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
		cells[currentMouseGridX][currentMouseGridY].alive = 0;
	}
}

void handleMouseButton(SDL_MouseButtonEvent* mouseEvent) {
	if (mouseEvent->button == SDL_BUTTON_LEFT) {
		cells[currentMouseGridX][currentMouseGridY].alive = 1;
	}
	else if (mouseEvent->button == SDL_BUTTON_RIGHT) {
		cells[currentMouseGridX][currentMouseGridY].alive = 0;
	}
}


int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
		return 1;
	}

	if (SDL_CreateWindowAndRenderer(WINDOW_SIZE, WINDOW_SIZE, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
		return 2;
	}

	initCells(0);

	while (1) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) {
			break;
		}
		if (event.type == SDL_KEYDOWN) {
			handleKeyInput(&event.key);
		}
		if (event.type == SDL_MOUSEMOTION) {
			handleMouseMotion(&event.motion);
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			handleMouseButton(&event.button);
		}

		currentTime = SDL_GetTicks();
		if (currentTime >= lastTime + delayTime) {
			if (!paused) {
				updateCells();
			}
			lastTime = currentTime;
		}

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(renderer);
		renderCells();
		renderFeintCell(currentMouseGridX, currentMouseGridY);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}