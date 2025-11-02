#ifndef CLIENTGRAPHICS
    #define CLIENTGRAPHICS
    #include "./ClientUI.hpp"
    #include "./UIRenderer.hpp"
    #include "../../Network/ClientTCP/include/ThreadSafeQueue.hpp"
    #include "../../Managers/ActionRegistry.hpp"

class ClientGraphics {
    public:
        ClientUI clientUI;
        UIRenderer renderer;
        std::shared_ptr<ThreadSafeQueue> _queue;
        bool _isAdmin = false;

        ClientGraphics(std::shared_ptr<ThreadSafeQueue> queue);
        void handleNetworkEvent(const std::string &string);
        void loadPages();
        ~ClientGraphics();
        bool isAdmin();
        void run();
};
#endif