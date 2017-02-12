#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>


static void onEsc(GLFWwindow* window, int key, int , int action, int )
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main ( int argc, const char * argv[] )
    {

// open context and window
    GLFWwindow* window;
    if ( !glfwInit() )
        {
        return -1;
        }

// data for the rendering
    int width ( 200 ),height ( 100 );
    glm::vec3 eye ( 1,4,10 );
    glm::vec3 center ( 1,6,0 );
    glm::vec3 up ( .1,.9,0 );
    glm::vec4 point ( 1.0f, 4.0f, 1.0f,1.0f );
    double fovx ( glm::radians ( 45.0f ) ), fovy ( glm::radians ( 45.0f ) );
    double zNear ( .1 ),zFar ( 20 );

// create a window and context
    window = glfwCreateWindow ( width, height, "Point to Pixel", NULL, NULL );
    if ( !window )
        {
        glfwTerminate();
        return -1;
        }
    glfwMakeContextCurrent ( window );
    glfwSetKeyCallback(window, onEsc);
    if ( glewInit() != GLEW_OK )
        {
        glfwTerminate();
        return -1;
        }

// projection matrix
    glm::mat4 projection = glm::perspective ( ( float ) fovx/ ( float ) fovy, ( float ) width / ( float ) height, ( float ) zNear, ( float ) zFar );

// view matrix
    glm::mat4 view = glm::lookAt ( eye,center, up );

// identity model matrix
    glm::mat4 model = glm::mat4 ( 1.0f );

// mvp matrix
    glm::mat4 mv =view * model;
    glm::mat4 mvp = projection * mv;


// set the matrices
    glClearColor ( 0,0,0,0 );
    glMatrixMode ( GL_PROJECTION );
    glLoadMatrixf ( glm::value_ptr ( projection ) );
    glMatrixMode ( GL_MODELVIEW );
    glLoadMatrixf ( glm::value_ptr ( mv ) );


// get matrices back from openGL
    glm::mat4x4 mv_get,projection_get;
    glGetFloatv ( GL_MODELVIEW_MATRIX, glm::value_ptr ( mv_get ) );
    glGetFloatv ( GL_PROJECTION_MATRIX, glm::value_ptr ( projection_get ) );
    glm::mat4x4 mvp_get = projection_get*mv_get;

// test
        {
        std::cout<< "mvp_get: "<<glm::to_string ( mvp_get ) <<std::endl;
        std::cout<< "mvp    : "<<glm::to_string ( mvp     ) <<std::endl;
        }

// project point manually
    std::cout<< "Point: "<<glm::to_string ( point ) <<std::endl;
    std::cout<< "Is projected onto pixel : ";
    glm::vec4 hpixel = mvp_get*point;
    glm::vec2 pixel ( ( hpixel.x/hpixel.w/2+.5 ) *width, ( hpixel.y/hpixel.w/2+.5 ) *height );
    std::cout<<pixel.x<<","<<pixel.y<<","<<hpixel.z<<std::endl;


// render
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glColor3f ( 1.0f, 1.0f, 0.0f );
    glBegin ( GL_POINTS );
    glVertex3f ( point.x,point.y,point.z );
    glEnd();
    glFlush();
   
    glfwSwapBuffers ( window );
    
// get rendered image , and test the pixel
    std::vector<unsigned char> image ( width*height*3 );
    int format ( GL_RGB ),btype ( GL_UNSIGNED_BYTE );
    glReadPixels ( 0,0,width,height,format,btype,&image[0] );
    size_t location = ( ( size_t ) pixel.y*width+ ( size_t ) pixel.x ) *3;
    std::cout<<"pixel value (ReadPixels) is :"<< ( int ) image[location]<<", "<< ( int ) image[location+1]<<", "<< ( int ) image[location+2]<<std::endl;

    while ( !glfwWindowShouldClose ( window ) )
    {
       glfwPollEvents();
    }; // forever
    glfwTerminate();
    return 0;
    }
