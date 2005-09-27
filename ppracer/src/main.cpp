/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
 *
 * Copyright (C) 1999-2001 Jasmin F. Patry
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "course_load.h"
#include "course_render.h"
#include "tux.h"
#include "hier_cb.h"
#include "phys_sim.h"
#include "part_sys.h"
#include "keyframe.h"
#include "gl_util.h"
#include "loop.h"
#include "render_util.h"

#include "bench.h"

#include "fog.h"
#include "lights.h"

#include "course_mgr.h"

#include "game_mgr.h"
#include "joystick.h"
#include "os_util.h"
#include "stuff.h"
#include "winsys.h"
#include "ui_snow.h"

#include "ppogl/translation.h"
#include "ppogl/ui/uitheme.h"
#include "ppogl/audio.h"
#include "ppogl/config.h"
#include "ppogl/base/os.h"

#include <iostream>

/// global instance of the script interpreter
ppogl::Script script;

#define WINDOW_TITLE "PlanetPenguin Racer " VERSION

void
showHelpMessage()
{
	std::cout << 
		"Usage: ppracer [OPTION...]\n"
		"\n"
		"  -c <config-file>         use custom config file\n"
		"  -d <data-directory>      use custom data directory\n"
		"\n"
		"Benchmark/autoplay\n"
		"  -a                       automatic playing mode\n"
		"  -f <course-name>         course to play\n"
		"  -rc <condition>          set condition for the course\n"
		"  -p <x> <y>               start at position x,y\n"
		"  -m <number-of-frames>    number of frames after which the playback gets aborted\n"
		"  -t <milliseconds>        timestep for each frame\n"
		"\n"
		"Experimental\n"
		"  --multiplayer            experimental multiplayer mode\n"
		<< std::endl;	
}

std::string cfile;
std::string data_dir;

static void
getopts( int argc, char *argv[] )
{
	for(int i=0; i<argc; i++){
		if( !strcmp(argv[i],"--help") ){
			showHelpMessage();
			exit(0);
		}else if( !strcmp(argv[i],"-c") ){
			i++;
			cfile = argv[i];
		}else if( !strcmp(argv[i],"-d") ){
			i++;
			data_dir = argv[i];
		}else if( !strcmp( argv[i],"-f") ){
			i++;
			if(argv[i] != NULL){			
				Benchmark::setCourse(argv[i]);
			}
		}else if( !strcmp( argv[i],"-m") ){
			i++;
			if(argv[i] != NULL){
				Benchmark::setMaxFrames(atoi(argv[i]));
			}
		}else if( !strcmp( argv[i],"-a") ){
			Benchmark::setMode(Benchmark::AUTO);
		}else if( !strcmp( argv[i],"-p") ){
			i++;
			ppogl::Vec2d pos;
			if(argv[i] != NULL){
				pos.x() = atoi(argv[i]);
				i++;
				if(argv[i] != NULL){
					pos.y() = atoi(argv[i]) * (-1);
					Benchmark::setPosition(pos);
					Benchmark::setMode(Benchmark::PAUSED);
				}			
			}		
		}else if( !strcmp( argv[i],"-t") ){
			i++;
			if(argv[i] != NULL){
				Benchmark::setTimeStep(atof(argv[i]));
			}
		}else if( !strcmp( argv[i],"-rc") ){
			i++;
			if(argv[i] != NULL){
				Benchmark::setRaceCondition(atoi(argv[i]));
			}
		}else if( !strcmp( argv[i],"--multiplayer") ){
			GameMode::exp_multiscreen=GameMode::MULTISCREEN_HORIZ;
		}
	}	
}

// this function is called on exit
void
cleanup()
{
    write_config_file();
	winsys_shutdown();
}

static void
init_log()
{
    ppogl::Log::Instance()->setFilename("diagnostic_log.txt");

	std::ostream& stream =
			ppogl::Log::Instance()->getStream();
	
	stream << "PlanetPenguin Racer Diagnostic Log\n" << std::endl;
		
	time_t t = time( NULL );
	stream << "Generated:       " << asctime( gmtime( &t ) ); 
	stream << "PlanetPenguin Racer Version: " << VERSION << std::endl;
	stream << "OS:              ";
	
	char buff[BUFF_LEN];
	
    if ( get_os_version( buff, sizeof( buff ) ) == 0 ) {
		stream << buff << "\n" << std::endl;
    } else {
		stream << "Could not determine!\n" << std::endl;
    }  
}

