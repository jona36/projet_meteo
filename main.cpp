#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <curl/curl.h>
#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/stringbuffer.h"
#include <mysql/mysql.h>
#include <ctime>
#include <unistd.h>

using namespace rapidjson;
using namespace std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

int main()
{
	while(1)
	{

		time_t actuel = time(0);
		tm *ltm = localtime(&actuel);

		if(ltm->tm_min==00)
		{
			MYSQL mysql;
			mysql_init(&mysql);
			mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP,"option");
			CURL *curl;
			CURLcode res;
			string readBuffer;
			std::ostringstream meteoValeur;
			curl = curl_easy_init();
			int T;

			if(curl) 
			{
				curl_easy_setopt(curl, CURLOPT_URL, "https://api.openweathermap.org/data/2.5/weather?id=6426887&units=metric&appid=047b86ef0e0320013c7ee78d7628a834");
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
				res = curl_easy_perform(curl);
				curl_easy_cleanup(curl);
			}
			else
			{
				cout << "Erreur lors de l'initialisation de CURL" << endl;
			}
			Document d;
			d.Parse<0>(readBuffer.c_str()).HasParseError();
			assert(d.HasMember("main"));
			assert(d["main"]["temp"].IsDouble());
			double temp = d["main"]["temp"].GetDouble();
			cout << "temperature = " << temp << " C" << endl;
			if(mysql_real_connect(&mysql, "127.0.0.1", "root", "PWD", "meteo", 0, NULL, 0))
			{
				meteoValeur << "INSERT INTO `valeurs_meteo` (`Date`, `Heure`, `degres`) VALUES ('"<< ltm->tm_year + 1900 << "-" << ltm->tm_mon + 1 << "-" << ltm->tm_mday << "', '" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "', " << temp << ");";
				mysql_query(&mysql,meteoValeur.str().c_str());
				mysql_close(&mysql);
				cout << "fonctionne" << endl;
			}
			else
			{
				cout << "erreur" << endl;
			}
			sleep(65);
		}
	}
	return 0;
}
