#include "main.h"

int main(void) {
    dpp::cluster bot(get_token());
    bot.on_log(dpp::utility::cout_logger());
    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == HELP_COMMAND) {
            std::stringstream ss;
            ss << HELP_TEXT;
            event.reply(ss.str());
        }
        if (event.command.get_command_name() == ADD_COMMAND) {
            std::string cat_name = std::get<std::string>(event.get_parameter(COMMAND_OPTION_CAT_NAME));
            dpp::snowflake user_id = std::get<dpp::snowflake>(event.get_parameter(COMMAND_OPTION_OWNER_NAME));
            dpp::snowflake file_id = std::get<dpp::snowflake>(event.get_parameter(COMMAND_OPTION_URL_NAME));
            dpp::attachment att = event.command.get_resolved_attachment(file_id);
 
            std::stringstream ss;
            ss << ADD_COMMAND_RESPONCE(cat_name);

            event.reply(ss.str());

            add_to_queue(bot, cat_name, user_id, att, false);
        }
        if (event.command.get_command_name() == ADDPRIO_COMMAND) {
            std::string cat_name = std::get<std::string>(event.get_parameter(COMMAND_OPTION_CAT_NAME));
            dpp::snowflake user_id = std::get<dpp::snowflake>(event.get_parameter(COMMAND_OPTION_OWNER_NAME));
            dpp::snowflake file_id = std::get<dpp::snowflake>(event.get_parameter(COMMAND_OPTION_URL_NAME));
            dpp::attachment att = event.command.get_resolved_attachment(file_id);
 
            std::stringstream ss;
            ss << ADDPRIO_COMMAND_RESPONCE(cat_name);

            event.reply(ss.str());

            add_to_queue(bot, cat_name, user_id, att, true);
        }
        if (event.command.get_command_name() == REMOVE_COMMAND) {
            uint64_t index = std::get<int64_t>(event.get_parameter(COMMAND_OPTION_REMOVE_NAME));
 
            std::stringstream ss;
            ss << REMOVE_COMMAND_RESPONCE(index);

            event.reply(ss.str());

            remove_from_queue(index);
        }
        if (event.command.get_command_name() == LIST_COMMAND) {
            std::stringstream ss;
            list_queue(ss);
            
            event.reply(ss.str());
        }
        if (event.command.get_command_name() == GET_COMMAND) {
            uint64_t index = std::get<int64_t>(event.get_parameter(COMMAND_OPTION_GET_NAME));
            dpp::message msg(event.command.channel_id, "");

            std::string url = get_url(index);

            if (url != "") {
                msg.set_file_content(dpp::utility::read_file(url));
                msg.set_filename(url);

                event.reply(msg);
            }
            else {
                event.reply(ERROR_REPLY_GET);
            }
        }
    });

    bot.on_ready([&bot](const dpp::ready_t&event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand(HELP_COMMAND, HELP_DESCRIPTION, bot.me.id));
            bot.global_command_create(dpp::slashcommand(ADD_COMMAND, ADD_DESCRIPTION, bot.me.id)
                .add_option(dpp::command_option(dpp::co_string, COMMAND_OPTION_CAT_NAME, COMMAND_OPTION_CAT_DESCRIPTION, true))
                .add_option(dpp::command_option(dpp::co_user, COMMAND_OPTION_OWNER_NAME, COMMAND_OPTION_OWNER_DESCRIPTION, true))
                .add_option(dpp::command_option(dpp::co_attachment, COMMAND_OPTION_URL_NAME, COMMAND_OPTION_URL_DESCRIPTION, true))
            );
            bot.global_command_create(dpp::slashcommand(ADDPRIO_COMMAND, ADDPRIO_DESCRIPTION, bot.me.id)
                .add_option(dpp::command_option(dpp::co_string, COMMAND_OPTION_CAT_NAME, COMMAND_OPTION_CAT_DESCRIPTION, true))
                .add_option(dpp::command_option(dpp::co_user, COMMAND_OPTION_OWNER_NAME, COMMAND_OPTION_OWNER_DESCRIPTION, true))
                .add_option(dpp::command_option(dpp::co_attachment, COMMAND_OPTION_URL_NAME, COMMAND_OPTION_URL_DESCRIPTION, true))
            );
            bot.global_command_create(dpp::slashcommand(REMOVE_COMMAND, REMOVE_DESCRIPTION, bot.me.id)
                .add_option(dpp::command_option(dpp::co_integer, COMMAND_OPTION_REMOVE_NAME, COMMAND_OPTION_REMOVE_DESCRIPTION, true))
            );
            bot.global_command_create(dpp::slashcommand(LIST_COMMAND, LIST_DESCRIPTION, bot.me.id));
            bot.global_command_create(dpp::slashcommand(GET_COMMAND, GET_DESCRIPTION, bot.me.id)
                .add_option(dpp::command_option(dpp::co_integer, COMMAND_OPTION_GET_NAME, COMMAND_OPTION_REMOVE_DESCRIPTION, true))
            );
        }
    });

    bot.start(dpp::st_wait);
}

