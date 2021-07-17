#include <gtest/gtest.h>

#include "../libs/MovingAverage.h"


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}


TEST(MovingAverage, init) {

    // 0 0 0 0 0
    MovingAverage<unsigned int> intAverage(5);
    for (size_t i = 0; i < intAverage.size(); ++i) {
      EXPECT_EQ(intAverage[i], 0);// << "Vectors x and y differ at index " << i;
    }
}

TEST(MovingAverage, push1) {
    MovingAverage<unsigned int> intAverage(5);

    EXPECT_EQ(intAverage[0], 0);
    EXPECT_EQ(intAverage._current_index, 0);
    // 1 2 3 4 0
    // 1 0 0 0 0
    //.push(2).push(3).push(4);
    intAverage.push(1);
    EXPECT_EQ(intAverage._average_sum, 1);

    EXPECT_EQ(intAverage[0], 1);
    EXPECT_EQ(intAverage._current_index, 1);

    // 1 2 0 0 0
    intAverage.push(2);
    EXPECT_EQ(intAverage._average_sum, 3);  // 2 + 1
    EXPECT_EQ(intAverage[0], 2);
    EXPECT_EQ(intAverage[1], 1);
    EXPECT_EQ(intAverage.getAverage(), 1.5);  // 2 + 1 / 2

    EXPECT_EQ(intAverage._current_index, 2);

    // 1 2 3 4 0
    intAverage.push(3).push(4);

    EXPECT_EQ(intAverage[0], 4);
    EXPECT_EQ(intAverage[1], 3);
    EXPECT_EQ(intAverage[2], 2);
    EXPECT_EQ(intAverage[3], 1);
    EXPECT_EQ(intAverage[4], 0);
}

TEST(MovingAverage, push2) {
    MovingAverage<unsigned int> intAverage(5);
    // 11 22 33 44 0
    intAverage.push(11).push(22).push(33).push(44);

    EXPECT_EQ(intAverage[0], 44);
    EXPECT_EQ(intAverage[1], 33);
    EXPECT_EQ(intAverage[2], 22);
    EXPECT_EQ(intAverage[3], 11);
    EXPECT_EQ(intAverage[4], 0);

    EXPECT_EQ(intAverage._current_index, 4);

    EXPECT_EQ(intAverage.getAverage(), 27.5);  // 11 + 22 + 33 + 44 / 4
    EXPECT_EQ(intAverage.size(), 4);

    // 66 22 33 44 55
    intAverage.push(55).push(66);
    EXPECT_EQ(intAverage.size(), 5);
    EXPECT_EQ(intAverage.getAverage(), 44);  // 66 + 11 + 22 + 33 + 44 + 55  / 5
}

TEST(MovingAverage, push3) {
    MovingAverage<unsigned int> intAverage(10);
    // 0 1 1 0 4 0 0 1 0 0
    intAverage.push(0).push(1).push(1).push(0).push(3).push(0).push(0).push(1);

    EXPECT_EQ(intAverage[0], 1);
    EXPECT_EQ(intAverage[1], 0);
    EXPECT_EQ(intAverage[2], 0);
    EXPECT_EQ(intAverage[3], 3);
    EXPECT_EQ(intAverage[4], 0);
    EXPECT_EQ(intAverage[5], 1);
    EXPECT_EQ(intAverage[6], 1);
    EXPECT_EQ(intAverage[7], 0);

    EXPECT_EQ(intAverage._current_index, 8);
    EXPECT_EQ(intAverage.getAverage(), 0.75);  // 0 + 1 + 1 + 0 + 3 + 0 + 0 + 1 / 8
}

TEST(MovingAverage, fill) {
    MovingAverage<unsigned int> intAverage(5);
    // 10 10 10 10 10
    intAverage.fill(10);

    for (size_t i = 0; i < intAverage.size(); ++i) {
      EXPECT_EQ(intAverage[i], 10);
    }
    //EXPECT_EQ(intAverage[3], 10);
}

TEST(MovingAverage, get1) {
    MovingAverage<unsigned int> intAverage(5);
    // 11 22 33 44 0
    intAverage.push(11).push(22).push(33).push(44);

    EXPECT_EQ(intAverage.getAverage(), 27.5);
}

