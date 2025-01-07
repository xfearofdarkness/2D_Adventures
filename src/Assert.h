//
// Created by Jamie Huta on 05.01.25.
//

#ifndef GAME_ASSERT_H
#define GAME_ASSERT_H

#include <iostream>

#define ASSERT(condition) if (!condition) __builtin_trap();


#endif // GAME_ASSERT_H
