#include "MyStdAny.hpp"

class TTask {
public:
    template<typename Func>
    TTask(const Func& func) : base_ptr_(std::make_shared<NoArgs<Func> >(NoArgs(func))) {}

    template<typename Func, typename T1>
    TTask(const Func& func, const T1& valueOne) 
        : base_ptr_(std::make_shared<OneArg<Func, T1> >(OneArg(func, valueOne))) {}

    template<typename Func, typename T1, typename T2>
    TTask(const Func& func, const T1& valueOne, const T2& valueTwo) 
        : base_ptr_(std::make_shared<TwoArgs<Func, T1, T2> >(TwoArgs(func, valueOne, valueTwo))) {}

    TTask(const TTask& other) {
        this->base_ptr_ = other.base_ptr_;
    }

    TTask& operator=(const TTask& other) {
        this->base_ptr_ = other.base_ptr_;

        return *this;
    }

    Any operator()() const {
        return base_ptr_->invoke();
    }

private:
    struct Base {
        virtual Any invoke() = 0;
        virtual ~Base() = default;
    };

    template<typename Func>
    class NoArgs : public Base {
    public:
        NoArgs(const Func& func) : func_(func) {}

        Any invoke() override {
            auto rez = func_();

            return Any(rez);
        }

    private:
        Func func_;
    };

    template<typename Func, typename T1>
    class OneArg : public Base {
    public:
        OneArg(const Func& func, const T1& valueOne) : func_(func), valueOne_(valueOne) {}

        Any invoke() override {
            auto rez = func_(valueOne_);

            return Any(rez);
        }

    private:
        Func func_;
        T1 valueOne_;
    };

    template<typename Func, typename T1, typename T2>
    class TwoArgs : public Base {
    public:
        TwoArgs(const Func& func, const T1& valueOne, const T2& valueTwo) 
                : func_(func), valueOne_(valueOne), valueTwo_(valueTwo) {}

        Any invoke() override {
            auto rez = func_(valueOne_, valueTwo_);

            return Any(rez);
        }

    private:
        Func func_;
        T1 valueOne_;
        T2 valueTwo_;
    };

private:
    std::shared_ptr<Base> base_ptr_;
};
