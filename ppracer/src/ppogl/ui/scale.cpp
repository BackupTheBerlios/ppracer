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

#include "scale.h"

#include "uitheme.h"
#include "../base/glstuff.h"

namespace ppogl{
	

Scale::Scale(int width, int height)
 : Widget(),
   m_upBtn(ppogl::Vec2d(16,32)),
   m_downBtn(ppogl::Vec2d(16,32)),
   m_value(0.0f),
   m_step(0.1f)
{
	m_size.x()=width;
	m_size.y()=height;
	
	m_upBtn.signalClicked.Connect(ppogl::CreateSlot(this,&Scale::upButtonCallback));		
	//mousewheel up
	signalMouseWheelUp.Connect(ppogl::CreateSlot(this,&Scale::upButtonCallback));		

	m_downBtn.signalClicked.Connect(ppogl::CreateSlot(this,&Scale::downButtonCallback));		
	//mousewheel down
	signalMouseWheelDown.Connect(ppogl::CreateSlot(this,&Scale::downButtonCallback));		

	alignment.signalChanged.Connect(ppogl::CreateSlot(this,&Scale::updateButtonPositions));
}	

void
Scale::setPosition(const ppogl::Vec2d& position)
{
	// set position of the widget
	Widget::setPosition(position);
	
	// update the position of the up and down button widget
	updateButtonPositions();
}

void
Scale::setValue(float value)
{
	if(value<0.0f){
		m_value=0.0f;
	}else if(value>1.0f){
		m_value=1.0f;
	}else{
		m_value = value;
	}
	updateButtonStates();
}

float
Scale::getValue()
{
	return m_value;
}

void
Scale::setStep(float step)
{
	PP_REQUIRE(step<1.0f,"Step must not be grater than 1.0: " << step);
	m_step=step;	
}

void
Scale::updateButtonStates()
///update the state of the up and down button widgets
{
	if(m_value <1.0f){
		m_upBtn.setState(0);
		m_upBtn.setMode(ImageButton::MODE_HILIT);
	}else{
		m_upBtn.setState(1);
		m_upBtn.setMode(ImageButton::MODE_NONE);
	}
	
	if(m_value >0.0f){
		m_downBtn.setState(0);
		m_downBtn.setMode(ImageButton::MODE_HILIT);
	}else{
		m_downBtn.setState(1);
		m_downBtn.setMode(ImageButton::MODE_NONE);
	}	
}

void
Scale::upButtonCallback()
{
	m_value+=m_step;
		
	//ensure m_value is valid
	if(m_value>1.0f){
		m_value=1.0f;
	}
	updateButtonStates();
}	

void
Scale::downButtonCallback()
{
	m_value-=m_step;
		
	//ensure m_value is valid
	if(m_value<0.0f){
		m_value=0.0f;
	}
	updateButtonStates();
}
		

HScale::HScale(int width, int height)
 : Scale(width,height)
{

	m_upBtn.setTexture(ppogl::UITheme::getInstance().hScaleTex);
	m_upBtn.addState(0,ppogl::Vec4f(0.75,0.5,1.0,1.0));
	m_upBtn.addState(1,ppogl::Vec4f(0.75,0.0,1.0,0.5));
	m_upBtn.addState(2,ppogl::Vec4f(0.25,0.0,0.5,0.5));
	m_upBtn.setState(0);
	m_upBtn.setHilitState(2);
	m_upBtn.setMode(ImageButton::MODE_HILIT);

	m_downBtn.setTexture(ppogl::UITheme::getInstance().hScaleTex);
	m_downBtn.addState(0,ppogl::Vec4f(0.5,0.5,0.75,1.0));
	m_downBtn.addState(1,ppogl::Vec4f(0.5,0.0,0.75,0.5));
	m_downBtn.addState(2,ppogl::Vec4f(0.0,0.0,0.25,0.5));
	m_downBtn.setState(0);
	m_downBtn.setHilitState(2);
	m_downBtn.setMode(ImageButton::MODE_HILIT);	
	
	updateButtonStates();
}

void
HScale::updateButtonPositions()
///update the position of the up and down button widgets
{
	ppogl::Vec2d temp = alignment.alignPosition(m_position,m_size);
	
	m_downBtn.setPosition(temp);
	
	temp.x()+=m_size.x()-16;
	m_upBtn.setPosition(temp);
}

void
HScale::draw(const ppogl::Vec2d& position)
{
	ppogl::Vec2d temp = position;
	ppogl::Vec2d size = m_size;
	
	temp.x()+=14;
	temp.y()+=m_size.y()/2;

	size.x()-=24;
	size.y()=2;
	
	ppogl::drawRectangle(temp,size,ppogl::Color(1.0,1.0,1.0,0.8));

	temp.y()-=7;
	
	float slider = size.x()-28;
	temp.x()+= 4 + slider*m_value;
	
	ppogl::drawRectangle(temp,ppogl::Vec2d(16,16),ppogl::Color::white);	
}

} //namepsace ppogl
