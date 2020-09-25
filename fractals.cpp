#include <iostream>
#include <vector>
#include "TinyPNG/png.h"
#include <cmath>
template<class T>
class Vector2
{
public:

	Vector2<T>(T x, T y)
	{
		X = x;
		Y = y;
	}

	T X;
	T Y;
};

template<class T>
class Vector3
{
public:

	Vector3<T>(T x, T y, T z)
	{
		X = x;
		Y = y;
		Z = z;
	}

	T X;
	T Y;
	T Z;
};

std::vector<Vector2<int>> Laberinto(int maxy, int maxx) {
	std::vector<Vector2<int>> pos;
	Vector2<int> actualPos = Vector2<int>(0, 0);
	pos.push_back(actualPos);
	for (int i = 1; i < maxx-1; i++) {
		for (int j = 1; j < maxy-1; j++) {
			int randomBit = rand() % 2;
			if (randomBit == 0 && actualPos.X<maxx) {
				actualPos.X++;
				pos.push_back(actualPos);
				std::cout << actualPos.X << endl;
			}
			else {
				if (actualPos.Y < maxy) {
					actualPos.Y++;
					pos.push_back(actualPos);
					std::cout << actualPos.Y << endl;
				}
			}

			
		}
	}
	Vector2<int> finalPos = Vector2<int>(maxx - 1, maxy - 1);
	pos.push_back(finalPos);
	return pos;
}

// Function to draw mandelbrot set 
std::vector<Vector3<int>> fractal(float left, float top, float xside, float yside, int maxX, int maxY, int maxCount)
{
	std::vector<Vector3<int>> pos;
	float xscale, yscale, zx, zy, cx, tempx, cy;
	int x, y, i, j;
	int count;


	// setting up the xscale and yscale 
	xscale = xside / maxX;
	yscale = yside / maxY;

	// Each point represents a Complex number (x + yi). 
	for (y = 1; y <= maxX - 1; y++)
	{
		for (x = 1; x <= maxY - 1; x++)
		{
			// c_real 
			cx = x * xscale + left;

			// c_imaginary 
			cy = y * yscale + top;

			// z_real 
			zx = 0;

			// z_imaginary 
			zy = 0;
			count = 0;

			// Calculate whether c(c_real + c_imaginary) belongs 
			// to the Mandelbrot set or not and draw a pixel 
			// at coordinates (x, y)
			while ((zx * zx + zy * zy < 4) && (count < maxCount))
			{
				// Calculate Mandelbrot function 
				// z = z*z + c where z is a complex number 

				// tempx = z_real*_real - z_imaginary*z_imaginary + c_real 
				tempx = zx * zx - zy * zy + cx;

				// 2*z_real*z_imaginary + c_imaginary 
				zy = 2 * zx * zy + cy;

				// Updating z_real = tempx 
				zx = tempx;

				// Increment count 
				count = count + 1;
			}

			// To display the created fractal 
			pos.push_back(Vector3<int>(x, y, count));
		}
	}
	return pos;
}
class Pixel
{
private:
	uint8_t _red, _green, _blue, _alpha;
public:
	Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
	{
		_red = red;
		_green = green;
		_blue = blue;
		_alpha = alpha;
	}

	Pixel(uint8_t red, uint8_t green, uint8_t blue) : Pixel(red, green, blue, 255) {}

	void ApplyToArray(uint8_t* buffer, int pos)
	{
		auto p = buffer + pos;
		*p = _red;
		p++;
		*p = _green;
		p++;
		*p = _blue;
		p++;
		*p = _alpha;
	}

	int static GetOffset(int x, int xPerRow, int y)
	{
		return x * 4 + y * xPerRow * 4;
	}

	void ApplyToPng(tinypng::PNG& png, unsigned x, unsigned y)
	{
		if (x < png.getWidth() && y < png.getHeight())
		{
			auto&& buffer = png.buffer();
			auto offset = GetOffset(x, png.getWidth(), y);
			ApplyToArray(buffer, offset);
		}
	}
};


std::vector<Vector2<int>> Line(float x1, float y1, float x2, float y2)
{
	std::vector<Vector2<int>> pos;

	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if (steep)
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	const float dx = x2 - x1;
	const float dy = fabs(y2 - y1);

	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;

	const int maxX = (int)x2;

	for (int x = (int)x1; x < maxX; x++)
	{
		if (steep)
		{
			pos.push_back(Vector2<int>(y, x));

		}
		else
		{
			pos.push_back(Vector2<int>(x, y));
		}

		error -= dy;
		if (error < 0)
		{
			y += ystep;
			error += dx;
		}
	}
	return pos;
}


void DrawThickLine(int width, Vector2<int> &end, tinypng::PNG &png);

int main()
{
	srand(time(NULL));
	tinypng::PNG png(500, 500);
	auto runs = 0;
	Pixel w(255, 255, 255);
	Pixel g(0, 255, 100);
	int l = 0; 

	srand(time(NULL));
	for (int i = 25; i < 30; i++) {
		auto ran1 = -(rand() % 200) * 0.01;
		auto ran2 = -(rand() % 200) * 0.01;
		auto ran3 = (rand() % 200) * 0.01 + 0.5;
		auto ran4 = (rand() % 200) * 0.01 + 0.5;
		int a = rand() % 100;
		int b = rand() % 100;
		int c = rand() % 100;
		auto pixels = fractal(-0.2,ran2, ran3, ran4, png.getHeight(), png.getWidth(), 200);
		

		for (auto p : pixels)
		{
			Pixel d(a * p.Z, b * p.Z, c * p.Z);
			d.ApplyToPng(png, p.X, p.Y);
		}

		std::cout << runs << std::endl;
		std::string name;
		name.append("test");
		name.append(std::to_string(i));
		name.append(".png");
		std::cout << name << std::endl;
		png.writeToFile(name);
	}
	std::cin.ignore();
	return 0;
}

void DrawThickLine(int width, Vector2<int> &end, tinypng::PNG &png)
{
	auto lineTop = Line(width / 2, 0, end.X + width / 2, end.Y);
	auto lineB = Line(-width / 2, 0, end.X - width / 2, end.Y);
	Pixel p(0, 153, 204);
	for (int i = 0; i < lineTop.size(); i++)
	{
		auto&& b = lineB[i];
		auto&& t = lineTop[i];
		if (b.Y == t.Y)
		{
			for (int i = b.X; i < t.X; i++)
			{
				p.ApplyToPng(png, i, b.Y);
			}
		}
		else
		{
			for (int i = b.Y; i < t.Y; i++)
			{
				p.ApplyToPng(png, b.X, i);
			}
		}
	}
}