//----------------------------------------------------------------------------------------
/**
 * \file       spline.h
 * \author     Miroslav Miksik & Jaroslav Sloup & Petr Felkel
 * \date       2013
 * \brief      Utility functions and stuff tasks concerning animation curves.
*/
//----------------------------------------------------------------------------------------
#ifndef __SPLINE_H
#define __SPLINE_H

#include "pgr.h" // glm

//**************************************************************************************************
/// Checks whether vector is zero-length or not.
bool isVectorNull(glm::vec3 &vect);

//**************************************************************************************************
/// Align (rotate and move) current coordinate system to given parameters.
/**
 This function work similarly to \ref gluLookAt, however it is used for object transform
 rather than view transform. The current coordinate system is moved so that origin is moved
 to the \a position. Object's local front (-Z) direction is rotated to the \a front and
 object's local up (+Y) direction is rotated so that angle between its local up direction and
 \a up vector is minimum.

 \param[in]  position           Position of the origin.
 \param[in]  front              Front direction.
 \param[in]  up                 Up vector.
 */
glm::mat4 alignObject(glm::vec3& position, const glm::vec3& front, const glm::vec3& up);


extern glm::vec3 curveData[];
extern const size_t  curveSize;

extern glm::vec3 cameraCurveData[];
extern const size_t  cameraCurveSize;


//**************************************************************************************************
/// Cyclic clamping of a value.
/**
 Makes sure that value is not outside the internal [\a minBound, \a maxBound].
 If \a value is outside the interval it treated as periodic value with period equal to the size
 of the interval. A necessary number of periods are added/subtracted to fit the value to the interval.

 \param[in]  value              Value to be clamped.
 \param[in]  minBound           Minimum bound of value.
 \param[in]  maxBound           Maximum bound of value.
 \return                        Value within range [minBound, maxBound].
 \pre                           \a minBound is not greater that \maxBound.
*/
template <typename T>
T cyclic_clamp(const T value, const T minBound, const T maxBound) {

  T amp = maxBound-minBound;
  T val = fmod(value-minBound, amp);

  if (val < T(0))
    val += amp;

  return val+minBound;
}

//**************************************************************************************************
/// Evaluates a position on Catmull-Rom curve segment.
/**
  \param[in] P0       First control point of the curve segment.
  \param[in] P1       Second control point of the curve segment.
  \param[in] P2       Third control point of the curve segment.
  \param[in] P3       Founrth control point of the curve segment.
  \param[in] t        Curve segment parameter. Must be within range [0, 1].
  \return             Position on the curve for parameter \a t.
*/
glm::vec3 evaluateCurveSegment(
    glm::vec3&  P0,
    glm::vec3&  P1,
    glm::vec3&  P2,
    glm::vec3&  P3,
    const float t
);
//**************************************************************************************************
/// Evaluates a fist derivative of Catmull-Rom curve segment.
/**
  \param[in] P0       First control point of the curve segment.
  \param[in] P1       Second control point of the curve segment.
  \param[in] P2       Third control point of the curve segment.
  \param[in] P3       Founrth control point of the curve segment.
  \param[in] t        Curve segment parameter. Must be within range [0, 1].
  \return             First derivative of the curve for parameter \a t.
*/
glm::vec3 evaluateCurveSegment_1stDerivative(
    glm::vec3&  P0,
    glm::vec3&  P1,
    glm::vec3&  P2,
    glm::vec3&  P3,
    const float t
);
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
    glm::vec3    points[],
    const size_t count,
    const float  t
);
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
);

#endif // __SPLINE_H
