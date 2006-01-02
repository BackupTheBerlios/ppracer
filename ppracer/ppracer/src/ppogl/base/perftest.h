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
 
/*
 * pp_rdtsc(t) is based on the code from
 * libsdl (src/timer/linux/SDL_systimer.c).
 * Copyright (C) 1997-2004 Sam Lantinga
 */
 
#ifndef _PPOGL_PERFTEST_H_
#define _PPOGL_PERFTEST_H_

// use rdtsc (>=Pentium) for performance tests
//#define PPOGL_PERFTEST_RDTSC

// use gettimeofday(..) for performance tests
//#define PPOGL_PERFTEST

#ifdef PPOGL_PERFTEST_RDTSC
	namespace ppogl{
		extern float perftestCpuSpeed;
		void initPerftest();
	} // namespace ppogl;
	#define pp_rdtsc(t) asm __volatile__ ("rdtsc" : "=A" (t))

	#define PP_PERFTEST_BEGIN(label) \
		unsigned long long pp_perftest##label; \
		pp_rdtsc(pp_perftest##label);

	#define PP_PERFTEST_END(label) \
		unsigned long long pp_perftest##label##2; \
		pp_rdtsc(pp_perftest##label##2); \
		std::cout << "PT RDTSC: " << #label << ": " << (pp_perftest##label##2-pp_perftest##label)/ppogl::perftestCpuSpeed << std::endl; 

#else
	
#if defined PPOGL_PERFTEST
	namespace ppogl{
		inline void initPerftest(){}
	} // namespace ppogl;

	#include <iostream>
	#include <sys/time.h>

	
	#define PP_PERFTEST_CALC(o,n) \
		(n.tv_sec  - o.tv_sec ) *1000000+ \
		(n.tv_usec - o.tv_usec) \
			
	#define PP_PERFTEST_BEGIN(label) \
		struct timeval pp_perftest##label; \
		gettimeofday(&pp_perftest##label, NULL);

	#define PP_PERFTEST_END(label) \
		struct timeval pp_perftest##label##2; \
		gettimeofday(&pp_perftest##label##2, NULL); \
		\
		std::cout << "PT: " << #label << ": " << PP_PERFTEST_CALC(pp_perftest##label,pp_perftest##label##2) << std::endl; 
		
#else
	namespace ppogl{
		inline void initPerftest(){}
	} // namespace ppogl;
	
	#define PP_PERFTEST_BEGIN(label)
	#define PP_PERFTEST_END(label)
#endif
#endif	

#endif // _PPOGL_PERFTEST_H_
