#include <array>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <GL/glut.h>
#include <Magick++/Image.h>

using Color = std::array<double, 3>;
std::vector<Color> pixels;
int width, height, center_x, center_y, radius;

double alpha = 0.5;

void display() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_POINTS);
  for (int x = 0; x < width; ++x)
    for (int y = 0; y < height; ++y) {
      int image_x = x, image_y = y;
      double dx = x - center_x, dy = y - center_y;
      double r = std::sqrt(dx * dx + dy * dy);
      if (r < radius) {
        double scale = alpha * radius / (radius - r);
        int px = std::round(center_x + dx * scale);
        int py = std::round(center_y + dy * scale);
        if (px >= 0 && px < width && py >= 0 && py < height) {
          image_x = px;
          image_y = py;
        }
      }
      glColor3dv(&pixels[image_x * height + image_y][0]);
      glVertex2i(x, y);
    }
  glEnd();
  glutSwapBuffers();
}

void keyboard(unsigned char key, int, int) {
  switch (key) {
  case 'd' : alpha /= 1.1; glutPostRedisplay(); break;
  case 'f' : alpha *= 1.1; glutPostRedisplay(); break;
  case 'j' : if (radius > 10) { radius /= 1.1; glutPostRedisplay(); } break;
  case 'k' : radius *= 1.1; glutPostRedisplay(); break;
  case 'q' : std::exit(0);
  }
}

void mouse(int x, int y) {
  center_x = x;
  center_y = y;
  glutPostRedisplay();
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <image file>" << std::endl;
    return 1;
  }

  Magick::Image image;
  image.read(argv[1]);
  width = image.columns();
  height = image.rows();
  center_x = width / 2;
  center_y = height / 2;
  radius = std::min(width, height) / 3;
  pixels.reserve(width * height);
  for (int x = 0; x < width; ++x)
    for (int y = 0; y < height; ++y) {
      Magick::ColorRGB color = image.pixelColor(x, y);
      pixels.push_back({ color.red(), color.green(), color.blue() });
    }

  glutInit(&argc, argv);
  glutInitWindowSize(width, height);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutCreateWindow("Magnifying Glass");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutPassiveMotionFunc(mouse);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, width, height, 0);

  glutMainLoop();
}
