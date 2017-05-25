// Copyright © 2017 Ruta Tamasauskaite.
// This software is licensed and distributed under a GNU Lesser General Public License version 3.

#include <ATS/TaskHandler.hpp>
#include <ATS/TaskQueue.hpp>

#include <BeoLog/BeoLog.hpp>

namespace {
    BeoLog::Logger blog("TaskManagement::TaskHandler");
}

namespace TaskManagement {

    TaskHandler::TaskHandler():taskQueue(new TaskQueue),terminate(false) { }

    TaskHandler::~TaskHandler() {
        terminateHandler();
        delete taskQueue;
    }

    bool TaskHandler::addTask(const TaskQueue::Task t) {
        if(taskQueue->insertTask(t)) {
            const std::string message("A task is inserted successfully.");
            BEOLOG_INFO(blog, message);
            return true;
        } else {
            const std::string message("Task handler encountered an error while trying to insert a task.");
            BEOLOG_ERROR(blog, message);
            return false;
        }
    }

    void TaskHandler::manageTasks() {
        while(!terminate) {
            TaskQueue::Task currentTask;
            if(TaskHandler::taskQueue->extractTask(currentTask)) {
                try {   
                    currentTask();
                } catch(std::exception &e) {
                    BEOLOG_ERROR(blog, e.what());
                }
            } else {
                const std::string message("Task handler encountered an error while trying to extract a task.");
                BEOLOG_ERROR(blog, message);
            }
        }
    }

    void TaskHandler::interrupt() {
        taskQueue->interruptTaskQueue();
    }

    void TaskHandler::terminateHandler() {
        terminate.store(true);
        interrupt();
    }
}
