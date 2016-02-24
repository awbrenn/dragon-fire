//
// Created by awbrenn on 2/14/16.
//

#ifndef DRAGON_FIRE_VIEWPORT_NAVIGATION_H
#define DRAGON_FIRE_VIEWPORT_NAVIGATION_H

enum ACTION
{
  mouse_wheel_forward = 3,
  mouse_wheel_backward = 4,
};


void calculateNewEyeLocation(enum ACTION mouse_action, float* eye_position, float* view_point);

#endif //DRAGON_FIRE_VIEWPORT_NAVIGATION_H
