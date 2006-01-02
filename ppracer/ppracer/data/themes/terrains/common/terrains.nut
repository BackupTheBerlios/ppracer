
// snow marks

pptheme.load_texture("terrains/common/snow_head", "buttstart.png", false);
pptheme.load_texture("terrains/common/snow_mark", "buttprint.png", true);
pptheme.load_texture("terrains/common/snow_tail", "buttstop.png", false);

// snow Particle
pptheme.load_texture("terrains/common/snow_particle", "snowparticles.png", false);

// icy terrains

pptheme.terrain_tex({
	name = "ice",
	filename = "ice.png",
	color = [0, 0, 0],
	friction = 0.22,
	compression = 0.03,
	envmap_texture = "terrain_envmap",
	sound = "ice_sound",
    wheight = 0
});

// hard/rock terrains

pptheme.terrain_tex({
	name = "rock",
	filename = "rock.png",
	color = [128, 128, 128],
	friction = 0.9,
	compression = 0.01,
	sound = "rock_sound",
    wheight = 150
});

// snowy/soft terrains

pptheme.terrain_tex({
	name = "snow",
	filename = "snow.png",
	color = [255, 255, 255],
	friction = 0.35,
	compression = 0.11,
	sound = "snow_sound",
	wheight = 300,
	particle = "terrains/common/snow_particle",
	track_head = "terrains/common/snow_head",
	track_mark = "terrains/common/snow_mark",
	track_tail = "terrains/common/snow_tail"
});
