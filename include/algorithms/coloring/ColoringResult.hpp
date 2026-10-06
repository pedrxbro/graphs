#pragma once 

#include <vector>

struct ColoringResult
{
	std::vector<int> colors;
	int colorCount = 0;
	bool success = false;
};