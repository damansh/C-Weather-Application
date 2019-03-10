// Daman Sharma
#include <iostream>
#include <fstream>
#include <sstream>

#include "nlohmann/json.hpp"
#include "curl/curl.h"

using json = nlohmann::json;
using namespace std;

string data;

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up) {
    for (int c = 0; c<size*nmemb; c++) {
        data.push_back(buf[c]);
    }
    return size*nmemb;
}

int main() {
    string city, url;
    double temp1, temp2, ID;
    
    while(true) {
        // Prompts the user to enter a Canadian city
        cout << "Enter a Canadian City: " << endl << "> ";
        getline(cin, city);
        for(int i=0; i < city.size(); i++) {
            if(city[i] == ' ') city[i] = '+';
        }
        
        // Uses the name of the city along with my API key to retrieve the json
        // file of the weather
        url = "http://api.openweathermap.org/data/2.5/forecast?q=" + city + ",ca&APPID=bddb7eaa49eeefaf2afc9d4522d44d21";
        
        // CURL library is used to set up a connection with Openweathermap
        data.erase();
        CURL* curl;
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
        curl_easy_perform(curl);
        
        json error = json::parse(data);
        
        // Checks if there is any error
        if(error["cod"] == "404") {
            cout << "Invalid city. Enter another Canadian city." << endl;
            
            curl_easy_cleanup(curl);
            curl_global_cleanup();
        } else {
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            
            break;
        }
    }
    
    json j = json::parse(data);
    
    temp1 = stod(j["list"][0]["main"]["temp"].dump());
    temp2 = stod(j["list"][1]["main"]["temp"].dump());
    ID = stod(j["list"][1]["weather"][0]["id"].dump());
    
    // Outputting the difference in temperature.
    // Steady temperature occurs when there is a +/- 0.5K difference in temperature
    // between the first tempreature and the second one.
    if (temp2 - temp1 > 0.5) cout << "UP" << endl;
    else if(temp2 - temp1 < -0.5 ) cout << "DOWN" << endl;
    else cout << "SAME" << endl;
    
    // To output the correct message, utilized the ID and ID table on
    // OpenWeatherMap. (https://openweathermap.org/weather-conditions)
    if(ID >= 200 && ID <= 531) {
        cout << "Flashing red" << endl;
    } else if(ID >= 600 && ID <= 622) {
        cout << "Flashing white" << endl;
    } else if(ID >= 801 && ID <= 804) {
        cout << "Steady red" << endl;
    } else if(ID == 800) {
        cout << "Steady green" << endl;
    }
    
    return 0;
}
