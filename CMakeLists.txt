# CMakeのバージョンを設定
cmake_minimum_required(VERSION 3.13)
# プロジェクト名と使用する言語を設定
project(test_cmake C)
# a.outという実行ファイルをmain.cppとhello.cppから作成
add_executable(a.out request.c p2ptest/http.c p2ptest/natpmp.c p2ptest/network.c p2ptest/parser.c p2ptest/portmapping.c p2ptest/ssdp.c p2ptest/timeutil.c)
target_link_libraries(a.out PUBLIC -lexpat)
target_link_libraries(a.out PUBLIC -ljson)
