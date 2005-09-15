
local dir = ppogl.dir("events",true);

foreach(val in dir){
	local dir2 = ppogl.dir("events/"+val, true);
	foreach(val2 in dir2){
		temp_data = "events/"+val+"/"+val2;
		dofile("events/"+val+"/"+val2+"/description.nut");
	}
	dofile("events/"+val+"/event.nut");
}
