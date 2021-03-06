#include "stdafx.h"

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
	if (scanf("%d", &buffer) == 0)
	{
		printf("\n" "expected floating-point number" "\n");
		std::exit(1);
	}
	return buffer;
}

float CalculateLiftTimeMax(const int maxHeight)
{
	if (maxHeight < 0)
	{
		printf("\n" "Height can't be negative" "\n");
		std::exit(1);
	}
	return sqrt(2 * maxHeight / G);
}

void PrintAllResults(const float maxTimeLift)
{
	bool reachedMaxHeight = false;
	float currentTime = 0;
	while (currentTime < 2 * maxTimeLift)
	{
		if (currentTime > maxTimeLift && !reachedMaxHeight)
		{
			reachedMaxHeight = true;
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
	const float maxTimeLift = CalculateLiftTimeMax(maxHeight);
	printf("Max Time Lift=%f\n", maxTimeLift);
	PrintAllResults(maxTimeLift);

	return 0;
}