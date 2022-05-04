#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram(){ program = glCreateProgram(); }
        ~ShaderProgram(){ if(program != 0) glDeleteProgram(program); }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name) {
            return glGetUniformLocation(program, name.c_str());
        }

        void set(const std::string &uniform, GLfloat value) {
            glUniform1f(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLuint value) {
            glUniform1ui(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLint value) {
            glUniform1i(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, glm::vec2 value) {
            glUniform2f(getUniformLocation(uniform), value.x, value.y);
        }

        void set(const std::string &uniform, glm::vec3 value) {
            glUniform3f(getUniformLocation(uniform), value.x, value.y, value.z);
        }

        void set(const std::string &uniform, glm::vec4 value) {
            glUniform4f(getUniformLocation(uniform), value.x, value.y, value.z, value.w);
        }

        void set(const std::string &uniform, glm::mat4 matrix) {
            glUniformMatrix4fv(getUniformLocation(uniform), 1, false, glm::value_ptr(matrix));
        }

        ShaderProgram(ShaderProgram const &) = delete;
        ShaderProgram &operator=(ShaderProgram const &) = delete;
    };

}

#endif