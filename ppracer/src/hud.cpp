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

#include "gl_util.h"
#include "fps.h"
#include "phys_sim.h"
#include "course_mgr.h"
#include "hud.h"

#include "stuff.h"
#include "game_mgr.h"
#include "gameconfig.h"

#include "ppogl/base/defs.h"

HUD HUD1;
HUD HUD2;

HUD::Element::Element()
 : type(-1), 
   x(0), y(0),
   font(NULL),
   texture(0),
   width(0), height(0),
   size(0),
   angle(0)   
{
}

HUD::HUD()
{
	m_numElements=0;	
}

void
HUD::reset()
{
	m_numElements=0;
	initGauge();
}

bool
HUD::add(Element newElement){
	if( m_numElements < HUD_MAX_ITEMS){
		m_element[m_numElements] = newElement;
		m_numElements++;
		return true;
	}else{
		return false;
	}	
}
	
bool
HUD::update(const int i, Element newElement){
	if( m_numElements > i ){
		m_element[i] = newElement;
		return true;	
	}else{
		//element i not available
		return false;
	}	
}

void
HUD::draw(Player& plyr, int width, int height)
{
	m_width=width;
	m_height = height;
	
	gl::PushMatrix();
	{
	gl::MatrixMode(GL_PROJECTION);
    gl::LoadIdentity();
    gl::Ortho(0.0, m_width, 0.0,
			  m_height, -1.0, 1.0);
    gl::MatrixMode(GL_MODELVIEW);
    gl::LoadIdentity();
    gl::Translate(0.0, 0.0, -1.0);
	
	set_gl_options( TEXFONT );
		
	for(int i=0; i<m_numElements; i++){	
		switch(m_element[i].type){
			case 0:
				text(i);
				break;
			case 1:
				if(GameConfig::displayFPS){
					fps(i);
    			}	
				break;
			case 2:
				herring(i,plyr.herring);
				break;
			case 3:
				image(i);
				break;
			case 4:
				time(i);
				break;	
			case 5:
				{
				ppogl::Vec3d vel = plyr.vel;
    			speed(i,vel.normalize()* M_PER_SEC_TO_KM_PER_H);
				}
				break;
			case 6:
				{
				ppogl::Vec3d vel = plyr.vel;
				gauge(i,vel.normalize()* M_PER_SEC_TO_KM_PER_H,plyr.control.jump_amt);
				}
				break;
			case 7:
				bar(i,plyr.control.jump_amt);
				break;
			case 8:
				{
				ppogl::Vec3d vel = plyr.vel;
				bar(i,vel.normalize()* M_PER_SEC_TO_KM_PER_H/135);
				}	
				break;
			case 9:
				if(GameConfig::displayCoursePercentage){
					coursePercentage(i, plyr);
				}
				break;
			case 10:
				if(GameConfig::displayCoursePercentage){		
					bar(i,plyr.getCoursePercentage()/100);
				}
				break;
			case 11:
				if(GameConfig::displayCoursePercentage){		
					image(i);
				}
				break;				
			case 12:
				if(GameConfig::displayFPS){
					image(i);
    			}	
				break;
		}
	}
	
	}
	gl::PopMatrix();
}

void
HUD::text(const int i)
{
	if(m_element[i].font){
		fix_xy(m_element[i].x, m_element[i].y,
			   m_element[i].height, m_element[i].width);
		
		m_element[i].font->draw(m_element[i].string,
				   m_element[i].x, m_element[i].y);		
	}
}

void
HUD::fps(const int i)
{
    if(m_element[i].font){
		char string[16];
		snprintf(string, 16, m_element[i].string.c_str(), fpsCounter.get());
		
		ppogl::Font::utf8ToUnicode(m_element[i].u_string,string);
		int width = int(m_element[i].font->advance(m_element[i].u_string));
		
		fix_xy(m_element[i].x,m_element[i].y,m_element[i].height,width);
		m_element[i].font->draw(m_element[i].u_string, m_element[i].x, m_element[i].y);		
	}
}

