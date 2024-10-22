
include(CMakeFindDependencyMacro)
find_dependency(Threads)
find_dependency(OpenSSL)
find_dependency(LibJuice)
find_dependency(plog CONFIG)
find_dependency(unofficial-usrsctp CONFIG)
find_dependency(libSRTP CONFIG)
include("${CMAKE_CURRENT_LIST_DIR}/LibDataChannelTargets.cmake")