void download_file(dpp::cluster& bot, const std::string& url, const std::string& path) {
    bot.request(url, dpp::m_get, [path](const dpp::http_request_completion_t& res) {
        if (res.status == 200) {
            std::ofstream out(path, std::ios::binary);
            out.write(res.body.data(), res.body.size());
            out.close();
        } else {
            std::cerr << "Download failed: HTTP " << res.status << std::endl;
        }
    });
}

void add_to_queue(dpp::cluster& bot, const std::string& cat_name, const dpp::snowflake& user_id, const dpp::attachment& att, bool prio) {
    std::queue<cat> queue = get_queue();
    std::queue<cat> prioqueue = get_prio_queue();

    std::stringstream local_url;
    local_url << std::filesystem::current_path().u8string() << "/files/" << att.filename;

    download_file(bot, att.url, local_url.str());

    cat new_cat;
    new_cat.name = cat_name;
    new_cat.owner_id = user_id;
    new_cat.url = local_url.str();

    if (prio) { prioqueue.push(new_cat); }
    else      { queue.push(new_cat); }

    write_queue(prioqueue, queue);
}

void remove_from_queue(uint64_t index) {
    std::queue<cat> queue = get_queue();
    std::queue<cat> prio_queue = get_prio_queue();

    std::queue<cat> new_queue;
    std::queue<cat> new_prio_queue;

    uint64_t offset = get_offset();

    while (!prio_queue.empty()) {
        if (offset != index) { new_prio_queue.push(prio_queue.front()); }
        else { std::remove(prio_queue.front().url.c_str()); }
        prio_queue.pop();
        offset++;
    }

    while (!queue.empty()) {
        if (offset != index) { new_queue.push(queue.front()); }
        else { std::remove(queue.front().url.c_str()); }
        queue.pop();
        offset++;
    }

    write_queue(new_prio_queue, new_queue);
}

void list_queue(std::stringstream& ss) {
    std::queue<cat> queue = get_queue();
    std::queue<cat> prio_queue = get_prio_queue();

    uint64_t offset = get_offset();

    while (!prio_queue.empty()) {
        ss << offset << ". "
           << prio_queue.front().name << " "
           << "<@" << prio_queue.front().owner_id << ">" << std::endl;
        prio_queue.pop();
        offset++;
    }

    while (!queue.empty()) {
        ss << offset << ". "
           << queue.front().name << " "
           << "<@" << queue.front().owner_id << ">" << std::endl;

        queue.pop();
        offset++;
    }
}

std::string get_url(uint64_t index) {
    std::queue<cat> queue = get_queue();
    std::queue<cat> prio_queue = get_prio_queue();

    uint64_t offset = get_offset();

    while (!prio_queue.empty()) {
        if (offset == index) { return prio_queue.front().url; }
        prio_queue.pop();
        offset++;
    }

    while (!queue.empty()) {
        if (offset == index) { return queue.front().url; }
        queue.pop();
        offset++;
    }

    return "";
}