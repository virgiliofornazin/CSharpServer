#pragma once

using namespace System;

namespace CSharpServer {

    template<typename T>
    ref class Embedded
    {
        T* t;

        !Embedded()
        {
            if (t != nullptr)
            {
                delete t;
                t = nullptr;
            }
        }

        ~Embedded()
        {
            this->!Embedded();
        }

    public:
        Embedded() : t(nullptr) {}
        Embedded(T* t) : t(t) {}

        property T& Value { T& get() { return *t; } }

        void Assign(T* t) { this->t = t; }
        void Release() { this->!Embedded(); }

        static T* operator&(Embedded% e) { return e.t; }
        static T* operator->(Embedded% e) { return e.t; }
    };

}
