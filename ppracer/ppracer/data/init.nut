
data_dir <- ppogl.cwd();

dofile("hazel.nut");

// fonts
Fonts.load("button_label",
		   "fonts/FreeSansBoldOblique.ttf",
		   30);

Fonts.bind("button_label_hilit",
		   "button_label",
		   [1.00, 0.89, 0.01]);

Fonts.bind("button_label_insensitive",
		   "button_label",
		   [1.0, 1.0, 1.0, 0.5]);

Fonts.bind("loading",
           "button_label_hilit",
		   [1.00, 0.89, 0.0]);

Fonts.bind("button_label_disabled",
		   "button_label",
		   [1.0, 1.0, 1.0]);

Fonts.bind("menu_label",
		   "button_label");

// listbox
Fonts.load("listbox_item",
		   "fonts/FreeSansBoldOblique.ttf",
		   22,
		   [1.00, 0.89, 0.01]);

Fonts.bind("listbox_item_insensitive",
		   "listbox_item",
		   [1.00, 0.89, 0.01, 0.5]);

// race requirements
Fonts.load("race_requirements",
		   "fonts/FreeSansBoldOblique.ttf",
		   13);

Fonts.bind("race_requirements_label",
		   "race_requirements",
		   [1.00, 0.95, 0.01]);

// heading
Fonts.load("heading",
		   "fonts/FreeSansBoldOblique.ttf",
		   40,
		   [1.00, 0.89, 0.01]);

Fonts.bind("paused",
		   "heading");

Fonts.bind("race_over",
		   "heading");

// several stuff
Fonts.load("textarea",
		   "fonts/FreeSansBoldOblique.ttf",
		   16);
		   
Fonts.bind("race_description",
		   "textarea");
		   
Fonts.bind("cup_status",
		   "race_description",
		   [1.00, 0.89, 0.01]);

Fonts.load("race_stats",
		   "fonts/FreeSansBoldOblique.ttf",
		   25,
		   [1.00, 0.89, 0.01]);

Fonts.load("event_and_cup_label",
		   "fonts/FreeSansBoldOblique.ttf",
		   22);

// credits
Fonts.load("credits_text",
		   "fonts/FreeSansBoldOblique.ttf",
		   20);
		   
Fonts.load("credits_text_small",
		   "fonts/FreeSansBoldOblique.ttf",
		   14);
		   
Fonts.bind("credits_h1",
		   "button_label_hilit");
		   
Fonts.bind("credits_h2",
		   "credits_text",
		   [1.00, 0.89, 0.01]);


// we don't bind this because hud fonts need to be fast
Fonts.load("time_label",
		   "fonts/FreeSansBoldOblique.ttf",
		   20,
		   [1.0, 0.89, 0.01]);
		   
Fonts.load("herring_count",
		   "fonts/FreeSansBoldOblique.ttf",
		   30,
		   [1.0, 0.89, 0.01]);
		   
Fonts.load("fps",
		   "fonts/FreeSansBoldOblique.ttf",
		   20,
		   [1.0, 0.89, 0.01]);
		   
Fonts.load("time_value",
		   "fonts/FreeSansBoldOblique.ttf",
		   30,
		   [1.0, 0.89, 0.01]);
		   
Fonts.load("speed_digits",
		   "fonts/FreeSansBoldOblique.ttf",
		   35,
		   [1.0, 0.89, 0.01]);
		   
Fonts.load("speed_units",
		   "fonts/FreeSansBoldOblique.ttf",
		   20,
		   [1.0, 0.89, 0.01]);

// textures

// splash screen
Textures.load("splash_screen",
			  "textures/splash.png");

// ui snow particle
Textures.load("ui_snow_particle",
			  "textures/snowparticles.png");

// mouse cursor
Textures.load("mouse_cursor",
			  "textures/mouse_cursor.png");

// hud
Textures.load("herring_icon",
			  "textures/herringicon.png");
			  
Textures.load("gauge_outline",
			  "textures/gaugeoutline.png");
			  
Textures.load("gauge_energy_mask",
			  "textures/gaugeenergymask.png");

Textures.load("gauge_speed_mask",
			  "textures/gaugespeedmask.png");

Textures.load("energy_mask",
			  "textures/energymask.png");

Textures.load("speed_mask",
			  "textures/speedmask.png");

Textures.load("mask_outline2",
			  "textures/mask_outline2.png");

Textures.load("hudtl",
			  "textures/hudtl.png");

Textures.load("hudtr",
			  "textures/hudtr.png");
			  
Textures.load("fpsbg",
			  "textures/fpsbg.png");			  

// ui elements
Textures.load("listbox_arrows",
			  "textures/listbox_arrows.png");
Textures.bind("textarea_arrows","listbox_arrows");

Textures.load("hscale_arrows",
			  "textures/hscale_arrows.png");

Textures.load("mirror_button",
			  "textures/mirror_button.png");

Textures.load("conditions_button",
			  "textures/conditions_button.png");

