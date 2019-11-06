#ifndef GDG_COMMON_TOOLS_SYNC_QUEUE_HPP_
#define GDG_COMMON_TOOLS_SYNC_QUEUE_HPP_

//
// Copyright (c) 2013 Juan Palacios juan.palacios.puyana@gmail.com
// Subject to the BSD 2-Clause License
// - see < http://opensource.org/licenses/BSD-2-Clause>
//
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

namespace gdg
{
template <typename T>
class sync_queue
{
public:
    sync_queue() = default;
    ~sync_queue() noexcept
    {
        notify_finishing();
        std::unique_lock<std::mutex> mlock{ m_Mutex };
    }

    sync_queue(const sync_queue&) = delete;
    sync_queue& operator=(const sync_queue&) = delete;
    sync_queue(sync_queue&&) = default;
    sync_queue& operator=(sync_queue&&) = default;

    ///
    ///
    ///
    [[nodiscard]] bool pop(T &item)
    {
        std::unique_lock<std::mutex> mlock(m_Mutex);
        while (m_Queue.empty())
        {
            if (m_Finishing)
                return false;
            else
                m_Condition.wait(mlock);
        }
        item = m_Queue.front();
        m_Queue.pop();
        return true;
    }

    ///
    ///
    ///
    [[nodiscard]] bool push(const T& item)
    {
        std::unique_lock<std::mutex> mlock(m_Mutex);
        if (m_Finishing)
            return false;
        m_Queue.push(item);
        mlock.unlock();
        m_Condition.notify_one();
        return true;
    }

    ///
    ///
    ///
    void notify_finishing()
    {
        std::unique_lock<std::mutex> mlock(m_Mutex);
        m_Finishing = true;
        m_Condition.notify_all();
    }

private:
    std::queue<T> m_Queue;
    std::mutex m_Mutex;
    std::condition_variable m_Condition;
    std::atomic_bool m_Finishing{ false };
};

} // gdg

#endif /*GDG_COMMON_TOOLS_SYNC_QUEUE_HPP_*/
