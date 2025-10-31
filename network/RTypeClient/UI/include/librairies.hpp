#ifndef LIBRAIRIES
    #define LIBRAIRIES
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include <vector>
#include <libconfig.h++>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <boost/asio.hpp>
#include <deque>
#include <atomic>
#include <thread>
#include <queue>
#include <mutex>
#include <optional>


enum class ElementTag { TEXT, BUTTON, RECTANGLE, CIRCLE, INPUTFIELD, TEXTURE };
enum class ButtonState { NORMAL, HOVER, CLICKED };
using boost::asio::ip::tcp;
namespace asio = boost::asio;
#endif