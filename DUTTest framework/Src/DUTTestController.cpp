// Copyright © 2017 Ruta Tamasauskaite.
// This software is licensed and distributed under a GNU Lesser General Public License version 3.

#include <ATS/DUTTestController.hpp>
#include <ATS/ATestCase.hpp>

#include <BeoLog/BeoLog.hpp>

#include <iostream>
#include <boost/thread.hpp>
#include <boost/ref.hpp>

namespace {
    BeoLog::Logger blog("DUTControllerFramework::DUTTestController");
}

namespace DUTControllerFramework {

    DUTTestController::DUTTestController()
        :TestCaseCallbackInterface(),
        taskHandler(new TaskManagement::TaskHandler),
        taskThread(NULL),
        factory(new TestCaseFramework::TestCaseFactory),
        testCase(NULL),
        testCaseThread(NULL) {
            taskThread = new boost::thread(boost::bind(&TaskManagement::TaskHandler::manageTasks, taskHandler));
         }

    DUTTestController::~DUTTestController() { 
        testExecutionEnd("External termination of a test case.");
        taskHandler->terminateHandler();
        taskThread->join();
        delete taskHandler;
        delete taskThread; 
    }

    void DUTTestController::configureTest(const std::string& testName, const TestCaseFramework::ATestCase::ArgsVector& args) {
        if(testName.empty()) {
            const std::string message("Cannot configure a test case: a test case name is invalid.");
            BEOLOG_ERROR(blog, message);
            transferData(failed, message);
        } else {
            if(!taskHandler->addTask(boost::bind(&DUTTestController::internalTestConfiguration, this, testName, args))) {
                transferData(failed, "Test case named:" + testName + "will not be executed on a DUT because of a scheduling failure.");
            }
        }
    }

    void DUTTestController::internalTestConfiguration(const std::string& testName, const TestCaseFramework::ATestCase::ArgsVector& args) {
        if(!testCase) {
            try { 
                testCase = factory->createTestCase(testName);
                BEOLOG_INFO(blog, "A test case object created.");
                testCase->configure(args);
                BEOLOG_INFO(blog, "A test case successfully configured with parameters.");
            } catch (const TestCaseFramework::TestCaseFactoryException &e) {
                BEOLOG_ERROR(blog, e.what());
                taskHandler->addTask(boost::bind(&DUTTestController::transferData, this, failed, e.what()));
            } catch (const TestCaseFramework::TestCaseException &e) {
                BEOLOG_ERROR(blog, e.what());
                taskHandler->addTask(boost::bind(&DUTTestController::transferData, this, failed, e.what()));
            } catch (...) {
                BEOLOG_ERROR(blog, "Something went wrong in configuration of a test case.");
                taskHandler->addTask(boost::bind(&DUTTestController::transferData, this, failed, "An unknown exception was thrown."));
            }
        } else {
            const std::string message("Cannot configure a test case named:" + testName + ". A test case is already running on a DUT.");
            BEOLOG_ERROR(blog, message);
            transferData(failed, message);
        }
    }

    void DUTTestController::startTest() {
        if(!taskHandler->addTask(boost::bind(&DUTTestController::executeTest, this))) {
            transferData(failed, "A test case will not be executed because of a scheduling failure.");
        }
    }

    void DUTTestController::executeTest() {
        if(!testCase || testCaseThread != 0)  {
            const std::string message("Test case is not configured or it is already running on a DUT.");
            BEOLOG_ERROR(blog, message);
            throw DUTTestControllerException(message);
        } else {
            BEOLOG_INFO(blog, "executeTest created new test case thread and started its execution");          
            testCaseThread = new boost::thread(boost::bind(&TestCaseFramework::ATestCase::execute, testCase, this));
        }
    }

    void DUTTestController::testExecutionEnd(const std::string& data) {
        if(testCaseThread) {
            if(testCaseThread->timed_join(boost::posix_time::seconds(WAIT_TIME_SEC))) {
                transferData(finished, data + ". Test case thread ended successfully." );
            } else {
                transferData(failed, data + ". Time out reached while waiting for a test case thread to end.");
            }
            delete testCaseThread; 
            testCaseThread = NULL;
        }
        delete testCase;
        testCase = NULL;
    }

    // debugData and resultData functions inherited from CallbackInterface
    void DUTTestController::debugData(const std::string& data) {
        taskHandler->addTask(boost::bind(&DUTTestController::transferData, this, in_progress, data));
    }
    
    void DUTTestController::resultData(const std::string& data) {
        taskHandler->addTask(boost::bind(&DUTTestController::testExecutionEnd, this, data));
    }
}
