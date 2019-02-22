//
// Created by devle on 21/02/2019.
//

#ifndef MAIN_CPP_HEADER_H
#define MAIN_CPP_HEADER_H

#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <curl/curl.h>
#include <mysql/mysql.h>
#include <ctime>
#include <unistd.h>

#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/stringbuffer.h"

using namespace rapidjson;
using namespace std;

void testFonction();
string curlApiWeather();
double temperature(string curl);
void tempToSQL();


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

#endif //MAIN_CPP_HEADER_H
