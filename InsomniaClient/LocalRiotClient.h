#pragma once

#include <curl/curl.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <algorithm>
#include "base64encode.h"
#include "json.h"

class LocalRiotClient {

	public:
		LocalRiotClient();
		~LocalRiotClient();

	public:
		bool bIsGameOpen();
		std::vector<std::string> parseLockFile();
		std::vector<std::string> getCredentials();

	private:
		CURL* curl;

};

