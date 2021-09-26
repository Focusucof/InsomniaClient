// ApertureClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "LocalRiotClient.h"

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

    std::string auth = client->getCredentials();
    std::cout << auth;

    delete client;
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
