template<typename T>
T* Entity::GetComponent() {
    for (auto component : _components) {
        if (dynamic_cast<T*>(component) != nullptr) {
            return dynamic_cast<T*>(component);
        }
    }
    return nullptr;
}

template<typename T>
std::vector<T*> Entity::GetComponents() {
    std::vector<T*> results  = std::vector<T*>();
    for (auto component : _components) {
        if (dynamic_cast<T*>(component) != nullptr) {
            results.push_back(dynamic_cast<T*>(component));
        }
    }
    return results;
}