SET(LIBS crypto ${GTEST_BOTH_LIBRARIES})

ADD_EXECUTABLE(base64_unittest ../src/base64.cxx base64_unittest.cxx)
TARGET_LINK_LIBRARIES(base64_unittest ${LIBS})
ADD_TEST(NAME base64_unittest COMMAND base64_unittest)

