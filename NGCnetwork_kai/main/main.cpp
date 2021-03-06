#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>

#include "network.h"
#include "node.h"

using namespace std;

#define DBG
#ifdef DBG
#define DEBUG(statement) statement
#else
#define DEBUG(statement)
#endif

//const static int NODE_NUM = 390;

Network* readFileName(char * filename);

Bdd getMaj(Node* node, int i, Network* net);

//Network* makeNetwork(Bdd func);



int main(int argc, char **argv)
{
    if (argc != 2) {
      std::cerr << "Usage diffnet <file1> \n";
        exit(1);
    }
      
    int maxbdd_power = 20;
    int match_n = 0;
    int match_d = 0;
    Bdd::alloc(16, maxbdd_power);
    Network* circ1 = readFileName(argv[1]);

    //int in_format = ( MyString(argv[1]).contains(".pla") ) ? 0 : 1;
    bool is_pla = std::string(argv[1]).find("pla") != std::string::npos;
    bool in_format = ( is_pla ) ? false : true;
    if ( in_format == false ) {
      circ1->calAllSpecPla('0');
    }
    else{
       circ1->calAllSpecBlif('0');
    }
    


    circ1->setCSPF_All(); //CSPFを設定
    vector<Node*> nodelist = circ1->getnodelist();
    for(auto no: nodelist){
      if(no){
        int frg = 0;
        for(int i=0; i<32; ++i){
          Bdd tmp = getMaj(no, i, circ1);
          if(circ1->isIncludeFunc(tmp, no) && tmp!=Bdd::null){
            ++match_n;
            ++frg;
            assert(frg<2);
            break;
         }
        }
      }
    }

    for(auto no: nodelist){
      if(no){
        int frg = 0;
        for(int i=0; i<28; ++i){
          Bdd tmp = getMaj(no, i, circ1);
          Bdd nodefunc = circ1->getOutfuncidx(no);
          if(tmp == nodefunc && tmp!=Bdd::null){
            ++match_d;
            ++frg;
            assert(frg<2);
            break;
         }
        }
      }
    }



   
    cout << nodelist.size() << " : node num" << endl;
    cout << float(match_d) << "  " ;
    cout << float(match_d)/float(nodelist.size()) *100;
    cout << "% : for no cspf" << endl;
   
    cout << float(match_n) << "  " ;
    cout << float(match_n)/float(nodelist.size()) *100;
    cout << "% : cspf" << endl;
    cout << float(match_n)/ float(match_d) << " : increase" << endl;
    cout << "----------------------------------------" << endl;

  return 0;
}

  

Bdd getMaj(Node* node, int i, Network* net)
{
  vector<Node*>& v = node->getInput();
  if(!v.empty()){

    Bdd b1 = net->getOutfuncidx(v[0]);
    Bdd b2 = net->getOutfuncidx(v[1]);
    Bdd b3 = net->getOutfuncidx(v[2]);
    //if(v.size()==3){
    switch(i){
    case 1  : return b2  & b3;
    case 2  : return b2  & ~b3;
    case 3  : return ~b2 & b3;
    case 4  : return ~b2 & ~b3;
    case 5  : return b1  & b3;
    case 6  : return b1  & ~b3;
    case 7  : return b1  | b3;
    case 8  : return b1  | ~b3;
    case 9  : return b1  & b2;
    case 10 : return b1  | b2;
    case 11 : return (b1&b2)|(b1&b3)|(b2&b3);
    case 12 : return (b1&b2)|(b1&~b3)|(b2&~b3);
    case 13 : return b1  & ~b2;
    case 14 : return b1  | ~b2;
    case 15 : return (b1&~b2)|(~b2&b3)|(~b2&b3);
    case 16 : return (b1&~b2)|(b1&~b3)|(~b2&~b3) ;
    case 17 : return ~b1 & b3;
    case 18 : return ~b1 & ~b3;
    case 19 : return ~b1 | b3;
    case 20 : return ~b1 | ~b3;
    case 21 : return ~b1 & b2;
    case 22 : return ~b1 | b2;
    case 23 : return (~b1&b2)|(~b1&b3)|(b2&b3);
    case 24 : return (~b1&b2)|(~b1&~b3)|(b2&~b3);
    case 25 : return ~b1 & ~b2;
    case 26 : return ~b1 | ~b2;
    case 27 : return (~b1&~b2)|(~b1&b3)|(~b2&b3);
    case 28 : return b2  | b3;
    case 29 : return b2  | ~b3;
    case 30 : return ~b2 | b3;
    case 31 : return ~b2 | ~b3;
    case 0  : return (~b1&~b2)|(~b1&b3)|(~b2&~b3);
    default : cout << "error" << endl;
    }
   // }
  }
  //exit(1);
  return Bdd::null;
}   

Network * readFileName(char * filename)
{
    Network* net = new Network();
    net->readFileName(filename);
    return net;
}


  //todo: node::func -> network::outfunc

/*vector<Node*> test_node;   //1から
  test_node.push_back(0);
   cout << "save2" << endl;

  for(int i=0; i<3; ++i ){
      Node* tmp = new Node(INPUT);
      test_node.push_back(tmp);    
    }

    for(int i=0; i<2; ++i){
      Node* tmp = new Node(AND);
      test_node.push_back(tmp);
    }

    for(int i=0; i<1; ++i){
      Node* tmp = new Node(OUTPUT);
      test_node.push_back(tmp);
    }regPI
    cout << "print test_node num :" << test_node.size() <<  endl;

    test_node[4]->addInput(test_node[1]);
    cout << "save3" << endl;
    test_node[4]->addInput(test_node[2]);
    test_node[5]->addInput(test_node[2]);
    test_node[5]->addInput(test_node[3]);
    test_node[6]->addInput(test_node[4]);
    test_node[6]->addInput(test_node[5]);
      cout << test_node[4]->nIn() << endl; 
  int f=0;
  for(auto i: test_node){
    if(i){
      string str = "node" ;
      string num = to_string(f);  
      str = str + num;
      char word[5];
      str.copy(word, str.size());
      cout << "node type is" << i->getType() << endl;
      circ2->newGateName(word, i->getType());
      ++f;
    }
  }
  cout << "outfuncs size is " << circ2->getOutfunc().size() <<endl;
*/
