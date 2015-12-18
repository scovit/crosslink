#include <functional>

#ifndef WINDOWER_HPP
#define WINDOWER_HPP

namespace renderer {

  class windower {
  protected:
    bool is_created;
    struct {
      double x;
      double y;
    } mouse;

    struct {
      int x;
      int y;
    } window;

  public:
    virtual int create_window(int w, int h) = 0;
    virtual void swap() = 0;
    virtual void destroy() = 0;
    virtual void process_events() = 0;

    void getWindowsize(int *x, int *y) {
      *x = window.x;
      *y = window.y;
    }

    // Events
    std::function<void(windower *, int, int)> Reshaper;
    std::function<void(windower *, int, int)> Keyboarder;
    std::function<void(windower *, int, int, double, double)> Mouser;

    // Creators and destructors
    windower () {
      is_created = false;
      Reshaper = 0;
      Keyboarder = 0;
    }

    ~windower () {
      if (is_created)
	this -> destroy();
    }

  };

}

#endif