Textures.load("snow_button",
			  "textures/snow_button.png");

Textures.load("wind_button",
			  "textures/wind_button.png");

Textures.load("checkmark",
			  "textures/checkmark.png");

// menu decorations
Textures.load("menu_bottom_left",
			  "textures/menu_bottom_left.png");

Textures.load("menu_bottom_right",
			  "textures/menu_bottom_right.png");

Textures.load("menu_top_left",
			  "textures/menu_top_left.png");

Textures.load("menu_top_right",
			  "textures/menu_top_right.png");

Textures.load("menu_title",
			  "textures/menu_title.png");

// life icon
Textures.load("life_icon",
			  "textures/tuxlife.png");
			  
// no preview image
Textures.load("no_preview",
			  "textures/nopreview.png");
			  

// audio
Audio.loadMusic("start_screen",
				"music/start1-jt.it");
Audio.bindMusic("splash_screen","start_screen");
Audio.bindMusic("loading","start_screen");

Audio.loadMusic("options_screen",
				"music/options1-jt.it");	
Audio.bindMusic("credits_screen", "options_screen");

Audio.loadMusic("racing",
				"music/race1-jt.it");
Audio.bindMusic("intro", "racing");
Audio.bindMusic("racing", "racing");
Audio.bindMusic("paused", "racing");

Audio.loadMusic("game_over",
				"music/wonrace1-jt.it");
		
// sound

Audio.loadSound("tree_hit",
				"sounds/tux_hit_tree1.wav");

Audio.loadSound("item_collect",
				"sounds/fish_pickup1.wav");
Audio.loadSound("item_collect2",
				"sounds/fish_pickup2.wav");
Audio.loadSound("item_collect3",
				"sounds/fish_pickup3.wav");

Audio.loadSound("snow_sound",
				"sounds/tux_on_snow1.wav");
Audio.bindSound("flying_sound", "snow_sound");

Audio.loadSound("rock_sound",
				"sounds/tux_on_rock1.wav");

Audio.loadSound("ice_sound",
				"sounds/tux_on_ice1.wav");

dofile("models/tux/model.nut");
dofile("models/tux2/model.nut");

// temporary terrain file
ppcourse.terrain_file <- "";
ppcourse.elements_file <- "";

function ppcourse::terrain(terrain)
{
    ppcourse.terrain_file = terrain;
}

function ppcourse::elements(elements)
{
    ppcourse.elements_file = elements;
}

// init specified theme
function ppcourse::theme_init(theme)
{
	::dofile(::data_dir+"/themes/" + theme + ".nut");
	ppcourse.load_elements(ppcourse.elements_file);
	ppcourse.load_terrain(ppcourse.terrain_file);
	::dofile(::data_dir+"/models/tux/model_walk.nut");
	::dofile(::data_dir+"/models/tux2/model_walk.nut");
}

// load theme pack
function pptheme::pack_load(pack_type, name)
{
	local cwd = ppogl.cwd();
		
	ppogl.cd(::data_dir+ "/themes/" + pack_type + "/" + name + "/");
	::dofile(pack_type + ".nut");
	
	ppogl.cd(cwd);
}

// load texture in theme context
function pptheme::load_texture(binding, filename, repeatable)
{
	//todo:	add binding to global table and unload/unbind 
	// 		the texture if a different theme is loaded
	Textures.load(binding, filename, repeatable);
}


// Init courses
local dir = ppogl.dir("courses");

temp_data <- "";

function ppcourse::register(table)
{
	table.path <- ::temp_data;	
	
	if("preview" in table){		
		Textures.load(::temp_data,::temp_data+"/"+table.preview);
	}

	ppracer.register_course(table);	
}

foreach(val in dir){
	temp_data = "courses/"+val;
	if(ppogl.isFile("courses/"+val+"/description.nut")){
		dofile("courses/"+val+"/description.nut");
	}
}

local userCourses = ppogl.getUserDir() + "courses/";

if(ppogl.isDirectory(userCourses)){
	local dir = ppogl.dir(userCourses);
	foreach(val in dir){
		temp_data = userCourses+val;
		if(ppogl.isFile(userCourses+val+"/description.nut")){
			dofile(userCourses+val+"/description.nut");
		}
	}
}

dofile("events/events.nut");

function ppcourse::init(info)
{
	ppcourse.course_dim(info.dimension);
	ppcourse.start_pt(info.start_point);
	ppcourse.angle(info.angle);
	
	if("base_height_value" in info){
		ppcourse.base_height_value(info.base_height_value);
	}
	
	ppcourse.elev_scale(info.elevation_scale);
	ppcourse.elev(info.elevation_data);

	::dofile(::data_dir+"/themes/" + info.theme + ".nut");
	
	ppcourse.load_elements(info.elements_data);
	ppcourse.load_terrain(info.terrain_data);
		
	::dofile(::data_dir+"/models/tux/model_walk.nut");
}
