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

#include "../include/Flags.hpp"	// Flags
#include <numeric>		// std::iota
#include <algorithm>		// std::random_shuffle
#include <vector>		// std::vector

using namespace std;


Harbor * Harbor::_instance(nullptr);

/*
 * Constructors
 */

// The one and only available constructor
Harbor::Harbor(unsigned const width, unsigned const height)
: _log("Harbor.log"), _width(width), _height(height)
{
	// Entry points generation
	_entryPoints.insert(Point(_width/2, 0));
	if(_width % 2 == 0)
		_entryPoints.insert(Point(_width/2-1, 0));

	// Will contain the dock IDs (ordered randomly)
	vector<unsigned> dockIds(2 * _height);
	// Dock IDs preparation (iota then permutation)
	iota(dockIds.begin(), dockIds.end(), 1);
	if(Flags::randomizeDocks())
		random_shuffle(dockIds.begin(), dockIds.end());

	_availableDocks = set<unsigned>(dockIds.begin(), dockIds.end());

	// Lets assign the dock IDs
	for(unsigned i = 0 ; i < _height ; ++i)
	{
		_docks[dockIds[i]] = Point(0 , i);
		_docks[dockIds[i + _height]] = Point(_width -1, i);

		_reverseDocks[Point(0 , i)] = dockIds[i];
		_reverseDocks[Point(_width -1 , i)] = dockIds[i + _height];
	}
}

Harbor::~Harbor()
{
	// Clean the remaining Ships on surface
	for(auto pair : _surface)
		delete pair.second;
}


/*
 * Singleton pattern-related methods
 */

// Create (if non-existent) and get the unique Harbor instance
Harbor * Harbor::getInstance(unsigned const width, unsigned const height)
{
	if(_instance == nullptr)
		_instance = new Harbor(width, height);

	return _instance;
}
// Delete the unique Harbor instance
void Harbor::deleteInstance()
{
	delete _instance;
	_instance = nullptr;
}


/*
 * Surface related methods
 */

// Add a Ship to the given position, if possible
bool Harbor::addShip(Ship const * s, Point const & p)
{
	set<Point>::const_iterator entryPointIt(_entryPoints.find(p));
	map<Point, Ship const *>::const_iterator shipIt(_surface.find(p));

	// If the given Point isn't in the entry points list
	if(entryPointIt == _entryPoints.end())
	{
		_log << warn << p << " is not a valid entry point!" << endl;
		return false;
	}
	// If there's already a Ship on the given Point
	else if(shipIt != _surface.end())
	{
		_log << warn << "There's already a Ship on " << p
		<< " (name: " << shipIt->second->name() << ")" << endl;
		return false;
	}

	// Else, assume everything is fine
	_surface.insert(make_pair(p,s));
	_reverseSurface.insert(make_pair(s,p));

	_log << info << "Added Ship " << s->name() << " at " << p << endl;

	return true;
}
bool Harbor::addShip(Ship const & s, Point const & p)
{
	return addShip(&s, p);
}

// Get the Ship located on the given Point
Ship const * Harbor::getShipAt(Point const & p) const
{
	map<Point, Ship const *>::const_iterator shipIt(_surface.find(p));

	if(shipIt != _surface.end())
		return shipIt->second;
	else
		return nullptr;
}

Point Harbor::getShipPosition(Ship const * s) const
{
	map<Ship const *, Point>::const_iterator
		pointIt(_reverseSurface.find(s));

	if(pointIt != _reverseSurface.end())
		return pointIt->second;
	else
		return Point(-1, -1);
}

// Move the Ship located at source to the given destination
// (if possible, see tests in the code)
bool Harbor::moveShip(Point const & source, Point const & destination)
{
	map<Point, Ship const *>::const_iterator
		sourceIt(_surface.find(source)),
		destinationIt(_surface.find(destination));

	// If the destination Point is out of Harbor's boundaries
	if(destination._x < 0 || destination._y < 0
	|| unsigned(destination._x) >= _width
	|| unsigned(destination._y) >= _height)
	{
		_log << warn << destination
		<< " is out of the Harbor's boundaries!" << endl;
		return false;
	}

	// If there's no Ship on the source point
	if(sourceIt == _surface.end())
	{
		_log << warn << "There's no Ship at " << source << endl;
		return false;
	}
	// If there's already a Ship on the destination point
	if(destinationIt != _surface.end())
	{
		// This means we'll experience a... COLLISION!
		// Let the games begin.
		if(collision(_surface[source], _surface[destination]))
		{
			// We can't crush the other Ship: the controller will
			// have to get around it
			return false;
		}
		else
		{
			// Log the action, send flowers to the Ship's family...
			_log << info << "[COLLISION] Ship "
			<< _surface[source]->name() << " crushed "
			<< _surface[destination]->name()
			<< " into little pieces with no mercy!"
			<< endl;

			// Keep a reference to the victim before removing it
			// from the maps
			Ship const * victim(_surface[destination]);


			// Revoke his dock reservation... he will no longer
			// need it :/
			_availableDocks.insert(_reservations[victim->name()]);
			_reservations.erase(victim->name());

			// Remove it from the surface, and then...
			_surface.erase(destination);
			_reverseSurface.erase(victim);

			// This. Is. SPARTAAAAA!
			delete victim;

			return true;
		}
	}

	// Everything should be fine from now on

	_reverseSurface[_surface[source]] = destination;
	_surface[destination] = _surface[source];
	_surface.erase(source);

	_log << info << "Moved Ship " << _surface[destination]->name()
	<< " from " << source << " to " << destination << endl;

	return true;
}

