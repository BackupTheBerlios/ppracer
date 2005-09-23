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

#ifndef _PPOGL_LISTBOX_H_
#define _PPOGL_LISTBOX_H_

#include "widget.h"

#include "../base/glstuff.h"
#include "../base/signal.h"
#include "imagebutton.h"
#include "uitheme.h"

#include <string>
#include <list>

namespace ppogl{
	
/// template class for a listbox widget
template <class T> class ListBox
 : public Widget
{
private:
	struct Element
	{
		Element(std::string _label, T _data):label(_label),data(_data){};
		std::string label;
		T data;
	};
	
protected:

	typename std::list<Element> m_elements;
	typename std::list<Element>::iterator m_it;

	ppogl::ImageButton m_upBtn;
	ppogl::ImageButton m_downBtn;

	void updateButtonStates();

public:
	ListBox(int width=176, int height=32);

	typedef typename std::list<Element>::iterator iterator;

    virtual void draw(const ppogl::Vec2d& position);

	typename std::list<Element>::iterator addElement(const std::string& label, T data);

	void setSelected(typename std::list<Element>::iterator it);
	bool setSelected(const std::string& label);
	bool setSelectedData(T data);

	typename std::list<Element>::iterator& getSelected();
	T getSelectedData();

	void setPosition(const ppogl::Vec2d& position);

	void upButtonCallback();
	void downButtonCallback();
	void updateButtonPositions();

	ListBox<T>& operator++();
	ListBox<T>& operator--();

	//signals
	ppogl::Signal0 signalChanged;
};

template <class T>
ListBox<T>::ListBox(int width, int height)
 : Widget(),
   m_upBtn(ppogl::Vec2d(32,16)),
   m_downBtn(ppogl::Vec2d(32,16))
{
	m_size.x()=width;
	m_size.y()=height;
	m_it = m_elements.end();	
	
	m_upBtn.setTexture(ppogl::UITheme::getInstance().listBoxTex);
	m_upBtn.addState(0,ppogl::Vec4f(0.0,0.25,0.5,0.5));
	m_upBtn.addState(1,ppogl::Vec4f(0.5,0.25,1.0,0.5));
	m_upBtn.addState(2,ppogl::Vec4f(0.5,0.75,1.0,1.0));
	m_upBtn.setState(0);
	m_upBtn.setHilitState(2);
	m_upBtn.setMode(ImageButton::MODE_HILIT);
	m_upBtn.signalClicked.Connect(ppogl::CreateSlot(this,&ListBox<T>::upButtonCallback));		
	
	//mousewheel up
	signalMouseWheelUp.Connect(ppogl::CreateSlot(this,&ListBox<T>::upButtonCallback));		

	m_downBtn.setTexture(ppogl::UITheme::getInstance().listBoxTex);
	m_downBtn.addState(0,ppogl::Vec4f(0.0,0.0,0.5,0.25));
	m_downBtn.addState(1,ppogl::Vec4f(0.5,0.0,1.0,0.25));
	m_downBtn.addState(2,ppogl::Vec4f(0.5,0.5,1.0,0.75));
	m_downBtn.setState(0);
	m_downBtn.setHilitState(2);
	m_downBtn.setMode(ImageButton::MODE_HILIT);
	m_downBtn.signalClicked.Connect(ppogl::CreateSlot(this,&ListBox<T>::downButtonCallback));		
	
	//mousewheel down
	signalMouseWheelDown.Connect(ppogl::CreateSlot(this,&ListBox<T>::downButtonCallback));		

	alignment.signalChanged.Connect(ppogl::CreateSlot(this,&ListBox<T>::updateButtonPositions));		
}

template <class T>	
void
ListBox<T>::setPosition(const ppogl::Vec2d& position)
{
	// set position of the widget
	Widget::setPosition(position);
	
	// update the position of the up and down button widget
	updateButtonPositions();
}

template <class T>	
typename std::list< typename ListBox<T>::Element>::iterator
ListBox<T>::addElement(const std::string& label, T data)
/// add new element
{
	bool first = m_elements.empty();	
	m_elements.push_back(typename ListBox<T>::Element(label,data));
	if(first){
		m_it = --m_elements.end();
	}	
	return --m_elements.end();
}

template <class T>	
void
ListBox<T>::setSelected(typename std::list<ListBox<T>::Element>::iterator it)
///set the selected element
{
	m_it = it;
}

template <class T>	
bool
ListBox<T>::setSelected(const std::string& label)
///select the elment with the corresponding label
{
	typename std::list<typename ListBox<T>::Element>::iterator it;
	
	for(it=m_elements.begin(); it!=m_elements.end(); it++){
		if((*it).label == label){
			m_it=it;
			return true;			
		}	
	}
	
	PP_PEDANTIC("Unable to find label in ListBox: " << label);
	return false;
}

template <class T>	
bool
ListBox<T>::setSelectedData(T data)
///select elment containing the corresponding data
{
	typename std::list< typename ListBox<T>::Element>::iterator it;

	for(it=m_elements.begin(); it!=m_elements.end(); it++){
		if((*it).data==data){
			m_it=it;
			return true;
		}	
	}
	PP_PEDANTIC("Unable to find data in ListBox");
	return false;
}

template <class T>
typename std::list< typename ListBox<T>::Element>::iterator&
ListBox<T>::getSelected()
///return iterator for the selected element
{
	return m_it;
}

template <class T>
T 
ListBox<T>::getSelectedData()
///return data of the selected element
{
	return (*m_it).data;
}
  
template <class T> 
void
ListBox<T>::draw(const ppogl::Vec2d& pos)
{
	updateButtonStates();
	
	ppogl::Vec2d temp = m_size;
	ppogl::Vec2d position = pos;

	temp.x()-=32;
	
	if(m_hasFocus){
		ppogl::drawRectangle(position,temp,ppogl::Color(1.0, 0.9, 0.0));
	}else{
		ppogl::drawRectangle(position,temp);
	}
	
	position.x()+=4;
	position.y()+=4;
	temp.x()-=8;
	temp.y()-=8;
	
	ppogl::drawRectangle(position,temp,ppogl::Color(0.5, 0.6, 0.9));

	position.x()+=3;
	position.y()+=5;
	
	if(!m_elements.empty() && m_it!=m_elements.end()){	
		ppogl::UITheme::getInstance().listBoxFont->draw((*m_it).label,position);
	}
}

template <class T> 
void
ListBox<T>::upButtonCallback()
{
	if(m_it!=m_elements.begin()){
		m_it--;
		signalChanged.Emit();
	}
}	

template <class T> 
void
ListBox<T>::downButtonCallback()
{
	if(m_it!= --m_elements.end()){
		m_it++;
		signalChanged.Emit();
	}
}

template <class T> 
void
ListBox<T>::updateButtonStates()
///update the state of the up and down button widgets
{
	if(m_it != m_elements.begin()){
		m_upBtn.setState(0);
		m_upBtn.setMode(ImageButton::MODE_HILIT);
	}else{
		m_upBtn.setState(1);
		m_upBtn.setMode(ImageButton::MODE_NONE);
	}
	
	if(m_it != --m_elements.end()){
		m_downBtn.setState(0);
		m_downBtn.setMode(ImageButton::MODE_HILIT);
	}else{
		m_downBtn.setState(1);
		m_downBtn.setMode(ImageButton::MODE_NONE);
	}	
}

template <class T> 
void
ListBox<T>::updateButtonPositions()
///update the position of the up and down button widgets
{
	ppogl::Vec2d temp = alignment.alignPosition(m_position,m_size);
	
	temp.x()+=m_size.x()-32;
	m_downBtn.setPosition(temp);
	
	temp.y()+=m_size.y()-16;
	m_upBtn.setPosition(temp);
}

template <class T> 
ListBox<T>&
ListBox<T>::operator++()
{
	if(m_it != --m_elements.end()){
		m_it++;
	}
	return *this;
}

template <class T> 
ListBox<T>&
ListBox<T>::operator--()
{
	if(m_it != m_elements.begin()){
		m_it--;
	}	
	return *this;
}

} //namepsace ppogl

#endif // _PPOGL_CHECKBOX_H_
