/* 
 * PPRacer 
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
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


#include "frame.h"
#include "ui_theme.h"
#include "ppracer.h"
#include "render_util.h"
#include "alg/glhelper.h"

namespace pp{

Frame::Frame( pp::Vec2d pos, pp::Vec2d size )
{
    m_position = pos;
    m_size = size;

    m_visible = false;
    m_active = false;
}

Frame::~Frame()
{
    setVisible( false );
    setActive( false );
}

void
Frame::draw()
{
	gl::Disable(GL_TEXTURE_2D);

    gl::Color(theme.foreground);
    gl::Rect(m_position, m_position + m_size);

    gl::Color(theme.background);
    gl::Rect(m_position + 4, m_position + m_size - 4);

    gl::Enable(GL_TEXTURE_2D);
}
	
} //namespace pp
