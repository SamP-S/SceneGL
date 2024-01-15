#pragma once

#include <iostream>
#include <string>

#include "json.hpp"
using namespace nlohmann;

typedef int ObjId;

class Object {

    private:
        ObjId _id;
        static ObjId _nextId;

    public:
        Object();
        ObjId GetId();

        virtual std::string Type() = 0;
        virtual void FromJson(json j) = 0;
        virtual json ToJson() = 0;
        virtual std::string ToString() = 0;

        bool operator==(const Object& cmp) const;
        bool operator!=(const Object& cmp) const;

        // static void Destroy(Object* obj);
        // static Object* Instantiate(Object* obj);

        // template<typename T>
        // static T* FindAnyObjectByType();
        // template<typename T>
        // static T* FindFirstObjectByType();
        // template<typename T>
        // static T* FindObjectOfType();
        // template<typename T>
        // static std::vector<T*> FindObjectsByType();
        // template<typename T>
        // static std::vector<T*> FindObjectsOfType();
};