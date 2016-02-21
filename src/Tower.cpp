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

#include "../include/Tower.hpp"

#include <thread>	// std::this_thread::sleep_for
#include <chrono>	// std::chrono

#include <iostream>	// std::cout
#include <cstdlib>	// abs()

/* Harbor */
#include "../include/Harbor.hpp"

/* Ships */
#include "../include/PassengerShip.hpp"
#include "../include/MilitaryShip.hpp"
#include "../include/FishingBoat.hpp"
#include "../include/PleasureCraft.hpp"

/* Factories */
#include "../include/LowCostManufactory.hpp"
#include "../include/PrestigiousManufactory.hpp"

/* Flags */
#include "../include/Flags.hpp"

/* Die */
#include "../include/Die.hpp"

using namespace std;


// Initialize logfiles and set the Harbor instance pointer
Tower::Tower(Harbor * h)
	: _log("Tower.log"), _xml("ships.xml"), _harbor(h)
{}


// Start a cycle using the given Ship creation probability
// (proba should be a number between 0 and 100 inclusive,
// 0 meaning "no ships", 100 meaning "a new Ship each turn")
void Tower::cycle(unsigned const proba)
{
	bool allDestinationsReached = false;


	// Initial Harbor display
	_harbor->display();

	// As long as docks are available from the Harbor OR some Ships
	// need to move
	while(!_harbor->availableDocks().empty() || !allDestinationsReached)
	{
		// Temporization
		sleep(Flags::cycleDelay());

		// Plan movements on the whole surface
		allDestinationsReached = planMovements();

		// Display the Ship queue and Harbor's surface
		cout << endl << endl << endl;
		displayQueue();
		_harbor->display();

		// Apply the planned moves onto the surface
		applyPlannedMovements();

		// Prepare new Ships arrival
		manageNewShips(proba);
	}
}

// Start the "reverse" cycle: this one causes all Ships
// emplaced in the Harbor to seek for the closest exit
// and disappear until they're all gone.
void Tower::cycleOut()
{
	Ship const * currentShip(nullptr);

	// While Ships are present in the Harbor
	while(_harbor->surface().size() > 0)
	{
		// Temporization
		sleep(Flags::cycleDelay());

		// Get a Ship to move out
		if(currentShip == nullptr)
			currentShip = _harbor->surface().begin()->second;

		// Plan outgoing movements on the whole surface
		manageOutgoingShip(currentShip);

		// Display the Harbor's surface
		cout << endl << endl << endl;
		_harbor->display();

		// Clean the exit Points
		cleanExit();

		// Detect eventual Ship deletion
		if(_harbor->reverseSurface().find(currentShip)
				== _harbor->reverseSurface().end())
			currentShip = nullptr;

		// Apply the planned moves onto the surface
		applyPlannedMovements();
	}

	cout << endl << endl << endl;
	_harbor->display();
}

// Displays the Ship queue state in a nice-looking
// and OS-compatible way
void Tower::displayQueue()
{
	unsigned size(_shipQueue.size());

	// Try displaying the Ships with their respective symbols
	if(size <= _harbor->width())
	{
		cout << "[";

		for(auto ship : _shipQueue)
			ship->display();

		for(unsigned i = 0 ; i < _harbor->width() - size ; ++i)
			cout << "  ";

		cout << "]";
	}
	// If there are too many Ships to do so, indicate their number instead
	else
	{
		cout << "[ Queue size: " << _shipQueue.size() << " ]";
	}

	cout << endl;
}

