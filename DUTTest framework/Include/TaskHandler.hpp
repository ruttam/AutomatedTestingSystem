// Copyright © 2017 Ruta Tamasauskaite.
// This software is licensed and distributed under a GNU Lesser General Public License version 3.

#ifndef TASK_HANDLER_HPP
#define TASK_HANDLER_HPP

#include <boost/thread.hpp>

#include <ATS/TaskQueue.hpp>

namespace TaskManagement {

    //! @brief A class that manages tasks from a task queue.
    class TaskHandler {
        public:
            TaskHandler();

            ~TaskHandler();

            //! @brief Add a new task to the task queue.
            //! @param task A parameter of boost::function<void()> type to insert.
            //! @return True - if a task was added successfully, otherwise - False.
            bool addTask(const TaskQueue::Task newTask);

            //! @brief Extract test case from a test case factory
            //! and execute/run it.
            void manageTasks();
            
            //! @brief Set an identifier to stop handling tasks.
            //! Sets a terminate identifier value to True.
            void terminateHandler();

        protected:
            //! @brief A pointer to a TaskQueue object.
            TaskQueue * taskQueue;

            boost::atomic<bool> terminate;

        private:
            //! @brief An indentifier for interrupting a therad of a task handler.
            void interrupt();
    };
}

#endif
