#pragma once

#include <glad/glad.h>

#include "imgui.h"


class Framebuffer {
public:
    Framebuffer(int width, int height);
    Framebuffer(Framebuffer&) = delete;
    Framebuffer(const Framebuffer&) = delete;

    ~Framebuffer();

    void Use() const
        { glBindFramebuffer(GL_FRAMEBUFFER, fbo); }

    static void UseDefault()
        { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    [[nodiscard]]
    unsigned int GetColorTextureId() const
        { return colorTexture; }

    Framebuffer& operator=(Framebuffer&) = delete;
private:
    unsigned int fbo = 0;
    unsigned int colorTexture = 0;
};
