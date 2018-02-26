#include <map>
#include <boost/program_options.hpp>

#include "CSVReader.h"
#include "MoveList.h"
#include "Board.h"
#include "HashTable.h"

using namespace std;
using namespace boost::program_options;
using namespace realcore;

typedef struct structPositionInfo{
  structPositionInfo(const MoveList &board_list, const string game_result)
  : black_win(0), white_win(0), draw(0)
  {
    position_str = board_list.str();

    AddResult(game_result);
  }

  void AddResult(const string game_result){
    if(game_result == "BlackWin"){
      black_win++;
    }else if(game_result == "WhiteWin"){
      white_win++;
    }else if(game_result == "Draw"){
      draw++;
    }else{
      assert(false);
    }
  }

  const size_t GetCount() const{
    return black_win + white_win + draw;
  }

  size_t black_win;
  size_t white_win;
  size_t draw;
  string position_str;
}PositionInfo;

void AddGamePosition(const MoveList &board_list, const string game_result, multimap<HashValue, PositionInfo> * const game_position_db);
bool IsSamePosition(const MoveList &board_list, const PositionInfo &position_info);

int main(int argc, char* argv[])
{
  // オプション設定
  options_description option;

  option.add_options()
    ("db", value<string>(), "棋譜データベース(csv)")
    ("help,h", "ヘルプを表示");
  
  variables_map arg_map;
  store(parse_command_line(argc, argv, option), arg_map);

  if(arg_map.count("help") || !arg_map.count("db")){
    cout << "Usage: " << argv[0] << " [options]" << endl;
    cout << option;
    cout << endl;
    cout << "Outputs:" << endl;
    cout << "\tGame Position database in standard output" << endl;

    return 0;
  }

  // 棋譜データベースの読込
  const string game_record_db_file = arg_map["db"].as<string>();

  map<string, StringVector> game_record_db;
  ReadCSV(game_record_db_file, &game_record_db);

  const auto game_result_list = game_record_db["game_result"];
  const auto game_record_list = game_record_db["game_record"];

  multimap<HashValue, PositionInfo> game_position_db;

  for(size_t i=0, size=game_record_list.size(); i<size; i++){
    const auto game_record_str = game_record_list[i];
    const auto game_result = game_result_list[i];

    MoveList board_move, game_record(game_record_str);
    Board board;

    for(const auto move : game_record){
      const bool is_normal_move = board.IsNormalMove(move);
      
      if(!is_normal_move){
        break;
      }

      board.MakeMove(move);
      board_move += move;

      AddGamePosition(board_move, game_result, &game_position_db);
    }
  }

  // 出力
  cout << "game_position_id,game_position,position_hash,record_count,record_black_win,record_white_win,record_draw" << endl;
  size_t id = 0;

  for(const auto& elem : game_position_db){
    const auto &position_info = elem.second;

    // game_position_id
    stringstream ss;
    ss << "P";
    ss << setw(9) << setfill('0') << id;
    cout << ss.str() << ",";
    id++;

    // game_position
    cout << position_info.position_str << ",";

    // position_hash
    MoveList position_list(position_info.position_str);
    cout << CalcSymmetricHashValue(position_list) << ",";

    // record_count
    cout << position_info.GetCount() << ",";

    // record_black_win
    cout << position_info.black_win << ",";

    // record_white_win
    cout << position_info.white_win << ",";

    // record_draw
    cout << position_info.draw << endl;
  }
  
  return 0;
}

void AddGamePosition(const MoveList &board_list, const string game_result, multimap<HashValue, PositionInfo> * const game_position_db)
{
  const auto hash_value = CalcSymmetricHashValue(board_list);
  const auto find_it = game_position_db->find(hash_value);

  if(find_it == game_position_db->end()){
    game_position_db->emplace(hash_value, PositionInfo(board_list, game_result));
    return;
  }

  const auto range = game_position_db->equal_range(hash_value);

  for(auto it=range.first; it!=range.second; ++it){
    auto &position_info = it->second;

    if(IsSamePosition(board_list, position_info)){
      position_info.AddResult(game_result);
      return;
    }
  }

  game_position_db->emplace(hash_value, PositionInfo(board_list, game_result));
}

bool IsSamePosition(const MoveList &board_list, const PositionInfo &position_info)
{
  MoveList position_list;
  GetMoveList(position_info.position_str, &position_list);
  BitBoard position_bit_board(position_list);

  for(const auto symmetry : GetBoardSymmetry()){
    MoveList check_list;
    GetSymmetricMoveList(board_list, symmetry, &check_list);
    BitBoard check_bit_board(check_list);

    if(check_bit_board == position_bit_board){
      return true;
    }
  }

  return false;
}
