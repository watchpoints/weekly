#include "gtest/gtest.h"
#include <string>
int test(int a, int b)
{
        return a+b;
}
 
TEST(test, AddTest) {
        EXPECT_EQ(4, test(2, 2));
}
//https://google.github.io/googletest/samples.html
//https://google.github.io/googletest/samples.html
//https://blog.csdn.net/russell_tao/article/details/7333226
TEST(MyTest, foobar) {
    std::string foo("foobar");
    std::string bar("foobar");
    ASSERT_STREQ(foo.c_str(), bar.c_str()); // working
    EXPECT_FALSE(false); // error
}

int main(int argc, char **argv){
        ::testing::InitGoogleTest( &argc, argv );
        return RUN_ALL_TESTS();
}