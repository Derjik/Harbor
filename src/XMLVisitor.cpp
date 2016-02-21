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

#include "../include/XMLVisitor.hpp"

#include "../include/Ship.hpp"		// Ship
#include "../include/Hull.hpp"		// Hull
#include "../include/Engine.hpp"	// Engine

using namespace std;


XMLVisitor::XMLVisitor(string path) : _xmlFile(path)
{}

XMLVisitor::~XMLVisitor()
{
	// Close the logfile
	_xmlFile.close();
}

// Visit a Ship and its components
void XMLVisitor::visit(Ship const * const s)
{
	_xmlFile << "<Ship>" << endl;
	_xmlFile << "\t<Name>" << s->name() << "</Name>" << endl;
	_xmlFile << "\t<Type>" << s->type() << "</Type>" << endl;
	_xmlFile << "\t<Priority>" << s->priority() << "</Priority>" << endl;
	_xmlFile
	<< "\t<FailureRate>" << s->failureProbability()
	<< "</FailureRate>" << endl;

	for(auto component : s->getVisitedItems())
		component->accept(this);

	_xmlFile << "</Ship>" << endl;
}

// Visit a Hull
void XMLVisitor::visit(Hull const * const h)
{
	_xmlFile << "\t<Hull>" << endl;
	_xmlFile << "\t\t<Solidity>" << h->solidity() << "</Solidity>" << endl;
	_xmlFile << "\t</Hull>" << endl;
}

// Visit an Engine
void XMLVisitor::visit(Engine const * const e)
{
	_xmlFile << "\t<Engine>" << endl;
	_xmlFile << "\t\t<Speed>" << e->speed() << "</Speed>" << endl;
	_xmlFile << "\t</Engine>" << endl;
}
