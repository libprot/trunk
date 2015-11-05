/*
    Libprot library, v. 0.0.1. Copyright (c) 2015, Alexander Ilin

    e-mail: alexander.ilin83 (at) gmail.com  
    https://github.com/libprot/trunk

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "stdafx.h"
#include <time.h>
#include "..\inc\random.h"

CRandomGenerator::CRandomGenerator()
: m_iSeed((unsigned int)time(NULL))
{
}

int CRandomGenerator::getRandom(int maximum) {
	m_iSeed = m_iSeed * 1103515245 + 12345;
	if (maximum == 0) {
		return m_iSeed;
	}
	return m_iSeed % maximum;
}

long long CRandomGenerator::getRandom(long long maximum) {
	unsigned long long random_value = getRandom((int)0) + ((long long)getRandom((int)0) << 32);
	return random_value % maximum;
}
