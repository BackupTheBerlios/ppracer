/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2005 Volker Stroebel <volker@planetpenguin.de>
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

#ifndef _PPOGL_SCALE_H_
#define _PPOGL_SCALE_H_

#include "widget.h"
#include "imagebutton.h"

namespace ppogl{
	
/// Base class for scale widgets
class Scale
 : public Widget
{	
protected:
	Scale(int width, int height);
	
	ppogl::ImageButton m_upBtn;
	ppogl::ImageButton m_downBtn;
	
	float m_value;
	float m_step;

public:
	void setPosition(const ppogl::Vec2d& position);
	
	void setValue(float value);
	float getValue();

	void setStep(float step);

	virtual void updateButtonPositions()=0;
	void updateButtonStates();

	void upButtonCallback();
	void downButtonCallback();

	//signals
	ppogl::Signal0 signalChanged;
};	

/// A horizontal slider widget
class HScale
 : public Scale
{
public:
	HScale(int width=176, int height=32);

	void updateButtonPositions();

	virtual void draw(const ppogl::Vec2d& position);
};

} //namepsace ppogl

#endif // _PPOGL_SCALE_H_
