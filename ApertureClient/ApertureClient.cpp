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

    LocalRiotClient* client = new LocalRiotClient();
    if(client->bIsGameOpen()) {
        std::cout << "\x1b[0\nmValorant is not currently running..." << std::endl;
        std::cout << "Press \x1b[96mENTER\x1b[0m to quit";
        system("pause >nul");
        delete client;
        return 0;
    }
    delete client;

    httplib::Server app;

    app.Get("/party/v1/create", [](const httplib::Request&, httplib::Response& res) {

        LocalRiotClient* client = new LocalRiotClient();
        std::vector<std::string> auth = client->getCredentials();

        std::string partyID;

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
        headers = curl_slist_append(headers, clientPlatform);

        str = "X-Riot-ClientVersion: " + clientVersion;
        const char* version = str.c_str();
        headers = curl_slist_append(headers, version);

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        std::string response;

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writef);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        curl_easy_perform(curl);

        nlohmann::json resp = nlohmann::json::parse(response);
        partyID = resp.value("CurrentPartyID", "");

        // make custom game post request
        str = "https://glz-na-1.na.a.pvp.net/parties/v1/parties/" + partyID + "/makecustomgame";
        const char* customgameURL = str.c_str();
        curl_easy_setopt(curl, CURLOPT_URL, customgameURL);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");

        curl_easy_perform(curl);

        // make party open
        str = "https://glz-na-1.na.a.pvp.net/parties/v1/parties/" + partyID + "/accessibility";
        const char* openpartyURL = str.c_str();
        curl_easy_setopt(curl, CURLOPT_URL, openpartyURL);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"Accessibility\": \"OPEN\"}");

        curl_easy_perform(curl);

        std::string returnHTML =
            std::string("<!DOCTYPE html>\r\n"
                        "<html>\r\n"
                        "<head>\r\n"
                        "<title>Insomnia</title>\r\n"
                        "<link rel=\"preconnect\" href=\"https:\/\/fonts.gstatic.com\">\r\n"
                        "<link href=\"https:\/\/fonts.googleapis.com/css2?family=Raleway&display=swap\" rel=\"stylesheet\">\r\n"
                        "<link rel=\"icon\" href=\"https:\/\/raw.githubusercontent.com/Focusucof/InsomniaClient/headless/resources/favicon.ico\">\r\n"
                        "</head>\r\n"
                        "<body>\r\n"
                        "<div class=\"main\">\r\n"
                        "<pre>\r\n"
                        " ___                                 _       \r\n"
                        "|_ _|_ __  ___  ___  _ __ ___  _ __ (_) __ _ \r\n"
                        " | || '_ \\/ __|/ _ \\| '_ ` _ \\| '_ \\| |/ _` |\r\n"
                        " | || | | \\__ \\ (_) | | | | | | | | | | (_| |\r\n"
                        "|___|_| |_|___/\\___/|_| |_| |_|_| |_|_|\\__,_|\r\n"
                        "</pre>\r\n"
                        "<p>Your party has been created</p>\r\n"
                        "<button class=\"hover\" onclick = \"copy()\">Copy</button>\r\n"
                        "<h3 id=\"partyID\">") + partyID + std::string("</h3>\r\n"
                        "</div>\r\n"

                        "<script>\r\n"
                        "function copy() {\r\n"
                        "if(window.getSelection) {\r\n"
                        "if(window.getSelection().empty) { // Chrome\r\n"
                        "window.getSelection().empty();\r\n"
                        "}\r\n"
                        "else if(window.getSelection().removeAllRanges) { // Firefox\r\n"
                        "window.getSelection().removeAllRanges();\r\n"
                        "}\r\n"
                        "}\r\n"
                        "else if(document.selection) { // IE?\r\n"
                        "document.selection.empty();\r\n"
                        "}\r\n"

                        "if(document.selection) {\r\n"
                        "var range = document.body.createTextRange();\r\n"
                        "range.moveToElementText(document.getElementById(\"partyID\"));\r\n"
                        "range.select().createTextRange();\r\n"
                        "document.execCommand(\"copy\");\r\n"
                        "} else if(window.getSelection) {\r\n"
                        "var range = document.createRange();\r\n"
                        "range.selectNode(document.getElementById(\"partyID\"));\r\n"
                        "window.getSelection().addRange(range);\r\n"
                        "document.execCommand(\"copy\");\r\n"
                        "}\r\n"
                        "}\r\n"
                        "</script>\r\n"

                        "<style>\r\n"
                        "*{\r\n"
                        "font-family: 'Raleway', sans-serif;\r\n"
                        "background-color: #18191c;\r\n"
                        "color: white;\r\n"
                        "}\r\n"
                        "p{\r\n"
                        "text-align: center;\r\n"
                        "}\r\n"
                        "h3{\r\n"
                        "text-align: center;\r\n"
                        "}\r\n"
                        "body{\r\n"
                        "position: absolute;\r\n"
                        "left: 50%;\r\n"
                        "top: 40%;\r\n"
                        "-webkit-transform: translate(-50%, -50%);\r\n"
                        "transform: translate(-50%, -50%);\r\n"
                        "}\r\n"

                        "button{\r\n"
                        "position: absolute;\r\n"
                        "left: 50%;\r\n"
                        "top: 105%;\r\n"
                        "-webkit-transform: translate(-50%, -50%);\r\n"
                        "transform: translate(-50%, -50%);\r\n"
                        "}\r\n"

                        ".hover{\r\n"
                        "/* font */\r\n"
                        "color: white;\r\n"
                        "font-size: 20px;\r\n"
                        "font-family: 'Raleway';\r\n"
                        "/* remove blue underline */\r\n"
                        "text-decoration: none;\r\n"
                        "/* border */\r\n"
                        "border: 2px solid #18191c;\r\n"
                        "border-radius: 20px;\r\n"
                        "/* transitions */\r\n"
                        "transition-duration: .2s;\r\n"
                        "-webkit-transition-duration: .2s;\r\n"
                        "-moz-transition-duration: .2s;\r\n"
                        "/* other */\r\n"
                        "background-color: #18191c;\r\n"
                        "padding: 4px 30px;\r\n"
                        "}\r\n"
                        ".hover:hover{\r\n"
                        "/* update text color and background color */\r\n"
                        "color: #18191c;\r\n"
                        "background-color:white;\r\n"
                        "/* transitions */\r\n"
                        "transition-duration: .2s;\r\n"
                        "-webkit-transition-duration: .11s;\r\n"
                        "-moz-transition-duration: .2s;\r\n"
                        "}\r\n"

                        ".hover:active{\r\n"
                        "background-color: cyan;\r\n"
                        "}\r\n"

                        "pre{\r\n"
                        "font-family: Consolas;\r\n"
                        "font-size: 20pt;\r\n"
                        "color: #f45397;\r\n"
                        "}\r\n"
                        "</style>\r\n"
                        "</body>\r\n"
                        "</html>\r\n"
                        );

        res.set_content(returnHTML, "text/html");

        delete client;

    });

    app.Get(R"(/party/v1/join/([a-f0-9\\-]+))", [&](const httplib::Request& req, httplib::Response& res) {

        auto partyID = req.matches[1];
        res.set_content(partyID, "text/text");
        std::cout << partyID;

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
