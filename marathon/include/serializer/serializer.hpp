#pragma once

// std libs
#include <memory>
#include <string>
#include <iostream>
#include <fstream>

// external libs
#include "json.hpp"
using namespace nlohmann;

// internal libs
#include "la_extended.h"
#include "ecs/ngine.hpp"
#include "renderer/components.hpp"
#include "platform/opengl/opengl_mesh.hpp"
#include "platform/opengl/opengl_material.hpp"

//// TODO:
// Might want to replace with Method Injection

//// NOTES:
// Using Dependancy Injection, passing in the scene to the serializer
// Should create a serializer per scene you want to serialize.
// Scene reference is the only state this class should have.

class SceneSerializer {
public:
    SceneSerializer(Scene& scene)
        : _scene(scene) {}

    // output scene to json file
    void Serialize(const std::string &filepath) {
        json j = json::array();
        for (auto e : _scene.GetEntities()) {
            j.push_back(SerializeEntity(e));
        }
        SaveJson(filepath, j);
    }

    // read in json file to scene
    void Deserialize(const std::string &filepath) {
        _scene.Clear();
        json j = LoadJson(filepath);
        for (const auto &entityJson : j) {
            DeserializeEntity(entityJson);
        }
    }

private:
    Scene& _scene;
    AssetManager& _assetManager = AssetManager::Instance();

    json SerializeEntity(Entity e) {
        json j;
        // create entity
        CoreComponent& cc = e.GetComponent<CoreComponent>();
        j["name"] = cc.name;
        // extract transform
        TransformComponent& tc = e.GetComponent<TransformComponent>();
        j["transform"]["position"] = SerializeVec3(tc.position);
        j["transform"]["rotation"] = SerializeVec3(tc.rotation);
        j["transform"]["scale"] = SerializeVec3(tc.scale);

        // iterate through non-essential components
        j["components"] = json::array();
        
        if (e.HasComponent<DirectionalLightComponent>()) {
            json next;
            DirectionalLightComponent& dlc = e.GetComponent<DirectionalLightComponent>();
            next["directionalLight"]["colour"] = SerializeColour3(dlc.colour);
            next["directionalLight"]["intensity"] = dlc.intensity;
            j["components"].push_back(next);
        }

        if (e.HasComponent<PointLightComponent>()) {
            json next;
            PointLightComponent& plc = e.GetComponent<PointLightComponent>();
            next["pointLight"]["colour"] = SerializeColour3(plc.colour);
            next["pointLight"]["intensity"] = plc.intensity;
            next["pointLight"]["range"] = plc.range;
            j["components"].push_back(next);
        }

        if (e.HasComponent<SpotLightComponent>()) {
            json next;
            SpotLightComponent& slc = e.GetComponent<SpotLightComponent>();
            next["spotLight"]["colour"] = SerializeColour3(slc.colour);
            next["spotLight"]["intensity"] = slc.intensity;
            next["spotLight"]["cutOff"] = slc.cutOff;
            next["spotLight"]["outerCutOff"] = slc.outerCutOff;
            j["components"].push_back(next);
        }

        if (e.HasComponent<MeshRendererComponent>()) {
            json next;
            MeshRendererComponent& mrc = e.GetComponent<MeshRendererComponent>();
            if (mrc.mesh != nullptr)
                next["meshRenderer"]["meshName"] = mrc.mesh->name;
            if (mrc.material != nullptr)
                next["meshRenderer"]["materialName"] = mrc.material->name;
            j["components"].push_back(next);
        }

        if (e.HasComponent<CameraComponent>()) {
            json next;
            CameraComponent &cc = e.GetComponent<CameraComponent>();
            next["camera"]["fov"] = cc.fov;
            next["camera"]["near"] = cc.near;
            next["camera"]["far"] = cc.far;
            j["components"].push_back(next);
        }
        return j;
    }
        