// Remove the reservation (if any) associated with the given Ship name
bool Harbor::removeReservation(string const & name)
{
	map<string, unsigned>::const_iterator
		reservationIt(_reservations.find(name));

	// If there's a dock reserved for this Ship,
	if(reservationIt != _reservations.end())
	{
		// erase it from the reservations map
		_availableDocks.insert(reservationIt->second);
		_reservations.erase(reservationIt);

		return true;
	}
	else
		return false;
}

// Remove, if any, the Ship emplaced in the given position
bool Harbor::removeShip(Point const & p)
{
	map<Point, Ship const *>::const_iterator shipIt(_surface.find(p));

	// If there's no Ship on the given point
	if(shipIt == _surface.end())
	{
		_log << warn << "There's no Ship at " << p << endl;
		return false;
	}

	// Else, assume everything is fine and proceed
	removeReservation(shipIt->second->name());

	_reverseSurface.erase(_surface[p]);
	_surface.erase(p);

	return true;
}

bool Harbor::removeShip(Ship const * s)
{
	map<Ship const *, Point>::const_iterator
		pointIt(_reverseSurface.find(s));

	// Anti-segfault measure
	if(s == nullptr)
	{
		_log << warn << "nullptr given to removeShip(Ship const * s)"
		<< endl;
		return false;
	}

	// Check mapping before removal attempt
	if(pointIt == _reverseSurface.end())
	{
		_log << warn << "Ship " << s->name() << " is not mapped!"
		<< endl;
		return false;
	}

	// Proceed
	removeReservation(s->name());

	_surface.erase(_reverseSurface[s]);
	_reverseSurface.erase(s);

	return true;
}

// Get a reference to the non-mutable set of entry points
set<Point> const & Harbor::entryPoints() const
{
	return _entryPoints;
}

// Get a reference to the non-mutable map representing the Harbor's surface
map<Point, Ship const *> const & Harbor::surface() const
{
	return _surface;
}

// Get a reference to the non-mutable reversed map representing the Harbor's
// surface
map<Ship const *, Point> const & Harbor::reverseSurface() const
{
	return _reverseSurface;
}

/*
 * Docks-related methods
 */

// Get the position of the given dock in the Harbor's _surface.
// Returns [-1, -1] if the dock doesn't exist.
Point Harbor::getDockPosition(unsigned const id) const
{
	map<unsigned, Point>::const_iterator dockIt(_docks.find(id));

	if(dockIt != _docks.end())
	{
		return dockIt->second;
	}
	else
	{
		return Point(-1,-1);
	}
}

// Get a reference to the non-mutable set of available docks
set<unsigned> const & Harbor::availableDocks() const
{
	return _availableDocks;
}

// Reserve the given dock for the given Ship
bool Harbor::reserveDock(unsigned const dockId, Ship const * const s)
{
	set<unsigned>::const_iterator
		availableDocksIt(_availableDocks.find(dockId));

	map<string, unsigned>::const_iterator
		reservationIt(_reservations.find(s->name()));

	// If the given Ship already possesses a dock
	if(reservationIt != _reservations.end())
	{
		_log << warn << "Ship " << s->name()
		<< " already has a reserved dock! "
		<< "(ID: " << getReservedDock(s) << ")" << endl;

		return false;
	}
	// If the given dock is available for reservation
	if(availableDocksIt != _availableDocks.end())
	{
		// Remove the dock ID from available docks
		_availableDocks.erase(dockId);
		// Add a reservation entry
		_reservations.insert(make_pair(s->name(), dockId));

		_log << info << "Reserved dock n°" << dockId << " for Ship "
		<< s->name() << endl;
		return true;
	}
	else
	{
		_log << warn << "Dock " << dockId
		<< " is invalid or already reserved!" << endl;
		// We don't give a dock.
		return false;
	}
}
bool Harbor::reserveDock(unsigned const dockId, Ship const & s)
{
	return reserveDock(dockId, &s);
}

// Get the reserved dock ID for the given Ship
unsigned Harbor::getReservedDock(Ship const * const s) const
{
	map<string, unsigned>::const_iterator
		dockIt(_reservations.find(s->name()));

	// If the given Ship has a reserved dock
	if(dockIt != _reservations.end())
	{
		// return it
		return dockIt->second;
	}
	else
	{
		// 0 means "no dock for this Ship"
		return 0;
	}
}
unsigned Harbor::getReservedDock(Ship const & s) const
{
	return getReservedDock(&s);
}

// Get a reference to the non-mutable map of docks positions
map<unsigned, Point> const & Harbor::dockMap() const
{
	return _docks;
}

// Get a reference to the non-mutable reversed map of docks positions
map<Point, unsigned> const & Harbor::reverseDockMap() const
{
	return _reverseDocks;
}


/*
 * Collisions-related methods
 */

// Handles collisions between two Ships, comparing their respective hulls
bool Harbor::collision(Ship const * const s1, Ship const * const s2)
{
	if(s1->hull() > s2->hull())
	{
		// "false" means "We don't care anymore"
		// (from s1's point of view)
		return false;
	}
	else
	{
		// "true" means "We'd rather try to avoid any further
		// interaction with this big bad Ship!" for s1
		return true;
	}
}
