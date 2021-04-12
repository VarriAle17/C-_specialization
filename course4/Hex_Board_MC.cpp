/**********************************************
/  Created by: A Varriale
/  Date: 6/04/21
/  Description: this program plays Hex board 11x11
/  using an AI algorithm based on MC simulation .
/  It plays randomly (1000 attempts for each available position)
/  and chooses the move whith the final higher probability
/
/*********************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <random>       
#include <algorithm>   //random_shuffle
#include <time.h>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std; //useful to avoid writing every time std::vector and similar

//define our enum class for Colour
enum class Colour:short{EMPTY, BLUE, RED};
ostream& operator<<(ostream& out, const Colour& c){
    char enum_val[] = {'.','B', 'R'};
  out << enum_val[static_cast<int>(c)];
  return out;
} 

//function useful for player choose turn
void choose_player(Colour & player_colour, Colour &computer_colour, bool& first_player){
  cout << endl;
    bool colour_player = false;
    char player_char;
    while(!colour_player){
      cout << "are you blue (B) or red (R) player? (Blue starts first)"<< endl;
      cin >> player_char;
      if(player_char == 'q') exit(1);
      if (player_char == 'B' || player_char == 'R'){
        colour_player = true;
        (player_char == 'B')? (first_player = true , player_colour = Colour::BLUE, computer_colour = Colour::RED, cout << "Well Done! You're the blue player. So you start first" << endl) 
        : (first_player = false, player_colour = Colour::RED, computer_colour = Colour::BLUE, cout << "Well Done! You're the read player. So you start second" << endl);                     
      }else
      cout << "Please insert B(blue) or R(red) to choose player. Enter q to quit" << endl;
    }
  }


//defined a struct for Vertex where the redifinition of < operator is useful for priority queue in Dijkstra algorithm
typedef struct Vertex{
  int row;
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
  _move_counter(0) {
      Init();
      Connected_vertex();
      shuffled_hex =_hex;
      srand(time(0));  
  }


  const vector<Vertex>& get_hex() const {return _hex;}

  int get_vertex_number() const {return _vertex_number;}

  // redifinition of cout accordingly to graph object 
  friend ostream& operator<<(ostream& out, const Hex& h){
  cout << "**************** HEX BOARD ******************" << endl;
  cout << "               R  E  D" << endl;
  string blue_side = "   BLUE         ";
    for(int i=0; i<h._vertex_number; i++){
        cout << blue_side[i];
        h.print_space(out, i);
      for(int j=0; j<h._vertex_number; j++){
          if(j==h._vertex_number-1) out << h._hex[h.get_vector_index(i,j)].col;
        else out << h._hex[h.get_vector_index(i,j)].col << " - " ;  
      }
      out << endl;
      h.print_space(out ,i+1);
     if(i!=h._vertex_number-1)h.print_middle_line(out); 
    }

    return out;
}

//choose MC move for computer player
void make_MC_move(const Colour& computer_colour, const Colour& player_colour){
  pair<int,int> higher_probability_move(-1,0);
  int half_vector_size = (shuffled_vertex.size()%2)? shuffled_vertex.size()/2 : (shuffled_vertex.size()/2)+1;
  //if it's first move selects central square
  if(_move_counter==0){
      int row = _vertex_number/2;
      int column = _vertex_number/2;
      int vector_index = get_vector_index(row, column);
      _hex[vector_index].col = computer_colour;
      shuffled_hex[vector_index].col = computer_colour;
        available_vertex.erase(vector_index);
        cout << *this << endl;
        cout << "Computer player selects cell (" <<row <<"," <<column <<")"<< endl;
        _move_counter++;
        return;
  }else{ //it evaluates all possible moves performing 1000 MC simulation for each one.
        cout << "Give me one moment to think...." << endl;
        for(const auto& a : available_vertex){
            //cout <<"verifica vertice " << a.first << endl;  
            available_vertex[a.first] = 0; // set to 0 number of wins
            shuffled_hex[a.first].col = computer_colour;
            create_shuffled_vertex(a.first);
            for(int i=0; i<1000; i++){ //number of attempts for available positions 1000
              //cout << "number of attempts" << i << endl;
              random_shuffle(shuffled_vertex.begin(),shuffled_vertex.end()); //shuffle the vector with available squares
              for(int k =0; k<half_vector_size;k++) //fill the board with player colour
                shuffled_hex[shuffled_vertex[k]].col = player_colour;
              for(int k= half_vector_size; k<shuffled_vertex.size();k++) //fill the board with computer colour
                 shuffled_hex[shuffled_vertex[k]].col = computer_colour;
              
              //at the end of every simulation it perform Dijkstra algorithm
            if (shortest_MCpath(computer_colour) == computer_colour) ++available_vertex[a.first]; //verify if computer is winner
            }
            if(available_vertex[a.first]>higher_probability_move.second){  //selects move with higher probability to win
              higher_probability_move= a;
            }   
        }
        //cout << "higher prob move is vertex " <<higher_probability_move.first << " with wins: " <<higher_probability_move.second << endl;
        //perform the selcted move
        _hex[higher_probability_move.first].col = computer_colour;
        shuffled_hex[higher_probability_move.first].col = computer_colour;
        available_vertex.erase(higher_probability_move.first);
        cout << *this << endl;
        cout << "Computer player selects cell (" <<higher_probability_move.first/_vertex_number <<"," <<higher_probability_move.first%_vertex_number <<")"<< endl;
        _move_counter++;
        //verify if with this move I am the real winner of the match
        shortest_path(computer_colour);
        return;
      }
}


//checks moves for human player
  void make_player_move(const Colour& player_colour){
    //cout << "function_PLAYER_call"<< endl;
    bool move_correct=false;
    int row;
    int column;
    cout << "NOW it's your turn"<< endl;
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
      shuffled_hex[vector_index].col = player_colour;
      available_vertex.erase(vector_index);
      cout << *this << endl;
      cout << "Human player selects cell (" <<row <<"," <<column <<")"<< endl;
      move_correct = true;
      _move_counter++;
     }
     //perform Djkstra algorithm to check if I am the winner
     shortest_path(player_colour);
     return;
}

  private:
//initialization called in the constructor
  void Init(){  
    int column =0;
    int vector_index=0;
    for(int row =0; row<_vertex_number; row++){
      for(int column =0; column<_vertex_number; column++){
         vector_index = get_vector_index(row,column);
         _hex.push_back(Vertex{row, column,vector_index});
         available_vertex.insert(make_pair(vector_index,0));
      }
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

    void print_middle_line(ostream& out) const{
      out << "\\ /";
      for(int k =1; k<_vertex_number-1; k++)out << " \\ /";
      out << " \\" << endl;
    }

// Dijkstra algorithm for MC_simulation - make use of MC variable
  Colour shortest_MCpath(const Colour& col_player){
    int start_vertex;
    //clear_path_vector();
    Colour winner_colour = Colour::EMPTY;
    for(int i=0; i<_vertex_number; i++){

      start_vertex = (col_player == Colour::BLUE)? i*_vertex_number : i;
      
      if(shuffled_hex[start_vertex].col!= col_player)continue;
      //cout << "call shortest_path for vertex " << i*_vertex_number << endl;
      clear_path_vector();
      _vertex_reached[start_vertex] = true;
      Vertex temp;
      for(int j : shuffled_hex[start_vertex].connected_vertex){
        if(shuffled_hex[j].col==col_player){
           temp =shuffled_hex[j];
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
        //_closet_set[closet_vertex.vector_index] = closet_vertex.shortest_path;
        open_set.pop();
        for (auto k : closet_vertex.connected_vertex){
          if (!_vertex_reached[k] && shuffled_hex[k].col==col_player){
            temp =shuffled_hex[k];
            temp.shortest_path+=1;
            open_set.push(temp);
          }
        }
        winner_colour = verify_MCwinner(col_player, closet_vertex);
        if (winner_colour!= Colour::EMPTY) return winner_colour;
    }
  }
  return winner_colour;
}

// Dijkstra algorithm to verify if there is a winner
  void shortest_path(const Colour& col_player){
    if(_move_counter < _vertex_number*2-1)return; //min condition to complete path 
    int start_vertex;
    Colour winner_colour = Colour::EMPTY;
    for(int i=0; i<_vertex_number; i++){

      start_vertex = (col_player == Colour::BLUE)? i*_vertex_number : i;
      
      if(_hex[start_vertex].col!= col_player)continue;
     // cout << "call shortest_path for vertex " << i*_vertex_number << endl;
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
       // _closet_set[closet_vertex.vector_index] = closet_vertex.shortest_path;
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

//print out the winner
void verify_winner(const Colour& col_player, const Vertex& closet_vertex){  
  if(col_player==Colour::BLUE && (closet_vertex.vector_index+1)%_vertex_number == 0){
        cout<< "CONGRATS PLAYER BLUE. You WIN!!!!" << endl;
        exit(0);
  }else if(col_player==Colour::RED && (closet_vertex.vector_index)/(_vertex_number*(_vertex_number-1)) != 0){
      cout<< "CONGRATS PLAYER RED. You WIN!!!!" << endl;
      exit(0);
    } 
    else return;  
}

//print out the winner
Colour verify_MCwinner(const Colour& col_player, const Vertex& closet_vertex){  
  if(col_player==Colour::BLUE && (closet_vertex.vector_index+1)%_vertex_number == 0)
    return Colour::BLUE;
  else if(col_player==Colour::RED && (closet_vertex.vector_index)/(_vertex_number*(_vertex_number-1)) != 0)
    return Colour::RED;
  else 
    return Colour::EMPTY;    
}

void clear_path_vector(){
 for(auto&& a : _vertex_reached) a=false;
  priority_queue<Vertex> dumped = move(open_set);

}

void create_shuffled_vertex(int no_value){
  vector<int> b = move(shuffled_vertex);
  for(const auto& a : available_vertex) if(a.first!=no_value) shuffled_vertex.push_back(a.first);
}

//private member
  int _vertex_number;
  int _move_counter;
  vector<Vertex> _hex;
  //shuffling variable
  vector<Vertex> shuffled_hex;
  unordered_map<int, int> available_vertex;
  vector<int> shuffled_vertex;
  //shortest path variable
  vector<bool> _vertex_reached;
  priority_queue<Vertex> open_set;
};



int main(void)
{   
  Colour player_colour;
  Colour computer_colour;
  Colour winner_colour = Colour::EMPTY;
  bool first_player =false;
  bool match_finished =false;
  
  //initialize board with 9 vertex for side
  Hex my_hex(11);
  
  cout <<my_hex;

  choose_player(player_colour, computer_colour, first_player);

//make moves
    while(!match_finished){
     if(first_player){
       my_hex.make_player_move(player_colour);
       my_hex.make_MC_move(computer_colour,player_colour);
     }else{
      my_hex.make_MC_move(computer_colour, player_colour);
      my_hex.make_player_move(player_colour);
    }  
   }
   return 0;
}