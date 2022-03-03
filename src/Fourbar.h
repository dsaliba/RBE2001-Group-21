#pragma once

class Fourbar {
    public:
        void init();
        void update();
        void setPosition(long pos);
        long getPosition();
        void pause();
        void setEffort(int effort);
};