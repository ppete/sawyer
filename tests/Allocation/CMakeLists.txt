add_executable(poolPerf poolPerf.C)
target_link_libraries(poolPerf sawyer)

add_executable(smallObjectPerf smallObjectPerf.C)
target_link_libraries(smallObjectPerf sawyer)

add_executable(poolMultiThread poolMultiThread.C)
target_link_libraries(poolMultiThread sawyer)
