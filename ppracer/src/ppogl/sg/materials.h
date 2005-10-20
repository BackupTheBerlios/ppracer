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

#ifndef _PPOGL_SG_MATERIALS_H_
#define _PPOGL_SG_MATERIALS_H_

#include "../base/vec2d.h"
#include "../base/color.h"

namespace ppogl{

class UV : public ppogl::Vec2d
{
public:
	double& u(){return values[0];};
	double& v(){return values[1];};
	
	double u() const {return values[0];};
	double v() const {return values[1];};
};

class Material
{
private:
	float m_shininess;
	float m_transparency;
	float m_specularExponent;

public:
	Material();

	ppogl::Color diffuse; 
	ppogl::Color ambient;
	ppogl::Color specular;
	ppogl::Color emissive;

	void setShininess(const float& shininess){m_shininess=shininess;}
	void setTransparency(const float& transparency){m_transparency=transparency;}
	void setSpecularExponent(const float& specularExponent ){m_specularExponent=specularExponent;}

	const float& getShininess(){return m_shininess;}
	const float& getTransparency(){return m_transparency;}
	const float& getSpecularExponent(){return m_specularExponent;}
};

} //namepsace ppogl

#endif // _PPOGL_SG_MATERIALS_H_
