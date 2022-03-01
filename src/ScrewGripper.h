#pragma once

class ScrewGripper {
    public:
        void init();
        void update();
        void write(int us);
        int read();
};