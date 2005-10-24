/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
 *
 * Copyright (C) 1999-2001 Jasmin F. Patry
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "course_load.h"
#include "course_render.h"
#include "game_mgr.h"

#include "ppogl/base/glwrappers.h"
#include "ppogl/base/os.h"
#include "ppogl/images/image.h"
#include "ppogl/textures.h"

#include "phys_sim.h"
#include "keyframe.h"
#include "gl_util.h"
#include "lights.h"
#include "fog.h"
#include "part_sys.h"

#include "hud.h"

#include "mirror_course.h"

#include "player.h"

#include "track_marks.h"

#include "ppogl/base/defs.h"

#include "ppogl/translation.h"

#include "elements.h"

#include <iostream>

#define MIN_ANGLE 5
#define MAX_ANGLE 80

TerrainTex terrain_texture[NUM_TERRAIN_TYPES];

unsigned int num_terrains=0;

static bool course_loaded = false;

float Course::angle;

float* Course::elevation;
int* Course::terrain;

ppogl::Vec2d Course::dimension;
ppogl::Vec2d Course::playDimension;

static float elev_scale;
static int nx, ny;
static ppogl::Vec2d start_pt;

static std::string courseAuthor;
static std::string courseName;

static int base_height_value;

static std::map< std::string,ppogl::RefPtr<ModelType> > modelTypes;
static std::map<std::string,ppogl::RefPtr<ItemType> > itemTypes;

///all terrains that are used in the current course.
///the list is sorted with respect to the terrain wheights
std::list<int> usedTerrains;

///interleaved vertex, normal, and color data
GLubyte* Course::vncArray = NULL;

///last loaded course
std::string Course::sm_loadedCourse;

///last loaded condition of the course
CourseData::Condition Course::sm_loadedCondition=static_cast<CourseData::Condition>(-1);

const ppogl::Vec2d&
Course::getStartPt()
{
	return start_pt;
}

void
Course::setStartPt(const ppogl::Vec2d& p)
{
	start_pt = p;
}

/*! 
  Returns the base (minimum) height of the terrain at \c distance
  \pre     A course has been loaded
  \arg \c  distance the (non-negative) distance down the course

  \return  Minimum height (y-coord) of terrain
  \author  jfpatry
  \date    Created:  2000-08-30
  \date    Modified: 2000-08-30
*/
float
Course::getTerrainBaseHeight( float distance )
{
    PP_REQUIRE( distance > -EPS, "distance should be positive" );
	
	float slope = tan(ANGLES_TO_RADIANS(angle));
    float base_height;
    

    /* This will need to be fixed once we add variably-sloped terrain */
    base_height = -slope * distance - 
	base_height_value / 255.0 * elev_scale;
    return base_height;
}

/*! 
  Returns the maximum height of the terrain at \c distance
  \pre     A course has been loaded
  \arg \c  distance the (non-negative) distance down the course

  \return  Maximum height (y-coord) of terrain
  \author  jfpatry
  \date    Created:  2000-08-30
  \date    Modified: 2000-08-30
*/
float
Course::getTerrainMaxHeight( float distance )
{
    return getTerrainBaseHeight( distance ) + elev_scale;
}

void
Course::getDivisions( int *x, int *y )
{
    *x = nx;
    *y = ny;
} 

static void
reset_course()
{
	num_terrains = 0;
	Course::angle  = 20;
	Course::dimension = ppogl::Vec2d(50,130);
	Course::playDimension = ppogl::Vec2d(50,130);

    nx = ny = -1;
    start_pt.x() = 0;
    start_pt.y() = 0;
    base_height_value = 127;
	
    reset_course_mirroring();

    reset_lights();
    fogPlane.reset();
    reset_particles();

	courseAuthor.erase();
	courseName.erase();

	if ( course_loaded == false ) return;

	courseRenderer.resetQuadtree();

    delete[] Course::elevation;
	Course::elevation=NULL;
	
    delete[] Course::terrain;
	Course::terrain=NULL;

    delete[] Course::vncArray;
	Course::vncArray=NULL;

	modelLocs.clear();
	modelTypes.clear();
		
	itemLocs.clear();
	itemTypes.clear();
	
	resetLocs.clear();

    course_loaded = false;

    keyFrames[0].reset();
	keyFrames[1].reset();
} 

