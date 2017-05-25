// Copyright © 2017 Ruta Tamasauskaite.
// This software is licensed and distributed under a GNU Lesser General Public License version 3.

#ifndef TESTCASE_FACTORY_HPP
#define TESTCASE_FACTORY_HPP

#include <map>
#include <memory>
#include <exception>

#include <ATS/ATestCase.hpp>

namespace TestCaseFramework {

    //! @brief Custom test case exception class.
    class TestCaseFactoryException : public std::runtime_error {
        public:
            TestCaseFactoryException(const std::string& msg):std::runtime_error(msg) { }
    };

    //! @brief A factory for registering and returning test cases.
    class TestCaseFactory {
        public:
            TestCaseFactory();

            ~TestCaseFactory();


            template<typename T>
            static ATestCase* createTestCase() {
                return new T();
            }

            typedef ATestCase* (*CreateObject)();

            typedef std::map<std::string, CreateObject> TestCaseMap;

            //! @brief          Register a new test case in a test case factory map.
            //! @param testName A title of a test case.
            //! @param T        A type of a test case.
            //! @throws         TestCaseFramework::TestCaseFactoryException is thrown if a test case
            //!                 with such name already exists in a map.
            template<typename T>
            void registerTestCase(const std::string& testName) {
                if(testCaseMap.find(testName) == testCaseMap.end()) {
                    testCaseMap.insert(std::make_pair(testName, &createTestCase<T>));
                } else {
                    throw TestCaseFramework::TestCaseFactoryException(std::string("ERROR: There is a test case already registered by the name: ").append(testName));
                }
            }

            //! @brief          Create a test case instance.
            //! @param testName A title of a test case.
            //! @throws         TestCaseFramework::TestCaseFactoryException is thrown if a test case
            //!                 with such name is not registered in a factory.
            //! @return         An auto pointer to a test case object.
            ATestCase * createTestCase(const std::string& testName);

        protected:
            //! @brief  A map of the test case name/type pairs.
            TestCaseMap testCaseMap;
    };

}

#endif
