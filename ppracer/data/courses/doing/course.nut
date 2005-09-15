
ppcourse.init({
	// the dimension of the course
	// the first two values are the size of the course (image)
	// the last two values are the size of the valid position for the player
	dimension = [100, 800, 60, 785], 
	
	// postion for tux at startup
	start_point = [38, 3.5],
	
	// the angle of the course	
	angle = 25,
	
	// the value for scaling the elevation
	elevation_scale = 22,
	
	// the image used for the elevation
	elevation_data = "elev.png",
	
	// the image used for the elements (items/models)
	elements_data = "elements.png",
	
	// the image used for the terrain types
	terrain_data = "terrain.png",
	
	// the theme used for this course
	theme = "ppracer"
});

// executes the files elements.nut that contains additional elements (items/models)
// this is an experimental feature. 
dofile("elements.nut");
