#ifndef CLIENTGRAPHICS
    #define CLIENTGRAPHICS
    #include "./ClientUI.hpp"
    #include "./UIRenderer.hpp"

class ClientGraphics {
    public:
        ClientUI clientUI;
        UIRenderer renderer;

        ClientGraphics();
        ~ClientGraphics();
        void run();
};

#endif