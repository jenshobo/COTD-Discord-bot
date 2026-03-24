#ifndef CAT_H
#define CAT_H

#include <iostream>

#define CAT_NAME_FIELD "name"
#define CAT_OWNER_ID_FIELD "user_id"
#define CAT_URL_FIELD "url"

#define FILE_FOLDER "/"

class cat {
    public:
        std::string name;
        dpp::snowflake owner_id;
        std::string url;
        // Change full URL to local URL
        std::string get_short_url() {
            std::stringstream ss;
            for (int index = url.length(); url[index] != '/'; index--) {
                ss << url[index];
            }

            std::stringstream sss;
            std::string s = ss.str();
            for (int index = s.length(); index >= 0; index--) {
                sss << s[index];
            }

            return sss.str();
        }
};

#endif