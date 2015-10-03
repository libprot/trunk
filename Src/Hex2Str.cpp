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

#include "..\Inc\Hex2Str.h"

CHex2Str::CHex2Str() : separator(", "), newline("\r\n"), hexStart("0x"), valuesPerLine(-1) {
}

CHex2Str::CHex2Str(const std::string& hex, const std::string& nl, const std::string& sep, int values) :
	separator(sep), hexStart(hex), valuesPerLine(values), newline(nl) 
{
}

bool CHex2Str::isHex(unsigned char c) {
	return isxdigit(c) != 0;
}

int CHex2Str::hex2Num(unsigned char c) {
	if (!isHex(c)) {
		return 0;
	}
	if (isdigit(c)) {
		return c - '0';
	}		
	return tolower(c) - 'a' + 10;
}

unsigned char CHex2Str::num2hex(int num) {
	num &= 0x0F;
	if (num < 10) {
		return num + '0';
	}
	return num - 10 + 'a';
}

void CHex2Str::byte2hex(unsigned char c, std::string& str) {
	str.append(1, (char)num2hex(c >> 4));
	str.append(1, (char)num2hex(c));
}

unsigned char CHex2Str::hex2byte(const std::string& str, size_t offset) {
	return hex2Num(str.at(offset)) << 4 | hex2Num(str.at(offset + 1)); 
}

int CHex2Str::string2data(const std::string& str, unsigned char* data) {
	const size_t size = str.length();
	int strOffset = 0;
	for (size_t index = 0; index < size; index++) {
		if (skipString(index, str, newline)) {
			continue;
		}
		if (skipString(index, str, hexStart)) {
			continue;
		}
		if (isHex(str.at(index)) && index + 1 < size && isHex(str.at(index + 1))) {
			*(data + strOffset++) = hex2byte(str, index);
			index++;
		} else {
			break;
		}
		if (skipString(index, str, separator)) {
			continue;
		}
	}
	return strOffset;
}

void CHex2Str::data2str(std::string& str, unsigned char* data, int size) {
	bool addedComma = true;
	int items = -1;
	for (int i = 0; i < size; i++) {
		if (addedComma == false) {
			str.append(separator);
		}
		addedComma = false;
		items++;
		if (items >= valuesPerLine && valuesPerLine != -1) {
			items = 0;
			str.append(newline);
		}
		str.append(hexStart);
		byte2hex(data[i], str);
	}
}
bool CHex2Str::skipString(size_t& index, const std::string& str, const std::string& part) {
	const size_t skipSize = part.length();
	if (skipSize == 0) {
		return false;
	}
	if (str.compare(index, std::min(str.length() - index, skipSize), part.c_str(), skipSize) != 0) {
		return false;
	}
	index += skipSize - 1;
	return true;
}

int CHex2Str::getDataLength(const std::string& str) {
	size_t size = 0;
	for (size_t index = 0; index < str.length(); index++) {
		if (skipString(index, str, newline)) {
			continue;
		}
		if (skipString(index, str, hexStart)) {
			continue;
		}
		if (!isHex(str.at(index))) {
			if (skipString(index, str, separator)) {
				continue;
			}
		}
		size++;
	}
	return (int)size / 2;
}

void CHex2Str::setSeparator(const std::string& str) {
	separator = str;
}

void CHex2Str::setNewLine(const std::string& str) {
	newline = str;
}

void CHex2Str::setValuesPerLine(int valPerLine) {
	if (valPerLine < -1) {
		valPerLine = -1;
	}
	if (valPerLine == 0) {
		valPerLine = 1;
	}
	valuesPerLine = valPerLine;
}

void CHex2Str::setHexStart(const std::string& str) {
	hexStart = str;
}