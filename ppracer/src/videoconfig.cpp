/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2006 Volker Stroebel <volker@planetpenguin.de>
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
 
/*
 * Note:
 * Perhaps we should combine resolution and bpps in one list.
 * The only problem i see here is that the videomode autodetection
 * cannot detect what resolutions are supported in different 
 * bpp modes.
 *
 * Multisamples:
 * This is currently a little bit broken.
 * There is no test whether the hardware supports this and
 * the list of available multisamples is static. 
 */
 
#include "videoconfig.h"

#include "winsys.h"
#include "stuff.h"
#include "ppogl/base/defs.h"

#include <sstream>

#define RES_MIN_WIDTH 600
#define RES_MIN_HEIGHT 400

VideoConfig::VideoConfig()
 : ConfigMode(_("Video Configuration")),
   m_resolutionLbl(_("Resolution:")),
   m_bppLbl(_("Bits Per Pixel:")),
   m_multisampleLbl(_("FSAA Multisamples:")),
   m_fullscreenLbl(_("Fullscreen:")),
   m_stencilLbl(_("Stencil Buffer:")),
   m_fsaaLbl(_("Enable FSAA:")),
   m_warningLbl(_("Experimental (needs restart)"))
{
	m_bppListBox.addElement("Display", 0);
	m_bppListBox.addElement("16", 1);
	m_bppListBox.addElement("32", 2);
	m_bppListBox.setSelectedData(PPConfig.getInt("bpp_mode"));
	
	m_multisampleListBox.addElement("1", 1);
	m_multisampleListBox.addElement("2", 2);
	m_multisampleListBox.addElement("4", 4);
	if(!m_multisampleListBox.setSelectedData(PPConfig.getInt("multisamples"))){
		char buff[4];
		snprintf(buff,4,"%d",PPConfig.getInt("multisamples"));		
		m_multisampleListBox.addElement(buff, PPConfig.getInt("multisamples"));
		m_multisampleListBox.setSelectedData(PPConfig.getInt("multisamples"));
	}	
	
	initVideoModes();
	
	{
		std::stringstream temp(std::ios::in| std::ios::out | std::ios::trunc);
		temp << resolution.x() 
		     << " x "
		     << resolution.y();

		if(!m_resolutionListBox.setSelected(temp.str())){
			m_resolutionListBox.addElement(temp.str(), Resolution(resolution));
			m_resolutionListBox.setSelected(temp.str());	
		}		
	}
		
	ppogl::Vec2d position(40,350);
	ppogl::Vec2d position2(600,350);
	
	m_resolutionLbl.setPosition(position);
	m_resolutionListBox.setPosition(position2);
	m_resolutionListBox.alignment.right();
	
	position.y()-=40;
	position2.y()-=40;
	
	m_bppLbl.setPosition(position);
	m_bppListBox.setPosition(position2);
	m_bppListBox.alignment.right();	
			
	position.y()-=40;
	position2.y()-=40;
	
	m_fullscreenLbl.setPosition(position);
	m_fullscreenBox.setPosition(position2);
	m_fullscreenBox.alignment.right();
	m_fullscreenBox.setSelected(PPConfig.getBool("fullscreen"));
	
	position.y()-=80;
	position2.y()-=80;
	
	m_warningLbl.setPosition(position);
		
	position.y()-=40;
	position2.y()-=40;
	
	m_stencilLbl.setPosition(position);
	m_stencilBox.setPosition(position2);
	m_stencilBox.alignment.right();
	m_stencilBox.setSelected(PPConfig.getBool("stencil_buffer"));	
	
	position.y()-=40;
	position2.y()-=40;
	
	m_fsaaLbl.setPosition(position);
	m_fsaaBox.setPosition(position2);
	m_fsaaBox.alignment.right();
	m_fsaaBox.setSelected(PPConfig.getBool("enable_fsaa"));
		
	position.y()-=40;
	position2.y()-=40;
	
	m_multisampleLbl.setPosition(position);
	m_multisampleListBox.setPosition(position2);
	m_multisampleListBox.alignment.right();
}