// Computes one movement for one Ship and returns true if the Ship will move
bool Tower::traceRoute(Point const & source, Point const & dest)
{
	// Ships
	Ship const	*ourShip(_harbor->getShipAt(source)),
			*otherShip(nullptr);

	// Navigation data
	unsigned movesToGo(ourShip->speed());
	Point currentLocation(source);
	Direction direction;

	// Aleas
	float fail;

	// If we're already on the destination Point
	if(source == dest)
		return false;

	// Prevent calls on nullptr
	if(ourShip == nullptr)
	{
		_log << warn << "Cannot trace route from " << source
		<< " to " << dest << " : no Ship found on "
		<< source << " !" << endl;
		return false;
	}

	_log << info << "Roadmap for Ship " << ourShip->name() << ":" << endl;

	while(movesToGo > 0 && currentLocation != dest)
	{
		// Compute delta between current location and given destination
		direction = Direction(dest - currentLocation);

		// Look for obstacles
		otherShip = _harbor->getShipAt(currentLocation + direction);

		fail = Die::roll(0.f, 1.f);

		// If the engine failes
		if(fail < ourShip->failureProbability())
		{
			_log << info << "\t"
			<< ourShip->speed() - movesToGo + 1
			<< ": [Engine failure] " << currentLocation << endl;
		}
		// If there's no ostacle OR if we can easily crush it
		// into pieces
		else if(otherShip == nullptr
			|| otherShip->hull() <= ourShip->hull())
		{
			_log << info << "\t"
			<< ourShip->speed() - movesToGo + 1 << ": "
			<< currentLocation << " -> "
			<< currentLocation + direction << endl;

			_plannedMovements.push_back(make_pair(currentLocation,
						currentLocation + direction));
			currentLocation += direction;
		}
		// Assume no movement is possible at the time
		else
		{
			_log << info << "\t"
			<< ourShip->speed() - movesToGo + 1
			<< ": [Stay put] " << currentLocation << endl;
		}
		--movesToGo;
	}

	return true;
}

// Find the nearest exit Point to a given source Point
Point Tower::chooseExit(Point const & source)
{
	// Initial distance
	unsigned dist(manhattanDistance(source, (*_harbor->entryPoints().begin())));
	// Default chosen exit
	Point chosenExit((*_harbor->entryPoints().begin()));

	// Probe available exit points to find the closest one
	for(auto entryPoint : _harbor->entryPoints())
	{
		if(manhattanDistance(source, entryPoint) < dist)
		{
			dist = manhattanDistance(source, entryPoint);
			chosenExit = entryPoint;
		}
	}

	return chosenExit;
}

void Tower::cleanExit()
{
	Ship const * s(nullptr);

	// Destroy Ships that reached the exit
	for(auto exit : _harbor->entryPoints())
	{
		s = _harbor->getShipAt(exit);
		if(s != nullptr)
		{
			_harbor->removeShip(exit);
			delete s;
		}
	}
}

// Plan outgoing movements and remove Ships that reached
// an exit Point
void Tower::manageOutgoingShip(Ship const * ship)
{
	Point source(-1, -1), dest(-1, -1);

	if(ship == nullptr)
		return;

	// Get the Ships current position
	map<Ship const *, Point>::const_iterator
		pair(_harbor->reverseSurface().find(ship));

	// Source is the Ships current position
	source = pair->second;

	// Choose the nearest exit
	dest = chooseExit(source);

	// Compute a route to the chosen exit
	traceRoute(source, dest);
}

// Computes the planned movements for each Ship onto the Harbor's surface
bool Tower::planMovements()
{
	// Currently manipulated dock ID
	unsigned dockId(0);

		// True when every Ship on the surface has reached
		// its destination
	bool 	allDestinationsReached(true),
		// True if a movement is needed
		// (determined turn by turn with traceRoute)
		movementNeeded(false);

	// Current Ship's source and destination
	Point source(-1,-1), dest(-1, -1);

	// For each Ship currently emplaced onto the Harbor...
	for(auto pair : _harbor->surface())
	{
		// Get its current position
		// and its reserved dock ID
		source = pair.first;
		dockId = _harbor->getReservedDock(pair.second);

		// If it has an assigned dock (which it SHOULD REALLY have)
		if(dockId != 0)
		{
			// set its destination to the assigned dock's position
			dest = _harbor->getDockPosition(dockId);

			_log << info << "Ship " << pair.second->name()
			<< " at " << pair.first
			<< " owns dock n°"
			<< _harbor->getReservedDock(pair.second)
			<< " located at " << dest << endl;

			// Trace the roadmap
			movementNeeded = traceRoute(source, dest);

			// Update the global movements flag
			allDestinationsReached &= !movementNeeded;
		}
		else
		{
			_log << error << "[CRITICAL] In spite of all our"
			<< "efforts, a Ship managed to go without an assigned"
			<< "dock. Please submit a bug report to the developper."
			<< endl;
		}
	}

	return allDestinationsReached;
}

