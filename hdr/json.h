#ifndef JSON_H
#define JSON_H

#include "common.h"
#include "cat.h"

#define OFFSET_FIELD        "offset"
#define TOKEN_FIELD         "token"
#define ALERT_CHANNEL_FIELD "alert_channel"
#define POST_CHANNEL_FIELD  "post_channel"
#define PRIO_QUEUE_FIELD    "prio_queue"
#define QUEUE_FIELD         "queue"
#define DATE_FIELD          "previous_date"

#define DAY_TIME_T 86400

void write_queue(std::queue<cat> prio_queue, std::queue<cat> queue);

uint64_t get_offset(void);
void increment_offset(void);
std::string get_token(void);
dpp::snowflake get_alert_channel(void);
dpp::snowflake get_post_channel(void);
std::queue<cat> get_prio_queue(void);
std::queue<cat> get_queue(void);

std::string get_time(void);
void write_time(const std::string& time);
bool get_today(void);
void set_today(void);

void download_file(dpp::cluster& bot, const std::string& url, const std::string& path);

#endif