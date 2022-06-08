#include <iostream>

#include <GL/glut.h>
#include <Magick++/Image.h>

Magick::Image image;
int width, height, center_x, center_y, radius;
double alpha = 0.5;

void display() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  int range = std::pow(2, image.modulusDepth());
  for (int x = 0; x < width; ++x)
    for (int y = 0; y < height; ++y) {
      int image_x = x, image_y = y;
      double dx = x - center_x, dy = y - center_y;
      double r = std::sqrt(dx * dx + dy * dy);
      if (r <= radius) {
        dx /= r; dy /= r;
        double scale = alpha * r * radius / (alpha * r + radius);
        int px = std::round(center_x + dx * scale);
        int py = std::round(center_y + dy * scale);
        if (px >= 0 && px < width && py >= 0 && py < height) {
          image_x = px;
          image_y = py;
        }
      }
      auto color = image.pixelColor(image_x, image_y);
      double red = color.quantumRed() / range;
      double green = color.quantumGreen() / range;
      double blue = color.quantumBlue() / range;
      glColor3d(red, green, blue);
      glVertex2i(x, y);
    }
  glutSwapBuffers();
}

void mouseMotion(int x, int y) {
  center_x = x;
  center_y = y;
  glutPostRedisplay();
}

void mouseWheel(int button, int dir, int x, int y) {
  glutPostRedisplay();
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <image file>" << std::endl;
    return 1;
  }

  image.read(argv[1]);
  width = image.columns();
  height = image.rows();
  center_x = width / 2;
  center_y = height / 2;
  radius = std::min(width, height) / 3;

  glutInit(&argc, argv);
  glutInitWindowSize(width, height);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutCreateWindow("Magnifying Glass");
  glutDisplayFunc(display);
  glutMotionFunc(mouseMotion);
  // glutMouseWheelFunc(mouseWheel);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, width, height, 0);

  glutMainLoop();
}
