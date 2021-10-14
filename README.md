<pre align=center>
 ___                                 _       
|_ _|_ __  ___  ___  _ __ ___  _ __ (_) __ _ 
 | || '_ \/ __|/ _ \| '_ ` _ \| '_ \| |/ _` |
 | || | | \__ \ (_) | | | | | | | | | | (_| |
|___|_| |_|___/\___/|_| |_| |_|_| |_|_|\__,_|
</pre>

## ***The Insomnia Client is a simple VALORANT party client used to organize custom games easily.***

## Features

- Create and join parties easily without adding people as friends
- Low CPU and memory usage
- Full integration with the Insomnia 10 mans Discord
- Single, statically linked binary

## Installation

1. Visit the releases page of this repository and download the latest release
2. Store the downloaded file anywhere you want as it is a single file

    Windows might flag the file as a virus. If so, press more info, then continue anyways.

## Usage 

VALORANT must be running on your computer before you open this program.

#### To join a party:

- Click the "Join Party" link provided by the Discord bot

#### To create a party:

- Click the "Create Party" link provided by the Discord bot
- Copy the party ID
- In the Discord chat, type .host {party ID} and it will create the party and the join link

## Building

This project is built with Visual Studio 2022.
The only external dependency is libcurl which can be install with vcpkg. The command to install libcurl is:

``` vcpkg install curl:x64-windows-static ```

## Legal

**This program isn't endorsed by Riot Games and doesn't reflect the views or opinions of Riot Games or anyone officially involved in producing or managing Riot Games properties. Riot Games, and all associated properties are trademarks or registered trademarks of Riot Games, Inc.**

## Third Party Dependencies

### - [yhirose/cpp-httplib](https://github.com/yhirose/cpp-httplib)
Copyright (c) 2021 Yuji Hirose. All rights reserved. License: MIT

### - [nlohmann/json](https://github.com/nlohmann/json)
Copyright (c) 2013-2019 Niels Lohmann. License: MIT

### - [tomykaira/Base64.h](https://gist.github.com/tomykaira/f0fd86b6c73063283afe550bc5d77594)
Copyright (c) 2016 tomykaira. License: MIT

### - [feathericons/feather](https://github.com/feathericons/feather)
Copyright (c) 2013-2017 Cole Bemis. License: MIT