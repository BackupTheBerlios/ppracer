
local bunny_hill = {
	name = "events/herring_run/bunny_hill",
	herrings =[23, 23, 23, 23],
	times = [37, 35, 32, 30],
	scores = [0, 0, 0, 0],
};

local twisty_slope = {
	name = "events/herring_run/twisty_slope",
	herrings = [24, 24, 24, 24], 
	times = [43, 40, 34, 31.5],
	scores = [0, 0, 0, 0],
	condition = "night"
};
   
local bumpy_ride = {
	name = "events/herring_run/bumpy_ride",
	herrings = [17, 17, 17, 17],
	times = [36, 34, 32, 30],
	scores = [0, 0, 0, 0],
    condition = "cloudy"                     
};

local canadian_cup = {
	name = "Canadian Cup",
	courses = [bunny_hill, twisty_slope, bumpy_ride],
	names = {
		de_DE = "Canadian Cup"
	}
};


local ice_labyrinth = {
	name = "events/herring_run/ice_labyrinth",
	herrings = [18, 18, 18, 18],
	times = [107, 106, 105, 104],
	scores = [0, 0, 0, 0],
	condition = "evening"
};

local mount_herring = {
	name = "events/herring_run/mount_herring",
	herrings = [95, 95, 95, 95],
	times = [240, 235, 230, 225],
	scores = [0, 0, 0, 0]
};

local hamburger_hill = {
	name ="events/herring_run/hamburger_hill",
	herrings = [80, 80, 80, 80],
	times = [220, 215, 210, 205],
	scores = [0, 0, 0, 0],
	condition = "cloudy"
};

local deadman = {
	name = "events/herring_run/deadman",
	herrings = [100, 100, 100, 100],
	times = [125, 120, 115, 110],
	scores = [0, 0, 0, 0]
};

local mount_satan = {
	name = "events/herring_run/mount_satan",
	herrings = [69, 69, 69, 69],
    times = [135, 130, 125, 120],
	scores = [0, 0, 0, 0],
	condition = "night"
};

local mountain_man = {
	name = "Mountain Man",
	courses = [ ice_labyrinth, mount_herring, hamburger_hill, deadman, mount_satan ],
};



local snow_valley = {
	name ="events/herring_run/snow_valley",
	herrings = [42, 42, 42, 42],
	times = [45, 43, 41, 39],
	scores = [0, 0, 0, 0],
	condition = "evening"
};

local hazzard_valley = {
	name = "events/herring_run/hazzard_valley",
    herrings = [68, 68, 68, 68],
	times = [105, 100, 95, 90],
	scores = [0, 0, 0, 0]
};

local skull_mountain = {
	name ="events/herring_run/skull_mountain",
	herrings = [25, 25, 25, 25],
	times = [116, 114, 112, 110],
	scores = [0, 0, 0, 0]
};

local the_narrow_way = {
	name = "events/herring_run/the_narrow_way",
	herrings = [23, 23, 23, 23],
	times = [68, 67, 66, 65],
	scores = [0, 0, 0, 0],
	condition = "cloudy"
};

local valley_cup = {
	name = "Valley Cup",
	courses = [ snow_valley, hazzard_valley, skull_mountain, the_narrow_way ],
};


local penguins_cant_fly = {
	name = "events/herring_run/penguins_cant_fly",
	herrings = [8, 8, 8, 8],
	times = [88, 86, 84, 82],
	scores = [0, 0, 0, 0]
};

local ski_jump = {
	name = "events/herring_run/ski_jump",
	herrings = [14, 14, 14, 14],
	times = [26, 25, 25, 25],
	scores = [0, 0, 0, 0],
	condition = "cloudy"
};
	
local toboggan_run = {
	name = "events/herring_run/tux-toboggan_run",
	herrings = [140, 145, 150, 155],
	times = [275, 265, 265, 265],
	scores = [0, 0, 0, 0]
};

local slalom = {
	name = "events/herring_run/slalom",
	herrings = [65, 65, 65, 65],
	times = [80, 79, 78, 77], 
	scores = [0, 0, 0, 0],
	condition = "evening"
};

local winter_sportsman = {
	name = "Tux the winter sportsman",
	courses = [ penguins_cant_fly, ski_jump, toboggan_run, slalom]
};

  
local high_road = {
	name = "events/herring_run/high_road",
	herrings = [15, 15, 15, 15],
	times = [60, 59, 58, 57],
	scores = [0, 0, 0, 0],
	conditions = "night"
};

local keep_it_up = {
	name = "events/herring_run/keep_it_up",
	herrings = [25, 25, 25, 25],
	times = [60, 59, 58, 47],
	scores = [0, 0, 0, 0],
	condition = "night"
};

local ive_got_a_woody = {
	name = "events/herring_run/ive_got_a_woody",
	herrings =[13, 13, 13, 13],
	times = [40, 39, 38, 37],
	scores = [0, 0, 0, 0],
	condition = "night"
};

local highway_to_hell = {
	name = "Highway To Hell",
	courses = [ high_road, keep_it_up, ive_got_a_woody ]
};

ppracer.register_event({
	name = "Herring Run",
	cups = [ canadian_cup, mountain_man, valley_cup, winter_sportsman, highway_to_hell],
	names = {
		de_DE = "Herring Run"
	}
});
