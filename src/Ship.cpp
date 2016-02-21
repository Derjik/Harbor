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

#include <sstream>			// std::ostringstream
#include "../include/Factory.hpp"	// Factory
#include "../include/Engine.hpp"	// Engine
#include "../include/Hull.hpp"		// Hull

using namespace std;


Ship::Ship(Factory const * const f, string const name)
	:
	_name(name),
	_engine(f->createEngine()),
	_hull(f->createHull()),
	_LinuxColor(240),
	_WindowsColor(7)
{
	if(_name == "")
	{
		// This is the best (or least worst, at least)
		// workaround we've found to generate guenuinely
		// unique Ship names.
		ostringstream o;
		// In fact, we cast its address into a string.
		const void * p(static_cast<const void*>(this));
		o << p;
		_name = o.str();
	}
}

Ship::~Ship()
{
	delete _engine;
	delete _hull;
}

unsigned Ship::speed() const
{
	return _engine->speed();
}

Hull const * Ship::hull() const
{
	return _hull;
}

string Ship::name() const
{
	return _name;
}

void Ship::setName(string const n)
{
	_name = n;
}

void Ship::accept(Visitor* v)
{
	v->visit(this);
}

list<Visited*> Ship::getVisitedItems() const
{
	list<Visited*> l;

	l.push_back(_engine);
	l.push_back(_hull);

	return l;
}
