cmake_minimum_required(VERSION 2.6)
PROJECT(testagi)

#FIND_PACKAGE(gd REQUIRED)
#INCLUDE_DIRECTORIES(${gd_INCLUDE_DIR})
SET(testagi_src hello.cpp )
ADD_EXECUTABLE(testagi ${testagi_src})



PROJECT(testagiwithClass)

#FIND_PACKAGE(gd REQUIRED)
#INCLUDE_DIRECTORIES(${gd_INCLUDE_DIR})
SET(testagiwithClass_src hellowithClass.cpp )
ADD_EXECUTABLE(testagiwithClass ${testagiwithClass_src})