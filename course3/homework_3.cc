#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <vector>
#include <queue>

using namespace std; //useful to avoid writing every time std::vector and similar

//defined a struct for Vertex where the redifinition of < operator is useful for priority queue
typedef struct Vertex{
  int index =-1;
  double shortest_path =-1.;

  bool operator <(const Vertex& v2)const {
      if(shortest_path== v2.shortest_path) return index> v2.index;
      else
      return shortest_path > v2.shortest_path;
  }
}Vertex;

//defining graph class with all the useful methods
class Graph{

  public:
  //multi purpose constructor
  Graph(int vertex_number=0, double density=0., double distance_range=0.):
  _graph(nullptr), _vertex_number(vertex_number), _density(density), _distance_range(distance_range) {
      allocate_memory();
      Init();
      rdn_generator();
      shortest_path();
      _graph_counter++;
      _graph_number = _graph_counter;
  }
  //constructor able to read data from a file
  Graph(ifstream* file){
      string dim;
      string i,j,cost;
      _vertex_number = (*file >> dim, stoi(dim));
      cout << "vertex number is: " << _vertex_number <<endl;
      allocate_memory();
      Init();
      while(file->good()){
        *file>> i >> j >> cost;
        _graph[stoi(i)][stoi(j)] = stoi(cost);

      }
      file->close();
  }

  const double* const* get_matrixes() const {return _graph;}
  int get_vertex_number() const {return _vertex_number;}

  // MST algorithm
  void minimum_spanning_tree(){
    double MST =0.;
    _vertex_reached[0] = true;
    cout<< "vertex " << 0<< " PATH " << 0<< endl;
    for(int j=1; j<_vertex_number; j++){
      if (_graph[0][j] != 0.) open_set.push(Vertex{j,_graph[0][j]});
    }
    //cout<< "vertex 0 dimension _open_set: " << open_set.size()<< endl;
    while(open_set.size()!=0){ 
      Vertex closet_vertex = open_set.top();
      
      if(_vertex_reached[closet_vertex.index]){
        open_set.pop();
        //cout<< "vertex " <<closet_vertex.index << " already reached - dimension _open_set: " << open_set.size()<< endl;
        continue;
      }
      _vertex_reached[closet_vertex.index] = true;
      _closet_set[closet_vertex.index] = closet_vertex.shortest_path;
      cout<< "vertex " <<closet_vertex.index << " PATH " << closet_vertex.shortest_path<< endl;
      MST += closet_vertex.shortest_path;
      open_set.pop();
      for (int k = 0; k<_vertex_number; k++){
        if (!_vertex_reached[k] && _graph[closet_vertex.index][k] !=0.)
        open_set.push(Vertex{k,_graph[closet_vertex.index][k]});

      }
    }
    cout<<"minimum spanning tree is " <<MST<< endl;
  }

  void average_shortest_path(){
    double sum_shortest_path = 0.;
    int valid_vertex_counter =0;
    
    for(int i=0; i<_vertex_number; i++){
      sum_shortest_path+=_closet_set[i];
      if(_closet_set[i]!=0.) valid_vertex_counter++;
    } 
    
    cout << _graph_number <<") AVERAGE SHORTEST PATH = " << ((valid_vertex_counter!=0)? sum_shortest_path/valid_vertex_counter : 0.) ;
    cout << endl;
  }

  void print_shortest_path(){
    cout << "\n" << "***** "<< _graph_number << ") VERTEX SHORTEST PATH with Density = " << _density <<" ****" <<endl;
    for(int i=0; i<_vertex_number; i++){
      cout <<i<<": "<<_closet_set[i] << "\t";
      if(!((i+1)%12))cout << endl;
    } 
    cout << "\n" << endl;
  }
  
  // redifinition of cout accordingly to graph object 
  friend ostream& operator<<(ostream& out, const Graph* g){
  cout << "****** MATRIX VALUES *****" << endl;
    for(int i=0; i<g->_vertex_number; i++){
      for(int j=0; j<g->_vertex_number; j++){
        out <<g->_graph[i][j] << "\t";  
      }
      out << endl;
    }
    return out;
}

  private:
    void allocate_memory(){
      if(_vertex_number){
      _vertex_reached = new bool[_vertex_number];
      _closet_set = new double[_vertex_number];

      _graph= new double*[_vertex_number];
      for(int i = 0; i < _vertex_number; ++i){
        _graph[i]= new double[_vertex_number]; 
      }
     }
    }

  void Init(){
      for(int i = 0; i < _vertex_number; ++i)
    {
      _vertex_reached[i] = false;
      _closet_set[i] = 0.;
      for (int j=i; j<_vertex_number; ++j)
        _graph[i][j] = _graph[j][i]= 0.;
      }
  }

  inline double prob(){
  return static_cast<double>(rand()/RAND_MAX);}

  inline double weight(){
  return (prob()*_distance_range);}

  void rdn_generator(){
    srand(time(0));
    for(int i = 0; i < _vertex_number; ++i)
    {
      for (int j=i; j<_vertex_number; ++j){
        if(j==i) _graph[i][j] =0.;
        else _graph[i][j] = _graph[j][i]= (prob()< _density)*weight();
      }
    }
  }

  void shortest_path(){
    _vertex_reached[0] = true;
    for(int j=1; j<_vertex_number; j++){
      if (_graph[0][j] != 0.) open_set.push(Vertex{j,_graph[0][j]});
    }
    //cout<< "vertex 0 dimension _open_set: " << open_set.size()<< endl;
    while(open_set.size()!=0){ 
      Vertex closet_vertex = open_set.top();
      
      if(_vertex_reached[closet_vertex.index]){
        open_set.pop();
        //cout<< "vertex " <<closet_vertex.index << " already reached - dimension _open_set: " << open_set.size()<< endl;
        continue;
      }
      _vertex_reached[closet_vertex.index] = true;
      _closet_set[closet_vertex.index] = closet_vertex.shortest_path;
      open_set.pop();
      for (int k = 0; k<_vertex_number; k++){
        if (!_vertex_reached[k] && _graph[closet_vertex.index][k] !=0.)
        open_set.push(Vertex{k,closet_vertex.shortest_path+_graph[closet_vertex.index][k]});

      }
      //cout<<"vertex " <<closet_vertex.index << " dimension _open_set: " << open_set.size()<< endl;
    }
  }

  static int _graph_counter;
  int _graph_number=0;
  int _vertex_number;
  double _density;
  double _distance_range;
  double** _graph;
  bool* _vertex_reached;
  priority_queue<Vertex> open_set;
  double* _closet_set;
};


int Graph::_graph_counter =0;


int main(void)
{   
    ifstream file("Sample_Test_Data.txt", ifstream::in);
    Graph* graph1 = new Graph(&file);
    cout<<graph1;
   
    graph1->minimum_spanning_tree();
    
   return 0;
}