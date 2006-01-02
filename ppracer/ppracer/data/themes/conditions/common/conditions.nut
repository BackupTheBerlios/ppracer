
pptheme.wind_velocity([90, 10, 10],1.0);

local condition = ppcourse.get_condition();

if(condition == "sunny")

    dofile("sunny_light.nut");

else if(condition == "cloudy")

    dofile("foggy_light.nut")

else if(condition == "night")

    dofile("night_light.nut");

else if(condition == "evening")

    dofile("evening_light.nut");
