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

#ifndef _ELEMENTS_H_
#define _ELEMENTS_H_


#include "ppogl/models/model.h"
#include "ppogl/base/vec3d.h"
#include "ppogl/textures.h"

#include <list>

class ItemType
 : public ppogl::RefObject
{
public:
	enum Type{
		UNCOLLECTABLE,
		HERRING,
		LIFE	
	};
	
	ItemType();
	
	bool operator!=(ItemType& _type){return this!=&_type;};

    ppogl::TextureRef texture;
    double diam, height;
    double above_ground;
    ppogl::Color4c color;
	ItemType::Type type;
	int score;
	ppogl::Vec3d normal;
    bool reset_point;
    bool use_normal;
};

class Item
{
	ppogl::RefPtr<ItemType> m_type;
	ppogl::Vec3d m_position;
	ppogl::Vec3d m_normal;
	double m_diameter, m_height;
	bool m_drawable;
	bool m_collected;
    bool m_useNormal;

public:
	Item(ppogl::RefPtr<ItemType> type, const ppogl::Vec3d& position);
		
	inline bool isDrawable(){ return m_drawable; };
	inline void setDrawable(const bool state=true){ m_drawable=state; };

	inline int getScore(){return m_type->score;}
	inline void setScore(const int score){m_type->score=score;}
	
	inline ppogl::RefPtr<ItemType> getItemType(){return m_type;};
	
	inline ItemType::Type getType(){return m_type->type;};
	inline void setType(ItemType::Type type){m_type->type=type;};
	
	inline void setCollected(bool collected=true){m_collected=collected;};
	inline bool isCollected(){return m_collected;};
	
	inline bool isCollectable(){return m_type->type!=ItemType::UNCOLLECTABLE;};
	
	inline ppogl::Vec3d& getPosition(){return m_position;};	
	
	inline double getDiameter(){return m_diameter;};
	inline void setDiameter(const double diameter){m_diameter=diameter;};
	
    inline double getHeight(){return m_height;};
	inline void setHeight(const double height){m_height=height;};

	
	void draw(ppogl::Vec3d normal);
	void setNormal(const ppogl::Vec3d& normal)
	{
		m_normal = normal;
		m_useNormal = true;
	}
};

class ModelType
 : public ppogl::RefObject
{
protected:
	ppogl::ModelRef m_model;
	
public:	
	ModelType();
	~ModelType();
		
	bool operator!=(const ModelType& type)const{return this!=&type;}
	
	double height;
	double diameter;
    ppogl::Color4c color;
	ppogl::Polyhedron* ph;
	
	void setModel(ppogl::ModelRef model);
	inline ppogl::ModelRef getModel(){return m_model;}
};

class Model
{
	ppogl::RefPtr<ModelType> m_type;
	ppogl::Vec3d m_position;
	ppogl::Vec3d m_scale;
	double m_angle;
	ppogl::Vec3d m_rotation;	
	
public:
	Model(ppogl::RefPtr<ModelType> type, const ppogl::Vec3d& position):m_type(type), m_position(position), m_angle(0.0){}
	
	inline ppogl::Polyhedron* getPolyhedron(){return m_type->ph;}
	inline ppogl::Vec3d& getPosition(){return m_position;}
	inline ppogl::RefPtr<ModelType> getType(){return m_type;}
	inline double getHeight(){return m_type->height;}
	inline double getDiameter(){return m_type->diameter;}

	inline void setScale(const ppogl::Vec3d& scale){m_scale=scale;}
	inline void setRotation(double angle, const ppogl::Vec3d& rotation){m_angle=angle; m_rotation=rotation;}
	void mirror();
	
	void draw(const ppogl::Vec3d& normal);
};

//Global instances of model/item/resetpoint locations

extern std::list<Model> modelLocs;
extern std::list<Item> itemLocs;
extern std::list<ppogl::Vec2d> resetLocs;

#endif // _ELEMENTS_H_
