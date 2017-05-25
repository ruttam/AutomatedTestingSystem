// Copyright © 2017 Ruta Tamasauskaite.
// This software is licensed and distributed under a GNU Lesser General Public License version 3.

#ifndef DUT_TEST_CONTROLLER_HPP
#define DUT_TEST_CONTROLLER_HPP

#include <boost/thread.hpp>

#include <BeoLog/BeoLog.hpp>

#include <ATS/ATestCase.hpp>
#include <ATS/TestCaseFactory.hpp>
#include <ATS/TestCaseCallbackInterface.hpp>
#include <ATS/TaskHandler.hpp>

namespace DUTControllerFramework {

    BeoLog::Logger blog("DUTControllerFramework::DUTTestController");

    //! @brief Custom test case exception class.
    class DUTTestControllerException : public std::runtime_error {
        public:
            DUTTestControllerException(const std::string& msg):std::runtime_error(msg) { }

            ~DUTTestControllerException() throw() { }
    };

    //! @brief Interface for main controlling DUT component to inherit.
    class DUTTestController : public TestCaseCallbackInterface {
        public:
            //! @brief Test case result status enumerator.
            enum TaskStatus { in_progress = 0, failed = 1, finished = 2 };
            
            DUTTestController();

            virtual ~DUTTestController();
            
            //! @brief A function to schedule configuration of a test case with the provided arguments.
            //! @param testName A string representing a test case title.
            //! @param arguments A vector string with config data for execution.
            void configureTest(const std::string& testName, const TestCaseFramework::ATestCase::ArgsVector& arguments);

            //! @brief A function to schedule an execution of a test case.
            void startTest();

            // Functions inherited and implemented from TestCaseCallbackInterface
            void debugData(const std::string& data);

            void resultData(const std::string& data);

            //! @brief A template function to add a specific test case of type T to the factory.
            //! @param testCaseTitle A string representing a test case title.
            //! @param T Type of a test case.
            template<typename T>
            void populateFactory(const std::string& testCaseTitle) {
                try {
                    factory->registerTestCase<T>(testCaseTitle);
                } catch(TestCaseFramework::TestCaseFactoryException &e) {
                    BEOLOG_ERROR(blog, e.what());
                }
            }

        protected:
            //! @brief A configuration function of a test case function
            //! @param testName A string representing a test case title.
            //! @param args A vector string with config data for execution.
            void internalTestConfiguration(const std::string& testName, const TestCaseFramework::ATestCase::ArgsVector& args);

            //! @brief A function to start executing a test case.
            void executeTest();

            //! @brief A pure virtual function to transfer data to AT system via a communicator.
            //! @param status A status of test case execution.
            //! @param data An execution data of string format.
            virtual void transferData(const TaskStatus status, const std::string data) = 0;

            //! @brief A function that handles termination of a test case thread and release of resources.
            //! @param data An execution data of string format.
            void testExecutionEnd(const std::string& data);

            //! @brief A pure virtual function to transfer data to AT system via a communicator.
            TaskManagement::TaskHandler * taskHandler;

            //! @brief An instance of task handler to schedule the tasks (functions).
            boost::thread * taskThread;   

            //! @brief A test case factory to add and retrieve concrete test case objects.
            std::auto_ptr<TestCaseFramework::TestCaseFactory> factory;

            //! @brief A test case object that is configured and ran on a DUT.
            TestCaseFramework::ATestCase * testCase;

            //! @brief A test case tread for execution of a test case concurrently.
            boost::thread * testCaseThread; 

            //! @brief A constant value of seconds to wait before terminating test case thread.
            static const int WAIT_TIME_SEC = 20;

    };

}

#endif
