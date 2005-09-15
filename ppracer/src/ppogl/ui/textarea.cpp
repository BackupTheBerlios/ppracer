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

#include "textarea.h"

#include "uitheme.h"
#include "../base/glstuff.h"

#include <iostream>

namespace ppogl{
	
Textarea::Textarea(int width, int height)
 : m_upBtn(ppogl::Vec2d(32,16)),
   m_downBtn(ppogl::Vec2d(32,16))
{
	m_size.x()=width;
	m_size.y()=height;
	
	m_upBtn.setTexture(ppogl::UITheme::getInstance().listBoxTex);
	m_upBtn.addState(0,ppogl::Vec4f(0.0,0.25,0.5,0.5));
	m_upBtn.addState(1,ppogl::Vec4f(0.5,0.25,1.0,0.5));
	m_upBtn.addState(2,ppogl::Vec4f(0.5,0.75,1.0,1.0));
	m_upBtn.setState(0);
	m_upBtn.setHilitState(2);
	m_upBtn.setMode(ImageButton::MODE_HILIT);
	m_upBtn.signalClicked.Connect(ppogl::CreateSlot(this,&Textarea::upButtonCallback));		
	
	//mousewheel up
	signalMouseWheelUp.Connect(ppogl::CreateSlot(this,&Textarea::upButtonCallback));		

	
	m_downBtn.setTexture(ppogl::UITheme::getInstance().listBoxTex);
	m_downBtn.addState(0,ppogl::Vec4f(0.0,0.0,0.5,0.25));
	m_downBtn.addState(1,ppogl::Vec4f(0.5,0.0,1.0,0.25));
	m_downBtn.addState(2,ppogl::Vec4f(0.5,0.5,1.0,0.75));
	m_downBtn.setState(0);
	m_downBtn.setHilitState(2);
	m_downBtn.setMode(ImageButton::MODE_HILIT);
	m_downBtn.signalClicked.Connect(ppogl::CreateSlot(this,&Textarea::downButtonCallback));		
	
	//mousewheel down
	signalMouseWheelDown.Connect(ppogl::CreateSlot(this,&Textarea::downButtonCallback));		

	alignment.signalChanged.Connect(ppogl::CreateSlot(this,&Textarea::updateButtonPositions));		

	m_font = ppogl::UITheme::getInstance().textareaFont;
	
	m_top = m_lines.begin();
	updateButtonStates();
}

void
Textarea::draw(ppogl::Vec2d position)
{
	ppogl::drawRectangle(position,m_size);
	
	ppogl::drawRectangle(position+ppogl::Vec2d(4,4),
						 m_size-ppogl::Vec2d(8,8),
						 ppogl::Color(0.5, 0.6, 0.9));

	if(m_font && !m_lines.empty()){
		float desc = m_font->descender();
		float asc = m_font->ascender();
		
		ppogl::Vec2d temp = position;
		temp.x()+=7;
		temp.y()+=m_size.y()-4+(desc-asc);
				
		std::list<std::string>::iterator it;
		for(it = m_top; it!=m_lines.end();it++){	
			m_font->draw((*it),temp);
			temp.y()+=desc-asc;
			
			if(temp.y() <= position.y()+4) break;			
		}
	}
}

void
Textarea::setPosition(ppogl::Vec2d position)
{
	// set position of the widget
	Widget::setPosition(position);
	
	// update the position of the up and down button widget
	updateButtonPositions();
}

void
Textarea::setText(const std::string& text)
/// set the text for the widget (clears previous text)
{
	m_text = text;
	m_lines.clear();
	
	if(m_text.empty()){
		return;
	}
	
	std::string::size_type newline;
	std::string::size_type current;
	
	std::string substring;
	
	current = 0;
	newline = m_text.find('\n');
	
	do{
		newline = m_text.find('\n',current);
	
		substring = m_text.substr(current,newline-current);
		current = newline;
		if(current!=std::string::npos) current++;

		if(m_font->advance(substring) < m_size.x()-48){
			// the entire substring fits in the textarea
			m_lines.push_back(substring);
		}else{
			std::string::size_type begin = 0;
			std::string::size_type end = substring.length();
				
			while(begin<substring.length() && end>begin && end!=std::string::npos){
				std::string temp = substring.substr(begin,end-begin+1);				
				
				if(m_font->advance(temp) < m_size.x()-48){
					m_lines.push_back(temp);
					begin = end+2;
					end = substring.length();
				}else{
					end = substring.rfind(" ",end);
					if(end<=begin || end==std::string::npos){
						end= substring.length();
						while(begin<substring.length() && end>begin){
							temp = substring.substr(begin,end-begin+1);				
				
							if(m_font->advance(temp) < m_size.x()-64){
								m_lines.push_back(temp);
								begin = end+1;
								end = substring.length();
								break;
							}else{
								end--;
							}				
						}
					}else{
						end--;
					}
				}				
			}
		}		
	}while(current!=std::string::npos);
	
    m_top = m_lines.begin();
	updateButtonStates();
}

void
Textarea::setFont(const std::string& binding)
/// sets font for the text
{
	m_font = ppogl::FontMgr::getInstance().get(binding);
	if(!m_font){
		PP_WARNING("Unable to find font for binding: " << binding);
		m_font = ppogl::UITheme::getInstance().textareaFont;
	}		
}

void
Textarea::updateButtonStates()
/// update the state of the up and down button widgets
{
	if(m_top != m_lines.begin()){
		m_upBtn.setState(0);
		m_upBtn.setMode(ImageButton::MODE_HILIT);
	}else{
		m_upBtn.setState(1);
		m_upBtn.setMode(ImageButton::MODE_NONE);
	}
	if(m_top != --m_lines.end()){
		m_downBtn.setState(0);
		m_downBtn.setMode(ImageButton::MODE_HILIT);
	}else{
		m_downBtn.setState(1);
		m_downBtn.setMode(ImageButton::MODE_NONE);
	}
}

void
Textarea::upButtonCallback()
{
	/// move top to the previous line if possible
	if(m_top != m_lines.begin()){
		m_top--;
		updateButtonStates();
	}
}

void
Textarea::downButtonCallback()
{
	/// move top to the next line if possible
	if(m_top != --m_lines.end()){
		m_top++;
		updateButtonStates();
	}
}

void
Textarea::updateButtonPositions()
/// update the position of the up and down button widgets
{
	ppogl::Vec2d temp = alignment.alignPosition(m_position,m_size);
	
	temp.x()+=m_size.x()-34;
	temp.y()+=5;
	m_downBtn.setPosition(temp);
	
	temp.y()+=m_size.y()-26;
	m_upBtn.setPosition(temp);
}

} //namepsace ppogl