void
Course::fillGLArrays()
{
    int x,y;
    ppogl::Vec3d *normals = courseRenderer.getNormals();
    ppogl::Vec3d nml;
    int idx;

    gl::DisableClientState(GL_VERTEX_ARRAY);
    gl::DisableClientState(GL_NORMAL_ARRAY);
    gl::DisableClientState(GL_COLOR_ARRAY);

    // Align vertices and normals on 16-byte intervals (Q3A does this)
    int arraySize = STRIDE_GL_ARRAY * nx * ny;
	Course::vncArray = new GLubyte[arraySize];
	PP_CHECK_ALLOC(vncArray);
	
    for (x=0; x<nx; x++) {
	for (y=0; y<ny; y++) {
	    idx = STRIDE_GL_ARRAY*(y*nx+x);
	   
#define floatval(i) (*reinterpret_cast<GLfloat*>(Course::vncArray+idx+(i)*sizeof(GLfloat)))

	    floatval(0) = GLfloat(x) / (nx-1.) * dimension.x();
	    floatval(1) = ELEV(x,y);
	    floatval(2) = -GLfloat(y)/ (ny-1.) * dimension.y();

	    nml = normals[ x + y * nx ];
	    floatval(4) = nml.x();
	    floatval(5) = nml.y();
	    floatval(6) = nml.z();
	    floatval(7) = 1.0f;
	   
#undef floatval
#define byteval(i) (*reinterpret_cast<GLubyte*>(Course::vncArray+idx+8*sizeof(GLfloat) +\
    i*sizeof(GLubyte)))

	    byteval(0) = 255;
	    byteval(1) = 255;
	    byteval(2) = 255;
	    byteval(3) = 255;

#undef byteval

	}
    }

		gl::EnableClientState(GL_VERTEX_ARRAY);
    	gl::VertexPointer( 3, GL_FLOAT, STRIDE_GL_ARRAY, Course::vncArray );

    	gl::EnableClientState(GL_NORMAL_ARRAY);
    	gl::NormalPointer( GL_FLOAT, STRIDE_GL_ARRAY, 
		     Course::vncArray + 4*sizeof(GLfloat) );

    	gl::EnableClientState(GL_COLOR_ARRAY);
    	gl::ColorPointer( 4, GL_UNSIGNED_BYTE, STRIDE_GL_ARRAY, 
		    Course::vncArray + 8*sizeof(GLfloat) );
}

bool
Course::load(const std::string& course)
{
    PP_MESSAGE("Load course");
	
	if( sm_loadedCourse.empty() ||
		sm_loadedCourse != course ||
		sm_loadedCondition != GameMgr::getInstance().getCurrentRace().condition ) 
    {
		reset_course();
		HUD1.reset();
		HUD2.reset();
	
		std::string cwd = ppogl::os::cwd();
		if(cwd.empty()){
			PP_ERROR("Unable to get curent working directory");
    	}
	
		if(course[0]!='/'){		
			std::string data_dir = PPConfig.getString("data_dir");
			if(!script.doFile(data_dir+"/"+course+"/course.nut")){
				// error parsing course script
				return false;
			}
		}else{
			if(!script.doFile(course+"/course.nut")){
				// error parsing course script
				return false;
			}		
		}
		
    	if(ppogl::os::chdir(cwd)==false){
			PP_ERROR("Unable to change into directory: " << cwd );
    	} 
	
		courseRenderer.init();
	
		fillGLArrays();

    	courseRenderer.initQuadtree( elevation, nx, ny,dimension.x()/(nx-1.), 
			  -dimension.y()/(ny-1),
			  players[0].view.pos);

		// init trackmarks for all players
    	TrackMarks::init();
		
    	course_loaded = true;
		
		sm_loadedCourse = course;
		sm_loadedCondition = GameMgr::getInstance().getCurrentRace().condition;		
	}else{
		std::list<Item>::iterator it;
		for(it=itemLocs.begin(); it!=itemLocs.end(); it++){
			(*it).setCollected(false);
			(*it).setDrawable();
		}	
	}
		
	return true;
}

static inline int
intensity_to_terrain(const int intensity)
{
	for(unsigned int i=0; i<num_terrains; i++) {
		if (terrain_texture[i].value == intensity){
			terrain_texture[i].count++;
			return i;
		}
	}
	terrain_texture[0].count++;
	return 0;
}


static int
course_dim_cb(ppogl::Script *vm) 
{
    if(vm->getTop() != 1){
        PP_WARNING("ppcourse.course_dim: Invalid number of arguments");
		return vm->defaultError();
    }

	if(vm->isArray()){
		vm->pushNull();
		vm->next(-2);
		Course::dimension.x() = vm->getFloat();
		vm->pop(2);
		vm->next(-2);
		Course::dimension.y() = vm->getFloat();
		vm->pop(2);
		
		if(vm->next(-2)){
			Course::playDimension.x() = vm->getFloat();
			vm->pop(2);
			vm->next(-2);
			Course::playDimension.y() = vm->getFloat();
			vm->pop(2);
		}else{
			Course::playDimension = Course::dimension;
		}

		vm->pop();
	}else{
		PP_WARNING("ppcourse.course_dim: Value is no array");
		return vm->defaultError();
	}		
	return 0;
} 

