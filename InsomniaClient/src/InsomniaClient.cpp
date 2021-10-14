#include "../include/httplib.h"
#include "LocalRiotClient.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

std::string clientVersion = "release-03.06-shipping-8-610061";

size_t writef(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

int main() {
    // For text colouring
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 13);

    /**********************************HEADER**********************************/
    std::cout
        << " ___                                 _       "      << std::endl
        << "|_ _|_ __  ___  ___  _ __ ___  _ __ (_) __ _ "      << std::endl
        << " | || '_ \\/ __|/ _ \\| '_ ` _ \\| '_ \\| |/ _` |"  << std::endl
        << " | || | | \\__ \\ (_) | | | | | | | | | | (_| |"    << std::endl
        << "|___|_| |_|___/\\___/|_| |_| |_|_| |_|_|\\__,_|"    << std::endl;
    /**************************************************************************/

    LocalRiotClient* client = new LocalRiotClient();
    if(!client->bIsGameOpen()) {
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << "Valorant is not currently running..." << std::endl;
        std::cout << "Press ";
        SetConsoleTextAttribute(hConsole, 11);
        std::cout << "ENTER ";
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << "to quit";
        system("pause >nul");
        delete client;
        return 0;
    }

    // Initial POST data
    std::string puuid;
    std::string name;
    int rankID;
    /*******************/
    
    std::vector<std::string> auth = client->getCredentials();
    delete client;

    puuid = auth[2];

    CURL* curl = curl_easy_init();

    struct curl_slist* headers = NULL;

    std::string str = "https://pd.na.a.pvp.net/mmr/v1/players/" + auth[2] + "/competitiveupdates?startIndex=0&endIndex=1&queue=competitive";
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
    rankID = resp["Matches"][0]["TierAfterUpdate"];

    curl_easy_setopt(curl, CURLOPT_URL, "https://pd.NA.a.pvp.net/name-service/v2/players");
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    str = "[\r\n\"" + puuid + "\"\r\n]";
    const char* nameBody = str.c_str();
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, nameBody);

    std::string nameResponse;

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writef);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &nameResponse);

    curl_easy_perform(curl);

    nlohmann::json jsonResp = nlohmann::json::parse(nameResponse);

    name = jsonResp[0]["GameName"];

    headers = NULL;

    /////////////////////////////////////////////////////////////////////////

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_URL, "https://insomnia-discordbot.herokuapp.com/"); // tentative until I find a hosting solution

    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    str = "{\n\t\"puuid\": \"" + puuid + "\",\n\t\"name\": \"" + name + "\",\n\t\"rankID\": " + std::to_string(rankID) + "\n }";
    const char* postFields = str.c_str();
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields);

    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    httplib::Server app;

    app.Get("/party/v1/create", [](const httplib::Request&, httplib::Response& res) {

        LocalRiotClient* client = new LocalRiotClient();
        std::vector<std::string> auth = client->getCredentials();
        delete client;

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
        curl_easy_cleanup(curl);

        std::string returnHTML =
            std::string("<!DOCTYPE html>\r\n"
                        "<html>\r\n"
                        "<head>\r\n"
                        "<title>Insomnia</title>\r\n"
                        "<link rel=\"preconnect\" href=\"https://fonts.gstatic.com\">\r\n"
                        "<link href=\"https://fonts.googleapis.com/css2?family=Raleway&display=swap\" rel=\"stylesheet\">\r\n"
                        "<link rel=\"icon\" href=\"https://raw.githubusercontent.com/Focusucof/InsomniaClient/master/assets/aperture.png\">\r\n"
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

    });

    app.Get(R"(/party/v1/join/([a-f0-9\\-]+))", [&](const httplib::Request& req, httplib::Response& res) {

        std::string partyID = req.matches[1];

        LocalRiotClient* client = new LocalRiotClient();
        std::vector<std::string> auth = client->getCredentials();

        CURL* curl;
        struct curl_slist* headers = NULL;

        curl = curl_easy_init();
        std::string str = "https://glz-na-1.na.a.pvp.net/parties/v1/players/" + auth[2] + "/joinparty/" + partyID;
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
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");

        curl_easy_perform(curl);

        std::string returnHTML =
            std::string("<!DOCTYPE html>\r\n"
                        "<html>\r\n"
                        "<head>\r\n"
                        "<title>Insomnia</title>\r\n"
                        "<link rel=\"preconnect\" href=\"https://fonts.gstatic.com\">\r\n"
                        "<link href=\"https://fonts.googleapis.com/css2?family=Raleway&display=swap\" rel=\"stylesheet\">\r\n"
                        "<link rel=\"icon\" href=\"https://raw.githubusercontent.com/Focusucof/InsomniaClient/master/assets/aperture.png\">\r\n"
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
                        "<p>You have successfully joined the party</p>\r\n"
                        "<p style=\"color: aqua; \">You may close this window</p>\r\n"
                        "</div>\r\n"

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

    app.Get("/stats/v1/link", [](const httplib::Request&, httplib::Response& res) {

        LocalRiotClient* client = new LocalRiotClient();
        std::vector<std::string> auth = client->getCredentials();
        delete client;
        std::string puuid = auth[2];

        std::string returnHTML =
            std::string("<!DOCTYPE html>\r\n"
                        "<html>\r\n"
                        "<head>\r\n"
                        "<title>Insomnia</title>\r\n"
                        "<link rel=\"preconnect\" href=\"https://fonts.gstatic.com\">\r\n"
                        "<link href=\"https://fonts.googleapis.com/css2?family=Raleway&display=swap\" rel=\"stylesheet\">\r\n"
                        "<link rel=\"icon\" href=\"https://raw.githubusercontent.com/Focusucof/InsomniaClient/master/assets/aperture.png\">\r\n"
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
                        "<p>Paste this into the #link channel in the Insomnia Discord</p>\r\n"
                        "<button class=\"hover\" onclick = \"copy()\">Copy</button>\r\n"
                        "<h3 id=\"puuid\"><code>.link ") + puuid + std::string("</code></h3>\r\n"
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
                        "range.moveToElementText(document.getElementById(\"puuid\"));\r\n"
                        "range.select().createTextRange();\r\n"
                        "document.execCommand(\"copy\");\r\n"
                        "} else if(window.getSelection) {\r\n"
                        "var range = document.createRange();\r\n"
                        "range.selectNode(document.getElementById(\"puuid\"));\r\n"
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

    });

    SetConsoleTextAttribute(hConsole, 11);
    std::cout << "App running on port 1337";
    SetConsoleTextAttribute(hConsole, 7);
    app.listen("127.0.0.1", 1337);

}
