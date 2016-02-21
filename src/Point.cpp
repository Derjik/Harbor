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

#include "../include/Point.hpp"

#include <sstream>		// std::ostringstream

using namespace std;

// Point + Direction operator
Point Point::operator + (Direction const d) const
{
	Point p;

	switch(d)
	{
		case UP:
			p = Point(_x , _y + -1);
		break;

		case DOWN:
			p = Point(_x , _y + 1);
		break;

		case LEFT:
			p = Point(_x -1 , _y);
		break;

		case RIGHT:
			p = Point(_x +1 , _y);
		break;
	}

	return p;
}

// Point += Direction operator
Point Point::operator += (Direction const d)
{
	return (*this) = (*this) + d;
}

// Direction to Point constructor
Point::Point(Direction const d)
{
	switch(d)
	{
		case LEFT:
			_x = -1;
			_y = 0;
		break;

		case RIGHT:
			_x = 1;
			_y = 0;
		break;

		case DOWN:
			_x = 0;
			_y = 1;
		break;

		case UP:
			_x = 0;
			_y = -1;
		break;
	}
}

// Point to Direction conversion
Point::operator Direction() const
{
	if(_x * _x > _y * _y)
	{
		if(_x > 0)
			return RIGHT;
		else
			return LEFT;
	}
	else
	{
		if(_y > 0)
			return DOWN;
		else
			return UP;
	}
}

// Point to string conversion
Point::operator string() const
{
	ostringstream s;
	s << "[" << _x << " , " << _y << "]";

	return s.str();
}

// Direciton display method
ostream & operator << (ostream & s, Direction const & d)
{
	switch(d)
	{
		case UP:
			s << "UP";
		break;

		case DOWN:
			s << "DOWN";
		break;

		case LEFT:
			s << "LEFT";
		break;

		case RIGHT:
			s << "RIGHT";
		break;
	}

	return s;
}

// Manhattan distance between two Points
unsigned manhattanDistance(Point const & a, Point const & b)
{
	unsigned d(0);

	d += (b._x - a._x) * (b._x - a._x);
	d += (b._y - a._y) * (b._y - a._y);

	return d;
}