void
HUD::herring(const int i, const int herring_count)
{
	if(m_element[i].font){
		char string[12];
		snprintf(string, 12, m_element[i].string.c_str(), herring_count );
		
		ppogl::Font::utf8ToUnicode(m_element[i].u_string,string);
		int width = int(m_element[i].font->advance(m_element[i].u_string));
		
		fix_xy(m_element[i].x,m_element[i].y,m_element[i].height,width);
		m_element[i].font->draw(m_element[i].u_string, m_element[i].x, m_element[i].y);		
	}
}

void
HUD::image(const int i)
{
	if(!m_element[i].texture) return;

	fix_xy( m_element[i].x, m_element[i].y, m_element[i].height, m_element[i].width);
	
    gl::PushMatrix();
    {
    gl::BindTexture(GL_TEXTURE_2D, m_element[i].texture);
	gl::Enable(GL_TEXTURE_2D);
	gl::Color(ppogl::Color::white);
		
	gl::Translate(m_element[i].x, m_element[i].y);

	gl::Begin( GL_QUADS );
	{
	   	gl::TexCoord(0, 0);
	    gl::Vertex(0, 0);

	    gl::TexCoord(double(m_element[i].width) / m_element[i].size,0.0);
	    gl::Vertex(m_element[i].width, 0);

	    gl::TexCoord( 
			double(m_element[i].width) / m_element[i].size,
			double(m_element[i].height) / m_element[i].size );
	    gl::Vertex(m_element[i].width, m_element[i].height);

	    gl::TexCoord(0.0, double(m_element[i].height) / m_element[i].size );
	    gl::Vertex(0.0, m_element[i].height);
	}
	gl::End();
    }
    gl::PopMatrix();
}

void
HUD::time(const int i)
{
    if(m_element[i].font){
		char string[16];
		int minutes, seconds, hundredths;
		    
		getTimeComponents( GameMgr::getInstance().time, minutes, seconds, hundredths );
		snprintf(string, 16, m_element[i].string.c_str(), minutes, seconds, hundredths);
		
		ppogl::Font::utf8ToUnicode(m_element[i].u_string,string);
		int width = int(m_element[i].font->advance(m_element[i].u_string));
		
		fix_xy(m_element[i].x,m_element[i].y,m_element[i].height,width);
		m_element[i].font->draw(m_element[i].u_string, m_element[i].x, m_element[i].y);		
	
	}
}

void
HUD::speed(const int i, const double speed)
{
	if(m_element[i].font){
		char string[16];
		snprintf(string, 16, m_element[i].string.c_str(), speed );
		
		ppogl::Font::utf8ToUnicode(m_element[i].u_string,string);
		int width = int(m_element[i].font->advance(m_element[i].u_string));
		
		fix_xy(m_element[i].x,m_element[i].y,m_element[i].height,width);
		m_element[i].font->draw(m_element[i].u_string, m_element[i].x-(width/2), m_element[i].y);		
		
	}	
}

void
HUD::bar(const int i, double percentage)
{
	if(!m_element[i].texture) return;
	
	if(percentage>1)percentage=1;
	
	double temp_sin=sin(double(m_element[i].angle)/180.0*M_PI);
	double temp_cos=cos(double(m_element[i].angle)/180.0*M_PI);
	
	fix_xy(m_element[i].x,m_element[i].y,int(m_element[i].height));
	
    gl::PushMatrix();
	{
	gl::Enable(GL_TEXTURE_2D);
    gl::BindTexture(GL_TEXTURE_2D, m_element[i].texture);
    gl::Color(ppogl::Color::white);
		
	gl::Translate(m_element[i].x, m_element[i].y,0);

	gl::Begin(GL_QUADS);
	{
		gl::TexCoord(0,0);
	    gl::Vertex(0,0);

	    gl::TexCoord(1,0);
	    gl::Vertex(temp_cos*m_element[i].width,temp_sin*m_element[i].width);

    	gl::TexCoord(1.0,percentage);
    	gl::Vertex(temp_cos*m_element[i].width+temp_sin*m_element[i].height*percentage,
			temp_sin*m_element[i].width-temp_cos*m_element[i].height*percentage);

    	gl::TexCoord(0.0,percentage);
    	gl::Vertex(temp_sin*m_element[i].height*percentage, (-1)*temp_cos*m_element[i].height*percentage);

	}
	gl::End();
    }
    gl::PopMatrix();
	
}

