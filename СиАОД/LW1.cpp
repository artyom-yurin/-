#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>

// This program takes max jump height from input and prints
// jump height for every time point with step 0.1 seconds.
// Program should print all time points when height is min and max.

void PrintResult(float g, float maxTimeLift, float currentTime)
{
	float initialVelocity = g * maxTimeLift;
	double currentHeight = initialVelocity * currentTime - 0.5 * g * currentTime * currentTime;
	printf("Current Time=%f, Current Height=%f\n", currentTime, currentHeight);
}
safe
int SafeScanf()
{
	int bufer = 0;
	if (0 == scanf("%d", &bufer))
	{
		printf("\n" "expected floating-point number" "\n");
		exit(1);
	}
	return bufer;
}

int main(int, char *[])
{
	const float g = 9.8f;
	printf("Max Height: ");
	int maxHeight = SafeScanf();
	float maxTimeLift = sqrt(maxHeight * 2 / g);
	printf("Max Time Lift=%f\n", maxTimeLift);
	bool doesMaxHeight = false;
	float currentTime = 0;
	while (currentTime < maxTimeLift * 2)
	{
		if (currentTime > maxTimeLift && !doesMaxHeight)
		{
			doesMaxHeight = true;
			PrintResult(g, maxTimeLift, maxTimeLift);
		}
		PrintResult(g, maxTimeLift, currentTime);
		currentTime += 0.1f;
	}

	PrintResult(g, maxTimeLift, maxTimeLift * 2);

	return 0;
}