static int
angle_cb(ppogl::Script *vm)
{
    double angle;

    if(vm->getTop()!=1){
        PP_WARNING("ppcourse.angle_cb: invalid number of arguments");
		return vm->defaultError();

    } 

    angle = vm->getFloat(1);

    if(angle < MIN_ANGLE){
		PP_WARNING("course angle is too small. Setting to " << MIN_ANGLE);
		angle = MIN_ANGLE;
    }

    if(MAX_ANGLE < angle){
		PP_WARNING("course angle is too large. Setting to " << MAX_ANGLE);
		angle = MAX_ANGLE;
    }

    Course::angle = angle;

    return 0;
} 


static int
elev_cb(ppogl::Script *vm)
{
    PP_MESSAGE("Loading elevation");
	
	ppogl::Image *elev_img;
    float slope;
    int   x,y;
    int   pad;

	if(vm->getTop() != 1){
        PP_WARNING("ppcourse.elev: Invalid number of arguments");
		return vm->defaultError();
    } 

    if(course_loaded){
		PP_WARNING("Ignoring ppcourse.elev: course already loaded");
		return vm->defaultError();
    }

	std::string filename = vm->getString(1);
    elev_img = ppogl::Image::readFile(filename);
    if ( elev_img == NULL ) {
		PP_WARNING( "ppcourse.elev: couldn't load " << filename);
		return vm->defaultError();
    }
	
    nx = elev_img->width;
    ny = elev_img->height;

    Course::elevation = new float[nx*ny];
	
    slope = tan(ANGLES_TO_RADIANS(Course::angle));

	pad = 0;    /* RGBA images rows are aligned on 4-byte boundaries */
    for (y=0; y<ny; y++) {
        for (x=0; x<nx; x++) {
	    ELEV(nx-1-x, ny-1-y) = 
		( ( elev_img->data[ (x + nx * y) * elev_img->depth + pad ] 
		    - base_height_value ) / 255.0 ) * elev_scale
		- double(ny-1.-y)/ny * Course::dimension.y() * slope;
        } 
        //pad += (nx*elev_img->depth) % 4;
    } 

	delete elev_img;
	
    return 0;
} 


static bool
sort_terrain(const int x, const int y)
{
	if(terrain_texture[x].wheight < terrain_texture[y].wheight){
		return true;
	}else{
		return false;
	}	
}


static int
terrain_cb(ppogl::Script *vm)
{
    PP_MESSAGE("Loading terrain");
	
	ppogl::Image *terrain_img;
    int idx;
	int	terrain_value;
	int	image_pointer;

    if(vm->getTop()!=1){
        PP_WARNING("ppcourse.load_terrain: Invalid number of arguments");
		return vm->defaultError();
    } 

	std::string filename = vm->getString(1);
	
    terrain_img = ppogl::Image::readFile(filename);

    if(terrain_img == NULL){
		PP_WARNING("ppcourse.load_terrain: couldn't load " << filename);
		return vm->defaultError();
    }

    if ( nx != terrain_img->width || ny != terrain_img->height ) {
        PP_WARNING("ppcourse.load_terrain: terrain bitmap must have same " 
			 "dimensions as elevation bitmap");
		return vm->defaultError();
    }

    Course::terrain = new int[nx*ny];
	PP_CHECK_ALLOC(Course::terrain);
		
    for(int y=0; y<ny; y++){
        for(int x=0; x<nx; x++){
			idx = (nx-1-x) + nx*(ny-1-y);	
			image_pointer=(x+nx*y)*terrain_img->depth;
			
			terrain_value=terrain_img->data[image_pointer] +
						(terrain_img->data[image_pointer+1] << 8)+ 
						(terrain_img->data[image_pointer+2] << 16);
			
	    Course::terrain[idx] = intensity_to_terrain(terrain_value);
		
        } 
    } 
	
    delete terrain_img;

	//build sorted list with used terrains for quadtree
	usedTerrains.clear();
	for (unsigned int i=0; i<num_terrains; i++){
		//check if the terraintype is used in the course
		if(terrain_texture[i].count>0){
			usedTerrains.push_back(i);
		}
	}
	usedTerrains.sort(sort_terrain);
	
    return 0;
} 


