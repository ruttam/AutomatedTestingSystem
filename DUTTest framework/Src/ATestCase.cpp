// Copyright © 2017 Ruta Tamasauskaite.
// This software is licensed and distributed under a GNU Lesser General Public License version 3.

#include <ATS/ATestCase.hpp>

namespace TestCaseFramework {

    ATestCase::ATestCase()
        :testName(""),
        arguments(),
        callbackObject() { }

    ATestCase::~ATestCase() { }
    
}
