#include "include/raylib.h"
#include <vector>
#include <cmath>
#include <utility>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#define TRUE_SPEED 4.5f

/*
	ce qui rest :
				- les POMMES !
				- la mort quand on touche autre chose qu'une pomme/rien
				- les menus
				- l'affichage du score
				- les particule/ le polishing
*/

bool horiz = true;
bool verti = false;
bool died = false;


static inline void makeSnakeGreatAgain(std::vector<std::vector<char>> &map, bool &apple, std::pair<unsigned int, unsigned int> &head, std::pair<unsigned int, unsigned int> &tail)
{
	int i = head.first;
	int j = head.second;
	unsigned int cpt = 0;
	char smallestNum = 1;
	std::pair<unsigned int, unsigned int> lastSnakePart = head;

	while (map[i][j] != (char)254)
	{
		lastSnakePart.first = i;
		lastSnakePart.second = j;

		if(i - 1 >= 0 && (map[i - 1][j] == static_cast<char>(smallestNum)))
		{
			++cpt;
			--i;
		}
		else if(i + 1 < map.size() && (map[i + 1][j] == static_cast<char>(smallestNum)))
		{
			++cpt;
			++i;
		}
		else if(j - 1 >= 0 && (map[i][j - 1] == static_cast<char>(smallestNum)))
		{
			++cpt;
			--j;
		}
		else if(j + 1 < map[i].size() && (map[i][j + 1] == static_cast<char>(smallestNum)))
		{
			++cpt;
			++j;
		}
		else if(i - 1 >= 0 && map[i - 1][j] == static_cast<char>(smallestNum + 1))
			--i;
		else if(i + 1 < map.size() && map[i + 1][j] == static_cast<char>(smallestNum + 1))
			++i;
		else if(j - 1 >= 0 && map[i][j - 1] == static_cast<char>(smallestNum + 1))
			--j;
		else if(j + 1 < map[i].size() && map[i][j + 1] == static_cast<char>(smallestNum + 1))
			++j;
		else if(i - 1 >= 0 && map[i - 1][j] == (char)254)
			--i;
		else if(i + 1 < map.size() && map[i + 1][j] == (char)254)
			++i;
		else if(j - 1 >= 0 && map[i][j - 1] == (char)254)
			--j;
		else if(j + 1 < map[i].size() && map[i][j + 1] == (char)254)
			++j;


		if (map[i][j] == static_cast<char>(smallestNum + 1))
		{
			++smallestNum;
			cpt = 0;
		}
		if (cpt > 2)
		{
			map[i][j] = static_cast<char>(smallestNum + 1);
			cpt = 0;
		}
	}
	if (!apple)
	{
		map[lastSnakePart.first][lastSnakePart.second] = (char)254;
		map[tail.first][tail.second] = (char)0;
	}
}

static inline void moveSnake(std::vector<std::vector<char>> &map, int offsetJ, int offsetI)
{
	std::pair<unsigned int, unsigned int> head;
	std::pair<unsigned int, unsigned int> tail;
	bool apple = false;
	bool h = false;
	bool t = false;

	for (unsigned int i = 0; i < map.size(); i++)
	{
		for (unsigned int j = 0; j < map[i].size(); j++)
		{
			if (map[i][j] == (char)1 && !h)
			{
				h = true;
				if (((i + offsetI) >= map.size()) || ((j + offsetJ) >= map[i].size()) || (i + offsetI) < 0 || (j + offsetJ) < 0)
				{
					died = true;
					return ;
				}
				if (map[i + offsetI][j + offsetJ] == (char)0 || map[i + offsetI][j + offsetJ] == (char)253)
				{
					if (map[i + offsetI][j + offsetJ] == (char)253)
					{
						apple = true;
						{
							int i = rand() % map.size(), j = rand() % map[0].size();
							while (1)
								if (map[(i = rand() % map.size())][(j = rand() % map[0].size())] == static_cast<char>(0))
									break ;
							map[i][j] = static_cast<char>(253);
						}
					}
					map[i + offsetI][j + offsetJ] = (char)1;
					map[i][j] = (char)2;
					if (offsetJ == 0)
						horiz = false;
					if (offsetI == 0)
						verti = false;
					head.first = i + offsetI;
					head.second = j + offsetJ;
				}
				else
				{
					died = true;
					return ;
				}
			}
			else if (map[i][j] == (char)254)
			{
				tail.first = i;
				tail.second = j;
				t = true;
			}
			if (h && t)
			{
				makeSnakeGreatAgain(map, apple, head, tail);
				return ;
			}
		}
	}
}

