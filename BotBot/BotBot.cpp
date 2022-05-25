#include <dpp/dpp.h>
#include <dpp/fmt/format.h>
#include <fstream>
#include <chrono>

int main()
{
	// Load the token from the file token.txt
    std::ifstream token;
    token.open("token.txt");
    std::string BOT_TOKEN;
    token >> BOT_TOKEN;
	token.close();
	
    /* Create bot cluster */
    dpp::cluster bot(BOT_TOKEN);

    /* Output simple log messages to stdout */
    bot.on_log(dpp::utility::cout_logger());

    /* Handle slash command */
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
         if (event.command.get_command_name() == "beep") {
            event.reply("boop!");
         }
		 if (event.command.get_command_name() == "info") {
             event.reply(fmt::format("Name: {}\nID: {}\nHas Nitro: {}\nJoined on: {}\n",
                 event.command.usr.username, 
                 event.command.usr.id, 
                 (event.command.usr.has_nitro_classic() || event.command.usr.has_nitro_full() ), 
                 event.command.member.joined_at)); // figure out how to convert unix time to human readable time later
             ));
		 }
	});
		

    dpp::slashcommand info;
    info.set_name("info")
        .set_description("Get information about yourself")
        .set_application_id(bot.me.id);
    //bot.global_command_create(beep);
	dpp::slashcommand beep;
	beep.set_name("beep")
		.set_description("robot")
		.set_application_id(bot.me.id);
    //bot.global_command_create(info);

    /* Register slash command here in on_ready 
    bot.on_ready([&](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
        }
    }); */

    /* Start the bot */
    bot.start(false);

    return 0;
}
