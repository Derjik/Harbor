/*
 * Copyright (c) 2016 Julien "Derjik" Laurent <julien.laurent@engineer.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "../include/Ship.hpp"

#include <iostream>	// std::cout, std::endl
#include <windows.h>	// Windows color options

using namespace std;

/*
 * _WindowsColor bits meaning:
 *
 * FOREGROUND_BLUE	= 1
 * FOREGROUND_GREEN	= 2
 * FOREGROUND_RED	= 4
 * FOREGROUND_INTENSITY	= 8
 * BACKGROUND_BLUE	= 16
 * BACKGROUND_GREEN	= 32
 * BACKGROUND_RED	= 64
 * BACKGROUND_INTENSITY	= 128
 */

void Ship::display() const
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _WindowsColor);
	cout << "<>";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
