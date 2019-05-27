#include <string>
#include "../JuceLibraryCode/JuceHeader.h"
#include "NoiseAlgorithms.h"

// Need to impliment Mersenne twist: https://github.com/cslarsen/mersenne-twister
// and std::rand()

//==============================================================================

class AlgorithmManager
{
private:
	AudioIODeviceCallback* currentAlgorithm;

	// Declare Algorithms
	JUCERandom jUCERandom;
	RAND rand;
	LCG lcg;
	XOR32 xor32;
	MersenneTwister mt;
	STDMT19937 stdmt;

	int numAlgorithms = 6;

public:
	AlgorithmManager()
	{}

	AudioIODeviceCallback* getCurrentAlgorithm()
	{
		return currentAlgorithm;
	}

	AudioIODeviceCallback* setCurrentAlgorithm(int value)
	{
		switch (value)
		{
			case 0: { currentAlgorithm = &jUCERandom; break; }
			case 1: { currentAlgorithm = &rand; break; }
			case 2: { currentAlgorithm = &lcg; break; }
			case 3: { currentAlgorithm = &xor32; break; }
			case 4: { currentAlgorithm = &mt; break; }
			case 5: { currentAlgorithm = &stdmt; break; }
		}

		return currentAlgorithm;
	}

	int getNumAlgorithms() {return numAlgorithms;}
};

//==============================================================================

std::string algorithmName(int value)
{
	switch(value)
		{
			case 0 : return "JUCE builtin Random class";
			case 1 : return "std::rand()";
			case 2 : return "LCG";
			case 3 : return "XOR32";
			case 4 : return "Mersenne Twister";
			case 5 : return "STD Mersenne Twister Implementaton";
		}

	return "error";
}

int main ()
{
	AudioDeviceManager dm;
	dm.initialiseWithDefaultDevices(0, 2);

	AlgorithmManager am;

	std::cout << "ArifD's Noise Testing Program" << std::endl;
	std::cout << "-----------------------------" << std::endl;
	std::cout << "repeatedly select the same algorithm to test its CPU ussage" << std::endl; 
	std::cout << "-----------------------------" << std::endl;
	std::cout << "Algorithms:" << std::endl;
	for (int i = 0; i < am.getNumAlgorithms(); i++)
	{
		std::cout << algorithmName(i) << std::endl;
	}
	std::cout << "-----------------------------" << std::endl;

	while (true) 
	{
		int input;
		std::cout << "Select algorithm (0-x): ";
		std::cin >> input;

		dm.removeAudioCallback(am.getCurrentAlgorithm());
		dm.addAudioCallback(am.setCurrentAlgorithm(input));

		std::cout << "Selected algorithm: ";

		std::cout << algorithmName(input) << std::endl;

		std::cout << "CPU Usage: " << dm.getCpuUsage() << std::endl;

	}


    return 0;
}
