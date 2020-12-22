#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

enum id_judgement
{
    id_unpickable = 0,
    id_pickable
};

// this may be in use when we add GUI or some other things

class EventHandler
{
    //virtual ~EventHandler();

    virtual void OnEvent() = 0;
};

#endif