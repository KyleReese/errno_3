#include <MedianFilter.h>
#include <unity.h>
#include <iostream>

#ifdef UNIT_TEST

void test_constructor(void) {
  const int buffLen = 10;
  float timeBuff[buffLen] = {0};
  float valBuff[buffLen] = {0};
  MedianFilter<float> f(timeBuff, valBuff, 10);
}

void test_addDataPoint(void){
  const int buffLen = 10;
  float timeBuff[buffLen] = {0};
  float valBuff[buffLen] = {0};
  MedianFilter<float> f(timeBuff, valBuff, 10);

  f.addDataPoint(1.0);

  TEST_ASSERT_EQUAL_FLOAT(timeBuff[0], 1.0);
  TEST_ASSERT_EQUAL_FLOAT(valBuff[0], 1.0);

  f.addDataPoint(5.0);

  TEST_ASSERT_EQUAL_FLOAT(timeBuff[1], 5.0);
  TEST_ASSERT_EQUAL_FLOAT(valBuff[1], 5.0);

  f.addDataPoint(-1);

  TEST_ASSERT_EQUAL_FLOAT(timeBuff[2], -1);
  TEST_ASSERT_EQUAL_FLOAT(valBuff[0], -1);

  // for(int i = 0; i < buffLen; i++)
  //   std::cout << valBuff[i] << std::endl;
  // std::cout << std::endl;
  // for(int i = 0; i < buffLen; i++)
  //   std::cout << timeBuff[i] << std::endl;
}

void test_addManyPoints(void){
  const int buffLen = 10;
  float timeBuff[buffLen] = {0};
  float valBuff[buffLen] = {0};
  MedianFilter<float> f(timeBuff, valBuff, 10);

  for(int i = 0; i < 300; i++){
    f.addDataPoint((float)i);
  }

  TEST_ASSERT_EQUAL_FLOAT(timeBuff[0], 290.0);
  TEST_ASSERT_EQUAL_FLOAT(valBuff[9], 299.0);
}

void test_getFilteredDataPoint(void){
  const int buffLen = 10;
  float timeBuff[buffLen] = {0};
  float valBuff[buffLen] = {0};
  MedianFilter<float> f(timeBuff, valBuff, 10);

  f.addDataPoint(-2);
  f.addDataPoint(5);
  f.addDataPoint(4);
  f.addDataPoint(0);
  f.addDataPoint(10);
  f.addDataPoint(9);
  f.addDataPoint(3);
  f.addDataPoint(-20);
  f.addDataPoint(30);
  f.addDataPoint(15);

  // -20, -2, 0, 3, 4, 5, 9, 10, 15, 30
  // for(int i = 0; i < buffLen; i++) std::cout << valBuff[i] << std::endl;
  TEST_ASSERT_EQUAL_FLOAT(f.getFilteredDataPoint(), 5.0);
}

void test_getFilteredDataPoint_bufferNotFilled(void){
  const int buffLen = 10;
  float timeBuff[buffLen] = {0,0,0,0,0,0,0,0,0,0};
  float valBuff[buffLen] = {0,0,0,0,0,0,0,0,0,0};
  MedianFilter<float> f(timeBuff, valBuff, 10);

  f.addDataPoint(1.0);
  TEST_ASSERT_EQUAL_FLOAT(f.getFilteredDataPoint(), 1.0);
}

void test_incrementCount(void){
  const int buffLen = 10;
  float timeBuff[buffLen] = {0};
  float valBuff[buffLen] = {0};
  MedianFilter<float> f(timeBuff, valBuff, 10);

  TEST_ASSERT_EQUAL_INT(f.incrementCount(9, 10), 0);
  TEST_ASSERT_EQUAL_INT(f.incrementCount(0, 10), 1);
}

void test_sortBuffer(void){
  const int buffLen = 10;
  float timeBuff[buffLen] = {0};
  float valBuff[buffLen] = {0};
  MedianFilter<float> f(timeBuff, valBuff, 10);

  float testBuff[buffLen] = {1, 5, -1, 4, 6, 12, 50, -1, 0, -1};

  f.sortBuffer(testBuff, buffLen);
  // for(int i = 0; i < buffLen; i++) std::cout << testBuff[i] << std::endl;

  for(int i = 0; i < buffLen-1; i++){
    TEST_ASSERT_TRUE(testBuff[i] <= testBuff[i+1]);
  }
}



int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_constructor);
    RUN_TEST(test_addDataPoint);
    RUN_TEST(test_getFilteredDataPoint_bufferNotFilled);
    RUN_TEST(test_addManyPoints);
    RUN_TEST(test_getFilteredDataPoint);
    RUN_TEST(test_incrementCount);
    RUN_TEST(test_sortBuffer);
    UNITY_END();

    return 0;
}


#endif
