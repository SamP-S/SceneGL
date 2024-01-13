/// TODO: Switch to references instead of pointers

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

template<typename T>
T* Entity::GetComponentInChildren() {
    T* childComp = GetComponent<T>();
    if (childComp != nullptr) {
        return childComp;
    }

    for (auto child : _children) {
        childComp = child->GetComponentInChildren<T>();
        if (childComp != nullptr) {
            return childComp;
        }
    }
    return nullptr;
}

template<typename T>
std::vector<T*> Entity::GetComponentsInChildren() {
    std::vector<T*> results = GetComponents<T>();
    for (auto child : _children) {
        std::vector<T*> childComponents = child->GetComponentsInChildren<T>();
        results.insert(results.end(), childComponents.begin(), childComponents.end());
    }
    return results;
}