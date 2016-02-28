//
// Created by awbrenn on 2/14/16.
//

#include "viewport_navigation.h"
#include "math.h"

float ZOOM_INCREMENT = 0.1f;
float ZOOM_MIN = 0.1f;
float ROTATE_MIN = 0.1f;


float getMagnitudeOfVector(float x, float y, float z) {
  return sqrtf(x*x+y*y+z*z);
}

void zoomEye(enum VIEWPORT_ACTION viewport_action, float* eye_position, float* view_point)
{
  float zoom_direction;
  float view_direction_magnitude;

  float view_direction[3];
  float view_direction_normalized[3];

  zoom_direction = ((viewport_action == viewport_zoom_in) ? 1.0f : -1.0f);

  // get normalized view direction
  view_direction[0] = *view_point - *eye_position;
  view_direction[1] = *(view_point+1) - *(eye_position+1);
  view_direction[2] = *(view_point+2) - *(eye_position+2);

  view_direction_magnitude = getMagnitudeOfVector(view_direction[0], view_direction[1], view_direction[2]);

  // don't zoom any further if zoom min is reached
  if (((view_direction_magnitude > 0 && view_direction_magnitude <= ZOOM_MIN) ||
        view_direction_magnitude < 0 && view_direction_magnitude >= (-1.0f * ZOOM_MIN))
        && viewport_action==viewport_zoom_in)
  {
    return;
  }

  view_direction_normalized[0] = view_direction[0] / view_direction_magnitude;
  view_direction_normalized[1] = view_direction[1] / view_direction_magnitude;
  view_direction_normalized[2] = view_direction[2] / view_direction_magnitude;

  // set new eye_position;

  *eye_position     += view_direction_normalized[0] * zoom_direction * ZOOM_INCREMENT;
  *(eye_position+1) += view_direction_normalized[1] * zoom_direction * ZOOM_INCREMENT;
  *(eye_position+2) += view_direction_normalized[2] * zoom_direction * ZOOM_INCREMENT;
}

// yields the result of rotating the point (x,y,z) about the line through (a,b,c)
// with direction vector ⟨u,v,w⟩ (where u2 + v2 + w2 = 1) by the angle θ.
void rotateByArbitraryAxis(float x, float y, float z,
                           float a, float b, float c,
                           float u, float v, float w,
                           float theta, float *result)
{
  float cos_theta = cosf(theta);
  float sin_theta = sinf(theta);

  (*(result    )) = (a*(v*v + w*w) - u*(b*v + c*w - u*x - v*y - w*z)) * (1-cos_theta) + x*cos_theta + ((-1.0f)*c*v + b*w - w*y + v*z)*sin_theta;
  (*(result + 1)) = (b*(u*u + w*w) - v*(a*u + c*w - u*x - v*y - w*z)) * (1-cos_theta) + y*cos_theta + (        c*u - a*w + w*x - u*z)*sin_theta;
  (*(result + 2)) = (c*(u*u + v*v) - w*(a*u + b*v - u*x - v*y - w*z)) * (1-cos_theta) + z*cos_theta + ((-1.0f)*b*u + a*v - v*x + u*y)*sin_theta;

}

// get the axis that is perpendicular to vector u and the y axis
void getRotationAxis(float *u, float *result) {
  (*(result  )) = (-1.0f)*(*(u+2));
  (*(result+1)) = 0.0f;
  (*(result+2)) = (*u);

}


void rotateEye(int x, int y, int start_x, int start_y, float* eye_position, float* view_point) {
  float view_point_to_eye[3];
  float distance_view_point_to_eye;
  float magnitude_of_rotation_axis;
  float change_in_yaw;
  float change_in_pitch;
  float rotation_axis[3];


  change_in_pitch =         ((float) y - start_y) / 500.0f;
  change_in_yaw = (-1.0f) * ((float) x - start_x) / 500.0f;

  view_point_to_eye[0] = *(eye_position    ) - *(view_point    );
  view_point_to_eye[1] = *(eye_position + 1) - *(view_point + 1);
  view_point_to_eye[2] = *(eye_position + 2) - *(view_point + 2);

  distance_view_point_to_eye = getMagnitudeOfVector(view_point_to_eye[0],
                                                    view_point_to_eye[1],
                                                    view_point_to_eye[2]);

  // normalize view_point_to_eye
  view_point_to_eye[0] /= distance_view_point_to_eye;
  view_point_to_eye[1] /= distance_view_point_to_eye;
  view_point_to_eye[2] /= distance_view_point_to_eye;

  // rotate the eye about the y axis relative to the view point
  rotateByArbitraryAxis(*(eye_position), *(eye_position+1), *(eye_position+2),
                        *(view_point)  , *(view_point+1)  , *(view_point+2),
                        0.0f, 1.0f, 0.0f, change_in_yaw, eye_position);

  getRotationAxis(view_point_to_eye, rotation_axis);

  // normalize rotation axis
  magnitude_of_rotation_axis = getMagnitudeOfVector(rotation_axis[0], rotation_axis[1], rotation_axis[2]);
  rotation_axis[0] /= magnitude_of_rotation_axis;
  rotation_axis[1] /= magnitude_of_rotation_axis;
  rotation_axis[2] /= magnitude_of_rotation_axis;

  // rotate the eye about the rotation axis relative to the viewpoint
  rotateByArbitraryAxis(*(eye_position ), *(eye_position +1), *(eye_position +2),
                        *(view_point   ), *(view_point   +1), *(view_point   +2),
                        *(rotation_axis), *(rotation_axis+1), *(rotation_axis+2),
                        change_in_pitch, eye_position);
}
