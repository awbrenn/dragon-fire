//
// Created by awbrenn on 2/14/16.
//

#include "viewport_navigation.h"

float ZOOM_INCREMENT = 0.1f;
float ZOOM_MIN = 0.03f;


void zoomEye(enum ACTION mouse_action, float* eye_position, float* view_point)
{
  float zoom_direction;
  float view_direction_magnitude;

  float view_direction_normalized[3];

  if (mouse_action == mouse_wheel_forward) zoom_direction = -1.0f; else zoom_direction = 1.0f;

  // get normalized view direction
  view_direction_normalized[0] = *view_point - *eye_position;
  view_direction_normalized[1] = *(view_point+1) - *(eye_position+1);
  view_direction_normalized[2] = *(view_point+2) - *(eye_position+2);

  view_direction_magnitude = view_direction_normalized[0] + view_direction_normalized[1] + view_direction_normalized[2];

  // don't zoom any further if zoom min is reached
  if (((view_direction_magnitude > 0 && view_direction_magnitude <= ZOOM_MIN) ||
        view_direction_magnitude < 0 && view_direction_magnitude >= (-1.0f * ZOOM_MIN))
        && mouse_action==mouse_wheel_forward)
  {
    return;
  }

  view_direction_normalized[0] /= view_direction_magnitude;
  view_direction_normalized[1] /= view_direction_magnitude;
  view_direction_normalized[2] /= view_direction_magnitude;

  // set new eye_position;

  *eye_position     += view_direction_normalized[0] * zoom_direction * ZOOM_INCREMENT;
  *(eye_position+1) += view_direction_normalized[1] * zoom_direction * ZOOM_INCREMENT;
  *(eye_position+2) += view_direction_normalized[2] * zoom_direction * ZOOM_INCREMENT;
}


void calculateNewEyeLocation(enum ACTION mouse_action, float* eye_position, float* view_point)
{
  if ((mouse_action == mouse_wheel_forward) || (mouse_action == mouse_wheel_backward))
    zoomEye(mouse_action, eye_position, view_point);
}