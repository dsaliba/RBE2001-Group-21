#pragma once

class Gripper {
    public:
        void init();
        void update();
        void write(int us);
        int read();

        void up();
        void down();

};