static int
terrain_tex_cb(ppogl::Script *vm)
{
	if(num_terrains>=NUM_TERRAIN_TYPES){
		PP_WARNING("pptheme.terrain_tex: Max number of terrains reached");
		return vm->defaultError();
	}
	
	// fill in values not specified with defaults
	terrain_texture[num_terrains].type=1;
	terrain_texture[num_terrains].value=0;
	terrain_texture[num_terrains].friction=0.5;
	terrain_texture[num_terrains].compression=0.1;
	terrain_texture[num_terrains].soundactive=false;
	terrain_texture[num_terrains].wheight=150;
	terrain_texture[num_terrains].count=0;
	
	std::string name = vm->getStringFromTable("name");
	std::string filename = vm->getStringFromTable("filename");
	
	if(name.empty() || filename.empty()){
		PP_WARNING("pptheme.terrain_tex: Please specify name and texture");
		return vm->defaultError();
	}
	
	ppogl::TextureRef texture =
		ppogl::TextureMgr::getInstance().load(name, filename);
			terrain_texture[num_terrains].texture = texture;
	if(!terrain_texture[num_terrains].texture){
		PP_WARNING("pptheme.terrain_tex: Unable to load texture " << filename << " for terrain " << name);
		return vm->defaultError();
	}	

	if(vm->isKeyInTable("color")){
		vm->pushString("color");
		if(vm->get() && vm->isArray()){
			//color value
			int temp_value=0;
			vm->pushNull();
			for(int i=0; i<3; i++){
				vm->next(-2);
				temp_value += (vm->getInt() << (8*i));
				vm->pop(2);
			}
			terrain_texture[num_terrains].value=temp_value;	
			vm->pop(2);
		}else{
			PP_WARNING("pptheme.terrain_tex: Invalid color in terrain type " << name);
		}
	}else{
		PP_WARNING("Terrain type " << name << " has no color");
	}

	if(vm->isKeyInTable("friction")){
		terrain_texture[num_terrains].friction=vm->getFloatFromTable("friction");
	}

	if(vm->isKeyInTable("compression")){
		terrain_texture[num_terrains].compression=vm->getFloatFromTable("compression");
	}
		
	if(vm->isKeyInTable("wheight")){
		terrain_texture[num_terrains].wheight=vm->getIntFromTable("wheight");
	}	
	
	if(vm->isKeyInTable("sound")){
		terrain_texture[num_terrains].sound=vm->getStringFromTable("sound");
	}
		
	if(vm->isKeyInTable("particle")){
		terrain_texture[num_terrains].particles = 
			ppogl::TextureMgr::getInstance().get(
						vm->getStringFromTable("particle"));
	}
			
	if(vm->isKeyInTable("track_head")){
		terrain_texture[num_terrains].trackmark.head = 
			ppogl::TextureMgr::getInstance().get(
					vm->getStringFromTable("track_head"));
	}
	
	if(vm->isKeyInTable("track_mark")){
		terrain_texture[num_terrains].trackmark.mark = 
			ppogl::TextureMgr::getInstance().get(
					vm->getStringFromTable("track_mark"));		
	}
		
	if(vm->isKeyInTable("track_tail")){
		terrain_texture[num_terrains].trackmark.tail = 
			ppogl::TextureMgr::getInstance().get(
					vm->getStringFromTable("track_tail"));
	}

	if(vm->isKeyInTable("envmap_texture")){
		terrain_texture[num_terrains].envmap = 
			ppogl::TextureMgr::getInstance().get(
					vm->getStringFromTable("envmap_texture"));
	}

    num_terrains++;
    return 0;
} 


static int
start_pt_cb(ppogl::Script *vm)
{
    double xcd =0;
	double ycd =0;

    if(vm->getTop() != 1){
        PP_WARNING("ppcourse.start_pt: Invalid number of arguments");
		return vm->defaultError();
    }
		
	if(vm->isArray()){
		vm->pushNull();
		vm->next(-2);
		xcd = vm->getFloat();
		vm->pop(2);
		vm->next(-2);
		ycd = vm->getFloat();
		vm->pop(2);
		
		vm->pop();
	}else{
		PP_WARNING("ppcourse.start_pt: Value is no array");		
	}

    if(!( xcd > 0 && xcd < Course::dimension.x() )){
		PP_WARNING("ppcourse.start_pt: x coordinate out of bounds, "
		       "using 0");
		xcd = 0;
    }

    if (!( ycd > 0 && ycd < Course::dimension.y() )){
		PP_WARNING("ppcourse.start_pt: y coordinate out of bounds, "
		       "using 0");
		ycd = 0;
    }

    start_pt.x() = xcd;
    start_pt.y() = -ycd;

    return 0;
} 

