#include <dpp/dpp.h>
#include <cstdlib>
#include <map>
#include <cctype>
#include <string>
#include <vector>

// usuario
std::map<dpp::snowflake, int> xp, level, xpn, bt, btw;
// server
std::map<dpp::snowflake, std::string[12][5]> user;

std::map<dpp::snowflake, int> roll, rollo; // roll y rollo para cambio de posicion en el arreglo

std::map<dpp::snowflake, dpp::snowflake, int> canmx, canmn, canmn2; // cantidad maxima y minima
// variable de comando

void inicia(int a, dpp::snowflake id){
	if(a == 10){
    for(int i = 0; i < 5; i++){
		for(int j = 1; j > 11; j++){
            user[id][j][i] = "?";
        }
    }
    else{
        a;
        for(int i = 1; i < 11; i++){
            user[id][i][a] = "?";
        }
    }
    }
}

void iniuser(dpp::snowflake id){
    if (xpn[id] == 0 && level[id] == 0)
            xpn[id] = 50;
        if (xp[id] >= xpn[id]) {
            xp[id] -= xpn[id];
            xpn[id] += 50;
            level[id]++;
        }
}

int main() {
    const char* token = std::getenv("DISCORD_TOKEN");

    if (!token) return 1;

    dpp::cluster bot(token);

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_commands>()) {
            //
            dpp::slashcommand pvp("pvp", "Comienza una batalla", bot.me.id);
            pvp.add_option( dpp::command_option(dpp::co_integer, "cantidad", "Cantidad de usuarios por equipo", true).set_min_value(1).set_max_value(5));
            
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
				dpp::snowflake ids = event.command.guild_id;
        bool stade = true;
		
        iniuser(id);

        if(user[ids][1][0] != "?"){
			inicia(10, ids);
        }
    	
        
// comando 1 ### hola
        if (event.command.get_command_name() == "hola") {
            event.reply("adios 🥺");
        }
// comando 2### pvp
		if(event.command.get_command_name() == "pvp"){
				int cantius = std::get<int>(event.get_parameter("cantidad"));
            dpp::component buton = dpp::component().set_type(dpp::cot_button).set_label("EQUIPO 1").set_id("join1").set_style(dpp::cos_primary);
            dpp::component boton = dpp::component().set_type(dpp::cot_button).set_label("EQUIPO 2").set_id("join2").set_style(dpp::cos_secondary);

			dpp::embed embes;

		inicia(roll, ids);

		rollo[ids].push_back(event.command.message_id);
        if(rollo[ids].size() > 5){
			rollo[ids].erase(0);
        }
		roll[event.command.message_id] = roll[ids];
		canmx[event.command.message_id] = cantius;
        canmn[event.command.message_id] = 0;
        canmn2[event.command.message_id] = 5;
        if(roll[ids] > 4){
            roll[ids] = 0;
        } else{
        roll[ids]++;}

            std::string eq1, eq2;
            embes.set_title("BUSCANDO EQUIPO");
            for(int i = 1; i < cantius; i++){
				eq1 += "<@" + std::to_string(user[ids][i][roll]) + "> ";
            }
			for(int i = 6; i < cantius + 5;i++){
                eq2 += "<@" + std::to_string(user[ids][i][roll]) + "> ";
            }

            embes.set_description("ES ESTA HARMANO UN PVP DE: " + std::to_string(cantius) + "USUARIOS POR EQUIPO");
			embes.add_field("TEAM 1: ", eq1);
            embes.add_field("TEAM 1: ", eq2);
            dpp::message msg(event.command.channel_id, embes);
            
            msg.add_component(
dpp::component().add_component(buton)
);			msg.add_component(
    dpp::component().add_component(boton)
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

                iniuser(id);
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
    bot.on_button_click([&bot](const dpp::button_click_t& event) {
    // ids
dpp::snowflake ids = event.command.guild_id;
        dpp::snowflake id = event.command.get_issuing_user().id;
        dpp::snowflake idm = event.command.message_id;

// botones XDD

if (event.custom_id == "join1") {
    event.reply(dpp::ir_deferred_update_message, dpp::message());
	bool estado = false;
    bot.message_get(event.command.message_id, event.command.channel_id,
    [&bot, event](const dpp::confirmation_callback_t& callback) {
        dpp::message msg = std::get<dpp::message>(callback.value);
		dpp::embed embes;

		for(int i = 0; i < 5; i++){
            if(rollo[ids][i] == idm){
                estado = true;
            }
        }

        if(estado){
            if(canmx[idm] < canmn[idm]){
            int cantius = canmx[ids][idm];
            user[ids][canmn[idm]][roll[idm]] = std::to_string(id);
			canmn[ids][idm]++;

        std::string eq1, eq2;
            embes.set_title("BUSCANDO EQUIPO");
            for(int i = 1; i < cantius; i++){
				eq1 += "<@" + std::to_string(user[ids][i][roll[idm]]) + "> ";
            }
			for(int i = 6; i < cantius + 5;i++){
                eq2 += "<@" + std::to_string(user[ids][i][roll[idm]]) + "> ";
            }

            embes.set_description("ES ESTA HARMANO UN PVP DE: " + std::to_string(cantius) + "USUARIOS POR EQUIPO");
			embes.add_field("TEAM 1: ", eq1);
            embes.add_field("TEAM 2: ", eq2);
            dpp::message msg(event.command.channel_id, embes);
            
            event.reply(msg);
            } else {
                event.reply("ta lleno");
            }
            
            }

            else{
                event.reply("PVP descontinuado, unete a uno mas actual");
                return;
            }
        
    });
}
if (event.custom_id == "join2") {
    event.reply(dpp::ir_deferred_update_message, dpp::message());
	bool estado = false;
    bot.message_get(event.command.message_id, event.command.channel_id,
    [&bot, event](const dpp::confirmation_callback_t& callback) {
        dpp::message msg = std::get<dpp::message>(callback.value);
		dpp::embed embes;

		for(int i = 0; i < 5; i++){
            if(rollo[ids][i] == idm){
                estado = true;
            }
        }

        if(estado){
            if(canmx[idm] + 5 < canmn2[idm]){
            int cantius = canmx[ids][idm];
            user[ids][canmn2[idm]][roll[idm]] = std::to_string(id);
			canmn2[ids][idm]++;

        std::string eq1, eq2;
            embes.set_title("BUSCANDO EQUIPO");
            for(int i = 1; i < cantius; i++){
				eq1 += "<@" + std::to_string(user[ids][i][roll[idm]]) + "> ";
            }
			for(int i = 6; i < cantius + 5;i++){
                eq2 += "<@" + std::to_string(user[ids][i][roll[idm]]) + "> ";
            }

            embes.set_description("ES ESTA HARMANO UN PVP DE: " + std::to_string(cantius) + "USUARIOS POR EQUIPO");
			embes.add_field("TEAM 1: ", eq1);
            embes.add_field("TEAM 2: ", eq2);
            dpp::message msg(event.command.channel_id, embes);
            
            event.reply(msg);
            } else {
                event.reply("ta lleno");
            }
            
            }

            else{
                event.reply("PVP descontinuado, unete a uno mas actual");
                return;
            }
        
    });
}



});

    bot.start(dpp::st_wait);
}