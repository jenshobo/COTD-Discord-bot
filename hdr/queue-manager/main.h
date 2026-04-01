#ifndef MANAGER_MAIN_H
#define MANAGER_MAIN_H

#include "common.h"
#include <filesystem>

#define ADD_COMMAND     "add"
#define ADDPRIO_COMMAND "addprio"
#define REMOVE_COMMAND  "remove"
#define LIST_COMMAND    "list"
#define GET_COMMAND     "get"
#define HELP_COMMAND    "help"

#define ADD_DESCRIPTION     "Add an entree to the back of the queue"
#define ADDPRIO_DESCRIPTION "Add an entree to the back of the prioqueue"
#define REMOVE_DESCRIPTION  "Remove an entree BY INDEX"
#define LIST_DESCRIPTION    "List the current names and users in the queue"
#define GET_DESCRIPTION     "Get the picture of a user in the list, BY INDEX"
#define HELP_DESCRIPTION    "I can't help you with stupidity"

#define ERROR_REPLY_GET "Could not get image, please check if the entree exists."

#define PERMISSION_REPLY_ERROR \
               "Helaas heb jij geen toegang om direct met deze bot te interacten." << std::endl \
            << "Mocht je iets willen doen met de bot, neem dan contact op met <@384087009553022977>." << std::endl

#define HELP_TEXT \
               "you can manage the queue using the following commands:" << std::endl \
            << "```" << std::endl \
            << "+----------+-------------------------------------------------+" << std::endl \
            << "| /" << ADD_COMMAND << "     | " << ADD_DESCRIPTION <<                 "          |" << std::endl \
            << "| /" << ADDPRIO_COMMAND << " | " << ADDPRIO_DESCRIPTION <<                 "      |" << std::endl \
            << "| /" << REMOVE_COMMAND << "  | " << REMOVE_DESCRIPTION << "                       |" << std::endl \
            << "| /" << LIST_COMMAND << "    | " << LIST_DESCRIPTION <<                       "   |" << std::endl \
            << "| /" << GET_COMMAND << "     | " << GET_DESCRIPTION <<                          " |" << std::endl \
            << "+----------+-------------------------------------------------+" << std::endl \
            << "```" << std::endl \
            << "To submit an entree, you'll need to provide the name of the cat, the Discord ID of the person submitting it and the URL to the image." << std::endl \
            << std::endl \
            << "To get the Discord ID of a user you can get this by going to their Discord profile, pressing ... (more) and pressing Copy User ID." << std::endl \
            << "To upload a picture, simply provide the URL by rightclicking the image in Discord and pressing Copy Link. Please make sure the URL contains ONLY the picture." << std::endl \
            << "Too avoid a really large message, /" << LIST_COMMAND << " will not show the pictures, please use /" << GET_COMMAND << " followed by the INDEX to see the one picture." << std::endl \
            << std::endl \
            << "The entree at the front of the queue will automatically be posted to the COTD channel and removed from the queue every day at 12:00." << std::endl \
            << "Please make sure the queue is alway filled with entrees." << std::endl \
            << std::endl \
            << "For further questions regarding this software, please contact the software commission." << std::endl

#define COMMAND_OPTION_CAT_NAME             "name"
#define COMMAND_OPTION_CAT_DESCRIPTION      "The name of the cat in the picture"
#define COMMAND_OPTION_OWNER_NAME           "id"
#define COMMAND_OPTION_OWNER_DESCRIPTION    "The Discord ID of the owner / submitter of the picture"
#define COMMAND_OPTION_URL_NAME             "url"
#define COMMAND_OPTION_URL_DESCRIPTION      "The URL to the picture"

#define COMMAND_OPTION_REMOVE_NAME          "index"
#define COMMAND_OPTION_REMOVE_DESCRIPTION   "Index of entree to be removed"
#define COMMAND_OPTION_GET_NAME             COMMAND_OPTION_REMOVE_NAME
#define COMMAND_OPTION_GET_DESCRIPTION      "Get picture of entree in queue"

#define ADD_COMMAND_RESPONCE(a)     "Adding " << a << " to the queue"
#define ADDPRIO_COMMAND_RESPONCE(a) "Adding " << a << " to the prio-queue"
#define REMOVE_COMMAND_RESPONCE(a)  "Removing item at index " << a << " from the queue"

void add_to_queue(dpp::cluster& bot, const std::string& cat_name, const dpp::snowflake& user_id, const dpp::attachment& att, bool prio);
void remove_from_queue(uint64_t index);
void list_queue(std::stringstream& ss);
std::string get_url(uint64_t index);

#endif