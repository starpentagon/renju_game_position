# Renju Game Position
This program generates a game position database from a game record database

## Game record database format
The game record format is the csv file with the following columns:
* game_result
* game_record

## Game position database format
The game position format is the columns:
* game_position_id: P + NNNNNNNN, ex: P12345678
* game_position: [a-o] format
* record_count: the game position count appeared in the record
* record_black_win: black win count in the record
* record_white_win: white win count in the record
* record_draw: draw count in the record

## Requirements
* C++ compiler
   * clang 3.8 or newer
* CMake 3.5.1 or newer
* boost 1.63 or newer
* ccache 3.3.4 or newer(optional)
