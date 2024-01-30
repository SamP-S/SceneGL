#include "platform/opengl/opengl_shader.hpp"

OpenGLShader::OpenGLShader(std::string name, std::shared_ptr<ShaderSource> vs, std::shared_ptr<ShaderSource> fs)
    : Shader(name, vs, fs) {
    Compile();
}

OpenGLShader::~OpenGLShader() {
    glDeleteProgram(_programId);
}

bool OpenGLShader::IsUsable() const {
    return _validShader;
}

void OpenGLShader::Compile() {
    _validShader = false;
    if (vs == nullptr) {
        std::cout << "WARNING (Shader): Can't compile shader with null vertex shader source." << std::endl;
        return;
    }
    if (fs == nullptr) {
        std::cout << "WARNING (Shader): Can't compile shader with null fragment shader source." << std::endl;
        return;
    }
    if (vs->stage != ShaderStage::VERTEX || fs->stage != ShaderStage::FRAGMENT) {
        std::cout << "WARNING (Shader): Trying to compile non-matching stages." << std::endl;
        return;
    }

    // compile vertex shader
    uint32_t vertex = 0;
    try {
        vertex = vs->Compile();
    } catch (const char* err) {
        std::cout << "Error (OpenGLShader): VERTEX shader compile failed:" << std::endl;
        std::cout << vs->name << std::endl;
        std::cout << err << std::endl;
        return;
    }

    // compile fragment shader
    uint32_t fragment = 0;
    try {
        fragment = fs->Compile();
    } catch (const char* err) {
        std::cout << "Error (OpenGLShader): FRAGMENT shader compile failed:" << std::endl;
        std::cout << fs->name << std::endl;
        std::cout << err << std::endl;
        return;
    }

    // link shader program
    _programId = glCreateProgram();
    glAttachShader(_programId, vertex);
    glAttachShader(_programId, fragment);
    glBindFragDataLocation(_programId, 0, "oColour");
    glLinkProgram(_programId);

    // free shaders as they are copied on linking
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    // validate shader program correct
    int success;
    static char infoLog[1024];
    glGetProgramiv(_programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_programId, 1024, NULL, infoLog);
        std::cout << "Error (OpenGLShader): Shader program linking failed:" << std::endl;
        std::cout << name << std::endl;
        std::cout << infoLog << std::endl;
        return;
    }

    // success
    _validShader = true;
}

void OpenGLShader::Bind() const {
    if (!IsUsable()) {
        std::cout << "WARNING (OpenGLShader): Cannot bind invalid shader." << std::endl;
        return;
    }
    glUseProgram(_programId);
}

void OpenGLShader::Unbind() const {
    glUseProgram(0);
}

void OpenGLShader::SetBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(_programId, name.c_str()), value);
}

void OpenGLShader::SetInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(_programId, name.c_str()), value);
}

void OpenGLShader::SetUint(const std::string& name, uint32_t value) const {
    glUniform1ui(glGetUniformLocation(_programId, name.c_str()), value);
}

void OpenGLShader::SetFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(_programId, name.c_str()), value);
}

void OpenGLShader::SetVec2(const std::string& name, const LA::vec2& v) const {
    glUniform2f(glGetUniformLocation(_programId, name.c_str()), v.x, v.y);
}

void OpenGLShader::SetVec2(const std::string& name, float x, float y) const {
    glUniform2f(glGetUniformLocation(_programId, name.c_str()), x, y);
}

void OpenGLShader::SetVec3(const std::string& name, const LA::vec3& v) const {
    glUniform3f(glGetUniformLocation(_programId, name.c_str()), v.x, v.y, v.z);
}

void OpenGLShader::SetVec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(_programId, name.c_str()), x, y, z);
}

void OpenGLShader::SetVec4(const std::string& name, const LA::vec4& v) const {
    glUniform4f(glGetUniformLocation(_programId, name.c_str()), v.x, v.y, v.z, v.w);
}

void OpenGLShader::SetVec4(const std::string& name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(_programId, name.c_str()), x, y, z, w);
}

void OpenGLShader::SetMat2(const std::string& name, const LA::mat2& m) const {
    glUniformMatrix2fv(glGetUniformLocation(_programId, name.c_str()), 1, GL_FALSE, &(m)[0][0]);
}

void OpenGLShader::SetMat2(const std::string& name, float* mPtr) const {
    glUniformMatrix2fv(glGetUniformLocation(_programId, name.c_str()), 1, GL_FALSE, mPtr);
}

void OpenGLShader::SetMat3(const std::string& name, const LA::mat3& m) const {
    glUniformMatrix3fv(glGetUniformLocation(_programId, name.c_str()), 1, GL_FALSE, &(m)[0][0]);
}

void OpenGLShader::SetMat3(const std::string& name, float* mPtr) const {
    glUniformMatrix3fv(glGetUniformLocation(_programId, name.c_str()), 1, GL_FALSE, mPtr);
}

void OpenGLShader::SetMat4(const std::string& name, const LA::mat4& m) const {
    glUniformMatrix4fv(glGetUniformLocation(_programId, name.c_str()), 1, GL_FALSE, &(m)[0][0]);
}

void OpenGLShader::SetMat4(const std::string& name, float* mPtr) const {
    glUniformMatrix4fv(glGetUniformLocation(_programId, name.c_str()), 1, GL_FALSE, mPtr);
}
