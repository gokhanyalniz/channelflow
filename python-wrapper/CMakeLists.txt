#
# Configure the channelflow python wrapper pycflib
#
# This file is a part of channelflow version 2.0 https://channelflow.ch.
# License is GNU GPL version 2 or later: ./LICENCE

set(
    pycf_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/cfwrapper.cpp
)

# Define the target with appropriate dependencies
pybind11_add_module(libpycf cfwrapper.cpp)
install(TARGETS libpycf DESTINATION lib)
target_link_libraries(libpycf PUBLIC ${pybind11_LIBRARIES} nsolver chflow)
