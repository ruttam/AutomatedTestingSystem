// Copyright © 2017 Ruta Tamasauskaite.
// This software is licensed and distributed under a GNU Lesser General Public License version 3.

#ifndef TASK_QUEUE_HPP
#define TASK_QUEUE_HPP

#include <boost/thread.hpp>
#include <list>

namespace TaskManagement {

    //! @brief A class that controls shared task queue - implements consumer-producer logic.
    class TaskQueue {
        public:
            typedef boost::function<void()> Task;

            TaskQueue();

            ~TaskQueue();

            //! @brief Add a new task to the beginning of the task queue.
            //! @param task A parameter of boost::function<void()> type to insert.
            //! @return True - if task was inserted successfully, otherwise - False.
            bool insertTask(const Task& task);

            //! @brief Retrieve task from the end of the task queue.
            //! @param task A reference parameter of boost::function<void()> type for retrieving a task.
            //! @return True - if task was extracted successfully, otherwise - False.
            bool extractTask(Task& task);

            //! @brief Interrupt processes of insertion or extraction of the task
            //! in case of execution's termination.
            //! Sets an interruptionProcess identifier value to True.
           void interruptTaskQueue();

            //! @brief Retrieve a value of the interruptProcess member variable.
            //! @return Value of interruption call indentifier - interruptProcess.
            bool isInterruptingProcess() const;
            
        protected:
            //! @brief A list of objects - tasks - of type boost::function<void()>.
            std::list<Task> taskList;

            //! @brief A capacity limit of a task list.
            static const int TASK_QUEUE_CAPACITY_LIMIT = 25;

        private:
            //! @brief A mutual exclusion synchronization primitive that
            //! controls access to the shared data.
            boost::mutex mx;
            
            //! @brief A synchronization primitive that is used to block threads.
            boost::condition_variable condition;

            //! @brief An indentifier for interruption of the processes in a task queue.
            boost::atomic<bool> interruptProcess;
    };
}

#endif
