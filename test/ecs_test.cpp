// std libs
#include <iostream>
#include <string>

// internal libs
#include "ecs/ngine.hpp"

// we have two example components here
struct PositionComponent {
    float x;
    float y;
};

struct VelocityComponent {
    float dx;
    float dy;
};

void test_ngine() {
    std::cout << "test_ngine" << std::endl;
    Scene scene = Scene();

    // we'll create 10 entities 
    for(std::size_t i = 0; i < 10; ++i) {
        std::string name = "cube_" + std::to_string(i);
        Entity entity = scene.CreateEntity(name);
        entity.AddComponent<PositionComponent>(i * 1.f, i * 1.f);
        if(i % 2 == 0) { 
            entity.AddComponent<VelocityComponent>(i * .1f, i * .1f); 
        }
    }

    std::vector<Entity> entities = scene.GetEntities();
    for(Entity entity : entities) {
        CoreComponent nc = entity.GetComponent<CoreComponent>();
        std::cout << uint32_t(entity) << ": \"" << nc.name;
        PositionComponent pc = entity.GetComponent<PositionComponent>();
        std::cout << "\" (pos=" << pc.x << "," << pc.y;
        if (entity.HasComponent<VelocityComponent>()) {
            VelocityComponent vc = entity.GetComponent<VelocityComponent>();
            std::cout << ", vel=" << vc.dx << "," << vc.dy;
        }
        std::cout << ")" << std::endl;
    }

    std::cout << "Find test: " << std::endl;
    Entity entity = scene.FindEntityByName("cube_4");
    std::cout << uint32_t(entity) << std::endl;
}

// void test_serializer() {
//     std::cout << "test_serializer" << std::endl;
//     std::shared_ptr<Scene> scene = std::make_shared<Scene>();
//     JsonSerializer js = JsonSerializer(scene);
//     js.Deserialize("./scene/Preset.json");
//     std::cout << "Loaded scene entity count: " << scene->GetEntityCount() << std::endl;
// }

int main() {
    test_ngine();
    // test_serializer();
}