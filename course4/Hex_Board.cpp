#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <vector>
#include <queue>

using namespace std; //useful to avoid writing every time std::vector and similar

//define our enum class for Colour
enum class Colour:short{EMPTY, BLUE, RED};
ostream& operator<<(ostream& out, const Colour& c){
    char enum_val[] = {'.','B', 'R'};
  out << enum_val[static_cast<int>(c)];
  return out;
} 

//function useful for player choose turn
void choose_player(Colour & player_color, Colour &computer_color, bool& first_player){
  cout << endl;
    bool color_player = false;
    char player_char;
    while(!color_player){
      cout << "are you blue (B) or red (R) player? (Blue starts first)"<< endl;
      cin >> player_char;
      if(player_char == 'q') exit(1);
      if (player_char == 'B' || player_char == 'R'){
        color_player = true;
        (player_char == 'B')? (first_player = true , player_color = Colour::BLUE, computer_color = Colour::RED, cout << "Well Done! You're the blue player. So you start first" << endl) 
        : (first_player = false, player_color = Colour::RED, computer_color = Colour::BLUE, cout << "Well Done! You're the read player. So you start second" << endl);                     
      }else
      cout << "Please insert B or R to choose player. Enter q to quit" << endl;
    }
  }


//defined a struct for Vertex where the redifinition of < operator is useful for priority queue
typedef struct Vertex{
  int row ;
  int column;
  int vector_index;
  int shortest_path =0;
  Colour col = Colour::EMPTY;
  vector<int> connected_vertex;

  bool operator <(const Vertex& v2) const{
    if(shortest_path== v2.shortest_path) return vector_index> v2.vector_index;
      else
      return shortest_path > v2.shortest_path;
  }
}Vertex;


//defining graph class with all the useful methods
class Hex{

  public:
  //multi purpose constructor
  Hex(int vertex_number=0): _hex(), _vertex_number(vertex_number), _vertex_reached(vertex_number*vertex_number,false),
  _closet_set(vertex_number*vertex_number,0), _move_counter(0)  {
      Init();
      Connected_vertex();
  }

  const vector<Vertex>& get_hex() const {return _hex;}
  int get_vertex_number() const {return _vertex_number;}

  // redifinition of cout accordingly to graph object 
  friend ostream& operator<<(ostream& out, const Hex& h){
  cout << "**************** HEX BOARD ******************" << endl;
  cout << "           R  E  D" << endl;
  string blue_side = " BLUE  ";
    for(int i=0; i<h._vertex_number; i++){
        cout << blue_side[i];
        h.print_space(out, i+1);
      for(int j=0; j<h._vertex_number; j++){
          if(j==h._vertex_number-1) out << h._hex[h.get_vector_index(i,j)].col;
        else out << h._hex[h.get_vector_index(i,j)].col << " - " ;  
      }
      out << endl;
      h.print_space(out ,i+1);
     if(i!=h._vertex_number-1) out << "  \\ / \\ / \\ / \\ / \\ / \\ / \\" << endl;
      
    }

    return out;
}

//check moves for computer player
  void make_random_move(const Colour& computer_colour){
    //cout << "COMPUTER TURN"<< endl;
     int row = rand()%_vertex_number;
     int column = rand()%_vertex_number;
     int vector_index = get_vector_index(row, column);
     if(_hex[vector_index].col!= Colour::EMPTY)make_random_move(computer_colour);
     else{
        _hex[vector_index].col = computer_colour;
        cout << *this << endl;
        cout << "Computer player selects cell (" <<row <<"," <<column <<")"<< endl;
        _move_counter++;
        cout << "NOW it's your turn"<< endl;
     }
     return;
}

//checks moves for human player
  void make_player_move(const Colour& player_colour){
    //cout << "function_PLAYER_call"<< endl;
    bool move_correct=false;
    int row;
    int column;
    while(!move_correct){
      cout << "Choose your move. Insert row index" << endl;
      cin >> row;
      if (row<0 || row> _vertex_number){cout << "Row not valid. TRY AGAIN!" << endl; continue;}
      cout << "Insert column index" << endl;
      cin >> column;
      if (column<0 || column> _vertex_number){cout << "Column not valid. TRY AGAIN!" << endl; continue;}
      int vector_index = get_vector_index(row, column);
      if(_hex[vector_index].col!= Colour::EMPTY){cout << "Cell already selected. TRY AGAIN" << endl; continue;}
      _hex[vector_index].col = player_colour;
      cout << *this << endl;
      cout << "Human player selects cell (" <<row <<"," <<column <<")"<< endl;
      move_correct = true;
      _move_counter++;
     }
     shortest_path(player_colour);
     return;
}

