#include <dpp/dpp.h>
#include <dpp/fmt/format.h>
#include <fstream>
#include <chrono>
#include <mpg123.h>
#include <out123.h>

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
	
	// Audio stuff, move to own file maybe
    std::vector<uint8_t> pcmdata;
    mpg123_init();
	int err = 0;
	unsigned char* buffer;
	size_t buffer_size, done;
	int channels, encoding;
	long rate;
	
	mpg123_handle *mh = mpg123_new(NULL, &err);
    mpg123_param(mh, MPG123_FORCE_RATE, 48000, 48000.0); // Force the rate to 48000 for discord

	buffer_size = mpg123_outblock(mh);
	buffer = new unsigned char[buffer_size];
	
	mpg123_open(mh, "C:/Users/blake/Desktop/Discord Bot/mp3"); // hardcoded path for test
    mpg123_getformat(mh, &rate, &channels, &encoding);

    unsigned int counter = 0;
    for (int totalBytes = 0; mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK; ) {
        for (auto i = 0; i < buffer_size; i++) {
            pcmdata.push_back(buffer[i]);
        }
        counter += buffer_size;
        totalBytes += done;
    }
    delete buffer;
    mpg123_close(mh);
    mpg123_delete(mh);
	mpg123_exit();
	
    /* Handle slash command */
    bot.on_slashcommand([&pcmdata](const dpp::slashcommand_t& event) {
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
                     "@" + std::to_string(event.command.member.roles.at(0)) // figure out how to mention roles by id
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
		 if (event.command.get_command_name() == "play") {
			 // join voice channel user is in if not already in one
			 // then play audio file
             dpp::guild * guild = dpp::find_guild(event.command.guild_id);
			 if (guild->connect_member_voice(event.command.usr.id)) {
                 dpp::voiceconn* voice = event.from->get_voice(event.command.guild_id);
                     if (voice && voice->voiceclient && voice->voiceclient->is_ready()) {
                         voice->voiceclient->send_audio_raw((uint16_t*)pcmdata.data(), pcmdata.size());
                     }
             }
             else {
                 event.reply("You must be in a voice channel to play music!"); // if user is not in a voice channel
             }
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
    dpp::slashcommand play;
	play.set_name("play")
		.set_description("Play")
		.set_application_id(bot.me.id);
	bot.global_command_create(play);
	


    /* Start the bot */
    bot.start(false);

    return 0;
}
