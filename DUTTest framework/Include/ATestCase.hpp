// Copyright © 2017 Ruta Tamasauskaite.
// This software is licensed and distributed under a GNU Lesser General Public License version 3.

#ifndef ABSTRACT_TESTCASE_HPP
#define ABSTRACT_TESTCASE_HPP

#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <vector>
#include <list>

#include <ATS/TestCaseCallbackInterface.hpp>

namespace TestCaseFramework {

    //! @brief Custom test case exception class.
    class TestCaseException : public std::runtime_error {
        public:
            TestCaseException(const std::string& msg):std::runtime_error(msg) { }
    };
    
    //! @brief Abstract class for concrete test cases to inherit.
    class ATestCase {
        public:
            typedef std::vector<std::string> ArgsVector;

            ATestCase();

            virtual ~ATestCase();

            //! @brief Configure a test case with the provided arguments.
            //! @param args A string vector of configuration data - particular for each test case.
            //! @throw TestCaseFramework::TestCaseException in case the arguments are not valid.
            virtual void configure(const ArgsVector& args)=0;

            //! @brief Execute a configured test case and process its output.
            //! @param cbObject A callback object to make a callback with execution data to.
            //! @throw TestCaseFramework::TestCaseException in case the execution of a test case failed unexpectedly.
            virtual void execute(DUTControllerFramework::TestCaseCallbackInterface * cbObject)=0;

        protected:
            //! @brief A name of a test case.
            std::string testName;
            //! @brief An arguments' list for an execution of a test case.
            ArgsVector arguments;
            //! @brief A pointer to a callback object.
            DUTControllerFramework::TestCaseCallbackInterface * callbackObject;
    };

}

#endif
