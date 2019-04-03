#include "Keycode.h"
#include "Keypress.h"
#include <array>
#include <utility>

#ifndef VKEY
#define VKEY

#define DEBOUNCE_TIME 1 //1*10ms = 10ms debounce time by default

using Activations = std::array<std::pair<Keycode, Keypress>, 3>;

//a thin wrapper around the activations
//the class has no notion of layers or the delta
class VKey
{
    private: 
        Activations activations;

    public:

        VKey() : activations {{ }} {}

        VKey(uint32_t keycode) 
            : activations {{ {keycode, {{ {false, DEBOUNCE_TIME} }} } }} { }

        VKey(Keypress k1, uint32_t a1) 
            : activations {{ {a1, k1} }} { }

        VKey(Keypress k1, uint32_t a1, Keypress k2, uint32_t a2) 
            : activations {{ {a1, k1}, {a2, k2} }} { }

        VKey(Keypress k1, uint32_t a1, Keypress k2, uint32_t a2, Keypress k3, uint32_t a3)
            : activations {{ {a1, k1}, {a2, k2}, {a3, k3} }} { }

        void press(unsigned long delta, bool wasPress)
        {
            for (auto& p : activations)
            {
                p.second.press(delta, wasPress);
            }
        }

        void clear(unsigned long delta, bool wasPress)
        {
            for (auto& p : activations)
            {
                p.second.clear(delta, wasPress);
            }
        }

        Keycode getKeycode()
        {
            for (auto& p : activations)
            {
                if (p.second.isActive())
                {
                    return p.first;
                }
            }   

            return {};
        }
};

#endif