#ifndef TESTBRAIN_H_
#define TESTBRAIN_H_

#include "roboBrain.h"
#include "LogCSV.h"
#include "Simulator.h"

class testBrain: public roboBrain
{
	protected:
		Interface& hardInterface;
	public:
		testBrain(double h, double e, double n, Interface& Linterface, Interface& LhardInterface, LogCSV& LlogC);
		LogCSV& logC;
		void showVector() const;
		void guide();
		void control();
};

#endif
