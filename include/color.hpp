#pragma once

#include <string>

class Color
{
public:
	Color(const std::string &hexcode);

	Color(int red, int green, int blue, int alpha) : _red(red),
													 _green(green),
													 _blue(blue),
													 _alpha(alpha) {}

	const int getRed() { return _red; }
	const int getGreen() { return _green; }
	const int getBlue() { return _blue; }
	const int getAlpha() { return _alpha; }

	static Color trasparent() { return Color(255, 255, 255, 0); }
	static Color white() { return Color(255, 255, 255, 255); }
	static Color black() { return Color(0, 0, 0, 255); }
	static Color red() { return Color(255, 0, 0, 255); }
	static Color green() { return Color(0, 255, 0, 255); }
	static Color blue() { return Color(0, 0, 255, 255); }
	static Color grey() { return Color(127, 127, 127, 255); }
	static Color darkGray() { return Color(70, 70, 70, 255); }
	static Color lightGray() { return Color(200, 200, 200, 255); }

private:
	int _red;
	int _green;
	int _blue;
	int _alpha;
};