//Sollte man bei Gelegenheit noch konfigurierbar machen
#define ENERGY_GAUGE_BOTTOM 3.0
#define ENERGY_GAUGE_CENTER_X 71.0
#define ENERGY_GAUGE_CENTER_Y 55.0
#define SPEEDBAR_OUTER_RADIUS ( ENERGY_GAUGE_CENTER_X )
#define SPEEDBAR_BASE_ANGLE 225
#define SPEEDBAR_MAX_ANGLE 45
#define SPEEDBAR_GREEN_MAX_SPEED ( MAX_PADDLING_SPEED * M_PER_SEC_TO_KM_PER_H )
#define SPEEDBAR_YELLOW_MAX_SPEED 100
#define SPEEDBAR_RED_MAX_SPEED 160
#define SPEEDBAR_GREEN_FRACTION 0.5
#define SPEEDBAR_YELLOW_FRACTION 0.25
#define SPEEDBAR_RED_FRACTION 0.25
#define SPEED_UNITS_Y_OFFSET 4.0

static ppogl::Color energy_background_color(0.2, 0.2, 0.2, 0.5);
static ppogl::Color energy_foreground_color(0.54, 0.59, 1.00, 0.5);
static ppogl::Color speedbar_background_color(0.2, 0.2, 0.2, 0.5);

void
HUD::initGauge()
{
	m_energymaskTex = 
		ppogl::TextureMgr::getInstance().get("gauge_energy_mask");

	m_speedmaskTex = 
		ppogl::TextureMgr::getInstance().get("gauge_speed_mask");

	m_outlineTex =
		ppogl::TextureMgr::getInstance().get("gauge_outline");
}

void
HUD::gauge(const int i, const double speed, const double energy)
{
	GLfloat xplane[4] = { 1.0/m_element[i].size, 0.0, 0.0, 0.0 };
    GLfloat yplane[4] = { 0.0, 1.0/m_element[i].size, 0.0, 0.0 };
    double y;
    double speedbar_frac;

	//the gauge bar needs it's own mode
	//we reset the mode at the end of the function
    set_gl_options( GAUGE_BARS );

    gl::TexGen(GL_S, GL_OBJECT_PLANE, xplane);
    gl::TexGen(GL_T, GL_OBJECT_PLANE, yplane);

    gl::PushMatrix();
    {
	gl::Translate(m_width - m_element[i].width,0);

	gl::Color(energy_background_color);

	gl::BindTexture(GL_TEXTURE_2D, m_energymaskTex);

	y = ENERGY_GAUGE_BOTTOM + energy * m_element[i].height;

	gl::Begin( GL_QUADS );
	{
	    gl::Vertex(0.0, y);
	    gl::Vertex(m_element[i].size, y);
	    gl::Vertex(m_element[i].size, m_element[i].size);
	    gl::Vertex(0.0, m_element[i].size);
	}
	gl::End();

	gl::Color(energy_foreground_color);

	gl::Begin(GL_QUADS);
	{
	    gl::Vertex(0.0, 0.0);
	    gl::Vertex(m_element[i].size, 0.0);
	    gl::Vertex(m_element[i].size, y);
	    gl::Vertex(0.0, y);
	}
	gl::End();

	/* Calculate the fraction of the speed bar to fill */
	speedbar_frac = 0.0;

	if ( speed > SPEEDBAR_GREEN_MAX_SPEED ) {
	    speedbar_frac = SPEEDBAR_GREEN_FRACTION;
	    
	    if ( speed > SPEEDBAR_YELLOW_MAX_SPEED ) {
		speedbar_frac += SPEEDBAR_YELLOW_FRACTION;
		
		if ( speed > SPEEDBAR_RED_MAX_SPEED ) {
		    speedbar_frac += SPEEDBAR_RED_FRACTION;
		} else {
		    speedbar_frac +=
			( speed - SPEEDBAR_YELLOW_MAX_SPEED ) /
			( SPEEDBAR_RED_MAX_SPEED - SPEEDBAR_YELLOW_MAX_SPEED ) *
			SPEEDBAR_RED_FRACTION;
		}

	    } else {
		speedbar_frac += 
		    ( speed - SPEEDBAR_GREEN_MAX_SPEED ) /
		    ( SPEEDBAR_YELLOW_MAX_SPEED - SPEEDBAR_GREEN_MAX_SPEED ) *
		    SPEEDBAR_YELLOW_FRACTION;
	    }
	    
	} else {
	    speedbar_frac +=  speed/SPEEDBAR_GREEN_MAX_SPEED * 
		SPEEDBAR_GREEN_FRACTION;
	}

	gl::Color(speedbar_background_color);

	gl::BindTexture(GL_TEXTURE_2D, m_speedmaskTex);

	draw_partial_tri_fan(1.0);

	gl::Color(ppogl::Color::white);

	draw_partial_tri_fan( MIN( 1.0, speedbar_frac ) );

	gl::Color(ppogl::Color::white);

	gl::BindTexture(GL_TEXTURE_2D, m_outlineTex);

	gl::Begin(GL_QUADS);
	{
	    gl::Vertex(0.0, 0.0);
	    gl::Vertex(m_element[i].size, 0.0);
	    gl::Vertex(m_element[i].size, m_element[i].size);
	    gl::Vertex(0.0, m_element[i].size);
	}
	gl::End();
	
    }
    gl::PopMatrix();
	
	//we reset this because all other elements need TEXFONT
	set_gl_options( TEXFONT );
}

