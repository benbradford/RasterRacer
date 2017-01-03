cmake_minimum_required(VERSION 2.8.9)
project(raster_racer)

# Select c++ version
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g")
#Bring the headers into the project
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
		    message(ERROR " GLUT not found!")
	endif(NOT GLUT_FOUND)
	find_package(OpenGL REQUIRED)
	include_directories(${OpenGL_INCLUDE_DIRS})
	link_directories(${OpenGL_LIBRARY_DIRS})
	add_definitions(${OpenGL_DEFINITIONS})
	if(NOT OPENGL_FOUND)
			    message(ERROR " OPENGL not found!")
		endif(NOT OPENGL_FOUND)
	
	include_directories(inc/ /usr/include)

#Add sources
file(GLOB SOURCES "src/*.cpp" "/usr/source/")

#Link static lib
set ( PROJECT_LINK_LIBS pthread )
link_directories (/usr/local/lib)

#sudo apt-get install libxmu-dev libxi-dev

add_executable(raster_racer ${SOURCES})
target_link_libraries( raster_racer ${PROJECT_LINK_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
