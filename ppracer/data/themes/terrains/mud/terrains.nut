
// mud marks
Textures.load("terrains/mud/mud_head", "mudstart.png", false);
Textures.load("terrains/mud/mud_mark", "mudprint.png", true);
Textures.load("terrains/mud/mud_tail", "mudstop.png", false);

// mud particle
Textures.load("terrains/mud/mud_particle", "mudparticles.png", false);

// icy terrains

// hard/rock terrains

// snowy/soft terrains

pptheme.terrain_tex({
	name = "mud",
	filename = "mud.png",
	color = [176, 128, 80],
	friction = 0.35,
	compression = 0.11,
	particles = "terrains/mud/mud_particle",
	track_head = "terrains/mud/mud_head",
	track_mark = "terrains/mud/mud_mark",
	track_tail = "terrains/mud/mud_tail",
	sound = "snow_sound",
    wheight = 301
});
