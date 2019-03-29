#ifndef  OPENGL_DEBUG_H
#define OPENGL_DEBUG_H

#include <glad/glad.h>
#include <string>
#include <iostream>

GLenum glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM 枚举参数不合法"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE 值参数不合法"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION 一个指令的状态对指令的参数不合法"; break;
		//case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW 压栈操作造成栈上溢(Overflow)"; break;
		//case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW 弹栈操作时栈在最低点"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY 内存调用操作无法调用（足够的）内存"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION 读取或写入一个不完整的帧缓冲"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

#endif // ! OPENGL_DEBUG_H

