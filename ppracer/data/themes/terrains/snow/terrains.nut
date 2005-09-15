
// snow marks
Textures.load("terrains/snow/snow_head", "buttstart.png", false);
Textures.load("terrains/snow/snow_mark", "buttprint.png", true);
Textures.load("terrains/snow/snow_tail", "buttstop.png", false);

// snow Particle
Textures.load("terrains/snow/snow_particle", "snowparticles.png", false);

// dirtsnow Particle
Textures.load("terrains/snow/dirtsnow_particle", "dirtsnowparticles.png", false);

// icy terrains

// hard/rock terrains

// snowy/soft terrains

pptheme.terrain_tex({
	name = "stsnow1",
	filename = "stsnow1.png",
	color = [176, 176, 255],
	friction = 0.35,
	compression = 0.11,
	particles = "terrains/snow/snow_particle",
	track_head = "terrains/snow/snow_head",
	track_mark = "terrains/snow/snow_mark",
	track_tail = "terrains/snow/snow_tail",
	sound = "snow_sound",
    wheight = 350
});

pptheme.terrain_tex({
	name = "stsnow2",
	filename = "stsnow2.png",
	color = [144, 144, 255],
	friction = 0.35,
	compression = 0.11,
	particles = "terrains/snow/snow_particle",
	track_head = "terrains/snow/snow_head",
	track_mark = "terrains/snow/snow_mark",
	track_tail = "terrains/snow/snow_tail",
	sound = "snow_sound",
    wheight = 351
});

pptheme.terrain_tex({
	name = "dirtsnow",
	filename = "dirtsnow.png",
	color = [192, 192, 192],
	friction = 0.35,
	compression = 0.11,
	particles = "terrains/snow/dirtsnow_particle",
	track_head = "terrains/snow/snow_head",
	track_mark = "terrains/snow/snow_mark",
	track_tail = "terrains/snow/snow_tail",
	sound = "snow_sound",
    wheight = 352
});

pptheme.terrain_tex({
	name = "dsnow",
	filename = "dsnow.png",
	color = [208, 208, 160],
	friction = 0.35,
	compression = 0.11,
	particles = "terrains/snow/dirtsnow_particle",
	track_head = "terrains/snow/snow_head",
	track_mark = "terrains/snow/snow_mark",
	track_tail = "terrains/snow/snow_tail",
	sound = "snow_sound",
    wheight = 353
});

pptheme.terrain_tex({
	name = "dsnow2",
	filename = "dsnow2.png",
	color = [208, 160, 208],
	friction = 0.35,
	compression = 0.11,
	particles = "terrains/snow/dirtsnow_particle",
	sound = "snow_sound",
    wheight = 354
});

pptheme.terrain_tex({
	name = "snowygrass",
	filename = "snowygrass.png",
	color = [176, 255, 176],
	friction = 0.5,
	compression = 0.06,
	particles ="terrains/snow/dirtsnow_particle",
	track_head = "terrains/snow/snow_head",
	track_mark = "terrains/snow/snow_mark",
	track_tail = "terrains/snow/snow_tail",
	sound = "snow_sound",
    wheight = 355
});
