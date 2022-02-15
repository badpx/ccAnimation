
#include <map>
#include "cc_easing_curve.h"
#include "easing.cc"

namespace anim
{
    static CurveFunction CurveToFunc(const CurveType &type)
    {
        switch (type)
        {
        case CurveType::InQuad:
            return &easeInQuad;
        case CurveType::OutQuad:
            return &easeOutQuad;
        case CurveType::InOutQuad:
            return &easeInOutQuad;
        case CurveType::OutInQuad:
            return &easeOutInQuad;
        case CurveType::InCubic:
            return &easeInCubic;
        case CurveType::OutCubic:
            return &easeOutCubic;
        case CurveType::InOutCubic:
            return &easeInOutCubic;
        case CurveType::OutInCubic:
            return &easeOutInCubic;
        case CurveType::InQuart:
            return &easeInQuart;
        case CurveType::OutQuart:
            return &easeOutQuart;
        case CurveType::InOutQuart:
            return &easeInOutQuart;
        case CurveType::OutInQuart:
            return &easeOutInQuart;
        case CurveType::InQuint:
            return &easeInQuint;
        case CurveType::OutQuint:
            return &easeOutQuint;
        case CurveType::InOutQuint:
            return &easeInOutQuint;
        case CurveType::OutInQuint:
            return &easeOutInQuint;
        case CurveType::InSine:
            return &easeInSine;
        case CurveType::OutSine:
            return &easeOutSine;
        case CurveType::InOutSine:
            return &easeInOutSine;
        case CurveType::OutInSine:
            return &easeOutInSine;
        case CurveType::InExpo:
            return &easeInExpo;
        case CurveType::OutExpo:
            return &easeOutExpo;
        case CurveType::InOutExpo:
            return &easeInOutExpo;
        case CurveType::OutInExpo:
            return &easeOutInExpo;
        case CurveType::InCirc:
            return &easeInCirc;
        case CurveType::OutCirc:
            return &easeOutCirc;
        case CurveType::InOutCirc:
            return &easeInOutCirc;
        case CurveType::OutInCirc:
            return &easeOutInCirc;
        case CurveType::InCurve:
            return &easeInCurve;
        case CurveType::OutCurve:
            return &easeOutCurve;
        case CurveType::SineCurve:
            return &easeSineCurve;
        case CurveType::CosineCurve:
            return &easeCosineCurve;
        case CurveType::Linear:
        default:
            return &easeNone;
        };
    }

    EasingCurve::EasingCurve(CurveType type) : func_(CurveToFunc(type)) {}

    float EasingCurve::ValueForProgress(float progress) const { 
        return func_ ? func_(progress) : progress;
    }
}
