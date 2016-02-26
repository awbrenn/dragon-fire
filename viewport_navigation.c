//
// Created by awbrenn on 2/14/16.
//

#include "viewport_navigation.h"
#include "math.h"

float ZOOM_INCREMENT = 0.1f;
float ZOOM_MIN = 0.1f;


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


float dotProduct(float *a, float *b) {
  return (*(a))*(*(b)) +
         (*(a+1))*(*(b+1)) +
         (*(a+2))*(*(b+2));
}


float getAngle(float x, float y, float z) {
  float view_point_to_eye_normalized[3];
  float forward[3] = {0.0f, 0.0f, 1.0f};
  float distance;

  // get rid of y component
  view_point_to_eye_normalized[0] = x;
  view_point_to_eye_normalized[1] = y;
  view_point_to_eye_normalized[2] = z;

  distance = getMagnitudeOfVector(view_point_to_eye_normalized[0],
                                   view_point_to_eye_normalized[1],
                                   view_point_to_eye_normalized[2]);

  view_point_to_eye_normalized[0] /= distance;
  view_point_to_eye_normalized[1] /= distance;
  view_point_to_eye_normalized[2] /= distance;

  return acosf(dotProduct(view_point_to_eye_normalized, forward));
}





void multiply3by3Matrices(float *a, float *b, float *r) {
  (*(r  )) = (*(a  ))*(*(b  )) + (*(a+1))*(*(b+3)) + (*(a+2))*(*(b+6));
  (*(r+1)) = (*(a  ))*(*(b+1)) + (*(a+1))*(*(b+4)) + (*(a+2))*(*(b+7));
  (*(r+2)) = (*(a  ))*(*(b+2)) + (*(a+1))*(*(b+5)) + (*(a+2))*(*(b+8));
  (*(r+3)) = (*(a+3))*(*(b  )) + (*(a+4))*(*(b+3)) + (*(a+5))*(*(b+6));
  (*(r+4)) = (*(a+3))*(*(b+1)) + (*(a+4))*(*(b+4)) + (*(a+5))*(*(b+7));
  (*(r+5)) = (*(a+3))*(*(b+2)) + (*(a+4))*(*(b+5)) + (*(a+5))*(*(b+8));
  (*(r+6)) = (*(a+6))*(*(b  )) + (*(a+7))*(*(b+3)) + (*(a+8))*(*(b+6));
  (*(r+7)) = (*(a+6))*(*(b+1)) + (*(a+7))*(*(b+4)) + (*(a+8))*(*(b+7));
  (*(r+8)) = (*(a+6))*(*(b+2)) + (*(a+7))*(*(b+5)) + (*(a+8))*(*(b+8));
}


void multiplyEyeByRotationMatrix(float *a, float *b, float *r, float distance) {
  (*(r  )) = ((*(a  ))*(*(b  )) + (*(a+1))*(*(b+1)) + (*(a+2))*(*(b+2))) * distance;
  (*(r+1)) = ((*(a+3))*(*(b  )) + (*(a+4))*(*(b+1)) + (*(a+5))*(*(b+2))) * distance;
  (*(r+2)) = ((*(a+6))*(*(b  )) + (*(a+7))*(*(b+1)) + (*(a+8))*(*(b+2))) * distance;
}


void rotateEye(int x, int y, int start_x, int start_y, float* eye_position, float* view_point) {
  float view_point_to_eye[3];
  float distance_view_point_to_eye;
  float rotation_matrix[9];
  float yaw_matrix[9];
  float change_in_yaw;
  float pitch_matrix[9];
  float change_in_pitch;


  change_in_pitch = (-1.0f) * ((float) x - start_x) / 100.0f;
  change_in_yaw = (-1.0f) * ((float) y - start_y) / 100.0f;

  view_point_to_eye[0] = *(eye_position) - *(view_point);
  view_point_to_eye[1] = *(eye_position + 1) - *(view_point + 1);
  view_point_to_eye[2] = *(eye_position + 2) - *(view_point + 2);

  distance_view_point_to_eye = getMagnitudeOfVector(view_point_to_eye[0],
                                                    view_point_to_eye[1],
                                                    view_point_to_eye[2]);

  // normalize view_point_to_eye
  view_point_to_eye[0] /= distance_view_point_to_eye;
  view_point_to_eye[1] /= distance_view_point_to_eye;
  view_point_to_eye[2] /= distance_view_point_to_eye;

  yaw_matrix[0] = cosf(change_in_yaw);
  yaw_matrix[1] = (-1.0f)*sinf(change_in_yaw);
  yaw_matrix[2] = 0.0f;
  yaw_matrix[3] = sinf(change_in_yaw);
  yaw_matrix[4] = cosf(change_in_yaw);
  yaw_matrix[5] = 0.0f;
  yaw_matrix[6] = 0.0f;
  yaw_matrix[7] = 0.0f;
  yaw_matrix[8] = 1.0f;

  pitch_matrix[0] = cosf(change_in_pitch);
  pitch_matrix[1] = 0.0f;
  pitch_matrix[2] = sinf(change_in_pitch);
  pitch_matrix[3] = 0.0f;
  pitch_matrix[4] = 1.0f;
  pitch_matrix[5] = 0.0f;
  pitch_matrix[6] = (-1.0f)*sinf(change_in_pitch);
  pitch_matrix[7] = 0.0f;
  pitch_matrix[8] = cosf(change_in_pitch);

  multiply3by3Matrices(yaw_matrix, pitch_matrix, rotation_matrix);
  multiplyEyeByRotationMatrix(rotation_matrix, view_point_to_eye, eye_position, distance_view_point_to_eye);
}
