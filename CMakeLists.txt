# VS-Bibliotheken in CLion nutzen
cmake_minimum_required(VERSION 3.16)

# Projektnamen anpassen
project(CGPrakt6_Clion)
set(CMAKE_CXX_STANDARD 20)
#hier
#Inhalt des source-Ordners auflisten
add_executable(${CMAKE_PROJECT_NAME}
        source/utils/Lights/ShadowMapGenerator.h
        source/utils/Lights/ShadowMapGenerator.cpp
        source/utils/Lights/ShaderLightMapper.h
        source/utils/Lights/ShaderLightMapper.cpp
        source/utils/Shader/PhongShader.h
        source/utils/Shader/PhongShader.cpp
        include/Framebuffer.h
        source/Framebuffer.cpp
        source/utils/Models/Aabb.h
        source/utils/Models/Aabb.cpp
        include/Application.h
        source/Application.cpp
        source/utils/Models/BaseModel.h
        source/utils/Models/BaseModel.cpp
        source/utils/Shader/BaseShader.h
        source/utils/Shader/BaseShader.cpp
        include/Camera.h
        source/Camera.cpp
        source/utils/YourClasses/color.h
        source/utils/YourClasses/color.cpp
        source/utils/Shader/ConstantShader.h
        source/utils/Shader/ConstantShader.cpp
        include/IndexBuffer.h
        source/IndexBuffer.cpp
        source/utils/Models/LineBoxModel.h
        source/utils/Models/LineBoxModel.cpp
        source/utils/Models/LinePlaneModel.h
        source/utils/Models/LinePlaneModel.cpp
        include/Matrix.h
        source/Matrix.cpp
        source/utils/Models/Model.h
        source/utils/Models/Model.cpp
        source/utils/YourClasses/rgbimage.h
        source/utils/YourClasses/rgbimage.cpp
        include/Texture.h
        source/Texture.cpp
        source/utils/Models/TriangleBoxModel.h
        source/utils/Models/TriangleBoxModel.cpp
        source/utils/Models/TrianglePlaneModel.h
        source/utils/Models/TrianglePlaneModel.cpp
        source/utils/Models/TriangleSphereModel.h
        source/utils/Models/TriangleSphereModel.cpp
        source/utils/YourClasses/vector.h
        source/utils/YourClasses/vector.cpp
        include/VertexBuffer.h
        source/VertexBuffer.cpp
        source/main.cpp
        source/Spaceship.cpp include/Spaceship.h source/Text.h source/Enemy.cpp include/Enemy.h source/Items.cpp source/Items.h source/ParticleSystem.cpp include/ParticleSystem.h source/utils/Utils.cpp source/utils/Utils.h source/utils/YourClasses/ColorA.cpp source/utils/YourClasses/ColorA.h source/utils/ParticleShader.cpp source/utils/ParticleShader.h source/ParticleProps.cpp source/ParticleProps.h)

include_directories(
        source
        libraries/glfw/include
        libraries/glew/include
        libraries/freeimage/include
        libraries/assimp/include
        libraries/freetype/include)

find_package(OpenGL REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME}
        OpenGL::GL
        OpenGL::GLU
        ../libraries/glfw/lib/glfw3
        ../libraries/glew/lib/glew32
        ../libraries/freeimage/lib/FreeImage
        ../libraries/assimp/lib/assimp-vc140-mt)
