#include "src/Rendering/RenderTarget.h"
#include "Test_scene.h"
#include "main.h"

// Entry point
int main(int argc, char* argv[]) {
    // Define render target size
    int width = 960;
    int height = 540;
    // Create scene
    TestScene scene;
    scene.Setup(); 
    // Create render target
    RenderTarget render_target(width, height);
    // Start renderer loop
    Run(render_target, scene);
    
    return 0;
}
