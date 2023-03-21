#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<iostream>
#include<string>
#include<vector>
#include"Vector2D.h"
#include"json.hpp"
#include"AssetsManager.h"
#include"InputHandler.h"
#include"game.h"

//the game class
Game* Game::s_pInstance = 0;

Game::Game() {
	m_pRenderer = nullptr;
	m_pWindow = nullptr;
}

Game::~Game() {}

SDL_Window* g_pWindow = 0;
SDL_Renderer* g_pRenderer = 0;

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	m_gameWidth = width;
	m_gameHeight = height;

	// initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		int flags = SDL_WINDOW_SHOWN;
		if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN;

		// if succeeded create our window
		std::cout << "SDL init success\n";
		m_pWindow = SDL_CreateWindow(title,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			width, height, flags);
		// if the window creation succeeded create our renderer
		if (m_pWindow != 0)
		{
			std::cout << "window creation success\n";
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			if (m_pRenderer != 0)
			{
				std::cout << "renderer creation success\n";
				SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
			}
			else
			{
				std::cout << "renderer init fail\n";
				return false;
			}
		}
		else
		{
			std::cout << "window init fail\n";
			return false;
		}
	}
	else
	{
		std::cout << "SDL init fail\n";
		return false; // sdl could not initialize
	}

	if (TTF_Init() == 0)
	{
		std::cout << "sdl font initialization success\n";
	}
	else
	{
		std::cout << "sdl font init fail\n";
		return false;
	}

	std::cout << "init success\n";
	m_bRunning = true;

	//pass the renderer to the assets manager
	AssetsManager::Instance()->renderer = m_pRenderer;
	//load all the assets in the json file
	AssetsManager::Instance()->loadAssetsJson();

	g_pRenderer = m_pRenderer;
	g_pWindow = m_pWindow;
	return true;
}

