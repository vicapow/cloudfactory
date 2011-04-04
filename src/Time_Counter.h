#ifndef TIME_COUNTER_H
#define TIME_COUNTER_H

class Time_Counter
{
public:
	//! constructor
	Time_Counter();
	//! indique au compteur que le temps apres cette instruction doit etre ajoutee au total
	void start();
	//! inqique au compteur d'arreter d'ajouter du temps au total
	void stop();
	//! retourn le temps compté, en secondes
	double getTime();
	//! reset le compteur
	void reset();
	//! reset le compteur et fait un start
	void restart();
	//! display le temps ecoulé
	void print();
private:
	double t0,t1;
	double total;
	bool actif;


};

#endif
