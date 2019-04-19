#include <iostream>
#include <cstdint>
#include <cmath>

#define N_BIT_FRACT 16
#define EXP_SCALE 32768 /* 2^(N_BIT_FRACT-1) */

typedef struct _PID
{
	int32_t C[6];
	int32_t Ulim[2];
	int32_t X[2];
	int32_t wo;
}PID;

typedef struct _InitParameters
{
	float T;
	float Kp;
	float Ti;
	float Td;
	float Tn;
	float Tw;
	float Ulim[2];
	bool realDerivative;
}InitParameters;

void pidInit(PID* pid, const InitParameters* p);

int32_t pidStep(PID* pid, void(*finput)(int32_t U[3], int32_t wo, void* arg), void* arg);

int value2int(int32_t x)
{
	return (x + EXP_SCALE) >> N_BIT_FRACT;
}

float value2float(int32_t x)
{
	return (float)x / (1 << N_BIT_FRACT);
}

int32_t float2value(float x)
{
	return (int32_t)roundf(x * (1 << N_BIT_FRACT));
}

int32_t valuemul(int32_t x, int32_t y)
{
	return ((int64_t)x * (int64_t)y + EXP_SCALE) >> N_BIT_FRACT;
}