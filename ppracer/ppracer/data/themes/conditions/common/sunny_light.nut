

pptheme.course_light(
	0,							//light number
	true,	 					//activated
	[1, 1, 0, 0],				//position
	[1.0, 0.9, 1.0, 1],			//diffuse
    [0.0, 0.0, 0.0, 1],			//specular
	[0.45, 0.53, 0.75, 1.0]); 	//ambient

pptheme.course_light(
	1,
	true,
	[1, 1, 2, 0],
	[0, 0, 0, 0],
	[0.8, 0.8, 0.8, 1],
	[0.6, 0.5, 0.5,1]
	); 

pptheme.fog(
	true, 				//activated
	"linear", 			//mode
	0.005,				//density
	[1.0, 1.0, 1.0, 1],	//color
	10,					//start
	ppconfig["forward_clip_distance"] //end
	); 
 
pptheme.load_texture("terrain_envmap", "envmap.png", false);

pptheme.particle_color([0.85, 0.9, 1.0, 1.0]);

pptheme.load_texture("sky_front", "sunnyfront.png", false);
pptheme.load_texture("sky_right", "sunnyright.png", false);
pptheme.load_texture("sky_left", "sunnyleft.png", false);
pptheme.load_texture("sky_back", "sunnyback.png", false);
pptheme.load_texture("sky_top", "sunnytop.png", false);
pptheme.load_texture("sky_bottom", "sunnybottom.png", false);
