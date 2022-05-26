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
             dpp::embed info_embed = dpp::embed().
                 set_color(dpp::colors::orange).
                 set_title("Some name").
                 set_author(event.command.usr.username + "#" + std::to_string(event.command.usr.discriminator), "https://google.com", event.command.usr.get_avatar_url()).
                 set_description("Some description here").
                 set_thumbnail(event.command.usr.get_avatar_url()).
                 add_field(
                     "Roles:",
                     "@" + std::to_string(event.command.member.roles.at(0)) 
                 ).
                 add_field(
                     "Join date",
                     dpp::utility::timestamp((event.command.member.joined_at), dpp::utility::time_format::tf_long_datetime )
                 ).
                 add_field(
                     "Registered", 
                     dpp::utility::timestamp((event.command.usr.get_creation_time()), dpp::utility::time_format::tf_long_datetime)
                 ).
                 set_footer(dpp::embed_footer().set_text( "ID: " + std::to_string(event.command.usr.id))).
                 set_timestamp(time(0));
			  event.reply(dpp::message().add_embed(info_embed));
             
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