static inline void updateMap(std::vector<std::vector<char>> &map, float const &deltaTime, std::pair <float, float> &directionSpeed, std::pair <float, float> &tmpVel)
{
	if (IsKeyDown(KEY_UP) && !verti)
	{;
		directionSpeed.first = 0;
		directionSpeed.second = -TRUE_SPEED;
		tmpVel.second -= (TRUE_SPEED * deltaTime + std::abs(tmpVel.first));
		tmpVel.first = 0;
		verti = true;
	}
	else if (IsKeyDown(KEY_DOWN) && !verti)
	{
		directionSpeed.first = 0;
		directionSpeed.second = TRUE_SPEED;
		tmpVel.second += (TRUE_SPEED * deltaTime + std::abs(tmpVel.first));
		tmpVel.first = 0;
		verti = true;
	}
	else if (IsKeyDown(KEY_RIGHT) && !horiz)
	{
		directionSpeed.first = TRUE_SPEED;
		directionSpeed.second = 0;
		tmpVel.first += (TRUE_SPEED * deltaTime + std::abs(tmpVel.second));
		tmpVel.second = 0;
		horiz = true;
	}
	else if (IsKeyDown(KEY_LEFT) && !horiz)
	{
		directionSpeed.first = -TRUE_SPEED;
		directionSpeed.second = 0;
		tmpVel.first -= (TRUE_SPEED * deltaTime + std::abs(tmpVel.second));
		tmpVel.second = 0;
		horiz = true;
	}
	else
	{
		tmpVel.first += (directionSpeed.first * deltaTime);
		tmpVel.second += (directionSpeed.second * deltaTime);
	}

	// std::cout << "Delta :" << deltaTime << std::endl;

	// std::cout << "directionSpeed.first :" << directionSpeed.first << std::endl;
	// std::cout << "directionSpeed.second :" << directionSpeed.second << std::endl;
	// std::cout << "tmpVel.first :" << tmpVel.first << std::endl;
	// std::cout << "tmpVel.second :" << tmpVel.second << std::endl;
	if (tmpVel.first >= 1.0f)
	{
		moveSnake(map, 1, 0);
		tmpVel.first = 0.0f;
	}
	else if (tmpVel.first <= -1.0f)
	{
		moveSnake(map, -1, 0);
		tmpVel.first = 0.0f;
	}
	else if (tmpVel.second <= -1.0f)
	{
		moveSnake(map, 0, -1);
		tmpVel.second = 0.0f;
	}
	else if (tmpVel.second >= 1.0f)
	{
		moveSnake(map, 0, 1);
		tmpVel.second = 0.0f;
	}
}

static inline void playDrawing(unsigned int &width, unsigned int &height, unsigned int &gridOffset, std::vector<std::vector<char>> &map)
{
	// DrawRectangleLines(0, 0, width, height, LIGHTGRAY);
	// for (size_t i = gridOffset; i < width; i += gridOffset)
	// {
	// 	DrawLine(i - 1, 0, i - 1, height, LIGHTGRAY);
	// 	DrawLine(i, 0, i, height, LIGHTGRAY);
	// 	DrawLine(i + 1, 0, i + 1, height, LIGHTGRAY);
	// }
	// for (size_t i = gridOffset; i < height; i += gridOffset)
	// {
	// 	DrawLine(0, i - 1, width, i - 1, LIGHTGRAY);
	// 	DrawLine(0, i, width, i, LIGHTGRAY);
	// 	DrawLine(0, i + 1, width, i + 1, LIGHTGRAY);
	// }

	for(unsigned int i = 0; i < map.size(); i++)
	{
		for(unsigned int j = 0; j < map[i].size(); j++)
		{
			if (map[i][j] == 1)
				DrawRectangle(j * gridOffset, i * gridOffset, gridOffset, gridOffset, RED);
			else if (map[i][j] > 1)
				DrawRectangle((j * gridOffset) + 10, (i * gridOffset) + 10, gridOffset - 20, gridOffset - 20, MAROON);
			else if (map[i][j] == (char)254)
				DrawRectangle((j * gridOffset) + 10, (i * gridOffset) + 10, gridOffset - 20, gridOffset - 20, GREEN);
			else if (map[i][j] == (char)253)
				DrawCircle(j * gridOffset + gridOffset/2, i * gridOffset + gridOffset/2, 9.0f, GREEN);
		}
	}

	if (died)
		DrawText("YOU DIED", 290, 300, 100, RED);
}

