#define _CRT_SECURE_NO_WARNINGS

#include "LocalRiotClient.h"
#include <stdlib.h>
#include <sstream>
#include <algorithm>

size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
	data->append((char*)ptr, size * nmemb);
	return size * nmemb;
}

LocalRiotClient::LocalRiotClient() {

	curl = curl_easy_init();
	
	/*if(curl) {

		curl_easy_setopt(curl, CURLOPT_URL, "localhost:3000");

		std::string response;
		std::string header;
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);

		curl_easy_perform(curl);

		std::cout << "A" << header << std::endl;
		std::cout << "B" << response << std::endl;

	}*/

}

LocalRiotClient::~LocalRiotClient() {

	curl_easy_cleanup(curl);

}

bool LocalRiotClient::bIsGameOpen() {

	const std::string appDataPath = std::getenv("LOCALAPPDATA");
	std::ifstream file(appDataPath + "\\Riot Games\\Riot Client\\Config\\lockfile");
	
	std::string t;
	if(std::getline(file, t)) {
		return true;
	} else {
		return false;
	}

}

std::vector<std::string> LocalRiotClient::parseLockFile() {

	// lockfile path
	const std::string appDataPath = std::getenv("LOCALAPPDATA");
	std::ifstream file(appDataPath + "\\Riot Games\\Riot Client\\Config\\lockfile");

	// reads lockfile and stores it in the lockfile string
	std::string lockfile;
	std::getline(file, lockfile);

	// replaces colons with spaces
	std::replace(lockfile.begin(), lockfile.end(), ':', ' ');  // replace ':' by ' '

	// splits the string on spaces and pushs it to an array
	std::vector<std::string> array;
	std::stringstream ss(lockfile);
	std::string temp;
	while(ss >> temp) {
		array.push_back(temp);
	}
	
	return array;

}