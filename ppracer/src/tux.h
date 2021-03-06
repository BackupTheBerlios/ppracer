/* 
 * PlanetPenguin Racer 
 * Copyright (C) 2004-2006 Volker Stroebel <volker@planetpenguin.de>
 *
 * Copyright (C) 1999-2001 Jasmin F. Patry
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

#ifndef _TUX_H_
#define _TUX_H_

#include <string>

#include "ppogl/base/vec3d.h"

class Tux
{
public:
	std::string rootNode;
	std::string leftShoulderJoint;
	std::string rightShoulderJoint;
	std::string leftHipJoint;
	std::string rightHipJoint;
	std::string leftKneeJoint;
	std::string rightKneeJoint;
	std::string leftAnkleJoint;
	std::string rightAnkleJoint;
	std::string tailJoint;
	std::string neck;
	std::string head;

	void adjustJoints(	double turnFact, bool isBraking, 
						double paddling_factor, double speed,
						const ppogl::Vec3d& net_force, double jump_factor );

	void draw();

	const std::string& getRootNode(){return rootNode;}
	const std::string& getLeftShoulderJoint(){return leftShoulderJoint;}
	const std::string& getRightShoulderJoint(){return rightShoulderJoint;}
	const std::string& getLeftHipJoint(){return leftHipJoint;}
	const std::string& getRightHipJoint(){return rightHipJoint;}
	const std::string& getLeftKneeJoint(){return leftKneeJoint;}
	const std::string& getRightKneeJoint(){return rightKneeJoint;}
	const std::string& getLeftAnkleJoint(){return leftAnkleJoint ;}
	const std::string& getRightAnkleJoint(){return rightAnkleJoint;}
	const std::string& getTailJoint(){return tailJoint;}
	const std::string& getNeck(){return neck;}
	const std::string& getHead(){return head;}
};

extern Tux tux[2];

void register_tux_callbacks();

#endif // _TUX_H_