  private:
//initialization
  void Init(){  
    srand(time(0));
    int column =0;
    for(int row =0; row<_vertex_number; row++){
      for(int column =0; column<_vertex_number; column++)
        _hex.push_back(Vertex{row, column,get_vector_index(row,column)});
    }
  }

//defining list of connected vertex for each one vertex
    void Connected_vertex(){
      for( auto& p : _hex){
        int row_up = p.row-1;
        int row_down = p.row+1;
        int column_before = p.column-1;
        int column_after = p.column+1;
        if(column_before>=0) p.connected_vertex.push_back(get_vector_index(p.row, column_before));
        if(column_after<_vertex_number) p.connected_vertex.push_back(get_vector_index(p.row, column_after));
        if(row_up>=0) {
          p.connected_vertex.push_back(get_vector_index(row_up, p.column));
          if(column_after<_vertex_number) p.connected_vertex.push_back(get_vector_index(row_up, column_after));
        }
        if(row_down<_vertex_number) {
          p.connected_vertex.push_back(get_vector_index(row_down, p.column));
          if(column_before>=0) p.connected_vertex.push_back(get_vector_index(row_down, column_before));
        }
      }
    }

  //convert row column pair into and absolute vector index
    inline int get_vector_index(const int& row, const int & column) const {
    return row*_vertex_number+column;}

  //function useful for better printout
    void print_space(ostream& out, int n) const{
      if (n==0) return;
      else {
          out << "  ";
          return print_space( out, n-1);
        }
  }

// Dijkstra algorithm
  void shortest_path(const Colour& col_player){
    int start_vertex;
    for(int i=0; i<_vertex_number; i++){

      start_vertex = (col_player == Colour::BLUE)? i*_vertex_number : i;
      
      if(_hex[start_vertex].col!= col_player)continue;
      cout << "call shortest_path for vertex " << i*_vertex_number << endl;
      clear_path_vector();
      _vertex_reached[start_vertex] = true;
      Vertex temp;
      for(int j : _hex[start_vertex].connected_vertex){
        if(_hex[j].col==col_player){
           temp =_hex[j];
           temp.shortest_path+=1;
           open_set.push(temp);
        }
      }
    //cout<< "vertex 0 dimension _open_set: " << open_set.size()<< endl;
      while(open_set.size()!=0){ 
        Vertex closet_vertex = open_set.top();
        
        if(_vertex_reached[closet_vertex.vector_index]){
          open_set.pop();
        //cout<< "vertex " <<closet_vertex.index << " already reached - dimension _open_set: " << open_set.size()<< endl;
          continue;
        }
        _vertex_reached[closet_vertex.vector_index] = true;
        _closet_set[closet_vertex.vector_index] = closet_vertex.shortest_path;
        open_set.pop();
        for (auto k : closet_vertex.connected_vertex){
          if (!_vertex_reached[k] && _hex[k].col==col_player){
            temp =_hex[k];
            temp.shortest_path+=1;
            open_set.push(temp);
          }
        }
        verify_winner(col_player, closet_vertex);
    }
  }
  return;
}

//verify if someone wins
void verify_winner(const Colour& col_player, const Vertex& closet_vertex){  
  if(_move_counter == _vertex_number*_vertex_number) cout << "MATCH FINISHED. IT'S A DRAW!!!";
  if(col_player==Colour::BLUE && (closet_vertex.vector_index+1)%_vertex_number == 0){
    cout<< "CONGRATS PLAYER BLUE. You WIN!!!!" << endl;
    exit(0);
  }else if(col_player==Colour::RED && (closet_vertex.vector_index)/(_vertex_number*(_vertex_number-1)) != 0){
    cout<< "CONGRATS PLAYER BLUE. You WIN!!!!" << endl;
    exit(0);
  }    
}

void clear_path_vector(){
 for(auto&& a : _vertex_reached) a=false;
  
  for(auto& b : _closet_set) b=0;

}
//private member
  int _vertex_number;
  int _move_counter;
  vector<Vertex> _hex;
  vector<bool> _vertex_reached;
  priority_queue<Vertex> open_set;
  vector<int> _closet_set;
};



int main(void)
{   
  Colour player_color;
  Colour computer_color;
  bool first_player =false;
  bool match_finished =false;
  
  //initialize board with 7 vertex for side
  Hex my_hex(7);
  
  cout <<my_hex;

  choose_player(player_color, computer_color, first_player);

//make moves
    while(!match_finished){
     if(first_player){
       my_hex.make_player_move(player_color);
       my_hex.make_random_move(computer_color);
     }else{
       my_hex.make_random_move(computer_color);
       my_hex.make_player_move(player_color);
    }  
   }
   return 0;
}