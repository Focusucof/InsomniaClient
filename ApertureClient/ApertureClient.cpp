// ApertureClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "httplib.h"
#include "LocalRiotClient.h"

std::string clientVersion = "release-03.06-shipping-8-610061";

size_t writef(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

int main()
{
    /**********************************HEADER**********************************/
    std::cout 
        << "\x1b[95m ___                                 _       " << std::endl
        << "|_ _|_ __  ___  ___  _ __ ___  _ __ (_) __ _ " << std::endl
        << " | || '_ \\/ __|/ _ \\| '_ ` _ \\| '_ \\| |/ _` |" << std::endl
        << " | || | | \\__ \\ (_) | | | | | | | | | | (_| |" << std::endl
        << "|___|_| |_|___/\\___/|_| |_| |_|_| |_|_|\\__,_|" << std::endl;
    /**************************************************************************/

    httplib::Server app;

    app.Get("/party/v1/create", [](const httplib::Request&, httplib::Response& res) {

        LocalRiotClient* client = new LocalRiotClient();
        std::vector<std::string> auth = client->getCredentials();

        CURL* curl;
        struct curl_slist* headers = NULL;


        curl = curl_easy_init();
        std::string str = "https://glz-na-1.na.a.pvp.net/parties/v1/players/" + auth[2];
        const char* url = str.c_str();
        curl_easy_setopt(curl, CURLOPT_URL, url);

        str = "Authorization: Bearer " + auth[0];
        const char* accessToken = str.c_str();
        headers = curl_slist_append(headers, accessToken);

        str = "X-Riot-Entitlements-JWT: " + auth[1];
        const char* entitlement = str.c_str();
        headers = curl_slist_append(headers, entitlement);

        str = "X-Riot-ClientPlatform: ew0KCSJwbGF0Zm9ybVR5cGUiOiAiUEMiLA0KCSJwbGF0Zm9ybU9TIjogIldpbmRvd3MiLA0KCSJwbGF0Zm9ybU9TVmVyc2lvbiI6ICIxMC4wLjE5MDQyLjEuMjU2LjY0Yml0IiwNCgkicGxhdGZvcm1DaGlwc2V0IjogIlVua25vd24iDQp9";
        const char* clientPlatform = str.c_str();
        headers = curl_slist_append(headers, "X-Riot-ClientPlatform: ew0KCSJwbGF0Zm9ybVR5cGUiOiAiUEMiLA0KCSJwbGF0Zm9ybU9TIjogIldpbmRvd3MiLA0KCSJwbGF0Zm9ybU9TVmVyc2lvbiI6ICIxMC4wLjE5MDQyLjEuMjU2LjY0Yml0IiwNCgkicGxhdGZvcm1DaGlwc2V0IjogIlVua25vd24iDQp9");

        str = "X-Riot-ClientVersion: " + clientVersion;
        const char* version = str.c_str();
        headers = curl_slist_append(headers, version);

        std::string response;

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writef);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        curl_easy_perform(curl);

        std::cout << response;

    });

    app.listen("127.0.0.1", 1337);

    //LocalRiotClient* client = new LocalRiotClient();

    //std::vector<std::string> auth = client->getCredentials();
    ////std::cout << auth[0];

    //delete client;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