static inline void menuDrawing(unsigned int &width, unsigned int &height, Texture2D &trueLogo, bool &menu, Rectangle &rec)
{


	// Rectangle rec2;
	// rec2.x = width / 2 - 151;
	// rec2.y = trueLogo.height + 30;
	// rec2.height = 121;
	// rec2.width = 302;

	DrawTexture(trueLogo, width / 2 - trueLogo.width / 2, 1, WHITE);

	if (CheckCollisionPointRec(GetMousePosition(), rec))
	{
		DrawRectangleRounded(rec, 0.3f, 0, MAROON);
		DrawText("Jouer", width / 2 - 60, trueLogo.height + 70, 40, RAYWHITE);
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			menu = false;
	}
	else
	{
		DrawRectangleRounded(rec, 0.3f, 0, RED);
		DrawText("Jouer", width / 2 - 60, trueLogo.height + 70, 40, LIGHTGRAY);
	}
	DrawRectangleRoundedLines({width / 2 - 150, trueLogo.height + 30, 300, 120}, 0.3f, 0, 3, LIGHTGRAY);

	// if (CheckCollisionPointRec(GetMousePosition(), rec2))
	// {
	// 	DrawRectangleRounded(rec2, 0.3f, 0, MAROON);
	// 	DrawText("Jouer", width / 2 - 60, trueLogo.height + 70, 40, RAYWHITE);
	// 	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	// 		menu = false;
	// }
	// else
	// {
	// 	DrawRectangleRounded(rec, 0.3f, 0, RED);
	// 	DrawText("Jouer", width / 2 - 60, trueLogo.height + 70, 40, LIGHTGRAY);
	// }

}

int main(void)
{
	unsigned int width = 1080;
	unsigned int height = 720;
	unsigned int gridOffset = 45;
	bool menu = true;
	float deltaTime;
	float timer = 0.0f;
	std::pair <float, float> tmpVel = {0, 0};
	std::pair <float, float> directionSpeed = {TRUE_SPEED, 0};
	std::vector<std::vector<char>> map;
	Image logo;
	Texture2D trueLogo;

	map.resize(height / gridOffset);
	for (unsigned int i = 0; i < (height / gridOffset); i++)
		map[i].resize(width / gridOffset);

	for(unsigned int i = 0; i < map.size(); i++)
		for(unsigned int j = 0; j < map[i].size(); j++)
			map[i][j] = (char)0;
	map[7][6] = (char)1;
	map[7][5] = (char)2;
	map[7][4] = (char)254;

	srand(time(NULL));

	{
		int i = rand() % map.size(), j = rand() % map[0].size();
		while (1)
			if (map[(i = rand() % map.size())][(j = rand() % map[0].size())] == static_cast<char>(0))
				break ;
		map[i][j] = static_cast<char>(253);
	}

	InitWindow(width, height, "Snake");

	logo = LoadImage("ressources/logo.png");

	SetWindowIcon(logo);

	trueLogo = LoadTextureFromImage(logo);
	UnloadImage(logo);

	Rectangle rec;
	rec.x = width / 2 - 151;
	rec.y = trueLogo.height + 30;
	rec.height = 121;
	rec.width = 302;

	SetTargetFPS(20);

	while (!WindowShouldClose())
	{
		deltaTime = GetFrameTime();
		if (!died)
			updateMap(map, deltaTime, directionSpeed, tmpVel);

			// for(int i = 0; i < map.size(); i++)
			// {
			// 	for(int j = 0; j < map[i].size(); j++)
			// 	{
			// 		if (map[i][j] == (char)0)
			// 			std::cout << ".";
			// 		else if (map[i][j] == (char)253)
			// 			std::cout << "P";
			// 		else
			// 			std::cout << "S";
			// 		// std::cout << static_cast<unsigned int>(map[i][j]);
			// 	}
			// 	std::cout << std::endl;
			// }
			// std::cout << std::endl << std::endl;

		BeginDrawing();

			ClearBackground(BLACK);
			if (menu)
				menuDrawing(width, height, trueLogo, menu, rec);
			else
				playDrawing(width, height, gridOffset, map);

			if (died)
			{
				timer += 1.0f * deltaTime;
				if (timer >= 1.5f)
				{
					died = false;
					menu = true;
					timer = 0.0f;
					for(unsigned int i = 0; i < map.size(); i++)
						for(unsigned int j = 0; j < map[i].size(); j++)
							map[i][j] = (char)0;
					map[7][6] = (char)1;
					map[7][5] = (char)2;
					map[7][4] = (char)254;
					{
						int i = rand() % map.size(), j = rand() % map[0].size();
						while (1)
							if (map[(i = rand() % map.size())][(j = rand() % map[0].size())] == static_cast<char>(0))
								break ;
						map[i][j] = static_cast<char>(253);
					}
					directionSpeed = {TRUE_SPEED, 0};
					tmpVel = {0, 0};
					horiz = true;
					verti = false;
				}
			}

		EndDrawing();
	}
	UnloadTexture(trueLogo);
}