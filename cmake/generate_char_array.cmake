file(READ ${CMAKE_ARGV3} hexdata HEX)
string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1, " hex_split ${hexdata})
file(WRITE ${CMAKE_ARGV4} ${hex_split})
