#pragma once

#include "types.h"
#include "utils.h"
#include "Shader.h"
#include "Window.h"

struct FrameBuffer
{
    GLuint FBO;
    GLuint RBO;
    GLuint VAO;
    GLuint VBO;
    GLuint texture;
    std::shared_ptr<Shader> shader;
};

class Renderer
{
public:
    void init(const WindowInfos& windowInfos);
    void prePass();
    void endPass() const;
    void initMesh(Mesh& mesh) const;
    void drawMesh(Mesh& mesh) const;
    void freeMesh(Mesh& mesh) const;
    void applyMaterial(Material& material, Camera& camera, Transform& transform) const;
    void initMaterial(Material& material) const;
    void initTexture3D(const std::vector<std::uint8_t>& texture, const std::uint32_t textureGL) const;
    void initTexture2D(const std::vector<std::uint8_t>& texture, const std::uint32_t textureGL) const;
    void writeImg(const std::uint32_t iteration) const;
    void setLineWidth(const float width) const;

private:
	std::shared_ptr<Window> _window = nullptr;
    FrameBuffer _screenbuffer {};
    FrameBuffer _raymarchingbuffer {};
    WindowInfos _windowInfos {};

    void _initFrameBuffer(FrameBuffer& framebuffer, std::string vert, std::string frag);
};

