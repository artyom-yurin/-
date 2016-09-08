#include "stdafx.h"

// This program takes max jump height from input and prints
// jump height for every time point with step 0.1 seconds.
// Program should print all time points when height is min and max.

const float G = 9.8f;
const float STEP = 0.1f;

void PrintCurrentResult(const float g, const float maxTimeLift, const float currentTime)
{
	const float initialVelocity = g * maxTimeLift;
	const float currentHeight = initialVelocity * currentTime - g * currentTime * currentTime / 2;
	printf("Current Time=%f, Current Height=%f\n", currentTime, currentHeight);
}

int SafeScanf()
{
	int buffer = 0;
	if (0 == scanf("%d", &buffer))
	{
		printf("\n" "expected floating-point number" "\n");
		exit(1);
	}
	return buffer;
}

float CalculateMaxTimeLift(const int maxHeight)
{
	if (maxHeight < 0)
	{
		printf("\n" "Height can't be negative" "\n");
		exit(1);
	}
	return sqrt(2 * maxHeight / G);
}

void PrintAllResults(const float maxTimeLift)
{
	bool doesMaxHeight = false;
	float currentTime = 0;
	while (currentTime < 2 * maxTimeLift)
	{
		if (currentTime > maxTimeLift && !doesMaxHeight)
		{
			doesMaxHeight = true;
			PrintCurrentResult(G, maxTimeLift, maxTimeLift);
		}
		PrintCurrentResult(G, maxTimeLift, currentTime);
		currentTime += STEP;
	}

	PrintCurrentResult(G, maxTimeLift, 2 * maxTimeLift);
}

int main(int, char *[])
{
	printf("Max Height: ");
	const int maxHeight = SafeScanf();
	const float maxTimeLift = CalculateMaxTimeLift(maxHeight);
	printf("Max Time Lift=%f\n", maxTimeLift);
	PrintAllResults(maxTimeLift);

	return 0;
}