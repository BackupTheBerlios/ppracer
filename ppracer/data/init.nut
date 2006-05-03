
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
Textures.load({
	name = "splash_screen",
	file = "textures/splash.png",
	scalable = false
});

// ui snow particle
Textures.load({
	name = "ui_snow_particle",
	file = "textures/snowparticles.png",
	scalable = false
});

// mouse cursor
Textures.load({
	name = "mouse_cursor",
	file = "textures/mouse_cursor.png"
	scalable = false
});

// hud
Textures.load({
	name = "herring_icon",
	file = "textures/herringicon.png",
	scalable = false
});
			  
Textures.load({
	name = "gauge_outline",
	file = "textures/gaugeoutline.png",
	scalable = false
});
			  
Textures.load({
	name = "gauge_energy_mask",
	file = "textures/gaugeenergymask.png",
	scalable = false
});

Textures.load({
	name = "gauge_speed_mask",
	file = "textures/gaugespeedmask.png",
	scalable = false
});

Textures.load({
	name = "energy_mask",
	file = "textures/energymask.png",
	scalable = false
});

Textures.load({
	name = "speed_mask",
	file = "textures/speedmask.png",
	scalable = false
});

Textures.load({
	name = "mask_outline2",
	file = "textures/mask_outline2.png",
	scalable = false
});

Textures.load({
	name = "hudtl",
	file = "textures/hudtl.png",
	scalable = false
});

Textures.load({
	name = "hudtr",
	file = "textures/hudtr.png",
	scalable = false
});
			  
Textures.load({
	name = "fpsbg",
	file = "textures/fpsbg.png",
	scalable = false
});

// ui elements
Textures.load({
	name = "listbox_arrows",
	file = "textures/listbox_arrows.png",
	scalable = false
});

Textures.bind({
	name = "textarea_arrows",
	texture = "listbox_arrows"
});

Textures.load({
	name = "hscale_arrows",
	file = "textures/hscale_arrows.png",
	scalable = false
});

Textures.load({
	name = "mirror_button",
	file = "textures/mirror_button.png",
	scalable = false
});

Textures.load({
	name = "conditions_button",
	file = "textures/conditions_button.png",
	scalable = false
});

Textures.load({
	name = "snow_button",
	file = "textures/snow_button.png",
	scalable = false
});

Textures.load({
	name = "wind_button",
	file = "textures/wind_button.png",
	scalable = false
});

Textures.load({
	name = "checkmark",
	file = "textures/checkmark.png",
	scalable = false
});

// menu decorations
Textures.load({
	name = "menu_bottom_left",
	file = "textures/menu_bottom_left.png",
	scalable = false
});

Textures.load({
	name = "menu_bottom_right",
	file = "textures/menu_bottom_right.png",
	scalable = false
});

Textures.load({
	name = "menu_top_left",
	file = "textures/menu_top_left.png",
	scalable = false
});

Textures.load({
	name = "menu_top_right",
	file = "textures/menu_top_right.png",
	scalable = false
});

Textures.load({
	name = "menu_title",
	file = "textures/menu_title.png",
	scalable = false
});

// life icon
Textures.load({
	name = "life_icon",
	file = "textures/tuxlife.png",
	scalable = false
});

// no preview image
Textures.load({
	name = "no_preview",
	file = "textures/nopreview.png",
	scalable = false
});
			  

// audio
Audio.loadMusic({
	name = "start_screen",
	file = "music/start1-jt.it",
	file2 = "music/start1-jt.ogg"
});

Audio.bindMusic({
	name = "splash_screen",
	music = "start_screen"
});

Audio.bindMusic({
	name = "loading",
	music = "start_screen"
});

Audio.loadMusic({
	name = "options_screen",
	file = "music/options1-jt.it",
	file2 = "music/options1-jt.ogg"
});

Audio.bindMusic({
	name = "credits_screen",
	music = "options_screen"
});

Audio.loadMusic({
	name = "racing",
	file = "music/race1-jt.it",
	file2 = "music/race1-jt.ogg"
});

Audio.bindMusic({
	name = "intro",
	music = "racing"
});

Audio.bindMusic({
	name = "paused",
	music = "racing"
});

Audio.loadMusic({
	name = "game_over",
	file = "music/wonrace1-jt.it",
	file2 = "music/wonrace1-jt.ogg"
});
		
// sound

Audio.loadSound({
	name = "tree_hit",
	file = "sounds/tux_hit_tree1.wav"
});

Audio.loadSound({
	name = "item_collect",
	file = "sounds/fish_pickup1.wav"
});

Audio.loadSound({
	name = "item_collect2",
	file = "sounds/fish_pickup2.wav"
});

Audio.loadSound({
	name = "item_collect3",
	file = "sounds/fish_pickup3.wav"
});

Audio.loadSound({
	name = "snow_sound",
	file = "sounds/tux_on_snow1.wav",
	loop = true
});

Audio.bindSound({
	name = "flying_sound",
	sound = "snow_sound"
});

Audio.loadSound({
	name = "rock_sound",
	file = "sounds/tux_on_rock1.wav",
	loop = true
});

Audio.loadSound({
	name = "ice_sound",
	file = "sounds/tux_on_ice1.wav",
	loop = true
});

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
	Textures.load({
		name = binding,
		file = filename,
		repeatable =	repeatable
	});
}


// Init courses
local dir = ppogl.dir("courses");

temp_data <- "";

function ppcourse::register(table)
{
	table.path <- ::temp_data;	
	
	if("preview" in table){		
		Textures.load({
			name = ::temp_data,
			file = ::temp_data+"/"+table.preview,
			scalable = false
		});
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
