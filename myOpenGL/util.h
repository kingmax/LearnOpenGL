#pragma once

void printMaxVASupport();

// �ӳ������(CPU��)��Shaderȫ�ֱ����ṩ���� (02.frag�е�uniform vec4 ourColor;)
void sendColor2Shader(unsigned shaderProgram, const char* uniformShaderVariableName);