    void DeserializeEntity(json j) {
        // create entity
        std::string name = j["name"];
        Entity entity = _scene.CreateEntity(name);
        // extract transform
        TransformComponent &transform = entity.GetComponent<TransformComponent>();
        json transformJson = j["transform"];
        transform.position = DeserializeVec3(transformJson["position"]);
        transform.rotation = DeserializeVec3(transformJson["rotation"]);
        transform.scale = DeserializeVec3(transformJson["scale"]);

        // iterate through non-essential components
        json components = j["components"];
        for (auto &[key, value] : components.items()) {
            if (value.contains("directionalLight")) {
                DirectionalLightComponent &plc = entity.AddComponent<DirectionalLightComponent>();
                plc.colour = DeserializeColour3(value["directionalLight"]["colour"]);
                plc.intensity = value["directionalLight"]["intensity"];

            } else if (value.contains("pointLight")) {
                PointLightComponent &plc = entity.AddComponent<PointLightComponent>();
                plc.colour = DeserializeColour3(value["pointLight"]["colour"]);
                plc.intensity = value["pointLight"]["intensity"];
                plc.range = value["pointLight"]["range"];

            } else if (value.contains("spotLight")) {
                SpotLightComponent &slc = entity.AddComponent<SpotLightComponent>();
                slc.colour = DeserializeColour3(value["spotLight"]["colour"]);
                slc.intensity = value["spotLight"]["intensity"];
                slc.cutOff = value["spotLight"]["cutOff"];
                slc.outerCutOff = value["spotLight"]["outerCutOff"];

            } else if (value.contains("meshRenderer")) {
                MeshRendererComponent &mrc = entity.AddComponent<MeshRendererComponent>();
                try {
                    std::string meshName = value["meshRenderer"]["meshName"];
                    mrc.mesh = _assetManager.FindAsset<OpenGLMesh>(meshName);
                } catch (const std::exception& e) {
                    std::cout << "WARNING (Serializer): mesh name bad." << std::endl;
                    std::cout << e.what() << std::endl;
                }

                try {
                    std::string materialName = value["meshRenderer"]["materialName"];
                    mrc.material = _assetManager.FindAsset<OpenGLMaterial>(materialName);
                } catch (const std::exception& e) {
                    std::cout << "WARNING (Serializer): material name bad." << std::endl;
                    std::cout << e.what() << std::endl;
                }

            } else if (value.contains("camera")) {
                CameraComponent &cc = entity.AddComponent<CameraComponent>();
                cc.fov = value["camera"]["fov"];
                cc.near = value["camera"]["near"];
                cc.far = value["camera"]["far"];
            } else {
                std::cout << "WARNING (SceneSerializer): Trying to deserializer unknown component:" << std::endl;
                std::cout << value << std::endl;
            }
        }
    }

    json LoadJson(const std::string &filepath)
    {
        std::ifstream inputFile(filepath);
        if (!inputFile.good())
        {
            std::cout << "WARNING (SceneSerializer): Scene file does not exist: " << filepath << std::endl;
            return json();
        }
        if (!inputFile.is_open())
        {
            std::cout << "WARNING (SceneSerializer): Can't open file: " << filepath << std::endl;
            return json();
        }
        json jsonFile = json::parse(inputFile);
        inputFile.close();
        return jsonFile;
    }

    bool SaveJson(const std::string &filepath, json jsonData)
    {
        std::ofstream outputFile(filepath);
        if (!outputFile.is_open())
        {
            std::cout << "WARNING (SceneSerializer): Can't open file: " << filepath << std::endl;
            return false;
        }
        outputFile << jsonData.dump(4); // Write the JSON data to the file with indentation of 4 spaces
        outputFile.close();
        return true;
    }

    json SerializeColour3(const LA::vec3& v) {
        json j;
        j["r"] = v.r;
        j["g"] = v.g;
        j["b"] = v.b;
        return j;
    }

    json SerializeColour4(const LA::vec4& v) {
        json j;
        j["r"] = v.r;
        j["g"] = v.g;
        j["b"] = v.b;
        j["a"] = v.a;
        return j;
    }

    json SerializeVec3(const LA::vec3& v) {
        json j;
        j["x"] = v.x;
        j["y"] = v.y;
        j["z"] = v.z;
        return j;
    }

    json SerializeVec4(const LA::vec4& v) {
        json j;
        j["x"] = v.x;
        j["y"] = v.y;
        j["z"] = v.z;
        j["w"] = v.w;
        return j;
    }

    LA::vec3 DeserializeColour3(json j) {
        return LA::vec3({j["r"], j["g"], j["b"]});
    }

    LA::vec3 DeserializeColour4(json j) {
        return LA::vec3({j["r"], j["g"], j["b"], j["a"]});
    }

    LA::vec3 DeserializeVec3(json j) {
        return LA::vec3({j["x"], j["y"], j["z"]});
    }

    LA::vec3 DeserializeVec4(json j) {
        return LA::vec3({j["x"], j["y"], j["z"], j["w"]});
    }
};