static int
elev_scale_cb(ppogl::Script *vm)
{
    double scale;
    
	if(vm->getTop() != 1){
        PP_WARNING("ppcourse.elev_scale: invalid number of arguments");
		return vm->defaultError();
    } 
	
	scale = vm->getFloat(1);
	
    if(scale <= 0){
		PP_WARNING("ppcourse.elev_scale: scale must be positive");
		return vm->defaultError();
    }

    elev_scale = scale;

    return 0;
} 

static int
elements_cb(ppogl::Script *vm)
{
	PP_MESSAGE("Loading elements position");
	
	if(vm->getTop()!=1){
		PP_WARNING("ppcourse.elements: Invalid number of arguments");
		return vm->defaultError();
	}
	
	ppogl::Image *elementsImg;
	
	std::string filename = vm->getString(1);
	
	if(filename.empty()){
		// no elements image specified
		return 0;
	}
	
	elementsImg = ppogl::Image::readFile(filename);
    if(elementsImg==NULL || elementsImg->data == NULL){
		PP_WARNING( "ppcourse.load_elements: couldn't load image: " << filename);
		return vm->defaultError();
    }
	
	ppogl::Image::iterator it;
	for(it=elementsImg->begin();it!=elementsImg->end();it++){
		
		if( !(*it).compareRGB(ppogl::Color4c::black) ){
			// the pixel is not black,
			// check for an element type with this color value			
				
			bool found=false;
			
			std::map<std::string, ppogl::RefPtr<ModelType> >::iterator modelit;
			for(modelit=modelTypes.begin();modelit!=modelTypes.end();modelit++){
				if( (*modelit).second->color.compareRGB(*it) ){
					// yes, that's the right model
				
					double x = (elementsImg->width-it.getX())/double(elementsImg->width-1.0)*Course::dimension.x();
					double z =-(elementsImg->height-it.getY())/double(elementsImg->height-1.0)*Course::dimension.y();
					double y = find_y_coord(x,z) + (*modelit).second->height;
					
					Model model((*modelit).second, ppogl::Vec3d(x,y,z));
															
					modelLocs.push_back(model);
					found=true;
					break;
				}
			}
			if(found)continue;
				
			std::map<std::string, ppogl::RefPtr<ItemType> >::iterator itemit;
			for(itemit=itemTypes.begin();itemit!=itemTypes.end();itemit++){
				if( (*itemit).second->color.compareRGB(*it) ){
					// yes, that's the right item
					double x = (elementsImg->width-it.getX())/double(elementsImg->width-1.0)*Course::dimension.x();
					double z =-(elementsImg->height-it.getY())/double(elementsImg->height-1.0)*Course::dimension.y();
					double y = find_y_coord(x,z) + (*itemit).second->above_ground;
					
					if( (*itemit).second->reset_point ){
						resetLocs.push_back(ppogl::Vec2d(x,z));					
						found=true;
						break;						
					}else{
						Item item((*itemit).second, ppogl::Vec3d(x,y,z));	
						
						itemLocs.push_back(item);
												
						found=true;
						break;
					}
				}
			}
		}	
	}
			
	delete elementsImg;
	
	return 0;
}

static int
model_spec_cb(ppogl::Script *vm)
{
	if(	!vm->isKeyInTable("name") ||
		!vm->isKeyInTable("file"))
	{
		PP_WARNING("pptheme.model: Please specify name and file");
		return vm->defaultError();
	}

	ppogl::RefPtr<ModelType> model = new ModelType;
		
	std::string name = vm->getStringFromTable("name");
	
	model->setModel(ppogl::ModelRef(
			new ppogl::Model(vm->getStringFromTable("file"))
		));

	if(vm->isKeyInTable("diameter")){
		model->diameter =
				vm->getFloatFromTable("diameter");		
	}
	
	if(vm->isKeyInTable("height")){
		model->height =
				vm->getFloatFromTable("height");		
	}
	
	if(vm->isKeyInTable("color")){
		vm->pushString("color");
		if(vm->get() && vm->isArray()){
			vm->pushNull();
			for(int i=0; i<3; i++){
				vm->next(-2);
				model->color.values[i] = vm->getInt();
				vm->pop(2);
			}
			vm->pop(2);
		}else{
			PP_WARNING("pptheme.model: Invalid color in model type " << name);
		}
	}else{
		PP_WARNING("pptheme.model: No color specified for model");
	}
		
	// attach model to map
	// an existing model with the same name will be replaced
	modelTypes[name]=model;
		
	return 0;
}

