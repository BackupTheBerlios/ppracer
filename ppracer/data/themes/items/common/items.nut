
pptheme.reset_point([255, 128, 255]);

pptheme.load_texture("items/common/fish", "herring_standard.png", false);

pptheme.item({
	name = "items/common/herring",
	element_type = "herring",
	texture = "items/common/fish",
	diameter = 1.0,
	height = 1.0,
	color = [28, 185, 204],
	above_ground = 0.2
});

pptheme.load_texture("items/common/flag1", "flag.png", false);

pptheme.item({
	name = "items/common/flag",
	element_type = "none",
	diameter = 1.0,
	height = 1.0,
    texture = "items/common/flag1",
	color = [194, 40, 40]
});

pptheme.load_texture("items/common/start", "start.png", false);

pptheme.item({
	name = "items/common/start", 
	element_type = "none",
	diameter = 9.0, 
	height = 6.0,
	texture = "items/common/start",
	color = [128, 128, 0],
    normal = [0, 0, 1],
	above_ground = -0.1
});
      
pptheme.load_texture("items/common/finish", "finish.png", false);

pptheme.item({
	name = "items/common/finish",
	element_type = "none",
	diameter = 9.0,
	height = 6.0,
	texture = "items/common/finish",
	color = [255, 255, 0],
   	normal = [0, 0, 1],
	above_ground = -0.1
});
