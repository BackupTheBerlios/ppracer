/* 
 * PPOGL - PlanetPenguin Open Game Library
 * Copyright (C) 2005-2006 Volker Stroebel <volker@planetpenguin.de>
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
 
/*
 * calc_cpu_mhz() is based on the code from
 * libsdl (src/timer/linux/SDL_systimer.c).
 * Copyright (C) 1997-2004 Sam Lantinga
 */
 
#include "perftest.h"

#ifdef PPOGL_PERFTEST_RDTSC

#include <unistd.h>
#include <sys/time.h>
#include <iostream>

namespace ppogl{
float perftestCpuSpeed = 0.0f;

/**
 * Calculate clock speed
 */
static float
calc_cpu_mhz()
{
	float cpu_mhz;
	unsigned long long tsc_start;
	unsigned long long tsc_end;
	struct timeval tv_start, tv_end;
	long usec_delay;

	pp_rdtsc(tsc_start);
	gettimeofday(&tv_start, NULL);
	sleep(1);
	pp_rdtsc(tsc_end);
	gettimeofday(&tv_end, NULL);
	usec_delay = 1000000L * (tv_end.tv_sec - tv_start.tv_sec) +
	                        (tv_end.tv_usec - tv_start.tv_usec);
	cpu_mhz = (float)(tsc_end-tsc_start) / usec_delay;

	return cpu_mhz;
}
	
void
initPerftest()
{
	perftestCpuSpeed = calc_cpu_mhz();
	std::cout << "Initialize performance test:\n\tCPU MHz: " << perftestCpuSpeed << std::endl;
}

} // namespace ppogl;

#endif
