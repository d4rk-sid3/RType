#ifndef UTILS
    #define UTILS
    #include "./librairies.hpp"

inline std::string getDate()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time_t);
    std::stringstream ss;

    ss << std::put_time(local_time, "%d/%m/%Y");
    return ss.str();
}

inline std::string getHour()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time_t);
    std::stringstream ss;

    ss << std::put_time(local_time, "%H:%M:%S");
    return ss.str();
}
#endif