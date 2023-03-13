#ifndef LIBTRACKER_PROCESSING_H
#define LIBTRACKER_PROCESSING_H

/**
 * @author Elias Engelbert Plank
 * @date 2022-2023
 */

#include <libtracker/libtracker.hpp>

/**
 * The processing view provides functionality for loading and processing images. It does this by utilizing little
 * programs, called `shaders` on the graphics card
 */
class Processing : public View {
public:
    /**
     * Creates a new Processing view. Here, nothing really happens as we have to wait until the graphics pipeline is
     * initialized, before we can allocate resources
     * @param windowHandle Handle to the native window
     */
    explicit Processing(void* windowHandle) noexcept;

    /**
     * Default destructor, does pretty much nothing as member lifetime is tracked by the owning smart pointers
     */
    ~Processing() noexcept override = default;

    /**
     * Allocation of buffers on the graphics card happens here
     */
    void OnInit() noexcept override;

    /**
     * Draws the entire processing view
     * @param deltaTime Time since the last frame
     */
    void OnUpdate(float deltaTime) noexcept override;

    /**
     * No resources have to be manually freed, so this does nothing
     */
    void OnDestroy() noexcept override;

private:
    /**
     * Initializes buffers on the graphics card, this function is called by OnInit
     */
    void initializeBuffers() noexcept;

    /*
     * Draws the responsive list of currently loaded textures
     */
    void drawTextureListPanel() noexcept;

    /**
     * Draws the main viewport
     */
    void drawRenderFrameBufferPanel() const noexcept;

    /**
     * Draws the filter panel, which contains a list of filters which where applied or created by the user, and the
     * controls for the individual filters
     */
    void drawFilterPanel() noexcept;

    /**
     * Draws the filter hierarchy, which is basically the list of all filters the user applied at some point
     */
    void drawFilterHierarchyPanel() noexcept;

    /**
     * Draws the controls for the kernel filter
     */
    void drawFilterKernel() noexcept;

    /**
     * Draws the controls for the stacking filter
     */
    void drawFilterStack() noexcept;

    /**
     * Renders the specified texture handle to the main viewport
     * @param textureNativeHandle Handle of the texture
     * @param width Width of the texture
     * @param height Height of the texture
     */
    void showTexture(u32 textureNativeHandle, s32 width, s32 height) const noexcept;

private:
    /**
     * Helper struct for storing the name of a filter and the corresponding drawing target
     */
    struct Filter {
        std::string Name;
        std::shared_ptr<graphics::FrameBuffer> Target;
    };

    /**
     * List of user created filters
     */
    std::vector<Filter> filterList;

    /**
     * List of textures that were loaded by the user
     */
    std::vector<std::shared_ptr<graphics::Texture>> textureList;

    /**
     * Frame buffer of the main viewport
     */
    std::shared_ptr<graphics::FrameBuffer> renderFrameBuffer;

    /**
     * Shader that is used for rendering textures to the main view port. Don't really know how to blip frame buffers,
     * therefore this rather bold approach was used. One draw call more or less won't hurt!
     */
    std::shared_ptr<graphics::Shader> copyShader;

    /**
     * Vertex array for the geometry of the texture quad
     */
    std::shared_ptr<graphics::VertexArray> renderVertexArray;

    /**
     * Vertex buffer for the geometry of the texture quad
     */
    std::shared_ptr<graphics::VertexBuffer> renderVertexBuffer;

    /**
     * Index buffer for the geometry of the texture quad
     */
    std::shared_ptr<graphics::IndexBuffer> renderIndexBuffer;
};

#endif// LIBTRACKER_PROCESSING_H