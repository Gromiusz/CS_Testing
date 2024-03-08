#include <gtest/gtest.h>
#include <algorithm>
#include <array>
#include <vector>
#include <functional>
#include <tuple>
#include <utility>
#include <execution>

struct SortOnArray : public testing::Test {
    std::array<int, 4> int_arr {2,5,3,8};
    std::array<double, 4> doube_arr {12.4, 5.3, 4.0, 8.5};
};

struct SortOnArrayWithParam : public testing::TestWithParam<std::array<int, 4>> {
    std::array<int, 4> int_arr_expected {2,3,5,8};
};

struct SortOnVectorWithParam : public testing::TestWithParam<std::pair<std::vector<int>, std::vector<int>>> {
};

struct SortOnVectorWithParamAndCustomComp : public testing::TestWithParam<std::tuple<std::vector<int>, std::function<bool(int, int)>, std::vector<int>>> {
};

//********************************************************

TEST_F(SortOnArray, TestOnArray)
{
    //GIVEN
    std::array<int, 4> int_arr_expected {2,3,5,8};
    std::array<double, 4> int_double_expected {4.0, 5.3, 8.5, 12.4};

    //WHEN
    std::sort(begin(int_arr), end(int_arr));
    std::sort(begin(doube_arr), end(doube_arr));

    //THEN
    EXPECT_EQ(int_arr, int_arr_expected);
    EXPECT_EQ(doube_arr, int_double_expected);
}

TEST_F(SortOnArray, TestOnArrayWithGreater)
{
    //GIVEN
    std::array<int, 4> int_arr_expected {8,5,3,2};
    std::array<double, 4> int_double_expected {12.4, 8.5, 5.3, 4.0};

    //WHEN
    std::sort(begin(int_arr), end(int_arr), std::greater<int>());
    std::sort(begin(doube_arr), end(doube_arr), std::greater<double>());

    //THEN
    EXPECT_EQ(int_arr, int_arr_expected);
    EXPECT_EQ(doube_arr, int_double_expected);
}

//**************************************************

INSTANTIATE_TEST_SUITE_P(ThreeElemArr, SortOnArrayWithParam, testing::Values(std::array<int, 4> {2,5,3,8},
                                                                            std::array<int, 4> {2,5,8,3},
                                                                            std::array<int, 4> {5,2,3,8},
                                                                            std::array<int, 4> {8,2,3,5},
                                                                            std::array<int, 4> {8,3,2,5},
                                                                            std::array<int, 4> {8,5,2,3},
                                                                            std::array<int, 4> {5,3,2,8}));


TEST_P(SortOnArrayWithParam, TestOnArrayWithParam)
{
    //GIVEN
    auto int_arr = GetParam();

    //WHEN
    std::sort(begin(int_arr), end(int_arr));

    //THEN
    EXPECT_EQ(int_arr, int_arr_expected);
}

//************************************************

std::vector<int> expectedAscendingVector {1, 2, 3};

INSTANTIATE_TEST_SUITE_P(ThreeElemVec, SortOnVectorWithParam, testing::Values(std::make_pair(std::vector<int>{1, 2, 3}, expectedAscendingVector),
                                                                            std::make_pair(std::vector<int>{1, 3, 2}, expectedAscendingVector),
                                                                            std::make_pair(std::vector<int>{2, 1, 3}, expectedAscendingVector),
                                                                            std::make_pair(std::vector<int>{2, 3, 1}, expectedAscendingVector),
                                                                            std::make_pair(std::vector<int>{3, 1, 2}, expectedAscendingVector),
                                                                            std::make_pair(std::vector<int>{3, 2, 1}, expectedAscendingVector)));


TEST_P(SortOnVectorWithParam, TestOnVectorWithParam)
{
    //GIVEN
    auto [v, expected] = GetParam();

    //WHEN
    std::sort(begin(v), end(v));

    //THEN
    EXPECT_EQ(v, expected);
}

//************************************************

std::vector<int> expectedDescendingVector {3, 2, 1};
auto AscendingSquare = [](int first, int second){return first*first <= second*second;};
auto DescendingSquare = [](int first, int second){return first*first >= second*second;};

INSTANTIATE_TEST_SUITE_P(ThreeElemVec2, SortOnVectorWithParamAndCustomComp, testing::Values(std::make_tuple(std::vector<int>{1, 2, 3}, std::less<int>{}, expectedAscendingVector),
                                                                                            std::make_tuple(std::vector<int>{1, 3, 2}, std::less<int>{},expectedAscendingVector),
                                                                                            std::make_tuple(std::vector<int>{3, 1, 2}, std::less<int>{}, expectedAscendingVector),
                                                                                            std::make_tuple(std::vector<int>{1, 2, 3}, std::greater<int>{}, expectedDescendingVector),
                                                                                            std::make_tuple(std::vector<int>{1, 3, 2}, std::greater<int>{}, expectedDescendingVector),
                                                                                            std::make_tuple(std::vector<int>{3, 1, 2}, std::greater<int>{}, expectedDescendingVector),
                                                                                            std::make_tuple(std::vector<int>{1, 2, 3}, AscendingSquare, expectedAscendingVector),
                                                                                            std::make_tuple(std::vector<int>{1, 3, 2}, AscendingSquare, expectedAscendingVector),
                                                                                            std::make_tuple(std::vector<int>{3, 1, 2}, AscendingSquare, expectedAscendingVector),
                                                                                            std::make_tuple(std::vector<int>{1, 2, 3}, DescendingSquare, expectedDescendingVector),
                                                                                            std::make_tuple(std::vector<int>{1, 3, 2}, DescendingSquare, expectedDescendingVector),
                                                                                            std::make_tuple(std::vector<int>{3, 1, 2}, DescendingSquare, expectedDescendingVector)));


TEST_P(SortOnVectorWithParamAndCustomComp, TestOnVectorWithParam2)
{
    //GIVEN
    auto [v, comp, expected] = GetParam();

    //WHEN
    std::sort(begin(v), end(v), comp);

    //THEN
    EXPECT_EQ(v, expected);
}

//***************************************

TEST(SortOnString, TestOnString)
{
    //GIVEN
    std::string s1 = "aBdc";
    std::string s1_expected = "Bacd";

    //WHEN
    std::sort(begin(s1), end(s1));

    //THEN
    EXPECT_EQ(s1, s1_expected);
}

TEST(executionSort, TestOnConstExecution)
{
    //GIVEN
    std::array<int, 4> int_arr {2,4,1,3};

    //WHEN
    std::sort(std::execution::par, begin(int_arr), end(int_arr));

    //THEN
    std::array<int, 4> int_arr_expected {1,2,3,4};
    EXPECT_EQ(int_arr, int_arr_expected);
}

TEST(executionSort2, TestOnConstExecution2)
{
    //GIVEN
    std::array<int, 4> int_arr {2,4,1,3};

    //WHEN
    std::sort(std::execution::par_unseq, begin(int_arr), end(int_arr));

    //THEN
    std::array<int, 4> int_arr_expected {1,2,3,4};
    EXPECT_EQ(int_arr, int_arr_expected);
}
