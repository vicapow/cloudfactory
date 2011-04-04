#include "Time_Counter.h"
#include <sys/time.h>
#include <stdlib.h>
#include <iostream>

/*------------------------------------------------------------------------------------*/
static void pcp_gettime(double *t)
{
    struct timeval tp;
    if (gettimeofday(&tp,(struct timezone *) NULL) == 0)
        *t = (double) tp.tv_sec + (double) tp.tv_usec / (double) 1E6;
    else *t=0;
}


/*------------------------------------------------------------------------------------*/
Time_Counter::Time_Counter()
{
	t0=0.0;
	t1=0.0;
	total = 0.0;
	actif = false;
}

/*------------------------------------------------------------------------------------*/
void Time_Counter::reset()
{
	t0=0.0;
	t1=0.0;
	total = 0.0;
	actif = false;
}

/*------------------------------------------------------------------------------------*/
void Time_Counter::start()
{
#ifdef DEBUG
	assert( ! actif);
	actif = true;
#endif
	pcp_gettime(&t0);
}

/*------------------------------------------------------------------------------------*/
void Time_Counter::restart()
{
	reset();
	start();
}

/*------------------------------------------------------------------------------------*/
void Time_Counter::stop()
{
#ifdef DEBUG
	assert( actif);
	actif = false;
#endif
	pcp_gettime(&t1);
	total += ( t1 - t0);
}

/*------------------------------------------------------------------------------------*/
double Time_Counter::getTime()
{
	return total;
}

/*------------------------------------------------------------------------------------*/
void Time_Counter::print()
{
	std::cout << "			---> temps : " << getTime() << "s" << std::endl;
}



