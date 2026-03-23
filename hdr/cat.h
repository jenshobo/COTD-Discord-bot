#ifndef CAT_H
#define CAT_H

#include <iostream>

#define CAT_NAME_FIELD "name"
#define CAT_OWNER_ID_FIELD "user_id"
#define CAT_URL_FIELD "url"

class cat {
    public:
        std::string name;
        dpp::snowflake owner_id;
        std::string url;
};

#endif