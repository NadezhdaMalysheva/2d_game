#include "common.h"
#include "Image.h"
#include "Player.h"
#include "./Map/GameLevel.h"
#include "./Objects/Object.h"
#include <unistd.h>



#define GLFW_DLL
#include <GLFW/glfw3.h>

constexpr GLsizei WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 1024;//1024;

struct InputState
{
  bool    keys[1024]             {}; //массив состояний кнопок - нажата/не нажата
  GLfloat lastX = 400, lastY   = 300; //исходное положение мыши
  bool    firstMouse           = true;
  bool    captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
  bool    capturedMouseJustNow = false;
} static  Input;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
  case GLFW_KEY_1:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  case GLFW_KEY_2:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
	default:
		if (action == GLFW_PRESS)
      Input.keys[key] = true;
		else if (action == GLFW_RELEASE)
      Input.keys[key] = false;
	}
}

void processPlayerMovement(Player &player)
{
  if (Input.keys[GLFW_KEY_W])
    player.ProcessInput(MovementDir::UP);
  else if (Input.keys[GLFW_KEY_S])
    player.ProcessInput(MovementDir::DOWN);
  if (Input.keys[GLFW_KEY_A])
    player.ProcessInput(MovementDir::LEFT);
  else if (Input.keys[GLFW_KEY_D])
    player.ProcessInput(MovementDir::RIGHT);
  if (Input.keys[GLFW_KEY_E])
    player.Destroy();

}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    Input.captureMouse = !Input.captureMouse;

  if (Input.captureMouse)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Input.capturedMouseJustNow = true;
  }
  else
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
  if (Input.firstMouse)
  {
    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
    Input.firstMouse = false;
  }

  GLfloat xoffset = float(xpos) - Input.lastX;
  GLfloat yoffset = Input.lastY - float(ypos);

  Input.lastX = float(xpos);
  Input.lastY = float(ypos);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
  // ...
}


int initGL()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  std::cout << "Controls: "<< std::endl;
  std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
  std::cout << "W, A, S, D - movement  "<< std::endl;
  std::cout << "press ESC to exit" << std::endl;

	return 0;
}

/*Image Camera(Image &allmap, Point coord, Level &GameLevel, bool update = true)
{
  Image screen(WINDOW_WIDTH, WINDOW_HEIGHT, 4);
  int x = std::min(std::max(coord.x-WINDOW_WIDTH/2, 0), allmap.Width()-WINDOW_WIDTH);
  int y = std::min(std::max(coord.y-WINDOW_HEIGHT/2, 0), allmap.Height()-WINDOW_HEIGHT);

  screen.PutPixels(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH, allmap, y, x);

  if (update) 
  {
    for (int i=0; i < GameLevel.moveObj.size(); i++)
    {
      GameLevel.UpdateObj(screen, x, y, x + WINDOW_WIDTH, y + WINDOW_HEIGHT, i);

    }
  }

  return screen;
}*/

Image Camera(Image &allmap, Point coord, Player &player, bool update = true)
{
  Image screen(WINDOW_WIDTH, WINDOW_HEIGHT, 4);
  int x = std::min(std::max(coord.x-WINDOW_WIDTH/2, 0), allmap.Width()-WINDOW_WIDTH);
  int y = std::min(std::max(coord.y-WINDOW_HEIGHT/2, 0), allmap.Height()-WINDOW_HEIGHT);

  screen.PutPixels(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH, allmap, y, x);

  if (update) 
  {
    int j = 0;
    for (int i=0; i < player.GameLevel -> moveObj.size(); i++)
    {

      bool up = player.GameLevel -> UpdateObj(screen, x, y, x + WINDOW_WIDTH, y + WINDOW_HEIGHT, i);
      //std::cout << x << " " << y << " end "<< x  + WINDOW_WIDTH << " " << y + WINDOW_HEIGHT <<std::endl;
      //std::cout << i << " " << up <<std::endl;
      if (up)
      {
        j++;
        if ((j%2)==1 && j>1)
        {
          //std::cout << "up " << i << std::endl;
          processPlayerMovement(player);
          player.Draw(allmap);
        }
      }
      
    }
  }

  return screen;
}


