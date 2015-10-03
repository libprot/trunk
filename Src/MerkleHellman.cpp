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

#include "..\inc\MerkleHellman.h"
#include <algorithm>

unsigned int CMerkelHellmanCrypt::byteCrypt(unsigned char input) const {
	unsigned int result = 0;
	for (int i = 0; i < BLOCK_SIZE; i++) {
		if ((input & (1 << i)) != 0) {
			result += b[i];
		}
	}
	return result;
}

int CMerkelHellmanCrypt::getModularInverse(int a, int m) const {
	
	int result = 1;
	if (m == 1) {
		return result;
	}
	if (m == 0) {
		return -1;
	}
	int offset = m;
	for (int p = 0; a > 1; ) {
		if (m == 0) {
			return -1;
		}
		int q = a / m;
		a = a % m;
		std::swap(a, m);

		result -= q *p;
		std::swap(result, p);
	}
	return result < 0 ? result + offset : result;
}

unsigned char CMerkelHellmanCrypt::byteDecrypt(unsigned int input) const
{
	unsigned char result = 0;
	input = (unsigned int) (((long long)input * r) % q);

	for (int i = 7; i >= 0; i--) {
		if (input >= w[i]) {
			input -= w[i];
			result |= 1 << i;
		}
	}

	return result;
}

bool CMerkelHellmanCrypt::IsCoprime(unsigned int r, unsigned int q) const
{
	if (r > q || r < 1) { // r should be in range [1, q)
		return false;
	} 

	for (unsigned int i = 0; q != 0; r = q, q = i) {
		i = r % q;
	}

	return r == 1;
}

void CMerkelHellmanCrypt::calculateVectorB() {
	for (int i = 0; i < BLOCK_SIZE; i++) {
		b[i] = (r * w[i]) % q; 
	}
}

void CMerkelHellmanCrypt::calculateQ() {
	int q;
	do {
		q = 1 + this->q +  m_rndGen.getRandom(MAX_NUM);
		r = (q + 1 + m_rndGen.getRandom(MAX_NUM)) / 2;
	} while (!IsCoprime(r, q));
	this->q = q;
}

void CMerkelHellmanCrypt::calculateVectorW() {
	q = 0;
	for (int i = 0; i < BLOCK_SIZE; i++) {
		w[i] = 0;
		for (int j = 0; j < i; j++) {
			w[i] += w[j];
		}
		w[i] += 1 + m_rndGen.getRandom(MAX_NUM);
		q += w[i];
	}
}

int CMerkelHellmanCrypt::getPublicKeyLenght() const {
	return sizeof(b); 
}

int CMerkelHellmanCrypt::getPrivateKeyLenght() const {
	return sizeof(w) + sizeof(q) + sizeof(r);
}

void CMerkelHellmanCrypt::getPublicKey(char *key) const {
	memcpy(key, &b[0], sizeof(b));
}

void CMerkelHellmanCrypt::getPrivateKey(char *key) const {
	memcpy(key, &w[0], sizeof(w));
	memcpy(key + sizeof(w), &q, sizeof(q));
	memcpy(key + sizeof(w) + sizeof(q), &r, sizeof(r));
}

void CMerkelHellmanCrypt::setPublicKey(const char *key) {
	memcpy(&b[0], key, sizeof(b));
}

void CMerkelHellmanCrypt::setPrivateKey(const char* key) {
	memcpy(&w[0], key, sizeof(w));
	memcpy(&q, key + sizeof(w), sizeof(q));
	memcpy(&r, key + sizeof(w) + sizeof(q), sizeof(r));
}

void CMerkelHellmanCrypt::calculateR_() {
	r = getModularInverse(r, q);
}

int CMerkelHellmanCrypt::keyGeneration() {
	calculateVectorW();
	calculateQ();
	calculateVectorB();
	calculateR_();
	return 0;
}