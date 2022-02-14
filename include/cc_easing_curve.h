#pragma once

#include <functional>

namespace anim
{
    enum class CurveType
    {
        Linear,
        InQuad,
        OutQuad,
        InOutQuad,
        OutInQuad,
        InCubic,
        OutCubic,
        InOutCubic,
        OutInCubic,
        InQuart,
        OutQuart,
        InOutQuart,
        OutInQuart,
        InQuint,
        OutQuint,
        InOutQuint,
        OutInQuint,
        InSine,
        OutSine,
        InOutSine,
        OutInSine,
        InExpo,
        OutExpo,
        InOutExpo,
        OutInExpo,
        InCirc,
        OutCirc,
        InOutCirc,
        OutInCirc,
        InElastic,
        OutElastic,
        InOutElastic,
        OutInElastic,
        InBack,
        OutBack,
        InOutBack,
        OutInBack,
        InBounce,
        OutBounce,
        InOutBounce,
        OutInBounce,
        InCurve,
        OutCurve,
        SineCurve,
        CosineCurve,
    };

    using CurveFunction = std::function<float(float)>;
    class EasingCurve
    {
    public:

        EasingCurve(CurveType type);
        virtual float ValueForProgress(float progress) const;

    private:
        CurveType type_;
        CurveFunction func_;
    };

    EasingCurve *GetCurveInstance(CurveType type);
}