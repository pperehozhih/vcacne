/*
 * Copyright 2009 Michael Georgoulpoulos <mgeorgoulopoulos at gmail>
 * test.cpp - vcacheopt test program
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "vcacheopt.h"
#include <stdio.h>
#include <stdlib.h>

#define PLANE_SIZE 100
#define RANDOM_SWAPS 0

// timer routine
unsigned int GetMSec(void);

// plane index buffer generator
std::vector<int> GeneratePlane(int n);

int main(int argc, char **argv)
{
	// generate the plane
	std::vector<int> pl = GeneratePlane(PLANE_SIZE);
	int tri_count = 2 * (PLANE_SIZE-1) * (PLANE_SIZE-1);

	printf("Mesh triangle count: %d\n", tri_count);

	VertexCache vertex_cache;
	int misses = vertex_cache.GetCacheMissCount(&pl[0], tri_count);

	printf("*** Before optimization ***\n");
	printf("Cache misses\t: %d\n", misses);
	printf("ACMR\t\t: %f\n", (float)misses / (float)tri_count);

	VertexCacheOptimizer vco;
	printf("Optimizing ... \n");
	unsigned int time = GetMSec();
	VertexCacheOptimizer::Result res = vco.Optimize(&pl[0], tri_count);
	time = GetMSec() - time;
	if (res)
	{
		printf("Error\n");
		return 0;
	}

	printf("Optimized in %f seconds (%d ns/triangle)\n",
		time / 1000.0f, (time * 1000) / tri_count);

	misses = vertex_cache.GetCacheMissCount(&pl[0], tri_count);

	printf("*** After optimization ***\n");
	printf("Cache misses\t: %d\n", misses);
	printf("ACMR\t\t: %f\n", (float)misses / (float)tri_count);

	return 0;
}

std::vector<int> GeneratePlane(int n)
{
	std::vector<int> ret;

	for (int i=0; i<n-1; i++)
	{
		for (int j=0; j<n-1; j++)
		{
			int top_left = i + n * j;
			int top_right = top_left + 1;
			int bottom_left = top_left + n;
			int bottom_right = bottom_left + 1;

			ret.push_back(top_left);
			ret.push_back(top_right);
			ret.push_back(bottom_left);

			ret.push_back(top_right);
			ret.push_back(bottom_right);
			ret.push_back(bottom_left);
		}
	}

	int tri_count = (int)ret.size() / 3;

	// swap some triangles
	for (int i=0; i<RANDOM_SWAPS; i++)
	{
		int swap_a = rand() % tri_count;
		int swap_b = rand() % tri_count;

		for (int j=0; j<3; j++)
		{
			int tmp = ret[3 * swap_a + j];
			ret[3 * swap_a + j] = ret[3 * swap_b + j];
			ret[3 * swap_b + j] = tmp;
		}
	}

	return ret;
}


/*
 * Cross platform timer routines
 * Thanks to John Tsiombikas
 */

#if defined(unix) || defined(__unix__)
#include <time.h>
#include <sys/time.h>

unsigned int GetMSec(void)
{
	static struct timeval tv, first_tv;

	gettimeofday(&tv, 0);

	if(first_tv.tv_sec == 0 && first_tv.tv_usec == 0) {
		first_tv = tv;
		return 0;
	}
	return (tv.tv_sec - first_tv.tv_sec) * 1000 + (tv.tv_usec - first_tv.tv_usec) / 1000;
}

#elif defined(WIN32) || defined(__WIN32__)
#include <windows.h>

unsigned int GetMSec(void)
{
	static unsigned int first_time;

	if(!first_time) {
		first_time = timeGetTime();
		return 0;
	}
	return timeGetTime() - first_time;
}

#else
#error "unsupported platform, or detection failed"
#endif