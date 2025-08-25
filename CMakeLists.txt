cmake_minimum_required(VERSION 3.12)

project(QDetailsView CXX)

option(BUILD_SHARED_LIBS "Build shared library (ON) or static library (OFF)" OFF)

find_package(Qt6 COMPONENTS Core Widgets Gui QuickWidgets QuickTemplates2 QuickControls2 REQUIRED)

qt6_add_resources(QRC_FILE Resources.qrc)

file(GLOB_RECURSE PROJECT_SOURCE FILES ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h  ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Resources/*)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROJECT_SOURCE})

if(BUILD_SHARED_LIBS)
	add_library(QDetailsView SHARED ${PROJECT_SOURCE} ${QRC_FILE})
	target_compile_definitions(QDetailsView PRIVATE QDETAILS_VIEW_SHARED_LIBRARY)
else()
	add_library(QDetailsView STATIC ${PROJECT_SOURCE} ${QRC_FILE})
	target_compile_definitions(QDetailsView PUBLIC QDETAILS_VIEW_STATIC_LIBRARY) 
endif()


set_property(TARGET QDetailsView PROPERTY AUTOMOC ON)
set_property(TARGET QDetailsView PROPERTY USE_FOLDERS ON)
set_property(TARGET QDetailsView PROPERTY AUTOGEN_SOURCE_GROUP "Generated Files")

target_compile_definitions(QDetailsView PRIVATE QDETAILS_VIEW_LIBRARY)

target_link_libraries(QDetailsView PUBLIC
	Qt::Gui
	Qt::GuiPrivate
	Qt::Widgets
	Qt::WidgetsPrivate
	Qt::QuickWidgets 
	Qt::QuickPrivate
	Qt::QuickTemplates2
	Qt::QuickTemplates2Private
	Qt::QuickControls2
)

target_include_directories(QDetailsView PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source/Public)
file(GLOB_RECURSE PUBLIC_FILES LIST_DIRECTORIES TRUE ${CMAKE_CURRENT_SOURCE_DIR}/Source/Public/*)
foreach(PUBLIC_FILE ${PUBLIC_FILES})
    if(IS_DIRECTORY ${PUBLIC_FILE})
        target_include_directories(QDetailsView PRIVATE  ${PUBLIC_FILE})
    endif()
endforeach()

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory(Example)
endif()
