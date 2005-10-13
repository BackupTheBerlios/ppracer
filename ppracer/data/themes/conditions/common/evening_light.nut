
pptheme.course_light(
	0,							//light number
	true,	 					//activated
	[1, 1, 0, 0],				//position
	[0.7, 0.6, 0.5, 1.0],		//diffuse
    [0.0,0.0,0.0,0.0],			//specular
	[0.4, 0.3, 0.3, 1.0]	 	//ambient
);

pptheme.course_light(
	1,						//light number
	true,	 				//activated
	[1, 1, 2, 0],			//position
	[0.0,0.0,0.0,0],		//diffuse
    [0.9, 0.6, 0.3, 1],		//specular
	[0,0,0,0]			 	//ambient
);

pptheme.fog(
	true, 					//activated
	"linear", 				//mode
	0.005,					//density
	[0.45 0.3 0.15 1],		//color
	0,						//start
	ppconfig["forward_clip_distance"] //end
); 	
	
pptheme.load_texture("terrain_envmap", "eveningenv.png", false);

pptheme.particle_color([0.9, 0.7, 0.35, 1.0]);

pptheme.load_texture("sky_front", "eveningfront.png", false);
pptheme.load_texture("sky_right", "eveningright.png", false);
pptheme.load_texture("sky_left", "eveningleft.png", false);
pptheme.load_texture("sky_back", "eveningback.png", false);
pptheme.load_texture("sky_top", "eveningtop.png", false);
pptheme.load_texture("sky_bottom", "eveningbottom.png", false);
