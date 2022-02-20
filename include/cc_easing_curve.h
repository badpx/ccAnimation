#pragma once

#include <functional>

namespace anim
{
    enum class CurveType : int
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

    using CurveFunction = float(*)(float);
    class EasingCurve
    {
    public:
        EasingCurve() = default;
        EasingCurve(CurveType type);
        EasingCurve(CurveFunction func) : func_(func) {}
        float ValueForProgress(float progress) const;
        bool IsValid() const { return nullptr != func_; }

    private:
        CurveFunction func_ = nullptr;
    };
}