#define CIRCLE_DIVISIONS 10

void 
HUD::draw_partial_tri_fan(const double fraction)
{
    int divs;
    double angle, angle_incr, cur_angle;
    int i;
    bool trifan = false;
    ppogl::Vec2d pt;

    angle = SPEEDBAR_BASE_ANGLE + 
	( SPEEDBAR_MAX_ANGLE - SPEEDBAR_BASE_ANGLE ) * fraction;

    divs = int(( SPEEDBAR_BASE_ANGLE - angle ) * CIRCLE_DIVISIONS / 360.0);

    cur_angle = SPEEDBAR_BASE_ANGLE;

    angle_incr = 360.0 / CIRCLE_DIVISIONS;

    for (i=0; i<divs; i++) {
	if ( !trifan ) {
	    start_tri_fan();
	    trifan = true;
	}

	cur_angle -= angle_incr;

	pt = calc_new_fan_pt( cur_angle );

	gl::Vertex(pt.x(), pt.y());
    }

    if ( cur_angle > angle + EPS ) {
	cur_angle = angle;
	if ( !trifan ) {
	    start_tri_fan();
	    trifan = true;
	}

	pt = calc_new_fan_pt( cur_angle );

	gl::Vertex(pt.x(), pt.y());
    }

    if ( trifan ) {
		gl::End();
		trifan = false;
    }
}

ppogl::Vec2d
HUD::calc_new_fan_pt(const double angle )
{
    ppogl::Vec2d pt;
    pt.x() = ENERGY_GAUGE_CENTER_X + cos( ANGLES_TO_RADIANS( angle ) ) *
	SPEEDBAR_OUTER_RADIUS;
    pt.y() = ENERGY_GAUGE_CENTER_Y + sin( ANGLES_TO_RADIANS( angle ) ) *
	SPEEDBAR_OUTER_RADIUS;

    return pt;
}

void
HUD::start_tri_fan(void)
{
    ppogl::Vec2d pt;

    gl::Begin(GL_TRIANGLE_FAN);
    gl::Vertex(ENERGY_GAUGE_CENTER_X, ENERGY_GAUGE_CENTER_Y);

    pt = calc_new_fan_pt( SPEEDBAR_BASE_ANGLE ); 

    gl::Vertex(pt);
}

void
HUD::fix_xy(int &x, int &y, const int asc, const int width)
{
	if(x<0){
		x=m_width+x-width+1;
	}
	if(y<0){
		y=m_height+y-asc+1;
	}
}

void
HUD::coursePercentage(const int i, const Player& plyr)
{
	if(m_element[i].font){
		char string[16];
		snprintf(string, 16, m_element[i].string.c_str(), plyr.getCoursePercentage() );
		
		ppogl::Font::utf8ToUnicode(m_element[i].u_string,string);
		int width = int(m_element[i].font->advance(m_element[i].u_string));
		
		fix_xy(m_element[i].x,m_element[i].y,m_element[i].height,width);
		m_element[i].font->draw(m_element[i].u_string, m_element[i].x, m_element[i].y);		
		
	}
}
