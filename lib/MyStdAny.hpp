#pragma once
#include <iostream>

class Any {
public:
    Any() : val_(nullptr) {}

    template<typename T>
    Any(T val) : val_(std::make_unique<Impl<T> >(val)) {}

    Any(const Any& other) {
        if (other.val_ != nullptr) {
            this->val_ = other.val_->clone();
        } else {
            this->val_ = nullptr;
        }
    }

    Any& operator=(const Any& other) {
        if (other.val_ != nullptr) {
            this->val_ = other.val_->clone();
        } else {
            this->val_ = nullptr;
        }

        return *this;
    }

    template<typename T>
    static T any_cast(const Any& value) {
        if (typeid(T) == value.val_->type()) {
            return static_cast<Impl<T>*>(value.val_.get())->getValue();
        } else {
            throw std::bad_cast();
        }
    }

    template<typename T>
    operator T() const {
        return any_cast<T>(*this);
    }

private:
    struct Base {
        virtual ~Base() = default;
        virtual std::unique_ptr<Base> clone() const = 0;
        virtual const std::type_info& type() const = 0;
    };

    template<typename T>
    class Impl : public Base {
    public:
        Impl(T val) : value(val) {}

        std::unique_ptr<Base> clone() const override {
            return std::make_unique<Impl>(value);
        }

        T getValue() const {
            return value;
        }

        const std::type_info& type() const override {
            return typeid(T);
        }

    private:
        T value;
    };

private:
    std::unique_ptr<Base> val_;
};