bool ChangeLevel(Image &screenBuffer, GLFWwindow *&window, Level &GameLevel, Player &player)
{
        if (GameLevel.levels == 2)
        {
            Image Win("./resources/background/End/win.png");
            screenBuffer = Win;
            return false;
        }

        screenBuffer = Image("./resources/background/End/nextlevel.png");

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
        glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, Camera(screenBuffer, {0, 0}, player, false).Data()); GL_CHECK_ERRORS;
        glfwSwapBuffers(window);

        sleep(1);
        Image wall = Image("./resources/background/End/next1.png");
        int h = wall.Height();
        int w = wall.Width();

        screenBuffer = Camera(screenBuffer, {0, 0}, player, false);

        for (int y = 0; y < screenBuffer.Height(); y+=h)
        for (int x = 0; x < screenBuffer.Width(); x+=w)
        {
            screenBuffer.PutPixels(x, y, std::min(w, screenBuffer.Width() - x - 1), std::min(h, screenBuffer.Height() - y - 1), wall);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
            glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;

            glfwSwapBuffers(window);
        }

        Level NewLevel("./Map/maps/2.txt", 4, 2);
        GameLevel = NewLevel;
        Image screenBuffer1 = GameLevel.background;
        player.PutLevel(GameLevel);
        player.Draw(screenBuffer1);
        Point coord_all = player.GetCoord();
        Image screenBuffer2 = Camera(screenBuffer1, coord_all, player);
        
        for (int y = 0; y < screenBuffer.Height(); y+=h)
        for (int x = 0; x < screenBuffer.Width(); x+=w)
        {
            screenBuffer.GetPixels(x, y, std::min(w, screenBuffer.Width() - x - 1), std::min(h, screenBuffer.Height() - y - 1), screenBuffer2);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
            glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, Camera(screenBuffer, coord_all, player, false).Data()); GL_CHECK_ERRORS;

            glfwSwapBuffers(window);
        }
        screenBuffer = screenBuffer1;
        return true;
}

int main(int argc, char** argv) {
    if (!glfwInit())
        return -1;

//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "task1 base project", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, OnKeyboardPressed);
    glfwSetCursorPosCallback(window, OnMouseMove);
    glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
    glfwSetScrollCallback(window, OnMouseScroll);

    if (initGL() != 0)
        return -1;

    GLenum gl_error = glGetError();
    while (gl_error != GL_NO_ERROR)
        gl_error = glGetError();


    Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT*2, 4);////////////////////////////////////////////////////////

    Level GameLevel("./Map/maps/1.txt", 4, 1);
    /*std::vector <std::string> hero1= {"./resources/background/1/Hero/big_zombie_idle_anim_f0.png",
                                       "./resources/background/1/Hero/big_zombie_idle_anim_f1.png",
                                       "./resources/background/1/Hero/big_zombie_idle_anim_f2.png",
                                       "./resources/background/1/Hero/big_zombie_idle_anim_f3.png"};*/
    std::vector <std::string> hero1= {"./resources/background/1/Hero/Left/zombie_0.png",
                                      "./resources/background/1/Hero/Left/zombie_1.png",
                                      "./resources/background/1/Hero/Left/zombie_2.png",
                                      "./resources/background/1/Hero/Left/zombie_3.png"};

    Player player{{0,0}, hero1};
    player.PutLevel(GameLevel);

    screenBuffer = GameLevel.background;


    //img.DrawImage();
    //img.SetCoord({32,32});
    //img.DrawImage();

    Image Die("./resources/background/End/gameover.png");


    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;
    Point coord_all = {0, 0};

    //game loop 
    while (!glfwWindowShouldClose(window))
    {
      GLfloat currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;
      glfwPollEvents();
      bool update = true;


      if (GameLevel.end)
      {

        if (GameLevel.win) 
        {
          if (ChangeLevel(screenBuffer, window, GameLevel, player))
            coord_all = player.GetCoord();
        }
        else 
        {
          if (GameLevel.die) 
            screenBuffer = Die;
          coord_all = {0, 0};
          update = false;
        }

      }
      else
      {
        processPlayerMovement(player);
        player.Draw(screenBuffer);
        coord_all = player.GetCoord();
      }


      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
      glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, Camera(screenBuffer, coord_all, player, update).Data()); GL_CHECK_ERRORS;//screenBuffer.Data()); GL_CHECK_ERRORS;//

      glfwSwapBuffers(window);

    }

    glfwTerminate();
    return 0;
  }

