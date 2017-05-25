// Copyright © 2017 Ruta Tamasauskaite.
// This software is licensed and distributed under a GNU Lesser General Public License version 3.

#include <ATS/TaskQueue.hpp>

namespace TaskManagement {

    TaskQueue::TaskQueue():taskList(),mx(),condition(),interruptProcess(false) { }

    TaskQueue::~TaskQueue() {
        interruptTaskQueue();
        taskList.clear();
    }

    bool TaskQueue::insertTask(const TaskQueue::Task& task) {
        boost::unique_lock<boost::mutex> lock(mx);
        while(taskList.size() >= TASK_QUEUE_CAPACITY_LIMIT && !interruptProcess) {
            condition.wait(lock);
        }
        if(interruptProcess) {
            return false;
        }
        taskList.push_front(task);
        condition.notify_one();
        return true;
    }

    bool TaskQueue::extractTask(TaskQueue::Task &task) {
        boost::unique_lock<boost::mutex> lock(mx);
        while(taskList.size() == 0 && !interruptProcess) {
            condition.wait(lock);
        }
        if(interruptProcess) {
            return false;
        }
        task = taskList.back();
        taskList.pop_back();
        condition.notify_one();
        return true;
    }

    void TaskQueue::interruptTaskQueue() {
        boost::unique_lock<boost::mutex> lock(mx);
        interruptProcess.store(true);
        condition.notify_one();
    }

    bool TaskQueue::isInterruptingProcess() const {
        return interruptProcess.load();
    }
}
