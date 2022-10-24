#include <raylib.h>
#include <cmath>
#include <iostream>
#include <limits>
#include "util/Types.hpp"

constexpr u16 screen_width  = 800;
constexpr u16 screen_height = 600;

Vector2 vertices1[] = {
	{350, 200},
	{450, 200},
	{450, 300},
	{350, 300},
	{350, 200},
};
Vector2 origin1 = {400, 250};

Vector2 vertices2[] = {
	{500, 250},
	{600, 250},
	{600, 350},
	{500, 350},
	{500, 250},
};
Vector2 origin2 = {550, 300};
bool colided = false;

void scale(Vector2& vec, f32 value)
{
	vec.x += value;
	vec.y += value;
}

void rotate_by_origin(Vector2& vec, const Vector2& origin, f32 degree)
{
	f32 radians = degree * PI / 180.0;
	f32 rsin = sinf(radians);
	f32 rcos = cosf(radians);
	f32 x = vec.x;
	f32 y = vec.y;

	vec.x = rcos * (x - origin.x) - rsin * (y - origin.y) + origin.x;
	vec.y = rsin * (x - origin.x) + rcos * (y - origin.y) + origin.y;
}

f32 dot(const Vector2& a, const Vector2& b)
{
	return (a.x * b.x) + (a.y * b.y);
}

void project_vert(const Vector2 a[], const Vector2& edge, f32& min, f32& max)
{
	min = std::numeric_limits<f32>::max();
	max = std::numeric_limits<f32>::min();

	for (u8 i = 0; i < 5; i++)
	{
		f32 proj = dot(a[i], edge);
		if (proj < min) { min = proj;}
		if (proj > max) { max = proj;}
	}
}

bool sat_colision(const Vector2 a[], const Vector2 b[])
{
	for (u8 i = 0; i < 5; i++)
	{
		Vector2 va = a[i];
		Vector2 vb = a[(i + 1) % 5];

		Vector2 edge =  {vb.x - va.x, vb.y - va.y};
		Vector2 normal = {edge.y, -edge.x};

		f32 min_a, min_b, max_a, max_b;
		project_vert(a, normal, min_a, max_a);
		project_vert(b, normal, min_b, max_b);

		if (min_a >= max_b || min_b >= max_a)
		{
			return false;
		}
	}
	for (u8 i = 0; i < 5; i++)
	{
		Vector2 va = b[i];
		Vector2 vb = b[(i + 1) % 5];

		Vector2 edge =  {vb.x - va.x, vb.y - va.y};
		Vector2 normal = {edge.y, -edge.x};

		f32 min_a, min_b, max_a, max_b;
		project_vert(a, normal, min_a, max_a);
		project_vert(b, normal, min_b, max_b);

		if (min_a >= max_b || min_b >= max_a)
		{
			return false;
		}
	}
	return true;
}

void Update()
{
	for (u8 i = 0; i < 5; i++)
	{
		rotate_by_origin(vertices1[i], origin1, 1.0);
		rotate_by_origin(vertices2[i], origin2, 1.0);
	}
	colided = sat_colision(vertices1, vertices2);
}

void Draw()
{
	BeginDrawing();
	ClearBackground(Color{32,32,32});

	// grid
	for (u8 i = 0; i < 16; i++)
	{
		DrawLine(i * 50, 0, i * 50, screen_height, Color{52,52,52,200});
		DrawLine(0, i * 50, screen_width, i * 50, Color{52,52,52,200});
	}
	// mouse pos
	DrawText(TextFormat("Mouse x: %i\nMouse y: %i", GetMouseX(), GetMouseY()), 10, 3, 20, LIME);
	
	if (colided) {
		DrawLineStrip(vertices1, 5, RED);
		DrawLineStrip(vertices2, 5, RED);

	} else {
		DrawLineStrip(vertices1, 5, BLUE);
		DrawLineStrip(vertices2, 5, ORANGE);
	}
	EndDrawing();
}

int main()
{
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(screen_width, screen_height, "Sat-Colision");
	SetTargetFPS(60);

	scale(origin2, -25);
	for (auto& vec : vertices2) {
		scale(vec, -25);
	}

	while (!WindowShouldClose())
	{
		Update();
		Draw();
	}
	CloseWindow();
}
