cmake_minimum_required(VERSION 3.10)
project(DaisyEngine CXX)

# Définir la version de C++
set(CMAKE_CXX_STANDARD 17)

# Définir les chemins pour Vulkan SDK
if(NOT DEFINED ENV{VULKAN_SDK})
    message(FATAL_ERROR "VULKAN_SDK environment variable is not set!")
endif()
set(VULKAN_INCLUDE_DIR $ENV{VULKAN_SDK}/include)
set(VULKAN_LIBRARY_DIR $ENV{VULKAN_SDK}/lib)

# Inclure les répertoires
include_directories(${VULKAN_INCLUDE_DIR})
link_directories(${VULKAN_LIBRARY_DIR})

# Ajouter GLFW manuellement
set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Libraries/glfw-3.4.bin.WIN64/include")
set(GLFW_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/Libraries/glfw-3.4.bin.WIN64/lib-vc2022")
set(GLFW_LIBRARIES "${GLFW_LIBRARY_DIR}/glfw3.lib")

# Vérifier que GLFW est disponible
if(NOT EXISTS "${GLFW_INCLUDE_DIR}/GLFW/glfw3.h")
    message(FATAL_ERROR "GLFW header file not found in ${GLFW_INCLUDE_DIR}")
endif()
if(NOT EXISTS "${GLFW_LIBRARIES}")
    message(FATAL_ERROR "GLFW library not found in ${GLFW_LIBRARY_DIR}")
endif()

# Inclure les répertoires de GLFW
include_directories(${GLFW_INCLUDE_DIR})
link_directories(${GLFW_LIBRARY_DIR})

# Ajouter GLM
set(GLM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Libraries/glm")
include_directories(${GLM_INCLUDE_DIR})

# Fichiers sources
file(GLOB SOURCES src/*.cpp)
file(GLOB VERT_SHADERS Shaders/*.vert)
file(GLOB FRAG_SHADERS Shaders/*.frag)

# Générer les shaders en SPIR-V
set(SPIRV_OUTPUT_DIR ${CMAKE_BINARY_DIR}/shaders)
file(MAKE_DIRECTORY ${SPIRV_OUTPUT_DIR})

foreach(SHADER ${VERT_SHADERS} ${FRAG_SHADERS})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    set(SPIRV_OUTPUT ${SPIRV_OUTPUT_DIR}/${SHADER_NAME}.spv)

    add_custom_command(
        OUTPUT ${SPIRV_OUTPUT}
        COMMAND $ENV{VULKAN_SDK}/bin/glslc ${SHADER} -o ${SPIRV_OUTPUT}
        DEPENDS ${SHADER}  # Assurez-vous que chaque shader est bien une dépendance
        COMMENT "Compiling ${SHADER} to SPIR-V"
    )
    list(APPEND SPIRV_FILES ${SPIRV_OUTPUT})
endforeach()

# Ajouter l'exécutable
add_executable(DaisyEngine ${SOURCES} ${SPIRV_FILES})
set_target_properties(DaisyEngine PROPERTIES LINKER_LANGUAGE CXX)

# Lier les bibliothèques
target_link_libraries(DaisyEngine ${GLFW_LIBRARIES} vulkan-1)

# Options de compilation (facultatif)
target_compile_options(DaisyEngine PRIVATE -Wall -Wextra)

# Répertoire de sortie pour l'exécutable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
