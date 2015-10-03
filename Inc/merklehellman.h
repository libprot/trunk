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

#ifndef __Merkel_Hellman__
#define __Merkel_Hellman__

#include "random.h"

class CMerkelHellmanCrypt {
	public:
		unsigned int byteCrypt(unsigned char input) const;
		unsigned char byteDecrypt(unsigned int input) const;
		int keyGeneration();
		int getPublicKeyLenght() const;
		int getPrivateKeyLenght() const;
		void getPublicKey(char *key) const;
		void getPrivateKey(char *key) const;
		void setPublicKey(const char *key);
		void setPrivateKey(const char* key);
	
	protected:
		int getModularInverse(int a, int m) const;
		void calculateVectorB();
		void calculateQ();
		void calculateVectorW();
		void calculateR_();
		bool IsCoprime(unsigned int r, unsigned int q) const;

	private:
		const static int BLOCK_SIZE = 8;
		const static int MAX_NUM = 0x100;
		unsigned int w[BLOCK_SIZE]; 
		unsigned int b[BLOCK_SIZE];
		unsigned int r, q;
		CRandomGenerator m_rndGen;
};

#endif