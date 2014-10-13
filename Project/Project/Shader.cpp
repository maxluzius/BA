#include "Shader.h"
#include <string>
#include <stdlib.h>

#include <iostream>

using namespace std;

static char* textFileRead(const GLchar *fileName) {
    char* text = NULL;
    
    if (fileName != NULL) {
        FILE *file = fopen(fileName, "rt");
        
        if (file != NULL) {
            fseek(file, 0, SEEK_END);
            GLint count = ftell(file);
            rewind(file);
            
            if (count > 0) {
                text = (GLchar*)malloc(sizeof(GLchar) * (count + 1));
                count = fread(text, sizeof(GLchar), count, file);
                text[count] = '\0';
            }
            fclose(file);
        }
    }
    return text;
}

Shader::Shader() {
    
}

Shader::Shader(const GLchar* csFile){
	init(csFile);
}

Shader::Shader(const GLchar *vsFile, const GLchar *fsFile) {
    init(vsFile, fsFile);
}

Shader::Shader(const GLchar *vsFile, const GLchar *gsFile, const GLchar *fsFile) {
    init(vsFile, gsFile, fsFile);
}

Shader::Shader(const GLchar *vsFile, const GLchar *tcsFile, const GLchar *tesFile, const GLchar *fsFile) {
    init(vsFile, tcsFile,tesFile, fsFile);
}

Shader::Shader(const GLchar *vsFile, const GLchar *tcsFile, const GLchar *tesFile, const GLchar* gsFile, const GLchar *fsFile)
{
	init(vsFile, tcsFile,tesFile,gsFile, fsFile);
}

void Shader::init(const GLchar *csFile) {
	shader_cs = glCreateShader(GL_COMPUTE_SHADER);
	const GLchar* csText = textFileRead(csFile);

	if (csText == NULL) {
        cerr << "Compute shader file not found." << endl;
        return;
    }

    glShaderSource(shader_cs, 1, &csText, 0);

	glCompileShader(shader_cs);
	checkCompileErrors(shader_cs);
	
	shader_id = glCreateProgram();
	
	glAttachShader(shader_id, shader_cs);

	glLinkProgram(shader_id);
	checkLinkErrors(shader_id);

	glDeleteShader(shader_cs);
}

void Shader::init(const GLchar *vsFile, const GLchar *fsFile) {
    shader_vp = glCreateShader(GL_VERTEX_SHADER);
    shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
    
    const GLchar* vsText = textFileRead(vsFile);
    const GLchar* fsText = textFileRead(fsFile);    
    
    if (vsText == NULL || fsText == NULL) {
        cerr << "Either vertex shader or fragment shader file not found." << endl;
        return;
    }
    
    glShaderSource(shader_vp, 1, &vsText, 0);
    glShaderSource(shader_fp, 1, &fsText, 0);
    
    glCompileShader(shader_vp);
	checkCompileErrors(shader_vp);
    glCompileShader(shader_fp);
	checkCompileErrors(shader_fp);
    
    shader_id = glCreateProgram();

    glAttachShader(shader_id, shader_fp);
    glAttachShader(shader_id, shader_vp);

    glLinkProgram(shader_id);
	checkLinkErrors(shader_id);

	glDeleteShader(shader_fp);
    glDeleteShader(shader_vp);
}

void Shader::init(const GLchar *vsFile, const GLchar *gsFile, const GLchar *fsFile) {
    shader_vp = glCreateShader(GL_VERTEX_SHADER);
	shader_gs = glCreateShader(GL_GEOMETRY_SHADER);
    shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
    
    const GLchar* vsText = textFileRead(vsFile);
	const GLchar* gsText = textFileRead(gsFile);
    const GLchar* fsText = textFileRead(fsFile);    
    
    if (vsText == NULL || fsText == NULL || gsText == NULL) {
        cerr << "Either vertex shader, geometry shader or fragment shader file not found." << endl;
        return;
    }
    
    glShaderSource(shader_vp, 1, &vsText, 0);
	glShaderSource(shader_gs, 1, &gsText, 0);
    glShaderSource(shader_fp, 1, &fsText, 0);
    
    glCompileShader(shader_vp);
	checkCompileErrors(shader_vp);
	glCompileShader(shader_gs);
	checkCompileErrors(shader_gs);
    glCompileShader(shader_fp);
	checkCompileErrors(shader_fp);
    
    shader_id = glCreateProgram();

    glAttachShader(shader_id, shader_fp);
	glAttachShader(shader_id, shader_gs);
    glAttachShader(shader_id, shader_vp);

    glLinkProgram(shader_id);
	checkLinkErrors(shader_id);

	glDeleteShader(shader_fp);
    glDeleteShader(shader_vp);
	glDeleteShader(shader_gs);
}

