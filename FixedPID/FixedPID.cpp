#include <cstdio>

#include "FixedPID.h"


typedef struct _args
{
	int32_t ref;
	int32_t Yold;
	int32_t Py;
	float Px;
}args_s;

float testProcess(float *X, float Y)
{
	float Ty = 0.01f;
	float T = 1e-5f;
	*X = expf(-T / Ty) * *X - Ty * (expf(-T / Ty) - 1) * Y;
	return 10 / Ty * *X;
}

void loopInput(int32_t U[3], int32_t wo, void* arg)
{
	args_s *x = (args_s*)arg;
	int32_t Ref = x->ref;
	float Px = x->Px;
	int32_t Yold = x->Yold;

	int32_t Py = float2value(testProcess(&Px, (value2float(Yold))));
	x->Px = Px;
	x->Py = Py;

	U[0] = Ref - Py;
	U[1] = wo;
	U[2] = Py;
}

int main()
{
	PID mypid;
	InitParameters mypars;
	mypars.T = 1e-5f;
	mypars.Kp = 1;
	mypars.Ti = 0.1f;
	mypars.Td = 0.1f;
	mypars.Tw = mypars.Ti / 5;
	mypars.Tn = 1e-3f;
	mypars.Ulim[0] = -1;
	mypars.Ulim[1] = 1;
	mypars.realDerivative = false;
	
	pidInit(&mypid, &mypars);

	float Ref = 0.5;
 
	args_s arg = { float2value(Ref), 0, 0, 0.0f };
	int32_t u;
		
	for (int32_t i = 0; i < 200000; i++)
	{
		u = pidStep(&mypid, loopInput, &arg);
		arg.Yold = u;
		
	}
	std::cout << value2float(arg.Py) << std::endl;
	return EXIT_SUCCESS;
}

void pidInit(PID* pid, const InitParameters* p)
{
	float Ltn = expf(-p->T / p->Tn);
	float Kn = -p->Kp / p->Tn;

	pid->C[0] = float2value(Ltn);
	pid->C[1] = float2value(p->Kp*p->T/p->Ti);
	pid->C[2] = float2value(p->T/p->Tw);
	pid->C[4] = float2value(p->Kp);

	if (p->realDerivative)
	{
		pid->C[3] = 0;
		pid->C[5] = float2value(p->Td);
	}
	else
	{
		pid->C[3] = float2value(Kn*p->Td*(Ltn - 1));
		pid->C[5] = float2value(Kn*p->Td);
	}

	pid->Ulim[0] = float2value(p->Ulim[0]);
	pid->Ulim[1] = float2value(p->Ulim[1]);

	pid->X[0] = pid->X[1] = 0;

	pid->wo = 0;
}

int32_t pidStep(PID* pid, void (*finput)(int32_t U[3], int32_t wo ,void* arg), void* arg)
{
	int32_t U[3];
	(*finput)(U, pid->wo, arg);

	pid->X[0] += valuemul(U[1], pid->C[2]) + valuemul(U[0], pid->C[1]);
	pid->X[1] = valuemul(U[2], pid->C[3]) + valuemul(pid->X[1], pid->C[0]);

	int32_t Y0 = pid->X[0] + pid->X[1] + valuemul(U[2], pid->C[5]) + valuemul(U[0], pid->C[4]);
	
	int32_t Y;
	if (Y0 < pid->Ulim[0])
		Y = pid->Ulim[0];
	else if (Y0 > pid->Ulim[1])
		Y = pid->Ulim[1];
	else
		Y = Y0;

	pid->wo = Y - Y0;

	return Y;
}

