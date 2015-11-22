//----------------------------------------------------------------------------------------
/**
 * \file       spline.cpp
 * \author     Miroslav Miksik & Jaroslav Sloup & Petr Felkel
 * \date       2013
 * \brief      Utility functions and stuff tasks concerning animation curves.
*/
//----------------------------------------------------------------------------------------

#include "spline.h"

//**************************************************************************************************
/// Checks whether vector is zero-length or not.
bool isVectorNull(glm::vec3 &vect) {

  return !vect.x && !vect.y && !vect.z;
}

//**************************************************************************************************
/// Align (rotate and move) current coordinate system to given parameters.
/**
 This function works similarly to \ref gluLookAt, however it is used for object transform
 rather than view transform. The current coordinate system is moved so that origin is moved
 to the \a position. Object's local front (-Z) direction is rotated to the \a front and
 object's local up (+Y) direction is rotated so that angle between its local up direction and
 \a up vector is minimum.

 \param[in]  position           Position of the origin.
 \param[in]  front              Front direction.
 \param[in]  up                 Up vector.
 */
glm::mat4 alignObject(glm::vec3& position, const glm::vec3& front, const glm::vec3& up) {

  glm::vec3 z = -glm::normalize(front);

  if (isVectorNull(z))
    z = glm::vec3(0.0, 0.0, 1.0);

  glm::vec3 x = glm::normalize(glm::cross(up, z));

  if (isVectorNull(x))
    x =  glm::vec3(1.0, 0.0, 0.0);

  glm::vec3 y = glm::cross(z, x);
  //mat4 matrix = mat4(1.0f);
  glm::mat4 matrix = glm::mat4(
      x.x,        x.y,        x.z,        0.0,
      y.x,        y.y,        y.z,        0.0,
      z.x,        z.y,        z.z,        0.0,
      position.x, position.y, position.z, 1.0
  );

  return matrix;
}



/// Number of control points of the animation curve.
const size_t  curveSize   = 6;
//const int OFFSET=2;
/// Control points of the animation curve.
glm::vec3 curveData[] = {
	glm::vec3( 0.35, 0.125,  1.3-1.5),

	glm::vec3(0.3,  0.125, 0.1-1.5),
	glm::vec3(-2.2,  0.125, 0.32-1.5),
	glm::vec3(-2.18, 0.125, 1.2-1.5),
	glm::vec3(-1.98, 0.125, 1.20-1.5),
	glm::vec3(0.12, 0.125, 1.78-1.5)
};

/// Number of control points of the animation curve.
const size_t  cameraCurveSize   = 9;
//const int OFFSET=2;
/// Control points of the animation curve.
glm::vec3 cameraCurveData[] = {
	glm::vec3( 0.00f, 0.80f, 3.00f),
	glm::vec3( 0.00f, 0.30f, 0.00f),
	glm::vec3( 0.70f, 0.30f,-0.174f),

	glm::vec3(-1.27f, 0.20f,-2.50f),
	glm::vec3( 3.27f, 0.20f,-2.50f),
	glm::vec3( 1.46f, 0.40f,-1.30f),
	glm::vec3( 0.02f, 0.28f, 0.45f),
	glm::vec3( 1.93f, 0.63f, 0.92f),
	glm::vec3( 3.24f, 0.88f, 2.48f)
	/*
	glm::vec3( 0.47f, 1.00f, 3.10f),
	glm::vec3( 0.70f, 0.22f,-0.17f),
	glm::vec3(-1.27f, 0.20f,-2.50f),
	glm::vec3( 3.27f, 0.20f,-2.50f),
	glm::vec3( 1.46f, 0.40f,-1.30f),
	glm::vec3( 0.02f, 0.28f, 0.45f),
	glm::vec3( 1.93f, 0.63f, 0.92f),
	glm::vec3( 3.24f, 0.88f, 2.48f)
	*/
};

