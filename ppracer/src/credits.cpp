/* 
 * PPRacer 
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


#include "credits.h"

#include "ppgltk/audio/audio.h"

#include "translation.h"

#define CREDITS_MAX_Y -140
#define CREDITS_MIN_Y 64

typedef struct {
    bool translateable;
	const char *binding;
    const char *text;
} credit_line_t;

static credit_line_t credit_lines[] = 
{
	{false, "credits_h1", "PlanetPenguin Racer" },
    {false, "credits_text", "http://racer.planetpenguin.de" },
	{false, "credits_text", "" },
    {true, "credits_h2", "Development Team" },
    {false, "credits_text", "Volker Ströbel" },
    {false, "credits_text", "" },
	{true, "credits_h2", "Translators" },
	{false, "credits_text", "Alexander Aksenov" },	
	{false, "credits_text", "Eric Alvarez Llimos" },	
	{false, "credits_text", "Helder Correia" },	
	{false, "credits_text", "Jan Karwowski" },	
	{false, "credits_text", "Karl Ove Hufthammer" },	
	{false, "credits_text", "Marco Antonio Blanco" },	
	{false, "credits_text", "Massimo Spazian" },	
	{false, "credits_text", "Mikel Olasagasti" },	
	{false, "credits_text", "Pascal Garcia" },	
	{false, "credits_text", "Theo Snelleman" },	
	{false, "credits_text", "Trygve B. Wiig" },
    {false, "credits_text", "" },
	{true, "credits_h2", "Contributors" },
	{false, "credits_text", "Peter Reichel" },
	{false, "credits_text", "Rainer Koschnick" },
	{false, "credits_text", "Teemu Vesala" },
	{false, "credits_text", "Theo Snelleman" },
    {false, "credits_text", "" },
    {true, "credits_h2", "Tux Racer Development Team" },
    {false, "credits_text", "Patrick \"Pog\" Gilhuly" },
    {false, "credits_text", "Eric \"Monster\" Hall" },
    {false, "credits_text", "Rick Knowles" },
    {false, "credits_text", "Vincent Ma" },
    {false, "credits_text", "Jasmin Patry" },
    {false, "credits_text", "Mark Riddell" },
    {false, "credits_text", "" },
    {true, "credits_h2", "Music" },
    {false, "credits_text", "Joseph Toscano" },
    {false, "credits_text", "" },
    {true, "credits_h2", "Thanks To:" },
    {false, "credits_text", "Larry Ewing" },
    {false, "credits_text", "Thatcher Ulrich" },
    {false, "credits_text", "Steve Baker" },
    {false, "credits_text", "Ingo Ruhnke" },
    {false, "credits_text", "James Barnard" },
    {false, "credits_text", "Alan Levy" },
    {false, "credits_text", "University of Waterloo Computer Graphics Lab" },
    {false, "credits_text", "" },
	{false, "credits_text", "PlanetPenguin Racer is based on Tux Racer" },
    {false, "credits_text", "Tux Racer is a Sunspire Studios Production" },
    {false, "credits_text_small", "Tux Racer is a trademark of Jasmin F. Patry" },
	{false, "credits_text_small", "PlanetPenguin Racer is copyright 2004, 2005 The PPRacer team" },
    {false, "credits_text_small", "Tux Racer is copyright 1999, 2000, 2001 Jasmin F. Patry" },
};

Credits::Credits()
 : m_yOffset(0.0)
{
	lines = new c_line_t[sizeof( credit_lines ) / sizeof( credit_lines[0])];
	
	for (unsigned int i=0; i<sizeof( credit_lines ) / sizeof( credit_lines[0] ); i++) {
		lines[i].font=pp::Font::get(credit_lines[i].binding);
		if(credit_lines[i].translateable){
			lines[i].text=_(credit_lines[i].text);
		}else{
			lines[i].text=credit_lines[i].text;
		}		
	}
	
    play_music( "credits_screen" );
}


Credits::~Credits()
{	
	delete [] lines;
}

void
Credits::loop(float timeStep)
{
	int width, height;
    width = getparam_x_resolution();
    height = getparam_y_resolution();

    update_audio();

    clear_rendering_context();

    set_gl_options( GUI );

    UIMgr.setupDisplay();

    drawText( timeStep );

	drawSnow(timeStep);

	theme.drawMenuDecorations();

    UIMgr.draw();

    reshape( width, height );

    winsys_swap_buffers();
}


void
Credits::drawText( float timeStep )
{
    int w = getparam_x_resolution();
    int h = getparam_y_resolution();
    float y;

    m_yOffset += timeStep * 30;
    y = CREDITS_MIN_Y + m_yOffset;

	//loop through all credit lines
	for (unsigned int i=0; i<sizeof( credit_lines ) / sizeof( credit_lines[0] ); i++) {
	    c_line_t line = lines[i];

		//get the font and sizes for the binding
		//pp::Font *font = pp::Font::get(line.binding);
		float width = line.font->advance(line.text);
		float desc = line.font->descender();
		float asc = line.font->ascender();
		
		//draw the line on the screen
		line.font->draw(line.text, w/2 - width/2, y);

		//calculate the y value for the next line
		y-=asc-desc;
	}

	//if the last string reaches the top, reset the y offset
    if ( y > h+CREDITS_MAX_Y ) {
		m_yOffset = 0;
    }

    // Draw strips at the top and bottom to clip out text 
    gl::Disable(GL_TEXTURE_2D);

	gl::Color(theme.background);
    gl::Rect(0, 0, w, CREDITS_MIN_Y);

    gl::Begin(GL_QUADS);
    {
		gl::Vertex(0, CREDITS_MIN_Y);
		gl::Vertex(w, CREDITS_MIN_Y);
		gl::Color(theme.background, 0.0f);
		gl::Vertex(w, CREDITS_MIN_Y + 30);
		gl::Vertex(0, CREDITS_MIN_Y + 30);
    }
    gl::End();

    gl::Color(theme.background);
    gl::Rect(0, h+CREDITS_MAX_Y, w, h);

    gl::Begin(GL_QUADS);
    {
		gl::Vertex(w, h+CREDITS_MAX_Y);
		gl::Vertex(0, h+CREDITS_MAX_Y);
		gl::Color(theme.background, 0.0f);
		gl::Vertex(0, h+CREDITS_MAX_Y - 30);
		gl::Vertex(w, h+CREDITS_MAX_Y - 30);
    }
    gl::End();

    gl::Color(pp::Color::white);

    gl::Enable(GL_TEXTURE_2D);
}

bool
Credits::mouseButtonReleaseEvent(int button, int x, int y)
{
	set_game_mode( GAME_TYPE_SELECT );
    winsys_post_redisplay();
	return true;
}

bool
Credits::keyReleaseEvent(SDLKey key)
{
	set_game_mode( GAME_TYPE_SELECT );
    winsys_post_redisplay();
	return true;
}
