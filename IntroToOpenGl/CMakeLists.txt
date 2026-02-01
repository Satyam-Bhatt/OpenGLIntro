cmake_minimum_required(VERSION 3.15)
project(IntroToOpenGl)

# Set C++ standard to C++20 (as specified in vcxproj)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific settings
if(WIN32)
    add_compile_definitions(
        UNICODE
        _UNICODE
    )
endif()

# Configuration-specific preprocessor definitions
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(_DEBUG _CONSOLE)
else()
    add_compile_definitions(NDEBUG _CONSOLE)
endif()

# Source files
set(SOURCES
    Right_LeftHanded.cpp
    StitchingTest.cpp
    TextureMapping_3D.cpp
    constants.cpp
    CoordinateSystems.cpp
    FirstShader.cpp
    FirstTriangle.cpp
    glad.c
    glmTest.cpp
    HelloTriangle.cpp
    Imgui/imgui.cpp
    Imgui/imgui_demo.cpp
    Imgui/imgui_draw.cpp
    Imgui/imgui_impl_glfw.cpp
    Imgui/imgui_impl_opengl3.cpp
    Imgui/imgui_tables.cpp
    Imgui/imgui_widgets.cpp
    Intro.cpp
    IntroTexture.cpp
    IntroTransformation.cpp
    main.cpp
    MatrixGraphTransformation.cpp
    MatrixUniform.cpp
    MipmapOptions.cpp
    MyMatrixClass.cpp
    SceneManager.cpp
    SecondTriangle.cpp
    Shader.cpp
    ShaderClass.cpp
    Shaders.cpp
    SierpinskiTriangle.cpp
    stb_image.cpp
    TestMain.cpp
    TexelViewer.cpp
    TextureMain.cpp
    TransformationMain.cpp
    Transformation_3D.cpp
    Translate_Rotate_Scale.cpp
    TriangleTexture.cpp
    TriangleWithVertexDataManipulation.cpp
    TwoTextures.cpp
    TwoTriangles.cpp
    UniformsInShader.cpp
    VertexAttributes.cpp
    VertexPosToFrag.cpp
    ViewMatrix.cpp
)

# Header files (for IDE support)
set(HEADERS
    Right_LeftHanded.h
    StitchingTest.h
    TextureMapping_3D.h
    constants.h
    CoordinateSystems.h
    FirstShader.h
    FirstTriangle.h
    GameState.h
    glmTest.h
    HelloTriangle.h
    Imgui/imconfig.h
    Imgui/imgui.h
    Imgui/imgui_impl_glfw.h
    Imgui/imgui_impl_opengl3.h
    Imgui/imgui_impl_opengl3_loader.h
    Imgui/imgui_internal.h
    Imgui/imstb_rectpack.h
    Imgui/imstb_textedit.h
    Imgui/imstb_truetype.h
    Intro.h
    IntroTexture.h
    IntroTransformation.h
    Matrix.h
    MatrixGraphTransformation.h
    MatrixUniform.h
    MipmapOptions.h
    MyMatrixClass.h
    SceneManager.h
    SecondTriangle.h
    Shader.h
    ShaderClass.h
    Shaders.h
    SierpinskiTriangle.h
    stb_image.h
    TexelViewer.h
    TextureMain.h
    TransformationMain.h
    Transformation_3D.h
    Translate_Rotate_Scale.h
    TriangleTexture.h
    TriangleWithVertexDataManipulation.h
    TwoTextures.h
    TwoTriangles.h
    UniformsInShader.h
    Vector.h
    VertexAttributes.h
    VertexPosToFrag.h
    ViewMatrix.h
)

# Shader files (for IDE support)
set(SHADERS
    CoordinateSystem2.shader
    CoordinateSystems.shader
    glmTest.shader
    IntroTexture.shader
    IntroTransformation.shader
    MatrixGraphTransformation.shader
    MatrixGraphTransformation2.shader
    MatrixUniform.shader
    MipmapOptions.shader
    MyMatrixClass.shader
    Right_LeftHanded.shader
    ShaderClass.shader
    SierpinskiTriangle.shader
    StitchingTest.shader
    TexelViewer.shader
    TextureMapping_3D.shader
    Transformation_3D.shader
    Translate_Rotate_Scale.shader
    Translate_Scale_Rotate_Pivot.shader
    TriangleTexture.shader
    TwoTextures.shader
    TwoTextureSecondShader.shader
    VertexToPos.shader
    ViewMatrix.shader
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${SHADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/KHR
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/glad
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLLibrary/glfw-3.4.bin.WIN64/include
)

# Library directories
target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLLibrary/glfw-3.4.bin.WIN64/lib-vc2022
)

# Link libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw3
        opengl32
        user32
        gdi32
        shell32
        kernel32
    )
else()
    # For Unix-like systems, you'll need to adjust these
    target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
        GL
        X11
        pthread
        Xrandr
        Xi
        dl
    )
endif()

# Compiler options
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W3>
    $<$<CXX_COMPILER_ID:GNU>:-Wall>
    $<$<CXX_COMPILER_ID:Clang>:-Wall>
)

# Set subsystem to Console for Windows
if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
endif()

# Enable conformance mode for MSVC
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /permissive-)
endif()

# Optimization settings for Release builds
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/O2 /Oi>
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/OPT:REF /OPT:ICF>
    )
endif()

# Copy shader files to build directory (optional)
# Uncomment if you want shaders copied to the build output
# file(COPY ${SHADERS} DESTINATION ${CMAKE_BINARY_DIR})