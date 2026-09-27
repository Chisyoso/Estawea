#include <dpp/dpp.h>
#include <cstdlib>

int main() {
    const char* token = std::getenv("DISCORD_TOKEN");

    if (!token) return 1;

    dpp::cluster bot(token);

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_commands>()) {
            bot.global_command_create(
                dpp::slashcommand("ping", "Responde con Pong!", bot.me.id)
            );
        }
    });

    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping")
            event.reply("Pong! 🏓");
    });

    bot.start(dpp::st_wait);
}