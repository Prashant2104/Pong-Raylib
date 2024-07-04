#include "iostream"
#include "raylib.h"

using namespace std;

int playerScore = 0;
int cpuScore = 0;

Color Green = GetColor(0x2ca165ff);
Color DarkGreen = GetColor(0x1e6d45ff);
Color LightGreen = GetColor(0x3ddf8dff);
Color Yellow = GetColor(0xffe54dff);


class Ball {
public:
	float x, y;
	int speedX, speedY;
	int radius;

	void Draw() {
		DrawCircle(x, y, radius, Yellow);
	}
	void Update() {
		x += speedX * GetFrameTime();
		y += speedY * GetFrameTime();

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
		//DrawRectangle(x, y, width, heigth, RAYWHITE);
		DrawRectangleRounded(Rectangle{ x, y, width, heigth }, 0.85f, 4, RAYWHITE);
	}
	void Update() {
		if (IsKeyDown(KEY_UP)) {
			y -= speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_DOWN)) {
			y += speed * GetFrameTime();
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
			y -= speed * GetFrameTime();
		}
		if (y - heigth / 2 < ballY) {
			y += speed * GetFrameTime();
		}
		EdgeDetection();
	}
};

class Sounds {
public:
	Sound paddleHit;
	Sound playerScore;
	Sound cpuScore;

	void Setup() {
		paddleHit = LoadSound("game/Sounds/Paddle_Hit.mp3");
		paddleHit.stream.buffer;

		playerScore = LoadSound("game/Sounds/Player_Score.mp3");
		playerScore.stream.buffer;

		cpuScore = LoadSound("game/Sounds/CPU_Score.mp3");
		cpuScore.stream.buffer;
	}
	void PaddleHit() {
		PlaySound(paddleHit);
	}
	void PlayerScore() {
		PlaySound(playerScore);
	}
	void CPUScore() {
		PlaySound(cpuScore);
	}
};

int main()
{
	Ball ball;
	Paddle player;
	CpuPaddle cpu;
	Sounds sfx;

	const int screen_width = 1300;
	const int screen_height = 700;

	ball.radius = 20;
	ball.x = screen_width / 2;
	ball.y = screen_height / 2;
	ball.speedX = 500;
	ball.speedY = 500;

	player.width = cpu.width = 25;
	player.heigth = cpu.heigth = 125;
	player.speed = cpu.speed = 450;

	player.x = screen_width - player.width - 10;
	player.y = screen_height / 2 - player.heigth / 2;

	cpu.x = 10;
	cpu.y = screen_height / 2 - cpu.heigth / 2;

	cout << "Starting Pong!" << endl;

	InitWindow(screen_width, screen_height, "Pong!");
	InitAudioDevice();

	//Sounds
	sfx.Setup();

	SetTargetFPS(120);

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
			sfx.PaddleHit();
		}
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ cpu.x, cpu.y, cpu.width, cpu.heigth })) {
			ball.speedX *= -1;			
			sfx.PaddleHit();
		}

		//Drawing static stuff
		ClearBackground(DarkGreen);
		DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
		DrawCircle(screen_width / 2, screen_height / 2, screen_height / 4, LightGreen);
		DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);

		//Drawing moveables
		ball.Draw();
		player.Draw();
		cpu.Draw();

		//TEXT
		DrawText(TextFormat("CPU"), screen_width / 4 - 60, 20, 50, WHITE);
		DrawText(TextFormat("PLAYER"), 3 * screen_width / 4 - 100, 20, 50, WHITE);
		DrawText(TextFormat("%i", cpuScore), screen_width / 4 - 20, 75, 50, WHITE);
		DrawText(TextFormat("%i", playerScore), 3 * screen_width / 4 - 20, 75, 50, WHITE);

		EndDrawing();
	}

	CloseWindow();
	CloseAudioDevice();
	return 0;
}