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

#ifndef __HEX_2_STR__
#define __HEX_2_STR__

#include <ctype.h>
#include <string>

class CHex2Str {
private:
	// returns true if character represent hex symbol
	bool static isHex(unsigned char c);

	// skip part of string in base string
	bool static skipString(size_t& index, const std::string& str, const std::string& part);

	// transforms hex symbol into a number
	int static hex2Num(unsigned char c);

	// transforms a number into hex form
	unsigned char static num2hex(int num);

	std::string separator; // how to separate the hex values
	std::string newline; // characters for new line
	std::string hexStart; // characters before hex value start
	int valuesPerLine; // a number of values per line, -1 = infinite

public:
	CHex2Str(); // default constructor

	CHex2Str(const std::string& hex, const std::string& nl, const std::string& sep, int values);

	// adds to the string the characters that represent hex number
	void static byte2hex(unsigned char c, std::string& str);
	
	// returns a character from a string at specified offset 
	unsigned char static hex2byte(const std::string& str, size_t offset);
	
	// decode a hex string into an array on chars
	int string2data(const std::string& str, unsigned char* data);
	
	// encode an array of chars into a hex string
	void data2str(std::string& str, unsigned char* data, int size);
	
	// return a size in bytes of array encoded in a string
	int getDataLength(const std::string& str);

	// set characters to separate hex values
	void setSeparator(const std::string& str);

	// set new line characters to separate lines
	void setNewLine(const std::string& str);

	// set a number of values per line, -1 = infinite 
	void setValuesPerLine(int valPerLine);

	// set characters before hex value start 
	void setHexStart(const std::string& str);
};

#endif