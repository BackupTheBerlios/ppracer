/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2004-2005 Peter Reichel <peter@apps4linux.org>
 *                         Volker Stroebel <volker@planetpenguin.de>
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
 
#include "signal.h"

namespace ppogl{
	
BaseArgument::BaseArgument(int cnt)
 : mCnt(cnt)
{
}

BaseSignal::BaseSignal()
 : mSlot(NULL)
{
}
	
BaseSignal::~BaseSignal()
{
	Clean();
}

void
BaseSignal::Connect(BaseSlot *slot)
{
	Clean();
	mSlot = slot;
}

} // namepsace ppogl
