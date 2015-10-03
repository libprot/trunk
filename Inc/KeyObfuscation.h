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

#ifndef __KEY_OBFUSCATION__
#define __KEY_OBFUSCATION__

class CKeyObfuscation {
public:
	CKeyObfuscation() : iValuesPerLine(25) {
		generateObfuscationKeys();
	}

	CKeyObfuscation(const std::string& hexStart, const std::string& newline, const std::string& separator, int valuesPerLine) :
		hex2Str(hexStart, newline, separator, valuesPerLine)	
	{
		CKeyObfuscation();
	}

	// generate new public and private key for obfuscation
	void generateObfuscationKeys();
	
	// return a public key that currently is used
	void getObfuscationPublicKey(char* ptrKey);

	// set a public key for obfuscation
	void setObfuscationPublicKey(const char* ptrKey);
	
	// return a size of public key
	int getObfuscationPublicKeyLenght();
	
	// return a size of private key
	int getObfuscationPrivateKeyLenght();
	
	// return a private key that currently is used
	void getObfuscationPrivateKey(char* ptrKey);

	// set a private obfuscation key
	void setObfuscationPrivateKey(const char* ptrKey);

	// set a number of bytes per line
	void setValuesPerLine(const int values);
	
	// obfuscate a string value
	std::string obfuscateKey(const std::string& str);

	// return non obfuscated string value
	std::string unobfuscateKey(const std::string& str);

private:
	int iValuesPerLine;
	static const std::string secretPhrase;
	CXorCrypt xorCrypt;
	CMerkelHellmanCrypt hellmanCrypt;
	CHex2Str hex2Str;
};

#endif