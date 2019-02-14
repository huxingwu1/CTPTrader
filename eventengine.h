#ifndef EVENTENGINE_H
#define EVENTENGINE_H

#include <QObject>
#include <QTimer>
#include <thread>    //lock()
#include <QThread>

#include "CallBack.h"
#include "cppqueue.h"

/*
 * __queue  包含事件Event的双端队列
 * __handlers 字典，key是string，value是含CallBack指针的队列
 * CallBack的成员变量slotimpl有两个变量，类T的指针和T的成员函数
 */

class EventEngine : public QThread
{
    Q_OBJECT
public:
    template<typename T>
    void addEvent(std::string type, T* pObj, void (T::*pMemberFunc)(Event));

    void ee_begin();
    void ee_stop();
    void putEvent(Event event);
    void removeEvent(std::string type);



private slots:
    void onTimer();
private:
    void run();
    void process(Event event);

private:
    QTimer* __timer {nullptr};
    ConcurrentQueue<Event> __queue;    //包含Event的双端队列deque
    std::atomic<bool> __active_put{ false };
    std::atomic<bool> __active_thread{ false };
    std::mutex handlers_mutex;         //互斥量
    //事件类型：回调函数，消费者加入处理事件的回调函数
    std::map < std::string, std::list<std::shared_ptr<CallBack>>> __handlers;    //list  一个事件对应多个回调（多个回调函数处理这个事件）
};

template<typename T>
void EventEngine::addEvent(std::string type, T *pObj, void (T::*pMemberFunc)(Event))
{
    std::lock_guard<std::mutex> lock(handlers_mutex);

    // 判断event_type是否存在，不存在则初始化map
    if (__handlers.find(type) == __handlers.end())
    {
        std::list<std::shared_ptr<CallBack>> list = {};
        __handlers[type] = list;
    }

    // 判断 pObj + T::*pMemberFunc 是否已经包含在 event_type 的回调列表中，已存在则直接return
    // 但无法判断是否已包含 pObj + T::*pMemberFunc ， 需要程序员人工保证

    __handlers[type].push_back(std::make_shared<CallBack>(pObj, pMemberFunc));
}

#endif // EVENTENGINE_H
