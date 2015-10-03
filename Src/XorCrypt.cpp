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

#include "../inc/XorCrypt.h"

CXorCrypt::~CXorCrypt() {
	if (ptrKey) {
		delete ptrKey;
	}
}

void CXorCrypt::setKey(void* ptr, int keySize) {
	if (ptrKey) {
		delete ptrKey;
	}
	this->ptrKey = malloc(keySize);
	memcpy(this->ptrKey, ptr, keySize);
	this->keySize = keySize;
}

void CXorCrypt::crypt(void* ptr, int blockSize, int offset) {
	for (int i = 0; i < blockSize; i++, offset++) {
		unsigned char* key = (unsigned char*)ptrKey + (offset % keySize);
		unsigned char* buff = (unsigned char*)ptr + offset;
		*buff ^= *key;
	}
}
