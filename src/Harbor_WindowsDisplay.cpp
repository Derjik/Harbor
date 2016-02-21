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

#include "../include/Harbor.hpp"

#include <iostream>	// std::cout, std::endl
#include <iomanip>	// std::setw, std::left, std::right...

using namespace std;


// Add Harbor information to the given stream
void Harbor::display() const
{
	map<Point, unsigned>::const_iterator di;	// Dock iterator
	map<Point, Ship const *>::const_iterator si;	// Ship iterator
	set<Point>::const_iterator epi;			// Entry point iterator

	// ASCII display header
	cout << "+";
	for(unsigned i = 0 ; i < _width-2 ; ++i)
		cout << "-";
	cout << "+  +";
	for(unsigned i = 0 ; i < _width-2 ; ++i)
		cout << "-";
	cout << "+" << endl;

	// ASCII display content
	for(unsigned y = 0 ; y < _height ; ++y)
	{
		cout << "|";
		for(unsigned x = 0 ; x < _width ; ++x)
		{
			di = _reverseDocks.find(Point(x, y));
			si = _surface.find(Point(x, y));
			epi = _entryPoints.find(Point(x, y));

			// Display priority: Ship, Entry point, Dock, Nothing
			if(si != _surface.end())
			{
				si->second->display();
			}
			else if(epi != _entryPoints.end())
			{
				cout << "##";
			}
			else if(di != _reverseDocks.end())
			{
				if(x == 0)
					cout << setw(2) << right << di->second;
				else
					cout << setw(2) << left << di->second;
			}
			else
			{
				cout << "  ";
			}
		}
		cout << "|" << endl;
	}

	// ASCII display footer
	cout << "+";
	for(unsigned i = 0 ; i < _width ; ++i)
		cout << "--";
	cout << "+" << endl;
}
