
Textures.load("items/herrings/fish2", "herring_red.png", false);

pptheme.item({
	name = "herringred",
	element_type = "herring",
	diameter = 1.0,
	height = 1.0,
	texture = "items/herrings/fish2",
	color = [0, 255, 0],
	above_ground = 0.2
});

Textures.load("items/herrings/fish3", "herring_green.png", false);

pptheme.item({
	name = "herringgreen",
	element_type = "herring",
	diameter = 1.0,
	height = 1.0,
    texture = "items/herrings/fish3",
	color = [255, 170, 0],
	above_ground = 0.2
});

Textures.load("items/herrings/star", "star.png", false);

pptheme.item({
	name = "star", 
	element_type = "herring", 
	diameter = 1.0,
	height = 1.0,
    texture ="items/herrings/star", 
	color = [188, 79, 105], 
	above_ground = 0.2,
	score = 5
});

Textures.load("items/herrings/deadfish", "herring_dead.png", false);

pptheme.item({
	name = "deadfish",
	element_type ="herring", 
	diameter = 1.0,
	height = 2.0,
    texture = "items/herrings/deadfish",
	color = [192, 192, 0],
	above_ground = 0.2,
	score = -10
});
