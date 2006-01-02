    
pptheme.course_light(
	0,							//light number
	true,	 					//activated
	[1, 1, 0, 0],				//position
	[0.39, 0.51, 0.88, 1],		//diffuse
    [0.0, 0.0, 0.0, 1],			//specular
	[0.0, 0.09, 0.34, 1.0]	 	//ambient
);

pptheme.course_light(
	1,							//light number
	true,	 					//activated
	[1, 1, 1, 0],				//position
	[0,0,0,0],					//diffuse
    [0.8, 0.8, 0.8, 1],			//specular
	[0.1, 0.1, 0.1, 1.0]	 	//ambient
);

pptheme.fog(
	true, 					//activated
	"linear", 				//mode
	0.005,					//density
	[0.0, 0.09, 0.34, 1],	//color
	0,						//start
	ppconfig["forward_clip_distance"] //end
); 

pptheme.load_texture("terrain_envmap", "nightenv.png", false);

pptheme.particle_color([0.39, 0.51, 0.88, 1.0]);

pptheme.load_texture("sky_front", "nightfront.png", false);
pptheme.load_texture("sky_right", "nightright.png", false);
pptheme.load_texture("sky_left", "nightleft.png", false);
pptheme.load_texture("sky_back", "nightback.png", false);
pptheme.load_texture("sky_top", "nighttop.png", false);
pptheme.load_texture("sky_bottom", "nightbottom.png", false);
