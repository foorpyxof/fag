#ifndef FAG_ENGINE_RENDERER_HPP
#define FAG_ENGINE_RENDERER_HPP

namespace fag {

class Renderer {
public:
  enum class Backend { Vulkan };

  struct Shape {
    // union {
    //   // Fpx3d_Vk_Shape vulkanShape;
    //   // ...
    // };
  };

public:
  virtual ~Renderer(void) {};

  virtual void initialize(void) = 0;
  virtual void render_frame(void) = 0;

  virtual Backend get_backend(void) = 0;
};

} // namespace fag

#endif // FAG_ENGINE_RENDERER_HPP
