# COTD Discord bot

Bot made for use in the [Peer2Peer](https://www.sv-peer2peer.nl/) Discord server. This bot is made with [sytemd](https://github.com/systemd/systemd) in mind, it consist of two programs, manager-app and update-app. Bot run under the same Discord bot.

### manager-app

This program is to be run at startup of the server, this can be achived using a ```.service``` program in systemd. This program will interact with people whom are allowed to manage the whole Discord bot. It'll hold all the logic for the commands to add and remove entrees.

### update-app

This program is to be run periodically, either manually or using ```.timer``` and ```.service``` files for automatic using in systemd. This program will take the top entree in the queue and post it in the channel it's pointed at. This program should not run more than ones an hour, I recommend ones every 24 hours.

## privilages

To run on a physical server and on the Discord server it is important to grant the bot specific privileges. On the physical server where the apps will run it is important the programs are allowed to read and write to the file ```COTD-queue.json``` which is to be placed in the same folder as the apps. To allow the bot to run in Discord it should be granted the following privilages buring the bot's creation in the [Discord developer environment](https://discord.com/developers/applications) OAuth2 URL generator, this URL can be pasted into a browser to invite the bot into the server:

| Catagory | permission |
| - | - |
| scopes | message.read |
| scopes | bot |
| text permissions | Send Messages |

Then for it to run properly in Discord it should have ```View Channel``` and ```Send Messages``` access to two channels, this being the private channel where questions are managed with the commission whom is responsible for this and the COTD channel where it'll send all it's items from the queue.

## commands

The manager app knows a number of commands in order to interface the queue with the people responsible for managing it. These commands are as follows:

| Command | Description |
| - | -- |
| ```/add``` | Add an entree to the back of the queue. |
| ```/addprio```| Add an entree to the back of the prioqueue. |
| ```/remove``` | Remove an entree BY INDEX. |
| ```/list``` | List the current names and users in the queue. |
| ```/get``` | Get the picture of a user in the list, BY INDEX. |
| ```/help``` | A short explaination about the functionality of the bot. |

As this bot is made for [Peer2Peer](https://www.sv-peer2peer.nl/) some text in the bot might reflect this.

## packages

In order to succesfully compile these apps the following programs are needed to be installed and/or downloaded:

| App | Where |
| - | - |
| [D++ Discord](https://dpp.dev/) | Global install |
| [nlohmann json.hpp](https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp) | place file in ```lib``` folder |

## install guide

To completely set up the program and bot the progress boils down to two steps: setting up the Discord bot and compiling the code.

### setup Discord bot

We assume you at least have a basic understanding of mananing a Discord server.

To start you need to setup the Discord bot that'll post the COTD. To do this you'll need to go the the [Discord developer environment](https://discord.com/developers/applications). Log in using your Discord account, then you should be able to create a new application.

Ones created you should be able to enter it. In the tab ```bot``` you should be able to reset your token, save this token as you'll need it during the install progress of the program later.

You then need to invite the bot over to your Discord, to do this you need to go to the tab ```OAuth2```. In here you need to use the ```OAuth2 URL Generator``` to create a URL, please make sure to check ```message.read``` and ```bot```, ```bot``` should open another list of checkmarks below, in here, check ```Send Messages```. Then at the bottom there should be a generated URL, copy and paste this into a new browser tab to invite the bot over to your server.

In Discord you need to give the bot permission to view the channel and send messages in two channels, one where you will manage the bot behind the scenes and one where the bot will post the C for every other user to see. The bot does not need permission to use any other channel (as a matter of fact I recommend not allowing the bot in any other channels).

Lastly the bot will need the channel ID of the channel you want it to send the COTD to. You can get this by right clicking the channel and press ```copy channel ID```. Save this ID for later as you need to during the install progress for the program. Do the same for alerts if you want to be alerted if the queue is not getting new enrtees.

### compile guide

To compile the program you'll need to all listed packages downloaded and/or installed. Furthermore please create a folder called build in the working directory. Then execute the following commands to compile the two programs, these will be saved to the build folder.

```sh
g++ -std=c++17 -Ihdr/queue-update -Ilib -Ihdr -o build/update-app src/queue-update/main.cpp src/json.cpp -ldpp
g++ -std=c++17 -Ihdr/queue-manager -Ilib -Ihdr -o build/manager-app src/queue-manager/main.cpp src/json.cpp -ldpp
```

Next you'll need to create a json file in which the queue will be stored among a few other things, this file looks like follows. You'll need to manually add the discord bot token you've generated earier. Offset is just a number used to index all the items in the queue, if you have never done a COTD in Discord before, leave this as 1. post-channel-id is the id of the channel the bot will post all the entrees.

```json
{
    "alert_channel": 0,
    "offset": 1,
    "post_channel": 0,
    "prio_queue": [],
    "queue": [],
    "token": ""
}
```

This file needs to be saved in the same folder as the two programs, as both programs interact with it. The file should be named ```COTD-queue.json``` Avoid sharing the contents of this file as the discord-bot-token is considered sensitive information.

Next simply run the manager-app program indefinitively and run the update-app ones a day to see the program in action.