//**************************************************************************************************
/// Evaluates a position on Catmull-Rom curve segment.
/**
  \param[in] P0       First control point of the curve segment.
  \param[in] P1       Second control point of the curve segment.
  \param[in] P2       Third control point of the curve segment.
  \param[in] P3       Fourth control point of the curve segment.
  \param[in] t        Curve segment parameter. Must be within range [0, 1].
  \return             Position on the curve for parameter \a t.
*/
glm::vec3 evaluateCurveSegment(
    glm::vec3&  P0,
    glm::vec3&  P1,
    glm::vec3&  P2,
    glm::vec3&  P3,
    const float t
) {
  glm::vec3 result(0.0, 0.0, 0.0);

  // evaluate point on a curve segment (defined by the control points P0...P3)
  // for the given value of parametr t

  const float t2 = t*t;
  const float t3 = t*t2;

  result = P0 * (-      t3 + 2.0f*t2 - t       )
      + P1 * (  3.0f*t3 - 5.0f*t2     + 2.0f)
      + P2 * (- 3.0f*t3 + 4.0f*t2 + t       )
      + P3 * (       t3 -      t2           );

  result *= 0.5f;

  return result;
}

//**************************************************************************************************
/// Evaluates a first derivative of Catmull-Rom curve segment.
/**
  \param[in] P0       First control point of the curve segment.
  \param[in] P1       Second control point of the curve segment.
  \param[in] P2       Third control point of the curve segment.
  \param[in] P3       Fourth control point of the curve segment.
  \param[in] t        Curve segment parameter. Must be within range [0, 1].
  \return             First derivative of the curve for parameter \a t.
*/
glm::vec3 evaluateCurveSegment_1stDerivative(
    glm::vec3&  P0,
    glm::vec3&  P1,
    glm::vec3&  P2,
    glm::vec3&  P3,
    const float t
) {
  glm::vec3 result(1.0, 0.0, 0.0);

  // evaluate first derivative for a point on a curve segment (defined by the control points P0...P3)
  // for the given value of parametr t

  const float t2 = t*t;

  result = P0 * (- 3.0f*t2 +  4.0f*t - 1.0f)
      + P1 * (  9.0f*t2 - 10.0f*t       )
      + P2 * (- 9.0f*t2 +  8.0f*t + 1.0f)
      + P3 * (  3.0f*t2 -  2.0f*t       );

  result *= 0.5f;

  return result;
}

//**************************************************************************************************
/// Evaluates a position on a closed curve composed of Catmull-Rom segments.
/**
  \param[in] points   Array of curve control points.
  \param[in] count    Number of curve control points.
  \param[in] t        Parameter for which the position shall be evaluated.
  \return             Position on the curve for parameter \a t.
  \note               Although the range of the paramter is from [0, \a count] (outside the range
                      the curve is periodic) one must presume any value (even negative).
*/
glm::vec3 evaluateClosedCurve(
    glm::vec3 points[],
    const size_t    count,
    const float     t
) {
  glm::vec3 result(0.0, 0.0, 0.0);

  // based on the value of parametr t first find corresponding segment and its control points => i
  // and then call evaluateCurveSegment function with proper parameters to get a point on a closed curve

  float param = cyclic_clamp(t, 0.0f, float(count));
  size_t index = size_t(param);

  result = evaluateCurveSegment(
      points[(index-1+count)%count],
      points[(index        )%count],
      points[(index+1      )%count],
      points[(index+2      )%count],
      param-floor(param)
      );

  return result;
}

//**************************************************************************************************
/// Evaluates a first derivative of a closed curve composed of Catmull-Rom segments.
/**
  \param[in] points   Array of curve control points.
  \param[in] count    Number of curve control points.
  \param[in] t        Parameter for which the derivative shall be evaluated.
  \return             First derivative of the curve for parameter \a t.
  \note               Although the range of the paramter is from [0, \a count] (outside the range
                      the curve is periodic) one must presume any value (even negative).
*/
glm::vec3 evaluateClosedCurve_1stDerivative(
    glm::vec3    points[],
    const size_t count,
    const float  t
) {
  glm::vec3 result(1.0, 0.0, 0.0);

  // based on the value of parametr t first find corresponding curve segment and its control points => i
  // and then call evaluateCurveSegment_1stDerivative function with proper parameters
  // to get a derivative for the given point on a closed curve

  float param = cyclic_clamp(t, 0.0f, float(count));
  size_t index = size_t(param);

  result = evaluateCurveSegment_1stDerivative(
      points[(index-1+count)%count],
      points[(index        )%count],
      points[(index+1      )%count],
      points[(index+2      )%count],
      param-floor(param)
      );

  return result;
}