#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_SIZE 500
#define CELLS_IN_AXIS 50

typedef struct {
	SDL_Rect rect;
	char alive;
} Cell;

Cell cells [CELLS_IN_AXIS][CELLS_IN_AXIS];
Cell oldCells[CELLS_IN_AXIS][CELLS_IN_AXIS];

void initCells()
{
	time_t t;
	srand((unsigned) time(&t));
	for (int i = 0; i < CELLS_IN_AXIS; i++)
	{
		for (int j = 0; j < CELLS_IN_AXIS; j++)
		{
			cells[i][j].alive = rand() % 2;
			cells[i][j].rect.x = i * 10;
			cells[i][j].rect.y = j * 10;
			cells[i][j].rect.h = 10;
			cells[i][j].rect.w = 10;
		}
	}
}

void renderCells(SDL_Renderer *renderer)
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

int main()
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
		return 1;
	}

	if (SDL_CreateWindowAndRenderer(WINDOW_SIZE, WINDOW_SIZE, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
		return 2;
	}

	initCells();

	while (1) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) {
			break;
		}

		updateCells();
		renderCells(renderer);
		SDL_RenderPresent(renderer);
		SDL_Delay(100);
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}