#include <chrono>
#include <thread>
#include "cc_value_animation.h"
#include <iostream>
class A {
    public:
    A() = default;
    A(int p) : prop_(p) {}
    void set_prop(int p) { prop_ = p; std::cout << "p=" << prop_ << std::endl; }
    int prop() const { return prop_; }

    int prop_;
};

using namespace std::chrono;
int main() {
    anim::ValueAnimation<size_t> anim1({0, 1000, 500});
    assert(anim1.state() == anim::Animation::State::kStopped);
    anim1.SetDuration(1000);
    // anim1.set_direction(anim::Animation::Direction::kReverse);
    assert(anim1.GetDuration() == 1000);
    auto keyframes = anim1.keyframes();
    assert(keyframes.size() == 3);
    assert(keyframes[0].value() == 0);
    assert(keyframes[0].progress() == 0.0f);
    assert(keyframes[1].value() == 1000);
    assert(keyframes[1].progress() == 0.5f);
    // anim1.subscriber_= ([](const int &value) {printf("%d\n", value);});
    A a;
    anim1.subscriber_= (std::bind(&A::set_prop, &a, std::placeholders::_1));
    anim1.Start();
    assert(anim1.state() == anim::Animation::State::kRunning);
    while (anim1.state() != anim::Animation::State::kStopped) {
        auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        anim1.UpdateAnimationFrame(ms.count());
        std::this_thread::sleep_for(milliseconds(10));
    }
    anim::ValueAnimation<float> anim2({anim::Keyframe<float>(1.0f, 100), anim::Keyframe<float>(0.0f, 0)});
    anim::ValueAnimation<double> anim3({1, 3, 5, 7, 9});
    // anim::ValueAnimation<A> anim4(A(0), A(1));
    return 0;
}
