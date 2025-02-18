#include "TTaskScheduler.hpp"
#include <gtest/gtest.h>

class TTaskSchedulerTest : public ::testing::Test {
protected:
    TTaskScheduler scheduler;
};

TEST_F(TTaskSchedulerTest, Example1) {
    float a = 1;
    float b = -2;
    float c = 0;

    auto id1 = scheduler.add([](float a, float c){return -4 * a * c;}, a, c);
    auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.getFutureResult<float>(id1));
    auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));
    auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));
    auto id5 = scheduler.add([](float a, float v){return v/(2*a);}, a, scheduler.getFutureResult<float>(id3));
    auto id6 = scheduler.add([](float a, float v){return v/(2*a);},a, scheduler.getFutureResult<float>(id4));

    scheduler.executeAll();

    EXPECT_EQ(scheduler.getResult<float>(id5), 2);
    EXPECT_EQ(scheduler.getResult<float>(id6), 0);
}

TEST_F(TTaskSchedulerTest, add) {
    int a = 3;
    int b = 10;

    auto id1 = scheduler.add([](int a, int b){return a + b;}, a, b);

    scheduler.executeAll();

    EXPECT_EQ(scheduler.getResult<int>(id1), 13);
}

TEST_F(TTaskSchedulerTest, getFuture) {
    float a = 11;
    float b = 10;
    float c = 3;

    auto id1 = scheduler.add([](float c, float b){return -c + b;}, c, b);
    auto id2 = scheduler.add([](float b, float v){return b * v + v;}, b, scheduler.getFutureResult<float>(id1));

    scheduler.executeAll();
    
    EXPECT_EQ(scheduler.getResult<float>(id2), 77);
}

TEST_F(TTaskSchedulerTest, AddDifTypes) {
    float a = 10;
    std::string s = "sqrt";

    auto id1 = scheduler.add([](float a, std::string s){return a * a;}, a, s);
    auto id2 = scheduler.add([](float a, float v){return a * v + a;}, a, scheduler.getFutureResult<float>(id1));

    scheduler.executeAll();
    
    EXPECT_EQ(scheduler.getResult<float>(id2), 1010);
}

TEST_F(TTaskSchedulerTest, Example2) {
    float a = 5;
    float b = 2;
    float c = 3;

    auto id1 = scheduler.add([](float c, float b){return c + b;}, c, b);
    auto id2 = scheduler.add([](float b, float v){return b * v + 1;}, b, scheduler.getFutureResult<float>(id1));
    auto id3 = scheduler.add([](float b, float v){return b + v;}, scheduler.getFutureResult<float>(id1), scheduler.getFutureResult<float>(id2));

    scheduler.executeAll();
    
    EXPECT_EQ(scheduler.getResult<float>(id3), 16);
}

TEST_F(TTaskSchedulerTest, Example3) {
    float a = 5;
    float b = 2;
    float c = 3;

    //auto id1 = scheduler.add([](){return 3 * 3;});
    auto id1 = scheduler.add([](float c){return c * c;}, c);
    auto id2 = scheduler.add([](float b, float v){return b + v;}, b, scheduler.getFutureResult<float>(-5));
    auto id3 = scheduler.add([](float c, float b){return c - b;}, c, b);
    auto id4 = scheduler.add([](float b, float v){return b + v;}, scheduler.getFutureResult<float>(id1), scheduler.getFutureResult<float>(id2));
    auto id5 = scheduler.add([](float b, float v){return b * v;}, b, scheduler.getFutureResult<float>(id3));
    auto id6 = scheduler.add([](float b, float v){return b + v;}, scheduler.getFutureResult<float>(id5), scheduler.getFutureResult<float>(id4));

    scheduler.executeAll();
    
    EXPECT_EQ(scheduler.getResult<float>(id6), 22);
}