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
            bot.global_command_create(
                dpp::slashcommand("stats", "observa tus stats", bot.me.id)
            );

	bot.global_command_create(
                dpp::slashcommand("hola", "Responde con un saludo!", bot.me.id)
            );
        }
    });

    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
		dpp::snowflake id = event.command.get_issuing_user().id;
		if(xpn[id] == 0){
			xpn[id] = 10;
            }
        if(xp[id] >= xpn[id]){
            xpn[id] += 20;
            xp[id] = 0;
            level[id]++;
        }

        if (event.command.get_command_name() == "hola")
            event.reply("adios 🥺");
    
		else if (event.command.get_command_name() == "stats"){
            dpp::embed embes;
        	embes.set_title("TU CARTA: ");
            std::string fmsg = "# TUS STATS \n";
            fmsg += "Xp: ";
            fmsg += std::to_string(xp[id]);
			fmsg += " / ";
            fmsg += std::to_string(xpn[id]);
            embes.set_description(fmsg);
            embes.add_field("nombre: ", event.command.get_issuing_user().username);
            embes.set_thumbnail(event.command.get_issuing_user().get_avatar_url());
            embes.add_field("nivel: ", std::to_string(level[id]));
            event.reply(dpp::message(event.command.channel_id, embes));
}
    });

    bot.start(dpp::st_wait);
}