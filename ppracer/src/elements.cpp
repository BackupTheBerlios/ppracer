/* 
 * PlanetPenguin Racer
 * Copyright (C) 2005 Volker Stroebel <volker@planetpenguin.de>
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

#include "elements.h"

#include "ppogl/base/glwrappers.h"


Item::Item(ppogl::RefPtr<ItemType> type, const ppogl::Vec3d position)
 : m_type(type),
   m_position(position),
   m_drawable(true),
   m_collected(false)
{
	m_useNormal=type->use_normal;
	m_normal = type->normal;
	m_diameter = type->diam;
	m_height = type->height;	
}

void
Item::draw(ppogl::Vec3d normal)
{
	gl::PushMatrix();
	{    
		PP_ASSERT(m_type->texture,"Unable to load texture for item type");
	    gl::BindTexture( GL_TEXTURE_2D, m_type->texture);
		
		gl::Translate(m_position);

		if(m_useNormal){
			normal = m_normal;
		}
			
		gl::Normal(normal);
	
		const double radius = m_diameter/2;
		const double &height = m_height;
		
		gl::Begin( GL_QUADS );
		{
			gl::TexCoord( 0., 0. );
			gl::Vertex( -radius*normal.z(), 
			    0.0, 
			    radius*normal.x() );
			gl::TexCoord( 1., 0. );
			gl::Vertex( radius*normal.z(), 
		 	   0.0, 
			    -radius*normal.x() );
			gl::TexCoord( 1., 1. );
			gl::Vertex( radius*normal.z(), 
			    height, 
			    -radius*normal.x() );
			gl::TexCoord( 0., 1. );
			gl::Vertex( -radius*normal.z(), 
				height, 
				radius*normal.x() );
	   	}
		gl::End();
	}	
    gl::PopMatrix();
}

void
Item::setNormal(ppogl::Vec3d normal)
{
	m_normal = normal;
	m_useNormal = true;
}

void
Model::draw(const ppogl::Vec3d& normal)
{
	PP_REQUIRE(m_type->getModel(),"The ModelRef for this ModelType is not initialized");
	gl::PushMatrix();
	{    
		gl::Translate(m_position);
		gl::Normal(normal);	
		m_type->getModel()->draw();
	}	
    gl::PopMatrix();
}

ModelType::~ModelType()
{
}

void
ModelType::setModel(ppogl::ModelRef model)
{
	PP_REQUIRE(model,"ModelRef is not initialized"); 
	
	if(model){	
		m_model = model;
		ph = model->getPolyhedron();	
	}
}


//locations of the models and items in the current course
std::list<Model> modelLocs;
std::list<Item> itemLocs;
std::list<ppogl::Vec2d> resetLocs;
