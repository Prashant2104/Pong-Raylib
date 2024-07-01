#include "iostream"
#include "raylib.h"

using namespace std;

int playerScore = 0;
int cpuScore = 0;

class Ball {
public:
	float x, y;
	int speedX, speedY;
	int radius;

	void Draw() {
		DrawCircle(x, y, radius, WHITE);
	}
	void Update() {
		x += speedX;
		y += speedY;

		if (y + radius >= GetScreenHeight() || y - radius <= 0) {
			speedY *= -1;
		}
		if (x + radius >= GetScreenWidth()) {
			cpuScore++;
			Reset();
		}
		if (x - radius <= 0) {
			playerScore++;
			Reset();
		}
	}
	void Reset() {
		x = GetScreenWidth() / 2;
		y = GetScreenHeight() / 2;

		int speedChoice[] = { -1, 1 };
		speedX *= speedChoice[GetRandomValue(0, 1)];
		speedY *= speedChoice[GetRandomValue(0, 1)];
	}
};

class Paddle {
public:
	float x, y;
	float width, heigth;
	int speed;

	void Draw() {
		DrawRectangle(x, y, width, heigth, WHITE);
	}
	void Update() {
		if (IsKeyDown(KEY_UP)) {
			y -= speed;
		}
		if (IsKeyDown(KEY_DOWN)) {
			y += speed;
		}
		EdgeDetection();
	}
protected:
	void EdgeDetection() {
		if (y <= 0) {
			y = 0;
		}
		if (y + heigth >= GetScreenHeight()) {
			y = GetScreenHeight() - heigth;
		}
	}
};

class CpuPaddle :public Paddle {
public:
	void Update(int ballY) {
		if (y + heigth / 2 > ballY) {
			y -= speed;
		}
		if (y - heigth / 2 < ballY) {
			y += speed;
		}
		EdgeDetection();
	}
};


Ball ball;
Paddle player;
CpuPaddle cpu;

int main()
{
	const int screen_width = 1280;
	const int screen_height = 720;

	ball.radius = 20;
	ball.x = screen_width / 2;
	ball.y = screen_height / 2;
	ball.speedX = 3;
	ball.speedY = 3;

	player.width = 25;
	player.heigth = 120;
	player.x = screen_width - player.width - 10;
	player.y = screen_height / 2 - player.heigth / 2;
	player.speed = 3;

	cpu.width = 25;
	cpu.heigth = 120;
	cpu.x = 10;
	cpu.y = screen_height / 2 - cpu.heigth / 2;
	cpu.speed = 3;

	cout << "Starting Pong!" << endl;

	InitWindow(screen_width, screen_height, "Pong!");
	SetTargetFPS(144);

	while (!WindowShouldClose())
	{
		BeginDrawing();

		//Update
		ball.Update();
		player.Update();
		cpu.Update(ball.y);

		//Collision Detection
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ player.x, player.y, player.width, player.heigth })) {
			ball.speedX *= -1;
		}
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ cpu.x, cpu.y, cpu.width, cpu.heigth })) {
			ball.speedX *= -1;
		}

		//Draw
		ClearBackground(BLACK);
		DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
		ball.Draw();
		player.Draw();
		cpu.Draw();
		DrawText(TextFormat("%i", cpuScore), screen_width / 4 - 20, 20, 80, WHITE);
		DrawText(TextFormat("%i", playerScore), 3 * screen_width / 4 - 20, 20, 80, WHITE);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}