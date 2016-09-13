#include "stdafx.h"

const float G = 9.8f;
const float STEP = 0.1f;

void PrintCurrentResult(const float g, const float maxTimeLift, const float currentTime)
{
	const float initialVelocity = g * maxTimeLift;
	const float currentHeight = initialVelocity * currentTime - g * currentTime * currentTime / 2;
	printf("Current Time=%f, Current Height=%f\n", currentTime, currentHeight);
}

int s_scanf()
{
	int buffer = 0;
	if (scanf("%d", &buffer) == 0)
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
	const int maxHeight = s_scanf();
	const float maxTimeLift = CalculateMaxTimeLift(maxHeight);
	printf("Max Time Lift=%f\n", maxTimeLift);
	PrintAllResults(maxTimeLift);

	return 0;
}