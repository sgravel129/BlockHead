#pragma once

#include <string>
#include <vector>

class Util
{
public:
	/*
		Returns an integer int the interval [lower,upper]
		Throws exception if lower > upper
	 */
	static const int randInt(int lower, int upper);

	/*
		Returns true if any file was found in path passed
	 */
	static const bool fileExists(const std::string &path);

	/*
		Returns true if float a and b are less then 10^(-6)
		far apart from each other false otherwise

	 */
	static const bool fequals(float a, float b);

	/*
		returns the scaled value scaled by sprite scale
	 */
	static int scale(int a, float scale);
	// static const float applyScale(float a, float scale);

	/*
		Permutes a list randomly
	 */
	template <typename T>
	static void permute(std::vector<T> &vector);
};