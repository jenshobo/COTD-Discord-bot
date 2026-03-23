#include "main.h"

int main(void) {
    dpp::cluster bot(get_token());
    bot.on_log(dpp::utility::cout_logger());
    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (get_queue_length() <= 3) {
            bot.message_create(dpp::message(get_alert_channel(), "**[ALERT]**. Queue has less then 3 items in it, check current list using `/list`."),
                [&bot](const dpp::confirmation_callback_t& callback) {
                    if (!callback.is_error()) {
                        std::cout << "Alert sent successfully" << std::endl;
                    } else {
                        std::cerr << "Failed to send alert: " << callback.get_error().message << std::endl;
                    }
                }
            );
        }

        cat entree;
        get_entree(entree);

        std::stringstream ss;
        ss << "<@" << entree.owner_id << "> would like you to meet " << entree.name;

        dpp::message msg(get_post_channel(), ss.str());
        msg.set_file_content(dpp::utility::read_file(entree.url));
        msg.set_filename(entree.url);

        bot.message_create(msg,
            [&bot, &entree](const dpp::confirmation_callback_t& callback) {
                if (!callback.is_error()) {
                    std::cout << "Question sent successfully" << std::endl;
                } else {
                    std::cerr << "Failed to send question: " << callback.get_error().message << std::endl;
                }

                // Keep program running for at least {SHUTDOWN_DELAY} time for shards to finish loading
                std::this_thread::sleep_for(std::chrono::seconds(SHUTDOWN_DELAY));

                std::remove(entree.url.c_str());

                bot.shutdown();
            }
        );
    });

    bot.start(dpp::st_wait);
    return 0;
}

int get_queue_length(void) {
    std::queue<cat> queue = get_queue();
    std::queue<cat> prio_queue = get_prio_queue();

    int queue_size = queue.size();
    int prio_queueu_size = prio_queue.size();

    return queue_size + prio_queueu_size;
}

void get_entree(cat& entree) {
    std::queue<cat> queue = get_queue();
    std::queue<cat> prio_queue = get_prio_queue();

    if (!prio_queue.empty()) {
        entree.name = prio_queue.front().name;
        entree.owner_id = prio_queue.front().owner_id;
        entree.url = prio_queue.front().url;
        prio_queue.pop();
    }
    else if (!queue.empty()) {
        entree.name = queue.front().name;
        entree.owner_id = queue.front().owner_id;
        entree.url = queue.front().url;
        queue.pop();
    }

    increment_offset();
    write_queue(prio_queue, queue);
}