void Game::handleEvents() {
	InputHandler::Instance()->update();

	xA = objetos_zona[zona][0][2]; //position on screen
	yA = objetos_zona[zona][0][3];
	zA = objetos_zona[zona][0][4];

	choque = 0;
	if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_P)) {
		sentido = AUM_X;
		if (xA < 72) {
			for (obj = 1; obj < NUM_OBJETOS; obj++) {
				if (!(x[obj] > x[0] + 8 || x[obj] < x[0] + 8 || y[obj] - y[0] >= 8 || y[0] - y[obj] >= 8 || z[obj] - z[0] >= 8 || z[0] - z[obj] >= 8))
					choque = 1;
			}
			if (choque == 0) { xA++; objetos_zona[zona][0][2]++; }
		}
		else if (mapa[zona_z][zona_x + 1] != 0)
		{
			//change zone.
			zona_x++;
			zona = mapa[zona_z][zona_x] - 1;
			objetos_zona[zona][0][2] = 0;
			objetos_zona[zona][0][3] = yA;
			objetos_zona[zona][0][4] = zA;
		}
	}
	
	choque = 0;
	if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_A)) {
		sentido = AUM_Z;
		if (zA < 72) {
			for (obj = 1; obj < NUM_OBJETOS; obj++) {
				if (!(x[obj] - x[0] >= 8 || x[0] - x[obj] >= 8 || y[obj] - y[0] >= 8 || y[0] - y[obj] >= 8 || z[obj] > z[0] + 8 || z[obj] < z[0] + 8))
					choque = 1;
			}
			if (choque == 0) { zA++; objetos_zona[zona][0][4]++; }
		}
		else if (mapa[zona_z + 1][zona_x] != 0)
		{
			zona_z++;
			zona = mapa[zona_z][zona_x] - 1;
			objetos_zona[zona][0][2] = xA;
			objetos_zona[zona][0][3] = yA;
			objetos_zona[zona][0][4] = 0;
		}
	}

	choque = 0;
	if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_O)) {
		sentido = DIS_X;
		if (xA > 0) {
			for (obj = 1; obj < NUM_OBJETOS; obj++) {
				if (!(x[obj]<x[0] - 8 || x[obj]>x[0] - 8 || y[obj] - y[0] >= 8 || y[0] - y[obj] >= 8 || z[obj] - z[0] >= 8 || z[0] - z[obj] >= 8))
					choque = 1;
			}
			if (choque == 0) { xA--; objetos_zona[zona][0][2]--; }
		}
		else if (mapa[zona_z][zona_x - 1] != 0)
		{
			zona_x--;
			zona = mapa[zona_z][zona_x] - 1;
			objetos_zona[zona][0][2] = 72;
			objetos_zona[zona][0][3] = yA;
			objetos_zona[zona][0][4] = zA;
		}
	}

	choque = 0;
	if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_Q)) {
		sentido = DIS_Z;
		if (zA > 0) {
			for (obj = 1; obj < NUM_OBJETOS; obj++) {
				if (!(x[obj] - x[0] >= 8 || x[0] - x[obj] >= 8 || y[obj] - y[0] >= 8 || y[0] - y[obj] >= 8 || z[obj]<z[0] - 8 || z[obj]>z[0] - 8))
					choque = 1;
			}
			if (choque == 0) { zA--; objetos_zona[zona][0][4]--; }
		}
		else if (mapa[zona_z - 1][zona_x] != 0)
		{
			zona_z--;
			zona = mapa[zona_z][zona_x] - 1;
			objetos_zona[zona][0][2] = xA;
			objetos_zona[zona][0][3] = yA;
			objetos_zona[zona][0][4] = 72;
		}
	}
	if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE)) {
		if (yA < 32) {
			for (obj = 1; obj < NUM_OBJETOS; obj++) {
				if (!(x[obj] - x[0] >= 8 || x[0] - x[obj] >= 8 || y[obj] > y[0] + 8 || y[obj] < y[0] + 8 || z[obj] - z[0] >= 8 || z[0] - z[obj] >= 8))
					choque = 1;
			}
			if (choque == 0) { yA += 4; objetos_zona[zona][0][3] += 4; }
		}
	}

	choque = 0;
	if (!InputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE) && objetos_zona[zona][0][3] > 0 && y > 0) {
		for (obj = 1; obj < NUM_OBJETOS; obj++) {
			if (!(x[obj] - x[0] >= 8 || x[0] - x[obj] >= 8 || y[obj]<y[0] - 8 || y[obj]>y[0] - 8 || z[obj] - z[0] >= 8 || z[0] - z[obj] >= 8))
				choque = 1;
		}
		if (choque == 0) objetos_zona[zona][0][3]--;
	}
}

void Game::update() {
	zona = mapa[zona_z][zona_x] - 1;

	for (n = 0; n < NUM_OBJETOS; n++)		//get the distances for the current zone
	{
		x[n] = objetos_zona[zona][n][2];
		y[n] = objetos_zona[zona][n][3];
		z[n] = objetos_zona[zona][n][4];

		dist[n][0] = x[n] + y[n] + z[n];
		dist[n][1] = n;
	}

	for (n = NUM_OBJETOS - 1; n > 0; n--)	//sort objects
	{
		for (m = 0; m < n; m++)
		{
			if (dist[m][0] > dist[m + 1][0])
			{
				temp = dist[m][0]; dist[m][0] = dist[m + 1][0]; dist[m + 1][0] = temp;
				temp = dist[m][1]; dist[m][1] = dist[m + 1][1]; dist[m + 1][1] = temp;
			}
		}
	}
}

