#include <stdio.h>
#include <stdlib.h>

typedef struct _PidState
{
	float u_1;
	float y0_1;
	float y1_1;
}PidState;

typedef struct _PidData
{
	float C;
	float ALP0;
	float BEAT0;
	float BETA1;
}PidData;

typedef struct _Pid
{
	float Kp;
	float Kd;
	float Ki;
	float Tf;
	float Tc;
}Pid;


void init(PidData* pd, PidState* ps, const Pid *p)
{
	ps->u_1 = ps->y0_1 = ps->y1_1 = 0;	

	pd->C = (p->Kd + p->Kp*p->Tf)/p->Tf;
	pd->ALP0 = -(p->Tc/p->Tf - 1);
	pd->BEAT0 = -p->Kd*p->Tc/p->Tf/p->Tf;
	pd->BETA1 = p->Ki*p->Tf;
}


float filt(const PidData* pd, PidState* ps, float u)
{
	float yc, y0, y1;

	yc = u*pd->C;
	y0 = pd->ALP0*ps->y0_1 + pd->BEAT0*ps->u_1;
	y1 = ps->y1_1 + pd->BETA1*ps->u_1;

	ps->u_1 = u;
	ps->y0_1 = y0;
	ps->y1_1 = y1;

	return yc + y0 + y1;
}
	


int main() /*test*/
{
	unsigned i;
	float u[256];	/* test data */

	Pid p;
	PidData pd;
	PidState ps;

	p.Tc = 0.01;	/* sample time (1/Fc) */
	p.Tf = 0.025;	/* exponential time costant (small as possible) > 1/FNyquist */
	p.Kp = 10;		/* proportional */
	p.Kd = 2;		/* derivative */
	p.Ki = 2;		/* integral */


	for (i=0; i<256; i++)
	{
		u[i]=1;		/* heaviside input */
	}

	init(&pd, &ps, &p);	/* pid initialize */


	/* this test is on open loop, you have to close the loop in real applications */

	for (i=0; i<256; i++)
	{
		printf ("%f\n", filt(&pd, &ps, u[i]));
	}

	return EXIT_SUCCESS;
}

