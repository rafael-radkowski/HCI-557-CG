set(GLM_VERSION "0.6.3")
set(GLM_INCLUDE_DIRS "$ENV{CS557_ROOT}/include")

if (NOT CMAKE_VERSION VERSION_LESS "3.0")
    include("${CMAKE_CURRENT_LIST_DIR}/glmTargets.cmake")
endif()
