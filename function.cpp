//function
#include "header.h"

string curlApiWeather(){
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();

    string readBuffer;

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openweathermap.org/data/2.5/weather?id=6426887&units=metric&appid=047b86ef0e0320013c7ee78d7628a834"); //recuperation de du json api.openweathermap.org
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if(curl){
        cout << "CURL est initialise" << endl;
        return readBuffer;

    }
    else{
        cout << "Erreur lors de l'initialisation de CURL" << endl;
    }
}

double temperature(string curl){
    Document d;

    d.Parse<0>(curl.c_str()).HasParseError();
    assert(d.HasMember("main"));
    assert(d["main"]["temp"].IsDouble());
    double temp = d["main"]["temp"].GetDouble();

    return temp;
}

void tempToSQL(tm *ltm){
    MYSQL mysql;    //fonction mysql
    mysql_init(&mysql); //initialisation MYSQL
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP,"option"); // Parametrage MYSQL

    ostringstream meteoValeur;

    double temp = temperature(curlApiWeather());

    if (mysql_real_connect(&mysql, "127.0.0.1", "meteo", "12345678", "meteo", 0, NULL, 0)) {
        meteoValeur << "INSERT INTO `valeurs_meteo` (`Date`, `Heure`, `degres`) VALUES ('"
                    << ltm->tm_year + 1900 << "-" << ltm->tm_mon + 1 << "-" << ltm->tm_mday << "', '"
                    << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "', " << temp << ");";
        mysql_query(&mysql, meteoValeur.str().c_str());
        mysql_close(&mysql);
        cout << "fonctionne" << endl;
    } else {
        cout << "erreur" << endl;
    }
}