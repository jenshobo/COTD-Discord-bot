#include "main.h"
#include "queue-manager/main.h"

void shutdown(dpp::cluster& bot) {
    // Keep program running for at least {SHUTDOWN_DELAY} time for shards to finish loading
    std::this_thread::sleep_for(std::chrono::seconds(SHUTDOWN_DELAY));

    bot.shutdown();
}

int main(void) {
    dpp::cluster bot(get_token());
    bot.on_log(dpp::utility::cout_logger());
    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (get_today()) { shutdown(bot); return; }
        set_today();

        cat entree;
        uint64_t index = get_offset();
        get_entree(entree);

        if (entree.url == "") { shutdown(bot); }

        if (get_queue_length() <= 1) {
            std::string url = "https://cataas.com/cat";

            std::queue<cat> queue = get_queue();
            std::queue<cat> prioqueue = get_prio_queue();

            std::stringstream local_url;
            local_url << std::filesystem::current_path().u8string() << FILE_FOLDER << index << ".jpeg";

            download_file(bot, url, local_url.str());
                
            cat new_cat;
            new_cat.name = "";
            new_cat.owner_id = 0;
            new_cat.url = local_url.str();
                
            queue.push(new_cat);
                
            write_queue(prioqueue, queue);
        }

        std::stringstream ss;
        if (entree.owner_id != 0) {
            ss << index << ". <@" << entree.owner_id << ">" << std::endl
               << "Deelt graag met jou de kat " << entree.name << "." << std::endl;
        } else {
            ss << index << "." << std::endl;
        }

        dpp::message msg(get_post_channel(), ss.str());
        msg.set_file_content(dpp::utility::read_file(entree.url));
        msg.set_filename(entree.get_short_url());

        bot.message_create(msg,
            [&bot, entree](const dpp::confirmation_callback_t& callback) {
                if (!callback.is_error()) {
                    std::cout << "Entree sent successfully" << std::endl;
                } else {
                    std::cerr << "Failed to send question: " << callback.get_error().message << std::endl;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                
                std::filesystem::path path = entree.url;
                std::cout << "Trying to delete: " << path << std::endl;
                std::error_code ec;
                bool removed = std::filesystem::remove(path, ec);

                if (!removed) {
                    std::cerr << "Failed to remove file: " << ec.message() << std::endl;
                } else {
                    std::cout << "File removed successfully\n";
                }

                increment_offset();
                shutdown(bot);
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
    else {
        return;
    }

    write_queue(prio_queue, queue);
}