void Shader::init(const GLchar *vsFile, const GLchar *tcsFile, const GLchar *tesFile, const GLchar *fsFile) {
    shader_vp = glCreateShader(GL_VERTEX_SHADER);
	shader_tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
	shader_tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
    shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
    
    const GLchar* vsText = textFileRead(vsFile);
	const GLchar* tcsText = textFileRead(tcsFile);
	const GLchar* tesText = textFileRead(tesFile);
    const GLchar* fsText = textFileRead(fsFile);    
	
    
    if (vsText == NULL || fsText == NULL || tcsText == NULL || tesText == NULL) {
        cerr << "Either vertex shader, tesselation control shader, tessleation evaluation shader or fragment shader file not found." << endl;
        return;
    }
    
    glShaderSource(shader_vp, 1, &vsText, 0);
	glShaderSource(shader_tcs, 1, &tcsText, 0);
	glShaderSource(shader_tes, 1, &tesText, 0);
    glShaderSource(shader_fp, 1, &fsText, 0);
    
    glCompileShader(shader_vp);
	checkCompileErrors(shader_vp);
	glCompileShader(shader_tcs);
	checkCompileErrors(shader_tcs);
	glCompileShader(shader_tes);
	checkCompileErrors(shader_tes);
    glCompileShader(shader_fp);
	checkCompileErrors(shader_fp);
    
    shader_id = glCreateProgram();

    glAttachShader(shader_id, shader_vp);
	glAttachShader(shader_id, shader_tcs);
	glAttachShader(shader_id, shader_tes);
    glAttachShader(shader_id, shader_fp);

    glLinkProgram(shader_id);
	checkLinkErrors(shader_id);

	glDeleteShader(shader_vp);
    glDeleteShader(shader_tcs);
	glDeleteShader(shader_tes);
	glDeleteShader(shader_fp);
}

void Shader::init(const GLchar *vsFile, const GLchar *tcsFile, const GLchar *tesFile, const GLchar* gsFile, const GLchar *fsFile) {
    shader_vp = glCreateShader(GL_VERTEX_SHADER);
	shader_tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
	shader_tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
	shader_gs = glCreateShader(GL_GEOMETRY_SHADER);
    shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
    
    const GLchar* vsText = textFileRead(vsFile);
	const GLchar* tcsText = textFileRead(tcsFile);
	const GLchar* tesText = textFileRead(tesFile);
	const GLchar* gsText = textFileRead(gsFile);
    const GLchar* fsText = textFileRead(fsFile);    
	
    
    if (vsText == NULL || fsText == NULL || tcsText == NULL || tesText == NULL || gsText == NULL) {
        cerr << "Either vertex shader, tesselation control shader, tessleation evaluation shader or fragment shader file not found." << endl;
        return;
    }
    
    glShaderSource(shader_vp, 1, &vsText, 0);
	glShaderSource(shader_tcs, 1, &tcsText, 0);
	glShaderSource(shader_tes, 1, &tesText, 0);
	glShaderSource(shader_gs, 1, &gsText, 0);
    glShaderSource(shader_fp, 1, &fsText, 0);
    
    glCompileShader(shader_vp);
	checkCompileErrors(shader_vp);
	glCompileShader(shader_tcs);
	checkCompileErrors(shader_tcs);
	glCompileShader(shader_tes);
	checkCompileErrors(shader_tes);
	glCompileShader(shader_gs);
	checkCompileErrors(shader_gs);
    glCompileShader(shader_fp);
	checkCompileErrors(shader_fp);
    
    shader_id = glCreateProgram();

    glAttachShader(shader_id, shader_vp);
	glAttachShader(shader_id, shader_tcs);
	glAttachShader(shader_id, shader_tes);
	glAttachShader(shader_id, shader_gs);
    glAttachShader(shader_id, shader_fp);

    glLinkProgram(shader_id);
	checkLinkErrors(shader_id);

	glDeleteShader(shader_vp);
    glDeleteShader(shader_tcs);
	glDeleteShader(shader_tes);
	glDeleteShader(shader_gs);
	glDeleteShader(shader_fp);
}

void Shader::checkCompileErrors(GLuint id){
	GLint success;
	glGetShaderiv(id,GL_COMPILE_STATUS, &success);
	if(!success){
		GLchar infoLog[1024];
		glGetShaderInfoLog(id,sizeof(infoLog),NULL,infoLog);
		fprintf(stderr,"Error compiling shader: '%s' \n", infoLog);
	} else {
		fprintf(stdout,"Shader %d compiled successful.\n", id);
	}
}

void Shader::checkLinkErrors(GLuint id){
	GLint success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar infoLog[1024];
		glGetProgramInfoLog(id, sizeof(infoLog), NULL, infoLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", infoLog);
	} else {
		fprintf(stdout,"Shaders %d linked successful.\n", id);
	}
}

Shader::~Shader() {
    glDetachShader(shader_id, shader_fp);
    glDetachShader(shader_id, shader_vp);
    
    glDeleteProgram(shader_id);
}

unsigned int Shader::id() {
    return shader_id;
}

void Shader::bind() {
    glUseProgram(shader_id);
}

void Shader::unbind() {
    glUseProgram(0);
}

