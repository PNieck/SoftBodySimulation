#include "simulation/views/visualization/framebuffer.hpp"

#include <stdexcept>


Framebuffer::Framebuffer(const int width, const int height)
{
    glGenFramebuffers(1, &fbo);
    Use();

    // Generate texture
    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Error while creating framebuffer");

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


Framebuffer::~Framebuffer()
{
    glDeleteTextures(1, &colorTexture);
    glDeleteFramebuffers(1, &fbo);
}
