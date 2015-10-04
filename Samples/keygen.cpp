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
#include "..\Inc\merklehellman.h"
#include <vector>

void main(int argc, _TCHAR* argv[])
{
	std::string key = "10/10/2020|Some Award Winning Software Corporation";
	     //  example of key 
         //	   1) expiration date
         //    2) separator
         //    3) name of the company 		 
	
	CKeyObfuscation keyGen;
	std::string s = keyGen.obfuscateKey(key);
	// s - the string with a obfuscated key 
	
	keyGen.unobfuscateKey(s);
	// should be equal to "10/10/2020|Some Award Winning Software Corporation"
}

