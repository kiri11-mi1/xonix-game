#include <SFML/Graphics.hpp>
#include <ctime>
using namespace sf;

const int height = 25;//height of field
const int width = 40;//width of field

int grid[height][width] = { 0 };//field

int tileSize = 18;

struct Enemy {
	int x, y, dx, dy;
	Enemy() {
		x = y = 300;
		dx = 4 - rand() % 8;
		dy = 4 - rand() % 8;
	 }

	void move() {
		x += dx;
		if (grid[y / tileSize][x / tileSize] == 1) {
			dx = -dx;
			x += dx;
		}

		y += dy;
		if (grid[y / tileSize][x / tileSize] == 1) {
			dy = -dy;
			y += dy;
		}
	}
};

void drop(int y, int x) {
	
	if (grid[y][x] == 0)
		grid[y][x] = -1;

	if (grid[y + 1][x] == 0)
		drop(y + 1, x);

	if (grid[y - 1][x] == 0)
		drop(y - 1, x);

	if (grid[y][x + 1] == 0)
		drop(y, x + 1);

	if (grid[y][x - 1] == 0)
		drop(y, x - 1);

};

int main() {
	srand(time(NULL));

	RenderWindow  window(VideoMode(tileSize*width, tileSize*height), "Xonix");
	window.setFramerateLimit(60);//60 FPS

	Texture t_tile, t_gameOver, t_enemy, t_back, t_win;
	t_tile.loadFromFile("images/tiles.png");
	t_gameOver.loadFromFile("images/gameover.png");
	t_enemy.loadFromFile("images/enemy.png");
	t_back.loadFromFile("images/background.jpg");
	t_win.loadFromFile("images/win.png");


	Sprite tile(t_tile), gameOver(t_gameOver), enemy(t_enemy), back(t_back), wingame(t_win);
	gameOver.setPosition(100, 100);
	wingame.setPosition(100, 100);

	int enemyCount = 2;
	Enemy a[10];

	bool Game = true, Win = false;

	int x = 0, y = 0, dx = 0, dy = 0;
	float timer = 0, delay = 0.07;
	Clock clock;

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (i == 0 || j == 0 || i == height - 1 || j == width - 1)
				grid[i][j] = 1;


	while (window.isOpen()) {
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();

		}

		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			dx = -1;
			dy = 0;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			dx = 1;
			dy = 0;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			dx = 0;
			dy = 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			dx = 0;
			dy = -1;
		}

		if (!Game)
			continue;

		if (Win)
			continue;

		if (timer > delay) {
			x += dx;
			y += dy;
			
			if (x < 0)
				x = 0;

			if (x > width - 1)
				x = width - 1;

			if (y < 0)
				y = 0;

			if (y > height - 1)
				y = height - 1;

			if (grid[y][x] == 2)//Eating yourself
				Game = false;

			if (grid[y][x] == 0)//Tail
				grid[y][x] = 2;

			timer = 0;
		}


		for (int i = 0; i < enemyCount; i++)
			a[i].move();

		if (grid[y][x] == 1) {

			dx = dy = 0;

			for (int i = 0; i < enemyCount; i++)
				drop(a[i].y/tileSize, a[i].x/tileSize);


			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
					if (grid[i][j] == -1)
						grid[i][j] = 0;
					else
						grid[i][j] = 1;
		}

		for (int i = 0; i < enemyCount; i++)
			if (grid[a[i].y/tileSize][a[i].x/tileSize] == 2)
				Game = false;


		window.clear();
		window.draw(back);
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {

				if (grid[i][j] == 1)
					continue;

				if (grid[i][j] == 2)
					tile.setTextureRect(IntRect(54, 0, tileSize, tileSize));

				if (grid[i][j] == 0)
					tile.setTextureRect(IntRect(1 * tileSize, 0, tileSize, tileSize));

				tile.setPosition(j*tileSize, i*tileSize);
				window.draw(tile);
			}

		tile.setTextureRect(IntRect(36, 0, tileSize, tileSize));
		tile.setPosition(x*tileSize, y*tileSize);
		window.draw(tile);

		for (int i = 0; i < enemyCount; i++) {
			enemy.setPosition(a[i].x, a[i].y);
			window.draw(enemy);
		}

		int  filled = 0;

		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				if (grid[i][j] == 1)
					filled++;

		if ((filled * 100) / (height * width) > 80)
			Win = true;

		if (Win)
			window.draw(wingame);

		if (!Game)
			window.draw(gameOver);

		window.display();

	}
	return 0;
}