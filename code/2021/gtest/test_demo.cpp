#include<gtest/gtest.h>

int add(int a, int b){
    return a+b;
}

TEST(testCase, test1){
    EXPECT_EQ(add(4,3), 7); // 断言检测两参数是否相等
}

TEST(testCase, test2){
    EXPECT_EQ(add(4,4), 7); // 断言检测两参数是否相等
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv); // 初始化，所有测试都是这里启动的
  return RUN_ALL_TESTS(); // 运行所有测试用例
}