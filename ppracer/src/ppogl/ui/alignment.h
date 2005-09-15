/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2004-2005 Volker Stroebel <volker@planetpenguin.de>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
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

#ifndef _PPOGL_ALIGNMENT_H_
#define _PPOGL_ALIGNMENT_H_

#include "../base/vec2d.h"
#include "../base/signal.h"

namespace ppogl{

///A class for storing the alignment of an UI widget
class Alignment
{
private:
	float m_horizontal;
	float m_vertical;
	
	void checkHorizontal();
	void checkVertical();

public:
	Alignment(float horizontal=0.0f, float vertical=0.0f);
	
	void left()		{ m_horizontal=0.0f; signalChanged.Emit();}; ///horizontal align left
	void right()	{ m_horizontal=1.0f; signalChanged.Emit();}; ///horizontal align right
	void center()	{ m_horizontal=0.5f; signalChanged.Emit();}; ///horizontal allign center
	void bottom()	{ m_vertical=0.0f; signalChanged.Emit();};   ///vertical align bottom
	void top()		{ m_vertical=1.0f; signalChanged.Emit();};   ///vertical align top
	void middle()	{ m_vertical=0.5f; signalChanged.Emit();};	///vertical align middle

	void setHorizontal(float horizontal);
	void setVertical(float vertical);
	void set(float horizontal, float vertical);	
	
	ppogl::Vec2d alignPosition(const ppogl::Vec2d &position, const ppogl::Vec2d &size) const;
	
	//signals
	ppogl::Signal0 signalChanged;
};

} //namepsace ppogl

#endif // _PPOGL_ALIGNMENT_H_
