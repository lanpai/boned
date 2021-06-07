#pragma once

class Screen {
    public:
        virtual ~Screen() { /* null */ };

        virtual void Loop() = 0;
        virtual void Input(char input) = 0;
};