// Applies the planned movements to the Ships
void Tower::applyPlannedMovements()
{
	// For each planned movement (first = source , second = dest)
	for(auto pair : _plannedMovements)
	{
		_harbor->moveShip(pair.first, pair.second);
	}

	// Clear the planned movements list
	_plannedMovements.clear();
}

// Extracts Ships from the waiting queue (if any) or
// creates new Ships if docks are available for reservation
// and tries to insert them and reserve docks
void Tower::manageNewShips(unsigned const proba)
{
	// Random number for Ship Generation Event
	unsigned randomNumber(Die::roll(1, 100));

	// Current Ship we're working on
	Ship const * s;

	// If the random Ship creation event occurs,
	if(randomNumber < proba &&
	_harbor->availableDocks().size() - _shipQueue.size() > 0)
		s = createShip();

	else if(_shipQueue.size() > 0)
	{
		s = _shipQueue.front();
		_shipQueue.pop_front();

		_log << info << "Ship " << s->name()
		<< " was popped from the waiting queue (queue size is now "
		<< _shipQueue.size() << ")" << endl;
	}
	else	// we'll manage a whole bunch of nothing :)
		s = nullptr;

	// If we got some Ship to insert into the Harbor
	if(s != nullptr)
		insertShip(s);
}

// Replace the original Ship's reservation with
// the given replacement Ship
bool Tower::replaceReservation(Ship const * const original,
				Ship const * const replacement)
{
	unsigned dockId(_harbor->getReservedDock(original));

	// Security
	if(original == replacement)
		return false;

	// Remove the original Ship from the surface
	// (this also resiliates its dock reservation)
	if(_harbor->removeShip(original))
		// and delete it
		delete original;
	else
	{
		_log << error << "Failed to delete Ship " << original->name()
		<< " while attempting replacement with " << replacement->name()
		<< endl;
		return false;
	}

	// Try to reserve the newly liberated dock for the
	// replacement Ship
	if(_harbor->reserveDock(dockId, replacement))
		return true;
	else
	{
		_log << error << "Failed to reserve supposedly freed dock "
		<< dockId << " for Ship " << replacement->name() << endl;

		return false;
	}
}

// Try, by several means, to assign a dock to the given
// Ship in the Harbor's dock database
bool Tower::assignDock(Ship const * const ship)
{
	// Assignation success flag
	bool success(false);

	// Priorities, dock acceptance
	unsigned p1, p2;
	bool accept(false);

	// Available docks iterator, surface iterator
	set<unsigned>::const_iterator
		dit(_harbor->availableDocks().begin());

	map<Point, Ship const *>::const_iterator
		sit(_harbor->surface().begin());


	// Step 1: try using the available docks (if any)
	_log << info << "Looking for a suitable dock for Ship "
	<< ship->name() << endl;

	while(!success && dit != _harbor->availableDocks().end())
	{
		if(ship->accept(*dit))
		{
			_log << info << "\tDock " << *dit << " was accepted"
			<< endl;

			success = _harbor->reserveDock((*dit), ship);
		}
		else
		{
			dit++;
		}
		// Note: reserveDock invalidates the dockIterator.
		// This if / else construct prevents calling dockIterator
		// on an invalid iterator (leading to an inevitable
		// segfault).
	}

	// Step 2: try replacing a Ship with inferior priority
	while(!success && sit != _harbor->surface().end())
	{
		// Skip the requesting ship itself to prevent
		// critical errors
		if(sit->second == ship)
			sit++;

		// Check priorities
		p1 = sit->second->priority();
		p2 = ship->priority();
		// Simulate dock proposal to our Ship
		accept = ship->accept(_harbor->getReservedDock(sit->second));

		_log << info << "\tProbing dock "
		<< _harbor->getReservedDock(sit->second)
		<< " : Ship " << sit->second->name() << " has priority " << p1
		<< " , while " << ship->name() << " has priority " << p2 << endl;

		if(p1 < p2 && accept)
		{
			_log << info << "\t" << ship->name()
			<< " wins and accepts dock "
			<< _harbor->getReservedDock(sit->second) << endl;

			// Try replacing the lower-priority Ship's reservation
			success = replaceReservation(sit->second, ship);
		}

		// If the replacement failed, try the next one
		if(!success)
			sit++;
	}

	return success;
}

