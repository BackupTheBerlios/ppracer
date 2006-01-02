
pptheme.hud({
	hud = 1,
	element_type = "image",
	position = [0,-1],
	texture = "hudtl",
	width = 256,
	height = 256,
	size = 256
});

pptheme.hud({
	hud = 1,
	element_type = "image",
	position = [-1, -1],
	texture = "hudtr",
	width = 128,
	height = 128,
	size = 128
});

pptheme.hud({
	hud = 1,
	element_type = "image",
	position = [-62, -10],
	texture = "herring_icon",
	width = 48,
	height = 30,
	size = 64
});

pptheme.hud({
	hud = 1,
	element_type = "herring",
	position = [-8, -10],
	font = "herring_count",
	string = "H|%03d"
});

pptheme.hud({
	hud = 1,
	element_type = "time",
	position = [20, -28],
	font = "time_value",
	string = "H|%02d:%02d:%02d"
});

pptheme.hud({
	hud = 1,
	element_type = "text",
	position = [10, -8],
	font = "time_label",
	string = "H|Time:"
});

pptheme.hud({
	hud = 1,
	element_type = "fpsimage",
	position = [0, 0],
	texture = "fpsbg",
	width = 128,
	height = 128,
	size = 128
});

pptheme.hud({
	hud = 1,
	element_type = "fsb",
	position = [6, 8],
	font = "fps",
	string = "H|FPS: %.1f"
});

pptheme.hud({
	hud = 1,
	element_type = "gauge",
	position = [-256, 12],
	width = 127,
	height = 103,
	size = 128
});

pptheme.hud({
	hud = 1,
	element_type = "text",
	position = [-32, 30],
	font = "speed_units",
	string = "H|km/h"
});

pptheme.hud({
	hud = 1,
	element_type = "speed",
	position = [-56, 52],
	font = "speed_digits",
	string = "H|%.0f"
});

pptheme.hud({
	hud = 1,
	element_type = "percentageimage",
	position = [-12, 150],
	texture = "mask_outline2",
	width = 32,
	height = 128,
	size = 128
});

pptheme.hud({
	hud = 1,
	element_type = "percentagebar",
	position = [-14, 152],
	texture = "energy_mask",
	width = 28,
	height = 124,
	angle = 180
});
