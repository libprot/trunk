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

#include "../inc/KeyObfuscation.h"

const std::string CKeyObfuscation::secretPhrase = "the pass phrase is secret";

void generateObfuscationKeys() {
	hellmanCrypt.keyGeneration();
}
	
void getObfuscationPublicKey(char* ptrKey) {
	hellmanCrypt.getPublicKey(ptrKey);
}

void setObfuscationPublicKey(const char* ptrKey) {
	hellmanCrypt.setPublicKey(ptrKey);
}
	
int getObfuscationPublicKeyLenght() {
	return hellmanCrypt.getPublicKeyLenght();
}
	
int getObfuscationPrivateKeyLenght() {
	return hellmanCrypt.getPrivateKeyLenght();
}
	
void getObfuscationPrivateKey(char* ptrKey) {
	hellmanCrypt.getPrivateKey(ptrKey);
}

void setObfuscationPrivateKey(const char* ptrKey) {
	hellmanCrypt.setPrivateKey(ptrKey);
}

void setValuesPerLine(const int values) {
	iValuesPerLine = values;
}
	
std::string obfuscateKey(const std::string& str) {
	std::vector<unsigned int> crypted;
	const char* buffStr = str.c_str();
	for (size_t i = 0; i < str.size(); i++) {
		const unsigned int charCrypted = hellmanCrypt.byteCrypt(buffStr[i]);
		crypted.push_back(charCrypted);
	}
	char* ptrData = (char*)&(*crypted.begin());
	xorCrypt.setKey((void *)secretPhrase.c_str(), secretPhrase.size());
	xorCrypt.crypt(ptrData, crypted.size() * sizeof(unsigned int), 0);
	std::string strOut;
	hex2Str.setHexStart("");
	hex2Str.setSeparator("");
	hex2Str.setValuesPerLine(iValuesPerLine);
	hex2Str.data2str(strOut, (unsigned char*)ptrData, crypted.size() * sizeof(unsigned int));
	return strOut;
}

std::string unobfuscateKey(const std::string& str) {
	const int dataLen = hex2Str.getDataLength(str);
	unsigned char *outData = new unsigned char[dataLen];
	hex2Str.string2data(str, outData);
	xorCrypt.crypt(outData, dataLen, 0);
	std::vector<char> decryptedChars;
	for (size_t i = 0; i < dataLen / sizeof(int); i++) {
		const unsigned int charCrypted = *(int *)&outData[i * sizeof(int)];
		unsigned char c = hellmanCrypt.byteDecrypt(charCrypted);
		decryptedChars.push_back(c);
	}
	delete [] outData;
	decryptedChars.push_back(0);
	std::string result(&*decryptedChars.begin());
	return result;
} 
