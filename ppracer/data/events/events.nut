
local dir = ppogl.dir("events",true);

foreach(val in dir){
	if(!ppogl.isFile("events/"+val+"/event.nut")) continue;
	local dir2 = ppogl.dir("events/"+val, true);
	foreach(val2 in dir2){
		if(!ppogl.isFile("events/"+val+"/"+val2+"/description.nut")) continue;
		temp_data = "events/"+val+"/"+val2;
		dofile("events/"+val+"/"+val2+"/description.nut");
	}
	dofile("events/"+val+"/event.nut");
}

/*
Todo:
Implement basedir function to support courses in user dir

local userEvents = ppogl.getUserDir() + "events/";

if(ppogl.isDirectory(userEvents)){
	local dir = ppogl.dir(userEvents,true);
	foreach(val in dir){
		if(!ppogl.isFile(userEvents+val+"/event.nut")) continue;
		local dir2 = ppogl.dir(userEvents+val, true);
		foreach(val2 in dir2){
			if(!ppogl.isFile(userEvents+val+"/"+val2+"/description.nut")) continue;
			temp_data = userEvents+val+"/"+val2;
			dofile(userEvents+val+"/"+val2+"/description.nut");
		}
		dofile(userEvents+val+"/event.nut");
	}
}
*/