static int
reset_point_cb(ppogl::Script *vm)
{
	if(!vm->isArray(1)){
		PP_WARNING("pptheme.reset_point: Invalid color table");
		return vm->defaultError();
	}
	
	ppogl::RefPtr<ItemType> item = new ItemType;	
	
	vm->pushNull();
	for(int i=0; i<3; i++){
		vm->next(1);
		item->color.values[i]=vm->getInt();	
		vm->pop(2);	
	}
	vm->pop();
			
	item->reset_point = true;
	item->type = ItemType::UNCOLLECTABLE;
		
	itemTypes["reset_point"]=item;
		
	return 0;
}	
		
static int
item_spec_cb(ppogl::Script *vm)
{
	if(	!vm->isKeyInTable("name") ||
		!vm->isKeyInTable("element_type") ||
		!vm->isKeyInTable("texture"))
	{
		PP_WARNING("pptheme.item: Please specify name, element_type and texture");
		return vm->defaultError();
	}
	
	std::string name = vm->getStringFromTable("name");
	std::string type = vm->getStringFromTable("element_type");
	std::string texture = vm->getStringFromTable("texture");	

	ppogl::RefPtr<ItemType> item = new ItemType;
		
	item->texture =
		ppogl::TextureMgr::getInstance().get(texture);
	
	if(!item->texture){
		PP_WARNING("pptheme.item: Unable to get texture for item " << name << ": " << texture);
		return vm->defaultError();
	}
				
	if(type=="herring"){
		item->type=ItemType::HERRING;
	}else if(type=="life"){
		item->type=ItemType::LIFE;
	}
	
	if(vm->isKeyInTable("diameter")){
		item->diam = vm->getFloatFromTable("diameter");
	}
		
	if(vm->isKeyInTable("height")){
		item->height = vm->getFloatFromTable("height");
	}
	
	if(vm->isKeyInTable("above_ground")){
		item->above_ground = vm->getFloatFromTable("above_ground");
	}
	
	if(vm->isKeyInTable("score")){
		item->score = vm->getIntFromTable("score");
	}
	
	if(vm->isKeyInTable("color")){
		vm->pushString("color");
		if(vm->get() && vm->isArray()){
			vm->pushNull();
			for(int i=0; i<3; i++){
				vm->next(-2);
				item->color.values[i] = vm->getInt();
				vm->pop(2);
			}
			vm->pop(2);	
		}else{
			PP_WARNING("pptheme.item: Invalid color in item type " << name);
		}
	}else{
		PP_WARNING("pptheme.item: No color specified for item");
	}
	
	if(vm->isKeyInTable("normal")){
		vm->pushString("normal");
		if(vm->get() && vm->isArray()){
			vm->pushNull();
			for(int i=0; i<3; i++){
				vm->next(-2);
				item->normal.values[i] = vm->getFloat();
				vm->pop(2);
			}
			vm->pop(2);
			item->use_normal=true;
		}else{
			PP_WARNING("pptheme.item: Invalid normal in item type " << name);
		}
		vm->pop(2);
	}

	itemTypes[name]=item;
	
	return 0;	
}

static int
wind_velocity_cb(ppogl::Script *vm)
{
	if(vm->getTop()>2){
		PP_WARNING("pptheme.wind_velocity: Invalid number of arguments");
		return vm->defaultError();
	}
		
	ppogl::Vec3d velocity;
	double scale = 1.0;
	
	if(vm->isArray(1)){
		vm->pushNull();
		for(int i=0; i<3; i++){
			vm->next(1);
			velocity.values[i]=vm->getFloat();
			vm->pop(2);
		}
	}else{
		PP_WARNING("pptheme.wind_velocity: Invalid direction");
		return vm->defaultError();
	}
	vm->pop();
	
	if(vm->getTop()==2){
		scale = vm->getFloat(2);	
	}
		
	set_wind_velocity(velocity,scale);

	return 0;
}


