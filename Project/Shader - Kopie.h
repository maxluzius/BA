#pragma once

#include "GL\glew.h"

/**
 * @brief The Shader class
 * Provides GLSL Shader functionality
 */
class Shader{
public:
    /**
     * @brief Shader ctr
     */
    Shader();

    /**
     * @brief Shader ctr
     * @param vsFile vertex
     * @param fsFile fragment
     */
    Shader(const GLchar* vsFile, const GLchar* fsFile);

    /**
     * @brief Shader ctr
     * @param vsFile vertex
     * @param gsFile geometry
     * @param fsFile fragment
     */
    Shader(const GLchar* vsFile, const GLchar* gsFile, const GLchar* fsFile);

    /**
     * @brief Shader ctr
     * @param vsFile vertex
     * @param tcsFile tesselation control
     * @param tesFile tesselation eval
     * @param fsFile fragment
     */
    Shader(const GLchar* vsFile, const GLchar* tcsFile, const GLchar* tesFile, const GLchar* fsFile);

    /**
     * @brief Shader ctr
     * @param vsFile vertex
     * @param tcsFile tesselation control
     * @param tesFile tesselation eval
     * @param gsFile geometry
     * @param fsFile fragment
     */
    Shader(const GLchar *vsFile, const GLchar *tcsFile, const GLchar *tesFile, const GLchar* gsFile, const GLchar *fsFile);

    /**
     * @brief Shader
     * @param csFile compute shader
     */
    Shader(const GLchar* csFile);

    /**
     * dtr
     */
    ~Shader();

    /**
     * @brief bind the shader program
     */
    void bind();

    /**
     * @brief unbind the shader program
     */
    void unbind();

    GLuint id();

private:
    GLuint shader_id;
    GLuint shader_vp;
    GLuint shader_gs;
    GLuint shader_fp;
    GLuint shader_cs;
    GLuint shader_tcs;
    GLuint shader_tes;

    void checkCompileErrors(GLuint id);
    void checkLinkErrors(GLuint id);

    void init(const GLchar* vsFile, const GLchar* fsFile);
    void init(const GLchar* vsFile, const GLchar* gsFile, const GLchar* fsFile);
    void init(const GLchar* vsFile, const GLchar* tcsFile, const GLchar* tesFile, const GLchar* fsFile);
    void init(const GLchar *vsFile, const GLchar *tcsFile, const GLchar *tesFile, const GLchar* gsFile, const GLchar *fsFile);
    void init(const GLchar* csFile);
};


