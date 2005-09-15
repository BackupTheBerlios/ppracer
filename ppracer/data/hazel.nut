

// list for storing already loaded files
local loadedfiles={};

// dofile if it's not already loaded
function require(filename):(loadedfiles)
{
	if(filename in loadedfiles){
		return;
	}else{
		::dofile(filename);
		loadedfiles[filename]=1;
	}
}
