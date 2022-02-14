/*
Disclaimer for Robert Penner's Easing Equations license:
TERMS OF USE - EASING EQUATIONS
Open source under the BSD License.
Copyright © 2001 Robert Penner
All rights reserved.
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of the author nor the names of contributors may be used to endorse or promote products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <algorithm>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2 (M_PI / 2)
#endif
namespace anim
{
using real = float;
/**
 * Easing equation function for a simple linear tweening, with no easing.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeNone(real progress)
{
    return progress;
}
/**
 * Easing equation function for a quadratic (t^2) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeInQuad(real t)
{
    return t*t;
}
/**
* Easing equation function for a quadratic (t^2) easing out: decelerating to zero velocity.
*
* @param t		Current time (in frames or seconds).
* @return		The correct value.
*/
static real easeOutQuad(real t)
{
    return -t*(t-2);
}
/**
 * Easing equation function for a quadratic (t^2) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeInOutQuad(real t)
{
    t*=2.0;
    if (t < 1) {
        return t*t/real(2);
    } else {
        --t;
        return -0.5 * (t*(t-2) - 1);
    }
}
/**
 * Easing equation function for a quadratic (t^2) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeOutInQuad(real t)
{
    if (t < 0.5) return easeOutQuad (t*2)/2;
    return easeInQuad((2*t)-1)/2 + 0.5;
}
/**
 * Easing equation function for a cubic (t^3) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeInCubic(real t)
{
    return t*t*t;
}
/**
 * Easing equation function for a cubic (t^3) easing out: decelerating to zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeOutCubic(real t)
{
    t-=1.0;
    return t*t*t + 1;
}
/**
 * Easing equation function for a cubic (t^3) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeInOutCubic(real t)
{
    t*=2.0;
    if(t < 1) {
        return 0.5*t*t*t;
    } else {
        t -= real(2.0);
        return 0.5*(t*t*t + 2);
    }
}
/**
 * Easing equation function for a cubic (t^3) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeOutInCubic(real t)
{
    if (t < 0.5) return easeOutCubic (2*t)/2;
    return easeInCubic(2*t - 1)/2 + 0.5;
}
/**
 * Easing equation function for a quartic (t^4) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeInQuart(real t)
{
    return t*t*t*t;
}
/**
 * Easing equation function for a quartic (t^4) easing out: decelerating to zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeOutQuart(real t)
{
    t-= real(1.0);
    return - (t*t*t*t- 1);
}
/**
 * Easing equation function for a quartic (t^4) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeInOutQuart(real t)
{
    t*=2;
    if (t < 1) return 0.5*t*t*t*t;
    else {
        t -= 2.0f;
        return -0.5 * (t*t*t*t- 2);
    }
}
/**
 * Easing equation function for a quartic (t^4) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeOutInQuart(real t)
{
    if (t < 0.5) return easeOutQuart (2*t)/2;
    return easeInQuart(2*t-1)/2 + 0.5;
}
/**
 * Easing equation function for a quintic (t^5) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeInQuint(real t)
{
    return t*t*t*t*t;
}
/**
 * Easing equation function for a quintic (t^5) easing out: decelerating to zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeOutQuint(real t)
{
    t-=1.0;
    return t*t*t*t*t + 1;
}
/**
 * Easing equation function for a quintic (t^5) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeInOutQuint(real t)
{
    t*=2.0;
    if (t < 1) return 0.5*t*t*t*t*t;
    else {
        t -= 2.0;
        return 0.5*(t*t*t*t*t + 2);
    }
}
/**
 * Easing equation function for a quintic (t^5) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeOutInQuint(real t)
{
    if (t < 0.5) return easeOutQuint (2*t)/2;
    return easeInQuint(2*t - 1)/2 + 0.5;
}
/**
 * Easing equation function for a sinusoidal (sin(t)) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeInSine(real t)
{
    return (t == 1.0) ? 1.0 : -std::cos(t * M_PI_2) + 1.0;
}
/**
 * Easing equation function for a sinusoidal (sin(t)) easing out: decelerating to zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeOutSine(real t)
{
    return std::sin(t* M_PI_2);
}
/**
 * Easing equation function for a sinusoidal (sin(t)) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeInOutSine(real t)
{
    return -0.5 * (std::cos(M_PI*t) - 1);
}
/**
 * Easing equation function for a sinusoidal (sin(t)) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeOutInSine(real t)
{
    if (t < 0.5) return easeOutSine (2*t)/2;
    return easeInSine(2*t - 1)/2 + 0.5;
}
/**
 * Easing equation function for an exponential (2^t) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeInExpo(real t)
{
    return (t==0 || t == 1.0) ? t : std::pow(2.0, 10 * (t - 1)) - real(0.001);
}
/**
 * Easing equation function for an exponential (2^t) easing out: decelerating to zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeOutExpo(real t)
{
    return (t==1.0) ? 1.0 : 1.001 * (-std::pow(2.0f, -10 * t) + 1);
}
/**
 * Easing equation function for an exponential (2^t) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeInOutExpo(real t)
{
    if (t==0.0) return real(0.0);
    if (t==1.0) return real(1.0);
    t*=2.0;
    if (t < 1) return 0.5 * std::pow(real(2.0), 10 * (t - 1)) - 0.0005;
    return 0.5 * 1.0005 * (-std::pow(real(2.0), -10 * (t - 1)) + 2);
}
/**
 * Easing equation function for an exponential (2^t) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeOutInExpo(real t)
{
    if (t < 0.5) return easeOutExpo (2*t)/2;
    return easeInExpo(2*t - 1)/2 + 0.5;
}
/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeInCirc(real t)
{
    return -(::sqrt(1 - t*t) - 1);
}
/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing out: decelerating to zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeOutCirc(real t)
{
    t-= real(1.0);
    return ::sqrt(1 - t* t);
}
/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeInOutCirc(real t)
{
    t*=real(2.0);
    if (t < 1) {
        return -0.5 * (::sqrt(1 - t*t) - 1);
    } else {
        t -= real(2.0);
        return 0.5 * (::sqrt(1 - t*t) + 1);
    }
}
/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static real easeOutInCirc(real t)
{
    if (t < 0.5) return easeOutCirc (2*t)/2;
    return easeInCirc(2*t - 1)/2 + 0.5;
}
static real easeInElastic_helper(real t, real b, real c, real d, real a, real p)
{
    if (t==0) return b;
    real t_adj = (real)t / (real)d;
    if (t_adj==1) return b+c;
    real s;
    if(a < std::fabs(c)) {
        a = c;
        s = p / 4.0f;
    } else {
        s = p / (2 * M_PI) * std::asin(c / a);
    }
    t_adj -= 1.0f;
    return -(a*std::pow(2.0f,10*t_adj) * std::sin( (t_adj*d-s)*(2*M_PI)/p )) + b;
}
/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
static real easeInElastic(real t, real a, real p)
{
    return easeInElastic_helper(t, 0, 1, 1, a, p);
}
static real easeOutElastic_helper(real t, real /*b*/, real c, real /*d*/, real a, real p)
{
    if (t==0) return 0;
    if (t==1) return c;
    real s;
    if(a < c) {
        a = c;
        s = p / 4.0f;
    } else {
        s = p / (2 * M_PI) * std::asin(c / a);
    }
    return (a*std::pow(2.0f,-10*t) * std::sin( (t-s)*(2*M_PI)/p ) + c);
}
/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing out: decelerating to zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
static real easeOutElastic(real t, real a, real p)
{
    return easeOutElastic_helper(t, 0, 1, 1, a, p);
}
/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
static real easeInOutElastic(real t, real a, real p)
{
    if (t==0) return 0.0;
    t*=2.0;
    if (t==2) return 1.0;
    real s;
    if(a < 1.0) {
        a = 1.0;
        s = p / 4.0f;
    } else {
        s = p / (2 * M_PI) * std::asin(1.0 / a);
    }
    if (t < 1) return -.5*(a*std::pow(2.0f,10*(t-1)) * std::sin( (t-1-s)*(2*M_PI)/p ));
    return a*std::pow(2.0f,-10*(t-1)) * std::sin( (t-1-s)*(2*M_PI)/p )*.5 + 1.0;
}
/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
static real easeOutInElastic(real t, real a, real p)
{
    if (t < 0.5) return easeOutElastic_helper(t*2, 0, 0.5, 1.0, a, p);
    return easeInElastic_helper(2*t - 1.0, 0.5, 0.5, 1.0, a, p);
}
/**
 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
static real easeInBack(real t, real s)
{
    return t*t*((s+1)*t - s);
}
/**
 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing out: decelerating to zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
static real easeOutBack(real t, real s)
{
    t-= real(1.0);
    return t*t*((s+1)*t+ s) + 1;
}
/**
 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
static real easeInOutBack(real t, real s)
{
    t *= 2.0;
    if (t < 1) {
        s *= 1.525f;
        return 0.5*(t*t*((s+1)*t - s));
    } else {
        t -= 2;
        s *= 1.525f;
        return 0.5*(t*t*((s+1)*t+ s) + 2);
    }
}
/**
 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
static real easeOutInBack(real t, real s)
{
    if (t < 0.5) return easeOutBack (2*t, s)/2;
    return easeInBack(2*t - 1, s)/2 + 0.5;
}
static real easeOutBounce_helper(real t, real c, real a)
{
    if (t == 1.0) return c;
    if (t < (4/11.0)) {
        return c*(7.5625*t*t);
    } else if (t < (8/11.0)) {
        t -= (6/11.0);
        return -a * (1. - (7.5625*t*t + .75)) + c;
    } else if (t < (10/11.0)) {
        t -= (9/11.0);
        return -a * (1. - (7.5625*t*t + .9375)) + c;
    } else {
        t -= (21/22.0);
        return -a * (1. - (7.5625*t*t + .984375)) + c;
    }
}
/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing out: decelerating to zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
static real easeOutBounce(real t, real a)
{
    return easeOutBounce_helper(t, 1, a);
}
/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
static real easeInBounce(real t, real a)
{
    return 1.0 - easeOutBounce_helper(1.0-t, 1.0, a);
}
/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
static real easeInOutBounce(real t, real a)
{
    if (t < 0.5) return easeInBounce (2*t, a)/2;
    else return (t == 1.0) ? 1.0 : easeOutBounce (2*t - 1, a)/2 + 0.5;
}
/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
static real easeOutInBounce(real t, real a)
{
    if (t < 0.5) return easeOutBounce_helper(t*2, 0.5, a);
    return 1.0 - easeOutBounce_helper (2.0-2*t, 0.5, a);
}
static inline real qt_sinProgress(real value)
{
    return std::sin((value * M_PI) - M_PI_2) / 2 + real(0.5);
}
static inline real qt_smoothBeginEndMixFactor(real value)
{
    return std::min(std::max(1 - value * 2 + real(0.3), real(0.0)), real(1.0));
}
// SmoothBegin blends Smooth and Linear Interpolation.
// Progress 0 - 0.3      : Smooth only
// Progress 0.3 - ~ 0.5  : Mix of Smooth and Linear
// Progress ~ 0.5  - 1   : Linear only
/**
 * Easing function that starts growing slowly, then increases in speed. At the end of the curve the speed will be constant.
 */
static real easeInCurve(real t)
{
    const real sinProgress = qt_sinProgress(t);
    const real mix = qt_smoothBeginEndMixFactor(t);
    return sinProgress * mix + t * (1 - mix);
}
/**
 * Easing function that starts growing steadily, then ends slowly. The speed will be constant at the beginning of the curve.
 */
static real easeOutCurve(real t)
{
    const real sinProgress = qt_sinProgress(t);
    const real mix = qt_smoothBeginEndMixFactor(1 - t);
    return sinProgress * mix + t * (1 - mix);
}
/**
 * Easing function where the value grows sinusoidally. Note that the calculated  end value will be 0 rather than 1.
 */
static real easeSineCurve(real t)
{
    return (std::sin(((t * M_PI * 2)) - M_PI_2) + 1) / 2;
}
/**
 * Easing function where the value grows cosinusoidally. Note that the calculated start value will be 0.5 and the end value will be 0.5
 * contrary to the usual 0 to 1 easing curve.
 */
static real easeCosineCurve(real t)
{
    return (std::cos(((t * M_PI * 2)) - M_PI_2) + 1) / 2;
}

} // namespace anim