void Game::render() {
	// set to black // This function expects Red, Green, Blue and
	// Alpha as color values
	SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);
	// clear the window to black
	SDL_RenderClear(g_pRenderer);

	//draw walls and floor
	switch (mapa[zona_z][zona_x])
	{
	case 0:
		break;
	case 1:
		for (int bloq = 0; mapa_zona_1[bloq][0] != ULTIMO; bloq++) {
			SDL_Rect src, dest;
			src.x = mapa_zona_1[bloq][0];
			src.y = mapa_zona_1[bloq][1];
			dest.x = mapa_zona_1[bloq][2];
			dest.y = mapa_zona_1[bloq][3];
			src.w = dest.w = mapa_zona_1[bloq][4];
			src.h = dest.h = mapa_zona_1[bloq][5];
			SDL_SetTextureAlphaMod(AssetsManager::Instance()->getTexture("filma"), 255);
			SDL_RenderCopyEx(g_pRenderer, AssetsManager::Instance()->getTexture("filma"), &src, &dest, 0.0, 0, SDL_FLIP_NONE);
		}
		break;
	case 2:
		for (int bloq = 0; mapa_zona_2[bloq][0] != ULTIMO; bloq++) {
			SDL_Rect src, dest;
			src.x = mapa_zona_2[bloq][0];
			src.y = mapa_zona_2[bloq][1];
			dest.x = mapa_zona_2[bloq][2];
			dest.y = mapa_zona_2[bloq][3];
			src.w = dest.w = mapa_zona_2[bloq][4];
			src.h = dest.h = mapa_zona_2[bloq][5];
			SDL_SetTextureAlphaMod(AssetsManager::Instance()->getTexture("filma"), 255);
			SDL_RenderCopyEx(g_pRenderer, AssetsManager::Instance()->getTexture("filma"), &src, &dest, 0.0, 0, SDL_FLIP_NONE);
		}
		break;
	case 3:
		for (int bloq = 0; mapa_zona_3[bloq][0] != ULTIMO; bloq++) {
			SDL_Rect src, dest;
			src.x = mapa_zona_3[bloq][0];
			src.y = mapa_zona_3[bloq][1];
			dest.x = mapa_zona_3[bloq][2];
			dest.y = mapa_zona_3[bloq][3];
			src.w = dest.w = mapa_zona_3[bloq][4];
			src.h = dest.h = mapa_zona_3[bloq][5];
			SDL_SetTextureAlphaMod(AssetsManager::Instance()->getTexture("filma"), 255);
			SDL_RenderCopyEx(g_pRenderer, AssetsManager::Instance()->getTexture("filma"), &src, &dest, 0.0, 0, SDL_FLIP_NONE);
		}
		break;
	case 4:
		for (int bloq = 0; mapa_zona_4[bloq][0] != ULTIMO; bloq++) {
			SDL_Rect src, dest;
			src.x = mapa_zona_4[bloq][0];
			src.y = mapa_zona_4[bloq][1];
			dest.x = mapa_zona_4[bloq][2];
			dest.y = mapa_zona_4[bloq][3];
			src.w = dest.w = mapa_zona_4[bloq][4];
			src.h = dest.h = mapa_zona_4[bloq][5];
			SDL_SetTextureAlphaMod(AssetsManager::Instance()->getTexture("filma"), 255);
			SDL_RenderCopyEx(g_pRenderer, AssetsManager::Instance()->getTexture("filma"), &src, &dest, 0.0, 0, SDL_FLIP_NONE);
		}
		break;
	case 5:
		for (int bloq = 0; mapa_zona_5[bloq][0] != ULTIMO; bloq++) {
			SDL_Rect src, dest;
			src.x = mapa_zona_5[bloq][0];
			src.y = mapa_zona_5[bloq][1];
			dest.x = mapa_zona_5[bloq][2];
			dest.y = mapa_zona_5[bloq][3];
			src.w = dest.w = mapa_zona_5[bloq][4];
			src.h = dest.h = mapa_zona_5[bloq][5];
			SDL_SetTextureAlphaMod(AssetsManager::Instance()->getTexture("filma"), 255);
			SDL_RenderCopyEx(g_pRenderer, AssetsManager::Instance()->getTexture("filma"), &src, &dest, 0.0, 0, SDL_FLIP_NONE);
		}
		break;
	}

	//the variable zona is updated in the update function
	//draw objects
	int x, y, z;
	for (n = 0; n < NUM_OBJETOS; n++) {
		//if the objects is the ball.
		if (dist[n][1] == 0)
		{
			x = objetos_zona[zona][0][2];
			y = objetos_zona[zona][0][3];
			z = objetos_zona[zona][0][4];

			//change the sprite of the ball by direction
			if (sentido == AUM_X) {
				pos_objeto[0] = xyz_bola_b[0];
				pos_objeto[1] = xyz_bola_b[1];
			}
			if (sentido == AUM_Z) {
				pos_objeto[0] = xyz_bola_a[0];
				pos_objeto[1] = xyz_bola_a[1];
			}
			if (sentido == DIS_X) {
				pos_objeto[0] = xyz_bola_d[0];
				pos_objeto[1] = xyz_bola_d[1];
			}
			if (sentido == DIS_Z) {
				pos_objeto[0] = xyz_bola_c[0];
				pos_objeto[1] = xyz_bola_c[1];
			}
			pos_objeto[4] = objetos_zona[zona][0][5];
			pos_objeto[5] = objetos_zona[zona][0][6];
		}

		m = dist[n][1];

		//if the object is not the ball.
		if (m == 1 || m == 2 || m == 3 || m == 4 || m == 5)
		{
			x = objetos_zona[zona][m][2];
			y = objetos_zona[zona][m][3];
			z = objetos_zona[zona][m][4];

			pos_objeto[0] = objetos_zona[zona][m][0];
			pos_objeto[1] = objetos_zona[zona][m][1];
			pos_objeto[4] = objetos_zona[zona][m][5];
			pos_objeto[5] = objetos_zona[zona][m][6];
		}

		pos_objeto[2] = 144 + ((x - z) << 1); //calculate the screen x = xorigin + (x-z) * 2;
		pos_objeto[3] = 21 + (x + z) - (y << 1); //calculate the scren y = yorigin + (x+z) - y*2;

		//render the object
		SDL_Rect src, dest;
		src.x = pos_objeto[0];
		src.y = pos_objeto[1];
		dest.x = pos_objeto[2];
		dest.y = pos_objeto[3];
		src.w = dest.w = pos_objeto[4];
		src.h = dest.h = pos_objeto[5];
		SDL_SetTextureAlphaMod(AssetsManager::Instance()->getTexture("filma"), 255);
		SDL_RenderCopyEx(g_pRenderer, AssetsManager::Instance()->getTexture("filma"), &src, &dest, 0.0, 0, SDL_FLIP_NONE);

	}

	// show the window
	SDL_RenderPresent(g_pRenderer);
}

void Game::quit() {
	m_bRunning = false;
}

const int FPS = 60;
const int DELAY_TIME = 1000.0f / FPS;

int main(int argc, char* args[])
{
	srand(time(nullptr));

	Uint32 frameStart, frameTime;

	if (Game::Instance()->init("Filmation", 100, 100, 320, 200,
		false))
	{
		while (Game::Instance()->running()) {
			frameStart = SDL_GetTicks();

			Game::Instance()->handleEvents();
			Game::Instance()->update();
			Game::Instance()->render();

			frameTime = SDL_GetTicks() - frameStart;

			if (frameTime < DELAY_TIME)
			{
				SDL_Delay((int)(DELAY_TIME - frameTime));
			}
		}
	}
	else
	{
		std::cout << "init failure - " << SDL_GetError() << "\n";
		return -1;
	}

	std::cout << "closing...\n";

	// clean up SDL
	SDL_DestroyWindow(g_pWindow);
	SDL_DestroyRenderer(g_pRenderer);
	AssetsManager::Instance()->clearAllTextures();
	InputHandler::Instance()->clean();
	SDL_Quit();
	return 0;
}