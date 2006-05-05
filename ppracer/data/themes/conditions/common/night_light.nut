    
pptheme.course_light({
	id = 0,
	position = [1, 1, 0, 0],
	diffuse = [0.39, 0.51, 0.88, 1],
	specular [0.0, 0.0, 0.0, 1],
	ambient = [0.0, 0.09, 0.34, 1.0]
});

pptheme.course_light({
	id = 1,
	position = [1, 1, 1, 0],
	diffuse = [0,0,0,0],
	specular = [0.8, 0.8, 0.8, 1],
	ambient = [0.1, 0.1, 0.1, 1.0]
});

pptheme.fog({
	enabled = true,
	mode = "linear",
	density = 0.005,
	color = [0.0, 0.09, 0.34, 1],
	start = 0,
	end = ppconfig["forward_clip_distance"]
}); 

pptheme.load_texture("terrain_envmap", "nightenv.png", false);

pptheme.particle_color([0.39, 0.51, 0.88, 1.0]);

pptheme.load_texture("sky_front", "nightfront.png", false);
pptheme.load_texture("sky_right", "nightright.png", false);
pptheme.load_texture("sky_left", "nightleft.png", false);
pptheme.load_texture("sky_back", "nightback.png", false);
pptheme.load_texture("sky_top", "nighttop.png", false);
pptheme.load_texture("sky_bottom", "nightbottom.png", false);