static int
hud_cb(ppogl::Script *vm)
{
	if(vm->getTop() != 1){
		PP_WARNING("pptheme.hud: Invalid number of arguments");
		return vm->defaultError();
	}
		
	int element_num=-1;
	HUD::Element element;
	
	int hud = int(vm->getFloatFromTable("hud")); 
		
	if(!vm->isKeyInTable("element_type")){
		PP_WARNING("pptheme.hud: No element type specified");
		return vm->defaultError();
	}
	
	std::string type = vm->getStringFromTable("element_type");
		
	if(type=="text"){
		element.type=0;
	}else if(type=="fsb"){
		element.type=1;
	}else if(type=="herring"){
		element.type=2;
	}else if(type=="image"){
		element.type=3;
	}else if(type=="time"){
		element.type=4;
	}else if(type=="speed"){
		element.type=5;	
	}else if(type=="gauge"){
		element.type=6;	
	}else if(type=="energybar"){
		element.type=7;	
	}else if(type=="speedbar"){
		element.type=8;	
	}else if(type=="percentage"){
		element.type=9;	
	}else if(type=="percentagebar"){
		element.type=10;	
	}else if(type=="percentageimage"){
		element.type=11;
	}else if(type=="fpsimage"){
		element.type=12;
	}else{
		PP_WARNING("pptheme.hud: Unknown type: " << type);
		return vm->defaultError();
	}

	if(vm->isKeyInTable("position")){
		vm->pushString("position");
		if(vm->get() && vm->isArray()){
			// position array
			vm->pushNull();
			vm->next(-2);
			element.x = vm->getInt();
			vm->pop(2);
			vm->next(-2);
			element.y = vm->getInt();
			vm->pop(2);
			vm->pop(2); //the table
		}else{
			PP_WARNING("pptheme.hud: Invalid position");
		}
	}
	
	//texture
	if(vm->isKeyInTable("texture")){
		std::string texture = vm->getStringFromTable("texture");	
		element.texture = 
			ppogl::TextureMgr::getInstance().get(texture);
		
		if(!element.texture){
			PP_WARNING("pptheme.hud: Unable to get texture: " << texture);
		}
	}
	
	// width
	if(vm->isKeyInTable("width")){
		element.width = vm->getIntFromTable("width");
	}

	// height	
	if(vm->isKeyInTable("height")){
		element.height = vm->getIntFromTable("height");
	}
	
	// size
	if(vm->isKeyInTable("size")){
		element.size = vm->getIntFromTable("size");
	}
	
	// string
	if(vm->isKeyInTable("string")){
		element.string = _(vm->getStringFromTable("string"));
	}
		
	// font
	if(vm->isKeyInTable("font")){
		std::string font = vm->getStringFromTable("font");
		element.font = ppogl::FontMgr::getInstance().get(font);
		if(!element.font){
			PP_WARNING("pptheme.hud: Unable to get font: " << font); 
		}else{
			element.height = int(element.font->ascender());			
		}			
	}
	
	// angle
	if(vm->isKeyInTable("angle")){
		element.angle = vm->getIntFromTable("angle");
	}	
	
	//precalculate unicode string and width of the font
	if(element.type==0 && element.font && !element.string.empty()){
		ppogl::Font::utf8ToUnicode(element.u_string,element.string);
		element.width = int(element.font->advance(element.u_string));
	}
		
	if(hud!=1){
		PP_WARNING("pptheme.hud: Invalid hud " << hud << " is not supported");
	}else{
		if(element_num==-1){
			HUD1.add(element);
			
			//only for testing purpose
			HUD2.add(element);
		}else{
			HUD1.update(element_num,element);
					
			//only for testing purpose
			HUD2.update(element_num,element);
		}
	}

	return 0;
}

static int
get_condition_cb(ppogl::Script *vm)
{
	switch(GameMgr::getInstance().getCurrentRace().condition)
	{
		case CourseData::CONDITION_CLOUDY:
			vm->pushString("cloudy");
			break;
		case CourseData::CONDITION_NIGHT:
			vm->pushString("night");
			break;
		case CourseData::CONDITION_EVENING:
			vm->pushString("evening");
			break;		
		default:
			vm->pushString("sunny");	
	}
	return 1;
}

static int
base_height_value_cb(ppogl::Script *vm)
{
	if(vm->getTop()!=1){
		PP_WARNING("ppcourse.base_height_value: Invalid number of arguments");
		return vm->defaultError();
	}
	
	int value = vm->getInt(1);
	
	if(value<0) value=0;
	else if(value>255) value=255;
	
	base_height_value = value;	
	return 0;
}

static int
add_model_cb(ppogl::Script *vm)
{	
	if(!vm->isTable()){
		PP_WARNING("ppcourse.add_model: Arguments needs to be a table");
		return vm->defaultError();
	}
	
	std::string name = vm->getStringFromTable("name");
	
	if(name.empty()){
		PP_WARNING("ppcourse.add_model: No name given for model");
	}
	
	ppogl::Vec3d position;
	
	if(vm->isKeyInTable("position")){
		vm->pushString("position");
		if(vm->get() && vm->isArray()){
			// position array
			vm->pushNull();
			vm->next(-2);
			position.x() = vm->getFloat();
			vm->pop(2);
			vm->next(-2);
			position.z() = (-1)*vm->getFloat();
			vm->pop(2);
			vm->pop(2);
		}else{
			PP_WARNING("ppcourse.add_model: Invalid position");
			return vm->defaultError();
		}
	}
	
	std::map<std::string,ppogl::RefPtr<ModelType> >::iterator it;

	if((it=modelTypes.find(name))==modelTypes.end()){
		PP_WARNING("ppcourse.add_item: Unable to find item: " << name);
		return vm->defaultError();
	}
	
	double aboveGround = (*it).second->height;
	
	if(vm->isKeyInTable("above_ground")){
		aboveGround += vm->getFloatFromTable("above_ground");
	}
		
	position.y() = find_y_coord(position.x(),position.z()) + aboveGround;
	
	Model model((*it).second, position);
	
	modelLocs.push_back(model);
	
	return 0;
}

