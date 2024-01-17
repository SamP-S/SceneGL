
template<typename T>
T* Component::GetComponent() {
    return entity->GetComponent<T>();
}

template<typename T>
T* Component::GetComponentInChildren() {
    return entity->GetComponentInChildren<T>();
}

template<typename T>
T* Component::GetComponentInParent() {
    return entity->GetComponentInParent<T>();
}

template<typename T>
std::vector<T*> Component::GetComponents() {
    return entity->GetComponents<T>();
}

template<typename T>
std::vector<T*> Component::GetComponentsInChildren() {
    return entity->GetComponentsInChildren<T>();
}

template<typename T>
std::vector<T*> Component::GetComponentsInParent() {
    return entity->GetComponentsInParent<T>();
}
