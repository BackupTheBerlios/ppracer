
// mud marks
pptheme.load_texture("terrains/mud/mud_head", "mudstart.png", false);
pptheme.load_texture("terrains/mud/mud_mark", "mudprint.png", true);
pptheme.load_texture("terrains/mud/mud_tail", "mudstop.png", false);

// mud particle
pptheme.load_texture("terrains/mud/mud_particle", "mudparticles.png", false);

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
