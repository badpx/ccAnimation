#include "cc_value_animation.h"

class A {
    public:
    A() = default;
    A(int) {}
};

int main() {
    anim::ValueAnimation<int> anim1(100, 0);
    anim::ValueAnimation<float> anim2({anim::Keyframe<float>(1.0f, 100), anim::Keyframe<float>(0.0f, 0)});
    anim::ValueAnimation<double> anim3({1, 3, 5, 7, 9});
    printf("sizeof EasingCurve=%lu\n", sizeof(anim::EasingCurve));
    anim::ValueAnimation<A> anim4(A(0), A(1));
    return 0;
}
