
ppcourse.init({
	dimension = [100, 800, 60, 785],
	start_point = [38, 3.5],
	angle = 25,
	elevation_scale = 22,
	elevation_data = "elev.png",
	elements_data = "elements.png",
	terrain_data = "terrain.png",
	theme = "ppracer"
});

dofile("elements.nut");
