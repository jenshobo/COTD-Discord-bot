#include "json.h"

std::vector<nlohmann::json> convert_to_vec(std::queue<cat>& queue) {
    std::vector<nlohmann::json> queue_vec;
    while (!queue.empty()) {
        nlohmann::json j;
        j[CAT_NAME_FIELD] = queue.front().name;
        j[CAT_OWNER_ID_FIELD] = static_cast<uint64_t>(queue.front().owner_id);
        j[CAT_URL_FIELD] = queue.front().url;
        queue_vec.push_back(j);
        queue.pop();
    }
    return queue_vec;
}

void write_queue(
    std::optional<uint64_t> offset = std::nullopt,
    std::optional<std::string> token = std::nullopt,
    std::optional<dpp::snowflake> alert_channel = std::nullopt,
    std::optional<dpp::snowflake> post_channel = std::nullopt,
    std::optional<std::queue<cat>> prio_queue = std::nullopt,
    std::optional<std::queue<cat>> queue = std::nullopt
) {
    uint64_t offset_field = offset.value_or(get_offset());
    std::string token_field = token.value_or(get_token());
    dpp::snowflake alert_channel_field = alert_channel.value_or(get_alert_channel());
    dpp::snowflake post_channel_field = post_channel.value_or(get_post_channel());
    std::queue<cat> prio_queue_field = prio_queue.value_or(get_prio_queue());
    std::queue<cat> queue_field = queue.value_or(get_queue());

    std::vector<nlohmann::json> prio_queue_vec = convert_to_vec(prio_queue_field);
    std::vector<nlohmann::json> queue_vec = convert_to_vec(queue_field);

    nlohmann::json j;
    j[OFFSET_FIELD] = offset_field;
    j[TOKEN_FIELD] = token_field;
    j[ALERT_CHANNEL_FIELD] = static_cast<uint64_t>(alert_channel_field);
    j[POST_CHANNEL_FIELD] = static_cast<uint64_t>(post_channel_field);
    j[PRIO_QUEUE_FIELD] = prio_queue_vec;
    j[QUEUE_FIELD] = queue_vec;    

    std::ofstream file(QUEUE_FILE_NAME);
    file << j.dump(4);
}

void write_queue(std::queue<cat> prio_queue, std::queue<cat> queue) {
    write_queue(std::nullopt, std::nullopt, std::nullopt, std::nullopt, prio_queue, queue);
}

uint64_t get_offset(void) {
    std::ifstream file(QUEUE_FILE_NAME);
    nlohmann::json j;

    if (file >> j) {
        if (j.contains(OFFSET_FIELD) && j[OFFSET_FIELD].is_number_unsigned()) {
            return static_cast<uint64_t>(j[OFFSET_FIELD]);
        }
    }
    
    return 0;
}

void increment_offset(void) {
    uint64_t offset = get_offset();
    offset++;
    write_queue(offset);
}

std::string get_token(void) {
    std::ifstream file(QUEUE_FILE_NAME);
    nlohmann::json j;

    if (file >> j) {
        if (j.contains(TOKEN_FIELD) && j[TOKEN_FIELD].is_string()) {
            return j[TOKEN_FIELD];
        }
    }

    return "";
}

dpp::snowflake get_channel(std::string field) {
    std::ifstream file(QUEUE_FILE_NAME);
    nlohmann::json j;

    if (file >> j) {
        if (j.contains(field) && j[field].is_number_integer()) {
            return static_cast<dpp::snowflake>(j[field].get<uint64_t>());
        }
    }

    return 0;
}

dpp::snowflake get_alert_channel(void) {
    return get_channel(ALERT_CHANNEL_FIELD);
}

dpp::snowflake get_post_channel(void) {
    return get_channel(POST_CHANNEL_FIELD);
}

std::queue<cat> get_queue(std::string field) {
    std::ifstream file(QUEUE_FILE_NAME);
    nlohmann::json j;
    std::queue<cat> queue;

    if (file >> j) {
        if (j.contains(field) && j[field].is_array()) {
            for (const auto& item : j[field]) {
                cat temp;
                if (item.contains(CAT_NAME_FIELD) && item[CAT_NAME_FIELD].is_string()) { temp.name = item[CAT_NAME_FIELD]; }
                if (item.contains(CAT_OWNER_ID_FIELD) && item[CAT_OWNER_ID_FIELD].is_number_unsigned()) { temp.owner_id = static_cast<uint64_t>(item[CAT_OWNER_ID_FIELD]); }
                if (item.contains(CAT_URL_FIELD) && item[CAT_URL_FIELD].is_string()) { temp.url = item[CAT_URL_FIELD]; }

                queue.push(temp);
            }
        }
    }

    return queue;
}

std::queue<cat> get_prio_queue(void) {
    return get_queue(PRIO_QUEUE_FIELD);
}

std::queue<cat> get_queue(void) {
    return get_queue(QUEUE_FIELD);
}
