#include <dpp/dpp.h>
#include <cstdlib>
#include <map>
#include <cctype>
#include <string>

std::map<dpp::snowflake, int> xp, level, xpn, bt, btw;

int main() {
    const char* token = std::getenv("DISCORD_TOKEN");

    if (!token) return 1;

    dpp::cluster bot(token);

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_commands>()) {
            dpp::slashcommand stats("stats", "Observa tus stats", bot.me.id);
            stats.add_option(
                dpp::command_option(dpp::co_user, "usuario", "Usuario que quieres consultar", false)
            );
            bot.global_command_create(stats);

            bot.global_command_create(
                dpp::slashcommand("hola", "Responde con un saludo!", bot.me.id)
            );
        }
    });

    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        dpp::snowflake id = event.command.get_issuing_user().id;
        dpp::user usere = event.command.get_issuing_user();
        bool stade = true;

        if (xpn[id] == 0 && level[id] == 0)
            xpn[id] = 10;

        if (xp[id] >= xpn[id]) {
            xp[id] -= xpn[id];
            xpn[id] += 20;
            level[id]++;
        }
// comando 1 ###
        if (event.command.get_command_name() == "hola") {
            event.reply("adios 🥺");
        }
// comando 2###

        else if (event.command.get_command_name() == "stats") {
            if (event.get_parameter("usuario").index() != 0) {
                id = std::get<dpp::snowflake>(event.get_parameter("usuario"));
                stade = false;
            }

            if (!stade) {
                dpp::user usuario = event.command.get_resolved_user(id);

                if (!usuario) {
                    event.reply("No encontré ese usuario");
                    return;
                }

                usere = usuario;

                if (xpn[id] == 0 && level[id] == 0)
                    xpn[id] = 10;

                if (xp[id] >= xpn[id]) {
                    xp[id] -= xpn[id];
                    xpn[id] += 20;
                    level[id]++;
                }
            }

            dpp::embed embes;
            std::string fmsg;

            embes.set_title("TU CARTA:");

            if (stade) {
                fmsg = "# TUS STATS\n";
            } else {
                fmsg = "# STATS DE <@";
                fmsg += std::to_string(id);
                fmsg += ">\n";
            }

            fmsg += "Xp: ";
            fmsg += std::to_string(xp[id]);
            fmsg += " / ";
            fmsg += std::to_string(xpn[id]);

            embes.set_description(fmsg);
            embes.add_field("nombre:", usere.username);
            embes.set_thumbnail(usere.get_avatar_url());
            embes.add_field("nivel:", std::to_string(level[id]));

            event.reply(dpp::message(event.command.channel_id, embes));
        }
    });

    bot.start(dpp::st_wait);
}