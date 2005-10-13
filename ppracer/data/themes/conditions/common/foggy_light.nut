
pptheme.course_light(
	0,							//light number
	true,	 					//activated
	[1, 1, 1, 0],				//position
	[0.45, 0.43, 0.47, 1.0],	//diffuse
    [0.3, 0.3, 0.3, 1],			//specular
	[0.39, 0.4, 0.44, 1.0]      //ambient
);

pptheme.course_light(
	1,							//light number
	true,	 					//activated
	[1, 1, 2, 0],				//position
	[0.0, 0.0, 0.0, 0.0],		//diffuse
    [0.7, 0.7, 0.7, 1.0],		//specular
    [0.3, 0.3, 0.3, 1.0]       	//ambient
);

local fog_dist = 40;
if(fog_dist > ppconfig["forward_clip_distance"]){
	fog_dist = ppconfig["forward_clip_distance"];

}

pptheme.fog(
	true, 					//activated
	"linear", 				//mode
	0.05,					//density
	[0.58, 0.59, 0.65, 1.0],//color
	0,						//start
	fog_dist 				//end
); 

pptheme.particle_color([0.36, 0.41, 0.49, 1.0]);

pptheme.load_texture("terrain_envmap", "envmap.png", false);

pptheme.load_texture("sky_front", "cloudyfront.png", false);
pptheme.load_texture("sky_right", "cloudyright.png", false);
pptheme.load_texture("sky_left", "cloudyleft.png", false);
pptheme.load_texture("sky_back", "cloudyback.png", false);
pptheme.load_texture("sky_top", "cloudytop.png", false);
pptheme.load_texture("sky_bottom", "cloudybottom.png", false);
