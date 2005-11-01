
// add item to course
ppcourse.add_item({
	// the name of the item
	name = "items/common/start",
	
	// the position the item will be placed (in pixel)
	position = [30,15],
	
	// the height above the ground at the specified position
	above_ground = -1,		//optional
	
	// the diameter of the items
	diameter = 10,			//optional
	
	// the height of the items
	height = 6.3,			//optional
		
	// the normal of the item
	normal = [-0.2,0,1],	//optional
	
});

// add model to course
ppcourse.add_model({
	// the name of the model
	name = "models/common/shrub",
	
	// the position the model will be placed (in pixel)
	position = [30,15],
	
	// the height above the ground at the specified position
	above_ground = 3.0,		// optional
	
	// the vector to scale the model
	scale = [5.0,5.0,5.0],
	
	// the angle and vector to rotation the model
	rotation = [90,1.0,0.0,0.0]
});
	
// add reset point to course at position 50,30
ppcourse.add_reset_point([50,30]);
