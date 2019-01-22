#include <iostream>       // std::cout
#include <thread>         // std::thread
#include "../cppchannel.h"
#include <memory>
#include <string>

CPPChannel<std::string> channel;

void foo()
{
    bool success = true;
    while (success) {
        auto str = channel.get(success, true);
        if (success && str != nullptr)
            std::cout << "received : " << *str << std::endl;
    }
    
}

int main()
{
    
    
    std::thread first (foo);     // spawn new thread that calls foo()
    
    for (int i = 0; i < 10; i++) {
        std::unique_ptr<std::string> str(new std::string("test"));
        channel.put(std::move(str));
    }
    
    channel.close();
    
    first.join();                // pauses until first finishes
    
    return 0;
}
