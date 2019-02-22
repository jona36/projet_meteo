#include "header.h"
#include "function.cpp"

int main()
{
	string bar = "------------------------------";
	cout << "initilisation TEST app" << endl;

	string curl = curlApiWeather();
	double temp = temperature(curl);

	cout << "temperature = " << temp << " °C" << endl;
	cout << bar << endl;

	sleep(10);

	cout << "START APP" << endl << endl;

	//start app
	while(1) {
		time_t actuel = time(0);    //
		tm *ltm = localtime(&actuel);
		if (ltm->tm_min == 00) {

			tempToSQL(ltm);

			cout << "temperature = " << temp << " C" << endl << endl;
			cout << ltm->tm_mday << "/" << ltm->tm_mon + 1 << "/" << ltm->tm_year + 1900 << " "
				 << ltm->tm_hour << "h" << ltm->tm_min << ":" << ltm->tm_sec << ", " << temp << "°C" << endl << endl;
			cout << bar << endl;

			sleep(70);
		}
	}
	return 0;
}
