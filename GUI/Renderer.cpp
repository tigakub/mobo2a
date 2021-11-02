#include "Renderer.hpp"

#include "lodepng.h"
#include <string>
#include <iostream>

using namespace std;

namespace mobo
{
    GLfloat vtxData[] = { -1.0, -1.0, 0.0, 1.0,    1.0, -1.0, 0.0, 1.0,    0.0, 1.0, 0.0, 1.0 };
    GLint vtxDataSize = 12;
    GLfloat clrData[] = { 1.0, 0.0, 0.0, 1.0,    0.0, 1.0, 0.0, 1.0,    0.0, 0.0, 1.0, 1.0 };
    GLint clrDataSize = 12;
    GLfloat uvData[] = { 0.0, 1.0,    1.0, 1.0,    0.5, 0.0 };
    GLint uvDataSize = 6;

    const string vtxShaderSource(
    R"(
        #version 330 core

        layout(location = 0) in vec4 iVtxPos;
        layout(location = 1) in vec4 iVtxClr;
        layout(location = 2) in vec2 iVtxUV;

        out vec4 fVtxClr;
        out vec2 fVtxUV;

        void main() {
            gl_Position = iVtxPos;
            fVtxClr = iVtxClr;
            fVtxUV = iVtxUV;
        }
    )");

    const string frgShaderSource(
    R"(
        #version 330 core

        uniform sampler2D tex;

        in vec4 fVtxClr;
        in vec2 fVtxUV;

        out vec4 oColor;

        void main() {
            oColor = texture(tex, fVtxUV);
        }
    )");

    void Renderer::init()
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_ALPHA_TEST);
        glDepthFunc(GL_LESS);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClearDepth(1.0f);

        auto vtxShader = glCreateShader(GL_VERTEX_SHADER);
        auto frgShader = glCreateShader(GL_FRAGMENT_SHADER);
        
        const GLchar* vtxSrcPtr = vtxShaderSource.c_str();
        const GLchar* frgSrcPtr = frgShaderSource.c_str();
        GLint vtxSrcSize = vtxShaderSource.size(), frgSrcSize = frgShaderSource.size();

        glShaderSource(vtxShader, 1, &vtxSrcPtr, &vtxSrcSize);
        glCompileShader(vtxShader);
        GLint compileStatus;
        glGetShaderiv(vtxShader, GL_COMPILE_STATUS, &compileStatus);
        if(compileStatus != GL_TRUE) {
            cerr << "Failed to compile vertex shader" << endl;
        }

        glShaderSource(frgShader, 1, &frgSrcPtr, &frgSrcSize);
        glCompileShader(frgShader);
        glGetShaderiv(frgShader, GL_COMPILE_STATUS, &compileStatus);
        if(compileStatus != GL_TRUE) {
            cerr << "Failed to compile fragment shader" << endl;
        }

        prog = glCreateProgram();
        glAttachShader(prog, vtxShader);
        glAttachShader(prog, frgShader);
        glLinkProgram(prog);
        GLint linkStatus;
        glGetProgramiv(prog, GL_LINK_STATUS, &linkStatus);
        if(linkStatus != GL_TRUE) {
            cerr << "Failed to link program" << endl;
        }

        glDeleteShader(frgShader);
        glDeleteShader(vtxShader);

        glUseProgram(prog);
        
        glGenBuffers(3, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, vtxDataSize * sizeof(GLfloat), vtxData, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, clrDataSize * sizeof(GLfloat), clrData, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
        glBufferData(GL_ARRAY_BUFFER, uvDataSize * sizeof(GLfloat), uvData, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        string imageFilename("BearInRed.png");
        unsigned imgWidth, imgHeight;
        vector<unsigned char> buffer;
        lodepng::load_file(buffer, imageFilename.c_str());
        lodepng::State state;
        vector<unsigned char> imageBytes;
        lodepng::decode(imageBytes, imgWidth, imgHeight, state, buffer);

        glGenTextures(1, &tex);
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex);

        // glTexStorage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imgWidth, imgHeight); // NOT Available until OpenGL V4.2
        // glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, imgWidth, imgHeight, GL_RGBA, GL_UNSIGNED_BYTE, (void*) &(imageBytes[0]));
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*) &(imageBytes[0]));

        glGenSamplers(1, &samp);
        glSamplerParameteri(samp, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glSamplerParameteri(samp, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glSamplerParameteri(samp, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(samp, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameterf(samp, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

        /*
        auto info = state.info_png;
        auto color = info.color;
        cout << "Image width: " << imgWidth << endl;
        cout << "Image height: " << imgHeight << endl;
        cout << "Bit depth: " << color.bitdepth << endl;
        cout << "Bits per pixel: " << lodepng_get_bpp(&color) << endl;
        cout << "Channels per pixel: " << lodepng_get_channels(&color) << endl;
        cout << "Can have alpha: " << lodepng_can_have_alpha(&color) << endl;
        */
       
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    Renderer::~Renderer()
    {
        if(prog) glDeleteProgram(prog);
    }

    void Renderer::didReshape(int w, int h)
    {
        glViewport(0, 0, w, h);
    }

    void Renderer::render()
    {
        glUseProgram(prog);

        GLint texLoc = glGetUniformLocation(prog, "tex");
        glUniform1i(texLoc, 0);

        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex);
        glBindSampler(0, samp);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindSampler(0, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);

        glFinish();
        
        glutSwapBuffers();
    }

    float Renderer::calculateFrameRate()
    {
        auto newTime = steady_clock::now();
        if(timestamp != time_point<steady_clock>(seconds(0))) {
            duration<float, micro> period(newTime - timestamp);
            float newFPS = seconds(1) / period;
            fpsAVG.push_back(newFPS);
            fpsWMA = fpsAVG.wma();
            
            if(emaWindow == 0) {
                fpsEMA = newFPS;
            } else {
                float k = 2.0f / (emaWindow + 1.0f);
                fpsEMA = newFPS * k + fpsEMA * (1.0 - k);
            }
            if(emaWindow < 15) emaWindow++;

        } else {
            fpsWMA = 0.0;
            fpsEMA = 0.0;
        }
        timestamp = newTime;
        return fpsEMA;
    }
}