int
main(int argc, char *argv[])
{
    try{ // used to catch all unhandled Errors and Assertions		
	
		// print copyright notice 
    	std::cout << "PlanetPenguin Racer " VERSION " -- http://racer.planetpenguin.de" << std::endl;
	    std::cout << "(c) 2004-2005 PlanetPenguin Racer team" << std::endl;
	    std::cout << "(c) 1999-2001 Jasmin F. Patry" << std::endl;
	    std::cout << "<jfpatry@sunspirestudios.com>" << std::endl;
	    std::cout << "PlanetPenguin Racer comes with ABSOLUTELY NO WARRANTY."<< std::endl;
	    std::cout << "This is free software,\nand you are welcome to redistribute it under certain conditions."<< std::endl;
	    std::cout << "See http://www.gnu.org/copyleft/gpl.html for details.\n" << std::endl;

		// set current directory to be used as base
		ppogl::os::setBaseDir();
						
		// parse comandline options
		getopts(argc,argv);
		
    	// seed the random number generator
    	srand( time(NULL) );
	
	if(GameMode::exp_multiscreen==GameMode::MULTISCREEN_NONE){
		GameMgr::getInstance().numPlayers = 1;
	}else{
		GameMgr::getInstance().numPlayers = 2;
	}
	
	ppogl::Config::getInstance().init(&script,"ppconfig");
	
	bool create_cfile=true;
	
	if(cfile.empty()){
		cfile=get_config_file_name();
	}
		
	if(ppogl::os::isFile(cfile)){
		script.doFile(cfile);
		// check whether the user set the data directory per commandline option
		if(data_dir.empty()){
			data_dir = PPConfig.getString("data_dir");
		}
		PP_MESSAGE("Load config file");
		create_cfile=false;
	}else if(data_dir.empty()){
		data_dir = DATA_DIR;
	}

	if(ppogl::os::isDirectory(data_dir)==false){
		PP_ERROR("Unable to find data dir: " << data_dir <<"\n\tUse \"ppracer -d YOUR_DATA_DIRECTORY\"");
	}
	
	if(ppogl::os::isFile(data_dir+"/config.nut")==false){
		PP_ERROR("Unable to find needed file config.nut in the data dir: " << data_dir <<"\n\tUse \"ppracer -d YOUR_DATA_DIRECTORY\"");
	}
		
	script.doFile(data_dir+"/config.nut");

	PPConfig.setString("data_dir",data_dir);

	if(create_cfile){
		write_config_file();
	}
	
    /* Setup diagnostic log if requested */
    if(PPConfig.getBool("write_diagnostic_log")){
		init_log();
	}

    /* 
     * Initialize rendering context, create window
     */
    winsys_init(WINDOW_TITLE, WINDOW_TITLE);
	
    /* Ingore key-repeat messages */
    winsys_enable_key_repeat(false);

    /* 
     * Initial OpenGL settings 
     */
    gl::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    init_opengl_extensions();

    /* Print OpenGL debugging information if requested */
    /*if ( debug_mode_is_active( DEBUG_GL_INFO ) ) {
		PP_LOG( DEBUG_GL_INFO, "OpenGL information:" );
		print_gl_info();
    }*/

    /* 
     * Load the game data and initialize game state
     */

    register_key_frame_callbacks();
	Course::registerCallbacks();
	FogPlane::registerCallbacks();

	register_course_light_callbacks();
	
    register_particle_callbacks();
    register_course_manager_callbacks();
	register_hier_callbacks();
    register_tux_callbacks();
	
	// init audio
	if(PPConfig.getBool("disable_audio")==false){
		bool stereo = PPConfig.getBool("audio_stereo");
		ppogl::AudioMgr::Format format;
		int freq;
		
		switch(PPConfig.getInt("audio_format_mode")){
			case 0:
				format = ppogl::AudioMgr::FORMAT_8;
				break;
			case 1:
				format = ppogl::AudioMgr::FORMAT_16;
				break;
			default:
				format = ppogl::AudioMgr::FORMAT_8;
		}

		switch(PPConfig.getInt("audio_freq_mode")){
			case 0:
				freq = ppogl::AudioMgr::FQ_11;
				break;
			case 1:
				freq = ppogl::AudioMgr::FQ_22;
				break;
			case 2:
				freq = ppogl::AudioMgr::FQ_44;
				break;
			default:
				freq = ppogl::AudioMgr::FQ_11;
		}
		ppogl::AudioMgr::getInstance().init(freq,format,stereo);
			
		if(PPConfig.getBool("sound_enabled")==false){
			ppogl::AudioMgr::getInstance().enableSound(false);
		}
		if(PPConfig.getBool("music_enabled")==false){
			ppogl::AudioMgr::getInstance().enableMusic(false);
		}
	}
		
	// Setup translations
	script.doFile(data_dir+"/translations/languages.nut");	
	script.doFile(data_dir+"/translations/"+PPConfig.getString("ui_language")+".nut");		

	script.doFile(data_dir+"/init.nut");	

	// load "cached" configuration values
	GameConfig::update();

    init_joystick();
	init_ui_snow();

    // Placeholder name until we give players way to enter name
    players[0].name = "tux";
	players[0].loadData();
	players[0].num = 0;
	
	if(GameMode::exp_multiscreen!=GameMode::MULTISCREEN_NONE){
	    players[1].name = "tux2";
		players[1].loadData();
		players[1].num=1;
	}

    GameMode::mode = GameMode::NO_MODE;
	
	if(Benchmark::getMode()==Benchmark::NONE){
		GameMode::setMode(GameMode::SPLASH);

	}else{
		GameMode::setMode(GameMode::BENCHMARK);
	}
	
    GameMgr::getInstance().difficulty = DIFFICULTY_LEVEL_NORMAL;
	
	winsys_show_cursor( false );

	/* 
     * ...and off we go!
     */

    winsys_process_events();
	
    return 0;
	
	}catch(ppogl::Error &e){
		/*
		 * It's possible that an exception is thrown outside of main.
		 * In most cases this is a problem with the object 
		 * destruction during program termination.
		 * If we see the abort message (see below) we known
		 * that the exception is thrown within main().
		 */		
		std::cerr << "Aborting main function :(" << std::endl;	
		abort();
	}
}
