#pragma once

// std libs
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
// internal libs
#include "ngine/ngine.hpp"
#include "tai/tai.hpp"
#include "platform/opengl/opengl_mesh.hpp"
// external libs
#include "json.hpp"
using namespace nlohmann;

namespace Ngine
{

    class ISerializer
    {
    public:
        Tai::AssetManager &assetManager = Tai::AssetManager::Instance();
        ISerializer(std::shared_ptr<Scene> scene)
            : _scene(scene) {}

        virtual void Serialize(const std::string &filepath) = 0;
        virtual void Deserialize(const std::string &filepath) = 0;

    protected:
        std::shared_ptr<Scene> _scene;
    };

    // note: should make a serializer per scene
    class JsonSerializer : public ISerializer
    {
    public:
        JsonSerializer(std::shared_ptr<Scene> scene)
            : ISerializer(scene) {}

        // output json file to scene
        void Serialize(const std::string &filepath) override
        {
            // not implemented
        }

        // read in json file to scene
        void Deserialize(const std::string &filepath) override
        {
            _scene->Clear();
            json j = LoadJson(filepath);
            for (const auto &entityJson : j)
            {
                DeserializeEntity(entityJson);
            }
        }

    private:
        LA::vec3 DeserializeColour(json j)
        {
            return LA::vec3({j["r"], j["g"], j["b"]});
        }

        LA::vec3 DeserializeVec3(json j)
        {
            return LA::vec3({j["x"], j["y"], j["z"]});
        }

        void DeserializeEntity(json j)
        {
            // create entity
            std::string name = j["name"];
            Entity entity = _scene->CreateEntity(name);
            // extract transform
            TransformComponent &transform = entity.GetComponent<TransformComponent>();
            json transformJson = j["transform"];
            transform.position = DeserializeVec3(transformJson["position"]);
            transform.rotation = DeserializeVec3(transformJson["rotation"]);
            transform.scale = DeserializeVec3(transformJson["scale"]);

            // iterate through non-essential components
            json components = j["components"];
            for (auto &[key, value] : components.items())
            {

                if (value.contains("directionalLight"))
                {
                    DirectionalLightComponent &plc = entity.AddComponent<DirectionalLightComponent>();
                    plc.colour = DeserializeColour(value["directionalLight"]["colour"]);
                    plc.intensity = value["directionalLight"]["intensity"];
                }
                else if (value.contains("pointLight"))
                {
                    PointLightComponent &plc = entity.AddComponent<PointLightComponent>();
                    plc.colour = DeserializeColour(value["pointLight"]["colour"]);
                    plc.intensity = value["pointLight"]["intensity"];
                    plc.range = value["pointLight"]["range"];
                }
                else if (value.contains("meshRenderer"))
                {
                    MeshRendererComponent &mrc = entity.AddComponent<MeshRendererComponent>();
                    std::string meshName = value["meshRenderer"]["meshName"];
                    std::shared_ptr<Mesh> mesh = assetManager.GetAsset<OpenGLMesh>(meshName);
                    if (mesh == nullptr)
                        mrc.mesh = 0;
                    else
                        mrc.mesh = mesh->id;
                }
                else if (value.contains("camera"))
                {
                    CameraComponent &cc = entity.AddComponent<CameraComponent>();
                    cc.fov = value["camera"]["fov"];
                    cc.near = value["camera"]["near"];
                    cc.far = value["camera"]["far"];
                }
                else
                {
                    std::cout << "WARNING (JsonSerializer): Trying to deserializer unknown component:" << std::endl;
                    std::cout << value << std::endl;
                }
            }
        }

        json LoadJson(const std::string &filepath)
        {
            std::ifstream inputFile(filepath);
            if (!inputFile.good())
            {
                std::cout << "WARNING (JsonSerializer): Scene file does not exist: " << filepath << std::endl;
                return json();
            }
            if (!inputFile.is_open())
            {
                std::cout << "WARNING (JsonSerializer): Can't open file: " << filepath << std::endl;
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
                std::cout << "WARNING (JsonSerializer): Can't open file: " << filepath << std::endl;
                return false;
            }
            outputFile << jsonData.dump(4); // Write the JSON data to the file with indentation of 4 spaces
            outputFile.close();
            return true;
        }
    };
}
