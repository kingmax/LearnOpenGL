#pragma once

void printMaxVASupport();

// 从程序代码(CPU端)给Shader全局变量提供数据 (02.frag中的uniform vec4 ourColor;)
void sendColor2Shader(unsigned shaderProgram, const char* uniformShaderVariableName);