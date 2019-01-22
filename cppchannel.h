//
//  Channel.hpp
//  app_core
//
//  Created by Prince Kumar on 1/21/19.
//  Copyright © 2019 Prince Kumar. All rights reserved.
//

#ifndef CPPChannel_hpp
#define CPPChannel_hpp

#include <list>
#include <thread>
#include <memory>

template<class item>
class CPPChannel {
private:
    std::list<unique_ptr<item>> queue;
    std::mutex m;
    std::condition_variable cv;
    bool closed;
public:
    CPPChannel() : closed(false) { }
    void close() {
        std::unique_lock<std::mutex> lock(m);
        closed = true;
        cv.notify_all();
    }
    bool is_closed() {
        std::unique_lock<std::mutex> lock(m);
        return closed;
    }
    void put(unique_ptr<item> i) {
        std::unique_lock<std::mutex> lock(m);
        if(closed)
            throw std::logic_error("put to closed channel");
        queue.push_back(std::move(i));
        cv.notify_one();
    }
    unique_ptr<item> get(bool &success, bool wait = true) {
        std::unique_lock<std::mutex> lock(m);
        if(wait)
            cv.wait(lock, [&](){ return closed || !queue.empty(); });
        if(queue.empty()) {
            success = false;
            return std::unique_ptr<item>(nullptr);
        }
        success = true;
        auto ret = std::move(queue.front());
        //out = queue.front();
        queue.pop_front();
        return std::move(ret);
    }
};


#endif /* CPPChannel_hpp */

