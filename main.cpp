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
            //
            dpp::slashcommand stats("stats", "Observa tus stats", bot.me.id);
            stats.add_option(
                dpp::command_option(dpp::co_user, "usuario", "Usuario que quieres consultar", false)
            );
            bot.global_command_create(stats);
            //
			bot.global_command_create(
				dpp::slashcommand("pvp", "busca pvp y alguien te respondera", bot.me.id)
            );
			//
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
            xpn[id] = 50;

        if (xp[id] >= xpn[id]) {
            xp[id] -= xpn[id];
            xpn[id] += 50;
            level[id]++;
        }
// comando 1 ### hola
        if (event.command.get_command_name() == "hola") {
            event.reply("adios 🥺");
        }
// comando 2### pvp
		if(event.command.get_command_name() == "pvp"){
            dpp::component buton = dpp::component().set_type(dpp::cot_button).set_label("ACEPTAR").set_id("pedir").set_style(dpp::cos_primary);

			dpp::embed embes;
            embes.set_title("BUSCANDO CONTRINCANTE");
            embes.set_description("EL USUARIO <@" + std::to_string(id) + "> \n esta buscando pvp");
            dpp::message msg(event.command.channel_id, embes);
            
            msg.add_component(
dpp::component().add_component(buton)
);
            event.reply(msg);
        }

// comando 3## stats

        else if (event.command.get_command_name() == "stats") {
            if (event.get_parameter("usuario").index() != 0) {
                id = std::get<dpp::snowflake>(event.get_parameter("usuario"));
                stade = false;

            }

            if (!stade) {
                dpp::user usuario = event.command.get_resolved_user(id);

                
                usere = usuario;

                if (xpn[id] == 0 && level[id] == 0)
                    xpn[id] = 50;

                if (xp[id] >= xpn[id]) {
                    xp[id] -= xpn[id];
                    xpn[id] += 50;
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

	// botones ##
    bot.on_button_click([](const dpp::button_click_t& event) {

    if (event.custom_id == "pedir") {
		dpp::embed embec;
        embec.set_title("CONTRINCANTE ENCONTRADO");
        embec.set_description("AHORA TE TOCARA LUCHAR CONTRA: <@" + std::to_string(event.command.get_issuing_user().id) + ">");
        event.reply(dpp::message(event.command.channel_id, embec));
        
    }

});

    bot.start(dpp::st_wait);
}