static int
add_item_cb(ppogl::Script *vm)
{
	if(!vm->isTable()){
		PP_WARNING("ppcourse.add_item: Arguments needs to be a table");
		return vm->defaultError();
	}
	
	std::string name = vm->getStringFromTable("name");
	
	if(name.empty()){
		PP_WARNING("ppcourse.add_item: No name given for item");
	}
	
	ppogl::Vec3d position;
	
	if(vm->isKeyInTable("position")){
		vm->pushString("position");
		if(vm->get() && vm->isArray()){
			// position array
			vm->pushNull();
			vm->next(-2);
			position.x() = vm->getFloat();
			vm->pop(2);
			vm->next(-2);
			position.z() = (-1)*vm->getFloat();
			vm->pop(2);
			vm->pop(2);
		}else{
			PP_WARNING("ppcourse.add_item: Invalid position");
			return vm->defaultError();
		}
	}
	
	std::map<std::string,ppogl::RefPtr<ItemType> >::iterator it;

	if((it=itemTypes.find(name))==itemTypes.end()){
		PP_WARNING("ppcourse.add_item: Unable to find item: " << name);
		return vm->defaultError();
	}
	
	double aboveGround = (*it).second->above_ground;
	
	if(vm->isKeyInTable("above_ground")){
		aboveGround = vm->getFloatFromTable("above_ground");
	}
		
	position.y() = find_y_coord(position.x(),position.z()) + aboveGround;
	
	Item item((*it).second, position);
	
	if(vm->isKeyInTable("normal")){
		vm->pushString("normal");
		if(vm->get() && vm->isArray()){
			ppogl::Vec3d normal;
			vm->pushNull();
			for(int i=0; i<3; i++){
				vm->next(-2);
				normal.values[i] = vm->getFloat();
				vm->pop(2);
			}
			vm->pop(2);
			item.setNormal(normal);
		}	
	}
	
	if(vm->isKeyInTable("diameter")){
		item.setDiameter(vm->getFloatFromTable("diameter"));
	}
	
	if(vm->isKeyInTable("height")){
		item.setHeight(vm->getFloatFromTable("height"));
	}
	
	itemLocs.push_back(item);
	
	return 0;
}

static int
add_reset_point_cb(ppogl::Script *vm)
{
	ppogl::Vec2d position;	
	if(vm->isArray()){
			// position array
			vm->pushNull();
			vm->next(-2);
			position.x() = vm->getFloat();
			vm->pop(2);
			vm->next(-2);
			position.x() = (-1)*vm->getFloat();
			vm->pop(2);
			vm->pop(2);
	}else{
		PP_WARNING("ppcourse.add_reset_point: Invalid position");
		return vm->defaultError();
	}
		
	resetLocs.push_back(position);
	
	return 0;
}

static const struct ppogl::Script::Lib ppcourselib[]={
	{"course_dim", course_dim_cb},
	{"start_pt", start_pt_cb},
	{"angle", angle_cb},  
	{"elev_scale", elev_scale_cb},
	{"elev", elev_cb},
	{"load_terrain", terrain_cb},
	{"load_elements", elements_cb},
	{"get_condition", get_condition_cb},
	{"base_height_value", base_height_value_cb},
	{"add_model", add_model_cb},
	{"add_item", add_item_cb},
	{"add_reset_point", add_reset_point_cb},
	{NULL, NULL}
};

static const struct ppogl::Script::Lib ppthemelib[]={
	{"terrain_tex", terrain_tex_cb},
	{"hud", hud_cb},
	{"wind_velocity", wind_velocity_cb},
	{"reset_point", reset_point_cb},
	{"item",  item_spec_cb},
	{"model",  model_spec_cb},
	{NULL, NULL}
};

void
Course::registerCallbacks()
{
	script.registerLib("ppcourse", ppcourselib);
	script.registerLib("pptheme", ppthemelib);
}
