// Copyright © 2017 Ruta Tamasauskaite.
// This software is licensed and distributed under a GNU Lesser General Public License version 3.

#include <ATS/TestCaseFactory.hpp>

namespace TestCaseFramework {

	TestCaseFactory::TestCaseFactory():testCaseMap() { }

	TestCaseFramework::ATestCase * TestCaseFactory::createTestCase(const std::string& name) {
		TestCaseMap::iterator it = testCaseMap.find(name);
		if(it == testCaseMap.end()) {
			throw TestCaseFramework::TestCaseFactoryException(std::string(
				"ERROR: There is no test case registered in the factory by the name: ").append(name));
		}
		return (* it).second();
	}

	TestCaseFactory::~TestCaseFactory() { }

}