// Try to insert the given Ship into the Harbor and
// reserve a dock for it
void Tower::insertShip(Ship const * const s)
{
	// Ship insertion flag
	bool shipInserted(false);

	// Dock reservation flag
	bool dockReserved(false);

	// Entry Point iterator
	set<Point>::const_iterator epi(_harbor->entryPoints().begin());

	// Search for a free entry point
	while(epi != _harbor->entryPoints().end() && !shipInserted)
	{
		shipInserted = _harbor->addShip(s, (*epi));

		epi++;
	}

	// If we managed to insert the Ship on an entry point,
	if(shipInserted)
	{
		_log << info << "Ship " << s
		<< " successfully entered the Harbor at "
		<< _harbor->getShipPosition(s) << endl;

		// try assigning it a dock
		dockReserved = assignDock(s);

		// If we fail to assign a dock to the newly
		// inserted Ship
		if(!dockReserved)
		{
			_log << warn << "No suitable docks were found for Ship "
			<< s << " : this Ship will be deleted!" << endl;

			// Remove it from the surface
			// and delete it (no other solution)
			_harbor->removeShip(s);
			delete s;
		}
	}
	// If we fail to insert it on an entry point
	else
	{
		// Put in in waiting line
		_shipQueue.push_back(s);

		_log << info << "Ship " << s
		<< " joined the waiting queue (queue size is now "
		<< _shipQueue.size() << ")" << endl;
	}
}

// Create a new Ship using a random Factory and
// specialized type
Ship const * Tower::createShip()
{
	// Random settings
	unsigned factoryType(Die::roll(1, 2));
	unsigned shipType(Die::roll(1, 4));

	// Factory and Ship interfaces pointers
	Factory * f(nullptr);
	Ship * s(nullptr);

	_log << info << "Will use a ";
	switch(factoryType)
	{
		default:
		case 1:
			_log << info << "Prestigious Manufactory";
			f = new PrestigiousManufactory();
		break;

		case 2:
			_log << info << "Low Cost Manufactory";
			f = new LowCostManufactory();
		break;
	}

	_log << info << " to build a ";

	switch(shipType)
	{
		default:
		case 1:
			_log << info << "Passenger Ship" << endl;
			s = new PassengerShip(f);
		break;

		case 2:
			_log << info << "Military Ship" << endl;
			s = new MilitaryShip(f);
		break;

		case 3:
			_log << info << "Pleasure Craft" << endl;
			s = new PleasureCraft(f);
		break;

		case 4:
			_log << info << "Fishing Boat" << endl;
			s = new FishingBoat(f);
		break;
	}
	// The Factory is no longer used
	delete f;

	// Log the newly created Ship's identity
	// in ships.xml
	s->accept(&_xml);

	return s;
}

void Tower::sleep(unsigned const milliseconds)
{
	this_thread::sleep_for(chrono::milliseconds(milliseconds));
}
