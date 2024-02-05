#pragma once

class RendererAPI {
public:
    enum class API {
        NONE = 0,
        OPENGL
    };

    static API GetAPI() {
        return _api;
    }

private:
    RendererAPI() = delete;
    RendererAPI(RendererAPI const&) = delete;
    void operator=(RendererAPI const&) = delete;

    static inline API _api = API::OPENGL;
};