void
VideoConfig::apply()
{
	bool updatevideo = false;
	
	ppogl::ListBox<Resolution>::Iterator resit = m_resolutionListBox.getSelected();
	ppogl::ListBox<int>::Iterator bppit = m_bppListBox.getSelected();
	ppogl::ListBox<int>::Iterator multiit = m_multisampleListBox.getSelected();
		
	if ( (*resit).data.x != resolution.x() ){
		PPConfig.setInt("x_resolution",(*resit).data.x);
		PPConfig.setInt("y_resolution",(*resit).data.y);
		updatevideo=true;	
	}
	
	if ( (*bppit).data != PPConfig.getInt("bpp_mode")){
		PPConfig.setInt("bpp_mode",(*bppit).data);
		updatevideo=true;	
	}
	
	if (m_fullscreenBox.isSelected() != PPConfig.getBool("fullscreen")){
		PPConfig.setBool("fullscreen",m_fullscreenBox.isSelected());
		updatevideo=true;
	}
	
	if (m_stencilBox.isSelected() != PPConfig.getBool("stencil_buffer")){
		PPConfig.setBool("stencil_buffer",m_stencilBox.isSelected());
		updatevideo=true;
	}
	
	if (m_fsaaBox.isSelected() != PPConfig.getBool("enable_fsaa")){
		PPConfig.setBool("enable_fsaa",m_fsaaBox.isSelected());
		updatevideo=true;
	}
	
	if ( (*multiit).data != PPConfig.getInt("multisamples")){
		PPConfig.setInt("multisamples",(*multiit).data);
		updatevideo=true;	
	}
	
	
 	if (updatevideo==true){
		printf("Set new videomode:%dx%d bpp:%d \n", (*resit).data.x, (*resit).data.y, (*bppit).data);
		setup_sdl_video_mode();
	}	
	
	write_config_file();
	
	setMode( GameMode::prevmode );
}

void
VideoConfig::initVideoModes()
/** 
 * fills m_resolutionList with modes 
 * by trying to autodetect the available modes.
 * If the autodetection fails, standard modes are used.
 */
{
	SDL_Rect **modes;

	// get available fullscreen OpenGL modes
	if(!PPConfig.getBool("disable_videomode_autodetection")){	
		modes=SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_OPENGL);
	}else{
		modes=reinterpret_cast<SDL_Rect **>(-1);
	}
	// check if we cannot receive the modes
	if(	modes == reinterpret_cast<SDL_Rect **>(-1) ||
		modes == reinterpret_cast<SDL_Rect **>(0) )
	{
		// unable to find modes... fall back to standard modes
		m_resolutionListBox.addElement("1280 x 1024", Resolution(1280,1024));
		m_resolutionListBox.addElement("1152 x 864", Resolution(1152,864));
		m_resolutionListBox.addElement("1024 x 768", Resolution(1024,768));
		m_resolutionListBox.addElement("800 x 600", Resolution(800,600));
		m_resolutionListBox.addElement("640 x 480", Resolution(640,480));

		PP_WARNING("No standard modes detected");
	}else{
		std::list<std::string> registered;	
		
		// fill list with modes
		for(int i=0; modes[i]; ++i){
			if(	modes[i]->w < RES_MIN_WIDTH || 
				modes[i]->w < RES_MIN_HEIGHT)
			{
				// resolution is too small	
				continue;
			}			
			
			std::stringstream temp(std::ios::in| std::ios::out | std::ios::trunc);
			temp << modes[i]->w 
			     << " x "
			     << modes[i]->h;
			
			
			std::list<std::string>::iterator it;	
			bool found=false;
			
			for(it=registered.begin(); it!=registered.end(); it++){
				if((*it)==temp.str()){
					found=true;
					break;
				}
			}
			
			if(found){
				// mode already registered
				continue;				
			}else{
				//new mode
				registered.push_back(temp.str());
				m_resolutionListBox.addElement(temp.str(), Resolution(modes[i]->w,modes[i]->h));
			}				
		}
	}
}
